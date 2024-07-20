// exec_logger.c
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static int (*real_execve)(const char *filename, char *const argv[], char *const envp[]) = NULL;

int execve(const char *filename, char *const argv[], char *const envp[]) {
    if (!real_execve) {
        real_execve = dlsym(RTLD_NEXT, "execve");
    }

    // Log the command to a file
    FILE *log = fopen("/var/log/execve.log", "a");
    if (log) {
        fprintf(log, "Executed command: %s\n", filename);
        for (int i = 0; argv[i] != NULL; i++) {
            fprintf(log, "Arg[%d]: %s\n", i, argv[i]);
        }
        fclose(log);
    }

    return real_execve(filename, argv, envp);
}
