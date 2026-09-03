
# Custom Dynamic Memory Allocator

A POSIX-compliant dynamic memory allocator written in C from scratch. This project completely replaces the standard Linux `glibc` memory manager with a custom implementation of `malloc`, `free`, `calloc`, and `realloc`. It features advanced heap profiling, external fragmentation analysis, and algorithmic optimizations designed for systems engineering environments.


##  🚀 Key Features

- **Core Memory API:** Fully implements `malloc`, `free`, `calloc`, and `realloc` using direct `sbrk()` system calls.
- **O(1) Fast-Path Allocation:** Implements a global tail pointer and free-block tracker to completely bypass O(N) linear searches when the heap is full, granting instant O(1) memory expansion.
- **Opportunistic Coalescing & Splitting:** Dynamically merges adjacent free blocks to fight fragmentation and splits oversized blocks to preserve heap density.
- **Strict 16-Byte Alignment:** Enforces hardware-level 16-byte boundary alignment for all payloads to ensure CPU architectural compatibility.
- **Dynamic Symbol Overriding:** Built to be injected directly into existing standard C programs at runtime using `LD_PRELOAD`.
## 📊 Built-in Systems Profiling

Unlike standard allocators, this project includes a custom analytics API to monitor heap health in real-time:

- **Heap Fragmentation Analyzer:** Calculates the exact External Fragmentation Score by mapping the largest available free block against total free heap space (1 - (Largest / Total)).
- **Memory Leak Detector:** Traverses the metadata linked list at program exit to identify and report the exact byte sizes and memory addresses of un-freed blocks.
## 🏗️ Architecture Under the Hood

The heap is managed as a linked list of metadata headers. Each block is prefixed with a 32-byte header containing its size, status flags, and a magic number for corruption detection.

```c
struct block_meta {
    size_t size;
    int free;
    int magic;
    struct block_meta *next;
    char padding[8]; // Aligns header to exactly 32 bytes
};
```
### 🛠️ Building and Running (WSL / Linux)
This project uses CMake for out-of-source builds to keep the workspace clean.

1. Automated Build and Test (Recommended)
The easiest way to compile the library and run the entire test suite (including the benchmark) is by running the automated bash script from the root directory:

```c 
bash run_tests.sh
```

2. Manual Compilation
If you prefer to compile the project manually:

bash
### Create a build directory
```c
mkdir -p build && cd build
```
### Configure and compile
```c
cmake ..
make
```

3. Manual Test Execution
Because the tests are compiled independently of the library, you must inject the allocator at runtime using the Linux dynamic linker:

bash
#### Run a specific test with dynamic injection from inside the build directory
```c 
LD_PRELOAD=./libcustom_malloc.so ./stress_test
```
## 📁 Project Structure

```c
custom-dynamic-memory-allocation/
├── src/
│   └── allocator.c          # Core memory management logic
├── include/
│   └── allocator.h          # API contract and profiling headers
├── test/
│   ├── stress_test.c        # 10,000-iteration randomized workload
│   ├── test_malloc.c        # Edge-case and basic allocation tests
│   └── ...                  # Unit tests for split, coalesce, etc.
├── benchmark/
│   └── benchmark.c          # Time complexity benchmarking (CPU ticks)
├── CMakeLists.txt           # Build configuration
└── run_tests.sh             # Automated testing and benchmarking script
```
## 🧪 Performance Benchmarks

The custom allocator has been stress-tested against the standard Linux allocator. Below are the results from a benchmark of 50,000 allocations of 256 bytes:
```c
Allocator	      Malloc Time	Free Time	Total Time
Standard Linux	  0.139879s	    0.002033s	0.141912s
Custom Allocator  26.567265s	0.001417s	26.568682s
```

Note: The custom allocator's performance is currently slower than glibc's implementation due to linear search overhead. Upcoming optimizations include segregated free lists and bidirectional coalescing to achieve near-O(1) allocation times.

## 🔧 Future Optimizations

*  Segregated Free Lists: Implement size-class based bins for O(1) allocation of common sizes

* Bidirectional Coalescing: Add prev pointer to merge with both adjacent blocks

* Thread Safety: Add mutex locks for multi-threaded environments

* mmap Support: Use mmap for large allocations (>128KB)

* TLS Caching: Thread-local storage caches for hot allocations

## 👨‍💻 Author 

Ansh Kumar Khandwal

www.linkedin.com/in/ansh-kumar-khandwal-bba415210

## 📚 References

https://danluu.com/malloc-tutorial/

https://csapp.cs.cmu.edu/