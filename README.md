# Containers

Download Ubuntu image `$ docker export $(docker create ubuntu) | tar -C ubuntu -xvf -`

## Namespaces

https://lwn.net/Articles/531114/

### CLONE_NEWUTS (Domain and hostname)

### CLONE_NEWPID (Processes)


### CLONE_NEWNS (Mount namespace)


### CLONE_NEWNET (Network namespace)


### CLONE_NEWIPC (IPC namespace)


### CLONE_NEWUSER (User namespace)

https://lwn.net/Articles/532593/

### CLONE_NEWCGROUP (Cgroup namespace)