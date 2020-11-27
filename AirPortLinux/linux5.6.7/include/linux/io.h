//
//  io.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/5/8.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef io_h
#define io_h

#include <linux/dma-mapping.h>
#include <asm/io.h>

static
u8 readb(const volatile void __iomem *addr)
{
    return *(volatile uint8_t *)addr;
}

static
u16 readw(const volatile void __iomem *addr)
{
    return *(volatile uint16_t *)addr;
}

static
u32 readl(const volatile void __iomem *addr)
{
    return *(volatile uint32_t *)addr;
}

static
u64 readq(const volatile void __iomem *addr)
{
    return *(volatile uint64_t *)addr;
}

static
void writeb(u8 b, volatile void __iomem *addr)
{
    *(volatile uint8_t *)addr = b;
}

static
void writew(u16 b, volatile void __iomem *addr)
{
    *(volatile uint16_t *)addr = b;
}

static
void writel(u32 b, volatile void __iomem *addr)
{
    *(volatile uint32_t *)addr = b;
}

static
void writeq(u64 b, volatile void __iomem *addr)
{
    *(volatile uint64_t *)addr = b;
}


#endif /* io_h */
