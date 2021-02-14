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

#define pci_err(d, arg...)  kprintf(arg)
#define pci_info(d, arg...) kprintf(arg)
#define pci_warn(d, arg...) kprintf(arg)
#define pci_dbg(d, arg...)  kprintf(arg)






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


#define PCI_PM_D2_DELAY         200
#define PCI_PM_D3_WAIT          10
#define PCI_PM_D3COLD_WAIT      100
#define PCI_PM_BUS_WAIT         50





struct msix_entry {
    u32    vector;    /* Kernel uses to write allocated vector */
    u16    entry;    /* Driver uses to specify entry, OS writes */
};

struct msi_desc {
   /* Shared device/bus type independent data */
   struct list_head        list;
   unsigned int            irq;
   unsigned int            nvec_used;
   struct device            *dev;
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

/* The pci_dev structure describes PCI devices */
struct pci_dev {
    struct list_head bus_list;    /* Node in per-bus list */
    struct pci_bus    *bus;        /* Bus this device is on */
    struct pci_bus    *subordinate;    /* Bus this device bridges to */

    void        *sysdata;    /* Hook for sys-specific extension */
    struct proc_dir_entry *procent;    /* Device entry in /proc/bus/pci */
    struct pci_slot    *slot;        /* Physical slot this device is in */

    unsigned int    devfn;        /* Encoded device & function index */
    unsigned short    vendor;
    unsigned short    device;
    unsigned short    subsystem_vendor;
    unsigned short    subsystem_device;
    unsigned int    _class;        /* 3 bytes: (base,sub,prog-if) */
    u8        revision;    /* PCI revision, low byte of class word */
    u8        hdr_type;    /* PCI header type (`multi' flag masked out) */
#ifdef CONFIG_PCIEAER
    u16        aer_cap;    /* AER capability offset */
    struct aer_stats *aer_stats;    /* AER stats for this device */
#endif
    u8        pcie_cap;    /* PCIe capability offset */
    u8        msi_cap;    /* MSI capability offset */
    u8        msix_cap;    /* MSI-X capability offset */
    u8        pcie_mpss:3;    /* PCIe Max Payload Size Supported */
    u8        rom_base_reg;    /* Config register controlling ROM */
    u8        pin;        /* Interrupt pin this device uses */
    u16        pcie_flags_reg;    /* Cached PCIe Capabilities Register */
    unsigned long    *dma_alias_mask;/* Mask of enabled devfn aliases */

    struct pci_driver *driver;    /* Driver bound to this device */
    u64        dma_mask;    /* Mask of the bits of bus address this
                       device implements.  Normally this is
                       0xffffffff.  You only need to change
                       this if your device has broken DMA
                       or supports 64-bit transfers.  */

//    struct device_dma_parameters dma_parms;

    pci_power_t    current_state;    /* Current operating state. In ACPI,
                       this is D0-D3, D0 being fully
                       functional, and D3 being off. */
    unsigned int    imm_ready:1;    /* Supports Immediate Readiness */
    u8        pm_cap;        /* PM capability offset */
    unsigned int    pme_support:5;    /* Bitmask of states from which PME#
                       can be generated */
    unsigned int    pme_poll:1;    /* Poll device's PME status bit */
    unsigned int    d1_support:1;    /* Low power state D1 is supported */
    unsigned int    d2_support:1;    /* Low power state D2 is supported */
    unsigned int    no_d1d2:1;    /* D1 and D2 are forbidden */
    unsigned int    no_d3cold:1;    /* D3cold is forbidden */
    unsigned int    bridge_d3:1;    /* Allow D3 for bridge */
    unsigned int    d3cold_allowed:1;    /* D3cold is allowed by user */
    unsigned int    mmio_always_on:1;    /* Disallow turning off io/mem
                           decoding during BAR sizing */
    unsigned int    wakeup_prepared:1;
    unsigned int    runtime_d3cold:1;    /* Whether go through runtime
                           D3cold, not set for devices
                           powered on/off by the
                           corresponding bridge */
    unsigned int    skip_bus_pm:1;    /* Internal: Skip bus-level PM */
    unsigned int    ignore_hotplug:1;    /* Ignore hotplug events */
    unsigned int    hotplug_user_indicators:1; /* SlotCtl indicators
                              controlled exclusively by
                              user sysfs */
    unsigned int    clear_retrain_link:1;    /* Need to clear Retrain Link
                           bit manually */
    unsigned int    d3_delay;    /* D3->D0 transition time in ms */
    unsigned int    d3cold_delay;    /* D3cold->D0 transition time in ms */

#ifdef CONFIG_PCIEASPM
    struct pcie_link_state    *link_state;    /* ASPM link state */
    unsigned int    ltr_path:1;    /* Latency Tolerance Reporting
                       supported from root to here */
#endif
    unsigned int    eetlp_prefix_path:1;    /* End-to-End TLP Prefix */

