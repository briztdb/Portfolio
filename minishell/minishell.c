#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>

#define BLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"

volatile sig_atomic_t interrupt = 0;
// Max Brizzio
// I pledge my honor that I have abided by the Stevens Honor System.

int readInput(char** cmd) { // takes cmd inputted into stdin and stores in cmd
    if(fgets(*cmd, 1024, stdin) == NULL) {
        if(interrupt == 0) { // if no interrupt signal received, print error message
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
        }
        exit(EXIT_FAILURE);
    }
    return 0;
}

int fillArgs(char* cmd, char** args) { // given a command, parse it and place each argument into args
    int count = 0;
    char* temp;
    temp = strtok(cmd, " "); // split cmd (tokenize)
    while(temp != NULL) {
        args[count] = temp;
        temp = strtok(NULL, " "); // continue tokenizing command
        count++;
    }
    for(int i = 0; i<strlen(args[count-1]); i++) {
        if(args[count-1][i] == '\n') { // parse for and replace newline operators from tokens with null terms
            args[count-1][i] = '\0';
        }
    }
    return count;
}

int exe(int argc, char* argv[]) { // executes commands using a child process given amt of args and each token
    pid_t pid = fork();
    if(pid < 0) fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
    if(pid == 0) { // child process
        char* bin = argv[0];
        if(strcmp(bin, "ls") != 0 || (strcmp(bin, "cd") != 0 && (strcmp(bin, "ls") == 0 && argc > 2) && strcmp(bin, "exit") != 0)) {
            if((execvp(bin, argv) == -1)) {
                fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    } else if(pid >= 1) {
        pid = wait(NULL); // wait to reap child
        if(pid == -1) {
            if(interrupt == 0) {
                fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
            }
        }
    }
    return EXIT_SUCCESS;
   
}

void sig_handler(int signo) { // signal handler
    puts("");
    interrupt = 1;
}
int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    if(sigaction(SIGINT, &sa, NULL) != 0) {
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    char* cmd = (char*)malloc(1024);
    if(cmd == NULL) {
        if(interrupt == 0) {
            fprintf(stderr, "Error: malloc() failed. %s.\n", strerror(errno));
        }
    }
    for(int i = 0; i < 1024; i++) { // fill buffer with null terms
        cmd[i] = '\0';
    }

    char* argv[1024];
    int argc;
    while(1) { // shell is active
        interrupt = 0;
        char cwd[PATH_MAX]; // getting current directory (dir in which program is called)
        if(getcwd(cwd, sizeof(cwd)) == NULL) { 
            fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
            free(cmd);
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "%s[%s]%s%s", BLUE, cwd, DEFAULT, "> ");
        readInput(&cmd);        // read input call
        if(interrupt == 0) {
            argc = fillArgs(cmd, argv);
        } else if(interrupt == 1) {
            for(int i = 0; i < argc; i++) {
                argv[i] = NULL;
            }
            interrupt = 0;
            continue;
        }

        uid_t uid = getuid();
        struct passwd* pwd = getpwuid(uid);
        char homedir[PATH_MAX];
        strcpy(homedir, pwd->pw_dir);
        
        if(strcmp(argv[0], "cd") == 0) {
            if((pwd = getpwuid(uid)) == NULL) {
                fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
                continue;
            } else if(argc > 2) { // too many args
                fprintf(stderr, "Error: Too many arguments to cd.\n");
                continue;
            } else if(argc <= 1 || strcmp(argv[1], "~") == 0) { // go to home directory
                if(chdir(homedir) == -1) {
                    fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", homedir, strerror(errno));
                    continue;
                }
            } else {
                if(chdir(argv[1]) == -1) {
                    fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", argv[1], strerror(errno));
                    continue;
                }
            }
        } else if(strcmp(argv[0], "exit") == 0) {
            free(cmd);
            exit(EXIT_SUCCESS);
        } else if(strcmp(argv[0], "ls") == 0) {
            pid_t pid = fork();
            if(pid == 0) {
                execlp(argv[0], argv[0], NULL);
            } else if(pid < 0) {
                fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
            } else {
                pid = wait(NULL); // wait to reap child
                if(pid == -1) {
                    if(interrupt == 0) {
                        fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
                    }
                }
            }
        } else {
            exe(argc, argv);
        }
        for(int i = 0; i < argc; i++) {
                argv[i] = NULL;
        }
    }

}

