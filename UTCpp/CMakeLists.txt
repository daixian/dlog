cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)

add_executable(utcpp.out pch.h pch.cpp DebugTest.cpp dlogTest.cpp MemoryLogTest.cpp)
# Dependencies
find_package(GTest MODULE REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(utcpp.out PRIVATE dlog GTest::GTest GTest::Main)

add_test(NAME TestsInUTCpp COMMAND utcpp.out)
