#include <stdio.h>
#include "shell.h"

int main() {
    char* line = readline();
    printf("Entered line: '%s'\n", line);
    return 0;
}