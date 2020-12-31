//
//  workqueue.c
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/7/12.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "workqueue.h"


struct workqueue_struct *system_wq = alloc_workqueue("system_wq", 1, 1);

void queue_work_run(void* tqarg, wait_result_t waitResult)
{
    struct workqueue_struct *wq = (struct workqueue_struct *)tqarg;
    
    struct work_struct *res, *tmp;
    
    for (;;) {
        mutex_lock(&wq->mutex);
        
        list_for_each_entry_safe(res, tmp, &wq->list, entry) {
            list_del(&res->entry);
            
            mutex_unlock(&wq->mutex);
            
            kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
            (*res->func)(res);
//            kfree(res);
        }
        
        int ret = IORecursiveLockSleep(wq->mutex.lock, &wq->work_color, THREAD_INTERRUPTIBLE);
        
    }
    
}

struct workqueue_struct * alloc_workqueue(char *wq_name, int how, int type)
{
    struct workqueue_struct *wq = (struct workqueue_struct *)malloc(sizeof(struct workqueue_struct));
    if (wq == NULL)
        return (NULL);
    
    memcpy(wq->name, wq_name, sizeof(wq->name));
    
    INIT_LIST_HEAD(&wq->list);

    mutex_init(&wq->mutex);

    kernel_thread_start(queue_work_run, wq, &wq->thread);
    thread_deallocate(wq->thread);

    return wq;
}

void __queue_work(int cpu, struct workqueue_struct *wq,
                         struct work_struct *work)
{
    mutex_lock(&wq->mutex);
    list_add_tail(&work->entry, &wq->list);
    IORecursiveLockWakeup(wq->mutex.lock, &wq->work_color, true);
    mutex_unlock(&wq->mutex);
    
}

/**
 * queue_work_on - queue work on specific cpu
 * @cpu: CPU number to execute work on
 * @wq: workqueue to use
 * @work: work to queue
 *
 * We queue the work to a specific CPU, the caller must ensure it
 * can't go away.
 *
 * Return: %false if @work was already on a queue, %true otherwise.
 */
bool queue_work_on(int cpu, struct workqueue_struct *wq,
                   struct work_struct *work)
{
    bool ret = false;
//    unsigned long flags;
    
//    local_irq_save(flags);
    
    if (!test_and_set_bit(WORK_STRUCT_PENDING_BIT, work_data_bits(work))) {
        __queue_work(cpu, wq, work);
        ret = true;
    }
    
//    local_irq_restore(flags);
    return ret;
}
EXPORT_SYMBOL(queue_work_on);






void delayed_work_timer_fn(struct timer_list *t)
{
    struct delayed_work *dwork = from_timer(dwork, t, timer);

    /* should have been called from irqsafe timer with irq already off */
    __queue_work(dwork->cpu, dwork->wq, &dwork->work);
}
EXPORT_SYMBOL(delayed_work_timer_fn);

static void __queue_delayed_work(int cpu, struct workqueue_struct *wq,
                struct delayed_work *dwork, unsigned long delay)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
    
    struct timer_list *timer = &dwork->timer;
    struct work_struct *work = &dwork->work;

    WARN_ON_ONCE(!wq);
    WARN_ON_ONCE(timer->function != delayed_work_timer_fn);
    WARN_ON_ONCE(timer_pending(timer));
    WARN_ON_ONCE(!list_empty(&work->entry));

    /*
     * If @delay is 0, queue @dwork->work immediately.  This is for
     * both optimization and correctness.  The earliest @timer can
     * expire is on the closest next tick and delayed_work users depend
     * on that there's no such delay when @delay is 0.
     */
    if (!delay) {
        __queue_work(cpu, wq, &dwork->work);
        return;
    }

    dwork->wq = wq;
    dwork->cpu = cpu;
    timer->expires = jiffies + delay;

    if (unlikely(cpu != WORK_CPU_UNBOUND))
        add_timer_on(timer, cpu);
    else
        add_timer(timer);
}

