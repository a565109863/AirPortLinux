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
