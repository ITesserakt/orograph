#include <chrono>
#include <iostream>
#include <ostream>

#include "launcher/launcher.h"

class TestRunner {
    unsigned int successes = 0;
    unsigned int total_tests = 0;
    const std::vector<TestCase>& test_cases;

    struct IncreaseTotal {
        unsigned int& total;

        explicit IncreaseTotal(unsigned int& total) : total(total) {}

        ~IncreaseTotal() { total++; }
    };

    template <typename T> static void report_failure(const TestCase& test_case, const T& message) {
        if (!test_case.extras.is_should_panic()) {
            std::cerr << "... FAIL [" << message << "]" << std::endl;
            return;
        }

        const auto expected_message = std::get_if<std::string_view>(&test_case.extras.should_panic);
        if (!expected_message) {
            std::cerr << "... OK [panicked]" << std::endl;
            return;
        }

        if (*expected_message != std::string_view{message}) {
            std::cerr << "... FAIL [" << message << "], but expected different panic message: " << *expected_message
                      << std::endl;
            return;
        }
        std::cerr << "... OK [panicked]" << std::endl;
    }

  public:
    bool fail_fast = false;

    enum class Outcome { Succeeded, Skipped, Failed };

    explicit TestRunner(const bool fail_fast = false, const std::vector<TestCase>& test_cases = registry())
        : test_cases(test_cases), fail_fast(fail_fast) {}

    Outcome run_test(const TestCase& test_case) {
        using namespace std::chrono;

        const IncreaseTotal _{this->total_tests};
        std::cerr << "Running test " << test_case.name;

        if (std::holds_alternative<bool>(test_case.extras.ignored) && std::get<bool>(test_case.extras.ignored)) {
            std::cerr << "... SKIP" << std::endl;
            return Outcome::Skipped;
        }
        if (std::holds_alternative<std::string_view>(test_case.extras.ignored)) {
            std::cerr << "... SKIP (" << std::get<std::string_view>(test_case.extras.ignored) << ")" << std::endl;
            return Outcome::Skipped;
        }

        const auto now = high_resolution_clock::now();
        try {
            test_case.run();
            const auto elapsed = high_resolution_clock::now();
            std::cerr << "... OK [" << duration_cast<milliseconds>(elapsed - now) << "]" << std::endl;
        } catch (const char* ex) {
            report_failure(test_case, ex);
            return Outcome::Failed;
        } catch (const std::exception& ex) {
            report_failure(test_case, ex.what());
            return Outcome::Failed;
        }
        this->successes++;
        return Outcome::Succeeded;
    }

    void run_all() {
        for (const auto& test : this->test_cases) {
            const auto outcome = run_test(test);
            if (this->fail_fast && outcome == Outcome::Failed)
                return;
        }
    }

    [[nodiscard]] const TestCase* get_test_by_name(const std::string_view& name, const bool exact = false) const {
        for (const auto& test : this->test_cases) {
            if (exact && test.name == name || !exact && test.name.find(name) != std::string::npos) {
                return &test;
            }
        }
        return nullptr;
    }

    [[nodiscard]] unsigned int total_runs() const { return this->total_tests; }
    [[nodiscard]] unsigned int successive_runs() const { return this->successes; }

    friend std::ostream& operator<<(std::ostream& os, const TestRunner& obj) {
        for (const auto& test : obj.test_cases) {
            os << " - " << test.name;

            std::vector<std::string_view> properties{};
            if (test.extras.is_ignored())
                properties.emplace_back("ignored");
            if (test.extras.is_should_panic())
                properties.emplace_back("should panic");
            if (!properties.empty()) {
                bool first = true;
                for (const auto& property : properties) {
                    if (first) {
                        first = false;
                        os << " [" << property;
                    } else {
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

static bool flag_passed(const int argc, char** argv, const std::string_view flag) {
    for (int i = 0; i < argc; i++)
        if (flag == argv[i])
            return true;
    return false;
}

int main(const int argc, char* argv[]) {
    TestRunner runner;
    if (argc < 2) {
        std::cout << "Available tests: " << std::endl << runner;
        return 0;
    }

    const std::string_view arg{argv[1]};
    const bool exact = flag_passed(argc - 2, argv + 2, "--exact");
    runner.fail_fast = flag_passed(argc - 2, argv + 2, "--fail-fast");

    if (arg == "--all") {
        runner.run_all();
    } else if (const auto test = runner.get_test_by_name(arg, exact)) {
        runner.run_test(*test);
    } else {
        std::cerr << "Test not found: " << arg << std::endl;
    }

    std::cerr << "Finished with " << runner.successive_runs() << " / " << runner.total_runs() << " success"
              << std::endl;
}
