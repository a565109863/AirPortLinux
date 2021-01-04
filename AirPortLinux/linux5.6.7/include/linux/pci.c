//
//  pci.c
//  AirPortLinux
//
//  Created by Zhong-Mac on 2020/5/14.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "AirPortLinux.hpp"
#include "pci.h"


int pci_read_config_byte(const struct pci_dev *dev, int where, u8 *val)
{
    if (pci_dev_is_disconnected(dev)) {
        *val = ~0;
        return PCIBIOS_DEVICE_NOT_FOUND;
    }
//    return pci_bus_read_config_byte(dev->bus, dev->devfn, where, val);
    
    *val = dev->dev.dev->fPciDevice->configRead8(where);
    return 0;
}
EXPORT_SYMBOL(pci_read_config_byte);

int pci_read_config_word(const struct pci_dev *dev, int where, u16 *val)
{
    if (pci_dev_is_disconnected(dev)) {
        *val = ~0;
        return PCIBIOS_DEVICE_NOT_FOUND;
    }
//    return pci_bus_read_config_word(dev->bus, dev->devfn, where, val);
    
    *val = dev->dev.dev->fPciDevice->configRead16(where);
    return 0;
}
EXPORT_SYMBOL(pci_read_config_word);

int pci_read_config_dword(const struct pci_dev *dev, int where,
                          u32 *val)
{
    if (pci_dev_is_disconnected(dev)) {
        *val = ~0;
        return PCIBIOS_DEVICE_NOT_FOUND;
    }
//    return pci_bus_read_config_dword(dev->bus, dev->devfn, where, val);
    *val = dev->dev.dev->fPciDevice->configRead32(where);
    return 0;
}
EXPORT_SYMBOL(pci_read_config_dword);


int pci_write_config_byte(const struct pci_dev *dev, int where, u8 val)
{
    if (pci_dev_is_disconnected(dev))
        return PCIBIOS_DEVICE_NOT_FOUND;
//    return pci_bus_write_config_byte(dev->bus, dev->devfn, where, val);
    dev->dev.dev->fPciDevice->configWrite8(where,val);
    return 0;
}
EXPORT_SYMBOL(pci_write_config_byte);

int pci_write_config_word(const struct pci_dev *dev, int where, u16 val)
{
    if (pci_dev_is_disconnected(dev))
        return PCIBIOS_DEVICE_NOT_FOUND;
//    return pci_bus_write_config_word(dev->bus, dev->devfn, where, val);
    dev->dev.dev->fPciDevice->configWrite16(where,val);
    return 0;
}
EXPORT_SYMBOL(pci_write_config_word);

int pci_write_config_dword(const struct pci_dev *dev, int where,
                           u32 val)
{
    if (pci_dev_is_disconnected(dev))
        return PCIBIOS_DEVICE_NOT_FOUND;
//    return pci_bus_write_config_dword(dev->bus, dev->devfn, where, val);
    dev->dev.dev->fPciDevice->configWrite32(where,val);
    return 0;
}
EXPORT_SYMBOL(pci_write_config_dword);


/**
 * pci_find_next_ext_capability - Find an extended capability
 * @dev: PCI device to query
 * @start: address at which to start looking (0 to start at beginning of list)
 * @cap: capability code
 *
 * Returns the address of the next matching extended capability structure
 * within the device's PCI configuration space or 0 if the device does
 * not support it.  Some capabilities can occur several times, e.g., the
 * vendor-specific capability, and this provides a way to find them all.
 */
int pci_find_next_ext_capability(struct pci_dev *dev, int start, int cap)
{
    u32 header;
    int ttl;
    int pos = PCI_CFG_SPACE_SIZE;
    
    /* minimum 8 bytes per capability */
    ttl = (PCI_CFG_SPACE_EXP_SIZE - PCI_CFG_SPACE_SIZE) / 8;
    
    if (dev->cfg_size <= PCI_CFG_SPACE_SIZE)
        return 0;
    
    if (start)
        pos = start;
    
    if (pci_read_config_dword(dev, pos, &header) != PCIBIOS_SUCCESSFUL)
        return 0;
    
    /*
     * If we have no capabilities, this is indicated by cap ID,
     * cap version and next pointer all being 0.
     */
    if (header == 0)
        return 0;
    
    while (ttl-- > 0) {
        if (PCI_EXT_CAP_ID(header) == cap && pos != start)
            return pos;
        
        pos = PCI_EXT_CAP_NEXT(header);
        if (pos < PCI_CFG_SPACE_SIZE)
            break;
        
        if (pci_read_config_dword(dev, pos, &header) != PCIBIOS_SUCCESSFUL)
            break;
    }
    
    return 0;
}
EXPORT_SYMBOL_GPL(pci_find_next_ext_capability);


int pci_find_ext_capability(struct pci_dev *dev, int cap)
{
    return pci_find_next_ext_capability(dev, 0, cap);
}



