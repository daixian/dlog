cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)
# add_definitions(-D SPDLOG_WCHAR_TO_UTF8_SUPPORT)

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
  set(dlog_OBJECTS ${CMAKE_BINARY_DIR}/dlog/dlog.dir/Release)
  set(dlog_LIBRARIES ${LIBRARY_OUTPUT_PATH}/dlog.lib 
      ${dlog_OBJECTS}/Common.obj
      ${dlog_OBJECTS}/Debug.obj
      ${dlog_OBJECTS}/FileHelper.obj
      ${dlog_OBJECTS}/MemoryLog.obj
      )
  else()
    # 寻找文件夹下的所有lib文件
    file(GLOB dlog_LIBRARIES "${LIBRARY_OUTPUT_PATH}/*.*") 
  endif()
endif()
# # 输出dlog库的目录看看
message(STATUS "dlog_INCLUDE_DIRS = ${CMAKE_SOURCE_DIR}")
message(STATUS "dlog_LIBRARIES = ${dlog_LIBRARIES}")
message(STATUS "dlog_OBJECTS = ${dlog_OBJECTS}")

# Dependencies
target_include_directories(test.out PRIVATE ${CMAKE_SOURCE_DIR} ${PROJECT_LIB_DIR})

if(WIN32)
    #去掉这个可能引起错误的libboost_unit_test_framework
    # STRING(REGEX REPLACE "libboost_unit_test_framework " "" CONAN_LIBS_CUT ${CONAN_LIBS})
    list(REMOVE_ITEM CONAN_LIBS "libboost_unit_test_framework") 
else()
    list(REMOVE_ITEM CONAN_LIBS "boost_unit_test_framework") 
endif()
# message(STATUS "CONAN_LIBS = " ${CONAN_LIBS})
target_link_libraries(test.out PRIVATE dlog ${dlog_LIBRARIES} ${CONAN_LIBS} )

add_test(NAME AllCppTest COMMAND test.out)

# 这是vcpkg时候的写法
# Dependencies
# find_package(GTest MODULE REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
# target_link_libraries(utcpp.out PRIVATE dlog GTest::GTest GTest::Main)
#add_test(NAME TestsInUTCpp COMMAND utcpp.out)
