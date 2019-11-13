#include <string>
#include <sstream>

#include "format.h"

using std::string;
using std::stringstream;

static std::string format(long number) {
    stringstream ss;

    if (number < 10) {
        ss << "0" << number;

        return ss.str();
    }

    ss << number;

    return ss.str();
}

// TODO: [DONE] Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = seconds / 3600;
    long remainder = seconds % 3600;
    long minutes = remainder / 60;
    remainder = remainder % 60;

    stringstream ss;

    ss << format(hours) << ":" << format(minutes) << ":" << format(remainder);

    return ss.str();
}
