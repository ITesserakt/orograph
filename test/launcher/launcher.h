#ifndef OROGRAPH_LAUNCHER_H
#define OROGRAPH_LAUNCHER_H

#include <string>
#include <utility>
#include <vector>
#include <filesystem>

struct TestCaseExtras
{
    bool ignored = false;
    bool should_panic = false;

    [[nodiscard]] bool is_default() const { return !ignored && !should_panic; }
};

struct TestCase
{
    TestCaseExtras extras;
    std::string name;
    std::filesystem::path file;
    unsigned int line;

    void (*run)();

    TestCase(std::string name, std::filesystem::path file, const unsigned int line, void (*run)(), const TestCaseExtras& extras)
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
