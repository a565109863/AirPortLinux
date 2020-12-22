//
//  gfp.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef gfp_h
#define gfp_h


#define GFP_KERNEL 0
#define GFP_ATOMIC 1

#include <linux/types.h>

typedef unsigned int __bitwise gfp_t;
typedef unsigned int __bitwise slab_flags_t;
typedef unsigned int __bitwise fmode_t;


/*
 * In case of changes, please don't forget to update
 * include/trace/events/mmflags.h and tools/perf/builtin-kmem.c
 */

/* Plain integer GFP bitmasks. Do not use this directly. */
#define ___GFP_DMA        0x01u
#define ___GFP_HIGHMEM        0x02u
#define ___GFP_DMA32        0x04u
#define ___GFP_MOVABLE        0x08u
#define ___GFP_RECLAIMABLE    0x10u
#define ___GFP_HIGH        0x20u
#define ___GFP_IO        0x40u
#define ___GFP_FS        0x80u
#define ___GFP_ZERO        0x100u
#define ___GFP_ATOMIC        0x200u
#define ___GFP_DIRECT_RECLAIM    0x400u
#define ___GFP_KSWAPD_RECLAIM    0x800u
#define ___GFP_WRITE        0x1000u
#define ___GFP_NOWARN        0x2000u
#define ___GFP_RETRY_MAYFAIL    0x4000u
#define ___GFP_NOFAIL        0x8000u
#define ___GFP_NORETRY        0x10000u
#define ___GFP_MEMALLOC        0x20000u
#define ___GFP_COMP        0x40000u
#define ___GFP_NOMEMALLOC    0x80000u
#define ___GFP_HARDWALL        0x100000u
#define ___GFP_THISNODE        0x200000u
#define ___GFP_ACCOUNT        0x400000u
#ifdef CONFIG_LOCKDEP
#define ___GFP_NOLOCKDEP    0x800000u
#else
#define ___GFP_NOLOCKDEP    0
#endif
/* If the above are modified, __GFP_BITS_SHIFT may need updating */

/*
 * Physical address zone modifiers (see linux/mmzone.h - low four bits)
 *
 * Do not put any conditional on these. If necessary modify the definitions
 * without the underscores and use them consistently. The definitions here may
 * be used in bit comparisons.
 */
#define __GFP_DMA    ((__force gfp_t)___GFP_DMA)
#define __GFP_HIGHMEM    ((__force gfp_t)___GFP_HIGHMEM)
#define __GFP_DMA32    ((__force gfp_t)___GFP_DMA32)
#define __GFP_MOVABLE    ((__force gfp_t)___GFP_MOVABLE)  /* ZONE_MOVABLE allowed */
#define GFP_ZONEMASK    (__GFP_DMA|__GFP_HIGHMEM|__GFP_DMA32|__GFP_MOVABLE)


/**
 * DOC: Watermark modifiers
 *
 * Watermark modifiers -- controls access to emergency reserves
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * %__GFP_HIGH indicates that the caller is high-priority and that granting
 * the request is necessary before the system can make forward progress.
 * For example, creating an IO context to clean pages.
 *
 * %__GFP_ATOMIC indicates that the caller cannot reclaim or sleep and is
 * high priority. Users are typically interrupt handlers. This may be
 * used in conjunction with %__GFP_HIGH
 *
 * %__GFP_MEMALLOC allows access to all memory. This should only be used when
 * the caller guarantees the allocation will allow more memory to be freed
 * very shortly e.g. process exiting or swapping. Users either should
 * be the MM or co-ordinating closely with the VM (e.g. swap over NFS).
 *
 * %__GFP_NOMEMALLOC is used to explicitly forbid access to emergency reserves.
 * This takes precedence over the %__GFP_MEMALLOC flag if both are set.
 */
