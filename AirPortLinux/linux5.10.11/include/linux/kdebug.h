/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KDEBUG_H
#define _LINUX_KDEBUG_H

//#include <asm/kdebug.h>

enum die_val {
    DIE_UNUSED,
    DIE_OOPS = 1,
};

struct pt_regs {
   unsigned long regs[16];
   unsigned long pc;
   unsigned long pr;
   unsigned long sr;
   unsigned long gbr;
   unsigned long mach;
   unsigned long macl;
   long tra;
};

struct notifier_block;

struct die_args {
    struct pt_regs *regs;
    const char *str;
    long err;
    int trapnr;
    int signr;
};

int register_die_notifier(struct notifier_block *nb);
int unregister_die_notifier(struct notifier_block *nb);

int notify_die(enum die_val val, const char *str,
           struct pt_regs *regs, long err, int trap, int sig);

#endif /* _LINUX_KDEBUG_H */