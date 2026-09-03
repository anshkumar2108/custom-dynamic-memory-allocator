#!/bin/bash
echo "Building the allocator and tests out-of-source..."
mkdir -p build && cd build
cmake ..
make

echo ""
echo "--- Running Tests via Dynamic Symbol Overriding ---"

# Notice the 'lib' prefix that CMake added automatically!
LD_PRELOAD=./libcustom_malloc.so ./test_malloc
LD_PRELOAD=./libcustom_malloc.so ./test_split
LD_PRELOAD=./libcustom_malloc.so ./test_alignment
LD_PRELOAD=./libcustom_malloc.so ./test_calloc
LD_PRELOAD=./libcustom_malloc.so ./test_realloc
LD_PRELOAD=./libcustom_malloc.so ./test_coalsce
LD_PRELOAD=./libcustom_malloc.so ./test_free
LD_PRELOAD=./libcustom_malloc.so ./stress_test
echo "--- PERFORMANCE BENCHMARK ---"
echo "1. Standard Linux Allocator:"
./benchmark

echo "2. Custom Allocator (Custom Code):"
LD_PRELOAD=./libcustom_malloc.so ./benchmark

echo "Done!"