static inline int pcie_cap_version(const struct pci_dev *dev)
{
    return pcie_caps_reg(dev) & PCI_EXP_FLAGS_VERS;
}

static bool pcie_downstream_port(const struct pci_dev *dev)
{
    int type = pci_pcie_type(dev);
    
    return type == PCI_EXP_TYPE_ROOT_PORT ||
    type == PCI_EXP_TYPE_DOWNSTREAM ||
    type == PCI_EXP_TYPE_PCIE_BRIDGE;
}

bool pcie_cap_has_lnkctl(const struct pci_dev *dev)
{
    int type = pci_pcie_type(dev);
    
    return type == PCI_EXP_TYPE_ENDPOINT ||
    type == PCI_EXP_TYPE_LEG_END ||
    type == PCI_EXP_TYPE_ROOT_PORT ||
    type == PCI_EXP_TYPE_UPSTREAM ||
    type == PCI_EXP_TYPE_DOWNSTREAM ||
    type == PCI_EXP_TYPE_PCI_BRIDGE ||
    type == PCI_EXP_TYPE_PCIE_BRIDGE;
}

static inline bool pcie_cap_has_sltctl(const struct pci_dev *dev)
{
    return pcie_downstream_port(dev) &&
    pcie_caps_reg(dev) & PCI_EXP_FLAGS_SLOT;
}

static inline bool pcie_cap_has_rtctl(const struct pci_dev *dev)
{
    int type = pci_pcie_type(dev);
    
    return type == PCI_EXP_TYPE_ROOT_PORT ||
    type == PCI_EXP_TYPE_RC_EC;
}

static bool pcie_capability_reg_implemented(struct pci_dev *dev, int pos)
{
    if (!pci_is_pcie(dev))
        return false;
    
    switch (pos) {
        case PCI_EXP_FLAGS:
            return true;
        case PCI_EXP_DEVCAP:
        case PCI_EXP_DEVCTL:
        case PCI_EXP_DEVSTA:
            return true;
        case PCI_EXP_LNKCAP:
        case PCI_EXP_LNKCTL:
        case PCI_EXP_LNKSTA:
            return pcie_cap_has_lnkctl(dev);
        case PCI_EXP_SLTCAP:
        case PCI_EXP_SLTCTL:
        case PCI_EXP_SLTSTA:
            return pcie_cap_has_sltctl(dev);
        case PCI_EXP_RTCTL:
        case PCI_EXP_RTCAP:
        case PCI_EXP_RTSTA:
            return pcie_cap_has_rtctl(dev);
        case PCI_EXP_DEVCAP2:
        case PCI_EXP_DEVCTL2:
        case PCI_EXP_LNKCAP2:
        case PCI_EXP_LNKCTL2:
        case PCI_EXP_LNKSTA2:
            return pcie_cap_version(dev) > 1;
        default:
            return false;
    }
}

/*
 * Note that these accessor functions are only for the "PCI Express
 * Capability" (see PCIe spec r3.0, sec 7.8).  They do not apply to the
 * other "PCI Express Extended Capabilities" (AER, VC, ACS, MFVC, etc.)
 */
int pcie_capability_read_word(struct pci_dev *dev, int pos, u16 *val)
{
    int ret;
    
    *val = 0;
    if (pos & 1)
        return -EINVAL;
    
    if (pcie_capability_reg_implemented(dev, pos)) {
        ret = pci_read_config_word(dev, pci_pcie_cap(dev) + pos, val);
        /*
         * Reset *val to 0 if pci_read_config_word() fails, it may
         * have been written as 0xFFFF if hardware error happens
         * during pci_read_config_word().
         */
        if (ret)
            *val = 0;
        return ret;
    }
    
    /*
     * For Functions that do not implement the Slot Capabilities,
     * Slot Status, and Slot Control registers, these spaces must
     * be hardwired to 0b, with the exception of the Presence Detect
     * State bit in the Slot Status register of Downstream Ports,
     * which must be hardwired to 1b.  (PCIe Base Spec 3.0, sec 7.8)
     */
    if (pci_is_pcie(dev) && pcie_downstream_port(dev) &&
        pos == PCI_EXP_SLTSTA)
        *val = PCI_EXP_SLTSTA_PDS;
    
    return 0;
}
EXPORT_SYMBOL(pcie_capability_read_word);


#define    IRQ_AFFINITY_MAX_SETS  4

struct irq_affinity {
   unsigned int    pre_vectors;
   unsigned int    post_vectors;
   unsigned int    nr_sets;
   unsigned int    set_size[IRQ_AFFINITY_MAX_SETS];
   void        (*calc_sets)(struct irq_affinity *, unsigned int nvecs);
   void        *priv;
};

