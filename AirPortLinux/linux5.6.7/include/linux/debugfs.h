//
//  debugfs.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef debugfs_h
#define debugfs_h

#define printk kprintf

#define dev_dbg(dev, fmt, ...)    kprintf(fmt, ##__VA_ARGS__)

#define BUG() do { \
printk("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __FUNCTION__); \
printk("BUG!"); \
} while (0)
#define BUG_ON(condition) do { if (unlikely((condition)!=0)) BUG(); } while(0)

struct debugfs_blob_wrapper {
    void *data;
    unsigned long size;
};

struct debugfs_reg32 {
    char *name;
    unsigned long offset;
};

struct debugfs_regset32 {
    const struct debugfs_reg32 *regs;
    int nregs;
    void __iomem *base;
};

#endif /* debugfs_h */
