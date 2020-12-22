#ifndef __BACKPORT_LINUX_DMA_MAPPING_H
#define __BACKPORT_LINUX_DMA_MAPPING_H
//#include_next <linux/dma-mapping.h>
//#include <linux/version.h>
#include <linux/gfp.h>
#include <linux/mm.h>

#define LINUX_VERSION_IS_LESS(x,y,z) 1

#define DMA_BIT_MASK(n)    (((n) == 64) ? ~0ULL : ((1ULL<<(n))-1))

/*
 * DMA_ATTR_NO_WARN: This tells the DMA-mapping subsystem to suppress
 * allocation failure reports (similarly to __GFP_NOWARN).
 */
#define DMA_ATTR_NO_WARN    (1UL << 8)


void *dma_alloc_attrs(struct device *dev, size_t size, dma_addr_t *dma_handle,
        gfp_t flag, unsigned long attrs);

static inline void *dma_alloc_coherent(struct device *dev, size_t size,
        dma_addr_t *dma_handle, gfp_t gfp)
{
    return dma_alloc_attrs(dev, size, dma_handle, gfp,
            (gfp & __GFP_NOWARN) ? DMA_ATTR_NO_WARN : 0);
}

//#if LINUX_VERSION_IS_LESS(3,2,0)
//#define dma_zalloc_coherent LINUX_BACKPORT(dma_zalloc_coherent)
static inline void *dma_zalloc_coherent(struct device *dev, size_t size,
                                        dma_addr_t *dma_handle, gfp_t flag)
{
    void *ret = dma_alloc_coherent(dev, size, dma_handle, flag);
    if (ret)
        memset(ret, 0, size);
    return ret;
}
//#endif


int dma_set_mask(struct device *dev, u64 mask);

//#if LINUX_VERSION_IS_LESS(3,13,0)
/*
 * Set both the DMA mask and the coherent DMA mask to the same thing.
 * Note that we don't check the return value from dma_set_coherent_mask()
 * as the DMA API guarantees that the coherent DMA mask can be set to
 * the same or smaller than the streaming DMA mask.
 */
//#define dma_set_mask_and_coherent LINUX_BACKPORT(dma_set_mask_and_coherent)
static inline int dma_set_mask_and_coherent(struct device *dev, u64 mask)
{
//    int rc = 0;
    int rc = dma_set_mask(dev, mask);
//    if (rc == 0)
//        dma_set_coherent_mask(dev, mask);
    return rc;
}
//#endif /* LINUX_VERSION_IS_LESS(3,13,0) */





enum dma_data_direction {
    DMA_BIDIRECTIONAL = kIODirectionOutIn,
    DMA_TO_DEVICE = kIODirectionOut,
    DMA_FROM_DEVICE = kIODirectionIn,
    DMA_NONE = kIODirectionNone,
};



static inline void dma_sync_single_for_cpu(struct device *dev, dma_addr_t addr,
                                           size_t size,
                                           enum dma_data_direction dir)
{
//    const struct dma_map_ops *ops = get_dma_ops(dev);
    
//    BUG_ON(!valid_dma_direction(dir));
//    if (dma_is_direct(ops))
//        dma_direct_sync_single_for_cpu(dev, addr, size, dir);
//    else if (ops->sync_single_for_cpu)
//        ops->sync_single_for_cpu(dev, addr, size, dir);
//    debug_dma_sync_single_for_cpu(dev, addr, size, dir);
    
    IOBufferMemoryDescriptor::withPhysicalAddress(addr, size, dir);
}

static inline void dma_sync_single_for_device(struct device *dev,
                                              dma_addr_t addr, size_t size,
                                              enum dma_data_direction dir)
{
//    const struct dma_map_ops *ops = get_dma_ops(dev);
//
//    BUG_ON(!valid_dma_direction(dir));
//    if (dma_is_direct(ops))
//        dma_direct_sync_single_for_device(dev, addr, size, dir);
//    else if (ops->sync_single_for_device)
//        ops->sync_single_for_device(dev, addr, size, dir);
//    debug_dma_sync_single_for_device(dev, addr, size, dir);
    
    IOBufferMemoryDescriptor::withPhysicalAddress(addr, size, dir);
}


static void dma_free_attrs(struct device *dev, size_t size, void *cpu_addr,
                           dma_addr_t dma_handle, unsigned long attrs)
{
}

static inline void dma_free_coherent(struct device *dev, size_t size,
                                     void *cpu_addr, dma_addr_t dma_handle)
{
    return dma_free_attrs(dev, size, cpu_addr, dma_handle, 0);
}

static inline void dma_unmap_page_attrs(struct device *dev, dma_addr_t addr,
                                        size_t size, enum dma_data_direction dir, unsigned long attrs)
{
//    const struct dma_map_ops *ops = get_dma_ops(dev);
//
//    BUG_ON(!valid_dma_direction(dir));
//    if (dma_is_direct(ops))
//        dma_direct_unmap_page(dev, addr, size, dir, attrs);
//    else if (ops->unmap_page)
//        ops->unmap_page(dev, addr, size, dir, attrs);
//    debug_dma_unmap_page(dev, addr, size, dir);
}

struct page *alloc_pages(gfp_t gtp, size_t size);
#define alloc_page(gfp_mask) alloc_pages(gfp_mask, 1)


dma_addr_t dma_map_page_attrs(struct device *dev,
        struct page *page, size_t offset, size_t size,
         enum dma_data_direction dir, unsigned long attrs);

#define dma_unmap_page(d, a, s, r) dma_unmap_page_attrs(d, a, s, r, 0)
#define dma_map_page(d, p, o, s, r) dma_map_page_attrs(d, p, o, s, r, 0)

static inline int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
//    debug_dma_mapping_error(dev, dma_addr);
//
//    if (dma_addr == DMA_MAPPING_ERROR)
//        return -ENOMEM;
    return 0;
}

static struct page *__virt_to_page(void *ptr, size_t ptrlen)
{
    struct page *page = (struct page *)kmalloc(sizeof(struct page), GFP_KERNEL);
    page->dm_mapsize = ptrlen;
    page->bufDes = (IOBufferMemoryDescriptor *)IOBufferMemoryDescriptor::withAddress(ptr, page->dm_mapsize, kIODirectionInOut);
    if (page->bufDes == NULL) {
        return NULL;
    }
    
    return page;
}

#define virt_to_page(x) __virt_to_page(x, size)
#define offset_in_page(x) 0

static inline dma_addr_t dma_map_single_attrs(struct device *dev, void *ptr,
                                              size_t size, enum dma_data_direction dir, unsigned long attrs)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
//    debug_dma_map_single(dev, ptr, size);
    return dma_map_page_attrs(dev, virt_to_page(ptr), offset_in_page(ptr),
                              size, dir, attrs);
    
    return 0;
}

static inline void dma_unmap_single_attrs(struct device *dev, dma_addr_t addr,
                                          size_t size, enum dma_data_direction dir, unsigned long attrs)
{
    return dma_unmap_page_attrs(dev, addr, size, dir, attrs);
}

#define dma_map_single(d, a, s, r) dma_map_single_attrs(d, a, s, r, 0)
#define dma_unmap_single(d, a, s, r) dma_unmap_single_attrs(d, a, s, r, 0)


#endif /* __BACKPORT_LINUX_DMA_MAPPING_H */
