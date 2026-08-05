cmake_minimum_required(VERSION 3.22.1)

project(Demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -fvisibility=hidden -ffunction-sections -fdata-sections -fomit-frame-pointer -funwind-tables")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -Wl,--strip-all -flto")

set(DOBBY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dobby)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/BNM
    ${DOBBY_DIR}/include
)

set(DOBBY_GENERATE_SHARED OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${DOBBY_DIR})

add_library(Demo SHARED
    Main.cpp
    BNM.cpp
    src/Utils.cpp
    src/Loading.cpp
    src/Class.cpp
    src/MethodBase.cpp
    src/FieldBase.cpp
    src/PropertyBase.cpp
    src/Image.cpp
    src/MonoStructures.cpp
    src/Defaults.cpp
    src/UnityStructures.cpp
    src/Delegates.cpp
    src/ClassesManagement.cpp
    src/Coroutine.cpp
)

target_compile_definitions(Demo PRIVATE BNM_USE_DOBBY)

find_library(ANDROID_LOG_LIB log)
if(ANDROID_LOG_LIB)
    target_link_libraries(Demo PRIVATE ${ANDROID_LOG_LIB})
endif()
target_link_libraries(Demo PRIVATE dobby dl)
