cmake_minimum_required(VERSION 2.6)
project(test)

include_directories(../include)

file(GLOB TEST_SOURCES "*.cpp")

set(SOURCES
    ${TEST_SOURCES}
    ../src/linux_parser.cpp
    ../src/system.cpp
    ../src/format.cpp
)

add_executable(test ${SOURCES})
