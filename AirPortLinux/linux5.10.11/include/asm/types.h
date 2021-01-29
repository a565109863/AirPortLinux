//
//  types.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef asm_types_h
#define asm_types_h


#include <string.h>

#include <sys/systm.h>
#include <sys/types.h>
#include <sys/kpi_mbuf.h>
#include <sys/mbuf.h>
#include <sys/cdefs.h>
#include <sys/_endian.h>
#include <sys/param.h>
#include <sys/malloc.h>
#include <sys/vm.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <sys/queue.h>

#include <net/if_var.h>
#include <net/if.h>
#include <net/if_var.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <net/dlil.h>
#include <net/bpf.h>
#include <netinet/if_ether.h>
#include <netinet/in_arp.h>


#include <libkern/OSTypes.h>
#include <libkern/OSKextLib.h>

#include <IOKit/network/IOEthernetInterface.h>
#include <IOKit/network/IOGatedOutputQueue.h>
#include <IOKit/network/IOMbufMemoryCursor.h>
#include <IOKit/network/IOPacketQueue.h>
#include <IOKit/network/IONetworkMedium.h>
#include <IOKit/network/IONetworkController.h>

#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IODMACommand.h>
#include <IOKit/IODeviceMemory.h>
#include <IOKit/IODataQueue.h>
#include <IOKit/IOMemoryDescriptor.h>
#include <IOKit/IOInterruptEventSource.h>
#include <IOKit/IOFilterInterruptEventSource.h>

#include <IOKit/assert.h>
#include <IOKit/pci/IOPCIDevice.h>



#include "errno.h"

typedef IOSimpleLock* spinlock_t;

//typedef IOPhysicalAddress dma_addr_t;


#define DebugLog(x, args...) \
if(1) { \
thread_t new_thread = current_thread(); \
uint64_t new_thread_id = thread_tid(new_thread); \
kprintf(x " tid = %llu", args, new_thread_id); \
IOSleep(1000); \
}


typedef struct { volatile int counter; } atomic_t;
typedef struct { volatile int counter; } atomic64_t;

typedef atomic64_t atomic_long_t;

typedef UInt8  u8;
typedef UInt16 u16;
typedef UInt32 u32;
typedef UInt64 u64;

typedef u8 __u8;
typedef u16 __u16;
typedef u32 __u32;
typedef u64 __u64;

typedef  UInt16 __be16;
typedef  UInt32 __be32;
typedef  UInt64 __be64;
typedef  UInt16 __le16;
typedef  UInt32 __le32;
typedef  UInt64 __le64;

typedef SInt8  s8;
typedef SInt16 s16;
typedef SInt32 s32;
typedef SInt64 s64;

typedef s8  __s8;
typedef s16 __s16;
typedef s32 __s32;
typedef s64 __s64;


#define U8_MAX        ((u8)~0U)
#define S8_MAX        ((s8)(U8_MAX >> 1))
#define S8_MIN        ((s8)(-S8_MAX - 1))
#define U16_MAX        ((u16)~0U)
#define S16_MAX        ((s16)(U16_MAX >> 1))
#define S16_MIN        ((s16)(-S16_MAX - 1))
#define U32_MAX        ((u32)~0U)
#define S32_MAX        ((s32)(U32_MAX >> 1))
#define S32_MIN        ((s32)(-S32_MAX - 1))
#define U64_MAX        ((u64)~0ULL)
#define S64_MAX        ((s64)(U64_MAX >> 1))
#define S64_MIN        ((s64)(-S64_MAX - 1))


//#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
//
//typedef unsigned char        u8;
//typedef unsigned short        u16;
//typedef unsigned int        u32;
//typedef unsigned long long    u64;
//typedef signed char        s8;
//typedef short            s16;
//typedef int            s32;
//typedef long long        s64;
//
/* required for opal-api.h */
typedef u8  uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;
typedef s8  int8_t;
typedef s16 int16_t;
typedef s32 int32_t;
typedef s64 int64_t;


typedef u64 dma_addr_t;
typedef u64 phys_addr_t;

typedef long long       __kernel_loff_t;
typedef __kernel_loff_t         loff_t;

//#define min(x,y) ({ \
//    typeof(x) _x = (x);    \
//    typeof(y) _y = (y);    \
//    (void) (&_x == &_y);    \
//    _x < _y ? _x : _y; })
//
//#define max(x,y) ({ \
//    typeof(x) _x = (x);    \
//    typeof(y) _y = (y);    \
//    (void) (&_x == &_y);    \
//    _x > _y ? _x : _y; })

