#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* GetFileName(char* path) {
    static char buf[DIRSIZ + 1];
    char* p;

    for (p = path + strlen(path); p >= path && *p != '/'; p--) {
        ;
    }

    p++;

    if (strlen(p) >= DIRSIZ){
        return p;
    }

    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void Find(char* path, char* file_name) {
    struct dirent de;
    struct stat st;
    int fd;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

    switch(st.type) {
        case T_FILE: {
            char* current_file_name = GetFileName(path);
            if (strcmp(current_file_name, file_name) == 0) {
                printf(path);
            }
            break;
        }
        case T_DIR: {
            char buf[512];
            char* p;
            if (strlen(path) + 1 + DIRSIZ + 1 >  sizeof buf){
                printf("find: path too long\n");
                break;
            }

            // printf("current path is %s\n", path);
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0){
                    continue;
                }

                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                // printf("current name is %s\n", p);
                if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0) {
                    continue;
                }

                if (stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", p);
                    continue;
                }

                if (st.type == T_DIR) {
                    Find(buf, file_name);
                }

                if (strcmp(p, file_name) == 0) {
                    printf("%s\n", buf);
                }
            }
            break;
        }
    }

    close(fd);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(2, "usage: find [dir] [file]\n");
        exit(1);
    }

    Find(argv[1], argv[2]);
    exit(0);
}