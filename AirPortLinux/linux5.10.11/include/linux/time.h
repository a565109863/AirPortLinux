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

/* Convert ktime_t to nanoseconds */
static inline s64 ktime_to_ns(const ktime_t kt)
{
    return kt;
}

static u64 ktime_add_us(uint64_t kt, ktime_t nsval) {
    return (kt / HZ) + nsval;
}

static u64 ktime_add_ns(uint64_t kt, ktime_t nsval) {
    return kt + nsval;
}

static inline u64 ktime_get_boottime_ns(void)
{
    uint64_t m,f;
    clock_get_uptime(&m);
    absolutetime_to_nanoseconds(m,&f);
    
    return (f * HZ);
}

static time64_t ktime_get_seconds(void)
{
    uint64_t m,f;
    clock_get_uptime(&m);
    absolutetime_to_nanoseconds(m,&f);
    return ((f * HZ) / 1000000000);
}
EXPORT_SYMBOL_GPL(ktime_get_seconds);

static time64_t ktime_get()
{
    uint64_t m,f;
    clock_get_uptime(&m);
    absolutetime_to_nanoseconds(m,&f);
    return (f * HZ);
}



/**
 * ktime_compare - Compares two ktime_t variables for less, greater or equal
 * @cmp1:    comparable1
 * @cmp2:    comparable2
 *
 * Return: ...
 *   cmp1  < cmp2: return <0
 *   cmp1 == cmp2: return 0
 *   cmp1  > cmp2: return >0
 */
static inline int ktime_compare(const ktime_t cmp1, const ktime_t cmp2)
{
    if (cmp1 < cmp2)
        return -1;
    if (cmp1 > cmp2)
        return 1;
    return 0;
}

/**
 * ktime_after - Compare if a ktime_t value is bigger than another one.
 * @cmp1:    comparable1
 * @cmp2:    comparable2
 *
 * Return: true if cmp1 happened after cmp2.
 */
static inline bool ktime_after(const ktime_t cmp1, const ktime_t cmp2)
{
    return ktime_compare(cmp1, cmp2) > 0;
}

#endif /* time_h */
