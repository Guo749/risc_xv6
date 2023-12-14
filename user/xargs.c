#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define XARG_MAX_ARG_LENGTH 256
#define XARG_FILE_STDIN 0
#define XARG_FILE_STDOUT 1
#define XARG_FILE_STDERR 2

void ExecuteInXargs(char** argv, char* args[MAXARG]) {
    int pipe_fd[2];
    if  (pipe(pipe_fd) == -1) {
        fprintf(2, "error when call pipe\n");
        exit(1);
    }

    int pid = fork();
    if (pid == -1) {
        fprintf(2, "error when fork\n");
        exit(1);
    } else if(pid == 0) {
        // child process
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        exec(argv[1], args);
        // Should not reach here
        exit(1);
    } else {
        // parent process
        wait(0);
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("xarg requires at least three args\n");
        exit(1);
    }

    char buf[512];
    char* args[MAXARG];
    args[0] = argv[1];
    while (1) {
        gets(buf, 512);
        if (buf[0] == '\0') {
            break;
        }

        buf[strlen(buf) - 1] = '\0';
        // printf("what we read: %s\n", buf);

        for (int i = 2; i < argc; i++){
            args[i - 1] = argv[i];
        }

        args[argc - 1] = buf;
        args[argc] = 0;

        ExecuteInXargs(argv, args);
    }


    exit(0);
}