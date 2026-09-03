#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include "../include/allocator.h"
int main(){
    printf("\n-----------------------\n");
    printf("Running coalsce test...\n");
    void*a=malloc(128);
    void*b=malloc(128);
    void*c=malloc(128);
    free(a);
    free(b);
    void*d=malloc(200);
    assert(d==a);
    free(c);
    free(d);
    printf("Coalsce test passed successfully.\n");
    printf("\n-----------------------\n");
    return 0;
}