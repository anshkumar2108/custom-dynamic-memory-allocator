#include<stdio.h>
#include<stdbool.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "../include/allocator.h"
// extern void measure_fragmentation();
int main()
{
    srand(time(NULL));
    void *allocations[1000];
    int count = 0;

    printf("Starting stress test...\n");

    for (int i = 0; i < 10000; i++)
    {
        int action = rand() % 3;
        
        if (action == 0 && count < 1000)
        { // Allocate
            size_t size = rand() % 10000 + 1;
            allocations[count] = malloc(size);
            if (allocations[count])
            {
                // Write pattern to test memory
                memset(allocations[count], 0xAA, size);
                count++;
            }
        }
        else if (action == 1 && count > 0)
        { // Free (if implemented)
            int idx = rand() % count;
            free(allocations[idx]);
            allocations[idx] = allocations[--count];
        }
        else
        { // Read/verify
            if (count > 0)
            {
                int idx = rand() % count;
                if (allocations[idx])
                {
                    // Just read a byte to ensure it's accessible
                    // volatile unsigned char c = *(unsigned char*)allocations[idx];
                }
            }
        }
        if(i!=0&&i%1000==0)measure_fragmentation();
    }

    printf("Stress test completed!\n");
    printf("Cleaning up remaining memory\n");
    for (int i = 0; i < count; i++)
    {
        if (allocations[i] != NULL)
            free(allocations[i]);
    }
    // check_for_leaks();

    return 0;
}