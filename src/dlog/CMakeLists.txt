cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# BUILD_SHARED_LIBS can controll build type!

set(CMAKE_CXX_STANDARD 14)

# 隐藏导出函数这个确实是有效的，但是目前看来隐藏它并没有什么意义，会导致测试工程找不到函数还
# set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# set(CMAKE_C_VISIBILITY_PRESET hidden)

#ZPCORE
set(SUB_NAME "dlog")

# 先自动添加算了
FILE(GLOB_RECURSE SOURCE_LIST "*.cpp" "*.h" "*.hpp" "*.c")

#windows下添加资源文件
if(WIN32)
    if(DLOG_BUILD_SHARED)
        list(APPEND SOURCE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/${SUB_NAME}.rc")
    endif()
endif()

add_definitions(-D UNICODE)
if(WIN32)
    add_definitions(-D _WINDOWS)
    if(DLOG_BUILD_SHARED)
        # _USERDLL是和dllmain函数相关的
        add_definitions(-D _USERDLL)
    endif()

    # 这个设置只支持windows，所以不要使用了
    # add_definitions(-D SPDLOG_WCHAR_TO_UTF8_SUPPORT)
endif()

# Introduce variables: * CMAKE_INSTALL_LIBDIR * CMAKE_INSTALL_BINDIR *
# CMAKE_INSTALL_INCLUDEDIR

# Layout. This works for all platforms: * <prefix>/lib*/cmake/<PROJECT-NAME> *
# <prefix>/lib*/ * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

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

# message(STATUS "CMAKE_CURRENT_SOURCE_DIR = " ${CMAKE_CURRENT_SOURCE_DIR})
# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
# https://stackoverflow.com/questions/31422680/how-to-set-visual-studio-filters-for-nested-sub-directory-using-cmake
# 但是这个不知道为什么不不管用
# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

message(STATUS "SOURCE_LIST = ${SOURCE_LIST}")

# Library definition
if(DLOG_BUILD_SHARED)
    add_definitions(-D DLOG_DLL)
    add_definitions(-D DLOG_DLL_EXPORTS)
    add_library(${SUB_NAME}
                SHARED
                ${SOURCE_LIST})
else()
    add_definitions(-D DLOG_STATIC)
    add_library(${SUB_NAME}
                STATIC
                ${SOURCE_LIST})
endif()

# Dependencies

# # 设置spdlog库的目录
# if(NOT DEFINED spdlog_INCLUDE_DIRS)
#     set(spdlog_INCLUDE_DIRS ${PROJECT_LIB_DIR}/spdlog/include)
# endif()

# target_include_directories(dlog PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${spdlog_INCLUDE_DIRS} ${rapidjson_INCLUDE_DIR} ${PROJECT_LIB_DIR})
# target_link_libraries(dlog PRIVATE ${Boost_LIBRARIES} ${CONAN_LIBS})
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CONAN_LIBS = ${CONAN_LIBS}")
target_include_directories(dlog PRIVATE  ${CMAKE_SOURCE_DIR} ${PROJECT_LIB_DIR})
target_link_libraries(dlog PRIVATE ${CONAN_LIBS})

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
