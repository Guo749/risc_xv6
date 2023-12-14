#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(2, "usage: sleep 10\n");
        exit(1);
    }

    char* sleep_duration = argv[1];
    int sleep_time = atoi(sleep_duration);

    int result = sleep(sleep_time);
    exit(result == 0 ? 0 : 1);
}