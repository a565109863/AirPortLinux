//
//  lockdep.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef lockdep_h
#define lockdep_h

struct stack_trace {
    unsigned int nr_entries, max_entries;
    unsigned long *entries;
    int skip;    /* input argument: How many entries to skip */
};

/*
 * We'd rather not expose kernel/lockdep_states.h this wide, but we do need
 * the total number of states... :-(
 */
#define XXX_LOCK_USAGE_STATES        (1+3*4)

#define MAX_LOCKDEP_SUBCLASSES        8UL

/*
 * NR_LOCKDEP_CACHING_CLASSES ... Number of classes
 * cached in the instance of lockdep_map
 *
 * Currently main class (subclass == 0) and signle depth subclass
 * are cached in lockdep_map. This optimization is mainly targeting
 * on rq->lock. double_rq_lock() acquires this highly competitive with
 * single depth.
 */
#define NR_LOCKDEP_CACHING_CLASSES    2

/*
 * Lock-classes are keyed via unique addresses, by embedding the
 * lockclass-key into the kernel (or module) .data section. (For
 * static locks we use the lock address itself as the key.)
 */
struct lockdep_subclass_key {
    char __one_byte;
} __attribute__ ((__packed__));

struct lock_class_key {
    struct lockdep_subclass_key    subkeys[MAX_LOCKDEP_SUBCLASSES];
};

extern struct lock_class_key __lockdep_no_validate__;

#define LOCKSTAT_POINTS        4

/*
 * The lock-class itself:
 */
struct lock_class {
    /*
     * class-hash:
     */
    struct list_head        hash_entry;
    
    /*
     * global list of all lock-classes:
     */
    struct list_head        lock_entry;
    
    struct lockdep_subclass_key    *key;
    unsigned int            subclass;
    unsigned int            dep_gen_id;
    
    /*
     * IRQ/softirq usage tracking bits:
     */
    unsigned long            usage_mask;
    struct stack_trace        usage_traces[XXX_LOCK_USAGE_STATES];
    
    /*
     * These fields represent a directed graph of lock dependencies,
     * to every node we attach a list of "forward" and a list of
     * "backward" graph nodes.
     */
    struct list_head        locks_after, locks_before;
    
    /*
     * Generation counter, when doing certain classes of graph walking,
     * to ensure that we check one node only once:
     */
    unsigned int            version;
    
    /*
     * Statistics counter:
     */
    unsigned long            ops;
    
    const char            *name;
    int                name_version;
    
#ifdef CONFIG_LOCK_STAT
    unsigned long            contention_point[LOCKSTAT_POINTS];
    unsigned long            contending_point[LOCKSTAT_POINTS];
#endif
};

/*
 * Map the lock object (the lock instance) to the lock-class object.
 * This is embedded into specific lock instances:
 */
struct lockdep_map {
    struct lock_class_key        *key;
    struct lock_class        *class_cache[NR_LOCKDEP_CACHING_CLASSES];
    const char            *name;
#ifdef CONFIG_LOCK_STAT
    int                cpu;
    unsigned long            ip;
#endif
};


// 378
# define lock_acquire(l, s, t, r, c, n, i)    do { } while (0)
# define lock_release(l, n, i)            do { } while (0)
# define lock_set_class(l, n, k, s, i)        do { } while (0)
# define lock_set_subclass(l, s, i)        do { } while (0)
# define lockdep_set_current_reclaim_state(g)    do { } while (0)
# define lockdep_clear_current_reclaim_state()    do { } while (0)
# define lockdep_trace_alloc(g)            do { } while (0)
# define lockdep_init()                do { } while (0)
# define lockdep_info()                do { } while (0)
# define lockdep_init_map(lock, name, key, sub) \
do { (void)(name); (void)(key); } while (0)
# define lockdep_set_class(lock, key)        do { (void)(key); } while (0)
# define lockdep_set_class_and_name(lock, key, name) \
do { (void)(key); (void)(name); } while (0)
#define lockdep_set_class_and_subclass(lock, key, sub) \
do { (void)(key); } while (0)
#define lockdep_set_subclass(lock, sub)        do { } while (0)

#define lockdep_set_novalidate_class(lock) do { } while (0)

/*
 * We don't define lockdep_match_class() and lockdep_match_key() for !LOCKDEP
 * case since the result is not well defined and the caller should rather
 * #ifdef the call himself.
 */

# define INIT_LOCKDEP
# define lockdep_reset()        do { debug_locks = 1; } while (0)
# define lockdep_free_key_range(start, size)    do { } while (0)
# define lockdep_sys_exit()             do { } while (0)
/*
 * The class key takes no space if lockdep is disabled:
 */
