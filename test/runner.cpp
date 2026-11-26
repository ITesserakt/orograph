#include <iostream>
#include <ostream>
#include <chrono>

#include "launcher/launcher.h"

class TestRunner
{
    unsigned int successes = 0;
    unsigned int total_tests = 0;
    const std::vector<TestCase>& test_cases = registry();

    struct IncreaseTotal
    {
        unsigned int& total;

        explicit IncreaseTotal(unsigned int& total) : total(total)
        {
        }

        ~IncreaseTotal() { total++; }
    };

    template <typename T>
    static void report_failure(const TestCase& test_case, const T& message)
    {
        std::cerr << "Test " << test_case.name << " at " << test_case.file.native() << ":" << test_case.line <<
            " failed: "
            << message << std::endl;
    }

public:
    void run_test(const TestCase& test_case)
    {
        using namespace std::chrono;

        const IncreaseTotal _{this->total_tests};
        std::cerr << "Running test " << test_case.name;

        if (test_case.extras.ignored)
        {
            std::cerr << "... skipped" << std::endl;
            return;
        }

        const auto now = high_resolution_clock::now();
        try
        {
            test_case.run();
            const auto elapsed = high_resolution_clock::now();
            std::cerr << "... OK [" << duration_cast<milliseconds>(elapsed - now) << "]" << std::endl;
        }
        catch (const char* ex)
        {
            if (!test_case.extras.should_panic) report_failure(test_case, ex);
            else std::cerr << "... OK [panicked]" << std::endl;
        }
        catch (const std::exception& ex)
        {
            if (!test_case.extras.should_panic) report_failure(test_case, ex.what());
            else std::cerr << "... OK [panicked]" << std::endl;
        }
        this->successes++;
    }

    void run_all()
    {
        for (const auto& test : this->test_cases)
        {
            run_test(test);
        }
    }

    [[nodiscard]] const TestCase* get_test_by_name(const std::string_view& name, const bool exact = false) const
    {
        for (const auto& test : this->test_cases)
        {
            if (exact && test.name == name || !exact && test.name.find(name) != std::string::npos)
            {
                return &test;
            }
        }
        return nullptr;
    }

    [[nodiscard]] unsigned int total_runs() const { return this->total_tests; }
    [[nodiscard]] unsigned int successive_runs() const { return this->successes; }

    friend std::ostream& operator<<(std::ostream& os, const TestRunner& obj)
    {
        for (const auto& test : obj.test_cases)
        {
            os << " - " << test.name;

            std::vector<std::string_view> properties {};
            if (test.extras.ignored) properties.emplace_back("ignored");
            if (test.extras.should_panic) properties.emplace_back("should panic");
            if (!properties.empty())
            {
                bool first = true;
                for (const auto& property: properties)
                {
                    if (first)
                    {
                        first = false;
                        os << " [" << property;
                    } else
                    {
                        os << ", " << property;
                    }
                }
                os << "]";
            }

            os << std::endl << "    at " << test.file.native() << ":" << test.line << std::endl;
        }
        return os;
    }
};

int main(const int argc, char* argv[])
{
    TestRunner runner;
    if (argc < 2)
    {
        std::cout << "Available tests: " << std::endl << runner;
        return 0;
    }

    const std::string_view arg{argv[1]};
    bool exact = false;
    if (argc > 2)
    {
        exact = std::string_view{argv[2]} == "--exact";
    }

    if (arg == "--all")
    {
        runner.run_all();
    }
    else if (const auto test = runner.get_test_by_name(arg, exact))
    {
        runner.run_test(*test);
    }
    else
    {
        std::cerr << "Test not found: " << arg << std::endl;
    }

    std::cerr << "Finished with "
        << runner.successive_runs() << " / " << runner.total_runs() << " success"
        << std::endl;
}
