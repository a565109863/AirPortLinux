//
//  pci.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef pci_h
#define pci_h


//#include <linux/mod_devicetable.h>

#include <linux/types.h>
//#include <linux/init.h>
//#include <linux/ioport.h>
#include <linux/list.h>
//#include <linux/compiler.h>
//#include <linux/errno.h>
//#include <linux/kobject.h>
#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/io.h>
//#include <linux/resource_ext.h>
//#include <uapi/linux/pci.h>
//
//#include <linux/pci_ids.h>
#include <linux/pci_regs.h>

#define PCI_VENDOR_ID_INTEL     0x8086
#define PCI_ANY_ID (~0)

/* Error values that may be returned by PCI functions */
#define PCIBIOS_SUCCESSFUL        0x00
#define PCIBIOS_FUNC_NOT_SUPPORTED    0x81
#define PCIBIOS_BAD_VENDOR_ID        0x83
#define PCIBIOS_DEVICE_NOT_FOUND    0x86
#define PCIBIOS_BAD_REGISTER_NUMBER    0x87
#define PCIBIOS_SET_FAILED        0x88
#define PCIBIOS_BUFFER_TOO_SMALL    0x89


#define PCIE_LINK_STATE_L0S        BIT(0)
#define PCIE_LINK_STATE_L1        BIT(1)
#define PCIE_LINK_STATE_CLKPM        BIT(2)
#define PCIE_LINK_STATE_L1_1        BIT(3)
#define PCIE_LINK_STATE_L1_2        BIT(4)
#define PCIE_LINK_STATE_L1_1_PCIPM    BIT(5)
#define PCIE_LINK_STATE_L1_2_PCIPM    BIT(6)


/* The number of legacy PCI INTx interrupts */
#define PCI_NUM_INTX    4

/*
 * pci_power_t values must match the bits in the Capabilities PME_Support
 * and Control/Status PowerState fields in the Power Management capability.
 */
typedef int __bitwise pci_power_t;

/**
 * typedef pci_channel_state_t
 *
 * The pci_channel state describes connectivity between the CPU and
 * the PCI device.  If some PCI bus between here and the PCI device
 * has crashed or locked up, this info is reflected here.
 */
typedef unsigned int __bitwise pci_channel_state_t;

enum pci_channel_state {
    /* I/O channel is in normal state */
    pci_channel_io_normal = (__force pci_channel_state_t) 1,
    
    /* I/O to channel is blocked */
    pci_channel_io_frozen = (__force pci_channel_state_t) 2,
    
    /* PCI card is dead */
    pci_channel_io_perm_failure = (__force pci_channel_state_t) 3,
};

#define PCI_D0        ((pci_power_t __force) 0)
#define PCI_D1        ((pci_power_t __force) 1)
#define PCI_D2        ((pci_power_t __force) 2)
#define PCI_D3hot    ((pci_power_t __force) 3)
#define PCI_D3cold    ((pci_power_t __force) 4)
#define PCI_UNKNOWN    ((pci_power_t __force) 5)
#define PCI_POWER_ERROR    ((pci_power_t __force) -1)


struct msix_entry {
    u32    vector;    /* Kernel uses to write allocated vector */
    u16    entry;    /* Driver uses to specify entry, OS writes */
};

struct pci_driver {
    struct list_head node;
    char *name;
    struct pci_dev *dev;
    const struct pci_device_id *id_table;   /* must be non-NULL for probe to be called */
    int  (*probe)  (struct pci_dev *dev, const struct pci_device_id *id);   /* New device inserted */
    void (*remove) (struct pci_dev *dev);   /* Device removed (NULL if not a hot-plug capable driver) */
    int  (*suspend) (struct pci_dev *dev, void *state);  /* Device suspended */
    int  (*suspend_late) (struct pci_dev *dev, void *state);
    int  (*resume_early) (struct pci_dev *dev);
    int  (*resume) (struct pci_dev *dev);                   /* Device woken up */
    int  (*enable_wake) (struct pci_dev *dev, void *state, int enable);   /* Enable wake event */
    void (*shutdown) (struct pci_dev *dev);
    
