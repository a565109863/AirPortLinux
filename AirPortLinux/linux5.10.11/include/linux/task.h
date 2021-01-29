//
//  task.h
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/21.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef _linux_task_h
#define _linux_task_h

/* Tasklets --- multithreaded analogue of BHs.

   Main feature differing them of generic softirqs: tasklet
   is running only on one CPU simultaneously.

   Main feature differing them of BHs: different tasklets
   may be run simultaneously on different CPUs.

   Properties:
   * If tasklet_schedule() is called, then tasklet is guaranteed
     to be executed on some cpu at least once after this.
   * If the tasklet is already scheduled, but its execution is still not
     started, it will be executed only once.
   * If this tasklet is already running on another CPU (or schedule is called
     from tasklet itself), it is rescheduled for later.
   * Tasklet is strictly serialized wrt itself, but not
     wrt another tasklets. If client needs some intertask synchronization,
     he makes it with spinlocks.
 */

struct tasklet_struct
{
    TAILQ_ENTRY(tasklet_struct) t_entry;
    struct tasklet_struct *next;
    unsigned long state;
    atomic_t count;
    void (*func)(unsigned long);
    char *func_name;
    unsigned long data;
};

TAILQ_HEAD(tasklet_struct_list, tasklet_struct);

struct taskletq;
extern struct taskletq *const systq;
struct taskletq *taskletq_create(const char *, unsigned int, int, unsigned int);

#define DECLARE_TASKLET(name, func, data) \
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }

#define DECLARE_TASKLET_DISABLED(name, func, data) \
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(1), func, data }


enum
{
    TASKLET_STATE_SCHED,    /* Tasklet is scheduled for execution */
    TASKLET_STATE_RUN    /* Tasklet is running (SMP only) */
};

#define tasklet_init(t, func, data) __tasklet_init(t, func, data, #func)

static void __tasklet_init(struct tasklet_struct *t,
          void (*func)(unsigned long), unsigned long data, char *func_name)
{
    t->next = NULL;
    t->state = 0;
    atomic_set(&t->count, 0);
    t->func = func;
    t->func_name = func_name;
    t->data = data;
}

void __tasklet_schedule(struct taskletq *tq, struct tasklet_struct *t);

static void tasklet_schedule(struct tasklet_struct *t)
{
    if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
        __tasklet_schedule(systq, t);
}

void __tasklet_kill(struct taskletq *tq, struct tasklet_struct *t);

static void tasklet_kill(struct tasklet_struct *t)
{
    __tasklet_kill(systq, t);
}

#endif /* _linux_task_h */
