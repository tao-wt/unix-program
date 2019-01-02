#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc,char *argv[])
{
    DIR *dp;
    struct dirent *dirp;
    if (argc != 2){
        printf("one para\n");
        exit(1);
    }
    if ((dp = opendir(argv[1])) == NULL){
        printf("open dir error\n");
        exit(2);
    }
    while ((dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);
    closedir(dp);
    exit(0);
}
