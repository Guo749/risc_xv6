#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SIZE_INTEGER 4

void Filter(int pipe_fd[2]){
    close(pipe_fd[1]);
    int prime;
    if (read(pipe_fd[0], &prime, SIZE_INTEGER) != SIZE_INTEGER) {
        fprintf(2, "erorr  when read the prime number\n");
        exit(1);
    }

    printf("prime %d\n", prime);

    int num;
    int flag = read(pipe_fd[0], &num, SIZE_INTEGER);

    if (flag == 0) {
        exit(0);
    }

    int new_pipe_fd[2];
    pipe(new_pipe_fd);
    int pid = fork();
    if (pid == -1) {
        fprintf(2, "error when fork in child\n");
        exit(1);
    }else if(pid  == 0) {
        Filter(new_pipe_fd);
    }else {
        close(new_pipe_fd[0]);
        if (num % prime != 0) {
            write(new_pipe_fd[1], &num, SIZE_INTEGER);
        }
        while (read(pipe_fd[0], &num, SIZE_INTEGER) > 0) {
            if (num % prime != 0) {
                write(new_pipe_fd[1], &num, SIZE_INTEGER);
            }
        }

        close(new_pipe_fd[1]);
        close(pipe_fd[0]);
        wait(0);
    }

    exit(0);
}

int main(int argc, char** argv) {
    int pipe_fd[2];
    pipe(pipe_fd);

    int pid = fork();
    if (pid == -1) {
        fprintf(2, "error when fork\n");
        exit(1);
    } else if(pid == 0) {
        // Child Process
        Filter(pipe_fd);
    } else {
        // Parent Process
        close(pipe_fd[0]);
        for (int i = 2; i <= 35; i++){
            if (write(pipe_fd[1], &i, SIZE_INTEGER) != SIZE_INTEGER){
                fprintf(2, "error when write %d\n", i);
                exit(1);
            }
        }

        close(pipe_fd[1]);
        wait(0);
    }

    exit(0);
}