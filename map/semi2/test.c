#include <stdio.h>
#include "dlmall.h"
int main()
{
    /*
    void *a[] = {
        dalloc((size_t) 6),
        dalloc((size_t) 8),
        dalloc((size_t) 16)
        //dalloc((size_t) 100)
    };
    
    int i;
    for(i = 0; i < 3; i++){
        
        //printf("loop-\n");
        //printAll(getFlist());
        dfree(a[i]);   
    }
    printAll(getFlist());
    printf("\ndone");*/
    
    void *a = dalloc((size_t) 6);
    printFlist();
    void *b = dalloc((size_t) 8);
    printFlist();
    void *c = dalloc((size_t) 16);
    printFlist();
    
    dfree(b);
    printf("after b merge:");
    printFlist();
    dfree(a);
    printf("after a merge:");
    printFlist();
    dfree(c);
    printf("after c merge:");
    printFlist();
    return 0;
}
