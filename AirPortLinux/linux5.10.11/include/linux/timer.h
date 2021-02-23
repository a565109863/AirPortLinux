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
#include <linux/bug.h>
#include <linux/debugfs.h>


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


class IOTimeout : public OSObject {
    OSDeclareDefaultStructors(IOTimeout)
    
public:
    static void timeout_run(OSObject* obj, IOTimerEventSource* timer);
public:
    IOTimerEventSource* timer;
    struct timer_list *tl;
};



struct timer_list {
    /*
     * All fields that change during normal runtime grouped to the
     * same cacheline
     */
    struct hlist_node entry;
    unsigned long expires;
//    struct tvec_base *base;
    IOTimeout* vt;
    
    const char *func_name;
    void (*function)(struct timer_list *);
//    unsigned long data;
//
//    int slack;
//
//#ifdef CONFIG_TIMER_STATS
//    int start_pid;
//    void *start_site;
//    char start_comm[16];
//#endif
//#ifdef CONFIG_LOCKDEP
//    struct lockdep_map lockdep_map;
//#endif
//
//    int vv;
//    int on;
};


/**
 * timer_setup - prepare a timer for first use
 * @timer: the timer in question
 * @callback: the function to call when timer expires
 * @flags: any TIMER_* flags
 *
 * Regular timer initialization should use either DEFINE_TIMER() above,
 * or timer_setup(). For timers on the stack, timer_setup_on_stack() must
 * be used and must be balanced with a call to destroy_timer_on_stack().
 */
#define timer_setup(timer, callback, flags)            \
    __init_timer((timer), (callback), (flags))

#define __init_timer(_timer, _fn, _flags)                \
init_timer_key((_timer), (_fn), (_flags), #_timer)

void init_timer_key(struct timer_list *timer,
                    void (*func)(struct timer_list *), unsigned int flags,
                    const char *name);
void add_timer_on(struct timer_list *timer, int cpu);
void add_timer(struct timer_list *timer);
int del_timer(struct timer_list * timer);
int mod_timer(struct timer_list *timer, unsigned long expires);
//int mod_timer_pending(struct timer_list *timer, unsigned long expires);
//int timer_reduce(struct timer_list *timer, unsigned long expires);
int timer_pending(const struct timer_list * timer);
#define del_timer_sync(t)        del_timer(t)

void cancel_timer(struct timer_list *t);


#define from_timer(var, callback_timer, timer_fieldname) \
container_of(callback_timer, typeof(*var), timer_fieldname)


//#undef MSEC_PER_SEC
//#undef USEC_PER_SEC
//#undef NSEC_PER_SEC
//#undef NSEC_PER_USEC

/* Parameters used to convert the timespec values: */
//#define MSEC_PER_SEC    1000L
//#define USEC_PER_MSEC    1000L
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
    
    DebugLog("--%s: line = %d r = %lu", __FUNCTION__, __LINE__, (min + r % (max - min)));
    udelay((min + r % (max - min)));
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

#define raw_smp_processor_id() 0

unsigned long round_jiffies_common(unsigned long j, int cpu,
        bool force_up);

/**
 * round_jiffies_up - function to round jiffies up to a full second
 * @j: the time in (absolute) jiffies that should be rounded
 *
 * This is the same as round_jiffies() except that it will never
 * round down.  This is useful for timeouts for which the exact time
 * of firing does not matter too much, as long as they don't fire too
 * early.
 */
static unsigned long round_jiffies_up(unsigned long j)
{
    return round_jiffies_common(j, raw_smp_processor_id(), true);
}
EXPORT_SYMBOL_GPL(round_jiffies_up);


#endif /* timer_h */
