#ifndef SHELL_H
#define SHELL_H

#define DEFAULT_BUFF_SIZE 16
#define TOKENS_DELIMITERS " \t"
#define CONTINUE 1
#define EXIT 0

/**
 * Display left shell info like:
 * username and current working directory
 */
void display();

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

/**
 * Calling builtin functions: cd, help, quit
 * Or launch linux terminal commands
 *
 * @param args - tokens for launch terminal
 * commands or cd builtin function
 *
 * @return status for main loop CONTINUE or EXIT
 */
int execute(char** args);

/**
 * Change current working directory
 *
 * @param args - tokens for switching directory
 * @return status for main loop - CONTINUE
 */
int cd(char** args);

/**
 * Print information about shell
 * and builtin functions
 *
 * @return status for main loop - CONTINUE
 */
int help();

/**
 * Terminate shell
 *
 * @return status for main loop - EXIT
 */
int quit();

#endif //SHELL_H