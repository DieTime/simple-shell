#ifndef SHELL_H
#define SHELL_H

#define DEFAULT_BUFF_SIZE 16
#define TOKENS_DELIMITERS " \t"

/**
 * Getting user input from shell

 * @return user-entered string
 */
char* readline();

/**
 * Parse string to tokens by delimiter symbols
 *
 * @param line - String with delimiter symbols
 * @return array with tokens
 */
char** split(char* line);

#endif //SHELL_H