# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
#
cmake_minimum_required (VERSION 3.0)

#设置一个工具链,如果进入设置了那么通常是能够正确工作,如果有被其他设置所覆盖那么可能无法工作
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "/home/dx/vcpkg/scripts/buildsystems/vcpkg.cmake" 
      CACHE STRING "") 
      message(STATUS "not defined CMAKE_TOOLCHAIN_FILE !") 
endif()

message(STATUS "vcpkg.cmake = " ${CMAKE_TOOLCHAIN_FILE})

if(NOT DEFINED DVCPKG_TARGET_TRIPLET)
  set(DVCPKG_TARGET_TRIPLET "x64-linux"
      CACHE STRING "") 
endif()

#在上面设置工具链之后下面要立马project命令
project(dlog
        VERSION 2.1.2
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

option(ENABLE_TEST "Enable test" OFF)

enable_testing()

#add_definitions(-DBOOST_ALL_NO_LIB)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREADED ON )
set( Boost_USE_STATIC_RUNTIME OFF )

add_definitions(-DBOOST_ALL_NO_LIB)

set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

set(Boost_DEBUG 0)

add_subdirectory(dlog/)
add_subdirectory(UTLinux/)
