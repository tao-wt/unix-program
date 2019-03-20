#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
// #define BUFFSIZE 4096

int main(int argc, char *argv[])
{
    int n;
    char str[5];
    char name[100] = "name: ";
    strcat(name, argv[0]);
    strcat(name, "\ntotal: ");
    sprintf(str,"%d",argc);
    strcat(name, str);
    strcat(name, "\n");
    fputs(name, stderr);
    int BUFFSIZE = atoi(argv[1]);
    char buf[BUFFSIZE];
    // int BUFFSIZE = 1;
    // printf("buf:%d", BUFFSIZE);
    // char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n){
            printf("print error...\n");
            exit(1);
        }
    if(n < 0){
        printf("get input error!\n");
        exit(2);
    }
    exit(0);
}
