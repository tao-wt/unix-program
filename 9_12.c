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
        printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgrp(),
            (long)tcgetpgrp(STDIN_FILENO));
        fflush(stdout);
}

int
main(void)
{
        char    c;
        pid_t   pid;

        pr_ids("parent");
        if ((pid = fork()) < 0) {
                printf("fork error");
        exit(1);
        } else if (pid > 0) {   /* parent */
                sleep(5);               /* sleep to let child stop itself */
        } else {                        /* child */
                pr_ids("child");
                printf("stdin: %d, stdout: %d\n", STDIN_FILENO, STDOUT_FILENO);
                signal(SIGHUP, sig_hup);        /* establish signal handler */
                kill(getpid(), SIGTSTP);        /* stop ourself */
                pr_ids("child");        /* prints only if we're continued */
                printf("stdin: %d, stdout: %d\n", STDIN_FILENO, STDOUT_FILENO);
                if (read(STDIN_FILENO, &c, 1) != 1)
                        printf("read error %d on controlling TTY\n", errno);
        }
        exit(0);
}