static inline unsigned int
irq_calc_affinity_vectors(unsigned int minvec, unsigned int maxvec,
              const struct irq_affinity *affd)
{
    return maxvec;
}



/*
 * Virtual interrupts allow for more interrupts to be allocated
 * than the device has interrupts for. These are not programmed
 * into the device's MSI-X table and must be handled by some
 * other driver means.
 */
#define PCI_IRQ_VIRTUAL        (1 << 4)

#define msix_table_size(flags)    ((flags & PCI_MSIX_FLAGS_QSIZE) + 1)

/**
 * pci_msix_vec_count - return the number of device's MSI-X table entries
 * @dev: pointer to the pci_dev data structure of MSI-X device function
 * This function returns the number of device's MSI-X table entries and
 * therefore the number of MSI-X vectors device is capable of sending.
 * It returns a negative errno if the device is not capable of sending MSI-X
 * interrupts.
 **/
int pci_msix_vec_count(struct pci_dev *dev)
{
    u16 control;

    if (!dev->msix_cap)
        return -EINVAL;

    pci_read_config_word(dev, dev->msix_cap + PCI_MSIX_FLAGS, &control);
    return msix_table_size(control);
}
EXPORT_SYMBOL(pci_msix_vec_count);


static inline void pci_msix_clear_and_set_ctrl(struct pci_dev *dev, u16 clear, u16 set)
{
    u16 ctrl;

    pci_read_config_word(dev, dev->msix_cap + PCI_MSIX_FLAGS, &ctrl);
    ctrl &= ~clear;
    ctrl |= set;
    pci_write_config_word(dev, dev->msix_cap + PCI_MSIX_FLAGS, ctrl);
}


/**
 * msix_capability_init - configure device's MSI-X capability
 * @dev: pointer to the pci_dev data structure of MSI-X device function
 * @entries: pointer to an array of struct msix_entry entries
 * @nvec: number of @entries
 * @affd: Optional pointer to enable automatic affinity assignment
 *
 * Setup the MSI-X capability structure of device function with a
 * single MSI-X IRQ. A return of zero indicates the successful setup of
 * requested MSI-X entries with allocated IRQs or non-zero for otherwise.
 **/
static int msix_capability_init(struct pci_dev *dev, struct msix_entry *entries,
                int nvec, struct irq_affinity *affd)
{
    /* Ensure MSI-X is disabled while it is set up */
    pci_msix_clear_and_set_ctrl(dev, PCI_MSIX_FLAGS_ENABLE, 0);

    dev->msix_enabled = 1;
    pci_msix_clear_and_set_ctrl(dev, PCI_MSIX_FLAGS_MASKALL, 0);

    return 0;
}

static int __pci_enable_msix(struct pci_dev *dev, struct msix_entry *entries,
                 int nvec, struct irq_affinity *affd, int flags)
{
//    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    
    int nr_entries;
    int i, j;

//    if (!pci_msi_supported(dev, nvec) || dev->current_state != PCI_D0)
//        return -EINVAL;

    nr_entries = pci_msix_vec_count(dev);
    if (nr_entries < 0)
        return nr_entries;
    if (nvec > nr_entries && !(flags & PCI_IRQ_VIRTUAL))
        return nr_entries;

    if (entries) {
        /* Check for any invalid entries */
//        for (i = 0; i < nvec; i++) {
//            if (entries[i].entry >= nr_entries)
//                return -EINVAL;        /* invalid entry */
//            for (j = i + 1; j < nvec; j++) {
//                if (entries[i].entry == entries[j].entry)
//                    return -EINVAL;    /* duplicate entry */
//            }
//        }
        
        IOReturn ret;
        int count = 0;
        for (i = 0; i < nr_entries ; i++)
        {
            int interruptType;
            ret = dev->dev.dev->fPciDevice->getInterruptType(i, &interruptType);
            if (ret != kIOReturnSuccess)
                continue;
            
#ifndef kIOInterruptTypePCIMessagedX
#define kIOInterruptTypePCIMessagedX 0x00020000
#endif
            if (interruptType & kIOInterruptTypePCIMessagedX)
            {
                DebugLog("--%s: line = %d i = %d", __FUNCTION__, __LINE__, i);
                DebugLog("--%s: line = %d i = %d", __FUNCTION__, __LINE__, i);
                DebugLog("--%s: line = %d i = %d", __FUNCTION__, __LINE__, i);
                DebugLog("--%s: line = %d i = %d", __FUNCTION__, __LINE__, i);
                entries[count++].entry = i;
            }
        }
        if (count == 0)
            return -EINVAL;
        
        kprintf("--%s: line = %d count = %d", __FUNCTION__, __LINE__, count);
        if (nvec > count)
            return count;
    }

    /* Check whether driver already requested for MSI IRQ */
    if (dev->msi_enabled) {
        pci_info(dev, "can't enable MSI-X (MSI IRQ already assigned)\n");
        return -EINVAL;
    }
    return msix_capability_init(dev, entries, nvec, affd);
}

