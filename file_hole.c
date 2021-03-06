#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char    buf1[] = "abcdefghij";
char    buf2[] = "ABCDEFGHIJ";

void err_sys(char *str)
{
        printf("%s", str);
        exit(3);
}

int main(void)
{
        int             fd;

        if ((fd = creat("file.hole", FILE_MODE)) < 0)
                err_sys("creat error");

        if (write(fd, buf1, 10) != 10)
                err_sys("buf1 write error");
        /* offset now = 10 */

        if (lseek(fd, 16384, SEEK_SET) == -1)
                err_sys("lseek error");
        /* offset now = 16384 */
        printf("offset:%ld\n", lseek(fd, 0, SEEK_CUR));
        if (write(fd, buf2, 10) != 10)
                err_sys("buf2 write error");
        /* offset now = 16394 */

        exit(0);
}
