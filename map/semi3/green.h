//what will be implemented for this semi
//green_create() : initializes a green thread

//green_yield() : suspends the current thread 
// and selects a new thread for execution

//green_join() : the current thread is suspended 
//waiting for a thread to terminate

#include <ucontext.h>

typedef struct green_t{
    ucontext_t *context; 
    void *(*fun)(void*); //function
    void *arg; //pointer to arguments
    struct green_t *next;
    struct green_t *join; //a thread that is waiting to join the thread to terminate
    void *retval;
    int zombie; //TRUE or FALSE on zombie
} green_t;

int green_create(green_t *thread, void *(*fun)(void*), void *arg);
int green_yield();
int green_join(green_t *thread, void** val);

typedef struct green_cond_t{
    //void *cond;
    struct green_t *h; 
}green_cond_t;

typedef struct green_mutex_t {
    volatile int taken;
    // handle the list ---
    green_t *susp;
} green_mutex_t;

int green_mutex_init ( green_mutex_t *mutex ) ;
int green_mutex_lock ( green_mutex_t *mutex ) ;
int green_mutex_unlock ( green_mutex_t *mutex ) ;
void green_cond_init(green_cond_t *cond);
void green_cond_wait(green_cond_t *cond, green_mutex_t *mutex);
void green_cond_signal(green_cond_t *cond);
/*
-void green_cond_init(green_cond_t*): initialize a green condition variable
- void green_cond_wait(green_cond_t*): suspend the current thread
on the condition
- void green_cond_signal(green_cond_t*): move the first suspended
thread to the ready queue
*/

