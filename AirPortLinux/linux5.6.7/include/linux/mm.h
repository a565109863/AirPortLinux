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


#define free_pages(a,b)

static
void *page_address(mbuf_t mac_data)
{
    return mbuf_data(mac_data);
}

static
void get_page(struct page * page)
{
    
}


static inline void *page_address(const struct page *page)
{
    return NULL;
}
static inline void set_page_address(struct page *page, void *address)
{
//    page->virtual = address;
}



static inline int get_order(unsigned long size)
{
    return (int)size;//hack
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
