
#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "format.h"
/**
 * Partner netid: jasneet2
 * Utilities Unleashed
 * CS 241 - Spring 2020
 */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_time_usage();
        return 0;
    }
    char ** child_proc = malloc(argc * sizeof(char*));
    for (int i = 0; i < argc - 1; i++) {
        child_proc[i] = argv[i + 1];
    }
    child_proc[argc - 1] = NULL;

    int id = fork();
    if (id == 0) {
        if (execvp(child_proc[0], child_proc)) {
            free(child_proc);
        }
    }
    if (id < 0) {
        free(child_proc);
        return 0;
    }
    
else {
    struct timespec init, stop;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &init); 
    waitpid(id, &i, WUNTRACED);
    clock_gettime(CLOCK_MONOTONIC, &stop);
    double timeup = (stop.tv_nsec - init.tv_nsec)/1000000000.0 + stop.tv_sec - init.tv_sec;
    if (!i)
        display_results(argv, timeup);
   }
   free(child_proc);
   return 0;
}
