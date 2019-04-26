# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
#
cmake_minimum_required(VERSION 3.0)

# 设置一个工具链,如果进入设置了那么通常是能够正确工作,如果有被其他设置所覆盖那么可能无法工作
# if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#   set(CMAKE_TOOLCHAIN_FILE "/home/dx/vcpkg/scripts/buildsystems/vcpkg.cmake"
#       CACHE STRING "")
#   message(STATUS "not defined CMAKE_TOOLCHAIN_FILE !")
# endif()

# message(STATUS "vcpkg.cmake = " ${CMAKE_TOOLCHAIN_FILE})

# if(NOT DEFINED DVCPKG_TARGET_TRIPLET)
#   set(DVCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")
# endif()

# 在上面设置工具链之后下面要立马project命令
project(dlog VERSION 2.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

option(ENABLE_TEST "Enable test" OFF)

option(CROSS_COMPILE "Cross Compile" ON)

enable_testing()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置工程外部库的文件夹
set(PROJECT_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
message(STATUS "PROJECT_LIB_DIR = " ${PROJECT_LIB_DIR})

# 设置boost的库的位置
if(WIN32)
message(STATUS "Platform = WIN32")
set(BOOST_ROOT ${PROJECT_LIB_DIR}/boost_1_70_0)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK OFF)
else()
message(STATUS "Platform = Other")
set(BOOST_ROOT ${PROJECT_LIB_DIR}/boost_1_70_0)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK OFF)
endif()

# 使用编好的boost库
if(CROSS_COMPILE) 
set(BOOST_ROOT $ENV{HOME}/sdk/boost/boost_1_70_0_arm64)
endif()


add_definitions(-DBOOST_ALL_NO_LIB)

set(Boost_DEBUG 0)

# 使能测试
enable_testing()

add_subdirectory(dlog)

# 这里要添加gtest，这样就会去编译gtest然后设置了一些变量
# 添加gtest前尝试设置一下编译选项为MD
#if(WIN32)
#set(CMAKE_CXX_FLAGS_RELEASE "/MD")
#set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
#endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/gtest)

include(CTest)
#include(GoogleTest)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)


