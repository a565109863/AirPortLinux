//
//  timer.h
//  AppleIwlWiFi
//
//  Created by User-PC on 2019/11/7.
//  Copyright © 2019 User-PC. All rights reserved.
//

#ifndef timer_h
#define timer_h

#include <linux/types.h>


#define TIMER_CPUMASK        0x0003FFFF
#define TIMER_MIGRATING        0x00040000
#define TIMER_BASEMASK        (TIMER_CPUMASK | TIMER_MIGRATING)
#define TIMER_DEFERRABLE    0x00080000
#define TIMER_PINNED        0x00100000
#define TIMER_IRQSAFE        0x00200000
#define TIMER_ARRAYSHIFT    22
#define TIMER_ARRAYMASK        0xFFC00000

//union ktime {
//    s64 tv64;
//    struct {
//        s32 nsec, sec;
//    } tv;
//};
//typedef union ktime ktime_t; /* Kill this */

//typedef UInt64 ktime_t;

struct timer_list {
    /*
     * All fields that change during normal runtime grouped to the
     * same cacheline
     */
    struct hlist_node entry;
    unsigned long expires;
    struct tvec_base *base;
    
    void (*function)(unsigned long);
    unsigned long data;
    
    int slack;
    
#ifdef CONFIG_TIMER_STATS
    int start_pid;
    void *start_site;
    char start_comm[16];
#endif
#ifdef CONFIG_LOCKDEP
    struct lockdep_map lockdep_map;
#endif
    
    int vv;
    int on;
};

#define typecheck(type,x) \
({    type __dummy; \
    typeof(x) __dummy2; \
    (void)(&__dummy == &__dummy2); \
    1; \
})

#define time_after(a,b)        \
    (typecheck(unsigned long, a) && \
     typecheck(unsigned long, b) && \
     ((long)((b) - (a)) < 0))
#define time_before(a,b)    time_after(b,a)

#define time_after_eq(a,b)    \
    (typecheck(unsigned long, a) && \
     typecheck(unsigned long, b) && \
     ((long)((a) - (b)) >= 0))
#define time_before_eq(a,b)    time_after_eq(b,a)

//#define time_after(a,b)    ((long)(b) - (long)(a) < 0)
//#define time_after_eq(a,b)    \
(typecheck(unsigned long, a) && \
typecheck(unsigned long, b) && \
((long)((a) - (b)) >= 0))


#define time_before(a,b)        time_after(b,a)
#define time_is_before_jiffies(a) time_after(jiffies, a)
#define time_is_after_jiffies(a) time_before(jiffies, a)

#define from_timer(var, callback_timer, timer_fieldname) \
container_of(callback_timer, typeof(*var), timer_fieldname)


//#undef MSEC_PER_SEC
//#undef USEC_PER_SEC
//#undef NSEC_PER_SEC
//#undef NSEC_PER_USEC

/* Parameters used to convert the timespec values: */
#define MSEC_PER_SEC    1000L
#define USEC_PER_MSEC    1000L
//#define NSEC_PER_USEC    1000L
//#define USEC_PER_SEC    1000000L
//#define NSEC_PER_SEC    1000000000L
#define FSEC_PER_SEC    1000000000000000LL


/**
 * usleep_range - Sleep for an approximate time
 * @min: Minimum time in usecs to sleep
 * @max: Maximum time in usecs to sleep
 *
 * In non-atomic context where the exact wakeup time is flexible, use
 * usleep_range() instead of udelay().  The sleep improves responsiveness
 * by avoiding the CPU-hogging busy-wait of udelay(), and the range reduces
 * power usage by allowing hrtimers to take advantage of an already-
 * scheduled interrupt instead of scheduling a new one just for this sleep.
 */
static
void __sched usleep_range(unsigned long min, unsigned long max)
{
    UInt32 r = random();
    udelay(min + abs(max - min - r));
//    ktime_t exp = ktime_add_us(ktime_get(), min);
//    u64 delta = (u64)(max - min) * NSEC_PER_USEC;
//
//    for (;;) {
//        __set_current_state(TASK_UNINTERRUPTIBLE);
//        /* Do not return before the requested sleep time has elapsed */
//        if (!schedule_hrtimeout_range(&exp, delta, HRTIMER_MODE_ABS))
//            break;
//    }
}
EXPORT_SYMBOL(usleep_range);



int del_timer(struct timer_list *timer);

static inline void timer_setup(struct timer_list *timer,
                               void (*callback) (struct timer_list *),
                               unsigned int flags)
{
//#ifdef __setup_timer
//    __setup_timer(timer, (TIMER_FUNC_TYPE) callback,
//                  (TIMER_DATA_TYPE) timer, flags);
//#else
//    if (flags & TIMER_DEFERRABLE)
//        setup_deferrable_timer(timer, (TIMER_FUNC_TYPE) callback,
//                               (TIMER_DATA_TYPE) timer);
//    else
//        setup_timer(timer, (TIMER_FUNC_TYPE) callback,
//                    (TIMER_DATA_TYPE) timer);
//#endif
}


static inline int timer_pending(const struct timer_list * timer)
{
    return timer->entry.pprev != NULL;
}


static int del_timer_sync(struct timer_list *timer)
{
    // todo
    return 0;
}
static int add_timer(struct timer_list *timer)
{
    // todo
    return 0;
}
static void mod_timer(struct timer_list *timer, int length)
{
    // todo
}



#endif /* timer_h */
