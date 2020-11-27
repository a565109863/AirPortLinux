//
//  time.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef time_h
#define time_h


/* Nanosecond scalar representation for kernel time values */
typedef s64    ktime_t;

typedef __s64 time64_t;
typedef __u64 timeu64_t;

static u64 ktime_to_ns() {
    uint64_t m,f;
    clock_get_uptime(&m);
    absolutetime_to_nanoseconds(m,&f);
    return ((f * HZ) / 1000000000);
}

static inline u64 ktime_get_boottime_ns(void)
{
    return ktime_to_ns();
}

static time64_t ktime_get_seconds(void)
{
    uint64_t m,f;
    clock_get_uptime(&m);
    absolutetime_to_nanoseconds(m,&f);
    return ((f * HZ) / 1000000000);
}
EXPORT_SYMBOL_GPL(ktime_get_seconds);

#endif /* time_h */
