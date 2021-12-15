#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// del 3 ("3.0")
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

    char *heap = malloc(20) ;
    printf( "the heap variable at: %p\n" , &heap ) ;
    printf( "pointing to : %p\n" , heap ) ;
    
    return 0;
}
/**
0x7fffcd392ea8 0x3
0x7fffcd392eb0 0x7fffcd392eb0
0x7fffcd392eb8 0x67e91ea495326400
0x7fffcd392ec0 0x7fffcd392ef0
0x7fffcd392ec8 0x7f75dc2f0283
0x7fffcd392ed0 0x0
0x7fffcd392ed8 0x7fffcd392f10
0x7fffcd392ee0 0x0
0x7fffcd392ee8 0x2
0x7fffcd392ef0 0x7fffcd392f60
0x7fffcd392ef8 0x7f75dc2f02bd
0x7fffcd392f00 0x7f75dc2ef040
0x7fffcd392f08 0xb800f0b5ff
0x7fffcd392f10 0x1
p: 0x7fffcd392f10
back : 0x7f75dc2f02bd


 /proc/184/maps

7f75dc0b0000-7f75dc0d5000 r--p 00000000 00:00 337911             /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f75dc0d5000-7f75dc24d000 r-xp 00025000 00:00 337911             /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f75dc24d000-7f75dc297000 r--p 0019d000 00:00 337911             /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f75dc297000-7f75dc298000 ---p 001e7000 00:00 337911             /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f75dc298000-7f75dc29b000 r--p 001e7000 00:00 337911             /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f75dc29b000-7f75dc29e000 rw-p 001ea000 00:00 337911             /usr/lib/x86_64-linux-gnu/libc-2.31.so
7f75dc29e000-7f75dc2a2000 rw-p 00000000 00:00 0
7f75dc2b0000-7f75dc2b1000 r--p 00000000 00:00 337358             /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f75dc2b1000-7f75dc2d3000 r-xp 00001000 00:00 337358             /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f75dc2d3000-7f75dc2d4000 r-xp 00023000 00:00 337358             /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f75dc2d4000-7f75dc2db000 r--p 00024000 00:00 337358             /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f75dc2db000-7f75dc2dc000 r--p 0002b000 00:00 337358             /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f75dc2dd000-7f75dc2de000 r--p 0002c000 00:00 337358             /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f75dc2de000-7f75dc2df000 rw-p 0002d000 00:00 337358             /usr/lib/x86_64-linux-gnu/ld-2.31.so
7f75dc2df000-7f75dc2e2000 rw-p 00000000 00:00 0
7f75dc2ef000-7f75dc2f0000 r--p 00000000 00:00 21442              /mnt/c/Users/Amanda/map/main
7f75dc2f0000-7f75dc2f1000 r-xp 00001000 00:00 21442              /mnt/c/Users/Amanda/map/main
7f75dc2f1000-7f75dc2f2000 r--p 00002000 00:00 21442              /mnt/c/Users/Amanda/map/main
7f75dc2f2000-7f75dc2f3000 r--p 00002000 00:00 21442              /mnt/c/Users/Amanda/map/main
7f75dc2f3000-7f75dc2f4000 rw-p 00003000 00:00 21442              /mnt/c/Users/Amanda/map/main
7fffc4e62000-7fffc4e83000 rw-p 00000000 00:00 0                  [heap]
7fffccb94000-7fffcd394000 rw-p 00000000 00:00 0                  [stack]
7fffcd7f0000-7fffcd7f1000 r-xp 00000000 00:00 0                  [vdso]
the heap variable at: 0x7fffcd392f18 <- adressen på stacken
pointing to : 0x7fffc4e632b0 <- pekar på ngt i heapen
**/