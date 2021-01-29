//
//  interrupt.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef interrupt_h
#define interrupt_h

enum irqreturn {
    IRQ_NONE        = (0 << 0),
    IRQ_HANDLED        = (1 << 0),
    IRQ_WAKE_THREAD        = (1 << 1),
};

typedef enum irqreturn irqreturn_t;

typedef irqreturn_t (*irq_handler_t)(int, void *);


class pci_intr_handle : public OSObject {
    OSDeclareDefaultStructors(pci_intr_handle)
public:
    IOWorkLoop*        workloop;
//    IOInterruptEventSource*    intr;
    IOFilterInterruptEventSource* fintr;
    IOPCIDevice*        dev;
    irq_handler_t thread_fn;
    irq_handler_t filter_fn;
    void *dev_id;
    unsigned int irq;
    const char *intrstr;
};


void interrupt_func(OSObject *owner, IOInterruptEventSource *src, int count);
bool interrupt_filter(OSObject *owner, IOFilterInterruptEventSource *sender);

#endif /* interrupt_h */
