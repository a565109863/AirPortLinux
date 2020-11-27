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

#endif /* rcupdate_h */
