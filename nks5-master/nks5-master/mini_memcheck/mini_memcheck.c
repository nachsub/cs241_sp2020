/**
 * Mini Memcheck
 * CS 241 - Spring 2020
 * Worked with anathan4, jasneet2, neilr3, vivekb2, vishnus3
 */
#include "mini_memcheck.h"
#include <stdio.h>
#include <string.h>

size_t invalid_addresses;
size_t total_memory_freed;
size_t total_memory_requested;
meta_data *head;

/*
 * Wrap a call to malloc.
 *
 * This malloc creates a metadata object and inserts it into the head of the
 * list. You have to allocate enough to hold both the user's requested amount
 * of memory and the meta_data structure. You should only call malloc once in
 * this function.
 *
 * If the requested size is 0, malloc's behavior is undefined. In this case,
 * you may just return a NULL pointer.
 *
 * @param request_size
 *  Size of the requested memory block, in bytes.
 * @param filename
 *  Name of the file invoking this call to mini_malloc. You should store this
 *  directly in the metadata without modification.
 * @param instruction
 *  Address of the instruction invoking this call to mini_malloc.
 *
 * @return
 *  On success, return a pointer to the memory block allocated by the function.
 *  This should be the start of the user's memory, and not the meta_data.
 *
 *  If the function fails to allocate the requested block of memory, return a
 *  NULL pointer.
 */


void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
    // your code here
    if (request_size <= 0) {
        return NULL;
    }

    void * space = malloc(sizeof(meta_data) + request_size);

    if (space != NULL) {
        meta_data * data = (meta_data *) (space);
        void * output = space + sizeof(meta_data);

        data -> request_size = request_size;
        data -> filename = strdup(filename);
        data -> instruction = instruction;
        data -> next = head;
        head = data;
        total_memory_requested = total_memory_requested + request_size;
        return output;
    } else {
        return NULL;
    }
}


/*
 * Wrap a call to calloc.
 *
 * This works just like malloc, but zeros out the allocated memory.
 *
 * You may call calloc, malloc, or mini_malloc in this function,
 * but you should only do it once.
 *
 * If either the number of elements or the element size is 0, calloc's behavior
 * is undefined.
 *
 * @param num_elements
 *  Number of elements to allocate.
 * @param element_size
 *  Size of each element, in bytes.
 * @param filename
 *  Name of the file invoking this call to mini_calloc.
 * @param instruction
 *  Address of the instruction invoking this call to mini_calloc.
 *
 * @return
 *  On success, return a pointer to the memory block allocated by the function.
 *  This should be the start of the user's memory, and not the meta_data.
 *
 *  If the function fails to allocate the requested block of memory, return a
 *  NULL pointer.
 */
void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
    // your code here
    if (num_elements == 0 || element_size == 0) {
        return NULL;
    }

    size_t new_request_size = num_elements * element_size;
    meta_data * data = calloc(1, (new_request_size + sizeof(meta_data)));

    if (data == NULL) {
        return NULL;
    } else {
        total_memory_requested = total_memory_requested + new_request_size;
        data -> request_size = new_request_size;
        data -> filename = strdup(filename);
        data -> instruction = instruction;
        data -> next = NULL;
        if (head == NULL) {
            head = data;
            head -> next = NULL;
        } else {
            data -> next = head;
            head = data;
        }
        return (void*)data + sizeof(meta_data);
    }
    return NULL;
}



/*
 * Wrap a call to realloc.
 *
 * If the given pointer is NULL, you should treat this like a call to
 * mini_malloc. If the requested size is 0, you should treat this like a call
 * to mini_free and return NULL. If the pointer is NULL and the size is 0,
 * the behavior is undefined.
 *
 * In all other cases, you should use realloc to resize an existing allocation,
 * and then update the existing metadata struct with new values. If the size of
 * the allocation has increased, you must increase total_memory_requested;
 * if it has decreased, you must increase total_memory_freed. In other words,
 * these values should never decrease.
 *
 * If the user tries to realloc an invalid pointer, increment invalid_addresses
 * and return NULL.
 *
 * As with the other functions, you should only call malloc or realloc once.
 *
 * @param ptr
 *  The pointer to realloc.
 * @param request_size
 *  Size of the requested memory block, in bytes.
 * @param filename
 *  Name of the file invoking this call to mini_realloc.
 * @param instruction
 *  Address of the instruction invoking this call to mini_realloc.
 *
 * @return
 *  On success, return a pointer to the memory block allocated by the function.
 *  This should be the start of the user's memory, and not the meta_data.
 *
 *  If the function fails to allocate the requested block of memory, return a
 *  NULL pointer.
 */

void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
    if(payload == NULL && request_size == 0) {
        return NULL;
        }
        if(payload == NULL) {
            return mini_malloc(request_size, filename, instruction);
        }

        if (request_size == 0) {
            mini_free(payload);
            return NULL;
        }
        
        meta_data* prev = NULL;
        meta_data* head_copy = head;
        meta_data* proxy_copy;
      
        while(head_copy != NULL) {
            proxy_copy = head_copy->next; 
             void* memory = ((void*) head_copy) + sizeof(meta_data);
             if (memory == payload) {
                 if(request_size == head_copy->request_size) {
                     head_copy->instruction = instruction;
                     head_copy->filename = filename;
                     return payload; 
                 }

                 if (prev != NULL) {
                     prev -> next = proxy_copy;
                 } else {
                     head = proxy_copy;
                 }
                 if (request_size > head_copy -> request_size) {
                     total_memory_requested += (request_size - head_copy -> request_size);
                 } else if (request_size < head_copy -> request_size) {
                     total_memory_freed += (head_copy -> request_size - request_size );
                 }

                 void* memory = realloc(head_copy, sizeof(meta_data) + request_size);
                 if (memory == NULL) {
                     return NULL;
                 }
                meta_data * updated_data = (meta_data *) memory;
                void * memory_requested = memory + sizeof(meta_data);
                updated_data -> request_size = request_size; 
                updated_data -> filename = filename; 
                updated_data -> instruction = instruction; 
                updated_data -> next = head; 
                head = updated_data; 
                return memory_requested;
             } 

             prev = head_copy;
             head_copy = proxy_copy;
        }
        invalid_addresses++;
        return NULL;
}


/*
 * Wrap a call to free.
 *
 * This free will also remove the metadata node from the list, assuming it is
 * a valid pointer.
 *
 * Unlike the regular free, you should not crash when given an invalid pointer.
 * Instead, increment invalid_addresses.
 *
 * Invalid pointers include pointers that you did not return from mini_malloc
 * (or mini_calloc, or mini_realloc), and double frees.
 *
 * @param ptr
 *  Pointer to a previously allocated memory block. If a NULL pointer is
 *  passed, no action occurs, and it does not count as a bad free.
 */
void mini_free(void *payload) {
    // your code here
    if (payload == NULL) {
        return;
    } else {

        meta_data * prev = NULL;
        meta_data * curr = head;
        meta_data * next = NULL;

        while (curr) {
            next = curr -> next;
            void * current_mem = (((void *) curr) + sizeof(meta_data));
            if (payload == current_mem) {
                if (prev != NULL) {
                    prev -> next = next;
                } else {
                    head = next;
                }
                total_memory_freed = total_memory_freed + curr -> request_size;
                free(curr);
                return;
            } else {
                prev = curr;
                curr = next;
            }
        }
        invalid_addresses = invalid_addresses + 1;
    }
}



