/**
 * Vector
 * CS 241 - Spring 2020
 */
#include "sstring.h"
#include "vector.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <string.h>

struct sstring {
    // Anything you want
    vector* vec;
};


sstring *cstr_to_sstring(const char *input) {
    // your code goes here
    sstring* s = malloc(sizeof(sstring));
    s->vec = vector_create(&char_copy_constructor, &char_destructor, &char_default_constructor);
    for (unsigned i = 0; input[i]; i++) {
        vector_push_back(s->vec, (void*)(input + i));
    }
    char c = 0;
    vector_push_back(s->vec, &c);
    return s;
}

char *sstring_to_cstr(sstring *input) {
    // your code goes here
    char* str = malloc((vector_size(input->vec) + 1)*sizeof(char*));
    for (unsigned i = 0; i < vector_size(input->vec); i++) {
        str[i] = *(char*)vector_get(input->vec, i);
    }
    str[vector_size(input->vec)] = 0;
    return str;
}

int sstring_append(sstring *this, sstring *addition) {
    // your code goes here
    vector_pop_back(this->vec);
    for (size_t i = 0; i < vector_size(addition->vec); i++) {
        vector_push_back(this->vec, vector_get(addition->vec, i));
    }
    return vector_size(this->vec) - 1;
}

vector *sstring_split(sstring *this, char delimiter) {
    // your code goes here
    vector* vec = vector_create(&char_copy_constructor, &char_destructor, &char_default_constructor);
    char cur[10];
    for (size_t i = 0; i < vector_size(this->vec); i++) {
        if (*(char*)vector_get(this->vec, i) == delimiter) {
            vector_push_back(vec, (void*)cur);
            strcpy(cur, ""); 
        } else {
            strcat(cur, (char*)(vector_get(this->vec, i)));
        }
    }
    return vec;
}

int sstring_substitute(sstring *this, size_t offset, char *target,
                       char *substitution) {
    // your code goes here
    if (vector_size(this->vec) <= offset) return -1;
    for (size_t i = offset; i < vector_size(this->vec) - strlen(target); i++) {
        for (size_t j = 0; j < strlen(target); j++) {
            if (vector_get(this->vec, i + j) != target+j) {
                break;
            }
            if (j == strlen(target) - 1) {
                for (size_t k = i; k < i + strlen(target); k++) {
                    vector_erase(this->vec, k);
                }
                for (size_t k = 0; k < strlen(substitution); k++)
                    vector_insert(this->vec, i, (void*)(substitution+k));
                return 0;
            }
        }
    }
    return -1;
}

char *sstring_slice(sstring *this, int start, int end) {
    // your code goes here
    char* str = malloc((end - start + 1) * sizeof(char));
    for (int i = start; i < end; i++) {
        strcat(str, (char*)vector_get(this->vec, i));
    }
    str[end - start] = 0;
    return str;
}

void sstring_destroy(sstring *this) {
    // your code goes here
    vector_destroy(this->vec);
    free(this);
}
