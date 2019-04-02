# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
#
cmake_minimum_required (VERSION 3.0)

option(ENABLE_TEST "Enable test" OFF)

#这种写法在windows下貌似无效
 
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "D:/SDK/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "") 
endif()
 
if(WIN32 AND NOT DEFINED DVCPKG_TARGET_TRIPLET)
  set(DVCPKG_TARGET_TRIPLET "x64-windows-static-md"
      CACHE STRING "") 
endif()

#add_definitions(-DBOOST_ALL_NO_LIB)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)

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
