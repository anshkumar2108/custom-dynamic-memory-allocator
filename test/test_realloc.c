#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/allocator.h"
int main(){
    printf("\n-----------------------\n");
    printf("Running Realloc tests..\n");
    // Test 1: Realloc on NULL acts exactly like malloc
    char*str=realloc(NULL,10);
    assert(str!=NULL);
    strcpy(str,"Qualcomm");

    //Test 2: Expand the block
    char*str_large=realloc(str,100);
    assert(str_large!=NULL);
    assert(strcmp(str_large,"Qualcomm")==0);
    free(str_large);
    printf("Realloc tests passed successfully..\n");
    printf("\n-----------------------\n");
    return 0;
    
}