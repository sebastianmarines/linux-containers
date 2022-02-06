#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <string.h>

int child();

static void
update_map(char *mapping, char *map_file)
{
    int fd, j;
    size_t map_len; /* Length of 'mapping' */

    /* Replace commas in mapping string with newlines */

    map_len = strlen(mapping);
    for (j = 0; j < map_len; j++)
        if (mapping[j] == ',')
            mapping[j] = '\n';

    fd = open(map_file, O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open %s: %s\n", map_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (write(fd, mapping, map_len) != map_len)
    {
        fprintf(stderr, "write %s: %s\n", map_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    int child_args_len = argc < 2 ? 2 : argc;
    char *child_args[child_args_len];
    char map_path[PATH_MAX];
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
    snprintf(map_path, PATH_MAX, "/proc/%ld/uid_map",
             (long)container);
    char *uid_map = "0 1000 1";
    update_map(uid_map, map_path);
    waitpid(container, NULL, 0);
    snprintf(map_path, PATH_MAX, "/proc/%ld/gid_map",
             (long)container);
    char *gid_map = "0 1000 1";
    update_map(gid_map, map_path);
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