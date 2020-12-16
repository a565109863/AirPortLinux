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