#define __GFP_ATOMIC    ((__force gfp_t)___GFP_ATOMIC)
#define __GFP_HIGH    ((__force gfp_t)___GFP_HIGH)
#define __GFP_MEMALLOC    ((__force gfp_t)___GFP_MEMALLOC)
#define __GFP_NOMEMALLOC ((__force gfp_t)___GFP_NOMEMALLOC)


#define __GFP_BITS_SHIFT 26
#define __GFP_BITS_MASK ((gfp_t)((1 << __GFP_BITS_SHIFT) - 1))

//#define __GFP_HIGH        0x20u
//#define __GFP_IO        0x40u
//#define __GFP_FS        0x80u
//#define __GFP_NOWARN        0x200u
//#define __GFP_ZERO        0x8000u
//#define __GFP_ATOMIC        0x80000u
#define __GFP_ACCOUNT        0x100000u
//#define __GFP_DIRECT_RECLAIM    0x400000u
//#define __GFP_KSWAPD_RECLAIM    0x2000000u

//#define __GFP_RECLAIM    (__GFP_DIRECT_RECLAIM|__GFP_KSWAPD_RECLAIM)

#define __GFP_IO    ((__force gfp_t)___GFP_IO)
#define __GFP_FS    ((__force gfp_t)___GFP_FS)
#define __GFP_DIRECT_RECLAIM    ((__force gfp_t)___GFP_DIRECT_RECLAIM) /* Caller can reclaim */
#define __GFP_KSWAPD_RECLAIM    ((__force gfp_t)___GFP_KSWAPD_RECLAIM) /* kswapd can wake */
#define __GFP_RECLAIM ((__force gfp_t)(___GFP_DIRECT_RECLAIM|___GFP_KSWAPD_RECLAIM))
#define __GFP_RETRY_MAYFAIL    ((__force gfp_t)___GFP_RETRY_MAYFAIL)
#define __GFP_NOFAIL    ((__force gfp_t)___GFP_NOFAIL)
#define __GFP_NORETRY    ((__force gfp_t)___GFP_NORETRY)


#define __GFP_WAIT      (0x10u)  /* Can wait and reschedule? */
//#define __GFP_HIGH      (0x20u)  /* Should access emergency pools? */
//#define __GFP_IO        (0x40u)  /* Can start physical IO? */
//#define __GFP_FS        (0x80u)  /* Can call down to low-level FS? */
#define __GFP_COLD      (0x100u) /* Cache-cold page required */
#define __GFP_NOWARN    (0x200u) /* Suppress page allocation failure warning */
#define __GFP_REPEAT    (0x400u) /* See above */
//#define __GFP_NOFAIL    (0x800u) /* See above */
//#define __GFP_NORETRY   (0x1000u)/* See above */
#define __GFP_COMP      (0x4000u)/* Add compound page metadata */
#define __GFP_ZERO      (0x8000u)/* Return zeroed page on success */
//#define __GFP_NOMEMALLOC (0x10000u) /* Don't use emergency reserves */
#define __GFP_HARDWALL   (0x20000u) /* Enforce hardwall cpuset memory allocs */
#define __GFP_THISNODE  (0x40000u)/* No fallback, no policies */
#define __GFP_RECLAIMABLE (0x80000u) /* Page is reclaimable */

//#define GFP_ATOMIC    (__GFP_HIGH|__GFP_ATOMIC|__GFP_KSWAPD_RECLAIM)
//#define GFP_KERNEL    (__GFP_RECLAIM | __GFP_IO | __GFP_FS)
#define GFP_KERNEL_ACCOUNT (GFP_KERNEL | __GFP_ACCOUNT)
#define GFP_NOWAIT    (__GFP_KSWAPD_RECLAIM)
#define GFP_NOIO    (__GFP_RECLAIM)
#define GFP_NOFS    (__GFP_RECLAIM | __GFP_IO)
#define GFP_USER    (__GFP_RECLAIM | __GFP_IO | __GFP_FS | __GFP_HARDWALL)
#define GFP_DMA        __GFP_DMA
#define GFP_DMA32    __GFP_DMA32

#endif /* gfp_h */
