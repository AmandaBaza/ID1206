#include <stdio.h>
#include "dlmall.h"
void printAll(struct head *h){
    printf("All sizes: \n");
    while (h != NULL){    
        printf("%d\n", (int) h->size);
        h = h->next;
    }
}
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
    //printAll(getFlist());
    void *b = dalloc((size_t) 8);
    //printAll(getFlist());
    void *c = dalloc((size_t) 16);
    //printAll(getFlist());

    dfree(a);
    //printAll(getFlist());
    dfree(b);
    //printAll(getFlist());
    dfree(c);
    //printAll(getFlist());
    return 0;
}