/**
 * queue_delayed_work_on - queue work on specific CPU after delay
 * @cpu: CPU number to execute work on
 * @wq: workqueue to use
 * @dwork: work to queue
 * @delay: number of jiffies to wait before queueing
 *
 * Return: %false if @work was already on a queue, %true otherwise.  If
 * @delay is zero and @dwork is idle, it will be scheduled for immediate
 * execution.
 */
bool queue_delayed_work_on(int cpu, struct workqueue_struct *wq,
               struct delayed_work *dwork, unsigned long delay)
{
    struct work_struct *work = &dwork->work;
    bool ret = false;
    unsigned long flags;

    /* read the comment in __queue_work() */
//    local_irq_save(flags);

    if (!test_and_set_bit(WORK_STRUCT_PENDING_BIT, work_data_bits(work))) {
        __queue_delayed_work(cpu, wq, dwork, delay);
        ret = true;
    }

//    local_irq_restore(flags);
    return ret;
}
EXPORT_SYMBOL(queue_delayed_work_on);


/**
 * try_to_grab_pending - steal work item from worklist and disable irq
 * @work: work item to steal
 * @is_dwork: @work is a delayed_work
 * @flags: place to store irq state
 *
 * Try to grab PENDING bit of @work.  This function can handle @work in any
 * stable state - idle, on timer or on worklist.
 *
 * Return:
 *  1        if @work was pending and we successfully stole PENDING
 *  0        if @work was idle and we claimed PENDING
 *  -EAGAIN    if PENDING couldn't be grabbed at the moment, safe to busy-retry
 *  -ENOENT    if someone else is canceling @work, this state may persist
 *        for arbitrarily long
 *
 * Note:
 * On >= 0 return, the caller owns @work's PENDING bit.  To avoid getting
 * interrupted while holding PENDING and @work off queue, irq must be
 * disabled on entry.  This, combined with delayed_work->timer being
 * irqsafe, ensures that we return -EAGAIN for finite short period of time.
 *
 * On successful return, >= 0, irq is disabled and the caller is
 * responsible for releasing it using local_irq_restore(*@flags).
 *
 * This function is safe to call from any context including IRQ handler.
 */
static int try_to_grab_pending(struct work_struct *work, bool is_dwork,
                   unsigned long *flags)
{
//    struct worker_pool *pool;
//    struct pool_workqueue *pwq;

//    local_irq_save(*flags);

    /* try to steal the timer if it exists */
    if (is_dwork) {
        struct delayed_work *dwork = to_delayed_work(work);

        /*
         * dwork->timer is irqsafe.  If del_timer() fails, it's
         * guaranteed that the timer is not queued anywhere and not
         * running on the local CPU.
         */
        if (likely(del_timer(&dwork->timer)))
            return 1;
    }

    /* try to claim PENDING the normal way */
    if (!test_and_set_bit(WORK_STRUCT_PENDING_BIT, work_data_bits(work)))
        return 0;
    
    return 1;
    
//
//    rcu_read_lock();
//    /*
//     * The queueing is in progress, or it is already queued. Try to
//     * steal it from ->worklist without clearing WORK_STRUCT_PENDING.
//     */
//    pool = get_work_pool(work);
//    if (!pool)
//        goto fail;
//
//    spin_lock(&pool->lock);
//    /*
//     * work->data is guaranteed to point to pwq only while the work
//     * item is queued on pwq->wq, and both updating work->data to point
//     * to pwq on queueing and to pool on dequeueing are done under
//     * pwq->pool->lock.  This in turn guarantees that, if work->data
//     * points to pwq which is associated with a locked pool, the work
//     * item is currently queued on that pool.
//     */
//    pwq = get_work_pwq(work);
//    if (pwq && pwq->pool == pool) {
//        debug_work_deactivate(work);
//
//        /*
//         * A delayed work item cannot be grabbed directly because
//         * it might have linked NO_COLOR work items which, if left
//         * on the delayed_list, will confuse pwq->nr_active
//         * management later on and cause stall.  Make sure the work
//         * item is activated before grabbing.
//         */
//        if (*work_data_bits(work) & WORK_STRUCT_DELAYED)
//            pwq_activate_delayed_work(work);
//
//        list_del_init(&work->entry);
//        pwq_dec_nr_in_flight(pwq, get_work_color(work));
//
//        /* work->data points to pwq iff queued, point to pool */
//        set_work_pool_and_keep_pending(work, pool->id);
//
//        spin_unlock(&pool->lock);
//        rcu_read_unlock();
//        return 1;
//    }
//    spin_unlock(&pool->lock);
//fail:
//    rcu_read_unlock();
//    local_irq_restore(*flags);
//    if (work_is_canceling(work))
//        return -ENOENT;
//    cpu_relax();
//    return -EAGAIN;
}

