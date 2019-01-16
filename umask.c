#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(void)
{
	umask(0);
	if (creat("un_foo", RWRWRW) < 0)
		printf("creat error for foo");
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("un_bar", RWRWRW) < 0)
		printf("creat error for bar");
        umask(S_IROTH | S_IWOTH);
	if (creat("un_tao", RWRWRW) < 0)
		printf("create tao error");
	umask(S_IRGRP | S_IROTH);
	if (creat("un_wang", RWRWRW) < 0)
		printf("creat error for wang");
	exit(0);
}
