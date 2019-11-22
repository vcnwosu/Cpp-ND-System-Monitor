#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <vector>

#include "system.h"
#include "linux_parser.h"
#include "format.h"
#include "test_assert.h"

static void procProcesses(System &system) {
    test("LinuxParser correctly parses process /proc data...");

    int pid = 0;

    for (auto &entry: std::experimental::filesystem::directory_iterator(LinuxParser::kProcDirectory)) {
        if (std::experimental::filesystem::is_directory(entry.path())) {
            try {
                pid = std::stoi(entry.path().filename());
                break;
            } catch (const std::invalid_argument &ia) {}
        }
    }

    assertGreaterThan(LinuxParser::ActiveJiffies(pid), 0);
    assertNotEqual(LinuxParser::Command(pid), "");
    assertNotEqual(LinuxParser::Ram(pid), "0.00%");
    assertEqual(LinuxParser::Uid(pid), "0");
}

static void cpuUtilization(System &system) {
    test("LinuxParser correctly parses CPU utilization...");

    std::vector<std::string> utilization = LinuxParser::CpuUtilization();

    assertEqual(utilization.size(), 10);
    assertNotEqual(utilization[0], "0%"); // user space
    assertNotEqual(utilization[3], "0%"); // idle
}

static void jiffies(System &system) {
    test("LinuxParser correctly parses jiffies...");

    assertGreaterThan(LinuxParser::Jiffies(), 0);

    int pid = 0;
    std::string path = LinuxParser::kProcDirectory;

    for (auto &entry: std::experimental::filesystem::directory_iterator(path)) {
        if (std::experimental::filesystem::is_directory(entry.path())) {
            try {
                pid = std::stoi(entry.path().filename());
                break;
            } catch (const std::invalid_argument &ia) { }
        }
    }

    assertGreaterThan(LinuxParser::ActiveJiffies(pid), 0);
    assertGreaterThan(LinuxParser::ActiveJiffies(), 0);
    assertGreaterThan(LinuxParser::IdleJiffies(), 0);
}

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
    jiffies(system);
    cpuUtilization(system);
    procProcesses(system);
}
