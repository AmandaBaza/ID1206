#include <stdio.h>
#include <unistd.h>

int main(){

    int pid = fork();
    printf("pid = %d\n", pid);
    return 0;
}
/**
pid = 100
pid = 0
**/