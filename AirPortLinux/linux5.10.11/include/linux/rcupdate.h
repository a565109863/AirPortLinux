//
//  rcupdate.h
//  AppleIntelWiFi
//
//  Created by User-PC on 2020/6/15.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef rcupdate_h
#define rcupdate_h


#define rcu_dereference_raw(p) rcu_dereference(p)
#define rcu_dereference_protected(p, cond) rcu_dereference(p)
#define rcu_dereference_check(p, cond) rcu_dereference(p)
#define RCU_INIT_POINTER(p, v)    do { (p) = (v); } while (0)


#define rcu_read_lock_held() 0
#define rcu_read_lock_bh_held() 0



static inline bool
rcu_head_after_call_rcu(struct rcu_head *rhp, rcu_callback_t f)
{
    rcu_callback_t func = READ_ONCE(rhp->func);

    if (func == f)
        return true;
    WARN_ON_ONCE(func != (rcu_callback_t)~0L);
    return false;
}


#endif /* rcupdate_h */
