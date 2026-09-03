#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ITERATIONS 50000
#define BLOCK_SIZE 256
int main(){
    printf("Benchmarking %d allocations of %d bytes...\n", ITERATIONS, BLOCK_SIZE);
    void *ptrs[ITERATIONS];
    //Measure raw allocation speed
    clock_t start_malloc=clock();
    for(int i=0;i<ITERATIONS;i++){
        ptrs[i]=malloc(BLOCK_SIZE);
    }
    clock_t end_malloc=clock();

    // Measure raw freeing speed
    clock_t start_free = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        free(ptrs[i]);
    }
    clock_t end_free = clock();

    double time_malloc=((double)(end_malloc-start_malloc))/CLOCKS_PER_SEC;
    double time_free=((double)(end_free-start_free))/CLOCKS_PER_SEC;
    printf("Malloc Time: %f seconds\n", time_malloc);
    printf("Free Time:   %f seconds\n", time_free);
    printf("Total Time:  %f seconds\n\n", time_malloc + time_free);
    return 0;
}
