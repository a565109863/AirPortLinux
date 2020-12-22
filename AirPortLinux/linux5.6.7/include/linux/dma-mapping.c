//
//  dma-mapping.c
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/21.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "dma-mapping.h"
#include "device.h"


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

int dma_alloc_from_dev_coherent(struct device *dev, ssize_t size,
        dma_addr_t *dma_handle, void **ret)
{
    UInt64 offset = 0;
    UInt32 numSegs = 1;
    IOBufferMemoryDescriptor *bufDes;
    IODMACommand *dmaCmd;
    IOMbufNaturalMemoryCursor*    mbufCursor;
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
//
//    /* let the implementation decide on the zone to allocate from: */
//    flag &= ~(__GFP_DMA | __GFP_DMA32 | __GFP_HIGHMEM);
//
//    if (dma_is_direct(ops))
//        cpu_addr = dma_direct_alloc(dev, size, dma_handle, flag, attrs);
//    else if (ops->alloc)
//        cpu_addr = ops->alloc(dev, size, dma_handle, flag, attrs);
//    else
//        return NULL;

//    debug_dma_alloc_coherent(dev, size, *dma_handle, cpu_addr);
    return cpu_addr;
}
EXPORT_SYMBOL(dma_alloc_attrs);



struct page *alloc_pages(gfp_t gtp, size_t size)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
    struct page *page = (struct page *)kmalloc(sizeof(struct page), GFP_KERNEL);
    page->dm_mapsize = size;
    page->dm_segs = (IOPhysicalSegment *)kcalloc(PAGE_SIZE, sizeof(IOPhysicalSegment), GFP_KERNEL);
    page->bufDes = IOBufferMemoryDescriptor::inTaskWithPhysicalMask(kernel_task, (kIODirectionInOut | kIOMemoryPhysicallyContiguous | kIOMapInhibitCache), page->dm_mapsize, 0xFFFFFFFFFFFFF000ull);
    if (page->bufDes == NULL) {
        return NULL;
    }
    page->vaddr = (caddr_t)page->bufDes->getBytesNoCopy();
    bzero(page->vaddr, page->bufDes->getLength());
    
    return page;
}

dma_addr_t dma_map_page_attrs(struct device *dev,
            struct page *page, size_t offset, size_t size,
            enum dma_data_direction dir, unsigned long attrs)
{
    
    //  UInt64 offset = 0;
    UInt32 numSegs = 1;
    
    if (page->bufDes == NULL) {
        return NULL;
    }
    
    if (page->bufDes->prepare(dir) != kIOReturnSuccess) {
        printf("prepare()\n");
        return NULL;
    }
    
    page->dmaCmd = IODMACommand::withSpecification(kIODMACommandOutputHost64, 64, 0, IODMACommand::kMapped, 0, 1);
    
    if (page->dmaCmd == NULL) {
        printf("withSpecification()\n");
        return NULL;
    }
    
    if (page->dmaCmd->prepare(offset, size) != kIOReturnSuccess) {
        printf("prepare()\n");
        return NULL;
    }
    
    if (page->dmaCmd->setMemoryDescriptor(page->bufDes) != kIOReturnSuccess) {
        printf("setMemoryDescriptor() failed.\n");
        return NULL;
    }
    
    if (page->dmaCmd->genIOVMSegments((UInt64 *)&offset, &page->dm_segs[0], &numSegs) != kIOReturnSuccess) {
        printf("genIOVMSegments() failed.\n");
        return NULL;
    }
    
    page->paddr = page->dm_segs[0].location;
    
    return page->paddr;
}
