//
//  cmpxchg.h
//  AirPortLinux
//
//  Created by Zhong-Mac on 2020/12/6.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef cmpxchg_h
#define cmpxchg_h


/* bug catcher for when unsupported size is used - won't link */
static void __cmpxchg_called_with_bad_pointer(void)
{
    
}
/* we only need to support cmpxchg of a u32 on sparc */
static unsigned long __cmpxchg_u32(volatile u32 *ptr, u32 old, u32 _new)
{
    unsigned long flags;
    u32 prev;
    
//    spin_lock_irqsave(ATOMIC_HASH(ptr), flags);
    if ((prev = *ptr) == old)
        *ptr = _new;
//    spin_unlock_irqrestore(ATOMIC_HASH(ptr), flags);
    
    return (unsigned long)prev;
}
EXPORT_SYMBOL(__cmpxchg_u32);


/* don't worry...optimizer will get rid of most of this */
static inline unsigned long
__cmpxchg(volatile void *ptr, unsigned long old, unsigned long new_, int size)
{
    switch (size) {
        case 4:
            return __cmpxchg_u32((u32 *)ptr, (u32)old, (u32)new_);
        default:
            __cmpxchg_called_with_bad_pointer();
            break;
    }
    return old;
}

#define cmpxchg(ptr, o, n)                        \
({                                    \
__typeof__(*(ptr)) _o_ = (o);                    \
__typeof__(*(ptr)) _n_ = (n);                    \
(__typeof__(*(ptr))) __cmpxchg((ptr), (unsigned long)_o_,    \
(unsigned long)_n_, sizeof(*(ptr)));        \
})

#endif /* cmpxchg_h */
