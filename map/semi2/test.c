#include <stdio.h>
#include "dlmall.h"

int main()
{
    void *a = dalloc((size_t) 6);
    void *b = dalloc((size_t) 8);
    void *c = dalloc((size_t) 100);
    dfree(a);
    dfree(b);
    dfree(c);
    return 0;
}
