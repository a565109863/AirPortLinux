//
//  cpu.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef cpu_h
#define cpu_h

#include <linux/types.h>
#define NR_CPUS 1


#define DECLARE_PER_CPU(type, val) extern type val
#define DEFINE_PER_CPU(type, val) type val

#define __get_cpu_var(var)    var
#define this_cpu_ptr(var)    var
#define this_cpu_read(var)    var
#define this_cpu_xchg(var, val)        uatomic_xchg(&var, val)
#define this_cpu_cmpxchg(var, old, new)    uatomic_cmpxchg(&var, old, new)
#define per_cpu_ptr(ptr, cpu)   ({ (void)(cpu); (ptr); })
#define per_cpu(var, cpu)    (*per_cpu_ptr(&(var), cpu))




/* Don't assign or return these: may not be this big! */
typedef struct cpumask { DECLARE_BITMAP(bits, NR_CPUS); } cpumask_t;

#endif /* cpu_h */
