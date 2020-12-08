#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

int main() {
    char*  line;
    char** args;
    int    status;

    do {
        line = readline();
        if (line == NULL) {
            exit(1);
        }

        args = split(line);
        if (args == NULL) {
            free(line);
            exit(2);
        }

        status = execute(args);

        free(line);
        free(args);
    } while (status);

    return 0;
}