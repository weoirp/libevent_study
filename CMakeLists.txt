cmake_minimum_required(VERSION 2.8)

# 获取文件夹名称
get_filename_component(Application ${CMAKE_SOURCE_DIR} NAME)
PROJECT(${Application})


message(STATUS "This is Binary dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is Source dir " ${PROJECT_SOURCE_DIR})


set_property(GLOBAL PROPERTY USE_FOLDERS ON)


SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH ".")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
message(STATUS "bin:" ${EXECUTABLE_OUTPUT_PATH})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
message(STATUS "lib:" ${LIBRARY_OUTPUT_PATH})

set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
message(STATUS "include:" ${INCLUDE_PATH})

set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
message(STATUS "src:" ${SOURCE_PATH})

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 定义工程的库文件
link_directories(
    # /lib
    # /usr/lib
    ${LIBRARY_OUTPUT_PATH}
)

include_directories(
    ${INCLUDE_PATH}
)

add_subdirectory(src)