//struct lock_class_key { };

#define lockdep_depth(tsk)    (0)

#define lockdep_assert_held(l)            do { (void)(l); } while (0)

#define lockdep_recursing(tsk)            (0)


// 477
/*
 * Map the dependency ops to NOP or to real lockdep ops, depending
 * on the per lock-class debug mode:
 */

#ifdef CONFIG_PROVE_LOCKING
#define lock_acquire_exclusive(l, s, t, n, i)        lock_acquire(l, s, t, 0, 2, n, i)
#define lock_acquire_shared(l, s, t, n, i)        lock_acquire(l, s, t, 1, 2, n, i)
#define lock_acquire_shared_recursive(l, s, t, n, i)    lock_acquire(l, s, t, 2, 2, n, i)
#else
#define lock_acquire_exclusive(l, s, t, n, i)        lock_acquire(l, s, t, 0, 1, n, i)
#define lock_acquire_shared(l, s, t, n, i)        lock_acquire(l, s, t, 1, 1, n, i)
#define lock_acquire_shared_recursive(l, s, t, n, i)    lock_acquire(l, s, t, 2, 1, n, i)
#endif

#define spin_acquire(l, s, t, i)        lock_acquire_exclusive(l, s, t, NULL, i)
#define spin_acquire_nest(l, s, t, n, i)    lock_acquire_exclusive(l, s, t, n, i)
#define spin_release(l, n, i)            lock_release(l, n, i)

#define rwlock_acquire(l, s, t, i)        lock_acquire_exclusive(l, s, t, NULL, i)
#define rwlock_acquire_read(l, s, t, i)        lock_acquire_shared_recursive(l, s, t, NULL, i)
#define rwlock_release(l, n, i)            lock_release(l, n, i)

#define seqcount_acquire(l, s, t, i)        lock_acquire_exclusive(l, s, t, NULL, i)
#define seqcount_acquire_read(l, s, t, i)    lock_acquire_shared_recursive(l, s, t, NULL, i)
#define seqcount_release(l, n, i)        lock_release(l, n, i)

#define mutex_acquire(l, s, t, i)        lock_acquire_exclusive(l, s, t, NULL, i)
#define mutex_acquire_nest(l, s, t, n, i)    lock_acquire_exclusive(l, s, t, n, i)
#define mutex_release(l, n, i)            lock_release(l, n, i)

#define rwsem_acquire(l, s, t, i)        lock_acquire_exclusive(l, s, t, NULL, i)
#define rwsem_acquire_nest(l, s, t, n, i)    lock_acquire_exclusive(l, s, t, n, i)
#define rwsem_acquire_read(l, s, t, i)        lock_acquire_shared(l, s, t, NULL, i)
#define rwsem_release(l, n, i)            lock_release(l, n, i)

#define lock_map_acquire(l)            lock_acquire_exclusive(l, 0, 0, NULL, _THIS_IP_)
#define lock_map_acquire_read(l)        lock_acquire_shared_recursive(l, 0, 0, NULL, _THIS_IP_)
#define lock_map_release(l)            lock_release(l, 1, _THIS_IP_)

#ifdef CONFIG_PROVE_LOCKING
# define might_lock(lock)                         \
do {                                    \
typecheck(struct lockdep_map *, &(lock)->dep_map);        \
lock_acquire(&(lock)->dep_map, 0, 0, 0, 2, NULL, _THIS_IP_);    \
lock_release(&(lock)->dep_map, 0, _THIS_IP_);            \
} while (0)
# define might_lock_read(lock)                         \
do {                                    \
typecheck(struct lockdep_map *, &(lock)->dep_map);        \
lock_acquire(&(lock)->dep_map, 0, 0, 1, 2, NULL, _THIS_IP_);    \
lock_release(&(lock)->dep_map, 0, _THIS_IP_);            \
} while (0)
#else
# define might_lock(lock) do { } while (0)
# define might_lock_read(lock) do { } while (0)
#endif

#ifdef CONFIG_PROVE_RCU
void lockdep_rcu_suspicious(const char *file, const int line, const char *s);
#endif


#define lockdep_is_held(x) x

#define list_del_rcu list_del


/*
 * For trivial one-depth nesting of a lock-class, the following
 * global define can be used. (Subsystems with multiple levels
 * of nesting should define their own lock-nesting subclasses.)
 */
#define SINGLE_DEPTH_NESTING            1

#endif /* lockdep_h */
