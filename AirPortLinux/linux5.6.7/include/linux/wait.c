//
//  wait.c
//  AirPortLinux
//
//  Created by Zhong-Mac on 2020/6/26.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "wait.h"
#include "AirPortLinux.hpp"

#include <linux/device.h>
#include <linux/pci.h>

extern struct pci_dev *_pdev;

int wait_event_timeout(wait_queue_head_t q, int i, u32 timeout) {
    if (_pdev->dev.dev->fCommandGate == 0) {
        // no command gate so we just sleep
        IODelay(timeout);
        return 1;
    }
    
    IOReturn ret;
    if (timeout == 0) {
        ret = _pdev->dev.dev->fCommandGate->runAction(AirPortLinux::tsleepHandler, q.ident);
    } else {
        ret = _pdev->dev.dev->fCommandGate->runAction(AirPortLinux::tsleepHandler, q.ident, &timeout);
    }
    
    if (ret == kIOReturnSuccess)
        return 0;
    else
        return 1;
}


int wake_up(wait_queue_head_t *q)
{
    if (_pdev->dev.dev->fCommandGate == 0)
        return 0;
    else
        _pdev->dev.dev->fCommandGate->commandWakeup(q->ident);
    
    return 0;
}

void wake_up_all(struct wait_queue_head *wq_head)
{
    struct wait_queue_head *curr;
    LIST_HEAD(tmp);

//    spin_lock_irq(&wq_head->lock);
    list_splice_init(&wq_head->head, &tmp);
    while (!list_empty(&tmp)) {
        curr = list_first_entry(&tmp, typeof(*curr), head);

//        wake_up_state(curr->lock, TASK_NORMAL);
        wake_up(curr);
        list_del_init(&curr->head);

        if (list_empty(&tmp))
            break;

//        raw_spin_unlock_irq(&wq_head->lock);
//        raw_spin_lock_irq(&wq_head->lock);
    }
//    spin_unlock_irq(&q->lock);
}

