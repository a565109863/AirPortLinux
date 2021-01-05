//
//  mm.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef mm_h
#define mm_h

#include <linux/types.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/kernel.h>

struct page {
    struct list_head list;
//    struct rcu_head rcu;
    int32_t    name;
    int32_t    sect;
    int32_t    arch;
    int32_t    desc;
    int32_t    file;
    
    void        *ptr;
    uint32_t    size;
    uint32_t                    dm_mapsize;    /* size of the mapping */
    int                         dm_nsegs;    /* # valid segments in mapping */
    IOPhysicalSegment           *dm_segs;    /* segments; variable length */
    
//    uint32_t alignment;
    IOBufferMemoryDescriptor    *bufDes;
    IODMACommand *dmaCmd;
    UInt64                      offset;
    dma_addr_t                  paddr;
    caddr_t                     vaddr;
};

#define free_pages(a,b)

static
void get_page(struct page * page)
{
//    if (page->mult_page_index >= page->mult_page_num) {
//        page = NULL;
//    } else {
//        page->mult_page_index++;
//    }
}


void __free_page(struct page * page);

static void __free_pages(struct page * page, u32 _rx_page_order)
{
//    page++;
    __free_page(page);
}


void *page_address(struct page *page);

//static inline void set_page_address(struct page *page, void *address)
//{
////    page->virtual = address;
//}


static inline int get_order(unsigned long size)
{
//    return (int)size;//hack
    int order;
    
    size = (size - 1) >> (PAGE_SHIFT - 1);
    order = -1;
    do {
        size >>= 1;
        order++;
    } while (size);
    return order;
}




//#define kmalloc(size, flags) malloc(size, flags)
//#define kfree(ptr) free(ptr)
//#define kfree(ptr)
#define vmalloc(size) malloc(size)
//#define vfree(ptr) do { if (ptr != NULL) free(ptr); } while (0)

//#define kzfree(x)

#endif /* mm_h */
