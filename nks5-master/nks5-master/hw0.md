Chapter 1:

1.
```
#include <stdio.h>
#include <unistd.h>

int main() {
	write(1, "Hello, my name is Nachiket", 26);
	return 0;
}
```

2.
```
#include <stdio.h>
#include <unistd.h>

void write_triangle(int n) {
	int i, j;
	for (i = 1; i <= n; i++) {
		for (j = 0; j < i; j++) {
			write(1, "*", 1);
		}
		write(1,"\n", 1);
	}
}

int main() {
	write_triangle(5);
}
```

3.
```
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main() {
	mode_t mode = S_IRUSR | S_IWUSR;
	int filedes = open("hello_world.txt", O_CREAT | O_TRUNC | O_RDWR, mode);
	write(filedes, "Hello, my name is Nachiket", 26);
	close(filedes);
	return 0;
}
```

4. 
```
int main() {
	mode_t mode = S_IRUSR | S_IWUSR;
	close(1);
	int filedes = open("hello_world.txt", O_CREAT | O_TRUNC | O_RDWR, mode);
	printf("Hello, my name is Nachiket\n");
	close(filedes);
	return 0;
}
```

5. write() takes a pointer to memory and reads a specified number of bytes to print out.
   printf() has a buffer, which only calls write() when the buffer is full or a new line
   is specified. 

Chapter 2:

1. A byte is at least 8 bits.
2. There is one byte in a char
3. int is 4 bytes, double is 8 bytes, float is 4 bytes, long is 8 bytes,
and long long is 8 bytes.
4. data+2 is at the address 0x7fbd9d48
5. C takes the data pointer and adds 3 units of integer memory to it. 
6. char* ptr = "hello" is a constant that can only be read, and the hardware is 
sophisticated enough to know which parts of memory are constant. *ptr = 'J' attempts 
to change this part of constant memory, which causes a segmentation fault. 
7. 12
8. 5
9. "He"
10. float

Chapter 3:

1. argc contains the length of argv, but another way to find arv's length 
is to loop through argv until the value is null :
	int count = 0; 
	while(argv[++count] != NULL); 
2. argv[0] represents the name of the program executable
3. the stack
4. sizeof(ptr) is 8, since each pointer is 8 bytes.
   sizeof(array) is 6, since "Hello" is 5 character bytes and the null byte. 
5. a stack

Chapter 4:
1. Put the data outside the function but below the include headers
and make the type static so it lasts for the lifetime of the program. 
2. In the stack, allocation happens on contiguous blocks of memory
automatically but in heap it is done programatically. Accessing the heap
takes more time than accessing the stack.
3. The text segment and data segment. 
4. free
5. The program used up all the heap memory
6. time() takes a pointer to time_t while ctime() takes a const pointer to time_t.
ctime() has formatting and uses static storage, but using multiple times causes 
the storage to be overwritten. 
7. When you call free(ptr), it tells the heap that you've freed some memory
that it uses for its own book keeping. In trying to free(ptr) again, it can
confuse the book keeping for the pointers that the heap is using itself. 
8. It continues using memory after it's been freed. If it is going to be reused
by some other program that is about to call malloc, we shouldn't expect anything that
was previously stored in there to be valid. 
9. After you call free(ptr), say ptr = NULL; to ensure that ptr doesnt point to any
invalid memory.
10. 
```
struct Person {
	char[] name;
	int age;
	*Person *persons[];
}
typedef struct Person person_t;
```

11.
```
int main() {
    person_t* ptr= (person_t*) malloc(sizeof(person_t));
    ptr->name = "Agent Smith";
    ptr->age = 128;
    person_t* ptr1 = (person_t*) malloc(sizeof(person_t));
    ptr1->name = Sonny Moore";
    ptr1->age = 256;
    free(ptr);
    free(ptr1);
}
```

12.
```
person_t* create(char[] *a_name, int an_age) {
    person_t* person = (person_t*)malloc(sizeof(person_t));
    person->name= strdup(a_name);
    person->age = an_age;
    return person;
}
```

13.
```
void destroy(person_t* p) {
   free(a_name);
   memset(p,0,sizeof(person_t));
   free(p);
}
```

Chapter 5:
1. getchar(), putchar(), printf()
2. gets() might accept input that is 
too long for it, which could corrupt
other variable values
3. 
```
int main() {
	char* data = "Hello 5 World";
	char str[6];
	char str1[6];
	int val = -42;
	sscanf(data, "%s %d %s", str, &val, str1);
	return EXIT_SUCCESS;
}
```
4. #define _GNU_SOURCE
5. 
```
int main() {
	char *buffer = NULL;
	size_t capacity = 0;
	getline(&buffer, &capacity, stdin);
	printf("%s\n", result);
	while( buffer ) {
		ssize_t result = getline( &buffer, &capacity, stdin); 
		printf("%s\n", result);
	}
	free(buffer);
	return EXIT_SUCCESS;
}
```
C Development:
1. -g
2. Using target specific variable values like $(CXX) or $(CC) 
in all compile commands
3. The rule must be indented by a tab
4. git commit saves the changes to the local repository.
sha allows one to keep a record of what changes were made when 
and by who
5. allow you to read the history of everything that happens 
to a repository
6. git status displays the current state of the working directory.
gitignore files determine which files and directories to ignore before 
committing, so git status doesnt display their state. 
7. git push uploads repository content to a remote repository and updates 
the remote repository.
8. It means that there have been other commits pushed to the remote repository
that differ from your commits, which can usually be solved with a git pull.
