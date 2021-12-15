#include <stdio.h>
//cd /mnt/c/Users/Amanda/map
int main(){
    volatile int a = 4;
    foo:
        printf("the code: %p\n", &&foo);

        fgetc(stdin);
        return 0;
}