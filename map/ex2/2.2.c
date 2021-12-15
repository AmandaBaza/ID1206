#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){

    int pid = fork();

    if( pid == 0 ) {
        return 44;
        }else {
            int res;
            wait(&res);
            printf("the result was %d\n", WEXITSTATUS(res));
            printf("res: %d\n", res);
        }
    printf("That is the end (%d)\n", getpid());
    return 0;
}
/**
the result was 42
res: 10752 <- varför inte 42 
That is the end (193)
**/