#define min_t(type, a, b) min((type) (a), (type) (b))
#define max_t(type, a, b) max((type) (a), (type) (b))


#define abs(x) ({               \
int __x = (x);          \
(__x < 0) ? -__x : __x;     \
})


#define __builtin_expect(x, expected_value) (x)
#define unlikely(x) __builtin_expect(!!(x), 0)

#include <net/compat.h>


#define debug_noinline
#define noinline

#define OPTIMIZER_HIDE_VAR(x)

#define __packed __attribute__((packed))
#define __aligned(x)        __attribute__((aligned(x)))
#define __must_check        __attribute__((warn_unused_result))

#define __alias(symbol)

#define  __printf(x, y) __printflike(x, y)
#define barrier()
#define mb()

#define EXPORT_SYMBOL(x)
#define EXPORT_SYMBOL_GPL(x)

#define ASSERT_RTNL()


#define pr_emerg                kprintf
#define pr_warn                 kprintf
#define pr_warn_ratelimited     kprintf
#define net_dbg_ratelimited     kprintf
#define seq_printf(m, arg...)   kprintf(arg)


#define might_sleep()
#define msleep(x) IODelay(x * 1000)
#define mdelay(x) IODelay(x * 1000)
#define udelay(x) IODelay(x)

static int msleep_interruptible(int x)
{
    IODelay(x);
    return 0;
}

#define IWL_EXPORT_SYMBOL(sym)
#define __percpu
#define __releases(x)
#define __release(x)
#define __acquires(x)
#define __acquire(x)
#define smp_rmb()

#define __stringify(x) x

#define dump_stack(x)
#define smp_mb()

#define __always_inline
#define __bitwise__
#define __bitwise
#define __force
#define __init
#define __exit
#define __user
#define __iomem
#define __maybe_unused
#define __sched

#define __attribute_const__
#define ____cacheline_aligned_in_smp
#define ____cacheline_aligned
#define __randomize_layout


#define __force
#define __user
#define __read_mostly

#define rcu_barrier()


typedef __u16 __bitwise __le16;
typedef __u16 __bitwise __be16;
typedef __u32 __bitwise __le32;
typedef __u32 __bitwise __be32;
typedef __u64 __bitwise __le64;
typedef __u64 __bitwise __be64;

typedef __u16 __bitwise __sum16;
typedef __u32 __bitwise __wsum;
typedef unsigned long kernel_ulong_t;
typedef unsigned int __bitwise __poll_t;



#define __cpu_to_le64(x) OSSwapHostToLittleInt64(x)
#define __le64_to_cpu(x) OSSwapLittleToHostInt64(x)
#define __cpu_to_le32(x) OSSwapHostToLittleInt32(x)
#define __le32_to_cpu(x) OSSwapLittleToHostInt32(x)
#define __cpu_to_le16(x) OSSwapHostToLittleInt16(x)
#define __le16_to_cpu(x) OSSwapLittleToHostInt16(x)
#define __cpu_to_be64(x) OSSwapHostToBigInt64(x)
#define __be64_to_cpu(x) OSSwapBigToHostInt64(x)
#define __cpu_to_be32(x) OSSwapHostToBigInt32(x)
#define __be32_to_cpu(x) OSSwapBigToHostInt32(x)
#define __cpu_to_be16(x) OSSwapHostToBigInt16(x)
#define __be16_to_cpu(x) OSSwapBigToHostInt16(x)

#define cpu_to_le64 __cpu_to_le64
#define le64_to_cpu __le64_to_cpu
#define cpu_to_le32 __cpu_to_le32
#define le32_to_cpu __le32_to_cpu
#define cpu_to_le16 __cpu_to_le16
#define le16_to_cpu __le16_to_cpu
#define cpu_to_be64 __cpu_to_be64
#define be64_to_cpu __be64_to_cpu
#define cpu_to_be32 __cpu_to_be32
#define be32_to_cpu __be32_to_cpu
#define cpu_to_be16 __cpu_to_be16
#define be16_to_cpu __be16_to_cpu

static inline void le16_add_cpu(__le16 *var, u16 val)
{
    *var = cpu_to_le16(le16_to_cpu(*var) + val);
}

static inline void le32_add_cpu(__le32 *var, u32 val)
{
    *var = cpu_to_le32(le32_to_cpu(*var) + val);
}

static inline void le64_add_cpu(__le64 *var, u64 val)
{
    *var = cpu_to_le64(le64_to_cpu(*var) + val);
}



