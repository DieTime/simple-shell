#include <stdlib.h>
#include <stdio.h>

#include "shell.h"

char* readline() {
    int position  = 0;
    size_t buff_size = DEFAULT_BUFF_SIZE;

    int character;

    char* buffer  = (char*)malloc(sizeof(char) * buff_size);
    if (!buffer) {
        printf("[ERROR] Couldn't allocate buffer for reading!\n");
        return NULL;
    }

    character = getchar();
    while (character != '\0' && character != '\n') {
        buffer[position++] = (char)character;

        if (position >= buff_size) {
            buff_size *= 2;
            buffer = realloc(buffer, buff_size);
            if (!buffer) {
                printf("[ERROR] Couldn't reallocate buffer for reading!\n");
                return NULL;
            }
        }

        character = getchar();
    }
    buffer[position] = '\0';

    return buffer;
}