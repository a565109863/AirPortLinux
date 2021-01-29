//
//  wait.c
//  AirPortLinux
//
//  Created by Zhong-Mac on 2020/6/26.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "wait.h"

#include <linux/device.h>
#include <linux/pci.h>

extern struct pci_dev *_pdev;

int wait_event_timeout(wait_queue_head_t q, int i, u32 timeout)
{
    if (_pdev->dev.dev->fCommandGate == 0) {
        // no command gate so we just sleep
        IODelay(timeout);
        return 1;
    }
    
    IOReturn ret;
    if (timeout == 0) {
        ret = _pdev->dev.dev->fCommandGate->runAction(AirPortLinux::tsleepHandler, q.ident);
    } else {
        timeout = timeout * USEC_PER_SEC;
        ret = _pdev->dev.dev->fCommandGate->runAction(AirPortLinux::tsleepHandler, q.ident, &timeout);
    }
    
    return ret == kIOReturnSuccess;
}


int wakeup_sleep(wait_queue_head_t *q, bool one)
{
    kprintf("--%s: line = %d irq", __FUNCTION__, __LINE__);
    
    if (_pdev->dev.dev->fCommandGate == 0)
        return 0;
    else
        _pdev->dev.dev->fCommandGate->commandWakeup(q->ident, one);
    
    return 0;
}


static int
try_to_wake_up(struct task_struct *p, unsigned int state, int wake_flags)
{
    return 0;
}

int default_wake_function(wait_queue_entry_t *curr, unsigned mode, int wake_flags,
                          void *key)
{
    return try_to_wake_up((struct task_struct *)curr->_private, mode, wake_flags);
}
EXPORT_SYMBOL(default_wake_function);

void add_wait_queue_exclusive(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
    unsigned long flags;

    wq_entry->flags |= WQ_FLAG_EXCLUSIVE;
    spin_lock_irqsave(&wq_head->lock, flags);
    __add_wait_queue_entry_tail(wq_head, wq_entry);
    spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_SYMBOL(add_wait_queue_exclusive);