    //    struct pci_error_handlers *err_handler;
    //    struct device_driver    driver;
    //    struct pci_dynids dynids;
    
    int multithread_probe;
};


struct pci_dev {
    unsigned long device;
    unsigned long subsystem_device;
    struct device dev;
    int cfg_size;
    u16        pcie_flags_reg;
    u8        pcie_cap;    /* PCIe capability offset */
    u8        msi_cap;    /* MSI capability offset */
    u8        msix_cap;
    u8        pm_cap;
    pci_power_t    current_state;    /* Current operating state. In ACPI, */
    pci_channel_state_t error_state;    /* Current connectivity state */
    unsigned int    pme_support:5;
    unsigned int irq;
    u32 saved_config_space[16];
};

int pci_enable_device(struct pci_dev *pdev);

static
int pcim_enable_device(struct pci_dev *pdev)
{
//    struct pci_devres *dr;
    int rc;
//
//    dr = get_pci_dr(pdev);
//    if (unlikely(!dr))
//        return -ENOMEM;
//    if (dr->enabled)
//        return 0;
    
    rc = pci_enable_device(pdev);
//    if (!rc) {
//        pdev->is_managed = 1;
//        dr->enabled = 1;
//    }
    return rc;
}

int pci_read_config_byte(const struct pci_dev *dev, int where, u8 *val);
int pci_read_config_word(const struct pci_dev *dev, int where, u16 *val);
int pci_read_config_dword(const struct pci_dev *dev, int where, u32 *val);
int pci_write_config_byte(const struct pci_dev *dev, int where, u8 val);
int pci_write_config_word(const struct pci_dev *dev, int where, u16 val);
int pci_write_config_dword(const struct pci_dev *dev, int where, u32 val);


int pci_find_ext_capability(struct pci_dev *dev, int cap);
int pcie_capability_read_word(struct pci_dev *dev, int pos, u16 *val);
int pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries,
                          int minvec, int maxvec);
int pci_enable_msi(struct pci_dev *dev);


void pci_set_master(struct pci_dev *pdev);


/**
 * pci_pcie_cap - get the saved PCIe capability offset
 * @dev: PCI device
 *
 * PCIe capability offset is calculated at PCI device initialization
 * time and saved in the data structure. This function returns saved
 * PCIe capability offset. Using this instead of pci_find_capability()
 * reduces unnecessary search in the PCI configuration space. If you
 * need to calculate PCIe capability offset from raw device for some
 * reasons, please use pci_find_capability() instead.
 */
static inline int pci_pcie_cap(struct pci_dev *dev)
{
    return dev->pcie_cap;
}

/**
 * pci_is_pcie - check if the PCI device is PCI Express capable
 * @dev: PCI device
 *
 * Returns: true if the PCI device is PCI Express capable, false otherwise.
 */
static inline bool pci_is_pcie(struct pci_dev *dev)
{
    return pci_pcie_cap(dev);
}

/**
 * pcie_caps_reg - get the PCIe Capabilities Register
 * @dev: PCI device
 */
static inline u16 pcie_caps_reg(const struct pci_dev *dev)
{
    return dev->pcie_flags_reg;
}

/**
 * pci_pcie_type - get the PCIe device/port type
 * @dev: PCI device
 */
static inline int pci_pcie_type(const struct pci_dev *dev)
{
    return (pcie_caps_reg(dev) & PCI_EXP_FLAGS_TYPE) >> 4;
}

static inline struct pci_dev *pcie_find_root_port(struct pci_dev *dev)
{
    while (1) {
        if (!pci_is_pcie(dev))
            break;
        if (pci_pcie_type(dev) == PCI_EXP_TYPE_ROOT_PORT)
            return dev;
//        if (!dev->bus->self)
//            break;
//        dev = dev->bus->self;
    }
    return NULL;
}

