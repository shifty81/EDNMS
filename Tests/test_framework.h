// Minimal test framework (no external dependencies)
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cmath>

namespace test {

struct TestCase {
    std::string name;
    std::function<bool()> func;
};

inline std::vector<TestCase>& GetTests() {
    static std::vector<TestCase> tests;
    return tests;
}

inline int RegisterTest(const std::string& name, std::function<bool()> func) {
    GetTests().push_back({name, func});
    return 0;
}

inline int RunAllTests() {
    int passed = 0;
    int failed = 0;
    for (const auto& tc : GetTests()) {
        bool ok = false;
        try {
            ok = tc.func();
        } catch (const std::exception& e) {
            std::cerr << "  EXCEPTION: " << e.what() << std::endl;
        }
        if (ok) {
            std::cout << "  PASS: " << tc.name << std::endl;
            ++passed;
        } else {
            std::cerr << "  FAIL: " << tc.name << std::endl;
            ++failed;
        }
    }
    std::cout << std::endl << passed << " passed, " << failed << " failed, "
              << (passed + failed) << " total" << std::endl;
    return failed > 0 ? 1 : 0;
}

#define TEST(suite, name) \
    static bool suite##_##name(); \
    static int suite##_##name##_reg = test::RegisterTest(#suite "." #name, suite##_##name); \
    static bool suite##_##name()

#define EXPECT_TRUE(expr) do { if (!(expr)) { std::cerr << "    Expected true: " #expr << std::endl; return false; } } while(0)
#define EXPECT_FALSE(expr) do { if ((expr)) { std::cerr << "    Expected false: " #expr << std::endl; return false; } } while(0)
#define EXPECT_EQ(a, b) do { if ((a) != (b)) { std::cerr << "    Expected equal: " #a " == " #b << std::endl; return false; } } while(0)
#define EXPECT_NE(a, b) do { if ((a) == (b)) { std::cerr << "    Expected not equal: " #a " != " #b << std::endl; return false; } } while(0)
#define EXPECT_NEAR(a, b, eps) do { if (std::abs((a) - (b)) > (eps)) { std::cerr << "    Expected near: " #a " ~= " #b << std::endl; return false; } } while(0)
#define EXPECT_GT(a, b) do { if (!((a) > (b))) { std::cerr << "    Expected greater: " #a " > " #b << std::endl; return false; } } while(0)

} // namespace test
