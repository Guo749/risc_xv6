#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char** argv) {
    if (argc >= 2) {
        fprintf(2, "usage: pingpong\n");
        exit(1);
    }

    int pipe_fd[2];
    char buf[1] = "a";

    if (pipe(pipe_fd) == -1) {
        fprintf(2, "error when call pipe\n");
        exit(1);
    }

    int pid;
    pid = fork();

    if (pid == -1) {
        fprintf(2, "error when fork\n");
        exit(1);
    } else if(pid == 0) {
        read(pipe_fd[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(pipe_fd[1], buf, 1);
    } else {
        write(pipe_fd[1], buf, 1);
        read(pipe_fd[0], buf, 1);
        printf("%d: received pong\n", getpid());
    }

    exit(0);
}