#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void err_sys(char *str)
{
    printf("%s", str);
    exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		err_sys("usage: a.out <pathname>");
	if (access(argv[1], R_OK) < 0){
		printf("access error for %s\n", argv[1]);
                //exit(2);
	}else
		printf("read access OK\n");
	if (open(argv[1], O_RDONLY) < 0){
		printf("open error for %s\n", argv[1]);
                //exit(3);
	}else
		printf("open for reading OK\n");
	exit(0);
}