static int __pci_enable_msix_range(struct pci_dev *dev,
                   struct msix_entry *entries, int minvec,
                   int maxvec, struct irq_affinity *affd,
                   int flags)
{
    int rc, nvec = maxvec;

    if (maxvec < minvec)
        return -ERANGE;

    if (WARN_ON_ONCE(dev->msix_enabled))
        return -EINVAL;

    for (;;) {
        if (affd) {
            nvec = irq_calc_affinity_vectors(minvec, nvec, affd);
            if (nvec < minvec)
                return -ENOSPC;
        }

        rc = __pci_enable_msix(dev, entries, nvec, affd, flags);
        if (rc == 0)
            return nvec;

        if (rc < 0)
            return rc;
        if (rc < minvec)
            return -ENOSPC;

        nvec = rc;
    }
}

/**
 * pci_enable_msix_range - configure device's MSI-X capability structure
 * @dev: pointer to the pci_dev data structure of MSI-X device function
 * @entries: pointer to an array of MSI-X entries
 * @minvec: minimum number of MSI-X IRQs requested
 * @maxvec: maximum number of MSI-X IRQs requested
 *
 * Setup the MSI-X capability structure of device function with a maximum
 * possible number of interrupts in the range between @minvec and @maxvec
 * upon its software driver call to request for MSI-X mode enabled on its
 * hardware device function. It returns a negative errno if an error occurs.
 * If it succeeds, it returns the actual number of interrupts allocated and
 * indicates the successful configuration of MSI-X capability structure
 * with new allocated MSI-X interrupts.
 **/
int pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries,
                          int minvec, int maxvec)
{
    return __pci_enable_msix_range(dev, entries, minvec, maxvec, NULL, 0);
}
EXPORT_SYMBOL(pci_enable_msix_range);

/**
 * pci_msi_vec_count - Return the number of MSI vectors a device can send
 * @dev: device to report about
 *
 * This function returns the number of MSI vectors a device requested via
 * Multiple Message Capable register. It returns a negative errno if the
 * device is not capable sending MSI interrupts. Otherwise, the call succeeds
 * and returns a power of two, up to a maximum of 2^5 (32), according to the
 * MSI specification.
 **/
int pci_msi_vec_count(struct pci_dev *dev)
{
    int ret;
    u16 msgctl;

    if (!dev->msi_cap)
        return -EINVAL;

    pci_read_config_word(dev, dev->msi_cap + PCI_MSI_FLAGS, &msgctl);
    ret = 1 << ((msgctl & PCI_MSI_FLAGS_QMASK) >> 1);

    return ret;
}
EXPORT_SYMBOL(pci_msi_vec_count);


static inline void pci_msi_set_enable(struct pci_dev *dev, int enable)
{
    u16 control;

    pci_read_config_word(dev, dev->msi_cap + PCI_MSI_FLAGS, &control);
    control &= ~PCI_MSI_FLAGS_ENABLE;
    if (enable)
        control |= PCI_MSI_FLAGS_ENABLE;
    pci_write_config_word(dev, dev->msi_cap + PCI_MSI_FLAGS, control);
}

/**
 * alloc_msi_entry - Allocate an initialize msi_entry
 * @dev:    Pointer to the device for which this is allocated
 * @nvec:    The number of vectors used in this entry
 * @affinity:    Optional pointer to an affinity mask array size of @nvec
 *
 * If @affinity is not NULL then an affinity array[@nvec] is allocated
 * and the affinity masks and flags from @affinity are copied.
 */
struct msi_desc *alloc_msi_entry(struct device *dev, int nvec,
                 const struct irq_affinity_desc *affinity)
{
    struct msi_desc *desc;

    desc = (struct msi_desc *)kzalloc(sizeof(*desc), GFP_KERNEL);
    if (!desc)
        return NULL;

    INIT_LIST_HEAD(&desc->list);
    desc->dev = dev;
    desc->nvec_used = nvec;
//    if (affinity) {
//        desc->affinity = kmemdup(affinity,
//            nvec * sizeof(*desc->affinity), GFP_KERNEL);
//        if (!desc->affinity) {
//            kfree(desc);
//            return NULL;
//        }
//    }

    return desc;
}

