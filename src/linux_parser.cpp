#include <dirent.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>
#include <experimental/filesystem>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stod;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::experimental::filesystem;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;

    for (auto &entry: fs::directory_iterator(kProcDirectory)) {
        if (fs::is_directory(entry.path())) {
            try {
                int pid = std::stoi(entry.path().filename());
                pids.push_back(pid);
            } catch (const std::invalid_argument &ia) {}
        }
    }

  return pids;
}

float LinuxParser::memTotal() {
    static float total = 0.0;

    if (!total) {
        string line, label, value;
        std::ifstream file(kProcDirectory + kMeminfoFilename);
        std::getline(file, line);
        std::istringstream is(line);

        is >> label >> value;

        total = std::stof(value);
    }

    return total;
}

// TODO: [DONE] Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    float memAvailable = 0.0;

    string line, label, value;

    std::ifstream file(kProcDirectory + kMeminfoFilename);

    if (file.is_open()) {
        while(std::getline(file, line)) {
            std::istringstream is(line);

            is >> label >> value;

            if (label == "MemAvailable:") {
                memAvailable = stof(value) / LinuxParser::memTotal();
                break;
            }
        }
    }

    return memAvailable;
}

// TODO: [DONE] Read and return the system uptime
long LinuxParser::UpTime() {
    string line, seconds;

    std::ifstream file(kProcDirectory + kUptimeFilename);

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream is(line);

        is >> seconds;

        return stol(seconds);
    }

    return 0;
}

// TODO: [DONE] Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
    string line, column;
    std::ifstream file(kProcDirectory + kStatFilename);

    long jiffies = 0;

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream is(line);

        while (is >> column) {
            if (column == "cpu") {
                continue;
            }

            jiffies += stol(column);
        }
    }

    return jiffies;
}

// TODO: [DONE] Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    string line, value;
    std::ifstream file(kProcDirectory + std::to_string(pid) + "/stat");

    std::getline(file, line);
    std::istringstream is(line);

    int count = 0;
    long jiffies = 0;

    while (is >> value) {
        count++;

        if (count == 14 || count == 15) {
            jiffies += stol(value);

            if (count == 15) {
                break;
            }
        }
    }

    return jiffies;
}

// TODO: [DONE] Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    /*for (auto &entry: fs::directory_iterator(kProcDirectory)) {
        if (fs::is_directory(entry.path())) {
            try {
                active_jiffies += LinuxParser::ActiveJiffies(std::stoi(entry.path().filename()));
            } catch (const std::invalid_argument &ia) {}
        }
    }*/

    string line, cpu, jiffies;
    std::ifstream file(kProcDirectory + kStatFilename);

    long active_jiffies = 0;

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream is(line);

        is >> cpu >> jiffies;
        active_jiffies = std::stol(jiffies);
    }

    return active_jiffies;
}

// TODO: [DONE] Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    string line, column;
    std::ifstream file(kProcDirectory + kStatFilename);

    int count = 0;
    long jiffies = 0;

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream is(line);

        while (is >> column && count++ < 4) { continue; }

        jiffies = stol(column);
    }

    return jiffies;
}

// TODO: [DONE] Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    vector<long> values;
    vector<string> utilization;

    string line, column;
    std::ifstream file1(kProcDirectory + kStatFilename);
    std::getline(file1, line);
    std::istringstream is1(line);

    while (is1 >> column) {
        if (column == "cpu") {
            continue;
        }

        values.push_back(std::stol(column));
    }

    file1.close();

    sleep(3);

    std::ifstream file2(kProcDirectory + kStatFilename);
    std::getline(file2, line);
    std::istringstream is2(line);

    long total = 0;
    int index = 0;

    while (is2 >> column) {
        if (column == "cpu") {
            continue;
        }

        values[index] = std::stol(column) - values[index];
        total += values[index++];
    }

    for (long v: values) {
        std::ostringstream os;
        os.precision(2);
        os << std::fixed << ((float) v * 100) / total;
        utilization.push_back(os.str() + "%");
    }

    return utilization;
}

// TODO: [DONE] Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    std::ifstream file(kProcDirectory + kStatFilename);
    std::string line, value;

    int processes = 0;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream is(line);

            is >> value;

            if (value == "processes") {
                is >> value;
                processes = stoi(value);
                break;
            }
        }
    }

    return processes;
}

// TODO: [DONE] Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, pid, name, status;
    int count = 0;

    for (auto &entry: fs::directory_iterator(kProcDirectory)) {
        if (fs::exists(entry.path() / "stat")) {
            std::ifstream file(entry.path() / "stat");
            std::getline(file, line);
            std::istringstream is(line);

            is >> pid >> name >> status;

            if (status == "R") {
                count++;
            }
        }
    }

    return count;
}

// TODO: [DONE] Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string line;
    std::ifstream file(kProcDirectory + std::to_string(pid) + "/cmdline");
    std::getline(file, line);

    return line;
}

// TODO: [DONE] Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string line, value;
    std::ifstream file(kProcDirectory + std::to_string(pid) + "/statm");
    std::getline(file, line);
    std::istringstream is(line);

    long memory = 0;

    while (is >> value) { memory += std::stoi(value); }

    std::ostringstream out;

    // 4096 for the default page size to bytes
    // 1000000 to convert Bytes to MB
    out << ((memory * 4096) / 1000000);

    return out.str();

}

// TODO: [DONE] Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line, label, value;
    std::ifstream file(kProcDirectory + std::to_string(pid) + "/status");

    while (std::getline(file, line)) {
        std::istringstream is(line);

        is >> label >> value;

        if (label == "Uid:") {
            break;
        }
    }

    return value;
}

// TODO: [DONE] Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string line, user, x, id;
    std::ifstream file(kPasswordPath);

    string uid = LinuxParser::Uid(pid);

    while (std::getline(file, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream is(line);
        is >> user >> x >> id;

        if (id == uid) {
            break;
        }
    }

    return user;
}

// TODO: [DONE] Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line, value;
    std::ifstream file(kProcDirectory + std::to_string(pid) + "/stat");
    std::getline(file, line);
    std::istringstream is(line);

    int count = 1;
    long start_time = 0;

    while (is >> value) {
        if (count++ == 22) {
            start_time = std::stol(value);
            break;
        }
    }

    return LinuxParser::UpTime() - (start_time / 100); // standard Hertz approximation

}
