#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pwd.h>
#include <wait.h>

#include "shell.h"

tasks t = { .foreground = -1, .background = NULL, .cursor = 0, .capacity = 0 };

void display() {
    // Try get and print username with color
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw != NULL) {
        printf("%s%s%s:", PRIMARY_COLOR, pw->pw_name, RESET_COLOR);
    }

    // Try get and print current directory with color
    char cwd[MAX_DIRECTORY_PATH];
    if (getcwd(cwd, MAX_DIRECTORY_PATH) != NULL) {
        printf("%s%s%s", SECONDARY_COLOR, cwd, RESET_COLOR);
    }

    // Print end of shell info
    printf("# ");
}

char* readline() {
    size_t position  = 0;
    size_t buff_size = DEFAULT_BUFF_SIZE;

    int character;

    // Allocating memory for line
    char* buffer  = (char*)malloc(sizeof(char) * buff_size);
    if (buffer == NULL) {
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

            buffer = (char*)realloc(buffer, buff_size);
            if (buffer == NULL) {
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
    char** tokens = (char**)malloc(sizeof(char*) * buff_size);
    if (tokens == NULL) {
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

            tokens = (char**)realloc(tokens, buff_size * sizeof(char*));
            if (tokens == NULL) {
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
    } else if (strcmp(args[0], "bg") == 0) {
        return bg();
    } else {
       return launch(args);
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
        "  bg   \n"
        "  quit \n\n"

        "Use the man command for information on other programs.\n"
    );

    return 1;
}

int quit() {
    kill_all();
    return EXIT;
}

int bg() {
    // Temp background task variable
    bg_task* bt;

    for (size_t i = 0; i < t.cursor; i++) {
        // Store task in temp variable
        bt = &t.background[i];

        // Print info about task
        printf(
            "[%zu]%s cmd: %s%s;%s pid: %s%d; %s"
            "state: %s%s;%s timestamp: %s%s", i,
            SECONDARY_COLOR, RESET_COLOR, bt->cmd,
            SECONDARY_COLOR, RESET_COLOR, bt->pid,
            SECONDARY_COLOR, RESET_COLOR, bt->finished ? "finished" : "active",
            SECONDARY_COLOR, RESET_COLOR, bt->timestamp
        );
    }

    return CONTINUE;
}

int launch(char** args) {
    pid_t pid;        // Fork process id
    int   background; // Is background task

    // Checking if task is background
    background = is_background(args);

    // Create child process
    pid = fork();

    // If created failure log error
    if (pid < 0) {
        printf("[ERROR] Couldn't create child process!\n");
    }
    // Child process
    else if (pid == 0) {
        // Try launch task
        if (execvp(args[0], args) == -1) {
            printf("[ERROR] Couldn't execute unknown command!\n");
        }
        exit(1);
    }
    // Parent process
    else {
        if (background) {
            // Try add background task to array
            if (add_background(pid, args[0]) == -1) {
                // Kill all processes and free
                // memory before exit
                kill_all();

                return EXIT;
            }

            // Add signal for removing background task
            // from array on task finished
            signal(SIGCHLD, mark_background);
        } else {
            // Set foreground task as pid
            set_foreground(pid);

            // Add signal for killing child on ctrl-c
            signal(SIGINT, kill_foreground);

            // Handle probably error
            if (waitpid(pid, NULL, WUNTRACED) == -1) {
                // Print problem
                printf("[ERROR] Couldn't track child process!\n");

                // Kill all processes and free
                // memory before exit
                kill_all();

                return EXIT;
            }

            // Set foreground to default value
            t.foreground = -1;

            // Reset signal
            signal(SIGINT, SIG_IGN);
        }
    }

    return CONTINUE;
}

int is_background(char** args) {
    // Current position in array
    int last_arg = 0;

    // Finding last arg in array
    while (args[last_arg + 1] != NULL) {
        last_arg += 1;
    }

    // Checking if task is background`
    if (strcmp(args[last_arg], "&") == 0) {
        // Remove '&' token for future executing
        args[last_arg] = NULL;

        // Return true
        return 1;
    }

    // Return false if  '&' wasn't founded
    return 0;
}

void set_foreground(pid_t pid) {
    t.foreground = pid;
}

void kill_foreground() {
    // Kill process
    kill(t.foreground, SIGTERM);

    // Set pid to default value
    t.foreground = -1;

    printf("\n");
}

int add_background(pid_t pid, char* name) {
    // Temp background task variable
    bg_task* bt;

    // If end of free space in background array - increase size
    if (t.cursor >= t.capacity) {
        t.capacity = t.capacity * 2 + 1;
        t.background = (bg_task*)realloc(t.background, sizeof(bg_task) * t.capacity);
        if (t.background == NULL) {
            printf("[ERROR] Couldn't reallocate buffer for background tasks!\n");
            return -1;
        }
    }

    // Print info about process start
    printf("[%zu] started.\n", t.cursor);

    // Save task in temp variable
    bt = &t.background[t.cursor];

    // Save process info in array
    bt->pid = pid;
    bt->finished = 0;

    time_t timestamp = time(NULL);
    bt->timestamp = ctime(&timestamp);

    bt->cmd = strdup(name);

    // Move cursor right
    t.cursor += 1;

    return 0;
}

void mark_background() {
    // Temp background task variable
    bg_task* bt;

    // Get process id of ended process
    pid_t pid = wait(NULL);

    // Handle error if background process doesn't tracking
    if (pid == -1 && t.foreground == -1) {
        printf("[WARNING] Couldn't get id of ended process!\n");
        return;
    }

    // Search and remove process form background tasks array
    for (size_t i = 0; i < t.cursor; i++) {
        // Place task to temp variable
        bt = &t.background[i];

        if (bt->pid == pid) {
            // Print info about process end
            printf("[%zu] finished.\n", i);

            // Set new state for background process
            bt->finished = 1;

            break;
        }
    }
}

void kill_all() {
    // Temp background task variable
    bg_task* bt;

    // Kill foreground process
    if (t.foreground != -1) {
        kill_foreground();
    }

    for (size_t i = 0; i < t.cursor; i++) {
        // Place task to temp variable
        bt = &t.background[i];

        // Kill process if active
        if (!bt->finished) {
            kill(bt->pid, SIGTERM);
        }

        // Free memory for command name
        free(bt->cmd);
    }
}