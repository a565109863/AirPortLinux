/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_RTNETLINK_H
#define __LINUX_RTNETLINK_H


#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/wait.h>
#include <linux/refcount.h>
#include <uapi/linux/rtnetlink.h>


extern int rtnetlink_send(struct sk_buff *skb, struct net *net, u32 pid, u32 group, int echo);
extern int rtnl_unicast(struct sk_buff *skb, struct net *net, u32 pid);
extern void rtnl_notify(struct sk_buff *skb, struct net *net, u32 pid,
            u32 group, struct nlmsghdr *nlh, gfp_t flags);
//extern void rtnl_set_sk_err(struct net *net, u32 group, int error);
extern int rtnetlink_put_metrics(struct sk_buff *skb, u32 *metrics);
extern int rtnl_put_cacheinfo(struct sk_buff *skb, struct dst_entry *dst,
                  u32 id, long expires, u32 error);

void rtmsg_ifinfo(int type, struct net_device *dev, unsigned change, gfp_t flags);
void rtmsg_ifinfo_newnet(int type, struct net_device *dev, unsigned int change,
             gfp_t flags, int *new_nsid, int new_ifindex);
struct sk_buff *rtmsg_ifinfo_build_skb(int type, struct net_device *dev,
                       unsigned change, u32 event,
                       gfp_t flags, int *new_nsid,
                       int new_ifindex);
void rtmsg_ifinfo_send(struct sk_buff *skb, struct net_device *dev,
               gfp_t flags);


#ifdef CONFIG_PROVE_LOCKING
extern bool lockdep_rtnl_is_held(void);
#else
static inline bool lockdep_rtnl_is_held(void)
{
    return true;
}
#endif /* #ifdef CONFIG_PROVE_LOCKING */

/**
 * rcu_dereference_rtnl - rcu_dereference with debug checking
 * @p: The pointer to read, prior to dereferencing
 *
 * Do an rcu_dereference(p), but check caller either holds rcu_read_lock()
 * or RTNL. Note : Please prefer rtnl_dereference() or rcu_dereference()
 */
#define rcu_dereference_rtnl(p)                    \
    rcu_dereference_check(p, lockdep_rtnl_is_held())


#endif /* rtnetlink_h */
