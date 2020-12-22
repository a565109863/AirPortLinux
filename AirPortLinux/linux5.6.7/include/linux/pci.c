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


static int __pci_enable_msix_range(struct pci_dev *dev,
                                   struct msix_entry *entries, int minvec,
                                   int maxvec, struct irq_affinity *affd,
                                   int flags)
{
    return -1;
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

int pci_enable_msi(struct pci_dev *dev){
    
    dev->dev.ih = new pci_intr_handle();
    
    if (dev->dev.ih == 0)
        return 1;
    
    IOReturn ret;
    int index, source = -1;
    for (index = 0; ; index++)
    {
       int interruptType;
       ret = dev->dev.dev->fPciDevice->getInterruptType(index, &interruptType);
       if (ret != kIOReturnSuccess)
           break;
       if (interruptType & kIOInterruptTypePCIMessaged)
       {
           source = index;
           break;
       }
    }

    dev->irq = source;
    return 0;
}

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
