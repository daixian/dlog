cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)

# 隐藏导出函数这个确实是有效的，但是目前看来隐藏它并没有什么意义，会导致测试工程找不到函数还
# set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# set(CMAKE_C_VISIBILITY_PRESET hidden)

add_definitions(-D DLOG_EXPORTS)
add_definitions(-D UNICODE)
if(WIN32)
  add_definitions(-D _WINDOWS)
  add_definitions(-D _USERDLL)
endif()

# Introduce variables: * CMAKE_INSTALL_LIBDIR * CMAKE_INSTALL_BINDIR *
# CMAKE_INSTALL_INCLUDEDIR

# Layout. This works for all platforms: * <prefix>/lib*/cmake/<PROJECT-NAME> *
# <prefix>/lib*/ * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# Library definition
add_library(dlog
            SHARED
            resource.h
            stdafx.h
            targetver.h
            Common/Common.h
            Common/Debug.h
            Common/FileHelper.h
            Common/stack_allocator.hpp
            Common/MemoryLog.h
            dlog.h
            Common/Debug.cpp
            Common/Common.cpp
            Common/FileHelper.cpp
            Common/MemoryLog.cpp
            dlog.cpp
            stdafx.cpp
            dllmain.cpp)
# target_compile_features(dlog PUBLIC cxx_std_14)

# Dependencies
find_package(Boost 1.69.0 REQUIRED COMPONENTS filesystem date_time)
message(STATUS "Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS})
message(STATUS "Boost_LIBRARIES = " ${Boost_LIBRARIES})
target_include_directories(dlog PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(dlog PRIVATE ${Boost_LIBRARIES})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(dlog PRIVATE spdlog::spdlog)

# target_include_directories(dlog PUBLIC
# $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
# $<INSTALL_INTERFACE:include/> PRIVATE lib/src/ )

# Add definitions for targets Values: * Debug:   -DEVENTBUS_DEBUG=1 * Release:
# -DEVENTBUS_DEBUG=0 * other:   -DEVENTBUS_DEBUG=0
# target_compile_definitions(dlog PUBLIC "EVENTBUS_DEBUG=$<CONFIG:Debug>")

# Configuration
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
