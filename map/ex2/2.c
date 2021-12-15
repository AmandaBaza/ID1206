#include <stdio.h>
#include <unistd.h>

int main(){

    int pid = fork();

    if(pid == 0){
        printf("I'm the child %d\n", getpid());
    }else{
        printf("My child is called %d\n", pid);
    }
    printf("That's it %d\n", getpid());

    return 0;
}
/** Parent och child sker samtidigt, 2 olika processes 
My child is called 107  
I'm the child 107
That's it 106
That's it 107
**/