#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include<signal.h>

static void
sig_hup(int signo)
{
        printf("SIGHUP received, pid = %ld\n", (long)getpid());
}

static void
pr_ids(char *name)
{
        printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, sid = %ld, tpgrp = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)getsid(0),
            (long)tcgetpgrp(STDIN_FILENO));
        fflush(stdout);
}

int
main(void)
{
        char    c;
        int fd;
        pid_t   pid;
        char *cmd = (char *)malloc(2048);

        pr_ids("parent");
        if ((pid = fork()) < 0) {
                printf("fork error");
        exit(1);
        } else if (pid > 0) {   /* parent */
                //sprintf(fname, "sub process is %d", pid);
                //printf();
                sleep(5);               /* sleep to let child stop itself */
                system("ps -o pid,ppid,pgid,sid,tpgid,stat,comm");
        } else {                        /* child */
                pr_ids("child");
                signal(SIGHUP, sig_hup);        /* establish signal handler */
                kill(getpid(), SIGTSTP);        /* stop ourself */
                pr_ids("child");
                setsid();
                pr_ids("child");        /* prints only if we're continued */
                system("ps -o pid,ppid,pgid,sid,tpgid,stat,comm");
                if (read(STDIN_FILENO, &c, 1) != 1) {
                        printf("read error\n");
                }
                printf("Get char from stdin:%s\n", c);
                system("ps -o pid,ppid,pgid,sid,tpgid,stat,comm");
        }
        exit(0);
}

