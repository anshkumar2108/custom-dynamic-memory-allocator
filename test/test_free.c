#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include "../include/allocator.h"
int main(){
    printf("\n-----------------------\n");
    printf("Running malloc free tests..\n");
    void*ptr1=malloc(256);
    free(ptr1);
    void*ptr2=malloc(256);
    assert(ptr1==ptr2);//Checking address reusablility;
    free(ptr2);
    printf("Test malloc free runs successfully.\n");
    printf("\n-----------------------\n");
    return 0;
}