/**
 * Extreme Edge Cases
 * CS 241 - Spring 2020
 */
#include "camelCaser.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


char** allocate_sentences(const char* input_str) {
    int i = 0;
    int num_sent = 0;
    while(input_str[i]) {
        if (ispunct(input_str[i])) {
            num_sent++;
        }
        i++;
    }
    char** camel_case = calloc((num_sent + 1), sizeof(char*)); //return string
    camel_case[num_sent] = NULL;
    return camel_case;
}

char** allocate_characters(char** camel_case, const char* input_str) {
    int sent_num = 0;
    int char_count = 0;
    int i = 0;
    while(input_str[i]) {
        if (ispunct(input_str[i])) {
            camel_case[sent_num] = calloc(char_count + 1, sizeof(char *));
            camel_case[sent_num][char_count] = '\0';
            sent_num++;
            char_count = 0;
        } else if (!isspace(input_str[i])){
            char_count++;
        }
        i++;
    }
    return camel_case;
}

char** concatenate_and_capitalize(char** camel_case, const char* input_str) {
    int i = 0;
    int sent_num = 0;
    int char_count = 0;
    bool cap = false;
    bool first_word = false;
    
    while(input_str[i]) {
        if (camel_case[sent_num] == NULL) break;
        else if (isspace(input_str[i])) {
            cap = true;
        } else if (ispunct(input_str[i])) {
            sent_num++;
            cap = false;
            first_word = true;
            char_count = 0;
        } else {
            char c;
            if (cap && !first_word && isalpha(input_str[i])) {
                c = toupper(input_str[i]);
            } else if (isalpha(input_str[i])) {
                c = tolower(input_str[i]);
            } else {
                c = input_str[i];
            }
            cap = false;
            first_word = false;
            camel_case[sent_num][char_count] = c;
            char_count++;
        }
        i++;
    }
    return camel_case;
}

char **camel_caser(const char *input_str) {
    // TODO: Implement me!
    if (input_str == NULL) {
        return NULL;
    }
    char** camel_case = allocate_sentences(input_str);
    camel_case = allocate_characters(camel_case, input_str);
    camel_case = concatenate_and_capitalize(camel_case, input_str);
    return camel_case;
}

void destroy(char **result) {
    // TODO: Implement me!
    int i = 0;
    while (result[i]) {
        free(result[i]);
        i++;
    }
    free(result);
    return;
}
