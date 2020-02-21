/**
 * Mini Memcheck
 * CS 241 - Spring 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include "mini_memcheck.h"


meta_data *head;
size_t total_memory_requested;
size_t total_memory_freed;
size_t invalid_addresses;

int main() {
    // Your tests here using malloc and free
    void* mem = malloc(5);
    free(mem);
    return 0;
}
