#ifndef OROGRAPH_LAUNCHER_H
#define OROGRAPH_LAUNCHER_H

#include <string>
#include <vector>
#include <filesystem>
#include <variant>

struct TestCaseExtras
{
    std::variant<bool, std::string_view> ignored = false;
    std::variant<bool, std::string_view> should_panic = false;

    [[nodiscard]] bool is_default() const
    {
        return !this->is_ignored() && !this->is_should_panic();
    }

    [[nodiscard]] bool is_ignored() const
    {
        return std::holds_alternative<std::string_view>(this->ignored) || std::get<bool>(this->ignored);
    }

    [[nodiscard]] bool is_should_panic() const
    {
        return std::holds_alternative<std::string_view>(this->should_panic) || std::get<bool>(this->should_panic);
    }
};

struct TestCase
{
    TestCaseExtras extras;
    std::string name;
    std::filesystem::path file;
    unsigned long line;

    void (*run)();

    TestCase(std::string name, std::filesystem::path file, const unsigned long line, void (*run)(),
             const TestCaseExtras& extras)
        : extras(extras),
          name(std::move(name)),
          file(std::move(file)),
          line(line),
          run(run)
    {
    }
};

inline std::vector<TestCase>& registry()
{
    static std::vector<TestCase> test_cases;
    return test_cases;
}

#define TEST(name, ...) void test_ ## name ## _(); \
    struct Add_ ## name ## _Static { \
        Add_ ## name ## _Static() { \
            registry().emplace_back(#name, __FILE__, __LINE__, test_ ## name ## _, TestCaseExtras {__VA_ARGS__}); \
        } \
    }; \
    static Add_ ## name ## _Static add_ ## name ## _static; \
    void test_ ## name ## _()

#endif // OROGRAPH_LAUNCHER_H
