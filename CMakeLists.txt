cmake_minimum_required(VERSION 3.16)

project(CustomMemoryAllocator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build the shared library from your newly named allocator.c
add_library(custom_malloc SHARED
    src/allocator.c
)

target_include_directories(custom_malloc
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(custom_malloc PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -g
)

# Tests - Compiled completely independently! 
# We deliberately do not use target_link_libraries here.
add_executable(test_malloc test/test_malloc.c)
add_executable(test_free test/test_free.c)
add_executable(test_calloc test/test_calloc.c)
add_executable(test_realloc test/test_realloc.c)
add_executable(test_alignment test/test_alignment.c)
add_executable(test_split test/test_split.c)
add_executable(test_coalsce test/test_coalsce.c)
add_executable(stress_test test/stress_test.c)
add_executable(benchmark benchmark/benchmark.c)

# We must link stress_test because it uses custom, non-standard API functions
target_link_libraries(stress_test custom_malloc)