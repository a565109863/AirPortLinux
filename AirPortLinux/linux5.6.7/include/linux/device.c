//
//  device.c
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/21.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "device.h"

#include "AirPortLinux.hpp"


int __must_check
devm_request_threaded_irq(struct device *dev, unsigned int irq,
                          irq_handler_t handler, irq_handler_t thread_fn,
                          unsigned long irqflags, const char *devname,
                          void *dev_id) {
    dev->ih = new pci_intr_handle();
    
    int index;
    irq = -1;
    for (index = 0; ; index++)
    {
        int interruptType;
        IOReturn ret = dev->dev->fPciDevice->getInterruptType(index, &interruptType);
        if (ret != kIOReturnSuccess)
            break;
        if (interruptType & kIOInterruptTypePCIMessaged)
        {
            irq = index;
            break;
        }
    }
    
    dev->ih->irq = irq;
    dev->ih->intrstr = "msi";
    dev->ih->dev = dev->dev->fPciDevice;  // pci device reference
    
    dev->ih->workloop = dev->dev->fWorkloop;
    
    dev->ih->dev_id = dev_id;
    dev->ih->thread_fn = thread_fn;
    dev->ih->filter_fn = handler;
    
//    dev->ih->intr = IOInterruptEventSource::interruptEventSource(dev->ih, (IOInterruptEventAction)interrupt_func, dev->ih->dev, dev->ih->source);
//
//    if (dev->ih->intr == 0)
//        return 0;
//    if (dev->ih->workloop->addEventSource(dev->ih->intr) != kIOReturnSuccess)
//        return 0;
//    dev->ih->intr->enable();
    
    dev->ih->fintr = IOFilterInterruptEventSource::filterInterruptEventSource(dev->ih, (IOInterruptEventAction)interrupt_func, interrupt_filter, dev->ih->dev, dev->ih->irq);
    
    if (dev->ih->fintr == 0)
        return 0;
    if (dev->ih->workloop->addEventSource(dev->ih->fintr) != kIOReturnSuccess)
        return 0;
    
    dev->ih->fintr->enable();
    
    return 0;
}
