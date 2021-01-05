/* SPDX-License-Identifier: GPL-2.0 */
/* Atomic operations usable in machine independent code */
#ifndef _LINUX_ATOMIC_H
#define _LINUX_ATOMIC_H
#include <linux/types.h>

#include <uapi/asm/atomic.h>
//#include <asm/barrier.h>


#define ATOMIC_INIT(i)        { (i) }
#define ATOMIC64_INIT(i)    { (i) }

#define atomic_read(v)        READ_ONCE((v)->counter)
#define atomic64_read(v)    READ_ONCE((v)->counter)

#define atomic_set(v,i)        WRITE_ONCE((v)->counter, (i))
#define atomic64_set(v,i)    WRITE_ONCE((v)->counter, (i))


#define ATOMIC_LONG_INIT(i)        ATOMIC64_INIT(i)


struct va_format {
    const char *fmt;
    va_list *va;
};

static inline int atomic_inc_return( atomic_t *v)
{
    v->counter++;
    return v->counter;
}

static inline void atomic_dec( atomic_t *v)
{
    v->counter--;
}

static inline int atomic_dec_return( atomic_t *v)
{
    v->counter--;
    return v->counter;
}

static inline void atomic_inc( atomic_t *v)
{
    if (v->counter == NULL) {
        atomic_set(v, 0);
    }
    v->counter++;
}

static inline int atomic_dec_if_positive(atomic_t *v)
{
    unsigned long flags;
    int res;
    
//    local_irq_save(flags);
    res = v->counter - 1;
    if (res >= 0)
        v->counter = res;
//    local_irq_restore(flags);
    
    return res;
}


//
///*
// * Relaxed variants of xchg, cmpxchg and some atomic operations.
// *
// * We support four variants:
// *
// * - Fully ordered: The default implementation, no suffix required.
// * - Acquire: Provides ACQUIRE semantics, _acquire suffix.
// * - Release: Provides RELEASE semantics, _release suffix.
// * - Relaxed: No ordering guarantees, _relaxed suffix.
// *
// * For compound atomics performing both a load and a store, ACQUIRE
// * semantics apply only to the load and RELEASE semantics only to the
// * store portion of the operation. Note that a failed cmpxchg_acquire
// * does -not- imply any memory ordering constraints.
// *
// * See Documentation/memory-barriers.txt for ACQUIRE/RELEASE definitions.
// */
//
///*
// * The idea here is to build acquire/release variants by adding explicit
// * barriers on top of the relaxed variant. In the case where the relaxed
// * variant is already fully ordered, no additional barriers are needed.
// *
// * If an architecture overrides __atomic_acquire_fence() it will probably
// * want to define smp_mb__after_spinlock().
// */
//#ifndef __atomic_acquire_fence
//#define __atomic_acquire_fence        smp_mb__after_atomic
//#endif
//
//#ifndef __atomic_release_fence
//#define __atomic_release_fence        smp_mb__before_atomic
//#endif
//
//#ifndef __atomic_pre_full_fence
//#define __atomic_pre_full_fence        smp_mb__before_atomic
//#endif
//
//#ifndef __atomic_post_full_fence
//#define __atomic_post_full_fence    smp_mb__after_atomic
//#endif
//
//#define __atomic_op_acquire(op, args...)                \
//({                                    \
//    typeof(op##_relaxed(args)) __ret  = op##_relaxed(args);        \
//    __atomic_acquire_fence();                    \
//    __ret;                                \
//})
//
//#define __atomic_op_release(op, args...)                \
//({                                    \
//    __atomic_release_fence();                    \
//    op##_relaxed(args);                        \
//})
//
//#define __atomic_op_fence(op, args...)                    \
//({                                    \
//    typeof(op##_relaxed(args)) __ret;                \
//    __atomic_pre_full_fence();                    \
//    __ret = op##_relaxed(args);                    \
//    __atomic_post_full_fence();                    \
//    __ret;                                \
//})
//
//#include <linux/atomic-fallback.h>
//
//#include <asm-generic/atomic-long.h>
//

static inline int
atomic_fetch_add_relaxed(int i, atomic_t *v)
{
//    kasan_check_write(v, sizeof(*v));
//    return arch_atomic_fetch_add_relaxed(i, v);
    return i;
}

static inline int
atomic_fetch_sub_release(int i, atomic_t *v)
{
//    kasan_check_write(v, sizeof(*v));
//    return arch_atomic_fetch_sub_release(i, v);
    return i;
}

static inline void
atomic64_add(int n, atomic64_t *v)
{
    v->counter = n;
}

static inline void
atomic64_inc(atomic64_t *v)
{
    atomic64_add(1, v);
}

static inline void
atomic_long_inc(atomic_long_t *v)
{
    atomic64_inc(v);
}

static inline long
atomic_long_read(const atomic_long_t *v)
{
    return atomic64_read(v);
}

#endif /* _LINUX_ATOMIC_H */
