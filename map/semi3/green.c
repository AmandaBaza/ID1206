#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <assert.h>
#include "green.h"

#include <signal.h>
#include <sys/time.h>

#define FALSE 0
#define TRUE 1
#define STACK_SIZE 4096

#define PERIOD 100
static sigset_t block;
void timer_handler ( int ) ;

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};
static green_t *running = &main_green;
struct green_t *head;
//? struct green_t *tail;
/*
The init() function will initialize the main_cntx so when we call the
scheduling function for the first time the running thread will be properly
initialized.
*/
static void init() __attribute__ ((constructor));

void addToReadyQ(struct green_t *new){
    /*
    if(new == NULL)
        return;
    if(head == NULL){
        new->next = NULL;
        head = new;
        return;
    }
    green_t* current = head;
    while(current->next != NULL){
        current = current->next;
    }
    new->next = NULL;
    current->next = new;
    */
    if(new != NULL){
        if(head != NULL){
            green_t* current = head;
            while(current->next != NULL){
                current = current->next;
            }
            new->next = NULL;
            current->next = new;
        }else{
            new->next = NULL;
            head = new;
        }
    }return;
    /*if(new != NULL){
        new->next =NULL;
    }
    if(head !=NULL){
        tail->next = new;
        tail = new;
    }else{ //if there is no queue
        head = new;
        tail = new;
    }*/
    /*
    struct readyQ ne = {new, NULL, NULL};
    struct readyQ *n = &ne;
    if(ready.current != NULL){
    //add to last in Queue 
        n->next = &ready;
        n->prev = ready.prev;
        ready.prev->next = n;  
        ready.prev = n;
        ready.next = 
    }else{
        ready.current = new;
        ready.next = n;
        ready.prev = n;
    }*/

}

green_t *getReadyFromQ(){

    if(head != NULL){
        struct green_t* current = head;
        head = head->next;
        current->next = NULL;

        return current;
    }else{
        return NULL;
    }


    /*
    if(head == NULL)
        return NULL;

    struct green_t* current = head;

    head = head->next;

    current->next = NULL;

    return current;
-------------------------
    struct green_t *r;
     r = head;
    if(head == NULL){
        return NULL;
    }
    if(head->next == NULL){ //only one element in queue
        head = NULL;
        tail = NULL;
    }else{
        head = head->next;
    }
    return r;*/

    /*
    r = ready.current; //first in the queue
    if(ready.prev != ready.next ){
        ready.prev->next = ready.next; //----
        ready.next->prev = ready.prev; //----
        ready = *ready.next;
    }else{//only one thing in ready
        ready.current = NULL;
        ready.next = NULL;
        ready.prev = NULL;
    }
    */
}

void timer_handler ( int sig ) {
    green_t * susp = running ;
    // add the running to the ready queue
    running = running->next;
    addToReadyQ(susp);
   
    // find the next thread for execution
    green_t *next = getReadyFromQ();

    running = next ;
    swapcontext( susp->context , next->context) ;
}

void init(){
    getcontext(&main_cntx);

    sigemptyset(&block);
    sigaddset(&block , SIGVTALRM);

    struct sigaction act = {0};
    struct timeval interval;
    struct itimerval period;

    act.sa_handler = timer_handler;
    assert(sigaction(SIGVTALRM, &act , NULL) == 0 );
    interval.tv_sec = 0;
    interval.tv_usec = PERIOD;
    period.it_interval = interval;
    period.it_value = interval;
    setitimer(ITIMER_VIRTUAL, &period, NULL);
}



void green_thread ( ) {
    green_t * this = running;
    void * result = (*this->fun) (this->arg);
    // place waiting (joining) thread in ready queue
    if(this->join != NULL){
        addToReadyQ(this->join);
    }
    // save result of execution
    this->retval= result; 
    // we're a zombie
    this->zombie = TRUE; 
    // find the next thread to run
    green_t *next = getReadyFromQ();
    
    running = next;
    setcontext(next->context);
}

int green_create(green_t *new, void *(*fun)(void*), void *arg){

    ucontext_t *cntx = (ucontext_t *)malloc(sizeof(ucontext_t));
    getcontext(cntx);

    void *stack = malloc(STACK_SIZE);

    cntx->uc_stack.ss_sp = stack;
    cntx->uc_stack.ss_size = STACK_SIZE;
    makecontext (cntx, green_thread, 0 ) ;

    new->context = cntx ;
    new->fun = fun ;
    new->arg = arg ;
    new->next = NULL;
    new->join = NULL;
    new->retval = NULL;
    new->zombie = FALSE;
    // add new to the ready queue 
    addToReadyQ(new);

    return 0 ;
}

int green_yield(){
green_t *susp = running;
// add susp to ready queue
addToReadyQ(susp);
// select the next thread for execution
green_t *next = getReadyFromQ();

running = next ;
swapcontext (susp->context , next->context) ;
return 0 ;
}

int green_join ( green_t * thread , void **res ) {
    if(!thread->zombie ){
        green_t * susp = running; 
        //add as joining thread - susp is waiting to join
        thread->join = susp; 
        // select the next thread for execution
        green_t *next = getReadyFromQ();
        running = next;
        swapcontext (susp->context , next->context);
    }

    // collect result ----
    res = thread->retval;
    // free context
    free(thread->context); 
    
    return 0 ;
}

/*
You should implement the following functionality:
- void green_cond_init(green_cond_t*): initialize a green condition variable
- void green_cond_wait(green_cond_t*): suspend the current thread
on the condition
- void green_cond_signal(green_cond_t*): move the first suspended
thread to the ready queue
*/

void addToCondQ(struct green_t *new, green_cond_t *cond){
    green_t* p = cond->h;
    
    if(p != NULL){
        while(p->next != NULL){
            p = p->next;
        }
        p->next = new;
    }else{
        cond->h = new;
    }
}

green_t *getFromCondQ(green_t *h){
    struct green_t *r;
    r = h;
    if(h == NULL){
        return r;
    }else if(h->next != NULL){ //only one element in queue
       h = h->next;
    }else{
         h = NULL;
    }
    return r;
}

void green_cond_init(green_cond_t* cond){
    cond->h= NULL;
}

void green_cond_wait(green_cond_t* cond){
    green_t * susp = running;  
    addToCondQ(susp, cond); 
    /*
    if(p != NULL){
        while(p->next != NULL){
            p = p->next;
        }
        p->next = susp;
    }else{
        cond->h = susp;
    }*/
    green_t* next = getReadyFromQ();

    running = next; 
    swapcontext (susp->context , next->context);
}

//suspend the current thread on the condition
/*void green_cond_wait(green_cond_t* cond){
    green_t * susp = running;  
    green_t *next = getReadyFromQ();

    if(next == NULL)
        return;

    running = next;
    addToCondQ(susp, cond->h); 
    swapcontext (susp->context , next->context);
}*/

//move the first suspended thread to the ready queue
void green_cond_signal(green_cond_t* cond){
    if(cond->h != NULL){
        addToReadyQ(cond->h);
        cond->h = cond->h->next; //? glömde att flytta head pekarn
    }
}