    pci_channel_state_t error_state;    /* Current connectivity state */
    struct device    dev;            /* Generic device interface */

    int        cfg_size;        /* Size of config space */

    /*
     * Instead of touching interrupt line and base address registers
     * directly, use the values stored here. They might be different!
     */
    unsigned int    irq;
//    struct resource resource[DEVICE_COUNT_RESOURCE]; /* I/O and memory regions + expansion ROMs */

    bool        match_driver;        /* Skip attaching driver */

    unsigned int    transparent:1;        /* Subtractive decode bridge */
    unsigned int    io_window:1;        /* Bridge has I/O window */
    unsigned int    pref_window:1;        /* Bridge has pref mem window */
    unsigned int    pref_64_window:1;    /* Pref mem window is 64-bit */
    unsigned int    multifunction:1;    /* Multi-function device */

    unsigned int    is_busmaster:1;        /* Is busmaster */
    unsigned int    no_msi:1;        /* May not use MSI */
    unsigned int    no_64bit_msi:1;        /* May only use 32-bit MSIs */
    unsigned int    block_cfg_access:1;    /* Config space access blocked */
    unsigned int    broken_parity_status:1;    /* Generates false positive parity */
    unsigned int    irq_reroute_variant:2;    /* Needs IRQ rerouting variant */
    unsigned int    msi_enabled:1;
    unsigned int    msix_enabled:1;
//    unsigned int    ari_enabled:1;        /* ARI forwarding */
//    unsigned int    ats_enabled:1;        /* Address Translation Svc */
//    unsigned int    pasid_enabled:1;    /* Process Address Space ID */
//    unsigned int    pri_enabled:1;        /* Page Request Interface */
//    unsigned int    is_managed:1;
//    unsigned int    needs_freset:1;        /* Requires fundamental reset */
//    unsigned int    state_saved:1;
//    unsigned int    is_physfn:1;
//    unsigned int    is_virtfn:1;
//    unsigned int    reset_fn:1;
//    unsigned int    is_hotplug_bridge:1;
//    unsigned int    shpc_managed:1;        /* SHPC owned by shpchp */
//    unsigned int    is_thunderbolt:1;    /* Thunderbolt controller */
    /*
     * Devices marked being untrusted are the ones that can potentially
     * execute DMA attacks and similar. They are typically connected
     * through external ports such as Thunderbolt but not limited to
     * that. When an IOMMU is enabled they should be getting full
     * mappings to make sure they cannot access arbitrary memory.
     */
    unsigned int    untrusted:1;
    unsigned int    __aer_firmware_first_valid:1;
    unsigned int    __aer_firmware_first:1;
    unsigned int    broken_intx_masking:1;    /* INTx masking can't be used */
    unsigned int    io_window_1k:1;        /* Intel bridge 1K I/O windows */
    unsigned int    irq_managed:1;
    unsigned int    non_compliant_bars:1;    /* Broken BARs; ignore them */
    unsigned int    is_probed:1;        /* Device probing in progress */
    unsigned int    link_active_reporting:1;/* Device capable of reporting link active */
    unsigned int    no_vf_scan:1;        /* Don't scan for VFs after IOV enablement */
//    pci_dev_flags_t dev_flags;
    atomic_t    enable_cnt;    /* pci_enable_device has been called */

