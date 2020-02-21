/**
 * Partner netid: jasneet2
 * Utilities Unleashed
 * CS 241 - Spring 2020
 */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "format.h"
#include <ctype.h>
#include <sys/wait.h>


void setup(char* var) {
    char * key = strtok(var, "=");
    char * val = strtok(NULL, "=");
    if (key == NULL || val == NULL) {
        print_env_usage();
        exit(1);
    }

    if (val[0] == '%') {
        val++;
        char * k = val;
        while (*k) {
            if ((!isalpha(*k)) && (!isdigit(*k)) && (*k != '_')) {
                print_env_usage();
                exit(1);
            }
            k++;
        }
        val = getenv(val);
        if (val == NULL) {
            print_environment_change_failed();
            exit(1);
        }
    } else {
        char * k = val;
        while (*k) {
            if ((!isalpha(*k)) && (!isdigit(*k)) && (*k != '_')) {
                print_env_usage();
                exit(1);
            }
            k++;
        }
    }
    int stat = setenv(key, val, 1);
    if (stat == -1) {
        print_environment_change_failed();
        exit(1);
    }
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_env_usage();
        exit(1);
    }
    int i = 0;
    int found = 0;
    while (argv[i] != NULL) {
        if (!(strcmp(argv[i], "--"))) {
            found = 1;
            break;
        }
        i++;
    }
    if (found == 0) {
        print_env_usage();
        exit(1);
    }
    for (int j = 1; j < i; j++) {
        setup(argv[j]);
    }
    for (int j = 0; j < i + 1; j++) {
        argv++;
    }
    
    pid_t child = fork();
    if (child == -1) {
        print_fork_failed();
        exit(1);
    }
    if (child == 0) {
        execvp(argv[0], argv);
        print_exec_failed();
        exit(1);
    } else {
        int status;
        waitpid(child, &status, 0);

    }
       
    return 0;
}



