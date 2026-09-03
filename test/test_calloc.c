#include<assert.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include "../include/allocator.h"
int main(){
    printf("\n-----------------------\n");
    printf("Running calloc test.....\n");
    int *arr=calloc(5,sizeof(int));
    assert(arr!=NULL);
    for(int i=0;i<5;i++){
        assert(arr[i]==0);
    }
    //Test 2: For the integer overflow case.
    //Volatile makes the compiler back off the error it is throwing.
    volatile size_t huge_amount=SIZE_MAX;
    void*ptr=calloc(huge_amount,huge_amount);//Trick compiler so that it cannot complain at compile time.
    assert(ptr==NULL);
    free(arr);
    free(ptr);
    printf("Calloc tests passed successfully..\n");
    printf("\n-----------------------\n");
    return 0;
}