    u32        saved_config_space[16]; /* Config space saved at suspend time */
    struct hlist_head saved_cap_space;
    struct bin_attribute *rom_attr;        /* Attribute descriptor for sysfs ROM entry */
    int        rom_attr_enabled;    /* Display of ROM attribute enabled? */
//    struct bin_attribute *res_attr[DEVICE_COUNT_RESOURCE]; /* sysfs file for resources */
//    struct bin_attribute *res_attr_wc[DEVICE_COUNT_RESOURCE]; /* sysfs file for WC mapping of resources */

#ifdef CONFIG_HOTPLUG_PCI_PCIE
    unsigned int    broken_cmd_compl:1;    /* No compl for some cmds */
#endif
#ifdef CONFIG_PCIE_PTM
    unsigned int    ptm_root:1;
    unsigned int    ptm_enabled:1;
    u8        ptm_granularity;
#endif
#ifdef CONFIG_PCI_MSI
    const struct attribute_group **msi_irq_groups;
#endif
    struct pci_vpd *vpd;
#ifdef CONFIG_PCI_ATS
    union {
        struct pci_sriov    *sriov;        /* PF: SR-IOV info */
        struct pci_dev        *physfn;    /* VF: related PF */
    };
    u16        ats_cap;    /* ATS Capability offset */
    u8        ats_stu;    /* ATS Smallest Translation Unit */
#endif
#ifdef CONFIG_PCI_PRI
    u16        pri_cap;    /* PRI Capability offset */
    u32        pri_reqs_alloc; /* Number of PRI requests allocated */
    unsigned int    pasid_required:1; /* PRG Response PASID Required */
#endif
#ifdef CONFIG_PCI_PASID
    u16        pasid_cap;    /* PASID Capability offset */
    u16        pasid_features;
#endif
#ifdef CONFIG_PCI_P2PDMA
    struct pci_p2pdma *p2pdma;
#endif
    phys_addr_t    rom;        /* Physical address if not from BAR */
    size_t        romlen;        /* Length if not from BAR */
    char        *driver_override; /* Driver name to force a match */

    unsigned long    priv_flags;    /* Private flags for the PCI driver */
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
//    while (1) {
//        if (!pci_is_pcie(dev))
//            break;
//        if (pci_pcie_type(dev) == PCI_EXP_TYPE_ROOT_PORT)
//            return dev;
////        if (!dev->bus->self)
////            break;
////        dev = dev->bus->self;
//    }
    return dev;
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
                                        pci_channel_state_t _new)
{
    bool changed = false;
    
//    device_lock_assert(&dev->dev);
    switch (_new) {
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
        dev->error_state = _new;
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
    return dma_set_mask(&dev->dev, mask);
//    return dma_set_coherent_mask(&dev->dev, mask);
}

static int pcim_iomap_regions_request_all(struct pci_dev *pdev, int mask,
                                   const char *name)
{
    return 0;
}

void __iomem * const *pcim_iomap_table(struct pci_dev *pdev);

#define    to_pci_dev(n) container_of(n, struct pci_dev, dev)

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


/**
 * pci_match_one_device - Tell if a PCI device structure has a matching
 *              PCI device id structure
 * @id: single PCI device id structure to match
 * @dev: the PCI device structure to match against
 *
 * Returns the matching pci_device_id structure or %NULL if there is no match.
 */
static inline const struct pci_device_id *
pci_match_one_device(const struct pci_device_id *_id, const struct pci_dev *dev)
{
    if ((_id->vendor == PCI_ANY_ID || _id->vendor == dev->vendor) &&
        (_id->device == PCI_ANY_ID || _id->device == dev->device) &&
        (_id->subvendor == PCI_ANY_ID || _id->subvendor == dev->subsystem_vendor) &&
        (_id->subdevice == PCI_ANY_ID || _id->subdevice == dev->subsystem_device))
        return _id;
    return NULL;
}

/**
 * pci_match_id - See if a pci device matches a given pci_id table
 * @ids: array of PCI device id structures to search in
 * @dev: the PCI device structure to match against.
 *
 * Used by a driver to check whether a PCI device present in the
 * system is in its list of supported devices.  Returns the matching
 * pci_device_id structure or %NULL if there is no match.
 *
 * Deprecated, don't use this as it will not catch any dynamic ids
 * that a driver might want to check for.
 */
static
const struct pci_device_id *pci_match_id(const struct pci_device_id *ids,
                                         struct pci_dev *dev)
{
    if (ids) {
        while (ids->vendor || ids->subvendor || ids->class_mask) {
            if (pci_match_one_device(ids, dev))
                return ids;
            ids++;
        }
    }
    return NULL;
}
EXPORT_SYMBOL(pci_match_id);

void pci_init_capabilities(struct pci_dev *dev);

static inline int pci_register_driver(struct pci_driver *drv, struct pci_dev *pdev)
{
    int err = 0;
    
    const struct pci_device_id *found_id = pci_match_id(drv->id_table, pdev);
    if (!found_id) {
        err = 1;
        return err;
    }
    
    pdev->dev.ent = found_id;
    
    pci_init_capabilities(pdev);
    
    return err;
    
}
static inline void pci_unregister_driver(struct pci_driver *drv) { }

#endif /* pci_h */
