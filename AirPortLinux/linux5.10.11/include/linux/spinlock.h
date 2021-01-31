//
//  spinlock.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef spinlock_h
#define spinlock_h

#include <linux/bitops.h>
#include <linux/lockdep.h>

#define PTHREAD_MUTEX_INITIALIZER NULL

#define pthread_mutex_t         spinlock_t
#define raw_spinlock_t          spinlock_t
#define DEFINE_SPINLOCK(x)    pthread_mutex_t x = PTHREAD_MUTEX_INITIALIZER
#define __SPIN_LOCK_UNLOCKED(x)    (pthread_mutex_t)NULL
//#define spin_lock_init(x)    pthread_mutex_init(x, NULL)


#define __ARCH_SPIN_LOCK_UNLOCKED PTHREAD_MUTEX_INITIALIZER


#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define RAW_SPIN_DEP_MAP_INIT(lockname)        \
.dep_map = {                    \
.name = #lockname,            \
.wait_type_inner = LD_WAIT_SPIN,    \
}
# define SPIN_DEP_MAP_INIT(lockname)            \
.dep_map = {                    \
.name = #lockname,            \
.wait_type_inner = LD_WAIT_CONFIG,    \
}
#else
# define RAW_SPIN_DEP_MAP_INIT(lockname)
# define SPIN_DEP_MAP_INIT(lockname)
#endif

#ifdef CONFIG_DEBUG_SPINLOCK
# define SPIN_DEBUG_INIT(lockname)        \
.magic = SPINLOCK_MAGIC,        \
.owner_cpu = -1,            \
.owner = SPINLOCK_OWNER_INIT,
#else
# define SPIN_DEBUG_INIT(lockname)
#endif

#define __RAW_SPIN_LOCK_INITIALIZER(lockname)  NULL

#define __RAW_SPIN_LOCK_UNLOCKED(lockname)    \
(raw_spinlock_t) __RAW_SPIN_LOCK_INITIALIZER(lockname)



static inline void spin_lock_init(spinlock_t *lock)
{
//    lock_impl_init(&lock->internal_lock);
}

/*
 * Map the spin_lock functions to the raw variants for PREEMPT_RT=n
 */

//static __always_inline raw_spinlock_t *spinlock_check(spinlock_t *lock)
//{
//    return &lock->rlock;
//}

//#define spin_lock_init(_lock)                \
//do {                            \
//    spinlock_check(_lock);                \
//    raw_spin_lock_init(&(_lock)->rlock);        \
//} while (0)

static __always_inline void spin_lock(spinlock_t *lock)
{
//    raw_spin_lock(&lock->rlock);
}

static __always_inline void spin_lock_bh(spinlock_t *lock)
{
//    raw_spin_lock_bh(&lock->rlock);
}

static __always_inline int spin_trylock(spinlock_t *lock)
{
//    return raw_spin_trylock(&lock->rlock);
    return 0;
}

#define spin_lock_nested(lock, subclass)            \
do {                                \
} while (0)

#define spin_lock_nest_lock(lock, nest_lock)                \
do {                                    \
} while (0)

static __always_inline void spin_lock_irq(spinlock_t *lock)
{
//    raw_spin_lock_irq(&lock->rlock);
}

#define spin_lock_irqsave(lock, flags)                \
do {                                \
} while (0)

#define spin_lock_irqsave_nested(lock, flags, subclass)            \
do {                                    \
} while (0)

static __always_inline void spin_unlock(spinlock_t *lock)
{
//    raw_spin_unlock(&lock->rlock);
}

static __always_inline void spin_unlock_bh(spinlock_t *lock)
{
//    raw_spin_unlock_bh(&lock->rlock);
}

static __always_inline void spin_unlock_irq(spinlock_t *lock)
{
//    raw_spin_unlock_irq(&lock->rlock);
}

static __always_inline void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
{
//    raw_spin_unlock_irqrestore(&lock->rlock, flags);
}

static __always_inline int spin_trylock_bh(spinlock_t *lock)
{
//    return raw_spin_trylock_bh(&lock->rlock);
    return 0;
}

static __always_inline int spin_trylock_irq(spinlock_t *lock)
{
//    return raw_spin_trylock_irq(&lock->rlock);
    return 0;
}

//#define spin_trylock_irqsave(lock, flags)            \
//({                                \
//    raw_spin_trylock_irqsave(spinlock_check(lock), flags); \
//})

/**
 * spin_is_locked() - Check whether a spinlock is locked.
 * @lock: Pointer to the spinlock.
 *
 * This function is NOT required to provide any memory ordering
 * guarantees; it could be used for debugging purposes or, when
 * additional synchronization is needed, accompanied with other
 * constructs (memory barriers) enforcing the synchronization.
 *
 * Returns: 1 if @lock is locked, 0 otherwise.
 *
 * Note that the function only tells you that the spinlock is
 * seen to be locked, not that it is locked on your CPU.
 *
 * Further, on CONFIG_SMP=n builds with CONFIG_DEBUG_SPINLOCK=n,
 * the return value is always 0 (see include/linux/spinlock_up.h).
 * Therefore you should not rely heavily on the return value.
 */
static __always_inline int spin_is_locked(spinlock_t *lock)
{
//    return raw_spin_is_locked(&lock->rlock);
    return 0;
}

static __always_inline int spin_is_contended(spinlock_t *lock)
{
//    return raw_spin_is_contended(&lock->rlock);
    return 0;
}


#endif /* spinlock_h */
