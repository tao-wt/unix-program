#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void)
{
    int f;
    printf("int:%d\n", sizeof(f));
    printf("uid: %d, gid: %d\n", getuid(), getgid());
    exit(0);
}
