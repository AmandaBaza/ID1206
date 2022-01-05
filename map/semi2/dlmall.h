#include <stdio.h>
#include <stdint.h>
//#include "dlmall.c"

void dfree(void* memory); 
void *dalloc(size_t request);
struct head *after(struct head *block);
size_t adjust(size_t req);