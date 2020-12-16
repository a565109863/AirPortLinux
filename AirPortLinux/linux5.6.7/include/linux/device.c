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
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    //    struct irq_devres *dr;
    int rc;
    //
    //    dr = devres_alloc(devm_irq_release, sizeof(struct irq_devres),
    //              GFP_KERNEL);
    //    if (!dr)
    //        return -ENOMEM;
    //
    //    if (!devname)
    //        devname = dev_name(dev);
    //    rc = request_threaded_irq(irq, handler, thread_fn, irqflags, devname,
    //                  dev_id);
    //    if (rc) {
    ////        devres_free(dr);
    //        return rc;
    //    }
    
    //    dr->irq = irq;
    //    dr->dev_id = dev_id;
    //    devres_add(dev, dr);
    
    
    dev->ih = new pci_intr_handle();
    
    int index, source = -1;
    for (index = 0; ; index++)
    {
        int interruptType;
        IOReturn ret = dev->dev->fPciDevice->getInterruptType(index, &interruptType);
        if (ret != kIOReturnSuccess)
            break;
        if (interruptType & kIOInterruptTypePCIMessaged)
        {
            source = index;
            break;
        }
    }
    dev->ih->source = source;
    dev->ih->intrstr = "msi";
    dev->ih->dev = dev->dev->fPciDevice;  // pci device reference
    
    dev->ih->workloop = dev->dev->fWorkloop;
    
    dev->ih->dev_id = dev_id;
    dev->ih->thread_fn = thread_fn;
    dev->ih->intr = IOInterruptEventSource::interruptEventSource(dev->ih, (IOInterruptEventAction)interrupt_func, dev->ih->dev, dev->ih->source);
    
    if (dev->ih->intr == 0)
        return 0;
    if (dev->ih->workloop->addEventSource(dev->ih->intr) != kIOReturnSuccess)
        return 0;
    
    dev->ih->intr->enable();
    
    handler(irq, dev_id);
    
    return 0;
}
