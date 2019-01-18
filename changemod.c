#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void)
{
	struct stat		statbuf;

	/* turn on set-group-ID and turn off group-execute */

	if (stat("un_foo", &statbuf) < 0){
		printf("stat error for foo");
		exit(0);
	}
	if (chmod("un_foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		printf("chmod error for foo");

	/* set absolute mode to "rw-r--r--" */

	if (chmod("un_bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
		printf("chmod error for bar");

        if (stat("un_wang", &statbuf) < 0){
                printf("stat error for wang");
                exit(0);
        }

        if (chmod("un_wang", (statbuf.st_mode & ~S_IWUSR) | S_IRUSR | S_IRGRP | S_IROTH | S_ISUID) < 0)
                printf("chmod error for wang");

	if (stat("walle", &statbuf) < 0){
                printf("stat error for walle");
                exit(0);
        }

	if (chmod("walle", (statbuf.st_mode & ~S_IXOTH)  | S_ISVTX)  < 0)
		printf("set sticky bit failed");
	exit(0);
}
