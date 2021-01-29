//
//  workqueue.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef workqueue_h
#define workqueue_h

#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/mutex.h>
#include <linux/task.h>


/*
 * The first word is the work queue pointer and the flags rolled into
 * one
 */
#define work_data_bits(work) ((unsigned long *)(&(work)->data))

enum {
    WORK_STRUCT_PENDING_BIT    = 0,    /* work item is pending execution */
    WORK_STRUCT_DELAYED_BIT    = 1,    /* work item is delayed */
    WORK_STRUCT_PWQ_BIT    = 2,    /* data points to pwq */
    WORK_STRUCT_LINKED_BIT    = 3,    /* next work is linked to this one */
#ifdef CONFIG_DEBUG_OBJECTS_WORK
    WORK_STRUCT_STATIC_BIT    = 4,    /* static initializer (debugobjects) */
    WORK_STRUCT_COLOR_SHIFT    = 5,    /* color for workqueue flushing */
#else
    WORK_STRUCT_COLOR_SHIFT    = 4,    /* color for workqueue flushing */
#endif
    
    WORK_STRUCT_COLOR_BITS    = 4,
    
    WORK_STRUCT_PENDING    = 1 << WORK_STRUCT_PENDING_BIT,
    WORK_STRUCT_DELAYED    = 1 << WORK_STRUCT_DELAYED_BIT,
    WORK_STRUCT_PWQ        = 1 << WORK_STRUCT_PWQ_BIT,
    WORK_STRUCT_LINKED    = 1 << WORK_STRUCT_LINKED_BIT,
#ifdef CONFIG_DEBUG_OBJECTS_WORK
    WORK_STRUCT_STATIC    = 1 << WORK_STRUCT_STATIC_BIT,
#else
    WORK_STRUCT_STATIC    = 0,
#endif
    
    /*
     * The last color is no color used for works which don't
     * participate in workqueue flushing.
     */
    WORK_NR_COLORS        = (1 << WORK_STRUCT_COLOR_BITS) - 1,
    WORK_NO_COLOR        = WORK_NR_COLORS,
    
    /* not bound to any CPU, prefer the local CPU */
    WORK_CPU_UNBOUND    = NR_CPUS,
    
    /*
     * Reserve 7 bits off of pwq pointer w/ debugobjects turned off.
     * This makes pwqs aligned to 256 bytes and allows 15 workqueue
     * flush colors.
     */
    WORK_STRUCT_FLAG_BITS    = WORK_STRUCT_COLOR_SHIFT +
    WORK_STRUCT_COLOR_BITS,
    
    /* data contains off-queue information when !WORK_STRUCT_PWQ */
    WORK_OFFQ_FLAG_BASE    = WORK_STRUCT_COLOR_SHIFT,
    
    __WORK_OFFQ_CANCELING    = WORK_OFFQ_FLAG_BASE,
    WORK_OFFQ_CANCELING    = (1 << __WORK_OFFQ_CANCELING),
    
    /*
     * When a work item is off queue, its high bits point to the last
     * pool it was on.  Cap at 31 bits and use the highest number to
     * indicate that no pool is associated.
     */
    WORK_OFFQ_FLAG_BITS    = 1,
    WORK_OFFQ_POOL_SHIFT    = WORK_OFFQ_FLAG_BASE + WORK_OFFQ_FLAG_BITS,
    WORK_OFFQ_LEFT        = BITS_PER_LONG - WORK_OFFQ_POOL_SHIFT,
    WORK_OFFQ_POOL_BITS    = WORK_OFFQ_LEFT <= 31 ? WORK_OFFQ_LEFT : 31,
    WORK_OFFQ_POOL_NONE    = (1LU << WORK_OFFQ_POOL_BITS) - 1,
    
