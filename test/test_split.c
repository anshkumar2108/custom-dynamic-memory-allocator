#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/allocator.h"
#define META_SIZE 32
int main(){
    printf("\n-----------------------\n");
    printf("Running test Split.\n");
    void*ptr_l=malloc(1024);
    free(ptr_l);
    void*ptr_s=malloc(16);
    assert(ptr_l==ptr_s);
    void*ptr_s2=malloc(16);
    uintptr_t exp_addr=(uintptr_t) ptr_s+16+META_SIZE;
    assert((uintptr_t)ptr_s2==exp_addr);
    free(ptr_s);
    free(ptr_s2);
    printf("Test split passed successfully.\n");
    printf("\n-----------------------\n");

    return 0;
}