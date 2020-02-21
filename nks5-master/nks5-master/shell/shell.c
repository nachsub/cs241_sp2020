/**
 * Shell
 * CS 241 - Spring 2020
 */
#include "format.h"
#include "shell.h"
#include "vector.h"
#include "sstring.h"
#include <signal.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <stdlib.h>


typedef struct process {
    char *command;
    pid_t pid;
} process;

static vector* history_vector;
static FILE* history_file;

static char* script_file;
static FILE* file;

static char *cwd;
static char currentPath[PATH_MAX];

static int execute_script = 0;
static int write_history = 0;
static int status = 0;
static int logical_op = 0;

void sigint_handler(int signum) {}
//./shell -h <history_file>
void history(char* history_file_name) {
    history_file = fopen(history_file_name, "a+");
    char* lineptr = NULL;
    size_t n = 0;
    while(getline(&lineptr, &n, history_file) != -1) {
        vector_push_back(history_vector, lineptr);
    }
    if(access(history_file_name, R_OK | F_OK) == 1) {
        print_history_file_error();
    }
    write_history = 1;
}

void script(char* read_file) {
    script_file = malloc(1 + strlen(read_file));
	script_file = strcpy(script_file, read_file);
	if((file = fopen(read_file, "rw"))){}
	else print_script_file_error();
	execute_script = 1;
}

//ltrim rtrim and trim provided by stack overflow
//https://stackoverflow.com/questions/656542/trim-a-string-in-c
char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s) {
    return rtrim(ltrim(s)); 
}

void cd(char* input) {
    vector_push_back(history_vector, (void*)input);
    input = input + 3;
   
    status = chdir(input);
    if (status < 0) {
        print_no_directory(input);
    }
    cwd = getcwd(currentPath, PATH_MAX + 1);
}

void handle_external(char* input) {
    vector_push_back(history_vector, input);

    char* input_copy = strdup(input);
    sstring* str = cstr_to_sstring(input);
    vector* v = sstring_split(str, ' ');
    char** args = malloc(vector_size(v) * sizeof(char*));
    if (vector_size(v) != 0) {
        for (size_t i = 0; i < vector_size(v); i++) {
            args[i] = (char*)(vector_get(v, i));
        }
    }
    fflush(stdout);
    fflush(stdin);
    fflush(history_file);
    fflush(file);

    pid_t id = fork();
    if (id == 0) {
        execvp(args[0], args);
        print_exec_failed(input_copy);
        exit(1);
    } else if (id > 0) {
        print_command_executed(id);
        waitpid(id, &status, 0);
    } else {
        print_fork_failed();
    }
   
    free(args);
	sstring_destroy(str);
    vector_destroy(v);
	free(input_copy);
}

void print_history(char* input) {
    for (size_t i = 0; i < vector_size(history_vector); i++) {
        print_history_line(i, vector_get(history_vector, i));
    }
}

//#<n>
void print_n(char* input) {
    input++;
    size_t len = strlen(input);
    if (!len)
        print_invalid_index();
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(input[i])) {
            print_invalid_index();
        }
    }
    
    if ((size_t)atoi(input) < vector_size(history_vector)) {
        char* c = (char*)vector_get(history_vector, (size_t)atoi(input));
        print_command(c);
        if (strlen(c)) {
            if(!strncmp(c, "cd", 2)) 
                cd(c);
	        else if(!strncmp(c, "!history", 8)) 
                print_history(c);
		    else if(!strncmp(c, "#", 1)) 
                print_n(c);
            else if(!strncmp(c, "exit", 4))
                exit(1);
		    else handle_external(c);
        }
    }
    else {
        print_invalid_index();
    }
}

//!<prefix>
void print_prefix(char* input) {
    input++;
    if (vector_size(history_vector) < 1) {
         print_no_history_match();
         return;
    }
    for (int i = vector_size(history_vector) - 1; i >= 0; i--) {
        if (!strncmp(input,(char*)vector_get(history_vector,(size_t)i),strlen(input))) {
            char* c = vector_get(history_vector,(size_t)i);
            print_command(c);
            if (strlen(c)) {
                if(!strncmp(c, "cd", 2)) 
                    cd(c);
	            else if(!strncmp(c, "!history", 8)) 
                    print_history(c);
		        else if(!strncmp(c, "#", 1)) 
                    print_n(c);
		        else if(!strncmp(c, "!", 1)) 
                    print_prefix(input);
                else if(!strncmp(c, "exit", 4))
                    exit(1);
		        else handle_external(c);
            }
            return;
        }
    }
    print_no_history_match();
}


void and(char* input) {
    logical_op = 1;
    char* new_input = strdup(input);
    char* x = strtok(new_input, "&&"); 
    char* y = strstr(input, "&&");
    y += 2;
    if (strlen(x)) {
        x = trim(x);
        if(!strncmp(x, "cd", 2)) 
            cd(x);
        else if(!strncmp(x, "!history", 8)) 
            print_history(x);
        else if(!strncmp(x, "#", 1)) 
            print_n(x);
        else if(!strncmp(x, "!", 1)) 
            print_prefix(x);
        else if(!strncmp(x, "exit", 4))
            exit(1);
        else handle_external(x);
    }
    if (strlen(y) && !status){
        y = trim(y);
        if(!strncmp(y, "cd", 2)) 
            cd(y);
	    else if(!strncmp(y, "!history", 8)) 
            print_history(y);
	    else if(!strncmp(y, "#", 1)) 
            print_n(y);
	    else if(!strncmp(y, "!", 1)) 
            print_prefix(y);
        else if(!strncmp(y, "exit", 4))
            exit(1);
	    else handle_external(y);
    }
}

