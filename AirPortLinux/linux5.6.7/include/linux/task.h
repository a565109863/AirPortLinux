//
//  task.h
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/21.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef _linux_task_h
#define _linux_task_h

struct tasklet_struct
{
    struct tasklet_struct *next;
    unsigned long state;
    atomic_t count;
    void (*func)(unsigned long);
    unsigned long data;
};

static void tasklet_init(struct tasklet_struct *t,
          void (*func)(unsigned long), unsigned long data)
{
    t->next = NULL;
    t->state = 0;
    atomic_set(&t->count, 0);
    t->func = func;
    t->data = data;
}

#endif /* _linux_task_h */
