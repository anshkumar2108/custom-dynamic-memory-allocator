#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

// Standard POSIX memory functions
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

// Custom profiling functions
void measure_fragmentation();
void check_for_leaks();

#endif