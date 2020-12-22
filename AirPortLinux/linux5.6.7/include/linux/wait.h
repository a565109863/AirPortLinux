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

#endif /* linux_wait_h */
