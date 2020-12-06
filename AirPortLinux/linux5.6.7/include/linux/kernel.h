//
//  kernel.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef kernel_h
#define kernel_h

#include <linux/types.h>
#include <linux/gfp.h>
#include <linux/time.h>
#include <linux/debugfs.h>
#include <linux/bitfield.h>
#include <linux/_string.h>
#include <linux/cpu.h>
#include <linux/rculist.h>

#include <asm/swab.h>


#define KERN_DEBUG 0

#define    NUMA_NO_NODE    (-1)
typedef unsigned int __wsum;


/*
 * Divide positive or negative dividend by positive or negative divisor
 * and round to closest integer. Result is undefined for negative
 * divisors if the dividend variable type is unsigned and for negative
 * dividends if the divisor variable type is unsigned.
 */
#define DIV_ROUND_CLOSEST(x, divisor)(            \
{                            \
    typeof(x) __x = x;                \
    typeof(divisor) __d = divisor;            \
    (((typeof(x))-1) > 0 ||                \
     ((typeof(divisor))-1) > 0 ||            \
     (((__x) > 0) == ((__d) > 0))) ?        \
        (((__x) + ((__d) / 2)) / (__d)) :    \
        (((__x) - ((__d) / 2)) / (__d));    \
}                            \
)


#define    ERFKILL        256    /* Operation not possible due to RF-kill */


static void* malloc(vm_size_t len) {
    void* addr = IOMalloc(len);
    if (addr == NULL) {
        return NULL;
    }
    bzero(addr, len);
    return addr;
}


static void free(void* addr, int type, vm_size_t len)
{
    if (addr == NULL) {
        return;
    }
    if (len == 0) {
        IOFree(addr, sizeof(addr));
    }else {
        IOFree(addr, len);
    }
    addr = NULL;
}

static inline void *kmalloc(size_t s, gfp_t gfp)
{
//    if (__kmalloc_fake)
//        return __kmalloc_fake;
    return IOMalloc(s);
}

static inline void *kmalloc_array(size_t n, size_t size, gfp_t flags)
{
    return kmalloc(n * size, flags);
}

static inline void *kzalloc(size_t s, gfp_t gfp)
{
    void *p = kmalloc(s, gfp);

    memset(p, 0, s);
    return p;
}

static inline void *kcalloc(size_t n, size_t size, gfp_t flags)
{
    return kmalloc_array(n, size, flags | __GFP_ZERO);
}
#define kvcalloc kcalloc

#define kvfree kfree

static inline void *krealloc(void* node, size_t s, gfp_t gfp)
{
    void *p = kmalloc(s, gfp);

    memset(p, 0, s);
    return p;
}

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
    void *ret = IOMalloc(size);
    return ret;
}

static inline void *alloc_pages_exact(size_t s, gfp_t gfp)
{
    return kmalloc(s, gfp);
}

static void __percpu *__alloc_percpu_gfp(size_t size, size_t align, gfp_t gfp)
{
    return vzalloc(size);
}

#define alloc_percpu_gfp(type, gfp)                    \
    (typeof(type) __percpu *)__alloc_percpu_gfp(sizeof(type),    \
                        __alignof__(type), gfp)

static inline void kfree(const void *p)
{
//    if (p >= __kfree_ignore_start && p < __kfree_ignore_end)
//        return;
//    free(p);
}

static void kzfree(const void *p)
{
    
}

static void vfree(const void *p)
{
    
}

static inline void free_pages_exact(void *p, size_t s)
{
    kfree(p);
}


static inline void free_page(unsigned long addr)
{
//    free((void *)addr);
}


static void *kmalloc_track_caller(size_t size, gfp_t gfp)
{
    return kmalloc(size, gfp);
}

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:    the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (const typeof( ((type *)0)->member ) *)(ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})



#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))



#define pr_err kprintf
#define pr_debug kprintf
#define pr_info kprintf

#define netdev_err(d, arg...) kprintf(arg)
#define net_info_ratelimited  kprintf


#define __builtin_expect(x, expected_value) (x)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define likely(x) __builtin_expect(!!(x), 1)

#define __WARN()
#define __WARN_printf(arg...)   do { IOLog(arg); __WARN(); } while (0)

