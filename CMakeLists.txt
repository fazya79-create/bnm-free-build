cmake_minimum_required(VERSION 3.14)
project(bnm-free LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DOBBY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dobby)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${DOBBY_DIR}/include
)

set(DOBBY_GENERATE_SHARED OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${DOBBY_DIR})

add_library(bnm-free STATIC BNM.cpp)

target_compile_definitions(bnm-free PRIVATE BNM_USE_DOBBY)

find_library(ANDROID_LOG_LIB log)
if(ANDROID_LOG_LIB)
    target_link_libraries(bnm-free PUBLIC ${ANDROID_LOG_LIB})
endif()
target_link_libraries(bnm-free PUBLIC dobby dl)
