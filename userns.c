#define _GNU_SOURCE
#include <sys/capability.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//sudo apt-get install libcap-dev

#define errExit(msg) do {perror(msg); exit (EXIT_FAILURE);} while (0)

static int childFunc(void *arg){
    cap_t caps;

    printf("We are in the child process\n");
    sethostname("NewNamedHost",12);

    for(;;){
        printf("Uid inside = %ld; GID insid = %ld;", (long)geteuid(), (long) getegid());
        caps = cap_get_proc();
        printf("capabilities: %s\n", cap_to_text(caps, NULL));

        if (arg == NULL)
            break;
        sleep(5);
    }
    return 0
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]){
    pid_t pid;

    pid = clone (childFunc, child_stack+STACK_SIZE,
            CLONE_NEWUSER | CLONE_NEWUTS | SIGCHLD, argv[1]);
    if (pid == -1)
        errExit("clone");
    if (waitpid(pid,NULL, 0) == -1)
        errExit("waitpid");
    exit(EXIT_SUCCESS);
}
