#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void zot (unsigned long * stop) {
    unsigned long r = 0x3;
    unsigned long*i;
    for ( i = &r ; i <= stop ; i++){
        printf("%p 0x%lx\n",i,*i);
    }
}

void foo (unsigned long *stop) {
    unsigned long q =0x2 ;
    zot (stop) ;
}

int main(){
    int pid = getpid();
    unsigned long p = 0x1 ;
    foo(&p);
    back:
    printf( "p: %p \n" , &p ) ;
    printf("back : %p \n" , &&back ) ;
    printf( "\n\n /proc/%d/maps \n\n" , pid);
    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);
    return 0;
}
/**
0x7fffef694548 0x3 <- r
0x7fffef694550 0x7fffef694550 <- i pekar på sin egen
0x7fffef694558 0xfc1499027162ec00
0x7fffef694560 0x7fffef694590

längre ner än alla andra andresser = är inte på heap(?) då malloc elr free ej används(?)
, ser inte ut som data -> Text -> retur adress
0x7fffef694568 0x7f2906cb3263 


0x7fffef694570 0x0
0x7fffef694578 0x7fffef6945a8
0x7fffef694580 0x0
0x7fffef694588 0x2 <- q
0x7fffef694590 0x7fffef6945f0
0x7fffef694598 0x7f2906cb329d
0x7fffef6945a0 0x9f000000c2 <-- where pid can de found . sluet är 1100 0010 (why?)
0x7fffef6945a8 0x1 <- p 
p: 0x7fffef6945a8
back : 0x7f2906cb329d

 /proc/159/maps
 ...
**/
//???? what is the value of i when the location of i is printed?
/**value of the process identifier? pid = 159? -> 0x9f found check earlier
**/
