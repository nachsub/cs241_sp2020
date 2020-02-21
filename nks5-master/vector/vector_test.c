/**
 * Vector
 * CS 241 - Spring 2020
 */
#include "vector.h"
#include <stdio.h>
int main() {
    // Write your test cases here
    vector* v = vector_create(&int_copy_constructor, &int_destructor, &int_default_constructor);
    int elem = 5;
    void* ptr = &elem;
    vector_push_back(v, ptr);
    printf("%d\n", *(int*)vector_get(v, 0));
    vector_destroy(v);
    return 0;
}
