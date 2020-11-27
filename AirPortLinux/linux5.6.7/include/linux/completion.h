//
//  completion.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef completion_h
#define completion_h

#include <linux/wait.h>

struct completion {
    unsigned int done;
    wait_queue_head_t wait;
    IOLock *lock;
    void *event;
};


static inline void init_completion(struct completion *x)
{
    x->done = 0;
    x->lock = IOLockAlloc();
    
    IOLockLock(x->lock);
//    init_waitqueue_head(&x->wait);
}

static void complete(struct completion *x)
{
    IOLockLock(x->lock);
    IOLockWakeup(x->lock, x->event, true);
    IOLockUnlock(x->lock);
    
//    unsigned long flags;
//
//    spin_lock_irqsave(&x->wait.lock, flags);
//
//    if (x->done != UINT_MAX)
//        x->done++;
//    __wake_up_locked(&x->wait, TASK_NORMAL, 1);
//    spin_unlock_irqrestore(&x->wait.lock, flags);
}
EXPORT_SYMBOL(complete);


static void complete_all(struct completion *x)
{
    IOLockLock(x->lock);
    IOLockWakeup(x->lock, x->event, true);
    IOLockUnlock(x->lock);
}

static void wakeupOn(void *ident)
{
//    if (_ifp->fCommandGate == 0)
//        return;
//    else
//        _ifp->fCommandGate->commandWakeup(ident);
}


static void __sched wait_for_completion(struct completion *x)
{
//    wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL(wait_for_completion);


static long __sched
wait_for_completion_killable_timeout(struct completion *x,
                     unsigned long timeout)
{
//    return wait_for_common(x, timeout, TASK_KILLABLE);
    return 0;
}
EXPORT_SYMBOL(wait_for_completion_killable_timeout);

#endif /* completion_h */
