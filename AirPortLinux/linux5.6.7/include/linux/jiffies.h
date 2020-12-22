//
//  jiffies.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef jiffies_h
#define jiffies_h

#include <linux/types.h>

static unsigned long volatile jiffies = ULONG_MAX - (10 * 60 * HZ);

//#define jiffies        \
//({        \
//    uint64_t m,f;        \
//    clock_get_uptime(&m);        \
//    absolutetime_to_nanoseconds(m,&f);        \
//    ((f * HZ) / 1000000000);        \
//})

#undef MSEC_PER_SEC
#undef USEC_PER_SEC
#undef NSEC_PER_SEC
#undef NSEC_PER_USEC

#define MSEC_PER_SEC        1000L
#define USEC_PER_SEC        1000000L
#define NSEC_PER_SEC        1000000000L
#define NSEC_PER_USEC        1000L

static inline unsigned int jiffies_to_msecs(const unsigned long j)
{
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
    return (MSEC_PER_SEC / HZ) * j;
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
    return (j + (HZ / MSEC_PER_SEC) - 1)/(HZ / MSEC_PER_SEC);
#else
    return (j * MSEC_PER_SEC) / HZ;
#endif
}

static inline unsigned long msecs_to_jiffies(const unsigned int m)
{
         //if (m > jiffies_to_msecs(MAX_JIFFY_OFFSET)) return MAX_JIFFY_OFFSET;
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
         return (m + (MSEC_PER_SEC / HZ) - 1) / (MSEC_PER_SEC / HZ);
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
         return m * (HZ / MSEC_PER_SEC);
#else
         return (m * HZ + MSEC_PER_SEC - 1) / MSEC_PER_SEC;
#endif
}

static inline unsigned long usecs_to_jiffies(const unsigned int u)
 {
         //if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
           //      return MAX_JIFFY_OFFSET;
 #if HZ <= USEC_PER_SEC && !(USEC_PER_SEC % HZ)
         return (u + (USEC_PER_SEC / HZ) - 1) / (USEC_PER_SEC / HZ);
 #elif HZ > USEC_PER_SEC && !(HZ % USEC_PER_SEC)
         return u * (HZ / USEC_PER_SEC);
 #else
         return (u * HZ + USEC_PER_SEC - 1) / USEC_PER_SEC;
 #endif
 }


#define round_jiffies_relative(x) 1

#define round_jiffies usecs_to_jiffies




#endif /* jiffies_h */
