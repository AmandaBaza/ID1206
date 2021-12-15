#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){

    int pid = fork();

    if(pid == 0) {
        printf("check the status\n");
        sleep(10);
        printf("and again\n");
        return 42;
    }else {
        sleep(20);
        int res;
        wait(&res);
        printf("the result was %d\n", WEXITSTATUS(res));
        printf("and again (parent)\n");
        sleep(10);
    }
    printf("That is the end (%d)\n", getpid());
    
    return 0;
}