static struct msi_desc *
msi_setup_entry(struct pci_dev *dev, int nvec, struct irq_affinity *affd)
{
    struct irq_affinity_desc *masks = NULL;
    struct msi_desc *entry;
    u16 control;

//    if (affd)
//        masks = irq_create_affinity_masks(nvec, affd);

    /* MSI Entry Initialization */
    entry = alloc_msi_entry(&dev->dev, nvec, masks);
    if (!entry)
        goto out;

    pci_read_config_word(dev, dev->msi_cap + PCI_MSI_FLAGS, &control);

//    entry->msi_attrib.is_msix    = 0;
//    entry->msi_attrib.is_64        = !!(control & PCI_MSI_FLAGS_64BIT);
//    entry->msi_attrib.is_virtual    = 0;
//    entry->msi_attrib.entry_nr    = 0;
//    entry->msi_attrib.maskbit    = !!(control & PCI_MSI_FLAGS_MASKBIT);
//    entry->msi_attrib.default_irq    = dev->irq;    /* Save IOAPIC IRQ */
//    entry->msi_attrib.multi_cap    = (control & PCI_MSI_FLAGS_QMASK) >> 1;
//    entry->msi_attrib.multiple    = ilog2(__roundup_pow_of_two(nvec));
//
//    if (control & PCI_MSI_FLAGS_64BIT)
//        entry->mask_pos = dev->msi_cap + PCI_MSI_MASK_64;
//    else
//        entry->mask_pos = dev->msi_cap + PCI_MSI_MASK_32;
//
//    /* Save the initial mask status */
//    if (entry->msi_attrib.maskbit)
//        pci_read_config_dword(dev, entry->mask_pos, &entry->masked);

out:
//    kfree(masks);
    return entry;
}
/**
 * msi_capability_init - configure device's MSI capability structure
 * @dev: pointer to the pci_dev data structure of MSI device function
 * @nvec: number of interrupts to allocate
 * @affd: description of automatic IRQ affinity assignments (may be %NULL)
 *
 * Setup the MSI capability structure of the device with the requested
 * number of interrupts.  A return value of zero indicates the successful
 * setup of an entry with the new MSI IRQ.  A negative return value indicates
 * an error, and a positive return value indicates the number of interrupts
 * which could have been allocated.
 */
static int msi_capability_init(struct pci_dev *dev, int nvec,
                   struct irq_affinity *affd)
{
    struct msi_desc *entry;
    int i;
//    unsigned mask;

    pci_msi_set_enable(dev, 0);    /* Disable MSI during set up */

    entry = msi_setup_entry(dev, nvec, affd);
    if (!entry)
        return -ENOMEM;

//    /* All MSIs are unmasked by default; mask them all */
//    mask = msi_mask(entry->msi_attrib.multi_cap);
//    msi_mask_irq(entry, mask, mask);
//
//    list_add_tail(&entry->list, dev_to_msi_list(&dev->dev));
//
//    /* Configure MSI capability structure */
//    ret = pci_msi_setup_msi_irqs(dev, nvec, PCI_CAP_ID_MSI);
//    if (ret) {
//        msi_mask_irq(entry, mask, ~mask);
//        free_msi_irqs(dev);
//        return ret;
//    }
//
//    ret = msi_verify_entries(dev);
//    if (ret) {
//        msi_mask_irq(entry, mask, ~mask);
//        free_msi_irqs(dev);
//        return ret;
//    }
//
//    ret = populate_msi_sysfs(dev);
//    if (ret) {
//        msi_mask_irq(entry, mask, ~mask);
//        free_msi_irqs(dev);
//        return ret;
//    }
    
    IOReturn ret;
    for (i = 0; ; i++)
    {
        int interruptType;
        ret = dev->dev.dev->fPciDevice->getInterruptType(i, &interruptType);
        if (ret != kIOReturnSuccess)
            break;
            
        if (interruptType & kIOInterruptTypePCIMessaged)
        {
            entry->irq = i;
            break;
        }
    }

    /* Set MSI enabled bits    */
//    pci_intx_for_msi(dev, 0);
    pci_msi_set_enable(dev, 1);
    
    dev->msi_enabled = 1;

//    pcibios_free_irq(dev);
    dev->irq = entry->irq;
    return 0;
}

static int __pci_enable_msi_range(struct pci_dev *dev, int minvec, int maxvec,
                  struct irq_affinity *affd)
{
    int nvec;
    int rc;

//    if (!pci_msi_supported(dev, minvec) || dev->current_state != PCI_D0)
//        return -EINVAL;

    /* Check whether driver already requested MSI-X IRQs */
    if (dev->msix_enabled) {
        pci_info(dev, "can't enable MSI (MSI-X already enabled)\n");
        return -EINVAL;
    }
    
    if (maxvec < minvec)
        return -ERANGE;

    if (WARN_ON_ONCE(dev->msi_enabled))
        return -EINVAL;

    nvec = pci_msi_vec_count(dev);
    
    if (nvec < 0)
        return nvec;
    if (nvec < minvec)
        return -ENOSPC;

    if (nvec > maxvec)
        nvec = maxvec;

    for (;;) {
        if (affd) {
            nvec = irq_calc_affinity_vectors(minvec, nvec, affd);
            if (nvec < minvec)
                return -ENOSPC;
        }

        rc = msi_capability_init(dev, nvec, affd);
        if (rc == 0)
            return nvec;

        if (rc < 0)
            return rc;
        if (rc < minvec)
            return -ENOSPC;

        nvec = rc;
    }
}

