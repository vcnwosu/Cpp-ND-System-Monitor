#include <iostream>
#include <cassert>

#include "system.h"
#include "linux_parser.h"
#include "format.h"

static void memoryUtilization(System &system) {
    std::cout << "LinuxParser correctly parses memory utilization... ";

    const float memory = LinuxParser::MemoryUtilization();

    assert(memory > 0.0 && memory <= 100.0);
    assert(system.MemoryUtilization() == memory);

    std::cout << "PASS." << std::endl;
}

static void uptime(System &system) {
    std::cout << "LinuxParser correctly parses uptime... ";

    const long uptime = LinuxParser::UpTime();

    assert(uptime > 0);
    assert(system.UpTime() == uptime);

    std::cout << "PASS." << std::endl;

    std::cout << "Format helper function parses time correctly into HH:SS:MM... ";

    assert(Format::ElapsedTime(0) == "00:00:00");
    assert(Format::ElapsedTime(5) == "00:00:05");
    assert(Format::ElapsedTime(3600) == "01:00:00");
    assert(Format::ElapsedTime(1800) == "00:30:00");
    assert(Format::ElapsedTime(127) == "00:02:07");

    std::cout << "PASS." << std::endl;
}

void RunLinuxParserTest(System &system) {
    memoryUtilization(system);
    uptime(system);
}
