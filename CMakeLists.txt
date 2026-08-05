cmake_minimum_required(VERSION 3.14)
project(bnm-free LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DOBBY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dobby)
set(DOBBY_GENERATE_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(${DOBBY_DIR})

add_library(bnm-free STATIC BNM.cpp)

target_include_directories(bnm-free PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${DOBBY_DIR}/include)

target_compile_definitions(bnm-free PRIVATE BNM_USE_DOBBY)

find_library(ANDROID_LOG_LIB log)
if(ANDROID_LOG_LIB)
    target_link_libraries(bnm-free PUBLIC ${ANDROID_LOG_LIB})
endif()
target_link_libraries(bnm-free PUBLIC dobby dl)
