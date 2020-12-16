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

struct work_struct {
    atomic_long_t data;
    struct list_head entry;
    work_func_t func;
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


struct delayed_work {
    struct work_struct work;
    struct timer_list timer;

    /* target workqueue and CPU ->timer uses to queue ->work */
    struct workqueue_struct *wq;
    int cpu;
};


static bool __cancel_work_timer(struct work_struct *work, bool is_dwork)
{
    return true;
}

/**
 * cancel_delayed_work_sync - cancel a delayed work and wait for it to finish
 * @dwork: the delayed work cancel
 *
 * This is cancel_work_sync() for delayed works.
 *
 * Return:
 * %true if @dwork was pending, %false otherwise.
 */
static bool cancel_delayed_work_sync(struct delayed_work *dwork)
{
    return __cancel_work_timer(&dwork->work, true);
}
EXPORT_SYMBOL(cancel_delayed_work_sync);


static int cancel_delayed_work(struct delayed_work *work) {
//    struct work_struct tmp = work->work;
//    struct work_struct *tmp2 = &tmp;
//    queue_td(tmp2->number,NULL);
    return 0;
}


/**
 * cancel_work_sync - cancel a work and wait for it to finish
 * @work: the work to cancel
 *
 * Cancel @work and wait for its execution to finish.  This function
 * can be used even if the work re-queues itself or migrates to
 * another workqueue.  On return from this function, @work is
 * guaranteed to be not pending or executing on any CPU.
 *
 * cancel_work_sync(&delayed_work->work) must not be used for
 * delayed_work's.  Use cancel_delayed_work_sync() instead.
 *
 * The caller must ensure that the workqueue on which @work was last
 * queued can't be destroyed before this function returns.
 *
 * Return:
 * %true if @work was pending, %false otherwise.
 */
static bool cancel_work_sync(struct work_struct *work)
{
    return __cancel_work_timer(work, false);
}
EXPORT_SYMBOL_GPL(cancel_work_sync);


static inline bool queue_work(struct workqueue_struct *wq,
                  struct work_struct *work)
{
//    return queue_work_on(WORK_CPU_UNBOUND, wq, work);
    return true;
}

extern bool cancel_work_sync(struct work_struct *work);

static bool flush_work(struct work_struct *work)
{
    return true;
}

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

static struct workqueue_struct * alloc_workqueue(char *wq_name, int how, int type)
{
    struct workqueue_struct *wq;
    return wq;
}

static bool flush_delayed_work(struct delayed_work *dwork)
{
//    local_irq_disable();
//    if (del_timer_sync(&dwork->timer))
//        __queue_work(dwork->cpu, dwork->wq, &dwork->work);
//    local_irq_enable();
    return flush_work(&dwork->work);
}


static inline void __init_work(struct work_struct *work, int onstack) { }

#define __INIT_WORK(_work, _func, _onstack)                \
    do {                                \
        __init_work((_work), _onstack);                \
        INIT_LIST_HEAD(&(_work)->entry);            \
        (_work)->func = (_func);                \
    } while (0)

#define INIT_WORK(_work, _func)                        \
    __INIT_WORK((_work), (_func), 0)


#define INIT_DELAYED_WORK(_work, _func)             \
do {                            \
INIT_WORK(&(_work)->work, (_func));     \
} while (0)


static int queue_delayed_work(struct workqueue_struct *wq, struct delayed_work *work, unsigned long delay) {
//    struct work_struct tmp = work->work;
//    struct work_struct *tmp2 = &tmp;
//    delay=jiffies_to_msecs(delay);
//    queue_te(tmp2->number,(thread_call_func_t)tmp2->func,work,(UInt32)delay,true);
    return 0;
}

static bool schedule_delayed_work(struct delayed_work *dwork,
                                         unsigned long delay)
{
    return queue_delayed_work(NULL, dwork, delay);
}


static struct workqueue_struct *system_wq = NULL;


static inline bool mod_delayed_work(struct workqueue_struct *wq,
                    struct delayed_work *dwork,
                    unsigned long delay)
{
//    return mod_delayed_work_on(WORK_CPU_UNBOUND, wq, dwork, delay);
    return true;
}


static inline struct delayed_work *to_delayed_work(struct work_struct *work)
{
    return container_of(work, struct delayed_work, work);
}

static
void schedule_work(struct work_struct *work)
{
 
}


#define alloc_ordered_workqueue(fmt, flags, args...)            \
    alloc_workqueue(fmt, WQ_UNBOUND | __WQ_ORDERED |        \
            __WQ_ORDERED_EXPLICIT | (flags), 1, ##args)


#define __WORK_INITIALIZER(n, f) { \
.data = 0, \
.entry = { &(n).entry, &(n).entry }, \
.func = f \
}

#define DECLARE_WORK(n, f) \
struct work_struct n = __WORK_INITIALIZER((n), (f))

#endif /* workqueue_h */
