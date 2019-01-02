#include <stdio.h>
#include <stdlib.h>

//printf("EOF:%c\nstdin:%c\nstdout%c\n",EOF,stdin,stdout);

int main(void)
{
    int n;

    while ((n = getc(stdin)) != EOF){
        printf("%c\n",n);
        if (putc(n, stdout) == EOF){
            printf("print error...\n");
            exit(1);
        }
    }
    if(ferror(stdin)){
        printf("get input error!\n");
        exit(2);
    }
    exit(0);
}
