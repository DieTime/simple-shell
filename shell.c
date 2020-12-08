#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "shell.h"

char* readline() {
    size_t position  = 0;
    size_t buff_size = DEFAULT_BUFF_SIZE;

    int character;

    // Allocating memory for line
    char* buffer  = (char*)malloc(sizeof(char) * buff_size);
    if (!buffer) {
        printf("[ERROR] Couldn't allocate buffer for reading!\n");
        return NULL;
    }

    // Process of reading
    character = getchar();
    while (character != '\0' && character != '\n') {
        // Emplace character to buffer
        buffer[position++] = (char)character;

        // If buffer free space ended - increase buffer
        if (position >= buff_size) {
            buff_size *= 2;

            buffer = realloc(buffer, buff_size);
            if (!buffer) {
                printf("[ERROR] Couldn't reallocate buffer for reading!\n");
                return NULL;
            }
        }

        // Read next character
        character = getchar();
    }

    // Emplace end of line to buffer
    buffer[position] = '\0';

    return buffer;
}

char** split(char* line) {
    size_t position  = 0;
    size_t buff_size = DEFAULT_BUFF_SIZE;

    char* token;

    // Allocate memory for tokens array
    char** tokens = malloc(sizeof(char*) * buff_size);
    if (!tokens) {
        printf("[ERROR] Couldn't allocate buffer for splitting!\n");
        return NULL;
    }

    // Tokenize process
    token = strtok(line, TOKENS_DELIMITERS);
    while (token != NULL) {
        // Emplace token to array
        tokens[position++] = token;

        // If array free space ended - increase array
        if (position >= buff_size) {
            buff_size *= 2;

            tokens = realloc(tokens, buff_size * sizeof(char*));
            if (!tokens) {
                printf("[ERROR] Couldn't reallocate buffer for tokens!\n");
                return NULL;
            }
        }

        // Getting next token
        token = strtok(NULL, TOKENS_DELIMITERS);
    }

    // Place NULL to the end of tokens array
    tokens[position] = NULL;

    return tokens;
}

int execute(char** args) {
    if (args[0] == NULL) {
        return CONTINUE;
    } else if (strcmp(args[0], "cd") == 0) {
        return cd(args);
    } else if (strcmp(args[0], "help") == 0) {
        return help();
    } else if (strcmp(args[0], "quit") == 0) {
        return quit();
    } else {
        printf("Linux terminal command not supported now...\n");
        return CONTINUE;
    }
}

int cd(char** args) {
    if (args[1] == NULL) {
        printf("[ERROR] Expected argument for \"cd\" command!\n");
    } else if (chdir(args[1]) != 0) {
        printf("[ERROR] Couldn't change directory to \"%s\"!\n", args[1]);
    }

    return CONTINUE;
}

int help() {
    printf(
        "Simple shell by Denis Glazkov\n"
        "Just type program names and arguments, and hit enter.\n\n"

        "Built in functions:\n"
        "  cd   \n"
        "  help \n"
        "  quit \n\n"

        "Use the man command for information on other programs.\n"
    );

    return 1;
}

int quit() {
    return EXIT;
}