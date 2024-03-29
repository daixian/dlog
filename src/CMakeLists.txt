﻿# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
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

# set(CMAKE_CXX_STANDARD 14)

# 在有了project之后需要第一时间搞个conan设置吧
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
# conan_set_vs_runtime()

# 设置是否编译成共享库,如果没有人配置过那么就配置一下
if(NOT DEFINED DLOG_BUILD_SHARED)
    message(STATUS "NOT DEFINED DLOG_BUILD_SHARED!")
    option(DLOG_BUILD_SHARED "DLOG BUILD SHARED" OFF)
endif()

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
message(STATUS "CMAKE_C_FLAGS_DEBUG = ${CMAKE_C_FLAGS_DEBUG}" )
message(STATUS "CMAKE_CXX_FLAGS_DEBUG = ${CMAKE_CXX_FLAGS_DEBUG}" )
message(STATUS "CMAKE_C_FLAGS_RELEASE = ${CMAKE_C_FLAGS_RELEASE}" )
message(STATUS "CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}" )

# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -s" )
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s" )
# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s" )
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s" )

message(STATUS "DLOG_BUILD_SHARED = ${DLOG_BUILD_SHARED}")

if(WIN32)
# # 设置编译选项为MT https://stackoverflow.com/questions/14172856/compile-with-mt-instead-of-md-using-cmake
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP")
    # 控制一下调试时候Debug的VC编译方式
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP")
endif()

# 设置生成一个VS工程里的文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions(-D DLOG_UTF8)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(dlog)

