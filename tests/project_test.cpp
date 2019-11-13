#include <iostream>
#include <cassert>
#include <typeinfo>

#include "system.h"
#include "linux_parser_test.h"

int main(void) {
    std::cout << "Instantiating System object... ";

    System system;

    std::cout << "Done." << std::endl;

    std::cout << "Testing LinuxParser... " << std::endl;
    RunLinuxParserTest(system);
}
