//
//  bitfield.h
//  AppleIwlWiFi
//
//  Created by User-PC on 2019/11/7.
//  Copyright © 2019 User-PC. All rights reserved.
//

#ifndef bitfield_h
#define bitfield_h

#include <linux/types.h>

# define __compiletime_assert(condition, msg, prefix, suffix) do { } while (0);

#define _compiletime_assert(condition, msg, prefix, suffix) \
__compiletime_assert(condition, msg, prefix, suffix)


#define compiletime_assert(condition, msg) \
_compiletime_assert(condition, msg, __compiletime_assert_, __LINE__)

#define BUILD_BUG_ON_MSG(cond, msg) compiletime_assert(!(cond), msg)

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

#define __BF_FIELD_CHECK(_mask, _reg, _val, _pfx)            \
({                                \
BUILD_BUG_ON_MSG(!__builtin_constant_p(_mask),        \
_pfx "mask is not constant");        \
BUILD_BUG_ON_MSG((_mask) == 0, _pfx "mask is zero");    \
BUILD_BUG_ON_MSG(__builtin_constant_p(_val) ?        \
~((_mask) >> __bf_shf(_mask)) & (_val) : 0, \
_pfx "value too large for the field"); \
BUILD_BUG_ON_MSG((_mask) > (typeof(_reg))~0ull,        \
_pfx "type of reg too small for mask"); \
__BUILD_BUG_ON_NOT_POWER_OF_2((_mask) +            \
(1ULL << __bf_shf(_mask))); \
})

/**
 * FIELD_FIT() - check if value fits in the field
 * @_mask: shifted mask defining the field's length and position
 * @_val:  value to test against the field
 *
 * Return: true if @_val can fit inside @_mask, false if @_val is too big.
 */
#define FIELD_FIT(_mask, _val)                        \
({                                \
!((((typeof(_mask))_val) << __bf_shf(_mask)) & ~(_mask)); \
})

/**
 * FIELD_PREP() - prepare a bitfield element
 * @_mask: shifted mask defining the field's length and position
 * @_val:  value to put in the field
 *
 * FIELD_PREP() masks and shifts up the value.  The result should
 * be combined with other fields of the bitfield using logical OR.
 */
#define FIELD_PREP(_mask, _val)                        \
({                                \
((typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask);    \
})

/**
 * FIELD_GET() - extract a bitfield element
 * @_mask: shifted mask defining the field's length and position
 * @_reg:  value of entire bitfield
 *
 * FIELD_GET() extracts the field specified by @_mask from the
 * bitfield passed in as @_reg by masking and shifting it down.
 */
#define FIELD_GET(_mask, _reg)                        \
({                                \
(typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask));    \
})

//extern void __compiletime_error("value doesn't fit into mask")
static void __field_overflow(void){}
//extern void __compiletime_error("bad bitfield mask")
static void __bad_mask(void){}
static __always_inline u64 field_multiplier(u64 field)
{
    if ((field | (field - 1)) & ((field | (field - 1)) + 1))
        __bad_mask();
    return field & -field;
}
static __always_inline u64 field_mask(u64 field)
{
    return field / field_multiplier(field);
}
#define ____MAKE_OP(type,base,to,from)                    \
static __always_inline __##type type##_encode_bits(base v, base field)    \
{                                    \
    return to((v & field_mask(field)) * field_multiplier(field));    \
}                                    \
static __always_inline __##type type##_replace_bits(__##type old,    \
                    base val, base field)        \
{                                    \
    return (old & ~to(field)) | type##_encode_bits(val, field);    \
}                                    \
static __always_inline void type##p_replace_bits(__##type *p,        \
                    base val, base field)        \
{                                    \
    *p = (*p & ~to(field)) | type##_encode_bits(val, field);    \
}                                    \
static __always_inline base type##_get_bits(__##type v, base field)    \
{                                    \
    return (from(v) & field)/field_multiplier(field);        \
}
#define __MAKE_OP(size)                            \
    ____MAKE_OP(le##size,u##size,cpu_to_le##size,le##size##_to_cpu)    \
    ____MAKE_OP(be##size,u##size,cpu_to_be##size,be##size##_to_cpu)    \
    ____MAKE_OP(u##size,u##size,,)
____MAKE_OP(u8,u8,,)
__MAKE_OP(16)
__MAKE_OP(32)
__MAKE_OP(64)
#undef __MAKE_OP
#undef ____MAKE_OP


/**
 * __ffs - find first set bit in word
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
//static inline unsigned long __ffs(unsigned long word)
//{
//    asm("rep; bsf %1,%0"
//        : "=r" (word)
//        : "rm" (word));
//    return word;
//}

//unsigned long find_next_bit(const unsigned long *addr, unsigned long size, unsigned long offset);


//#define for_each_set_bit(bit, addr, size) \
//for ((bit) = find_first_bit((addr), (size));        \
//(bit) < (size);                    \
//(bit) = find_next_bit((addr), (unsigned long)(size), (bit) + 1))
//
//
//#define find_first_bit(addr, size) find_next_bit((addr), (size), 0)
//
//#define FIELD_GET(_mask, _reg)                        \
//({                                \
//__BF_FIELD_CHECK(_mask, _reg, 0U, "FIELD_GET: ");    \
//(typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask));    \
//})

#endif /* bitfield_h */
