#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char    buf1[] = "abcdefghij";
char    buf2[] = "ABCDEFGHIJ";
char    buf3[] = "-";

void err_sys(char *str)
{
        printf("%s", str);
        exit(3);
}

int main(void)
{
        int             fd;
        printf("off_t: %d\nint: %d\nchar: %d\nfloat: %d\n", sizeof(off_t), sizeof(int), sizeof(char), sizeof(float));
        printf("ssize_t: %d\nsize_t: %d\n",sizeof(ssize_t), sizeof(size_t));
}