//#define WARN_ON(condition) ({                                           \
//int __ret_warn_on = !!(condition);                              \
//if (unlikely(__ret_warn_on))                                    \
//__WARN();                                               \
//unlikely(__ret_warn_on);                                        \
//})


#define WARN_ON(cond)    \
    ((cond) ? printf("Internal warning(%s:%d, %s): %s\n",    \
            __FILE__, __LINE__, __func__, #cond) : 0)

#define WARN(condition, format...) ({                                           \
int __ret_warn_on = !!(condition);                              \
if (unlikely(__ret_warn_on))                                    \
__WARN_printf(format);                                  \
unlikely(__ret_warn_on);                                        \
})

#define WARN_ON_ONCE(condition) ({                              \
static int __warned;                                    \
int __ret_warn_once = !!(condition);                    \
\
if (unlikely(__ret_warn_once))                          \
if (WARN_ON(!__warned))                         \
__warned = 1;                           \
unlikely(__ret_warn_once);                              \
})

#define WARN_ONCE(condition, format...) ({                      \
static int __warned;                                    \
int __ret_warn_once = !!(condition);                    \
\
if (unlikely(__ret_warn_once))                          \
if (WARN(!__warned, format))                    \
__warned = 1;                           \
unlikely(__ret_warn_once);                              \
})


#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define ENOTSUPP 524


#define ERR_CAST(err)    ((void *)((long)(err)))
#define ERR_PTR(err)    ((void *)((long)(err)))
#define PTR_ERR(ptr)    ((long)(ptr))
#define IS_ERR(ptr)     ((unsigned long)(ptr) > (unsigned long)(-1000))

/**
 * upper_32_bits - return bits 32-63 of a number
 * @n: the number we're accessing
 *
 * A basic shift-right of a 64- or 32-bit quantity.  Use this to suppress
 * the "right shift count >= width of type" warning when that quantity is
 * 32-bits.
 */
#define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((u32)(n))

struct va_format {
    const char *fmt;
    va_list *va;
};


#define __ALIGN_KERNEL_MASK(x, mask)    (((x) + (mask)) & ~(mask))
#define __ALIGN_KERNEL(x, a)        __ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
//#define ALIGN(x, a)            __ALIGN_KERNEL((x), (a))


/* @a is a power of 2 value */
#define ALIGN(x, a)        __ALIGN_KERNEL((x), (a))
#define ALIGN_DOWN(x, a)    __ALIGN_KERNEL((x) - ((a) - 1), (a))
#define __ALIGN_MASK(x, mask)    __ALIGN_KERNEL_MASK((x), (mask))
#define PTR_ALIGN(p, a)        ((typeof(p))ALIGN((unsigned long)(p), (a)))
#define IS_ALIGNED(x, a)        (((x) & ((typeof(x))(a) - 1)) == 0)



#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))


#define atomic_read(v)        READ_ONCE((v)->counter)
#define atomic64_read(v)    READ_ONCE((v)->counter)

#define ATOMIC_INIT(i)        { (i) }
#define ATOMIC64_INIT(i)    { (i) }

#define atomic_read(v)        READ_ONCE((v)->counter)
#define atomic64_read(v)    READ_ONCE((v)->counter)

#define atomic_set(v,i)        WRITE_ONCE((v)->counter, (i))
#define atomic64_set(v,i)    WRITE_ONCE((v)->counter, (i))


static inline int atomic_inc_return( atomic_t *v)
{
    v->counter++;
    return v->counter;
}

static inline void atomic_dec( atomic_t *v)
{
    v->counter--;
}

static inline int atomic_dec_return( atomic_t *v)
{
    v->counter--;
    return v->counter;
}

static inline void atomic_inc( atomic_t *v)
{
    v->counter++;
}

static inline int atomic_dec_if_positive(atomic_t *v)
{
    unsigned long flags;
    int res;
    
//    local_irq_save(flags);
    res = v->counter - 1;
    if (res >= 0)
        v->counter = res;
//    local_irq_restore(flags);
    
    return res;
}


