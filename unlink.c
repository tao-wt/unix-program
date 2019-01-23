#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
	if (open("tempfile", O_RDWR) < 0){
		printf("open errori\n");
		exit(1);
	}
	if (unlink("tempfile") < 0){
		printf("unlink error");
		exit(2);
	}
	printf("file unlinked\n");
	sleep(15);
	printf("done\n");
	exit(0);
}
