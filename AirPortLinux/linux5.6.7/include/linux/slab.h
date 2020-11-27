//
//  slab.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef slab_h
#define slab_h

#include <linux/gfp.h>
#include <linux/log2.h>

#define SLAB_HWCACHE_ALIGN 0

#define __assume_slab_alignment
#define __malloc

static void *kmem_cache_alloc(struct kmem_cache *, gfp_t flags) __assume_slab_alignment __malloc
{
    return NULL;
}
static void kmem_cache_free(struct kmem_cache *, void *)
{
    
}

/*
 * Shortcuts
 */
static inline void *kmem_cache_zalloc(struct kmem_cache *k, gfp_t flags)
{
    return kmem_cache_alloc(k, flags | __GFP_ZERO);
}


static struct kmem_cache *kmem_cache_create(const char *name, unsigned int size,
            unsigned int align, slab_flags_t flags,
            void (*ctor)(void *))
{
    return NULL;
}
static struct kmem_cache *kmem_cache_create_usercopy(const char *name,
            unsigned int size, unsigned int align,
            slab_flags_t flags,
            unsigned int useroffset, unsigned int usersize,
            void (*ctor)(void *))
{
    return NULL;
}
static void kmem_cache_destroy(struct kmem_cache *)
{
    
}
static int kmem_cache_shrink(struct kmem_cache *)
{
    return 0;
}

#endif /* slab_h */
