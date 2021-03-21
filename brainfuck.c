#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define TAPE_SIZE 30000

void interpret(char* input) {
	// initialize tape
	unsigned char tape[TAPE_SIZE] = {0};
	 
	// set the pointer to point at the left-most cell of the tape
	unsigned char* ptr = tape;
    char currentChar;
    size_t i;
    size_t loop;

    for (i = 0; input[i] != 0; i++) {
        currentChar = input[i];
        if (currentChar == '>') {
            ++ptr;
        } else if (currentChar == '<') {
            --ptr;
        } else if (currentChar == '+') {
            ++*ptr;
        } else if (currentChar == '-') {
            --*ptr;
        } else if (currentChar == '.' ) {
            putchar(*ptr);
        } else if (currentChar == ',') {
            *ptr = getchar();
        } else if (currentChar == '[') {
            continue;
        } else if (currentChar == ']' && *ptr) {
            loop = 1;
            while (loop > 0) {
                currentChar = input[--i];
                if (currentChar == '[') {
                    loop--;
                } else if (currentChar == ']') {
                    loop++;
                }
            }
        }
    }
}

bool syntaxCheck(char* input)
{
    size_t oc = 0;
    size_t cc = 0;
    char* chars = "+-<>[],.";

    for (size_t i = 0; input[i] != 0; i++) {
        if (!strchr(chars, input[i])) {
        	printf("Invalid Character(s) Detected\n");
        	return 0;
        }
        if (input[i] == '[') {
            oc++;
        }
        else if (input[i] == ']') {
            cc++;
        }
    }
    if (oc != cc) {
        printf("Unclosed Loop(s)\n");
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2)
    {
        printf("Incorrect Argument Number\n");
        return -1;
    }
    FILE *fp = fopen(argv[1], "r");
    if(fp==NULL) { perror("fopen"); return -1; }

    // get length of file
    fseek(fp, 0L, SEEK_END);
    size_t sz = ftell(fp);
    rewind(fp);

    // read file into char buffer
    char buffer[sz];
    memset(buffer, 0, sz * sizeof(char));
    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fp) != -1) {
        strcat(buffer, line);
    }
    char* ptr = buffer;
    size_t j, n = strlen(ptr);
    for (size_t i = j = 0; i < n; i++) {
        if (ptr[i] != '\n') {
            ptr[j++] = ptr[i];
        }
    }
    ptr[j] = '\0';

    // simple syntax check
    if (syntaxCheck(ptr) == 0) {
        return -1;
    }

    // run code
    interpret(ptr);
    return 0;
}