#ifndef MONITOR_TEST_ASSERT_H
#define MONITOR_TEST_ASSERT_H

#include <iostream>
#include <string>
#include <cassert>

template <typename T, typename U> void assertGreaterThan(T val1, U val2);
template <typename T, typename U> void assertLessThan(T val1, U val2);
template <typename T, typename U> void assertEqual(T val1, U val2);
void assertTrue(bool expression);
void test(std::string title);

void test(std::string title) {
    std::cout << std::endl << "[TEST] " << title << std::endl << std::endl;
}

template <typename T, typename U> void assertEqual(T val1, U val2) {
    std::cout << "Assert Equal: " << val1 << " " << val2 << std::endl;
    assert(val1 == val2);
}

void assertTrue(bool expression) {
    std::cout << "Assert True: " << expression << std::endl;
    assert(expression);
}

template <typename T, typename U> void assertGreaterThan(T val1, U val2) {
    std::cout << "Assert Greater Than: " << val1 << " " << val2 << std::endl;
    assert(val1 > val2);
}

template <typename T, typename U> void assertLessThan(T val1, U val2) {
    std::cout << "Assert Less Than: " << val1 << " " << val2 << std::endl;
    assert(val1 < val2);
}

#endif
