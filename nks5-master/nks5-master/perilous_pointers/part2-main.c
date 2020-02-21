/**
 * Perilous Pointers
 * CS 241 - Spring 2020
 */
#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
    // your code here
    first_step(81);

    int value = 132;
    second_step(&value);

    int x = 8942;
    int *y = &x;
    double_step(&y);

    char* c = malloc(sizeof(int) + 5);
    *(int*)(c + 5) = 15;
    strange_step(c);
    free(c);

    char* ch = malloc(4*sizeof(char));
    *(ch + 3) = 0;
    empty_step((void*)ch);

    *(ch + 3) = 'u';
    two_step((void*)ch, ch);
    free(ch);

    char* three = malloc(5*sizeof(char));
    three_step(three, three+2, three+4);
    free(three);

    char* eight = malloc(4*sizeof(char));
    eight[1] = -8;
    eight[2] = 0;
    eight[3] = 8;
    step_step_step(eight,eight,eight);
    free(eight);

    char a = 69;
    int b = 69;
    it_may_be_odd(&a, b);

    char str[] = "bloo ,CS241";
    tok_step(str);

    int *orange = malloc(sizeof(int));
    char *blue = (char*)orange;
    ((char*)blue)[0] = 1;
    *((int *)orange) = 513;
    the_end((void*) orange, (void*) blue);
    free(orange);
    return 0;
}
