#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <bits/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAXLINE 4096

void sig_int(int signo)
{
    printf("interrupt\n%% ");
}

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR){
        printf("signal error");
        exit(5);
    }

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL){
        if (buf[strlen(buf)-1] == '\n')
            buf[strlen(buf)-1] = 0;
        if ((pid = fork()) < 0){
            printf("fork error!\n");
            exit(2);
        } else if (pid == 0){
            execlp(buf, buf, (char *)0);
            printf("exec error!\n");
            exit(127);
        }

        if((pid = waitpid(pid, &status, 0)) < 0){
            printf("waitpid error!\n");
            exit(3);
        }
        printf("%% ");
    }
    printf("\n");
}
