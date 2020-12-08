#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

int main() {
    char* line = readline();
    printf("Entered line: '%s'\n", line);

    char** tokens = split(line);
    printf("Tokens in line: ");
    for (size_t i = 0; tokens[i] != NULL; i++) {
        printf("'%s' ", tokens[i]);
    }

    free(line);
    free(tokens);
    return 0;
}