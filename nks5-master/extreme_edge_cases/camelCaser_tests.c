/**
 * Extreme Edge Cases
 * CS 241 - Spring 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "camelCaser.h"
#include "camelCaser_tests.h"

int test_camelCaser(char **(*camelCaser)(const char *),
                    void (*destroy)(char **)) {
    // TODO: Implement me!
    char** null_case = (*camelCaser)(NULL);
    if (null_case) {
        return 0;
    }
    char** first_case = (*camelCaser)("/a.b'cDe[Hello]lol;ayy lmao,(69)2");
    char** first_solution = malloc(10*sizeof(char*));
    first_solution[0] = "";
	first_solution[1] = "a";
    first_solution[2] = "b";
    first_solution[3] = "cde";
    first_solution[4] = "hello";
    first_solution[5] = "lol";
    first_solution[6] = "ayyLmao";
    first_solution[7] = "";
	first_solution[8] = "69";
	first_solution[9] = NULL;
    if (sizeof(first_case)/sizeof(char*) != sizeof(first_solution)/sizeof(char*)) {
        free(first_solution);
        (*destroy)(first_case);
        return 0;
    }
    int i = 0;
    while (first_solution[i]) {
        if (strcmp(first_solution[i], first_case[i])) {
            free(first_solution);
            (*destroy)(first_case);
            return 0;
        }
        i++;
    }
    free(first_solution);
    (*destroy)(first_case);

    char** second_case = (*camelCaser)("\a");
    if (second_case[0] != NULL) {
        (*destroy)(second_case);
        return 0;
    }
    (*destroy)(second_case);

    char** third_case = (*camelCaser)("");
    if (third_case[0] != NULL) {
        (*destroy)(third_case);
        return 0;
    }
    (*destroy)(third_case);
    
    char** fourth_case = (*camelCaser)("-a-hello world**.. . ?Binky?boop>== = diddy ::boop@scooppitypoop.");
    char** fourth_solution = malloc(18*sizeof(char*));
    fourth_solution[0] = "";
    fourth_solution[1] = "a";
    fourth_solution[2] = "helloWorld";
    fourth_solution[3] = "";
    fourth_solution[4] = "";
    fourth_solution[5] = "";
    fourth_solution[6] = "";
    fourth_solution[7] = "";
    fourth_solution[8] = "binky";
    fourth_solution[9] = "boop";
    fourth_solution[10] = "";
    fourth_solution[11] = "";
    fourth_solution[12] = "";
    fourth_solution[13] = "diddy";
    fourth_solution[14] = "";
    fourth_solution[15] = "boop";
    fourth_solution[16] = "scooppitypoop";
    fourth_solution[17] = NULL;
    if (sizeof(fourth_case)/sizeof(char*) != sizeof(fourth_solution)/sizeof(char*)) {
        free(fourth_solution);
        (*destroy)(fourth_case);
        return 0;
    }
    i = 0;
    while (fourth_solution[i]) {
        if (strcmp(fourth_solution[i], fourth_case[i])) {
            free(fourth_solution);
            (*destroy)(fourth_case);
            return 0;
        }
        i++;
    }
    free(fourth_solution);
    (*destroy)(fourth_case);

    char** fifth_case = (*camelCaser)("In 1994 23 women were selected for the first all-female team to race for the America's Cup. They were: Stephanie Armitage-Johnson, Amy Baltzell, Shelley Beattie, Courtenay Becker, Sarah Bergeron, Merritt Carey, Sarah Cavanagh, Elizabeth Charles, Leslie Egnot, Christie Evans, Jennifer Isler, Diana Klybert");
    char** fifth_solution = malloc(17*sizeof(char*));
    fifth_solution[0] = "in199423WomenWereSelectedForTheFirstAll";
    fifth_solution[1] = "femaleTeamToRaceForTheAmerica";
    fifth_solution[2] = "sCup";
    fifth_solution[3] =  "theyWere";
    fifth_solution[4] =  "stephanieArmitage";
    fifth_solution[5] =  "johnson";
    fifth_solution[6] = "amyBaltzell";
    fifth_solution[7] =  "shelleyBeattie";
    fifth_solution[8] =  "courtenayBecker";
    fifth_solution[9] =  "sarahBergeron";
    fifth_solution[10] =  "merrittCarey";
    fifth_solution[11] =  "sarahCavanagh";
    fifth_solution[12] =  "elizabethCharles";
    fifth_solution[13] =  "leslieEgnot";
    fifth_solution[14] =   "christieEvans";
    fifth_solution[15] =  "jenniferIsler";
    fifth_solution[16] =  NULL;
    if (sizeof(fifth_case)/sizeof(char*) != sizeof(fifth_solution)/sizeof(char*)) {
        free(fifth_solution);
        (*destroy)(fifth_case);
        return 0;
    }
    i = 0;
    while (fifth_solution[i]) {
        if (strcmp(fifth_solution[i], fifth_case[i])) {
            free(fourth_solution);
            (*destroy)(fifth_case);
            return 0;
        }
        i++;
    }
    free(fifth_solution);
    (*destroy)(fifth_case);

    return 1;
}
