//
//  mutex.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef mutex_h
#define mutex_h

#include <linux/types.h>

struct mutex {
    IORecursiveLock *lock;
};

static void mutex_init(struct mutex *m) {
     m->lock = IORecursiveLockAlloc();
}

static void mutex_lock(struct mutex *m) {
    if (m->lock == NULL) {
        mutex_init(m);
    }
    
    IORecursiveLockLock(m->lock);
}

static void mutex_unlock(struct mutex *m) {
    IORecursiveLockUnlock(m->lock);
}

static void mutex_destroy(struct mutex *m) {
    IORecursiveLockLock(m->lock);
}

#define DEFINE_MUTEX(mutexname) \
struct mutex mutexname

#endif /* mutex_h */
