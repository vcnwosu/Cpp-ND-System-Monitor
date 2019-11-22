cmake_minimum_required(VERSION 2.6)
project(test)

include_directories(../include)

file(GLOB TEST_SOURCES "*.cpp")

set(SOURCES
    ${TEST_SOURCES}
    ../src/linux_parser.cpp
    ../src/process.cpp
    ../src/system.cpp
    ../src/format.cpp
)

add_executable(test ${SOURCES})

set_property(TARGET test PROPERTY CXX_STANDARD 17)

target_link_libraries(test stdc++fs)
