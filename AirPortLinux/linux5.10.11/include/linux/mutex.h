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

static void __mutex_init(struct mutex *m) {
     m->lock = IORecursiveLockAlloc();
}

static void __mutex_lock(struct mutex *m, const char *name, const char *fn) {
    if (m->lock == NULL) {
        __mutex_init(m);
    }
    
//    kprintf("-----%s: line = %d, name = %s, fn = %s start", __FUNCTION__, __LINE__, name, fn);
    IORecursiveLockLock(m->lock);
}

static void __mutex_unlock(struct mutex *m, const char *name, const char *fn) {
//    kprintf("-----%s: line = %d, name = %s, fn = %s start", __FUNCTION__, __LINE__, name, fn);
    IORecursiveLockUnlock(m->lock);
}

#define _mutex_init(m) __mutex_init(m)
#define _mutex_lock(m) __mutex_lock(m, #m, __FUNCTION__)
#define _mutex_unlock(m) __mutex_unlock(m, #m, __FUNCTION__)

#define mutex_init(m)
#define mutex_lock(m)
#define mutex_unlock(m)


static void mutex_destroy(struct mutex *m) {
    IORecursiveLockFree(m->lock);
}

#define DEFINE_MUTEX(mutexname) \
struct mutex mutexname

#endif /* mutex_h */
