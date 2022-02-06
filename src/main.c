#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mount.h>

int child();

int main(int argc, char *argv[])
{
    int child_args_len = argc < 2 ? 2 : argc;
    char *child_args[child_args_len];
    if (argc < 2)
    {
        child_args[0] = "/bin/bash";
        child_args[1] = NULL;
    }
    else
    {
        for (int i = 0; i < argc; i++)
            child_args[i] = argv[i + 1];
        child_args[child_args_len] = NULL;
    }

    int clone_flags = CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWNET | CLONE_NEWIPC | CLONE_NEWUSER;
    pid_t container = clone(child, malloc(4096) + 4096, SIGCHLD | clone_flags, child_args);
    if (container == -1)
    {
        perror("clone");
        exit(1);
    }
    printf("Container pid: %d\n", container);
    waitpid(container, NULL, 0);
    return 0;
}

int child(void *arg)
{
    char **argv = arg;
    sethostname("container", 9);
    chroot("ubuntu");
    chdir("/");
    mount("proc", "proc", "proc", 0, "");
    execv(argv[0], argv);
    return 0;
}