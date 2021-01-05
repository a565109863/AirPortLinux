//
//  dma-mapping.c
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/21.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "dma-mapping.h"
#include "device.h"

LIST_HEAD(page_list);

int dma_set_mask(struct device *dev, u64 mask)
{
    /*
     * Truncate the mask to the actually supported dma_addr_t width to
     * avoid generating unsupportable addresses.
     */
//    mask = (dma_addr_t)mask;

//    if (!dev->dma_mask || !dma_supported(dev, mask))
//        return -EIO;
//
//    arch_dma_set_mask(dev, mask);
    dev->dma_mask = &mask;
    return 0;
}

void *page_address(struct page *page)
{
    if (page->bufDes == NULL) {
        page->bufDes = IOBufferMemoryDescriptor::inTaskWithPhysicalMask(kernel_task, (kIODirectionInOut | kIOMemoryPhysicallyContiguous | kIOMapInhibitCache), page->dm_mapsize, DMA_BIT_MASK(64));
        if (page->bufDes == NULL) {
            return NULL;
        }
        page->vaddr = (caddr_t)page->bufDes->getBytesNoCopy();
        bzero(page->vaddr, page->bufDes->getLength());
    }
    
    return page->vaddr;
}

int dma_alloc_from_dev_coherent(struct device *dev, ssize_t size,
        dma_addr_t *dma_handle, void **ret)
{
    UInt64 offset = 0;
    UInt32 numSegs = 1;
    IOBufferMemoryDescriptor *bufDes;
    IODMACommand *dmaCmd;
    IOPhysicalSegment dm_segs;    /* segments; variable length */
    
    bufDes = IOBufferMemoryDescriptor::inTaskWithPhysicalMask(kernel_task, (kIODirectionInOut | kIOMemoryPhysicallyContiguous | kIOMapInhibitCache), size, *dev->dma_mask);
    if (bufDes == NULL) {
        return 0;
    }
    
    *ret = (dma_addr_t *)bufDes->getBytesNoCopy();
    
    if (bufDes->prepare() != kIOReturnSuccess) {
        printf("prepare()\n");
        return 0;
    }
    
    dmaCmd = IODMACommand::withSpecification(kIODMACommandOutputHost64, 64, 0, IODMACommand::kMapped, 0, 1);
    
    if (dmaCmd == NULL) {
        printf("withSpecification()\n");
        return 0;
    }
    
    if (dmaCmd->setMemoryDescriptor(bufDes) != kIOReturnSuccess) {
        printf("setMemoryDescriptor() failed.\n");
        return 0;
    }

    if (dmaCmd->genIOVMSegments((UInt64 *)&offset, &dm_segs, &numSegs) != kIOReturnSuccess) {
        printf("genIOVMSegments() failed.\n");
        return 0;
    }
    
    *dma_handle = dm_segs.location;
    
    return 1;
}


void *dma_alloc_attrs(struct device *dev, size_t size, dma_addr_t *dma_handle,
        gfp_t flag, unsigned long attrs)
{
//    const struct dma_map_ops *ops = get_dma_ops(dev);
    void *cpu_addr;

//    WARN_ON_ONCE(!dev->coherent_dma_mask);

    if (dma_alloc_from_dev_coherent(dev, size, dma_handle, &cpu_addr))
        return cpu_addr;
    
    return NULL;
}
EXPORT_SYMBOL(dma_alloc_attrs);



struct page *alloc_pages(gfp_t gtp, size_t size)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
    struct page *page = (struct page *)kmalloc(sizeof(struct page), GFP_KERNEL);
    page->dm_mapsize = PAGE_SIZE << size;
    page->dm_segs = (IOPhysicalSegment *)kcalloc(BIT(size), sizeof(IOPhysicalSegment), GFP_KERNEL);
    
    list_add(&page->list, &page_list);
    
    return page;
}

void __free_page(struct page * page)
{
//    page++;
//    struct page *tmp;
//    list_for_each_entry(tmp, &page_list, list) {
//        if (tmp == page) {
//            kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
////            if (tmp->dmaCmd) {
////                tmp->dmaCmd->clearMemoryDescriptor();
////                tmp->dmaCmd->release();
////                tmp->dmaCmd = NULL;
////            }
////            if (tmp->bufDes) {
////                tmp->bufDes->complete();
////                tmp->bufDes->release();
////                tmp->bufDes = NULL;
////            }
////            if (tmp->memDes) {
////                tmp->memDes->complete();
////                tmp->memDes->release();
////                tmp->memDes = NULL;
////            }
//
//            list_del(&tmp->list);
//            break;
//        }
//    }
    
//    free(page, 1, sizeof(struct page));
}

