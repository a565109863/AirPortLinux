//
//  _malloc.h
//  AirPortLinux
//
//  Created by User-Mac on 2021/2/12.
//  Copyright © 2021 User-Mac. All rights reserved.
//

#ifndef _malloc_h
#define _malloc_h

#include <linux/types.h>
#include <linux/gfp.h>

#include <sys/queue.h>

struct malloc_ptr {
    LIST_ENTRY(malloc_ptr)    list;
    size_t              size;
    void                *ptr;
};

void* malloc(vm_size_t len);
void free(void* addr);
void free(void* addr, int type, vm_size_t len);

static inline void *zalloc(size_t size)
{
    return malloc(size);
}

static inline void *kmalloc(size_t size, gfp_t gfp)
{
    return malloc(size);
}

static inline void *kmalloc_array(size_t n, size_t size, gfp_t flags)
{
    return kmalloc(n * size, flags);
}

static inline void *kzalloc(size_t size, gfp_t gfp)
{
    void *p = kmalloc(size, gfp);
    return p;
}

static inline void *kcalloc(size_t n, size_t size, gfp_t flags)
{
    return kmalloc_array(n, size, flags | __GFP_ZERO);
}
#define kvcalloc kcalloc

#define kvfree kfree


static inline void *realloc(void* node, size_t s)
{
    void *p = malloc(s);
    if (p && node) {
        memcpy(p, node, s);
    }
    return p;
}

static inline void *__krealloc(void* node, size_t node_size, size_t s, gfp_t gfp)
{
    if (node_size >= s) {
        return node;
    }
    void *p = kmalloc(s, gfp);
    if (p && node) {
        memcpy(p, node, node_size);
    }
    return p;
}


static inline void *realloc_array(void *ptr, size_t nmemb, size_t size)
{
    if (size && nmemb > (~(size_t) 0) / size)
        return NULL;
    return realloc(ptr, nmemb * size);
}

#define krealloc(n, s, g) __krealloc(n, s - sizeof(*n) , s, g)

static void *kmemdup(const void *src, size_t len, gfp_t gfp)
{
    void *p;
    p = kmalloc(len, gfp);
    if (p)
        memcpy(p, src, len);
    return p;
}


static inline void *kvmalloc(size_t size, gfp_t flags)
{
    return kmalloc(size, flags | __GFP_ZERO);
}

static inline void *kvzalloc(size_t size, gfp_t flags)
{
    return kvmalloc(size, flags | __GFP_ZERO);
}

static void *vzalloc(unsigned long size)
{
    void *ret = malloc(size);
    return ret;
}

static void *__kmalloc(size_t size, gfp_t flags)
{
    return kmalloc(size, flags);
}

static __always_inline void *__kmalloc_node(size_t size, gfp_t flags, int node)
{
    return __kmalloc(size, flags);
}

static __always_inline void *kmalloc_node(size_t size, gfp_t flags, int node)
{
    //#ifndef CONFIG_SLOB
    //    if (__builtin_constant_p(size) &&
    //        size <= KMALLOC_MAX_CACHE_SIZE) {
    //        unsigned int i = kmalloc_index(size);
    //
    //        if (!i)
    //            return ZERO_SIZE_PTR;
    //
    //        return kmem_cache_alloc_node_trace(
    //                kmalloc_caches[kmalloc_type(flags)][i],
    //                        flags, node, size);
    //    }
    //#endif
    return __kmalloc_node(size, flags, node);
}

/**
 * kzalloc_node - allocate zeroed memory from a particular memory node.
 * @size: how many bytes of memory are required.
 * @flags: the type of memory to allocate (see kmalloc).
 * @node: memory node from which to allocate
 */
static inline void *kzalloc_node(size_t size, gfp_t flags, int node)
{
    return kmalloc_node(size, flags | __GFP_ZERO, node);
}





static bool is_vmalloc_addr(const void *x)
{
    return true;
}
EXPORT_SYMBOL(is_vmalloc_addr);

static inline void *alloc_pages_exact(size_t s, gfp_t gfp)
{
    return kmalloc(s, gfp);
}

static void __percpu *__alloc_percpu_gfp(size_t size, size_t align, gfp_t gfp)
{
    return vzalloc(size);
}

static void __percpu *__alloc_percpu(size_t size, size_t align)
{
    return vzalloc(size);
}
EXPORT_SYMBOL_GPL(__alloc_percpu);

#define alloc_percpu_gfp(type, gfp)                    \
(typeof(type) __percpu *)__alloc_percpu_gfp(sizeof(type),    \
__alignof__(type), gfp)

static inline void kfree(const void *p)
{
    //    if (p >= __kfree_ignore_start && p < __kfree_ignore_end)
    //        return;
        free((void *)p);
        p = NULL;
}

static void kfree_sensitive(const void *p)
{
    //   size_t ks;
    void *mem = (void *)p;
    
    //   ks = ksize(mem);
    //   if (ks)
    //       memzero_explicit(mem, ks);
    kfree(mem);
}
EXPORT_SYMBOL(kfree_sensitive);

static inline void kfree(void *p, size_t n, size_t size)
{
    //    if (p >= __kfree_ignore_start && p < __kfree_ignore_end)
    //        return;
    free(p, 1, n * size);
    p = NULL;
    //    free(void* addr, int type, vm_size_t len)
}

static void kzfree(const void *p)
{
    free((void *)p);
    p = NULL;
}

static void vfree(const void *p)
{
    free((void *)p);
    p = NULL;
}

static inline void free_pages_exact(void *p, size_t s)
{
    kfree(p, 1, s);
}


static inline void free_page(unsigned long addr)
{
    //    free((void *)addr);
}


static void *kmalloc_track_caller(size_t size, gfp_t gfp)
{
    return kmalloc(size, gfp);
}


#endif /* _malloc_h */
