#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void err_sys(char *str)
{
    printf("%s\n", str);
    exit(3);
}

int main(int argc, char *argv[])
{
    int fd, i, j, sfd, n;
    int blksize;
    struct stat file_info;
    if  (argc != 2) {
        err_sys("Please provide a file to be splited!");
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        printf("%s open error!\n", argv[1]);
    }
    printf("open %s successfully.\n", argv[1]);

    if (fstat(fd, &file_info) < 0)
        err_sys("fstat error!");
    blksize = file_info.st_blksize;
    printf("stat ok\nblksize is %d\nfile size %d Bytes\n", blksize, file_info.st_size);

    if ( file_info.st_size/(1024*1024) < 1 )
        err_sys("file lower 1MB ,do not need split.");

    char buf[blksize];
    char *fname = (char *)malloc(2048);
    for (i=0; i != -1; i++) {
        sprintf(fname, "%s_%d", argv[1], i);
        printf("creat split file: %s\n", fname);
        if ((sfd = creat(fname, FILE_MODE)) == -1) {
            printf("creat %s error!\n", fname);
            exit(1);
        }
        printf("creat %s sucessfully.\n", fname);
        for (j = 0; j<= 256; j++) {
            n = read(fd, buf, blksize);
            if ( n < 0) {
                err_sys("read error");
            } else if (n == 0) {
                i = -1;
                break;
            }
            if (write(sfd, buf, n) != n)
                err_sys("write error");
        }
        close(sfd);
        printf("%s write finish\n", fname);
        if (i == -1)
            break;
    }
    printf("%s split finish.\n", argv[1]);
}
