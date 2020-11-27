/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_BITS_H
#define __LINUX_BITS_H

#include <linux/const.h>
#include <asm/bitsperlong.h>

#define BIT(nr)            (1UL << (nr))
#define BIT_ULL(nr)        (1ULL << (nr))
#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)
#define BIT_ULL_MASK(nr)    (1ULL << ((nr) % BITS_PER_LONG_LONG))
#define BIT_ULL_WORD(nr)    ((nr) / BITS_PER_LONG_LONG)
#define BITS_PER_BYTE        8
#define BITS_TO_LONGS(nr)    DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))


#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)
#define BIT_ULL_WORD(nr)    ((nr) / BITS_PER_LONG_LONG)
#define BITS_PER_BYTE        8

#define DMA_BIT_MASK(n)    (((n) == 64) ? ~0ULL : ((1ULL<<(n))-1))

/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
    (((~UL(0)) - (UL(1) << (l)) + 1) & \
     (~UL(0) >> (BITS_PER_LONG - 1 - (h))))

#define GENMASK_ULL(h, l) \
    (((~ULL(0)) - (ULL(1) << (l)) + 1) & \
     (~ULL(0) >> (BITS_PER_LONG_LONG - 1 - (h))))


static inline UInt64 OSBitwiseAtomic64(unsigned long and_mask, unsigned long or_mask, unsigned long xor_mask, unsigned long * value)
{
    unsigned long    oldValue;
    unsigned long    newValue;

    do {
        oldValue = *value;
        newValue = ((oldValue & and_mask) | or_mask) ^ xor_mask;
    } while (! OSCompareAndSwap64(oldValue, newValue, value));
    
    return oldValue;
}

static inline unsigned long OSBitAndAtomic64(unsigned long mask, unsigned long * value)
{
    return OSBitwiseAtomic64(mask, 0, 0, value);
}

static inline unsigned long OSBitOrAtomic64(unsigned long mask, unsigned long * value)
{
    return OSBitwiseAtomic64(-1, mask, 0, value);
}


/*
 *    Bit access functions vary across the ColdFire and 68k families.
 *    So we will break them out here, and then macro in the ones we want.
 *
 *    ColdFire - supports standard bset/bclr/bchg with register operand only
 *    68000    - supports standard bset/bclr/bchg with memory operand
 *    >= 68020 - also supports the bfset/bfclr/bfchg instructions
 *
 *    Although it is possible to use only the bset/bclr/bchg with register
 *    operands on all platforms you end up with larger generated code.
 *    So we use the best form possible on a given platform.
 */

static inline void bset_reg_set_bit(int nr, volatile unsigned long *vaddr)
{
    char *p = (char *)vaddr + (nr ^ 31) / 8;

//    __asm__ __volatile__ ("bset %1,(%0)"
//        :
//        : "a" (p), "di" (nr & 7)
//        : "memory");
}

static inline void bset_mem_set_bit(int nr, volatile unsigned long *vaddr)
{
    char *p = (char *)vaddr + (nr ^ 31) / 8;

//    __asm__ __volatile__ ("bset %1,%0"
//        : "+m" (*p)
//        : "di" (nr & 7));
}

static inline void bfset_mem_set_bit(int nr, volatile unsigned long *vaddr)
{
//    __asm__ __volatile__ ("bfset %1{%0:#1}"
//        :
//        : "d" (nr ^ 31), "o" (*vaddr)
//        : "memory");
}

#if defined(CONFIG_COLDFIRE)
#define    set_bit(nr, vaddr)    bset_reg_set_bit(nr, vaddr)
#elif defined(CONFIG_CPU_HAS_NO_BITFIELDS)
#define    set_bit(nr, vaddr)    bset_mem_set_bit(nr, vaddr)
#else
#define set_bit(nr, vaddr)    (__builtin_constant_p(nr) ? \
                bset_mem_set_bit(nr, vaddr) : \
                bfset_mem_set_bit(nr, vaddr))
#endif

#define __set_bit(nr, vaddr)    set_bit(nr, vaddr)



static inline void __clear_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
    
    *p &= ~mask;
}

static inline void clear_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
    OSBitAndAtomic64(~mask, p);
}


static inline int
test_bit(int nr, const volatile unsigned long *addr)
{
    return (OSAddAtomic(0, addr) & (1 << nr)) != 0;
}

static inline int test_and_set_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
    unsigned long old;
//    unsigned long flags;
    
    old = *p;
    *p = old | mask;
    
    return (old & mask) != 0;
}


// Bit manipulation, rewritten to use mach routines
#define test_bit(x, y) isset(y, x)
#define clear_bit(x, y) clrbit(y, x)
#define test_and_clear_bit(x,y)        \
({        \
    int r;        \
    r=test_bit(x, y);        \
    clear_bit(x, y);        \
    r;        \
})
#define test_and_set_bit(x,y)        \
({        \
    int r;        \
    r=test_bit(x, y);        \
    set_bit(x, y);        \
    r;        \
})


#endif    /* __LINUX_BITS_H */
