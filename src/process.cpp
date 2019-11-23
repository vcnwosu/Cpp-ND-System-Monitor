#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: [DONE] Instantiate the process
Process::Process(int pid): pid_{pid} {
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
}

// TODO: [DONE] Return this process's ID
int Process::Pid() {
    return pid_;
}

// TODO: [DONE] Return this process's CPU utilization
float Process::CpuUtilization() const {
    return ((float) LinuxParser::ActiveJiffies(pid_) / 100 / UpTime());
}

// TODO: [DONE] Return the command that generated this process
string Process::Command() { return command_; }

// TODO: [DONE] Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// TODO: [DONE] Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: [DONE] Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// TODO: [DONE] Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return CpuUtilization() > a.CpuUtilization();
}
