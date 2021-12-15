#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    /**
    char *heap = malloc(20);
    *heap = 0x61;
    printf("heap pointing to : 0x%x\n" , *heap);
    free(heap);
    char * foo = malloc(20);
    * foo = 0x62;
    printf("foo pointing to : 0x%x\n" , *foo);
    // danger ahead 
    *heap = 0x63;
    printf("or is it pointing to : 0x%x\n" , *foo);
    **/
    
    long *heap = (unsigned long*) calloc (40, sizeof(unsigned long));

    printf("heap [2]: 0x%lx\n" , heap[2]);
    printf("heap [1]: 0x%lx\n" , heap[1]);
    printf("heap [0]: 0x%lx\n" , heap[0]);
    printf("heap [-1]: 0x%lx\n" , heap[-1]);
    printf("heap [-2]: 0x%lx\n" , heap[-2]);

    free(heap);
    printf("\nafter free:\n");
    printf("heap [2]: 0x%lx\n" , heap[2]);
    printf("heap [1]: 0x%lx\n" , heap[1]);
    printf("heap [0]: 0x%lx\n" , heap[0]);
    printf("heap [-1]: 0x%lx\n" , heap[-1]);
    printf("heap [-2]: 0x%lx\n" , heap[-2]);


    //print memorymap?
    int pid = getpid();
    printf( "\n\n /proc/%d/maps \n\n" , pid);
    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);
    
    return 0;
}
//One thing you might wonder is how free knows
//the size of the object that it is about to free - any clues?


/**
heap pointing to : 0x61
foo pointing to : 0x62
or is it pointing to : 0x63 <- core/memory dump?

**************

heap [2]: 0x0
heap [1]: 0x0
heap [0]: 0x0
heap [-1]: 0x151  <- From "Data" --> calloc n free cant free it
heap [-2]: 0x0

after free:
heap [2]: 0x0
heap [1]: 0x7ffff185f010 <- (an andress on [heap]) Random value comes when freed
heap [0]: 0x0
heap [-1]: 0x151
heap [-2]: 0x0

**/
