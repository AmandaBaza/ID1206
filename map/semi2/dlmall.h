#include <stdio.h>
#include <stdint.h>
//#include "dlmall.c"

void dfree(void* memory); 
void *dalloc(size_t request);
struct head *after(struct head *block);
size_t adjust(size_t req);
void printFlist();

struct head
{
    uint16_t bfree; //2B
    uint16_t bsize; //2B
    uint16_t free; //2B
    uint16_t size; // 2 bytes,the size (max 2^16 i.e. 64 Kibyte)
    struct head *next; //8B
    struct head *prev;//8B
};