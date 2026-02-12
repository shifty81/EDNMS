#include "test_framework.h"

int main() {
    std::cout << "EDNMS Test Suite" << std::endl;
    std::cout << "================" << std::endl;
    return test::RunAllTests();
}
