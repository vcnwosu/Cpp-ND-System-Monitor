#include <iostream>

#include "process.h"
#include "system.h"
#include "linux_parser_test.h"

int main(void) {
    std::cout << "Instantiating System object... ";

    System system;

    std::cout << "Done." << std::endl << std::endl;

    RunLinuxParserTest(system);
}
