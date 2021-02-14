//
//  compat.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/5/6.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef net_compat_h
#define net_compat_h

#include <linux/types.h>

static inline s64
atomic64_add_return(s64 i, atomic64_t *v)
{
    s64 ret = 0;
//    __atomic_pre_full_fence();
//    ret = atomic64_add_return_relaxed(i, v);
//    __atomic_post_full_fence();
    return ret;
}

#define atomic64_add_return_relaxed atomic64_add_return

static inline s64
atomic64_inc_return_relaxed(atomic64_t *v)
{
    return atomic64_add_return_relaxed(1, v);
}

static inline s64
atomic64_inc_return(atomic64_t *v)
{
    s64 ret;
//    __atomic_pre_full_fence();
    ret = atomic64_inc_return_relaxed(v);
//    __atomic_post_full_fence();
    return ret;
}


static __inline__ int atomic_fetch_add_unless(atomic_t *v, int a, int u)
{
//    int c, _new, old;
//    smp_mb();
//    __asm__ __volatile__(
//                         "1:    ldl_l    %[old],%[mem]\n"
//                         "    cmpeq    %[old],%[u],%[c]\n"
//                         "    addl    %[old],%[a],%[_new]\n"
//                         "    bne    %[c],2f\n"
//                         "    stl_c    %[_new],%[mem]\n"
//                         "    beq    %[_new],3f\n"
//                         "2:\n"
//                         ".subsection 2\n"
//                         "3:    br    1b\n"
//                         ".previous"
//                         : [old] "=&r"(old), [_new] "=&r"(_new), [c] "=&r"(c)
//                         : [mem] "m"(*v), [a] "rI"(a), [u] "rI"((long)u)
//                         : "memory");
//    smp_mb();
    return 0;
}



static u32 prandom_u32(void)
{
    /* Used only by lock_pin_lock() which is dead code */
//    abort();
    return random();
}

/**
 * prandom_u32_max - returns a pseudo-random number in interval [0, ep_ro)
 * @ep_ro: right open interval endpoint
 *
 * Returns a pseudo-random number that is in interval [0, ep_ro). Note
 * that the result depends on PRNG being well distributed in [0, ~0U]
 * u32 space. Here we use maximally equidistributed combined Tausworthe
 * generator, that is, prandom_u32(). This is useful when requesting a
 * random index of an array containing ep_ro elements, for example.
 *
 * Returns: pseudo-random number in interval [0, ep_ro)
 */
static inline u32 prandom_u32_max(u32 ep_ro)
{
    return (u32)(((u64) prandom_u32() * ep_ro) >> 32);
}

#endif /* net_compat_h */
