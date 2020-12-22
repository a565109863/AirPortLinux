//
//  device.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef device_h
#define device_h

#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include "AirPortLinux.hpp"

struct pci_device_id {
    __u32 vendor, device;        /* Vendor and device ID or PCI_ANY_ID*/
    __u32 subvendor, subdevice;    /* Subsystem ID's or PCI_ANY_ID */
    __u32 _class, class_mask;    /* (class,subclass,prog-if) triplet */
    kernel_ulong_t driver_data;    /* Data private to the driver */
};

struct kobject {
    void *ptr;
};

struct bus_type {
    const char        *name;
};

struct device {
//    void *dev;
    AirPortLinux* dev;
    char            name[IFNAMSIZ];
    struct device *parent;
//    AppleIwlWiFi *parent;
    struct kobject kobj; // Device of type IOPCIDevice.
    void *driver_data;
    const struct pci_device_id *ent;
    struct bus_type *bus;
    
    pci_intr_handle *ih;
    u64        *dma_mask;
    void (*cont)(const struct firmware *, void *);
};

static
const char *dev_name(struct device *dev)
{
    return dev->name;
}

static inline void *dev_get_drvdata(const struct device *dev)
{
    return dev->driver_data;
}

static inline void dev_set_drvdata(struct device *dev, void *data)
{
    dev->driver_data = data;
}

/*
 * Managed kmalloc/kfree
 */
static void devm_kmalloc_release(struct device *dev, void *res)
{
    /* noop */
}

static int devm_kmalloc_match(struct device *dev, void *res, void *data)
{
    return res == data;
}

/**
 * devm_kmalloc - Resource-managed kmalloc
 * @dev: Device to allocate memory for
 * @size: Allocation size
 * @gfp: Allocation gfp flags
 *
 * Managed kmalloc.  Memory allocated with this function is
 * automatically freed on driver detach.  Like all other devres
 * resources, guaranteed alignment is unsigned long long.
 *
 * RETURNS:
 * Pointer to allocated memory on success, NULL on failure.
 */
static void *devm_kmalloc(struct device *dev, size_t size, gfp_t gfp)
{
//    struct devres *dr;

//    /* use raw alloc_dr for kmalloc caller tracing */
//    dr = alloc_dr(devm_kmalloc_release, size, gfp, dev_to_node(dev));
//    if (unlikely(!dr))
//        return NULL;
//
//    /*
//     * This is named devm_kzalloc_release for historical reasons
//     * The initial implementation did not support kmalloc, only kzalloc
//     */
//    set_node_dbginfo(&dr->node, "devm_kzalloc_release", size);
//    devres_add(dev, dr->data);
//    return dr->data;
    
    return kzalloc(size, gfp);
}
EXPORT_SYMBOL_GPL(devm_kmalloc);

static inline void *devm_kzalloc(struct device *dev, size_t size, gfp_t gfp)
{
    return devm_kmalloc(dev, size, gfp | __GFP_ZERO);
}

/**
 * devm_kvasprintf - Allocate resource managed space and format a string
 *             into that.
 * @dev: Device to allocate memory for
 * @gfp: the GFP mask used in the devm_kmalloc() call when
 *       allocating memory
 * @fmt: The printf()-style format string
 * @ap: Arguments for the format string
 * RETURNS:
 * Pointer to allocated string on success, NULL on failure.
 */
static
char *devm_kvasprintf(struct device *dev, gfp_t gfp, const char *fmt,
              va_list ap)
{
    unsigned int len;
    char *p;
    va_list aq;

    va_copy(aq, ap);
    len = vsnprintf(NULL, 0, fmt, aq);
    va_end(aq);

    p = (char *)devm_kmalloc(dev, len+1, gfp);
    if (!p)
        return NULL;

    vsnprintf(p, len+1, fmt, ap);

    return p;
}
EXPORT_SYMBOL(devm_kvasprintf);

/**
 * devm_kasprintf - Allocate resource managed space and format a string
 *            into that.
 * @dev: Device to allocate memory for
 * @gfp: the GFP mask used in the devm_kmalloc() call when
 *       allocating memory
 * @fmt: The printf()-style format string
 * @...: Arguments for the format string
 * RETURNS:
 * Pointer to allocated string on success, NULL on failure.
 */
static
char *devm_kasprintf(struct device *dev, gfp_t gfp, const char *fmt, ...)
{
    va_list ap;
    char *p;

    va_start(ap, fmt);
    p = devm_kvasprintf(dev, gfp, fmt, ap);
    va_end(ap);

    return p;
}
EXPORT_SYMBOL_GPL(devm_kasprintf);

int __must_check
devm_request_threaded_irq(struct device *dev, unsigned int irq,
                          irq_handler_t handler, irq_handler_t thread_fn,
                          unsigned long irqflags, const char *devname,
                          void *dev_id);

#endif /* device_h */