static inline __u16 __le16_to_cpup(const __le16 *p)
{
    return __le16_to_cpu(*p);
}

static inline __u32 __le32_to_cpup(const __le32 *p)
{
    return __le32_to_cpu(*p);
}

static inline __u64 __le64_to_cpup(const __le64 *p)
{
    return __le64_to_cpu(*p);
}

static inline __u16 __be16_to_cpup(const __be16 *p)
{
    return __be16_to_cpu(*p);
}

static inline __u32 __be32_to_cpup(const __be32 *p)
{
    return __be32_to_cpu(*p);
}

static inline __u64 __be64_to_cpup(const __be64 *p)
{
    return __be64_to_cpu(*p);
}

#define le64_to_cpup __le64_to_cpup
#define le32_to_cpup __le32_to_cpup
#define le16_to_cpup __le16_to_cpup
#define be64_to_cpup __be64_to_cpup
#define be32_to_cpup __be32_to_cpup
#define be16_to_cpup __be16_to_cpup





#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))







#define HZ 1000
#define USEC_PER_MSEC    1000UL
//#define USEC_PER_SEC    1000000UL
#define USEC_PER_JIFFY    (USEC_PER_SEC / HZ)


#define CONFIG_IWLDVM   1
#define CONFIG_IWLMVM   1
#define CONFIG_CFG80211 1
//#define CONFIG_ACPI     1

#define IS_ENABLED(x) (x)

static
void get_random_bytes(void *buf, int nbytes)
{
//    static void *previous;
//
//    warn_unseeded_randomness(&previous);
//    _get_random_bytes(buf, nbytes);
}

struct rcu_head {
    struct rcu_head *next;
    void (*func)(struct rcu_head *head);
};


#define IS_ERR_VALUE(x) unlikely((x) > (unsigned long)-1000L)

static inline void * ERR_PTR(long error)
{
    return (void *) error;
}

static inline long __must_check PTR_ERR(__force const void *ptr)
{
    return (long) ptr;
}

static inline bool __must_check IS_ERR(__force const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

static inline bool __must_check IS_ERR_OR_NULL(__force const void *ptr)
{
    return unlikely(!ptr) || IS_ERR_VALUE((unsigned long)ptr);
}

static inline int __must_check PTR_ERR_OR_ZERO(__force const void *ptr)
{
    if (IS_ERR(ptr))
        return PTR_ERR(ptr);
    else
        return 0;
}



#define __rcu
#define rcu_dereference(x) x
#define rcu_read_lock()
#define rcu_read_unlock()
//#define rcu_dereference_protected(x, y) x
//#define RCU_INIT_POINTER(x, y) x = y

#define rtnl_lock()
#define rtnl_unlock()

#define rcu_access_pointer(x) (x)
//#define rcu_dereference_check(p, c) p

#define smp_wmb()
#define rcu_assign_pointer(p, v) \
({ \
(p) = (v); \
})

#define kfree_rcu(x, y)



#define struct_size(x, y, z) sizeof(*x) + sizeof(typeof(x->y[0])) * z

#define rtnl_dereference(x) x

#define num_online_cpus() 8
#define cpumask_next(x, y) 1
#define cpumask_set_cpu(x, y)
#define irq_set_affinity_hint(x, y) 0
#define synchronize_irq(x)

#define IRQF_SHARED 0

#define dev_printk(level, dev, format...)  kprintf(format)
#define dev_emerg(dev, format...)   kprintf(format)
#define dev_alert(dev, format...)   kprintf(format)
#define dev_crit(dev, format...)    kprintf(format)
#define dev_err(dev, format...)     kprintf(format)
#define dev_warn(dev, format...)    kprintf(format)
#define dev_notice(dev, format...)  kprintf(format)
#define dev_info(dev, format...)    kprintf(format)
#define ksize(x) sizeof(&x)


#define sizeof_field(TYPE, MEMBER) sizeof((((TYPE *)0)->MEMBER))

#define offsetofend(TYPE, MEMBER) \
(offsetof(TYPE, MEMBER)    + sizeof_field(TYPE, MEMBER))

#define CONFIG_IWLWIFI_LEDS 1
#define CONFIG_IWLWIFI_DEBUGFS 1
#define CONFIG_MAC80211_DEBUGFS 1

#define CONFIG_PM 1
#define CONFIG_64BIT 1

#define CONFIG_MAC80211_STA_HASH_MAX_SIZE 128

#endif /* asm_types_h */
