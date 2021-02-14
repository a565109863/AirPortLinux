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
#include <linux/bug.h>
#include <linux/_malloc.h>
#include <linux/time.h>
#include <linux/debugfs.h>
#include <linux/bitfield.h>
#include <linux/_string.h>
#include <linux/cpu.h>
#include <linux/atomic.h>

#include <asm/swab.h>

#define fallthrough

#define KERN_DEBUG 0
#define __KERNEL__ 1


#define __ro_after_init
#define __net_exit
#define  noinline_for_stack

#define inline

#define smp_load_acquire(x) *(x)
#define smp_store_release(x, y)

#include <linux/list.h>

#define NOTIFY_DONE        0x0000        /* Don't care */
#define NOTIFY_OK        0x0001        /* Suits me */

#define    NUMA_NO_NODE    (-1)
typedef unsigned int __wsum;


static inline void prefetch(const void *x)
{
    __builtin_prefetch(x, 0, 3);
}

static inline void prefetchw(const void *ptr)
{
    __builtin_prefetch(ptr, 1, 3);
}

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



#define request_module          kprintf

#define pr_emerg                kprintf
#define pr_warn                 kprintf
#define pr_warn_ratelimited     kprintf
#define pr_err                  kprintf
#define pr_debug                kprintf
#define pr_info                 kprintf
#define pr_err_ratelimited      kprintf

#define net_dbg_ratelimited     kprintf
#define net_warn_ratelimited    kprintf
#define net_info_ratelimited    kprintf

#define seq_printf(m, arg...)       kprintf(arg)
#define netdev_info(d, arg...)      kprintf(arg)
#define netdev_err(d, arg...)       kprintf(arg)
#define netdev_warn(d, arg...)      kprintf(arg)
#define netdev_crit(d, arg...)      kprintf(arg)
#define netdev_notice(d, arg...)    kprintf(arg)
#define netdev_alert(d, arg...)     kprintf(arg)
#define netdev_emerg(d, arg...)     kprintf(arg)
#define netdev_printk(l, d, arg...) kprintf(arg)

#define dev_printk(level, dev, fmt...)  kprintf(fmt)
#define dev_emerg(dev, fmt...)          kprintf(fmt)
#define dev_alert(dev, fmt...)          kprintf(fmt)
#define dev_crit(dev, fmt...)           kprintf(fmt)
#define dev_err(dev, fmt...)            kprintf(fmt)
#define dev_warn(dev, fmt...)           kprintf(fmt)
#define dev_notice(dev, fmt...)         kprintf(fmt)
#define dev_info(dev, fmt...)           kprintf(fmt)

#define __cond_lock(nic_access, y) y

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

#define __ALIGN_KERNEL_MASK(x, mask)    (((x) + (mask)) & ~(mask))
#define __ALIGN_KERNEL(x, a)        __ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
//#define ALIGN(x, a)            __ALIGN_KERNEL((x), (a))

#define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

/* @a is a power of 2 value */
#define ALIGN(x, a)        __ALIGN_KERNEL((x), (a))
#define ALIGN_DOWN(x, a)    __ALIGN_KERNEL((x) - ((a) - 1), (a))
#define __ALIGN_MASK(x, mask)    __ALIGN_KERNEL_MASK((x), (mask))
#define PTR_ALIGN(p, a)        ((typeof(p))ALIGN((unsigned long)(p), (a)))
#define IS_ALIGNED(x, a)        (((x) & ((typeof(x))(a) - 1)) == 0)



#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))


# define do_div(n,base) ({                    \
uint32_t __base = (base);                \
uint32_t __rem;                        \
__rem = ((uint64_t)(n)) % __base;            \
(n) = ((uint64_t)(n)) / __base;                \
__rem;                            \
})


#define pci_name(x)  x->dev.name

#define NR_CPUS 1

/*        0 - Reserved to indicate value not set
 *     1..NR_CPUS - Reserved for sender_cpu
 *  NR_CPUS+1..~0 - Region available for NAPI IDs
 */
#define MIN_NAPI_ID ((unsigned int)(NR_CPUS + 1))

#define for_each_possible_cpu(cpu) \
    for ((cpu) = 0; (cpu) < NR_CPUS; ++(cpu))

//#define per_cpu_ptr(x, y) x

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


static int copy_to_user(void * udaddr, void *kaddr, size_t len)
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



#define barrier()

/* This should act as an optimization barrier on var.
 * Given that this compiler does not have inline assembly, a compiler barrier
 * is the best we can do.
 */
#define OPTIMIZER_HIDE_VAR(var) barrier()

static inline unsigned long array_index_mask_nospec(unsigned long index,
                            unsigned long size)
{
    /*
     * Always calculate and emit the mask even if the compiler
     * thinks the mask is not needed. The compiler does not take
     * into account the value of @index under speculation.
     */
    OPTIMIZER_HIDE_VAR(index);
    return ~(long)(index | (size - 1UL - index)) >> (BITS_PER_LONG - 1);
}

/*
 * array_index_nospec - sanitize an array index after a bounds check
 *
 * For a code sequence like:
 *
 *     if (index < size) {
 *         index = array_index_nospec(index, size);
 *         val = array[index];
 *     }
 *
 * ...if the CPU speculates past the bounds check then
 * array_index_nospec() will clamp the index within the range of [0,
 * size).
 */
#define array_index_nospec(index, size)                    \
({                                    \
    typeof(index) _i = (index);                    \
    typeof(size) _s = (size);                    \
    unsigned long _mask = array_index_mask_nospec(_i, _s);        \
                                    \
    BUILD_BUG_ON(sizeof(_i) > sizeof(long));            \
    BUILD_BUG_ON(sizeof(_s) > sizeof(long));            \
                                    \
    (typeof(_i)) (_i & _mask);                    \
})


static inline __sum16 csum_fold(__wsum csum)
{
    u32 sum = (__force u32)csum;
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return (__force __sum16)~sum;
}


static float dcn_bw_pow(float a, float exp)
{
    float temp;
    /*ASSERT(exp == (int)exp);*/
    if ((int)exp == 0)
        return 1;
    temp = dcn_bw_pow(a, (int)(exp / 2));
    if (((int)exp % 2) == 0) {
        return temp * temp;
    } else {
        if ((int)exp > 0)
            return a * temp * temp;
        else
            return (temp * temp) / a;
    }
}

static inline double pow(double a, int exp)
{
    return (double) dcn_bw_pow(a, exp);
}

void synchronize_rcu(void);


/*
 * Values used for system_state. Ordering of the states must not be changed
 * as code checks for <, <=, >, >= STATE.
 */
extern enum system_states {
    SYSTEM_BOOTING,
    SYSTEM_SCHEDULING,
    SYSTEM_RUNNING,
    SYSTEM_HALT,
    SYSTEM_POWER_OFF,
    SYSTEM_RESTART,
    SYSTEM_SUSPEND,
} system_state;

#define set_current_state(x)

#endif /* kernel_h */
