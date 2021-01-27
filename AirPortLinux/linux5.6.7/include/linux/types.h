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

struct llist_head {
    struct llist_node *first;
};

struct llist_node {
    struct llist_node *next;
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


#define WRITE_ONCE(var, val) \
    (*((volatile typeof(val) *)(&(var))) = (val))

#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))


/**
 * struct callback_head - callback structure for use with RCU and task_work
 * @next: next update requests in a list
 * @func: actual update function to call after the grace period.
 *
 * The struct is aligned to size of pointer. On most architectures it happens
 * naturally due ABI requirements, but some architectures (like CRIS) have
 * weird ABI and we need to ask it explicitly.
 *
 * The alignment is required to guarantee that bit 0 of @next will be
 * clear under normal conditions -- as long as we use call_rcu() or
 * call_srcu() to queue the callback.
 *
 * This guarantee is important for few reasons:
 *  - future call_rcu_lazy() will make use of lower bits in the pointer;
 *  - the structure shares storage space in struct page with @compound_head,
 *    which encode PageTail() in bit 0. The guarantee is needed to avoid
 *    false-positive PageTail().
 */
struct callback_head {
    struct callback_head *next;
    void (*func)(struct callback_head *head);
} __attribute__((aligned(sizeof(void *))));
//#define rcu_head callback_head

typedef void (*rcu_callback_t)(struct rcu_head *head);
typedef void (*call_rcu_func_t)(struct rcu_head *head, rcu_callback_t func);

typedef void (*swap_func_t)(void *a, void *b, int size);

typedef int (*cmp_r_func_t)(const void *a, const void *b, const void *priv);
typedef int (*cmp_func_t)(const void *a, const void *b);

#endif /* linux_types_h */