    /* convenience constants */
    WORK_STRUCT_FLAG_MASK    = (1UL << WORK_STRUCT_FLAG_BITS) - 1,
    WORK_STRUCT_WQ_DATA_MASK = ~WORK_STRUCT_FLAG_MASK,
    WORK_STRUCT_NO_POOL    = (unsigned long)WORK_OFFQ_POOL_NONE << WORK_OFFQ_POOL_SHIFT,
    
    /* bit mask for work_busy() return values */
    WORK_BUSY_PENDING    = 1 << 0,
    WORK_BUSY_RUNNING    = 1 << 1,
    
    /* maximum string length for set_worker_desc() */
    WORKER_DESC_LEN        = 24,
};

/*
 * Workqueue flags and constants.  For details, please refer to
 * Documentation/core-api/workqueue.rst.
 */
enum {
    WQ_UNBOUND        = 1 << 1, /* not bound to any cpu */
    WQ_FREEZABLE        = 1 << 2, /* freeze during suspend */
    WQ_MEM_RECLAIM        = 1 << 3, /* may be used for memory reclaim */
    WQ_HIGHPRI        = 1 << 4, /* high priority */
    WQ_CPU_INTENSIVE    = 1 << 5, /* cpu intensive workqueue */
    WQ_SYSFS        = 1 << 6, /* visible in sysfs, see wq_sysfs_register() */

    /*
     * Per-cpu workqueues are generally preferred because they tend to
     * show better performance thanks to cache locality.  Per-cpu
     * workqueues exclude the scheduler from choosing the CPU to
     * execute the worker threads, which has an unfortunate side effect
     * of increasing power consumption.
     *
     * The scheduler considers a CPU idle if it doesn't have any task
     * to execute and tries to keep idle cores idle to conserve power;
     * however, for example, a per-cpu work item scheduled from an
     * interrupt handler on an idle CPU will force the scheduler to
     * excute the work item on that CPU breaking the idleness, which in
     * turn may lead to more scheduling choices which are sub-optimal
     * in terms of power consumption.
     *
     * Workqueues marked with WQ_POWER_EFFICIENT are per-cpu by default
     * but become unbound if workqueue.power_efficient kernel param is
     * specified.  Per-cpu workqueues which are identified to
     * contribute significantly to power-consumption are identified and
     * marked with this flag and enabling the power_efficient mode
     * leads to noticeable power saving at the cost of small
     * performance disadvantage.
     *
     * http://thread.gmane.org/gmane.linux.kernel/1480396
     */
    WQ_POWER_EFFICIENT    = 1 << 7,

    __WQ_DRAINING        = 1 << 16, /* internal: workqueue is draining */
    __WQ_ORDERED        = 1 << 17, /* internal: workqueue is ordered */
    __WQ_LEGACY        = 1 << 18, /* internal: create*_workqueue() */
    __WQ_ORDERED_EXPLICIT    = 1 << 19, /* internal: alloc_ordered_workqueue() */

    WQ_MAX_ACTIVE        = 512,      /* I like 512, better ideas? */
    WQ_MAX_UNBOUND_PER_CPU    = 4,      /* 4 * #cpus for unbound wq */
    WQ_DFL_ACTIVE        = WQ_MAX_ACTIVE / 2,
};

struct work_struct;
typedef void (*work_func_t)(struct work_struct *work);
void delayed_work_timer_fn(struct timer_list *t);

struct work_struct {
    atomic_long_t data;
    struct list_head entry;
    work_func_t func;
    char *func_name;
#ifdef CONFIG_LOCKDEP
    struct lockdep_map lockdep_map;
#endif
};


//struct timer_list {
//    struct hlist_node    entry;
//    unsigned long        expires;
//    void            (*function)(unsigned long);
//    unsigned long        data;
//    u32            flags;
//    int            slack;
//};

