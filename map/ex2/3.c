#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();
    if(pid == 0) {
        int child= getpid();
        printf(" I'm the child %d with parent %d\n" , getpid(), getppid()) ;
    }else{
        int parent = getpid();
        printf(" I'm the parent %d with parent %d\n" , getpid(), getppid()) ;
        wait(NULL) ;
    }
return 0 ;
}
/**
 I'm the parent 150 with parent 8
 I'm the child 151 with parent 150

 ------------

  8 tty1     00:00:06 bash <- parent (innan min fork, från datorns processer, pid för shell)
**/