#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

int child();

int main()
{
    int clone_flags = CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWNET | CLONE_NEWIPC;
    pid_t container = clone(child, malloc(4096) + 4096, SIGCHLD | clone_flags, NULL);
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
    char *cmd[] = {"/bin/sh", NULL};
    sethostname("container", 9);
    chroot("alpine");
    chdir("/");
    execv("/bin/sh", cmd);
    return 0;
}