/* deprecated, don't use */
int pci_enable_msi(struct pci_dev *dev)
{
    int rc = __pci_enable_msi_range(dev, 1, 1, NULL);
    if (rc < 0)
        return rc;
    return 0;
}
EXPORT_SYMBOL(pci_enable_msi);

int pci_enable_device(struct pci_dev *dev)
{
    dev->dev.dev->fPciDevice->setMemoryEnable(true);
    dev->dev.dev->fPciDevice->setIOEnable(true);
    return 0;
//    return pci_enable_device_flags(dev, IORESOURCE_MEM | IORESOURCE_IO);
}

void pci_set_master(struct pci_dev *dev)
{
    dev->dev.dev->fPciDevice->setBusMasterEnable(true);
}


void __iomem * const *pcim_iomap_table(struct pci_dev *pdev)
{
    struct pcim_iomap_devres *dr, *new_dr;
//
//    dr = devres_find(&pdev->dev, pcim_iomap_release, NULL, NULL);
//    if (dr)
//        return dr->table;
//
//    new_dr = devres_alloc(pcim_iomap_release, sizeof(*new_dr), GFP_KERNEL);
//    if (!new_dr)
//        return NULL;
//    dr = devres_get(&pdev->dev, new_dr, NULL, NULL);
//    return dr->table;
    
    dr = new struct pcim_iomap_devres;
    IOPCIDevice* fPciDevice = pdev->dev.dev->fPciDevice;
    
    for(UInt32 i = 0; i < fPciDevice->getDeviceMemoryCount(); i++) {
        IODeviceMemory *desc = fPciDevice->getDeviceMemoryWithIndex(i);
        IOMemoryMap* map  = desc->map();
        if (map == 0)
            return NULL;
        
        dr->table[i] = reinterpret_cast<caddr_t>(map->getVirtualAddress());
        
    }
    
    return dr->table;
}


/**
 * pci_find_capability - query for devices' capabilities
 * @dev: PCI device to query
 * @cap: capability code
 *
 * Tell if a device supports a given PCI capability.
 * Returns the address of the requested capability structure within the
 * device's PCI configuration space or 0 in case the device does not
 * support it.  Possible values for @cap include:
 *
 *  %PCI_CAP_ID_PM           Power Management
 *  %PCI_CAP_ID_AGP          Accelerated Graphics Port
 *  %PCI_CAP_ID_VPD          Vital Product Data
 *  %PCI_CAP_ID_SLOTID       Slot Identification
 *  %PCI_CAP_ID_MSI          Message Signalled Interrupts
 *  %PCI_CAP_ID_CHSWP        CompactPCI HotSwap
 *  %PCI_CAP_ID_PCIX         PCI-X
 *  %PCI_CAP_ID_EXP          PCI Express
 */
int pci_find_capability(struct pci_dev *dev, int cap)
{
    UInt8 _offset;
    UInt32 value = dev->dev.dev->fPciDevice->findPCICapability(cap, &_offset);
    return _offset;
}
EXPORT_SYMBOL(pci_find_capability);




static void pci_msi_setup_pci_dev(struct pci_dev *dev)
{
    /*
     * Disable the MSI hardware to avoid screaming interrupts
     * during boot.  This is the power on reset default so
     * usually this should be a noop.
     */
    dev->msi_cap = pci_find_capability(dev, PCI_CAP_ID_MSI);
    if (dev->msi_cap)
        pci_msi_set_enable(dev, 0);

    dev->msix_cap = pci_find_capability(dev, PCI_CAP_ID_MSIX);
    if (dev->msix_cap)
        pci_msix_clear_and_set_ctrl(dev, PCI_MSIX_FLAGS_ENABLE, 0);
    
}


static void __pci_pme_active(struct pci_dev *dev, bool enable)
{
    u16 pmcsr;

    if (!dev->pme_support)
        return;

    pci_read_config_word(dev, dev->pm_cap + PCI_PM_CTRL, &pmcsr);
    /* Clear PME_Status by writing 1 to it and enable PME# */
    pmcsr |= PCI_PM_CTRL_PME_STATUS | PCI_PM_CTRL_PME_ENABLE;
    if (!enable)
        pmcsr &= ~PCI_PM_CTRL_PME_ENABLE;

    pci_write_config_word(dev, dev->pm_cap + PCI_PM_CTRL, pmcsr);
}

/**
 * pci_pme_restore - Restore PME configuration after config space restore.
 * @dev: PCI device to update.
 */
