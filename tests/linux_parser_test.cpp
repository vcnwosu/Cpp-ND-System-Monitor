#include <iostream>

#include "system.h"
#include "linux_parser.h"
#include "format.h"
#include "test_assert.h"

static void memoryUtilization(System &system) {
    test("LinuxParser correctly parses memory utilization... ");

    const float memory = LinuxParser::MemoryUtilization();

    assertTrue(memory > 0.0 && memory <= 100.0);
    assertEqual(system.MemoryUtilization(), memory);
}

static void uptime(System &system) {
    test("LinuxParser correctly parses uptime... ");

    const long uptime = LinuxParser::UpTime();

    assertGreaterThan(uptime, 0);
    assertEqual(system.UpTime(), uptime);

    test("Format helper function parses time correctly into HH:SS:MM... ");

    assertEqual(Format::ElapsedTime(0), "00:00:00");
    assertEqual(Format::ElapsedTime(5), "00:00:05");
    assertEqual(Format::ElapsedTime(3600), "01:00:00");
    assertEqual(Format::ElapsedTime(1800), "00:30:00");
    assertEqual(Format::ElapsedTime(127), "00:02:07");
}

void RunLinuxParserTest(System &system) {
    std::cout << "Testing the LinuxParser namespace..." << std::endl;

    memoryUtilization(system);
    uptime(system);
}
