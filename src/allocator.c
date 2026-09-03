#include <assert.h>    //For Debugging Purposes
#include <unistd.h>    //For system calls like sbrk
#include <sys/types.h> //For datatypes such as size_t
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <time.h> //Debugging
#include <stdlib.h>
#include <valgrind/memcheck.h> //Debugging
#define META_SIZE sizeof(struct block_meta)
#define ALIGNMENT 16 //Converting every block to mulptile of 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1)) // Align the size of blocks needed to multiple of 16
void *global_base = NULL;
struct block_meta*roving_ptr=NULL;//Used while finding block->Next fit optimization
struct block_meta
{
    size_t size;
    int free;  // 1=Free 0=Used
    int magic; // Only for debug purposes
    struct block_meta *next;
    char padding[8]; // Make the block size exactly 32 bytes
};
void coalse(struct block_meta*block){
    // write(1,"Coalsce custom function",24);//For debugging
    if(block==NULL||block->next==NULL)return;
    struct block_meta*next_block=block->next;
    if(block->free&&next_block->free){
        block->size=block->size+next_block->size+META_SIZE;
        block->next=next_block->next;
        next_block->next=NULL;
        block->free=1;
        block->magic=0x55555555;
    }else return;
}
void split_block(struct block_meta *block, size_t size)
{
    // Calculate the exact memory required for the new block with the meta data size.
    struct block_meta *new_block = (struct block_meta *)((char *)block + META_SIZE + size); // The new Free block
    new_block->size = block->size - size - META_SIZE;
    new_block->free = 1;
    new_block->magic = 0x55555555;
    new_block->next = block->next;
    block->next = new_block;
    block->size = size;
}

//Using the First fit method uncomment it and then run bash run_tests.sh
//for getting the results.
// struct block_meta *find_free_block(struct block_meta **last, size_t size)
// {
//     struct block_meta *current = global_base;
//     while(current){
//         while(current->next&&current->free&&current->next->free){
//             coalse(current);
//         }
//         if(current->free&&current->size>=size){
//             return current;
//         }
//         *last=current;
//         current=current->next;
//     }
//     return NULL;
// }
// Incase no block is found then request the space from os using sbrk

//Next Fit optimization
struct block_meta* find_free_block(struct block_meta**last,size_t size){
    if(!roving_ptr)roving_ptr=global_base;
    struct block_meta*current=roving_ptr;
    struct block_meta*start_check=roving_ptr;//Remember from where we started
    while(current){
        while(current->next&&current->free&&current->next->free){
            coalse(current);
        }
        if(current->free&&current->size>=size){
            roving_ptr=current;//Save the pointer for next time where we left at last.
            return current;
        }
        *last=current;
        current=current->next;
    }
    //If we didnt find anything. start from start(global base)
    current=global_base;
    while(current!=start_check){
        while(current->next&&current->free&&current->next->free){
            coalse(current);
        }
        if (current->free && current->size >= size) {
            roving_ptr = current; // Save our spot!
            return current;
        }
        current = current->next;
    }
    return NULL;

}
struct block_meta *request_space(struct block_meta *last, size_t size)
{
    struct block_meta *block;
    block = sbrk(0);
    struct block_meta *request;
    request = sbrk(size + META_SIZE);
    assert((void *)block == request);
    if (request == (void *)-1)
        return NULL; // sbrk failed
    if (last)
    { // if ll is not empty
        last->next = block;
    }
    block->next = NULL;
    block->size = size;
    block->free = 0;
    block->magic = 0x12345678;
    return block;
}
void *malloc(size_t size)
{
    // write(1, "CUSTOM MALLOC CALLED\n", 21); For debugging that this malloc is called or not
    struct block_meta *block;
    if (size <= 0)
        return NULL;
    size = ALIGN(size);
    if (!global_base)
    { // First call
        block = request_space(NULL, size);
        if (!block)
            return NULL;
        // roving_ptr=block;//new 
        global_base = block;
    }
    else
    {
        struct block_meta *last = global_base;
        block = find_free_block(&last, size);
        if (!block)
        {
            block = request_space(last, size);
            if (!block)
                return NULL;
            roving_ptr=block;//new 
        }
        else
        {
            if (block->size >= size + META_SIZE + ALIGNMENT)
            {
                split_block(block, size);
            }
            block->free = 0;
            block->magic = 0x77777777;
        }
    }
    void *ptr = block + 1;
    VALGRIND_MALLOCLIKE_BLOCK(ptr, size, 0, 0); // For letting valgrind look for other than malloc and free words
    return (ptr);
}
struct block_meta *get_block_ptr(void *ptr)
{
    return (struct block_meta *)ptr - 1;
}
void free(void *ptr)
{
    if (!ptr)
        return;
    struct block_meta *block_head = get_block_ptr(ptr);
    assert(block_head->free == 0);
    assert(block_head->magic == 0x77777777 || block_head->magic == 0x12345678);
    block_head->free = 1;
    block_head->magic = 0x55555555;
    VALGRIND_FREELIKE_BLOCK(ptr, 0);
}
void *realloc(void *ptr, size_t size)
{
    if (!ptr)
        return malloc(size); // If NULL pointer passed then should work like malloc.
    struct block_meta *block_head = get_block_ptr(ptr);
    if (block_head->size >= size)
        return ptr; // If need less memory then why to reallocate return the same pointer.
    void *new_ptr;
    new_ptr = malloc(size);
    if (!new_ptr)
        return NULL;
    memcpy(new_ptr, ptr, block_head->size);
    free(ptr);
    return new_ptr;
}
void *calloc(size_t nel, size_t elsize)
{
    size_t size = nel * elsize;
    if (nel != 0 && elsize != 0 && nel > (SIZE_MAX / elsize))
    {
        errno = ENOMEM;
        return NULL;
    }
    void *ptr = malloc(size);
    memset(ptr, 0, size); // Setting the memory to 0;
    return ptr;
}
void check_for_leaks()
{
    struct block_meta *current = global_base;
    int leak_count = 0;
    size_t leaked_bytes = 0;

    printf("\n--- MEMORY PROFILER REPORT ---\n");
    while (current != NULL)
    {
        if (current->free == 0)
        {
            leak_count++;
            leaked_bytes += current->size;
            printf("LEAK: %zu bytes at block address %p\n", current->size, (void *)(current + 1));
        }
        current = current->next;
    }

    if (leak_count == 0)
    {
        printf("SUCCESS: All blocks are free. No memory leaks!\n");
    }
    else
    {
        printf("FAILED: %d blocks leaked, totaling %zu bytes.\n", leak_count, leaked_bytes);
    }
    printf("------------------------------\n\n");
}
void measure_fragmentation(){
    struct block_meta*current=global_base;
    size_t total_free_memory=0;
    size_t max_memory_block=0;
    printf("------------ Fragmentation-Analysis -------------\n");
    while(current){
        if(current->free==1){
            total_free_memory+=current->size;
            if(current->size>max_memory_block){
                max_memory_block=current->size;
            }
        }
        current=current->next;
    }
    if (total_free_memory == 0) {
        printf("Total Free Memory: 0 bytes\n");
        printf("External Fragmentation: 0.00%%\n");
        printf("--------------------------------\n\n");
        return;
    }
    //We will calculate the metric 1-(largest/total);
    double fragmentation=1.0-((double)max_memory_block/(double)total_free_memory);
    printf("Total Free Memory: %zu bytes\n", total_free_memory);
    printf("Largest Free Block: %zu bytes\n", max_memory_block);
    printf("External Fragmentation Score: %.2f%%\n", fragmentation * 100.0);
    printf("--------------------------------\n\n");
}
