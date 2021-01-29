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
    
    /* Usage count. *DO NOT USE DIRECTLY*. See page_ref.h */
    atomic_t _refcount;
    
    void        *ptr;
//    uint32_t    size;
    size_t         order;
    uint32_t                    dm_mapsize;    /* size of the mapping */
    int                         dm_nsegs;    /* # valid segments in mapping */
    IOPhysicalSegment           *dm_segs;    /* segments; variable length */
    
//    uint32_t alignment;
    IOBufferMemoryDescriptor    *bufDes;
    IODMACommand                *dmaCmd;
//    dma_addr_t                  paddr;
    caddr_t                     vaddr;
};

#define free_pages(a,b)

static
void get_page(struct page * page)
{
    atomic_inc(&page->_refcount);
}


void __free_page(struct page * page);

//static void __free_pages(struct page * page, u32 _rx_page_order)
//{
////    page++;
//    __free_page(page);
//}


static inline int page_ref_dec_and_test(struct page *page)
{
    int ret = atomic_dec_and_test(&page->_refcount);

//    if (page_ref_tracepoint_active(__tracepoint_page_ref_mod_and_test))
//        __page_ref_mod_and_test(page, -1, ret);
    return ret;
}

/*
 * Drop a ref, return true if the refcount fell to zero (the page has no users)
 */
static inline int put_page_testzero(struct page *page)
{
//    VM_BUG_ON_PAGE(page_ref_count(page) == 0, page);
    return page_ref_dec_and_test(page);
}


/*
 * Free a 0-order page
 */
static void free_unref_page(struct page *page)
{
    __free_page(page);
}

static void __free_pages_ok(struct page *page, unsigned int order)
{
    __free_page(page);
//    unsigned long flags;
//    int migratetype;
//    unsigned long pfn = page_to_pfn(page);
//
//    if (!free_pages_prepare(page, order, true))
//        return;
//
//    migratetype = get_pfnblock_migratetype(page, pfn);
//    local_irq_save(flags);
//    __count_vm_events(PGFREE, 1 << order);
//    free_one_page(page_zone(page), page, pfn, order, migratetype);
//    local_irq_restore(flags);
}

static inline void free_the_page(struct page *page, unsigned int order)
{
    kprintf("--%s: line = %d order = %d", __FUNCTION__, __LINE__, order);
    if (order == 0)        /* Via pcp? */
        free_unref_page(page);
    else
        __free_pages_ok(page, order);
}

static void __free_pages(struct page *page, unsigned int order)
{
    kprintf("--%s: line = %d order = %d", __FUNCTION__, __LINE__, order);
    if (put_page_testzero(page))
        free_the_page(page, order);
}
EXPORT_SYMBOL(__free_pages);


void *page_address(struct page *page);

//static inline void set_page_address(struct page *page, void *address)
//{
////    page->virtual = address;
//}


static inline int get_order(unsigned long size)
{
    int order;
    
    size = (size - 1) >> (PAGE_SHIFT - 1);
    order = -1;
    do {
        size >>= 1;
        order++;
    } while (size);
    return order;
}


static inline void *kmap_atomic(struct page *page)
{
//    preempt_disable();
//    pagefault_disable();
    return page_address(page);
}

static inline void kunmap_atomic(void *addr)
{
//    pagefault_enable();
//    preempt_enable();
}


//#define kmalloc(size, flags) malloc(size, flags)
//#define kfree(ptr) free(ptr)
//#define kfree(ptr)
#define vmalloc(size) malloc(size)
//#define vfree(ptr) do { if (ptr != NULL) free(ptr); } while (0)

//#define kzfree(x)

#endif /* mm_h */
