cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(PackageTest CXX)
# include(GoogleTest)

# 设置生成一个VS工程里的文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 14)
set(SUB_NAME "package_test")

add_definitions(-D UNICODE)
if(WIN32)
    add_definitions(-D _WINDOWS)
endif()

#这里会搜索到build文件夹下面的一个cpp文件,所以增加一个*Test.cpp的约束
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
FILE(GLOB_RECURSE SOURCE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*Test.cpp" "pch.h" "pch.cpp")
message(STATUS "SOURCE_LIST = ${SOURCE_LIST}")
add_executable(${SUB_NAME}
               ${SOURCE_LIST})
 
#find_package(Threads)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# CMake 2.8.11 之后gtest是自动添加的，所以下面的没有执行
if (CMAKE_VERSION  VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()


# message(STATUS "test修改之后CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

message(STATUS "Test工程当前CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message(STATUS "Test工程当前CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Test工程当前EXECUTABLE_OUTPUT_PATH = ${EXECUTABLE_OUTPUT_PATH}")

find_package (Threads)

# Dependencies
target_include_directories(${SUB_NAME} PRIVATE ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/src")

# 这里exe链接不进来,One may link only to INTERFACE, OBJECT, STATIC or SHARED libraries                                               
target_link_libraries(${SUB_NAME} PRIVATE ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT} "dlog")

# if(WIN32)
#     add_custom_command(TARGET ${SUB_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory "${CONAN_BIN_DIRS_DLOG}" ${EXECUTABLE_OUTPUT_PATH}) #拷贝整个文件夹算了
# endif()

# gtest_discover_tests(${SUB_NAME})
# add_test(NAME AllCppTest COMMAND ${SUB_NAME})