enum {
    /*
     * worker_pool flags
     *
     * A bound pool is either associated or disassociated with its CPU.
     * While associated (!DISASSOCIATED), all workers are bound to the
     * CPU and none has %WORKER_UNBOUND set and concurrency management
     * is in effect.
     *
     * While DISASSOCIATED, the cpu may be offline and all workers have
     * %WORKER_UNBOUND set and concurrency management disabled, and may
     * be executing on any CPU.  The pool behaves as an unbound one.
     *
     * Note that DISASSOCIATED should be flipped only while holding
     * wq_pool_attach_mutex to avoid changing binding state while
     * worker_attach_to_pool() is in progress.
     */
    POOL_MANAGER_ACTIVE    = 1 << 0,    /* being managed */
    POOL_DISASSOCIATED    = 1 << 2,    /* cpu can't serve workers */
    
    /* worker flags */
    WORKER_DIE        = 1 << 1,    /* die die die */
    WORKER_IDLE        = 1 << 2,    /* is idle */
    WORKER_PREP        = 1 << 3,    /* preparing to run works */
    WORKER_CPU_INTENSIVE    = 1 << 6,    /* cpu intensive */
    WORKER_UNBOUND        = 1 << 7,    /* worker is unbound */
    WORKER_REBOUND        = 1 << 8,    /* worker was rebound */
    
    WORKER_NOT_RUNNING    = WORKER_PREP | WORKER_CPU_INTENSIVE |
    WORKER_UNBOUND | WORKER_REBOUND,
    
    NR_STD_WORKER_POOLS    = 2,        /* # standard pools per cpu */
    
    UNBOUND_POOL_HASH_ORDER    = 6,        /* hashed by pool->attrs */
    BUSY_WORKER_HASH_ORDER    = 6,        /* 64 pointers */
    
    MAX_IDLE_WORKERS_RATIO    = 4,        /* 1/4 of busy can be idle */
    IDLE_WORKER_TIMEOUT    = 300 * HZ,    /* keep idle ones for 5 mins */
    
    MAYDAY_INITIAL_TIMEOUT  = HZ / 100 >= 2 ? HZ / 100 : 2,
    /* call for help after 10ms
     (min two ticks) */
    MAYDAY_INTERVAL        = HZ / 10,    /* and then every 100ms */
    CREATE_COOLDOWN        = HZ,        /* time to breath after fail */
    
    /*
     * Rescue workers are used only on emergencies and shared by
     * all cpus.  Give MIN_NICE.
     */
//    RESCUER_NICE_LEVEL    = MIN_NICE,
//    HIGHPRI_NICE_LEVEL    = MIN_NICE,
    
    WQ_NAME_LEN        = 24,
};

/*
 * The externally visible workqueue.  It relays the issued work items to
 * the appropriate worker_pool through its pool_workqueues.
 */
struct workqueue_struct {
    struct list_head    pwqs;        /* WR: all pwqs of this wq */
    struct list_head    list;        /* PR: list of all workqueues */
    
    struct mutex        mutex;        /* protects this wq */
    int            work_color;    /* WQ: current work color */
    int            flush_color;    /* WQ: current flush color */
    atomic_t        nr_pwqs_to_flush; /* flush in progress */
//    struct wq_flusher    *first_flusher;    /* WQ: first flusher */
    struct list_head    flusher_queue;    /* WQ: flush waiters */
    struct list_head    flusher_overflow; /* WQ: flush overflow list */
    
    struct list_head    maydays;    /* MD: pwqs requesting rescue */
//    struct worker        *rescuer;    /* MD: rescue worker */
    thread_t            thread;
    
    int            nr_drainers;    /* WQ: drain in progress */
    int            saved_max_active; /* WQ: saved pwq max_active */
    
//    struct workqueue_attrs    *unbound_attrs;    /* PW: only for unbound wqs */
//    struct pool_workqueue    *dfl_pwq;    /* PW: only for unbound wqs */
    
#ifdef CONFIG_SYSFS
    struct wq_device    *wq_dev;    /* I: for sysfs interface */
#endif
#ifdef CONFIG_LOCKDEP
    char            *lock_name;
    struct lock_class_key    key;
    struct lockdep_map    lockdep_map;
#endif
    char            name[WQ_NAME_LEN]; /* I: workqueue name */
    
