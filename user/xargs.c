#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define XARG_MAX_ARG_LENGTH 256
#define XARG_FILE_STDIN 0
#define XARG_FILE_STDOUT 1
#define XARG_FILE_STDERR 2

int Dup2(int old_fd, int new_fd){
    if (old_fd == new_fd){
        return new_fd;
    }

    // close the new fd if it's open
    close(new_fd);
    int temp_fd;

    while ((temp_fd = dup(old_fd)) != new_fd) {
        if (temp_fd == -1){
            printf("Dup2 fails\n");
            exit(1);
        }
    }

    return new_fd;
}


int main(int argc, char** argv) {
    if (argc < 3) {
        printf("xarg requires at least three args\n");
        exit(1);
    }

    char buf[512];
    int n;
    printf("being to read\n");
    while((n = read(XARG_FILE_STDIN, buf, sizeof(buf))) >= 0) {
        printf("we read: %s\n", buf);
        exit(0);
    }
    printf("end to read\n");

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
        Dup2(pipe_fd[1], XARG_FILE_STDOUT);
        close(pipe_fd[1]);
        
        char* first_part_argv [MAXARG];
        for (int i = 1 ; i <= argc; i++) {
            if (i == argc) {
                first_part_argv[i - 2][0] = '\0';
            }else {
                strcpy(first_part_argv[i - 2], argv[i]);
            }
        }

        
        for (int i = 0; strcmp(first_part_argv[i], "\0") != 0 ; i++) {
            printf("the arg is %s\n", first_part_argv[i]);
        }

        exec(argv[1], first_part_argv);
        // should not reach here;
        exit(1);
    } else {
        // parent process
    }

    exit(0);
}