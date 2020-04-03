#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int pi[2];
    pipe(pi);

    if(fork() == 0){
        dup2(pi[1], 1);
        close(pi[0]);
        close(pi[1]);

        char *argv[] = {"ls", NULL};
        execv("/bin/ls", argv);
    }

    if(fork() == 0){
        dup2(pi[0], 0);
        close(pi[1]);
        close(pi[0]);
            
        char *argv[] = {"wc", "-l", NULL};
        execv("/usr/bin/wc", argv);
    }
}
