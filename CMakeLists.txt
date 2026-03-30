cmake_minimum_required(VERSION 4.2)
project(dsp_template)
set(CMAKE_CXX_STANDARD 14)

# 编译选项
set(OPTIMIZATION_FLAGS "-O3 -ffast-math -mcpu=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPTIMIZATION_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS}")

# 使用NEON
add_compile_definitions(ARM_MATH_NEON)

# 针对AARCH64位架构，如果是32位处理器请注释掉此行
add_compile_definitions(ARM_MATH_AARCH64)

include(FetchContent)

FetchContent_Declare(
        cmsis_dsp
        GIT_REPOSITORY https://github.com/ARM-software/CMSIS-DSP.git
        GIT_TAG        v1.17.0
)

set(HOST ON CACHE BOOL "Build for host")
set(NEON ON CACHE BOOL "Enable NEON acceleration in CMSIS-DSP")

FetchContent_MakeAvailable(cmsis_dsp)

add_executable(dsp_template main.cpp)
target_link_libraries(dsp_template PRIVATE CMSISDSP)