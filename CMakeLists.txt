cmake_minimum_required(VERSION 3.22.1)

project(Demo C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto -fvisibility=hidden -ffunction-sections -fdata-sections -fomit-frame-pointer -funwind-tables")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -Wl,--strip-all -flto")

# --- ShadowHook ---
set(SHADOWHOOK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shadowhook)

if(CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a" OR ANDROID_ABI STREQUAL "arm64-v8a")
    set(SHADOWHOOK_ARCH arm64)
else()
    set(SHADOWHOOK_ARCH arm)
endif()

file(GLOB SHADOWHOOK_SRC
    ${SHADOWHOOK_DIR}/*.c
    ${SHADOWHOOK_DIR}/arch/${SHADOWHOOK_ARCH}/*.c
    ${SHADOWHOOK_DIR}/arch/${SHADOWHOOK_ARCH}/*.S
    ${SHADOWHOOK_DIR}/common/*.c
    ${SHADOWHOOK_DIR}/third_party/*/*.c
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Includes/BNM
    ${SHADOWHOOK_DIR}
    ${SHADOWHOOK_DIR}/include
    ${SHADOWHOOK_DIR}/arch/${SHADOWHOOK_ARCH}
    ${SHADOWHOOK_DIR}/common
    ${SHADOWHOOK_DIR}/third_party/xdl
    ${SHADOWHOOK_DIR}/third_party/bsd
    ${SHADOWHOOK_DIR}/third_party/lss
)

add_library(Demo SHARED
    Main.cpp
    src/BNM.cpp
    ${SHADOWHOOK_SRC}
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

target_compile_definitions(Demo PRIVATE BNM_USE_SHADOWHOOK)

find_library(ANDROID_LOG_LIB log)
if(ANDROID_LOG_LIB)
    target_link_libraries(Demo PRIVATE ${ANDROID_LOG_LIB})
endif()
target_link_libraries(Demo PRIVATE dl)