void pci_pme_restore(struct pci_dev *dev)
{
    u16 pmcsr;

    if (!dev->pme_support)
        return;

    pci_read_config_word(dev, dev->pm_cap + PCI_PM_CTRL, &pmcsr);
    if (dev->wakeup_prepared) {
        pmcsr |= PCI_PM_CTRL_PME_ENABLE;
        pmcsr &= ~PCI_PM_CTRL_PME_STATUS;
    } else {
        pmcsr &= ~PCI_PM_CTRL_PME_ENABLE;
        pmcsr |= PCI_PM_CTRL_PME_STATUS;
    }
    pci_write_config_word(dev, dev->pm_cap + PCI_PM_CTRL, pmcsr);
}

struct pci_pme_device {
    struct list_head list;
    struct pci_dev *dev;
};

/**
 * pci_pme_active - enable or disable PCI device's PME# function
 * @dev: PCI device to handle.
 * @enable: 'true' to enable PME# generation; 'false' to disable it.
 *
 * The caller must verify that the device is capable of generating PME# before
 * calling this function with @enable equal to 'true'.
 */
void pci_pme_active(struct pci_dev *dev, bool enable)
{
    __pci_pme_active(dev, enable);

    /*
     * PCI (as opposed to PCIe) PME requires that the device have
     * its PME# line hooked up correctly. Not all hardware vendors
     * do this, so the PME never gets delivered and the device
     * remains asleep. The easiest way around this is to
     * periodically walk the list of suspended devices and check
     * whether any have their PME flag set. The assumption is that
     * we'll wake up often enough anyway that this won't be a huge
     * hit, and the power savings from the devices will still be a
     * win.
     *
     * Although PCIe uses in-band PME message instead of PME# line
     * to report PME, PME does not work for some PCIe devices in
     * reality.  For example, there are devices that set their PME
     * status bits, but don't really bother to send a PME message;
     * there are PCI Express Root Ports that don't bother to
     * trigger interrupts when they receive PME messages from the
     * devices below.  So PME poll is used for PCIe devices too.
     */

    if (dev->pme_poll) {
        struct pci_pme_device *pme_dev;
        if (enable) {
            pme_dev = (struct pci_pme_device *)kmalloc(sizeof(struct pci_pme_device),
                      GFP_KERNEL);
            if (!pme_dev) {
                pci_warn(dev, "can't enable PME#\n");
                return;
            }
            pme_dev->dev = dev;
//            mutex_lock(&pci_pme_list_mutex);
//            list_add(&pme_dev->list, &pci_pme_list);
//            if (list_is_singular(&pci_pme_list))
//                queue_delayed_work(system_freezable_wq,
//                           &pci_pme_work,
//                           msecs_to_jiffies(PME_TIMEOUT));
//            mutex_unlock(&pci_pme_list_mutex);
        } else {
//            mutex_lock(&pci_pme_list_mutex);
//            list_for_each_entry(pme_dev, &pci_pme_list, list) {
//                if (pme_dev->dev == dev) {
//                    list_del(&pme_dev->list);
//                    kfree(pme_dev);
//                    break;
//                }
//            }
//            mutex_unlock(&pci_pme_list_mutex);
        }
    }

    pci_dbg(dev, "PME# %s\n", enable ? "enabled" : "disabled");
}
EXPORT_SYMBOL(pci_pme_active);


/**
 * pci_pm_init - Initialize PM functions of given PCI device
 * @dev: PCI device to handle.
 */
void pci_pm_init(struct pci_dev *dev)
{
    int pm;
    u16 status;
    u16 pmc;

//    pm_runtime_forbid(&dev->dev);
//    pm_runtime_set_active(&dev->dev);
//    pm_runtime_enable(&dev->dev);
//    device_enable_async_suspend(&dev->dev);
//    dev->wakeup_prepared = false;

    dev->pm_cap = 0;
    dev->pme_support = 0;

    /* find PCI PM capability in list */
    pm = pci_find_capability(dev, PCI_CAP_ID_PM);
    if (!pm)
        return;
    /* Check device's ability to generate PME# */
    pci_read_config_word(dev, pm + PCI_PM_PMC, &pmc);

    if ((pmc & PCI_PM_CAP_VER_MASK) > 3) {
        pci_err(dev, "unsupported PM cap regs version (%u)\n",
            pmc & PCI_PM_CAP_VER_MASK);
        return;
    }

    dev->pm_cap = pm;
    dev->d3_delay = PCI_PM_D3_WAIT;
    dev->d3cold_delay = PCI_PM_D3COLD_WAIT;
//    dev->bridge_d3 = pci_bridge_d3_possible(dev);
    dev->d3cold_allowed = true;

    dev->d1_support = false;
    dev->d2_support = false;
//    if (!pci_no_d1d2(dev)) {
//        if (pmc & PCI_PM_CAP_D1)
//            dev->d1_support = true;
//        if (pmc & PCI_PM_CAP_D2)
//            dev->d2_support = true;
//
//        if (dev->d1_support || dev->d2_support)
//            pci_info(dev, "supports%s%s\n",
//                   dev->d1_support ? " D1" : "",
//                   dev->d2_support ? " D2" : "");
//    }

    pmc &= PCI_PM_CAP_PME_MASK;
    if (pmc) {
        pci_info(dev, "PME# supported from%s%s%s%s%s\n",
             (pmc & PCI_PM_CAP_PME_D0) ? " D0" : "",
             (pmc & PCI_PM_CAP_PME_D1) ? " D1" : "",
             (pmc & PCI_PM_CAP_PME_D2) ? " D2" : "",
             (pmc & PCI_PM_CAP_PME_D3) ? " D3hot" : "",
             (pmc & PCI_PM_CAP_PME_D3cold) ? " D3cold" : "");
        dev->pme_support = pmc >> PCI_PM_CAP_PME_SHIFT;
        dev->pme_poll = true;
        /*
         * Make device's PM flags reflect the wake-up capability, but
         * let the user space enable it to wake up the system as needed.
         */
//        device_set_wakeup_capable(&dev->dev, true);
        /* Disable the PME# generation functionality */
        pci_pme_active(dev, false);
    }

    pci_read_config_word(dev, PCI_STATUS, &status);
    if (status & PCI_STATUS_IMM_READY)
        dev->imm_ready = 1;
}

