#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

// TODO: [DONE] Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> utilization = LinuxParser::CpuUtilization();
    std::string active = utilization[0];

    std::replace(active.begin(), active.end(), '%', ' ');

    return std::stof(active) / 100;
}
