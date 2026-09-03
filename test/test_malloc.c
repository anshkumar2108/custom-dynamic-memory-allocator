#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include "../include/allocator.h"
//Tests that basic memory allocation works and that edge cases (like asking for 0 bytes) are handled safely without crashing.
int main(){
    printf("\n-----------------------\n");
    printf("Running test malloc\n");
    void*ptr1=malloc(100);
    assert(ptr1!=NULL);
    memset(ptr1,0xAA,100);

    void *ptr2=malloc(0);
    assert(ptr2==NULL);
    free(ptr1);
    printf("Test malloc passed\n");
    printf("\n-----------------------\n");
    return 0;
}