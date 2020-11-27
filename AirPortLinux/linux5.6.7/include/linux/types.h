//
//  types.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef linux_types_h
#define linux_types_h

#include <asm/types.h>


#define DECLARE_BITMAP(name,bits) \
    unsigned long name[BITS_TO_LONGS(bits)]

#define bitmap_zero bzero
#define memzero_explicit bzero

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

//typedef __signed__ char __s8;
//typedef unsigned char __u8;
//
//typedef __signed__ short __s16;
//typedef unsigned short __u16;
//
//typedef __signed__ int __s32;
//typedef unsigned int __u32;

//typedef __signed__ long __s64;
//typedef unsigned long __u64;


typedef u64            uint64_t;
typedef u64            u_int64_t;
typedef s64            int64_t;

typedef unsigned long kernel_ulong_t;

typedef void (*swap_func_t)(void *a, void *b, int size);

typedef int (*cmp_r_func_t)(const void *a, const void *b, const void *priv);
typedef int (*cmp_func_t)(const void *a, const void *b);



//typedef enum {
//    GFP_KERNEL,
//    GFP_ATOMIC,
//    __GFP_HIGHMEM,
//    __GFP_HIGH
//} gfp_t;

/*
 * We define u64 as uint64_t for every architecture
 * so that we can print it with "%"PRIx64 without getting warnings.
 *
 * typedef __u64 u64;
 * typedef __s64 s64;
 */
//typedef uint64_t u64;
//typedef int64_t s64;
//
//typedef __u32 u32;
//typedef __s32 s32;
//
//typedef __u16 u16;
//typedef __s16 s16;
//
//typedef __u8  u8;
//typedef __s8  s8;

#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif
#define __bitwise __bitwise__

#define __force
#define __user
#define __must_check
#define __cold

//typedef __u16 __bitwise __le16;
//typedef __u16 __bitwise __be16;
//typedef __u32 __bitwise __le32;
//typedef __u32 __bitwise __be32;
//typedef __u64 __bitwise __le64;
//typedef __u64 __bitwise __be64;


#ifndef __aligned_u64
# define __aligned_u64 __u64 __attribute__((aligned(8)))
#endif

struct list_head {
    struct list_head *next, *prev;
};

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};


#include <linux/bitops.h>


/* Probably won't need to deal with bottom halves. */
static inline void local_bh_disable(void) {}
static inline void local_bh_enable(void) {}


enum {
    DUMP_PREFIX_NONE,
    DUMP_PREFIX_ADDRESS,
    DUMP_PREFIX_OFFSET
};


#define print_hex_dump(x, args...) kprintf(args)

#endif /* linux_types_h */
