/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_
#define _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_


#include <libkern/OSTypes.h>
#include <libkern/OSAtomic.h>
#include <libkern/libkern.h>


#define BITS_PER_LONG 64
#define BITS_PER_LONG_LONG 64


#define swap(a, b) \
    do { __typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while(0)

static inline int
fls64(long long mask)
{
    int bit;
    
    if (mask == 0)
        return (0);
    for (bit = 1; mask != 1; bit++)
        mask = (unsigned long long)mask >> 1;
    return (bit);
}

//#define fls fls64

static inline unsigned long __ffs(unsigned long x)
{
    return fls64(x) - 1;
}

static inline unsigned long __fls(unsigned long x)
{
    return fls64(x) - 1;
}


static inline int __fls(int x)
{
    return fls(x) - 1;
}



/* If normalization is done by loops, the even/odd algorithm is a win. */
static unsigned long gcd(unsigned long a, unsigned long b)
{
    unsigned long r = a | b;

    if (!a || !b)
        return r;

    /* Isolate lsbit of r */
    r &= -r;

    while (!(b & r))
        b >>= 1;
    if (b == r)
        return r;

    for (;;) {
        while (!(a & r))
            a >>= 1;
        if (a == r)
            return r;
        if (a == b)
            return a;

        if (a < b)
            swap(a, b);
        a -= b;
        a >>= 1;
        if (a & r)
            a += b;
        a >>= 1;
    }
}












/*
 * hweightN: returns the hamming weight (i.e. the number
 * of bits set) of a N-bit word
 */
static __inline__ unsigned long __arch_hweight64(unsigned long x)
{
    unsigned long result;
//    result = ia64_popcnt(x);
    return result;
}

#define __arch_hweight32(x) ((unsigned int) __arch_hweight64((x) & 0xfffffffful))
#define __arch_hweight16(x) ((unsigned int) __arch_hweight64((x) & 0xfffful))
#define __arch_hweight8(x)  ((unsigned int) __arch_hweight64((x) & 0xfful))

/*
 * Compile time versions of __arch_hweightN()
 */
#define __const_hweight8(w)        \
    ((unsigned int)            \
     ((!!((w) & (1ULL << 0))) +    \
      (!!((w) & (1ULL << 1))) +    \
      (!!((w) & (1ULL << 2))) +    \
      (!!((w) & (1ULL << 3))) +    \
      (!!((w) & (1ULL << 4))) +    \
      (!!((w) & (1ULL << 5))) +    \
      (!!((w) & (1ULL << 6))) +    \
      (!!((w) & (1ULL << 7)))))

#define __const_hweight16(w) (__const_hweight8(w)  + __const_hweight8((w)  >> 8 ))
#define __const_hweight32(w) (__const_hweight16(w) + __const_hweight16((w) >> 16))
#define __const_hweight64(w) (__const_hweight32(w) + __const_hweight32((w) >> 32))

/*
 * Generic interface.
 */
#define hweight8(w)  (__builtin_constant_p(w) ? __const_hweight8(w)  : __arch_hweight8(w))
#define hweight16(w) (__builtin_constant_p(w) ? __const_hweight16(w) : __arch_hweight16(w))
#define hweight32(w) (__builtin_constant_p(w) ? __const_hweight32(w) : __arch_hweight32(w))
#define hweight64(w) (__builtin_constant_p(w) ? __const_hweight64(w) : __arch_hweight64(w))


#define BUILD_BUG_ON_ZERO(e) (0)

/*
 * Interface for known constant arguments
 */
#define HWEIGHT8(w)  (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight8(w))
#define HWEIGHT16(w) (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight16(w))
#define HWEIGHT32(w) (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight32(w))
#define HWEIGHT64(w) (BUILD_BUG_ON_ZERO(!__builtin_constant_p(w)) + __const_hweight64(w))

/*
 * Type invariant interface to the compile time constant hweight functions.
 */
#define HWEIGHT(w)   HWEIGHT64((u64)w)

#endif /* _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_ */
