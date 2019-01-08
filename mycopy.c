#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define BUFFSIZE 4096

void err_sys(char *str)
{
        printf("%s\n", str);
        exit(3);
}

int main(void)
{
    int n;
    char buf[BUFFSIZE];
    while (n = read(STDIN_FILENO, buf, BUFFSIZE) > 0)
        if(write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
    if(n < 0)
        err_sys("read error");
    exit(0);
}