dma_addr_t dma_map_page_attrs(struct device *dev,
            struct page *page, size_t offset, size_t size,
            enum dma_data_direction dir, unsigned long attrs)
{
    UInt32 numSegs = 1;
    
    page->offset = offset;
    
    if (page->ptr != NULL) {
        page->dm_mapsize = size;
        page->size = size;
    }
    
    if (page->bufDes == NULL) {
        page->bufDes = IOBufferMemoryDescriptor::inTaskWithPhysicalMask(kernel_task, (kIODirectionInOut | kIOMemoryPhysicallyContiguous | kIOMapInhibitCache), page->dm_mapsize, *dev->dma_mask);
        if (page->bufDes == NULL) {
            return NULL;
        }
        page->vaddr = (caddr_t)page->bufDes->getBytesNoCopy();
        bzero(page->vaddr, page->bufDes->getLength());
    }
    
    if (page->ptr != NULL) {
        bcopy(page->ptr, page->vaddr, page->size);
        page->ptr = page->vaddr;
    }
    
    if (page->bufDes->prepare(dir) != kIOReturnSuccess) {
        printf("prepare()\n");
        return NULL;
    }
    
    page->dmaCmd = IODMACommand::withSpecification(kIODMACommandOutputHost64, 64, 0, IODMACommand::kMapped, 0, 1);
    
    if (page->dmaCmd == NULL) {
        printf("withSpecification() failed.\n");
        return NULL;
    }
    
    if (page->dmaCmd->prepare(offset, size) != kIOReturnSuccess) {
        printf("prepare() failed.\n");
        return NULL;
    }
    
    if (page->dmaCmd->setMemoryDescriptor(page->bufDes) != kIOReturnSuccess) {
        printf("setMemoryDescriptor() failed.\n");
        return NULL;
    }
    
    kprintf("--%s: 80211 line = %d, offset = %lu, page->dm_mapsize = %d, size = %lu", __FUNCTION__, __LINE__, offset, page->dm_mapsize, size);
    
    if (page->dmaCmd->genIOVMSegments((UInt64 *)&offset, &page->dm_segs[0], &numSegs) != kIOReturnSuccess) {
        printf("genIOVMSegments() failed.\n");
        return NULL;
    }
    
    kprintf("--%s: 80211 line = %d, location = %llu, single_page->offset = %llu, offset = %lu, numSegs = %d, len = %llu", __FUNCTION__, __LINE__, page->dm_segs[0].location, page->offset, offset, numSegs, page->bufDes->getLength());
    
    
    page->paddr = page->dm_segs[0].location;
    
    return page->paddr;
    
}


void dma_unmap_page_attrs(struct device *dev, dma_addr_t addr,
    size_t size, enum dma_data_direction dir, unsigned long attrs)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
    
//    struct page *tmp;
//    list_for_each_entry(tmp, &page_list, list) {
//        int order = get_order(tmp->size);
//        if (tmp->paddr == addr && order == 0 && tmp->offset == tmp->dm_mapsize) {
//            kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
//            if (tmp->dmaCmd) {
//                tmp->dmaCmd->clearMemoryDescriptor();
//                tmp->dmaCmd->release();
//                tmp->dmaCmd = NULL;
//            }
//            if (tmp->bufDes) {
//                tmp->bufDes->complete();
//                tmp->bufDes->release();
//                tmp->bufDes = NULL;
//            }
//
////            list_del(&tmp->list);
//            break;
//        }
//    }
    
    //    const struct dma_map_ops *ops = get_dma_ops(dev);
    //
    //    BUG_ON(!valid_dma_direction(dir));
    //    if (dma_is_direct(ops))
    //        dma_direct_unmap_page(dev, addr, size, dir, attrs);
    //    else if (ops->unmap_page)
    //        ops->unmap_page(dev, addr, size, dir, attrs);
    //    debug_dma_unmap_page(dev, addr, size, dir);
}

dma_addr_t dma_map_single_attrs(struct device *dev, void *ptr,
    size_t size, enum dma_data_direction dir, unsigned long attrs)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
//    debug_dma_map_single(dev, ptr, size);
    return dma_map_page_attrs(dev, virt_to_page(ptr), offset_in_page(ptr),
                              size, dir, 1);
}
