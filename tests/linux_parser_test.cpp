#include <iostream>
#include <cassert>

#include "system.h"
#include "linux_parser.h"

void RunLinuxParserTest(System &system) {

    std::cout << "LinuxParser correctly parses memory utilization... ";

    const float memory = LinuxParser::MemoryUtilization();

    assert(typeid(memory) == typeid(float));
    assert(memory > 0.0 && memory <= 100.0);
    assert(system.MemoryUtilization() == memory);

    std::cout << "PASS." << std::endl;
}