/**
 * pci_pme_capable - check the capability of PCI device to generate PME#
 * @dev: PCI device to handle.
 * @state: PCI state from which device will issue PME#.
 */
static bool pci_pme_capable(struct pci_dev *dev, pci_power_t state)
{
    if (!dev->pm_cap)
        return false;
    
    return !!(dev->pme_support & (1 << state));
}
EXPORT_SYMBOL(pci_pme_capable);


/**
 * pci_dev_set_io_state - Set the new error state if possible.
 *
 * @dev - pci device to set new error_state
 * @new - the state we want dev to be in
 *
 * Must be called with device_lock held.
 *
 * Returns true if state has been changed to the requested state.
 */
static inline bool pci_dev_set_io_state(struct pci_dev *dev,
                                        pci_channel_state_t nnew)
{
    bool changed = false;
    
//    device_lock_assert(&dev->dev);
    switch (nnew) {
        case pci_channel_io_perm_failure:
            switch (dev->error_state) {
                case pci_channel_io_frozen:
                case pci_channel_io_normal:
                case pci_channel_io_perm_failure:
                    changed = true;
                    break;
            }
            break;
        case pci_channel_io_frozen:
            switch (dev->error_state) {
                case pci_channel_io_frozen:
                case pci_channel_io_normal:
                    changed = true;
                    break;
            }
            break;
        case pci_channel_io_normal:
            switch (dev->error_state) {
                case pci_channel_io_frozen:
                case pci_channel_io_normal:
                    changed = true;
                    break;
            }
            break;
    }
    if (changed)
        dev->error_state = nnew;
    return changed;
}

static inline int pci_dev_set_disconnected(struct pci_dev *dev, void *unused)
{
//    device_lock(&dev->dev);
    pci_dev_set_io_state(dev, pci_channel_io_perm_failure);
//    device_unlock(&dev->dev);
    
    return 0;
}

static inline bool pci_dev_is_disconnected(const struct pci_dev *dev)
{
    return dev->error_state == pci_channel_io_perm_failure;
}

static inline int pci_set_dma_mask(struct pci_dev *dev, u64 mask)
{
    return dma_set_mask(&dev->dev, mask);
}


static inline int pci_set_consistent_dma_mask(struct pci_dev *dev, u64 mask)
{
    return 0;
//    return dma_set_coherent_mask(&dev->dev, mask);
}

static int pcim_iomap_regions_request_all(struct pci_dev *pdev, int mask,
                                   const char *name)
{
    return 0;
}

void __iomem * const *pcim_iomap_table(struct pci_dev *pdev);

static struct pci_dev *to_pci_dev(struct device *device) {
    return (struct pci_dev *)device->driver_data;
}

static
int pci_disable_link_state(struct pci_dev *pdev, int state)
{
    return 0;
}


/*
 * Similar to the helpers above, these manipulate per-pci_dev
 * driver-specific data.  They are really just a wrapper around
 * the generic device structure functions of these calls.
 */
static inline void *pci_get_drvdata(struct pci_dev *pdev)
{
    return dev_get_drvdata(&pdev->dev);
}

static inline void pci_set_drvdata(struct pci_dev *pdev, void *data)
{
    dev_set_drvdata(&pdev->dev, data);
}


static inline int pci_register_driver(struct pci_driver *drv, struct pci_dev *pdev)
{
    int err = 0;
    for (int i = 0; i < sizeof(*drv->id_table) / sizeof(const struct pci_device_id); i++) {
        struct iwl_cfg *cfg = (struct iwl_cfg *)drv->id_table[i].driver_data;
        if (drv->id_table[i].device == pdev->device && drv->id_table[i].subdevice == pdev->subsystem_device) {
            pdev->dev.driver_data = (void*)drv->id_table[i].driver_data;
            pdev->dev.ent = &drv->id_table[i];
            err = 0;
            break;
        }
        err = 1;
    }
    
    return err;
}
static inline void pci_unregister_driver(struct pci_driver *drv) { }

#endif /* pci_h */
