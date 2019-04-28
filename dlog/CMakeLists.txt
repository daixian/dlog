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
    # 这个设置只支持windows，所以不要使用了
    # add_definitions(-D SPDLOG_WCHAR_TO_UTF8_SUPPORT)
endif()

# Introduce variables: * CMAKE_INSTALL_LIBDIR * CMAKE_INSTALL_BINDIR *
# CMAKE_INSTALL_INCLUDEDIR

# Layout. This works for all platforms: * <prefix>/lib*/cmake/<PROJECT-NAME> *
# <prefix>/lib*/ * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# Library definition
if(WIN32)
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
                dllmain.cpp
                dlog.rc)
else()
    add_library(dlog
                STATIC
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
endif()
# target_compile_features(dlog PUBLIC cxx_std_14)

# Dependencies

# 设置spdlog库的目录
if(NOT DEFINED spdlog_INCLUDE_DIRS)
    set(spdlog_INCLUDE_DIRS ${PROJECT_LIB_DIR}/spdlog/include)
endif()
# 输出spdlog库的目录看看
message(STATUS "spdlog_INCLUDE_DIRS = " ${spdlog_INCLUDE_DIRS})

find_package(Boost 1.70.0 REQUIRED COMPONENTS filesystem date_time)
message(STATUS "Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS})
message(STATUS "Boost_LIBRARIES = " ${Boost_LIBRARIES})

target_include_directories(dlog PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${spdlog_INCLUDE_DIRS} ${PROJECT_LIB_DIR})
target_link_libraries(dlog PRIVATE ${Boost_LIBRARIES})

# 全都安装到根目录
install(TARGETS dlog
       RUNTIME DESTINATION "./"
       LIBRARY DESTINATION "./"
       ARCHIVE DESTINATION "./"
)
install(FILES "dlog.h" DESTINATION "./")
install(FILES "../UnitTest/DLog.cs" DESTINATION "./")

# Configuration
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
