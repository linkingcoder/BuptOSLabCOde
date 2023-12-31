#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

#define STACK_SIZE (1024 * 1024)

int child_function(void *arg) {
    printf("Child process ID: %d\n", getpid());
    printf("Argument passed to child: %s\n", (char *) arg);
    sleep(2);
    printf("Child process exiting...\n");
    return 0;
}

int main() {
    char *stack;
    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int flagmode;
    int flag;
    scanf("%d", &flagmode);
    switch (flagmode) {
        case 1:
            flag = CLONE_FS;
            break;
        case 2:
            flag = CLONE_VFORK;
            break;
        case 3:
            flag = CLONE_FILES;
            break;
        case 4:
            flag = CLONE_NEWPID;
            break;
    }
            char *arg = "Hello from the parent!";
            pid_t pid = clone(child_function, stack + STACK_SIZE, flag, arg);
            if (pid == -1) {
                perror("clone");
                exit(EXIT_FAILURE);
            }

            printf("Parent process ID: %d\n", getpid());
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Parent working directory: %s\n", cwd);
            } else {
                perror("getcwd");
                exit(EXIT_FAILURE);
            }
            printf("Parent waiting for child to exit...\n");
            if (waitpid(pid, NULL, 0) == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            printf("Parent process exiting...\n");
            return 0;
}