    /*
     * Destruction of workqueue_struct is RCU protected to allow walking
     * the workqueues list without grabbing wq_pool_mutex.
     * This is used to dump all workqueues from sysrq.
     */
    struct rcu_head        rcu;
    
    /* hot fields used during command issue, aligned to cacheline */
    unsigned int        flags ____cacheline_aligned; /* WQ: WQ_* flags */
    struct pool_workqueue __percpu *cpu_pwqs; /* I: per-cpu pwqs */
    struct pool_workqueue __rcu *numa_pwq_tbl[]; /* PWR: unbound pwqs indexed by node */
};


extern struct workqueue_struct *system_wq;
//extern struct workqueue_struct *system_highpri_wq;
//extern struct workqueue_struct *system_long_wq;
//extern struct workqueue_struct *system_unbound_wq;
extern struct workqueue_struct *system_freezable_wq;
extern struct workqueue_struct *system_power_efficient_wq;
//extern struct workqueue_struct *system_freezable_power_efficient_wq;


static void queue_work_run(void* tqarg, wait_result_t waitResult);

struct workqueue_struct *alloc_workqueue(const char *fmt,
                                         unsigned int flags,
                                         int max_active, ...);

/*
 * Test whether @work is being queued from another work executing on the
 * same workqueue.
 */
static bool is_chained_work(struct workqueue_struct *wq)
{
    return true;
}

void __queue_work(int cpu, struct workqueue_struct *wq,
                  struct work_struct *work);
bool queue_work_on(int cpu, struct workqueue_struct *wq,
                   struct work_struct *work);
static inline bool queue_work(struct workqueue_struct *wq,
                  struct work_struct *work)
{
    kprintf("-----%s: line = %d, wq->name = %s, work->func_name = %s", __FUNCTION__, __LINE__, wq->name, work->func_name);
    return queue_work_on(WORK_CPU_UNBOUND, wq, work);
}






struct delayed_work {
    struct work_struct work;
    struct timer_list timer;

    /* target workqueue and CPU ->timer uses to queue ->work */
    struct workqueue_struct *wq;
    int cpu;
};

extern bool flush_work(struct work_struct *work);
extern bool cancel_work_sync(struct work_struct *work);

extern bool flush_delayed_work(struct delayed_work *dwork);
extern bool cancel_delayed_work(struct delayed_work *dwork);
extern bool cancel_delayed_work_sync(struct delayed_work *dwork);

static void flush_workqueue(struct workqueue_struct *wq)
{
    
}

static void destroy_workqueue(struct workqueue_struct *wq)
{
    
}


static void init_waitqueue_head(wait_queue_head_t *mod_tx_wq){
    spin_lock_init(&mod_tx_wq->lock);
//    lockdep_set_class_and_name(&mod_tx_wq->lock, key, name);
    INIT_LIST_HEAD(&mod_tx_wq->head);
}

static inline void __init_work(struct work_struct *work, int onstack) { }


#define WORK_DATA_INIT()    ATOMIC_LONG_INIT((unsigned long)WORK_STRUCT_NO_POOL)

#define __INIT_WORK(_work, _func, _onstack)                \
    do {                                \
        __init_work((_work), _onstack);                \
        (_work)->data = (atomic_long_t) WORK_DATA_INIT();    \
        INIT_LIST_HEAD(&(_work)->entry);            \
        (_work)->func = (_func);                \
        (_work)->func_name = #_func;                \
    } while (0)

#define INIT_WORK(_work, _func)                        \
    __INIT_WORK((_work), (_func), 0)



