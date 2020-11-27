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
#define __GFP_HIGHMEM 2
//#define __GFP_HIGH 3

#include <linux/types.h>

typedef unsigned int __bitwise gfp_t;
typedef unsigned int __bitwise slab_flags_t;
typedef unsigned int __bitwise fmode_t;

#define __GFP_BITS_SHIFT 26
#define __GFP_BITS_MASK ((gfp_t)((1 << __GFP_BITS_SHIFT) - 1))

//#define __GFP_HIGH        0x20u
//#define __GFP_IO        0x40u
//#define __GFP_FS        0x80u
//#define __GFP_NOWARN        0x200u
//#define __GFP_ZERO        0x8000u
#define __GFP_ATOMIC        0x80000u
#define __GFP_ACCOUNT        0x100000u
#define __GFP_DIRECT_RECLAIM    0x400000u
#define __GFP_KSWAPD_RECLAIM    0x2000000u

#define __GFP_RECLAIM    (__GFP_DIRECT_RECLAIM|__GFP_KSWAPD_RECLAIM)

#define GFP_ZONEMASK    0x0fu
//#define GFP_ATOMIC    (__GFP_HIGH|__GFP_ATOMIC|__GFP_KSWAPD_RECLAIM)
//#define GFP_KERNEL    (__GFP_RECLAIM | __GFP_IO | __GFP_FS)
//#define GFP_NOWAIT    (__GFP_KSWAPD_RECLAIM)


#define __GFP_WAIT      (0x10u)  /* Can wait and reschedule? */
#define __GFP_HIGH      (0x20u)  /* Should access emergency pools? */
#define __GFP_IO        (0x40u)  /* Can start physical IO? */
#define __GFP_FS        (0x80u)  /* Can call down to low-level FS? */
#define __GFP_COLD      (0x100u) /* Cache-cold page required */
#define __GFP_NOWARN    (0x200u) /* Suppress page allocation failure warning */
#define __GFP_REPEAT    (0x400u) /* See above */
#define __GFP_NOFAIL    (0x800u) /* See above */
#define __GFP_NORETRY   (0x1000u)/* See above */
#define __GFP_COMP      (0x4000u)/* Add compound page metadata */
#define __GFP_ZERO      (0x8000u)/* Return zeroed page on success */
#define __GFP_NOMEMALLOC (0x10000u) /* Don't use emergency reserves */
#define __GFP_HARDWALL   (0x20000u) /* Enforce hardwall cpuset memory allocs */
#define __GFP_THISNODE  (0x40000u)/* No fallback, no policies */
#define __GFP_RECLAIMABLE (0x80000u) /* Page is reclaimable */

#endif /* gfp_h */
