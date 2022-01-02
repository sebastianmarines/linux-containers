#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int child();

int main()
{
    int cloneFlags = CLONE_NEWUTS | CLONE_NEWPID;
    pid_t container = clone(child, malloc(4096) + 4096, SIGCHLD | cloneFlags, NULL);
    if (container == -1)
    {
        perror("clone");
        exit(1);
    }
    printf("Container pid: %d\n", container);
    waitpid(container, NULL, 0);
    return 0;
}

int child()
{
    int host = sethostname("container", 9);
    printf("Hello from child! PID=%d\n", getpid());
    execv("/bin/sh", NULL);
    return 0;
}