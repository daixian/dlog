cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!
set(SUB_NAME "dlog_test")

set(CMAKE_CXX_STANDARD 14)
# add_definitions(-D SPDLOG_WCHAR_TO_UTF8_SUPPORT)

#去掉dlog的dllexport
add_definitions(-D DLOG_STATIC)

#这里会搜索到build文件夹下面的一个cpp文件,所以增加一个*Test.cpp的约束
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
FILE(GLOB_RECURSE SOURCE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*Test.cpp" 
                              "${CMAKE_SOURCE_DIR}/src/dlog/*.h" 
                              "${CMAKE_SOURCE_DIR}/src/dlog/*.cpp"
                              "${CMAKE_SOURCE_DIR}/src/dlog/*.hpp"

)
message(STATUS "SOURCE_LIST = ${SOURCE_LIST}")

# 这个函数在VS工程里生成一个文件路径
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        #这个表示在VS里划分一个组
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${SOURCE_LIST})

add_executable(${SUB_NAME}
               ${SOURCE_LIST})

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# CMake 2.8.11 之后gtest是自动添加的，所以下面的没有执行
if (CMAKE_VERSION  VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

# # 输出dlog库的目录看看
message(STATUS "dlog_INCLUDE_DIRS = ${CMAKE_SOURCE_DIR}")

# Dependencies
target_include_directories(${SUB_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(${SUB_NAME} PRIVATE dlog ${CONAN_LIBS})

add_test(NAME AllCppTest COMMAND ${SUB_NAME})
