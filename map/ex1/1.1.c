#include <stdio.h>
int main(){
    volatile int a = 4;
    foo:
        printf("the code: %p\n", &&foo);

        fgetc(stdin);
        return 0;
}