# define do_div(n,base) ({                    \
uint32_t __base = (base);                \
uint32_t __rem;                        \
__rem = ((uint64_t)(n)) % __base;            \
(n) = ((uint64_t)(n)) / __base;                \
__rem;                            \
})


#define pci_name(x)  x->dev.name

#define NR_CPUS 1

#define for_each_possible_cpu(cpu) \
    for ((cpu) = 0; (cpu) < NR_CPUS; ++(cpu))

#define per_cpu_ptr(x, y) x

#define free_percpu(x)




static char *kvasprintf(gfp_t gfp, const char *fmt, va_list ap)
{
    unsigned int first, second;
    char *p;
    va_list aq;

    va_copy(aq, ap);
    first = vsnprintf(NULL, 0, fmt, aq);
    va_end(aq);

    p = (char *)kmalloc(first+1, gfp);
    if (!p)
        return NULL;

    second = vsnprintf(p, first+1, fmt, ap);
    WARN(first != second, "different return values (%u and %u) from vsnprintf(\"%s\", ...)",
         first, second, fmt);

    return p;
}

static char *kasprintf(gfp_t gfp, const char *fmt, ...)
{
    va_list ap;
    char *p;

    va_start(ap, fmt);
    p = kvasprintf(gfp, fmt, ap);
    va_end(ap);

    return p;
}


static int copy_to_user(void *kaddr, void * udaddr, size_t len)
{
    memcpy(udaddr, kaddr, len);
    return 0;
}

static int copy_from_user(void *kaddr, void * udaddr, size_t len)
{
    memcpy(kaddr, udaddr, len);
    return 0;
}

static int __must_check kstrtouint(const char *s, unsigned int base, unsigned int *res)
{
    return 0;
}

static inline int __must_check kstrtou32(const char *s, unsigned int base, u32 *res)
{
    return kstrtouint(s, base, res);
}
static int __must_check kstrtoint(const char *s, unsigned int base, int *res)
{
    return 0;
}


static int kstrtou8(const char *s, unsigned int base, u8 *res)
{
//    unsigned long long tmp;
//    int rv;
//
//    rv = kstrtoull(s, base, &tmp);
//    if (rv < 0)
//        return rv;
//    if (tmp != (u8)tmp)
//        return -ERANGE;
//    *res = tmp;
    return 0;
}

static int __must_check kstrtou16(const char *s, unsigned int base, u16 *res)
{
    return 0;
}

static inline int __must_check kstrtou32_from_user(const char __user *s, size_t count, unsigned int base, u32 *res)
{
//    return kstrtouint_from_user(s, count, base, res);
    return 0;
}

static int __must_check kstrtobool_from_user(const char __user *s, size_t count, bool *res)
{
    return 0;
}

static bool mac_pton(const char *s, u8 *mac)
{
    return true;
}


static void __dev_printk(const char *level, const struct device *dev,
            struct va_format *vaf)
{
    printk("%s(NULL device *): %pV", level, vaf);
}

static void dev_printk(const char *level, const struct device *dev,
        const char *fmt, ...)
{
    struct va_format vaf;
    va_list args;

    va_start(args, fmt);

    vaf.fmt = fmt;
    vaf.va = &args;

    __dev_printk(level, dev, &vaf);

    va_end(args);
}


#define max3(x, y, z) max((typeof(x))max(x, y), z)

#ifdef MOJAVE

/**
 * vscnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * The return value is the number of characters which have been written into
 * the @buf not including the trailing '\0'. If @size is == 0 the function
 * returns 0.
 *
 * If you're not already dealing with a va_list consider using scnprintf().
 *
 * See the vsnprintf() documentation for format string extensions over C99.
 */
static int vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int i;
    
    i = vsnprintf(buf, size, fmt, args);
    
    if (likely(i < size))
        return i;
    if (size != 0)
        return size - 1;
    return 0;
}

/**
 * scnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @...: Arguments for the format string
 *
 * The return value is the number of characters written into @buf not including
 * the trailing '\0'. If @size is == 0 the function returns 0.
 */

static int scnprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int i;
    
    va_start(args, fmt);
    i = vscnprintf(buf, size, fmt, args);
    va_end(args);
    
    return i;
}
#endif

#endif /* kernel_h */
