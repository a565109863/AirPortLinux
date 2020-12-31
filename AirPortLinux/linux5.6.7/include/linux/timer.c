//
//  timer.c
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/21.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#include <linux/device.h>
#include <linux/pci.h>

#include "timer.h"

OSDefineMetaClassAndStructors(IOTimeout, OSObject)


extern struct pci_dev *_pdev;

void IOTimeout::timeout_run(OSObject* obj, IOTimerEventSource* timer)
{
    if (obj == NULL) {
        return;
    }
    IOTimeout *vt = OSDynamicCast(IOTimeout, obj);
    if (vt == NULL) {
        return;
    }
    kprintf("--%s: line = %d, fn = %s", __FUNCTION__, __LINE__, vt->tl->name);
    vt->tl->function(vt->tl);
}

void init_timer_key(struct timer_list *timer,
                    void (*func)(struct timer_list *), unsigned int flags,
                    const char *name)
{
    timer->function = func;
    timer->name = name;
    
    if (timer->vt == NULL) {
        timer->vt = new IOTimeout();
    }
    
    timer->vt->tl = timer;
    
    timer->vt->timer = IOTimerEventSource::timerEventSource(timer->vt,(IOTimerEventSource::Action)IOTimeout::timeout_run);
    _pdev->dev.dev->fWorkloop->addEventSource(timer->vt->timer);
}


int __mod_timer(struct timer_list *timer, unsigned long expires, int flag)
{
    if (timer == NULL || timer->vt == NULL || timer->vt->timer == NULL) {
        return 0;
    }
    
    kprintf("--%s: line = %d, timer->expires = %lu", __FUNCTION__, __LINE__, timer->expires);
    timer->expires = expires;
    timer->vt->timer->cancelTimeout();
    timer->vt->timer->setTimeoutUS(timer->expires);
    
    return 0;
}

/**
 * add_timer_on - start a timer on a particular CPU
 * @timer: the timer to be added
 * @cpu: the CPU to start it on
 *
 * This is not very scalable on SMP. Double adds are not possible.
 */
void add_timer_on(struct timer_list *timer, int cpu)
{
    BUG_ON(timer_pending(timer) || !timer->function);
    
    unsigned long expires = 0;
    __mod_timer(timer, expires, 0);
}
EXPORT_SYMBOL_GPL(add_timer_on);


/**
 * add_timer - start a timer
 * @timer: the timer to be added
 *
 * The kernel will do a ->function(@timer) callback from the
 * timer interrupt at the ->expires point in the future. The
 * current time is 'jiffies'.
 *
 * The timer's ->expires, ->function fields must be set prior calling this
 * function.
 *
 * Timers with an ->expires field in the past will be executed in the next
 * timer tick.
 */
void add_timer(struct timer_list *timer)
{
    BUG_ON(timer_pending(timer));
    mod_timer(timer, timer->expires);
}
EXPORT_SYMBOL(add_timer);

/**
 * del_timer - deactivate a timer.
 * @timer: the timer to be deactivated
 *
 * del_timer() deactivates a timer - this works on both active and inactive
 * timers.
 *
 * The function returns whether it has deactivated a pending timer or not.
 * (ie. del_timer() of an inactive timer returns 0, del_timer() of an
 * active timer returns 1.)
 */
int del_timer(struct timer_list *timer)
{
    int ret = 0;

    if (timer_pending(timer)) {
        timer->vt->timer->cancelTimeout();
    }

    return ret;
}
EXPORT_SYMBOL(del_timer);

int mod_timer(struct timer_list *timer, unsigned long expires)
{
    return __mod_timer(timer, expires, 0);
}

int timer_pending(const struct timer_list * timer)
{
    return (timer && timer->vt && timer->vt->timer && timer->vt->timer->onThread() && (!timer->vt->timer->checkForWork()));
}



static void do_init_timer(struct timer_list *timer,
              void (*func)(struct timer_list *))
{
//    timer->entry.pprev = NULL;
    timer->function = func;
//    timer->flags = flags | raw_smp_processor_id();
//    lockdep_init_map(&timer->lockdep_map, name, key, 0);
}

