/**
 * Vector
 * CS 241 - Spring 2020
 */
#include "sstring.h"

int main() {
    // TODO create some tests
    sstring* str = cstr_to_sstring("asd fa");
    sstring* str1 = cstr_to_sstring(" asd");
    printf("first test: %s\n", sstring_to_cstr(str));
    sstring* s = cstr_to_sstring("abc");
    sstring* s1 = cstr_to_sstring("def");
    int app = sstring_append(s, s1);
    int app1 = sstring_append(str, str1);
    printf("first test: %s\n", sstring_to_cstr(s));
    printf("second test: %s\n", sstring_to_cstr(str));
    printf("append test: %d\n", app);
    char* ch = sstring_slice(str, 4, 6);
    printf("third test: %s\n", ch);
    sstring_substitute(str, 0, "d", "friend");
    printf("fourth test: %s\n", sstring_to_cstr(str));
    vector* v = sstring_split(str, ' ');
    printf("fifth test: %s\n", vector_get(v, 0));
    sstring_destroy(str);
    sstring_destroy(str1);
    sstring_destroy(s);
    sstring_destroy(s1);
    vector_destroy(v);
    free(ch);
    return 0;
}
