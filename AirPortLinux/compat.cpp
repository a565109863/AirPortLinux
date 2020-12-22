//
//  compat.cpp
//  AppleIntelWiFi
//
//  Created by Mac-PC on 2020/3/18.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include <compat.h>

#include <linux/device.h>

OSDefineMetaClassAndStructors(pci_intr_handle, OSObject)

void interrupt_func(OSObject *ih, IOInterruptEventSource *src, int count)
{
    pci_intr_handle* _ih = OSDynamicCast(pci_intr_handle, ih);
    if (_ih == NULL)
        return;
    _ih->thread_fn(_ih->irq, _ih->dev_id); // jump to actual interrupt handler
}

bool interrupt_filter(OSObject *ih, IOFilterInterruptEventSource *sender)
{
    pci_intr_handle* _ih = OSDynamicCast(pci_intr_handle, ih);
    if (_ih == NULL)
        return false;
    
    return _ih->filter_fn(_ih->irq, _ih->dev_id);
}
