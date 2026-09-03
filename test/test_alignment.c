#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "../include/allocator.h"
void test_alignment()
{
    printf("\n-----------------------\n");
    printf("Running test_alignment..\n");
    void *ptr1 = malloc(1);
    void *ptr2 = malloc(7);
    void *ptr3 = malloc(13);
    void *ptr4 = malloc(17);
    // Cast the pointers to raw integers so we can perform maths operations on it
    uintptr_t addr1 = (uintptr_t)ptr1;
    uintptr_t addr2 = (uintptr_t)ptr2;
    uintptr_t addr3 = (uintptr_t)ptr3;
    uintptr_t addr4 = (uintptr_t)ptr4;

    assert((addr1 % 16) == 0);
    assert((addr2 % 16) == 0);
    assert((addr3 % 16) == 0);
    assert((addr4 % 16) == 0);
    // Clean up the pointers
    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);
    printf("Test_alignment PASSED!\n");
}
int main()
{
    printf("Starting unit tests for alignment\n");
    test_alignment();
    printf("All tests finished successfully.\n");
    printf("\n-----------------------\n");
    return 0;
}