#define __INIT_DELAYED_WORK(_work, _func, _tflags)            \
    do {                                \
        INIT_WORK(&(_work)->work, (_func));            \
        __init_timer(&(_work)->timer,                \
                 delayed_work_timer_fn,            \
                 (_tflags) | TIMER_IRQSAFE);        \
    } while (0)

#define INIT_DELAYED_WORK(_work, _func)                    \
    __INIT_DELAYED_WORK(_work, _func, 0)

bool queue_delayed_work_on(int cpu, struct workqueue_struct *wq,
            struct delayed_work *work, unsigned long delay);

/**
 * queue_delayed_work - queue work on a workqueue after delay
 * @wq: workqueue to use
 * @dwork: delayable work to queue
 * @delay: number of jiffies to wait before queueing
 *
 * Equivalent to queue_delayed_work_on() but tries to use the local CPU.
 */
static inline bool queue_delayed_work(struct workqueue_struct *wq,
                      struct delayed_work *dwork,
                      unsigned long delay)
{
    return queue_delayed_work_on(WORK_CPU_UNBOUND, wq, dwork, delay);
}

static inline bool schedule_delayed_work(struct delayed_work *dwork,
                                         unsigned long delay)
{
    return queue_delayed_work(system_wq, dwork, delay);
}

bool mod_delayed_work_on(int cpu, struct workqueue_struct *wq,
            struct delayed_work *dwork, unsigned long delay);

static inline bool mod_delayed_work(struct workqueue_struct *wq,
                    struct delayed_work *dwork,
                    unsigned long delay)
{
    return mod_delayed_work_on(WORK_CPU_UNBOUND, wq, dwork, delay);
}


static inline struct delayed_work *to_delayed_work(struct work_struct *work)
{
    return container_of(work, struct delayed_work, work);
}

/**
 * schedule_work_on - put work task on a specific cpu
 * @cpu: cpu to put the work task on
 * @work: job to be done
 *
 * This puts a job on a specific cpu
 */
static inline bool schedule_work_on(int cpu, struct work_struct *work)
{
    return queue_work_on(cpu, system_wq, work);
}

/**
 * schedule_work - put work task in global workqueue
 * @work: job to be done
 *
 * Returns %false if @work was already on the kernel-global workqueue and
 * %true otherwise.
 *
 * This puts a job in the kernel-global workqueue if it was not already
 * queued and leaves it in the same position on the kernel-global
 * workqueue otherwise.
 *
 * Shares the same memory-ordering properties of queue_work(), cf. the
 * DocBook header of queue_work().
 */
static inline bool schedule_work(struct work_struct *work)
{
    kprintf("--%s: line = %d irq work->func_name = %s", __FUNCTION__, __LINE__, work->func_name);
    return queue_work(system_wq, work);
}

#define alloc_ordered_workqueue(fmt, flags, args...)            \
    alloc_workqueue(fmt, WQ_UNBOUND | __WQ_ORDERED |        \
            __WQ_ORDERED_EXPLICIT | (flags), 1, ##args)

#define __WORK_INITIALIZER(n, f) { \
.data = 0, \
.entry = { &(n).entry, &(n).entry }, \
.func = f, \
.func_name = #f                \
}

#define DECLARE_WORK(n, f) \
struct work_struct n = __WORK_INITIALIZER((n), (f))


#define __TIMER_LOCKDEP_MAP_INITIALIZER(_kn)

#define __TIMER_INITIALIZER(_function, _flags) {        \
.entry = { .next = TIMER_ENTRY_STATIC },    \
.function = (_function),            \
}

#define __DELAYED_WORK_INITIALIZER(n, f, tflags) {            \
.work = __WORK_INITIALIZER((n).work, (f)),            \
.timer = __TIMER_INITIALIZER(delayed_work_timer_fn,\
(tflags) | TIMER_IRQSAFE),        \
}

#define DECLARE_DELAYED_WORK(n, f)                    \
struct delayed_work n = __DELAYED_WORK_INITIALIZER(n, f, 0)


#endif /* workqueue_h */
