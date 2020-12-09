#ifndef SHELL_H
#define SHELL_H

#define DEFAULT_BUFF_SIZE  16
#define MAX_DIRECTORY_PATH 1024
#define TOKENS_DELIMITERS  " \t"
#define CONTINUE 1
#define EXIT 0

#define PRIMARY_COLOR   "\033[92m"
#define SECONDARY_COLOR "\033[90m"
#define RESET_COLOR     "\033[0m"

// Struct of background task
struct bg_task_t {
    pid_t   pid;           // Process id
    int     finished;      // Process state
    char*   timestamp;     // Process state
    char*   cmd;          // Command cmd
};
typedef struct bg_task_t bg_task;

// Struct of all tasks
struct tasks_t {
    pid_t    foreground; // Process id of foreground task
    bg_task* background; // Background tasks
    size_t   cursor;     // Cursor of background tasks
    size_t   capacity;   // Background array capacity
};
typedef struct tasks_t tasks;

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

/**
 * Print background tasks
 *
 * @return status for main loop - CONTINUE
 */
int bg();

/**
 * Executing linux shell commands
 *
 * @param args - tokens for executing
 * @return status for main loop - CONTINUE
 */
int launch(char** args);

/**
 * Background task checking
 *
 * @param args - tokens for launching
 * linux shell command
 *
 * @return true or false
 */
int is_background(char** args);

/**
 * Save foreground process id
 *
 * @param pid - child foreground process id
 */
void set_foreground(pid_t pid);

/**
 * Ctrl-C handler for killing child
 * foreground process
 */
void kill_foreground();

/**
 * Emplace background child process id
 * into background tasks
 *
 * @param pid - child background process id
 *
 * @return 0 on success, -1 on error
 */
int add_background(pid_t pid, char* name);

/**
 * Child process end handler for
 * removing task from background
 */
void mark_background();

/**
 * Killing all active processes
 * before EXIT execute status
 */
void kill_all();

#endif //SHELL_H