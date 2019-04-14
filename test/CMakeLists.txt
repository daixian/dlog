cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)

add_executable(test.out pch.h pch.cpp DebugTest.cpp dlogTest.cpp MemoryLogTest.cpp)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# CMake 2.8.11 之后gtest是自动添加的，所以下面的没有执行
if (CMAKE_VERSION  VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

# 设置dlog库的目录
if(NOT DEFINED dlog_LIBRARIES)
  if(WIN32)
    set(dlog_LIBRARIES ${CMAKE_SOURCE_DIR}/cryptopp/x64/Output/Release/cryptlib.lib)
  else()
    set(dlog_LIBRARIES ${LIBRARY_OUTPUT_PATH}/libdlog.so)
  endif()
endif()
# 输出dlog库的目录看看
message(STATUS "dlog_INCLUDE_DIRS = " ${CMAKE_SOURCE_DIR})
message(STATUS "dlog_LIBRARIES = " ${dlog_LIBRARIES})

# 设置spdlog库的目录
if(NOT DEFINED spdlog_INCLUDE_DIRS)
  set(spdlog_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/spdlog/include)
endif()
# 输出spdlog库的目录看看
message(STATUS "spdlog_INCLUDE_DIRS = " ${spdlog_INCLUDE_DIRS})

# Dependencies
target_include_directories(test.out PRIVATE ${CMAKE_SOURCE_DIR} ${spdlog_INCLUDE_DIRS})
target_link_libraries(test.out PRIVATE dlog ${dlog_LIBRARIES} gtest gtest_main)

add_test(NAME AllCppTest COMMAND test.out)


# 这是vcpkg时候的写法
# Dependencies
# find_package(GTest MODULE REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
# target_link_libraries(utcpp.out PRIVATE dlog GTest::GTest GTest::Main)
#add_test(NAME TestsInUTCpp COMMAND utcpp.out)
