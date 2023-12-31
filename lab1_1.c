#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STACK_SIZE 1024

int childProcess() {
    printf("I am the child process.\n");
    sleep(2);
    printf("Child PID: %d\n", getpid());
    exit(1);
}

void parentProcess(pid_t child_pid, int kills) {
    printf("I am the parent process.\n");
    sleep(5);
    printf("Parent PID: %d\n", getpid());
    printf("Child PID: %d\n", child_pid);
    if (kills != 0) {
        printf("Successfully kill child_process\n");
        sleep(2);
        kill(child_pid, SIGKILL);
    }
    sleep(3);
    execlp("ls", "ls", NULL);
    exit(0);
}

int main(int argc, char** argv) {
    pid_t pid;
    int killflag;
    int cloneflag;
    int status = 0;
    char *stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    scanf("%d", &killflag);
    scanf("%d", &cloneflag);
    if (cloneflag != 0) {
        pid = clone(&childProcess, (char *)stack + STACK_SIZE,SIGCHLD, NULL);
        if (pid == -1) {
            perror("clone");
            exit(EXIT_FAILURE);
        }
        printf("clone start!\n");
        free(stack);
        waitpid(pid, &status, 0);
        parentProcess(pid, killflag);
    } else {
        if ((pid = fork()) > 0) {
            wait(&status);
            parentProcess(pid, killflag);
        } else if (pid == 0) {
            childProcess();
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