void set_pcie_port_type(struct pci_dev *pdev)
{
    int pos;
    u16 reg16;
//    int type;
//    struct pci_dev *parent;
    
    pos = pci_find_capability(pdev, PCI_CAP_ID_EXP);
    if (!pos)
        return;
    
    pdev->pcie_cap = pos;
    pci_read_config_word(pdev, pos + PCI_EXP_FLAGS, &reg16);
    pdev->pcie_flags_reg = reg16;
    pci_read_config_word(pdev, pos + PCI_EXP_DEVCAP, &reg16);
    pdev->pcie_mpss = reg16 & PCI_EXP_DEVCAP_PAYLOAD;
    
//    parent = pci_upstream_bridge(pdev);
//    if (!parent)
//        return;
//
//    /*
//     * Some systems do not identify their upstream/downstream ports
//     * correctly so detect impossible configurations here and correct
//     * the port type accordingly.
//     */
//    type = pci_pcie_type(pdev);
//    if (type == PCI_EXP_TYPE_DOWNSTREAM) {
//        /*
//         * If pdev claims to be downstream port but the parent
//         * device is also downstream port assume pdev is actually
//         * upstream port.
//         */
//        if (pcie_downstream_port(parent)) {
//            pci_info(pdev, "claims to be downstream port but is acting as upstream port, correcting type\n");
//            pdev->pcie_flags_reg &= ~PCI_EXP_FLAGS_TYPE;
//            pdev->pcie_flags_reg |= PCI_EXP_TYPE_UPSTREAM;
//        }
//    } else if (type == PCI_EXP_TYPE_UPSTREAM) {
//        /*
//         * If pdev claims to be upstream port but the parent
//         * device is also upstream port assume pdev is actually
//         * downstream port.
//         */
//        if (pci_pcie_type(parent) == PCI_EXP_TYPE_UPSTREAM) {
//            pci_info(pdev, "claims to be upstream port but is acting as downstream port, correcting type\n");
//            pdev->pcie_flags_reg &= ~PCI_EXP_FLAGS_TYPE;
//            pdev->pcie_flags_reg |= PCI_EXP_TYPE_DOWNSTREAM;
//        }
//    }
}

void pci_init_capabilities(struct pci_dev *dev)
{
//    pci_ea_init(dev);        /* Enhanced Allocation */

    /* Setup MSI caps & disable MSI/MSI-X interrupts */
    pci_msi_setup_pci_dev(dev);
    dev->msix_enabled = 0;
    dev->msi_enabled = 0;

    /* Buffers for saving PCIe and PCI-X capabilities */
//    pci_allocate_cap_save_buffers(dev);

    pci_pm_init(dev);        /* Power Management */
//    pci_vpd_init(dev);        /* Vital Product Data */
//    pci_configure_ari(dev);        /* Alternative Routing-ID Forwarding */
//    pci_iov_init(dev);        /* Single Root I/O Virtualization */
//    pci_ats_init(dev);        /* Address Translation Services */
//    pci_pri_init(dev);        /* Page Request Interface */
//    pci_pasid_init(dev);        /* Process Address Space ID */
//    pci_enable_acs(dev);        /* Enable ACS P2P upstream forwarding */
//    pci_ptm_init(dev);        /* Precision Time Measurement */
//    pci_aer_init(dev);        /* Advanced Error Reporting */
//
//    pcie_report_downtraining(dev);
//
//    if (pci_probe_reset_function(dev) == 0)
//        dev->reset_fn = 1;
    
    set_pcie_port_type(dev);

}
