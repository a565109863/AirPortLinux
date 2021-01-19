//
//  task.c
//  AirPortLinux
//
//  Created by Zhong-MAC on 2021/1/19.
//  Copyright © 2021 User-Mac. All rights reserved.
//


#include <linux/kernel.h>
#include <linux/mutex.h>
#include "task.h"


typedef struct mutex    pthread_mutex_t;

struct pthread_cond {
    volatile unsigned int seq;
    pthread_mutex_t          mtx;
};

typedef struct    pthread_cond*        pthread_cond_t;

struct taskletq {
    thread_t            thread;
    struct tasklet_struct_list    list;
    pthread_mutex_t     mtx;
    pthread_cond_t      cv;
};


int
pthread_mutex_init(pthread_mutex_t *mutex,
                   void *attr)
{
//    mutex->lock = IORecursiveLockAlloc();
    mutex_init(mutex);
    return (0);
}

int
pthread_mutex_lock(pthread_mutex_t *mutex)
{
//    IORecursiveLockLock(mutex->lock);
    mutex_lock(mutex);
    return (0);
}

int
pthread_mutex_unlock(pthread_mutex_t *mutex)
{
//    IORecursiveLockUnlock(mutex->lock);
    mutex_unlock(mutex);
    return (0);
}

int
pthread_cond_init(pthread_cond_t *condp, const void *attr)
{
    *condp = (pthread_cond_t)kmalloc(sizeof(struct pthread_cond), M_NOWAIT | M_ZERO);
    if (*condp == NULL)
        return 1;
    
    return 0;
}

int
pthread_cond_wait(pthread_cond_t condp, pthread_mutex_t *mutexp)
{
    int ret;
    pthread_mutex_lock(mutexp);
    ret = IORecursiveLockSleep(mutexp->lock, condp, THREAD_INTERRUPTIBLE);
    pthread_mutex_unlock(mutexp);
    
    /* return value is always >= 0 */
    //    if (ret != THREAD_AWAKENED)
    //        return -ETIMEDOUT;
    return 0;
}

int
pthread_cond_signal(pthread_cond_t condp)
{
    pthread_mutex_lock(&condp->mtx);
    IORecursiveLockWakeup(condp->mtx.lock, condp, true);
    pthread_mutex_unlock(&condp->mtx);
    return 1;
}



static void
tasklet_action_common(void* tqarg, wait_result_t waitResult)
{
    struct taskletq *tq = (struct taskletq *)tqarg;
    struct tasklet_struct *t;
    
    for (;;) {
        pthread_mutex_lock(&tq->mtx);
        while ((t = TAILQ_FIRST(&tq->list)) == NULL)
            pthread_cond_wait(tq->cv, &tq->mtx);
        
        TAILQ_REMOVE(&tq->list, t, t_entry);
        test_and_clear_bit(TASKLET_STATE_SCHED,
            &t->state);
        
        pthread_mutex_unlock(&tq->mtx);
        
        DebugLog("-----%s: line = %d, t->func_name = %s", __FUNCTION__, __LINE__, t->func_name);
        t->func(t->data);
        kprintf("-----%s: line = %d, t->func_name = %s", __FUNCTION__, __LINE__, t->func_name);
    }
    
}

struct taskletq *const systq = taskletq_create("systq", 1, 3, 0);

struct taskletq *
taskletq_create(const char *name, unsigned int nthreads, int ipl,
             unsigned int flags)
{
    DebugLog("-----%s: line = %d", __FUNCTION__, __LINE__);
    struct taskletq *tq;
    int error;
    
    tq = (struct taskletq *)kmalloc(sizeof(*tq), M_NOWAIT | M_ZERO);
    if (tq == NULL)
        return (NULL);

    TAILQ_INIT(&tq->list);

    error = pthread_mutex_init(&tq->mtx, NULL);

    error = pthread_cond_init(&tq->cv, NULL);
    
    kernel_thread_start(tasklet_action_common, tq, &tq->thread);
    thread_deallocate(tq->thread);

    return (tq);

}

void __tasklet_schedule(struct taskletq *tq, struct tasklet_struct *t)
{
    DebugLog("-----%s: line = %d, t->func_name = %s", __FUNCTION__, __LINE__, t->func_name);
    pthread_mutex_lock(&tq->mtx);
    TAILQ_INSERT_TAIL(&tq->list, t, t_entry);
    tq->cv->mtx = tq->mtx;
    pthread_cond_signal(tq->cv);
    pthread_mutex_unlock(&tq->mtx);
}


void __tasklet_kill(struct taskletq *tq, struct tasklet_struct *t)
{
    if (!test_bit(TASKLET_STATE_SCHED, &t->state))
        return;
    
    pthread_mutex_lock(&tq->mtx);
    if (test_bit(TASKLET_STATE_SCHED, &t->state)){
        TAILQ_REMOVE(&tq->list, t, t_entry);
        clear_bit(TASKLET_STATE_SCHED, &t->state);
    }
    
    pthread_mutex_unlock(&tq->mtx);
    
}