/**
 * mod_delayed_work_on - modify delay of or queue a delayed work on specific CPU
 * @cpu: CPU number to execute work on
 * @wq: workqueue to use
 * @dwork: work to queue
 * @delay: number of jiffies to wait before queueing
 *
 * If @dwork is idle, equivalent to queue_delayed_work_on(); otherwise,
 * modify @dwork's timer so that it expires after @delay.  If @delay is
 * zero, @work is guaranteed to be scheduled immediately regardless of its
 * current state.
 *
 * Return: %false if @dwork was idle and queued, %true if @dwork was
 * pending and its timer was modified.
 *
 * This function is safe to call from any context including IRQ handler.
 * See try_to_grab_pending() for details.
 */
bool mod_delayed_work_on(int cpu, struct workqueue_struct *wq,
             struct delayed_work *dwork, unsigned long delay)
{
    unsigned long flags;
    int ret;

    do {
        ret = try_to_grab_pending(&dwork->work, true, &flags);
    } while (unlikely(ret == -EAGAIN));

    if (likely(ret >= 0)) {
        __queue_delayed_work(cpu, wq, dwork, delay);
//        local_irq_restore(flags);
    }

    /* -ENOENT from try_to_grab_pending() becomes %true */
    return ret;
}
EXPORT_SYMBOL_GPL(mod_delayed_work_on);


static bool __cancel_work(struct work_struct *work, bool is_dwork)
{
//    unsigned long flags;
    int ret = true;

//    do {
//        ret = try_to_grab_pending(work, is_dwork, &flags);
//    } while (unlikely(ret == -EAGAIN));
//
//    if (unlikely(ret < 0))
//        return false;
//
//    set_work_pool_and_clear_pending(work, get_work_pool_id(work));
//    local_irq_restore(flags);
    return ret;
}

static bool __cancel_work_timer(struct work_struct *work, bool is_dwork)
{
//    static DECLARE_WAIT_QUEUE_HEAD(cancel_waitq);
//    unsigned long flags;
    int ret = true;
//
//    do {
//        ret = try_to_grab_pending(work, is_dwork, &flags);
//        /*
//         * If someone else is already canceling, wait for it to
//         * finish.  flush_work() doesn't work for PREEMPT_NONE
//         * because we may get scheduled between @work's completion
//         * and the other canceling task resuming and clearing
//         * CANCELING - flush_work() will return false immediately
//         * as @work is no longer busy, try_to_grab_pending() will
//         * return -ENOENT as @work is still being canceled and the
//         * other canceling task won't be able to clear CANCELING as
//         * we're hogging the CPU.
//         *
//         * Let's wait for completion using a waitqueue.  As this
//         * may lead to the thundering herd problem, use a custom
//         * wake function which matches @work along with exclusive
//         * wait and wakeup.
//         */
//        if (unlikely(ret == -ENOENT)) {
//            struct cwt_wait cwait;
//
//            init_wait(&cwait.wait);
//            cwait.wait.func = cwt_wakefn;
//            cwait.work = work;
//
//            prepare_to_wait_exclusive(&cancel_waitq, &cwait.wait,
//                          TASK_UNINTERRUPTIBLE);
//            if (work_is_canceling(work))
//                schedule();
//            finish_wait(&cancel_waitq, &cwait.wait);
//        }
//    } while (unlikely(ret < 0));
//
//    /* tell other tasks trying to grab @work to back off */
//    mark_work_canceling(work);
//    local_irq_restore(flags);
//
//    /*
//     * This allows canceling during early boot.  We know that @work
//     * isn't executing.
//     */
//    if (wq_online)
//        __flush_work(work, true);
//
//    clear_work_data(work);
//
//    /*
//     * Paired with prepare_to_wait() above so that either
//     * waitqueue_active() is visible here or !work_is_canceling() is
//     * visible there.
//     */
//    smp_mb();
//    if (waitqueue_active(&cancel_waitq))
//        __wake_up(&cancel_waitq, TASK_NORMAL, 1, work);

    return ret;
}

