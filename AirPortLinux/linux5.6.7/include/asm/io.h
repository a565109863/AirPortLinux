//
//  io.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef asm_io_h
#define asm_io_h

#include <linux/device.h>
#include <linux/compiler.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <asm/byteorder.h>
//#include <asm/synch.h>
//#include <asm/delay.h>
//#include <asm/mmiowb.h>
//#include <asm/mmu.h>
//#include <asm/ppc_asm.h>
//#include <asm/pgtable.h>

/**
 *    phys_to_virt    -    map physical address to virtual
 *    @address: address to remap
 *
 *    The returned virtual address is a current CPU mapping for
 *    the memory address given. It is only valid to use this function on
 *    addresses that have a kernel mapping
 *
 *    This function does not handle bus mappings for DMA transfers. In
 *    almost all conceivable cases a device driver should not be using
 *    this function
 */
static inline void * phys_to_virt(unsigned long address)
{
//    return (void *)__va(address);
    return NULL;
}

/*
 * Change "struct page" to physical address.
 */
static inline phys_addr_t page_to_phys(struct page *page)
{
//    unsigned long pfn = page_to_pfn(page);
//
//    WARN_ON(IS_ENABLED(CONFIG_DEBUG_VIRTUAL) && !pfn_valid(pfn));
//
//    return PFN_PHYS(pfn);
    return 0;
}

#endif /* asm_io_h */