void or(char* input) {
    logical_op = 1;
    char* new_input = strdup(input);
    char* x = strtok(new_input, "||"); 
    char* y = strstr(input, "||");
    y += 2;
    if (strlen(x)) {
        x = trim(x);
        if(!strncmp(x, "cd", 2)) 
            cd(x);
        else if(!strncmp(x, "!history", 8)) 
            print_history(x);
        else if(!strncmp(x, "#", 1)) 
            print_n(x);
        else if(!strncmp(x, "!", 1)) 
            print_prefix(x);
        else if(!strncmp(x, "exit", 4))
            exit(1);
        else handle_external(x);
    }
    if (strlen(y) && status){
        y = trim(y);
        if(!strncmp(y, "cd", 2)) 
            cd(y);
	    else if(!strncmp(y, "!history", 8)) 
            print_history(y);
	    else if(!strncmp(y, "#", 1)) 
            print_n(y);
	    else if(!strncmp(y, "!", 1)) 
            print_prefix(y);
        else if(!strncmp(y, "exit", 4))
            exit(1);
	    else handle_external(y);
    }
}

void separator(char* input) {
    logical_op = 1;
    char* new_input = strdup(input);
    char* x = strtok(new_input, ";"); 
    char* y = strstr(input, ";");
    y += 1;
    if (strlen(x)) {
        x = trim(x);
        if(!strncmp(x, "cd", 2)) 
            cd(x);
        else if(!strncmp(x, "!history", 8)) 
            print_history(x);
        else if(!strncmp(x, "#", 1)) 
            print_n(x);
        else if(!strncmp(x, "!", 1)) 
            print_prefix(x);
        else if(!strncmp(x, "exit", 4))
            exit(1);
        else handle_external(x);
    }
    if (strlen(y)){
        y = trim(y);
        if(!strncmp(y, "cd", 2)) 
            cd(y);
	    else if(!strncmp(y, "!history", 8)) 
            print_history(y);
	    else if(!strncmp(y, "#", 1)) 
            print_n(y);
	    else if(!strncmp(y, "!", 1)) 
            print_prefix(y);
        else if(!strncmp(y, "exit", 4))
            exit(1);
	    else handle_external(y);
    }
}


void handle_any_command(char* input) {
    if (strlen(input)) {
        if(!strncmp(input, "cd", 2)) 
            cd(input);
	    else if(!strncmp(input, "!history", 8)) 
            print_history(input);
		else if(!strncmp(input, "#", 1)) 
            print_n(input);
		else if(!strncmp(input, "!", 1)) 
            print_prefix(input);
        else if(!strncmp(input, "exit", 4))
            exit(0);
		else handle_external(input);
    }
}

void handle_logical_operators(char* input) {
    if (strlen(input)) {
        if (strstr(input, "&&") && !strncmp(strstr(input, "&&"),"&&", 2))
            and(input);
        else if (strstr(input, "||") && !strncmp(strstr(input, "||"),"||", 2))
            or(input);
        else if (strstr(input, ";") &&!strncmp(strstr(input, ";"), ";", 1)) 
            separator(input);
    }
}


int shell(int argc, char *argv[]) {
    // TODO: This is the entry point for your shell.
    history_vector = vector_create(string_copy_constructor, string_destructor,
                                    string_default_constructor);
    if (argc > 1) {
        int opt = getopt(argc, argv, "h:f:"); 
        while (opt != -1) {
            if (opt == 'h') {
                history(optarg);
                break;
            }
            if (opt == 'f') {
                script(optarg);
                break;
            }
            else {
                print_usage();
            }
            opt = getopt(argc, argv, "h:f:"); 
        }
    }
    signal(SIGINT, sigint_handler);

	cwd = getcwd(currentPath, PATH_MAX + 1);

    char* input_command;
    int eof = 0;
    int done = 0;
    size_t n = 0;
    while (!done) {
        print_prompt(cwd, getpid()); 
        char* remove_nl;
        if (!execute_script) {
            eof = getline(&input_command, &n ,stdin);
            remove_nl = input_command;
            if (input_command[strlen(input_command) - 1] == '\n') {
                input_command[strlen(input_command) - 1] = 0;
                remove_nl = input_command;
            }
            if (!strncmp(remove_nl, "exit", 4)) {
                vector_destroy(history_vector);
                exit(0);
            }
            if (write_history && strncmp(remove_nl, "exit", 4)
                && strncmp(remove_nl, "!history", 8) && strncmp(remove_nl, "!", 1)
                && strncmp(remove_nl, "#", 1)) {
                fprintf(history_file, "%s\n", remove_nl);
            }
        } else {
            eof = getline(&input_command, &n, file);
            remove_nl = input_command;
            if (input_command[strlen(input_command) - 1] == '\n') {
                input_command[strlen(input_command) - 1] = 0;
                remove_nl = input_command;
            }
        }
        if (eof == -1) {
            if (execute_script) {
                free(script_file);
                exit(0);
            }            
            break;
        }
        if (execute_script) {
            print_command(input_command);
        }
        handle_logical_operators(input_command);
        if (!logical_op)
            handle_any_command(input_command);
        logical_op = 0;
    }
    return 0;
}