/**
 * cancel_delayed_work - cancel a delayed work
 * @dwork: delayed_work to cancel
 *
 * Kill off a pending delayed_work.
 *
 * Return: %true if @dwork was pending and canceled; %false if it wasn't
 * pending.
 *
 * Note:
 * The work callback function may still be running on return, unless
 * it returns %true and the work doesn't re-arm itself.  Explicitly flush or
 * use cancel_delayed_work_sync() to wait on it.
 *
 * This function is safe to call from any context including IRQ handler.
 */
bool cancel_delayed_work(struct delayed_work *dwork)
{
    return __cancel_work(&dwork->work, true);
}
EXPORT_SYMBOL(cancel_delayed_work);

/**
 * cancel_delayed_work_sync - cancel a delayed work and wait for it to finish
 * @dwork: the delayed work cancel
 *
 * This is cancel_work_sync() for delayed works.
 *
 * Return:
 * %true if @dwork was pending, %false otherwise.
 */
bool cancel_delayed_work_sync(struct delayed_work *dwork)
{
    return __cancel_work_timer(&dwork->work, true);
}
EXPORT_SYMBOL(cancel_delayed_work_sync);


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
bool cancel_work_sync(struct work_struct *work)
{
    return __cancel_work_timer(work, false);
}
EXPORT_SYMBOL_GPL(cancel_work_sync);

/**
 * flush_delayed_work - wait for a dwork to finish executing the last queueing
 * @dwork: the delayed work to flush
 *
 * Delayed timer is cancelled and the pending work is queued for
 * immediate execution.  Like flush_work(), this function only
 * considers the last queueing instance of @dwork.
 *
 * Return:
 * %true if flush_work() waited for the work to finish execution,
 * %false if it was already idle.
 */
bool flush_delayed_work(struct delayed_work *dwork)
{
//    local_irq_disable();
    if (del_timer_sync(&dwork->timer))
        __queue_work(dwork->cpu, dwork->wq, &dwork->work);
//    local_irq_enable();
    return flush_work(&dwork->work);
}
EXPORT_SYMBOL(flush_delayed_work);


static bool __flush_work(struct work_struct *work, bool from_cancel)
{
//    struct wq_barrier barr;
//
//    if (WARN_ON(!wq_online))
//        return false;
//
//    if (WARN_ON(!work->func))
//        return false;
//
//    if (!from_cancel) {
//        lock_map_acquire(&work->lockdep_map);
//        lock_map_release(&work->lockdep_map);
//    }
//
//    if (start_flush_work(work, &barr, from_cancel)) {
//        wait_for_completion(&barr.done);
//        destroy_work_on_stack(&barr.work);
//        return true;
//    } else {
//        return false;
//    }
    return true;
}

/**
 * flush_work - wait for a work to finish executing the last queueing instance
 * @work: the work to flush
 *
 * Wait until @work has finished execution.  @work is guaranteed to be idle
 * on return if it hasn't been requeued since flush started.
 *
 * Return:
 * %true if flush_work() waited for the work to finish execution,
 * %false if it was already idle.
 */
bool flush_work(struct work_struct *work)
{
    return __flush_work(work, false);
}
EXPORT_SYMBOL_GPL(flush_work);
