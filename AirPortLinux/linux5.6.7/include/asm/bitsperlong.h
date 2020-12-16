/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_
#define _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_


#include <libkern/OSTypes.h>
#include <libkern/OSAtomic.h>


#define BITS_PER_LONG 64
#define BITS_PER_LONG_LONG 64


static inline int fls64(UInt64 x)
{
    int bitpos = -1;
    /*
     * AMD64 says BSRQ won't clobber the dest reg if x==0; Intel64 says the
     * dest reg is undefined if x==0, but their CPU architect says its
     * value is written to set it to the same as before.
     */
    asm("bsrq %1,%q0"
        : "+r" (bitpos)
        : "rm" (x));
    return bitpos + 1;
}

#define fls fls64

static inline unsigned long __ffs(unsigned long x)
{
    return ffs(x) - 1;
}

static inline int __fls(int x)
{
    return fls(x) - 1;
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
