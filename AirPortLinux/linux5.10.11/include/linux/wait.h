//
//  wait.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef linux_wait_h
#define linux_wait_h

#include <linux/types.h>
#include <linux/list.h>


typedef struct wait_queue_entry wait_queue_entry_t;

typedef int (*wait_queue_func_t)(struct wait_queue_entry *wq_entry, unsigned mode, int flags, void *key);

struct wait_queue_entry {
    unsigned int        flags;
    void            *_private;
    wait_queue_func_t    func;
    struct list_head    entry;
};

struct wait_queue_head {
    spinlock_t          lock;
    void                *ident;
    struct list_head    head;
};
typedef struct wait_queue_head wait_queue_head_t;

int wait_event_timeout(wait_queue_head_t q, int i, u32 timeout);

int wakeup_sleep(wait_queue_head_t *q, bool one);

#define wake_up(q)      wakeup_sleep(q, true);
#define wake_up_all(q)  wakeup_sleep(q, false);



int default_wake_function(struct wait_queue_entry *wq_entry, unsigned mode, int flags, void *key);


/*
 * Macros for declaration and initialisaton of the datatypes
 */

#define __WAITQUEUE_INITIALIZER(name, tsk) {                    \
._private    = tsk,                            \
.func        = default_wake_function,                \
.entry        = { NULL, NULL } }

#define DECLARE_WAITQUEUE(name, tsk)                        \
struct wait_queue_entry name = __WAITQUEUE_INITIALIZER(name, tsk)

#define __WAIT_QUEUE_HEAD_INITIALIZER(name) {                    \
.lock        = __SPIN_LOCK_UNLOCKED(name.lock),            \
.head        = { &(name).head, &(name).head } }

#define DECLARE_WAIT_QUEUE_HEAD(name) \
struct wait_queue_head name = __WAIT_QUEUE_HEAD_INITIALIZER(name)


/* wait_queue_entry::flags */
#define WQ_FLAG_EXCLUSIVE    0x01
#define WQ_FLAG_WOKEN        0x02
#define WQ_FLAG_BOOKMARK    0x04

void add_wait_queue_exclusive(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);

static inline void __add_wait_queue_entry_tail(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
    list_add_tail(&wq_entry->entry, &wq_head->head);
}



/*
 * wake flags
 */
#define WF_SYNC            0x01        /* Waker goes to sleep after wakeup */
#define WF_FORK            0x02        /* Child wakeup after fork */
#define WF_MIGRATED        0x04        /* Internal use, task got migrated */
#define WF_ON_CPU        0x08        /* Wakee is on_cpu */



extern void add_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);
extern void add_wait_queue_exclusive(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);
extern void remove_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry);

static inline void __add_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
    list_add(&wq_entry->entry, &wq_head->head);
}

static inline void
__remove_wait_queue(struct wait_queue_head *wq_head, struct wait_queue_entry *wq_entry)
{
    list_del(&wq_entry->entry);
}

long wait_woken(struct wait_queue_entry *wq_entry, unsigned mode, long timeout);
int woken_wake_function(struct wait_queue_entry *wq_entry, unsigned mode, int sync, void *key);
int autoremove_wake_function(struct wait_queue_entry *wq_entry, unsigned mode, int sync, void *key);

extern void *current;

#define DEFINE_WAIT_FUNC(name, function)                    \
    struct wait_queue_entry name = {                    \
        ._private    = current,                    \
        .func        = function,                    \
        .entry        = LIST_HEAD_INIT((name).entry),            \
    }

#define DEFINE_WAIT(name) DEFINE_WAIT_FUNC(name, autoremove_wake_function)

#define init_wait(wait)                                \
    do {                                    \
        (wait)->_private = current;                    \
        (wait)->func = autoremove_wake_function;            \
        INIT_LIST_HEAD(&(wait)->entry);                    \
        (wait)->flags = 0;                        \
    } while (0)

#endif /* linux_wait_h */
