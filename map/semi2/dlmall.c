#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>
#include <sys/mman.h>
#include <errno.h>


#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(struct head))

#define MIN(size) (((size)>(8) ? (size) : (8)))
#define LIMIT(size) (MIN(size) + HEAD + size)
#define MAGIC(memory) ((struct head*) memory -1)
#define HIDE(block) (void*) ((struct head*) block +1)
#define ALIGN 8
#define ARENA (64*1024)

struct head
{
    uint16_t bfree; //2B
    uint16_t bsize; //2B
    uint16_t free; //2B
    uint16_t size; // 2 bytes,the size (max 2^16 i.e. 64 Kibyte)
    struct head *next; //8B
    struct head *prev;//8B
};

//in the arena, NOT free list
struct head *after(struct head *block)
{
    return (struct head*)(block + HEAD + block->size); //ändrade: tog bort +1 
};
struct head *before(struct head *block)
{
    return (struct head*)(block - HEAD- block->bsize); //ändrade: tog bort -1
};

struct head *split(struct head * block , int size){
    int rsize = block->size - size - HEAD; 
    block->size = rsize;

    struct head *splt = block + HEAD + block->size; //ändrade &block + block->size +1
    splt->bsize = block->size; 
    splt->bfree = block->free; 
    splt->size = size;
    splt->free = TRUE; //får ta bort när jag använt den???????

    struct head *aft = after(splt);
    aft->bsize = splt->size;

    return splt;
}



struct head *arena = NULL;

struct head *new(){
    if(arena != NULL){
        printf( "one arena already allocated\n" ) ;
        return NULL;
    }

    // using mmap, but could have used sbrk
    struct head *new = mmap(NULL, ARENA,
    PROT_READ | PROT_WRITE,
    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 ) ;
    if(new == MAP_FAILED) {
        printf( "mmap failed : error %d\n", errno) ;
        return NULL;
    }
    /* make room for head and dummy */
    uint size = ARENA - 2*HEAD;  
    new->bfree = FALSE; //Så den inte mergar
    new->bsize = FALSE; //????????????
    new->free= TRUE;
    new->size = size; 
    struct head * sentinel = after(new) ;
    /* only touch the status fields */
    sentinel ->bfree = TRUE; //new->free= TRUE ???????
    sentinel ->bsize = size; 
    sentinel ->free = FALSE;
    sentinel ->size = 0; 

    /*this is the only arena we have */
    arena = ( struct head *) new ;
    return new ; 
}

struct head * flist; //TODO
void detach ( struct head * block ) {
    if ( block->next != NULL){
        //TODO
    }if ( block->prev != NULL){
        //TODO
    }else
        HIDE(block); //???????????
}

void insert ( struct head * block ) {
    block->next = flist; 
    block->prev = NULL;  
    
    if (flist != NULL){//TODO
    flist->prev = block;
    flist = block; //block is now head
    }
}

//sammanfattning av mall instrutioner

// jämn multipel av ALIGN OCH !< min size (MIN?)

// go though flist, first block(fblock) 
// large enough to meet our req.
// and unlink (detach) from flist
// if flist == empty --> create the arena

//if fblock big enough to splitt in 2 
//(size exkl. head =< 40)-> split -> rest i flist

//detatch (fblock)?? update block efter Taken block?
//fblock->free= FALSE 

//'pointer = HIDE() ?????

struct head *find(int fsize){
struct head *find;
struct head *block;
    if(flist != NULL){
        block = flist;
    }else{
        flist = new(); //korrekt?????
    }
    while(block != NULL){
        if(block->size >= fsize){
            *find = *block;
            detach(find); //eller detach(block) ?????
            break;
        }
        block= block->next;
    }
    //if(*find != NULL){
        if(find->size >= fsize + HEAD + ALIGN){
            * find = *split(find, fsize);  
            insert(find + HEAD + find->size); //insert rest to flist  
        }
        find->free=FALSE; //mark the found block as taken
        //"and make sure to also update the block after the taken block."
        //is done in detach
        return find + HEAD;
    //}else{
        //if no of good size it to be found, merge to get one
    //}
}
int adjust(size_t req){
    return LIMIT(req);
    //MULTIPLE AV 8 TODO
}

void * dalloc(size_t request) {
    if (request <= 0 ){
        return NULL; 
    }
    int size=adjust(request) ;
    struct head * taken = find(size) ;
    if ( taken == NULL)
        return NULL;
    else
        return taken;
}

void dfree ( void *memory ) {
    if (memory != NULL) {
        struct head * block = MAGIC(memory);
        
        struct head * aft = block->next;
        block->free = TRUE;
        aft->bfree = TRUE;
        insert(block); 
    }
    return;
}
