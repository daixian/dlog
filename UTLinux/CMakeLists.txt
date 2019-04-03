cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)


# Dependencies
find_package(GTest MODULE REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

 
add_executable(utlinux main.cpp)
#target_link_libraries(utcpp PRIVATE GTest::GTest GTest::Main)

#add_test(AllTestsInUtcpp utcpp)
