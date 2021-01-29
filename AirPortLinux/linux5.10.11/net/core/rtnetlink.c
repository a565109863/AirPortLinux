// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * INET        An implementation of the TCP/IP protocol suite for the LINUX
 *        operating system.  INET is implemented using the  BSD Socket
 *        interface as the means of communication with the user level.
 *
 *        Routing netlink socket interface: protocol independent part.
 *
 * Authors:    Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 *    Fixes:
 *    Vitaly E. Lavrov        RTA_OK arithmetics was wrong.
 */

#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/_string.h>
//#include <linux/sockios.h>
#include <linux/net.h>
//#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/skbuff.h>
#include <linux/init.h>
//#include <linux/security.h>
#include <linux/mutex.h>
//#include <linux/if_addr.h>
//#include <linux/if_bridge.h>
#include <linux/if_vlan.h>
#include <linux/pci.h>
#include <linux/etherdevice.h>
//#include <linux/bpf.h>

#include <linux/uaccess.h>

//#include <linux/inet.h>
#include <linux/netdevice.h>
#include <net/ip.h>
//#include <net/protocol.h>
//#include <net/arp.h>
#include <net/route.h>
//#include <net/udp.h>
//#include <net/tcp.h>
//#include <net/sock.h>
#include <net/pkt_sched.h>
//#include <net/fib_rules.h>
#include <net/rtnetlink.h>
#include <net/net_namespace.h>


/* All VF info */
static inline int rtnl_vfinfo_size(const struct net_device *dev,
                   u32 ext_filter_mask)
{
//    if (dev->dev.parent && (ext_filter_mask & RTEXT_FILTER_VF)) {
//        int num_vfs = dev_num_vf(dev->dev.parent);
//        size_t size = nla_total_size(0);
//        size += num_vfs *
//            (nla_total_size(0) +
//             nla_total_size(sizeof(struct ifla_vf_mac)) +
//             nla_total_size(sizeof(struct ifla_vf_broadcast)) +
//             nla_total_size(sizeof(struct ifla_vf_vlan)) +
//             nla_total_size(0) + /* nest IFLA_VF_VLAN_LIST */
//             nla_total_size(MAX_VLAN_LIST_LEN *
//                    sizeof(struct ifla_vf_vlan_info)) +
//             nla_total_size(sizeof(struct ifla_vf_spoofchk)) +
//             nla_total_size(sizeof(struct ifla_vf_tx_rate)) +
//             nla_total_size(sizeof(struct ifla_vf_rate)) +
//             nla_total_size(sizeof(struct ifla_vf_link_state)) +
//             nla_total_size(sizeof(struct ifla_vf_rss_query_en)) +
//             nla_total_size(0) + /* nest IFLA_VF_STATS */
//             /* IFLA_VF_STATS_RX_PACKETS */
//             nla_total_size_64bit(sizeof(__u64)) +
//             /* IFLA_VF_STATS_TX_PACKETS */
//             nla_total_size_64bit(sizeof(__u64)) +
//             /* IFLA_VF_STATS_RX_BYTES */
//             nla_total_size_64bit(sizeof(__u64)) +
//             /* IFLA_VF_STATS_TX_BYTES */
//             nla_total_size_64bit(sizeof(__u64)) +
//             /* IFLA_VF_STATS_BROADCAST */
//             nla_total_size_64bit(sizeof(__u64)) +
//             /* IFLA_VF_STATS_MULTICAST */
//             nla_total_size_64bit(sizeof(__u64)) +
//             /* IFLA_VF_STATS_RX_DROPPED */
//             nla_total_size_64bit(sizeof(__u64)) +
//             /* IFLA_VF_STATS_TX_DROPPED */
//             nla_total_size_64bit(sizeof(__u64)) +
//             nla_total_size(sizeof(struct ifla_vf_trust)));
//        return size;
//    } else
        return 0;
}

static size_t rtnl_port_size(const struct net_device *dev,
                 u32 ext_filter_mask)
{
    size_t port_size = nla_total_size(4)        /* PORT_VF */
        + nla_total_size(PORT_PROFILE_MAX)    /* PORT_PROFILE */
        + nla_total_size(PORT_UUID_MAX)        /* PORT_INSTANCE_UUID */
        + nla_total_size(PORT_UUID_MAX)        /* PORT_HOST_UUID */
        + nla_total_size(1)            /* PROT_VDP_REQUEST */
        + nla_total_size(2);            /* PORT_VDP_RESPONSE */
    size_t vf_ports_size = nla_total_size(sizeof(struct nlattr));
    size_t vf_port_size = nla_total_size(sizeof(struct nlattr))
        + port_size;
    size_t port_self_size = nla_total_size(sizeof(struct nlattr))
        + port_size;

    if (!dev->netdev_ops->ndo_get_vf_port || !dev->dev.parent ||
        !(ext_filter_mask & RTEXT_FILTER_VF))
        return 0;
//    if (dev_num_vf(dev->dev.parent))
//        return port_self_size + vf_ports_size +
//            vf_port_size * dev_num_vf(dev->dev.parent);
//    else
        return port_self_size;
}

static size_t rtnl_xdp_size(void)
{
    size_t xdp_size = nla_total_size(0) +    /* nest IFLA_XDP */
              nla_total_size(1) +    /* XDP_ATTACHED */
              nla_total_size(4) +    /* XDP_PROG_ID (or 1st mode) */
              nla_total_size(4);    /* XDP_<mode>_PROG_ID */

    return xdp_size;
}

static size_t rtnl_prop_list_size(const struct net_device *dev)
{
    struct netdev_name_node *name_node;
    size_t size;

    if (list_empty(&dev->name_node->list))
        return 0;
    size = nla_total_size(0);
    list_for_each_entry(name_node, &dev->name_node->list, list)
        size += nla_total_size(ALTIFNAMSIZ);
    return size;
}

static size_t rtnl_link_get_slave_info_data_size(const struct net_device *dev)
{
    struct net_device *master_dev;
    const struct rtnl_link_ops *ops;
    size_t size = 0;

    rcu_read_lock();

    master_dev = netdev_master_upper_dev_get_rcu((struct net_device *)dev);
    if (!master_dev)
        goto out;

    ops = master_dev->rtnl_link_ops;
    if (!ops || !ops->get_slave_size)
        goto out;
    /* IFLA_INFO_SLAVE_DATA + nested data */
    size = nla_total_size(sizeof(struct nlattr)) +
           ops->get_slave_size(master_dev, dev);

out:
    rcu_read_unlock();
    return size;
}

static size_t rtnl_link_get_size(const struct net_device *dev)
{
    const struct rtnl_link_ops *ops = dev->rtnl_link_ops;
    size_t size;

    if (!ops)
        return 0;

    size = nla_total_size(sizeof(struct nlattr)) + /* IFLA_LINKINFO */
           nla_total_size(strlen(ops->kind) + 1);  /* IFLA_INFO_KIND */

    if (ops->get_size)
        /* IFLA_INFO_DATA + nested data */
        size += nla_total_size(sizeof(struct nlattr)) +
            ops->get_size(dev);

    if (ops->get_xstats_size)
        /* IFLA_INFO_XSTATS */
        size += nla_total_size(ops->get_xstats_size(dev));

    size += rtnl_link_get_slave_info_data_size(dev);

    return size;
}

static LIST_HEAD(rtnl_af_ops);

static size_t rtnl_link_get_af_size(const struct net_device *dev,
                    u32 ext_filter_mask)
{
    struct rtnl_af_ops *af_ops;
    size_t size;

    /* IFLA_AF_SPEC */
    size = nla_total_size(sizeof(struct nlattr));

    rcu_read_lock();
    list_for_each_entry_rcu(af_ops, &rtnl_af_ops, list) {
        if (af_ops->get_link_af_size) {
            /* AF_* + nested data */
            size += nla_total_size(sizeof(struct nlattr)) +
                af_ops->get_link_af_size(dev, ext_filter_mask);
        }
    }
    rcu_read_unlock();

    return size;
}

static noinline size_t if_nlmsg_size(const struct net_device *dev,
                     u32 ext_filter_mask)
{
    return NLMSG_ALIGN(sizeof(struct ifinfomsg))
           + nla_total_size(IFNAMSIZ) /* IFLA_IFNAME */
           + nla_total_size(IFALIASZ) /* IFLA_IFALIAS */
           + nla_total_size(IFNAMSIZ) /* IFLA_QDISC */
           + nla_total_size_64bit(sizeof(struct rtnl_link_ifmap))
           + nla_total_size(sizeof(struct rtnl_link_stats))
           + nla_total_size_64bit(sizeof(struct rtnl_link_stats64))
           + nla_total_size(MAX_ADDR_LEN) /* IFLA_ADDRESS */
           + nla_total_size(MAX_ADDR_LEN) /* IFLA_BROADCAST */
           + nla_total_size(4) /* IFLA_TXQLEN */
           + nla_total_size(4) /* IFLA_WEIGHT */
           + nla_total_size(4) /* IFLA_MTU */
           + nla_total_size(4) /* IFLA_LINK */
           + nla_total_size(4) /* IFLA_MASTER */
           + nla_total_size(1) /* IFLA_CARRIER */
           + nla_total_size(4) /* IFLA_PROMISCUITY */
           + nla_total_size(4) /* IFLA_NUM_TX_QUEUES */
           + nla_total_size(4) /* IFLA_NUM_RX_QUEUES */
           + nla_total_size(4) /* IFLA_GSO_MAX_SEGS */
           + nla_total_size(4) /* IFLA_GSO_MAX_SIZE */
           + nla_total_size(1) /* IFLA_OPERSTATE */
           + nla_total_size(1) /* IFLA_LINKMODE */
           + nla_total_size(4) /* IFLA_CARRIER_CHANGES */
           + nla_total_size(4) /* IFLA_LINK_NETNSID */
           + nla_total_size(4) /* IFLA_GROUP */
           + nla_total_size(ext_filter_mask
                    & RTEXT_FILTER_VF ? 4 : 0) /* IFLA_NUM_VF */
           + rtnl_vfinfo_size(dev, ext_filter_mask) /* IFLA_VFINFO_LIST */
           + rtnl_port_size(dev, ext_filter_mask) /* IFLA_VF_PORTS + IFLA_PORT_SELF */
           + rtnl_link_get_size(dev) /* IFLA_LINKINFO */
           + rtnl_link_get_af_size(dev, ext_filter_mask) /* IFLA_AF_SPEC */
           + nla_total_size(MAX_PHYS_ITEM_ID_LEN) /* IFLA_PHYS_PORT_ID */
           + nla_total_size(MAX_PHYS_ITEM_ID_LEN) /* IFLA_PHYS_SWITCH_ID */
           + nla_total_size(IFNAMSIZ) /* IFLA_PHYS_PORT_NAME */
           + rtnl_xdp_size() /* IFLA_XDP */
           + nla_total_size(4)  /* IFLA_EVENT */
           + nla_total_size(4)  /* IFLA_NEW_NETNSID */
           + nla_total_size(4)  /* IFLA_NEW_IFINDEX */
           + nla_total_size(1)  /* IFLA_PROTO_DOWN */
           + nla_total_size(4)  /* IFLA_TARGET_NETNSID */
           + nla_total_size(4)  /* IFLA_CARRIER_UP_COUNT */
           + nla_total_size(4)  /* IFLA_CARRIER_DOWN_COUNT */
           + nla_total_size(4)  /* IFLA_MIN_MTU */
           + nla_total_size(4)  /* IFLA_MAX_MTU */
           + rtnl_prop_list_size(dev)
           + nla_total_size(MAX_ADDR_LEN) /* IFLA_PERM_ADDRESS */
           + 0;
}


static int put_master_ifindex(struct sk_buff *skb, struct net_device *dev)
{
    const struct net_device *upper_dev;
    int ret = 0;

    rcu_read_lock();

    upper_dev = netdev_master_upper_dev_get_rcu(dev);
    if (upper_dev)
        ret = nla_put_u32(skb, IFLA_MASTER, upper_dev->ifindex);

    rcu_read_unlock();
    return ret;
}

static int nla_put_iflink(struct sk_buff *skb, const struct net_device *dev,
              bool force)
{
    int ifindex = dev_get_iflink(dev);

    if (force || dev->ifindex != ifindex)
        return nla_put_u32(skb, IFLA_LINK, ifindex);

    return 0;
}

#define  noinline_for_stack
static noinline_for_stack int nla_put_ifalias(struct sk_buff *skb,
                          struct net_device *dev)
{
    char buf[IFALIASZ];
    int ret;

    ret = dev_get_alias(dev, buf, sizeof(buf));
    return ret > 0 ? nla_put_string(skb, IFLA_IFALIAS, buf) : 0;
}

static int rtnl_fill_link_ifmap(struct sk_buff *skb, struct net_device *dev)
{
    struct rtnl_link_ifmap map;

    memset(&map, 0, sizeof(map));
    map.mem_start   = dev->mem_start;
    map.mem_end     = dev->mem_end;
    map.base_addr   = dev->base_addr;
    map.irq         = dev->irq;
    map.dma         = dev->dma;
    map.port        = dev->if_port;

    if (nla_put_64bit(skb, IFLA_MAP, sizeof(map), &map, IFLA_PAD))
        return -EMSGSIZE;

    return 0;
}


static int rtnl_vf_ports_fill(struct sk_buff *skb, struct net_device *dev)
{
    struct nlattr *vf_ports;
    struct nlattr *vf_port;
    int vf;
    int err;

    vf_ports = nla_nest_start_noflag(skb, IFLA_VF_PORTS);
    if (!vf_ports)
        return -EMSGSIZE;

//    for (vf = 0; vf < dev_num_vf(dev->dev.parent); vf++) {
//        vf_port = nla_nest_start_noflag(skb, IFLA_VF_PORT);
//        if (!vf_port)
//            goto nla_put_failure;
//        if (nla_put_u32(skb, IFLA_PORT_VF, vf))
//            goto nla_put_failure;
//        err = dev->netdev_ops->ndo_get_vf_port(dev, vf, skb);
//        if (err == -EMSGSIZE)
//            goto nla_put_failure;
//        if (err) {
//            nla_nest_cancel(skb, vf_port);
//            continue;
//        }
//        nla_nest_end(skb, vf_port);
//    }

    nla_nest_end(skb, vf_ports);

    return 0;

nla_put_failure:
    nla_nest_cancel(skb, vf_ports);
    return -EMSGSIZE;
}

static int rtnl_port_self_fill(struct sk_buff *skb, struct net_device *dev)
{
    struct nlattr *port_self;
    int err;

    port_self = nla_nest_start_noflag(skb, IFLA_PORT_SELF);
    if (!port_self)
        return -EMSGSIZE;

    err = dev->netdev_ops->ndo_get_vf_port(dev, PORT_SELF_VF, skb);
    if (err) {
        nla_nest_cancel(skb, port_self);
        return (err == -EMSGSIZE) ? err : 0;
    }

    nla_nest_end(skb, port_self);

    return 0;
}

static int rtnl_port_fill(struct sk_buff *skb, struct net_device *dev,
              u32 ext_filter_mask)
{
    int err;

    if (!dev->netdev_ops->ndo_get_vf_port || !dev->dev.parent ||
        !(ext_filter_mask & RTEXT_FILTER_VF))
        return 0;

    err = rtnl_port_self_fill(skb, dev);
    if (err)
        return err;

//    if (dev_num_vf(dev->dev.parent)) {
//        err = rtnl_vf_ports_fill(skb, dev);
//        if (err)
//            return err;
//    }

    return 0;
}

static int rtnl_phys_port_id_fill(struct sk_buff *skb, struct net_device *dev)
{
    int err;
    struct netdev_phys_item_id ppid;

    err = dev_get_phys_port_id(dev, &ppid);
    if (err) {
        if (err == -EOPNOTSUPP)
            return 0;
        return err;
    }

    if (nla_put(skb, IFLA_PHYS_PORT_ID, ppid.id_len, ppid.id))
        return -EMSGSIZE;

    return 0;
}

static int rtnl_phys_port_name_fill(struct sk_buff *skb, struct net_device *dev)
{
    char name[IFNAMSIZ];
    int err;

    err = dev_get_phys_port_name(dev, name, sizeof(name));
    if (err) {
        if (err == -EOPNOTSUPP)
            return 0;
        return err;
    }

    if (nla_put_string(skb, IFLA_PHYS_PORT_NAME, name))
        return -EMSGSIZE;

    return 0;
}

static int rtnl_phys_switch_id_fill(struct sk_buff *skb, struct net_device *dev)
{
    struct netdev_phys_item_id ppid = { };
    int err;

    err = dev_get_port_parent_id(dev, &ppid, false);
    if (err) {
        if (err == -EOPNOTSUPP)
            return 0;
        return err;
    }

    if (nla_put(skb, IFLA_PHYS_SWITCH_ID, ppid.id_len, ppid.id))
        return -EMSGSIZE;

    return 0;
}

static noinline_for_stack int rtnl_fill_stats(struct sk_buff *skb,
                          struct net_device *dev)
{
    struct rtnl_link_stats64 *sp;
    struct nlattr *attr;

    attr = nla_reserve_64bit(skb, IFLA_STATS64,
                 sizeof(struct rtnl_link_stats64), IFLA_PAD);
    if (!attr)
        return -EMSGSIZE;

    sp = (struct rtnl_link_stats64 *)nla_data(attr);
    dev_get_stats(dev, sp);

    attr = nla_reserve(skb, IFLA_STATS,
               sizeof(struct rtnl_link_stats));
    if (!attr)
        return -EMSGSIZE;

//    copy_rtnl_link_stats(nla_data(attr), sp);

    return 0;
}

static noinline_for_stack int rtnl_fill_vf(struct sk_buff *skb,
                       struct net_device *dev,
                       u32 ext_filter_mask)
{
    struct nlattr *vfinfo;
    int i, num_vfs;

    if (!dev->dev.parent || ((ext_filter_mask & RTEXT_FILTER_VF) == 0))
        return 0;

//    num_vfs = dev_num_vf(dev->dev.parent);
//    if (nla_put_u32(skb, IFLA_NUM_VF, num_vfs))
//        return -EMSGSIZE;

    if (!dev->netdev_ops->ndo_get_vf_config)
        return 0;

    vfinfo = nla_nest_start_noflag(skb, IFLA_VFINFO_LIST);
    if (!vfinfo)
        return -EMSGSIZE;

//    for (i = 0; i < num_vfs; i++) {
//        if (rtnl_fill_vfinfo(skb, dev, i, vfinfo))
//            return -EMSGSIZE;
//    }

    nla_nest_end(skb, vfinfo);
    return 0;
}

static int rtnl_xdp_fill(struct sk_buff *skb, struct net_device *dev)
{
    struct nlattr *xdp;
    u32 prog_id;
    int err;
    u8 mode;

    xdp = nla_nest_start_noflag(skb, IFLA_XDP);
    if (!xdp)
        return -EMSGSIZE;

    prog_id = 0;
//    mode = XDP_ATTACHED_NONE;
//    err = rtnl_xdp_report_one(skb, dev, &prog_id, &mode, XDP_ATTACHED_SKB,
//                  IFLA_XDP_SKB_PROG_ID, rtnl_xdp_prog_skb);
//    if (err)
//        goto err_cancel;
//    err = rtnl_xdp_report_one(skb, dev, &prog_id, &mode, XDP_ATTACHED_DRV,
//                  IFLA_XDP_DRV_PROG_ID, rtnl_xdp_prog_drv);
//    if (err)
//        goto err_cancel;
//    err = rtnl_xdp_report_one(skb, dev, &prog_id, &mode, XDP_ATTACHED_HW,
//                  IFLA_XDP_HW_PROG_ID, rtnl_xdp_prog_hw);
//    if (err)
//        goto err_cancel;
//
//    err = nla_put_u8(skb, IFLA_XDP_ATTACHED, mode);
//    if (err)
//        goto err_cancel;
//
//    if (prog_id && mode != XDP_ATTACHED_MULTI) {
//        err = nla_put_u32(skb, IFLA_XDP_PROG_ID, prog_id);
//        if (err)
//            goto err_cancel;
//    }

    nla_nest_end(skb, xdp);
    return 0;

err_cancel:
    nla_nest_cancel(skb, xdp);
    return err;
}


static bool rtnl_have_link_slave_info(const struct net_device *dev)
{
    struct net_device *master_dev;
    bool ret = false;

    rcu_read_lock();

    master_dev = netdev_master_upper_dev_get_rcu((struct net_device *)dev);
    if (master_dev && master_dev->rtnl_link_ops)
        ret = true;
    rcu_read_unlock();
    return ret;
}

static int rtnl_link_info_fill(struct sk_buff *skb,
                   const struct net_device *dev)
{
    const struct rtnl_link_ops *ops = dev->rtnl_link_ops;
    struct nlattr *data;
    int err;

    if (!ops)
        return 0;
    if (nla_put_string(skb, IFLA_INFO_KIND, ops->kind) < 0)
        return -EMSGSIZE;
    if (ops->fill_xstats) {
        err = ops->fill_xstats(skb, dev);
        if (err < 0)
            return err;
    }
    if (ops->fill_info) {
        data = nla_nest_start_noflag(skb, IFLA_INFO_DATA);
        if (data == NULL)
            return -EMSGSIZE;
        err = ops->fill_info(skb, dev);
        if (err < 0)
            goto err_cancel_data;
        nla_nest_end(skb, data);
    }
    return 0;

err_cancel_data:
    nla_nest_cancel(skb, data);
    return err;
}

static int rtnl_link_slave_info_fill(struct sk_buff *skb,
                     const struct net_device *dev)
{
    struct net_device *master_dev;
    const struct rtnl_link_ops *ops;
    struct nlattr *slave_data;
    int err;

    master_dev = netdev_master_upper_dev_get((struct net_device *) dev);
    if (!master_dev)
        return 0;
    ops = master_dev->rtnl_link_ops;
    if (!ops)
        return 0;
    if (nla_put_string(skb, IFLA_INFO_SLAVE_KIND, ops->kind) < 0)
        return -EMSGSIZE;
    if (ops->fill_slave_info) {
        slave_data = nla_nest_start_noflag(skb, IFLA_INFO_SLAVE_DATA);
        if (!slave_data)
            return -EMSGSIZE;
        err = ops->fill_slave_info(skb, master_dev, dev);
        if (err < 0)
            goto err_cancel_slave_data;
        nla_nest_end(skb, slave_data);
    }
    return 0;

err_cancel_slave_data:
    nla_nest_cancel(skb, slave_data);
    return err;
}

static int rtnl_link_fill(struct sk_buff *skb, const struct net_device *dev)
{
    struct nlattr *linkinfo;
    int err = -EMSGSIZE;

    linkinfo = nla_nest_start_noflag(skb, IFLA_LINKINFO);
    if (linkinfo == NULL)
        goto out;

    err = rtnl_link_info_fill(skb, dev);
    if (err < 0)
        goto err_cancel_link;

    err = rtnl_link_slave_info_fill(skb, dev);
    if (err < 0)
        goto err_cancel_link;

    nla_nest_end(skb, linkinfo);
    return 0;

err_cancel_link:
    nla_nest_cancel(skb, linkinfo);
out:
    return err;
}

static int rtnl_fill_link_netnsid(struct sk_buff *skb,
                  const struct net_device *dev,
                  struct net *src_net, gfp_t gfp)
{
    bool put_iflink = false;

    if (dev->rtnl_link_ops && dev->rtnl_link_ops->get_link_net) {
        struct net *link_net = dev->rtnl_link_ops->get_link_net(dev);

//        if (!net_eq(dev_net(dev), link_net)) {
//            int id = peernet2id_alloc(src_net, link_net, gfp);
//
//            if (nla_put_s32(skb, IFLA_LINK_NETNSID, id))
//                return -EMSGSIZE;
//
//            put_iflink = true;
//        }
    }

    return nla_put_iflink(skb, dev, put_iflink);
}

static int rtnl_fill_link_af(struct sk_buff *skb,
                 const struct net_device *dev,
                 u32 ext_filter_mask)
{
    const struct rtnl_af_ops *af_ops;
    struct nlattr *af_spec;

    af_spec = nla_nest_start_noflag(skb, IFLA_AF_SPEC);
    if (!af_spec)
        return -EMSGSIZE;

    list_for_each_entry_rcu(af_ops, &rtnl_af_ops, list) {
        struct nlattr *af;
        int err;

        if (!af_ops->fill_link_af)
            continue;

        af = nla_nest_start_noflag(skb, af_ops->family);
        if (!af)
            return -EMSGSIZE;

        err = af_ops->fill_link_af(skb, dev, ext_filter_mask);
        /*
         * Caller may return ENODATA to indicate that there
         * was no data to be dumped. This is not an error, it
         * means we should trim the attribute header and
         * continue.
         */
        if (err == -ENODATA)
            nla_nest_cancel(skb, af);
        else if (err < 0)
            return -EMSGSIZE;

        nla_nest_end(skb, af);
    }

    nla_nest_end(skb, af_spec);
    return 0;
}

static int rtnl_fill_alt_ifnames(struct sk_buff *skb,
                 const struct net_device *dev)
{
    struct netdev_name_node *name_node;
    int count = 0;

    list_for_each_entry(name_node, &dev->name_node->list, list) {
        if (nla_put_string(skb, IFLA_ALT_IFNAME, name_node->name))
            return -EMSGSIZE;
        count++;
    }
    return count;
}


static int rtnl_fill_prop_list(struct sk_buff *skb,
                   const struct net_device *dev)
{
    struct nlattr *prop_list;
    int ret;

    prop_list = nla_nest_start(skb, IFLA_PROP_LIST);
    if (!prop_list)
        return -EMSGSIZE;

    ret = rtnl_fill_alt_ifnames(skb, dev);
    if (ret <= 0)
        goto nest_cancel;

    nla_nest_end(skb, prop_list);
    return 0;

nest_cancel:
    nla_nest_cancel(skb, prop_list);
    return ret;
}

static int rtnl_fill_ifinfo(struct sk_buff *skb,
                struct net_device *dev, struct net *src_net,
                int type, u32 pid, u32 seq, u32 change,
                unsigned int flags, u32 ext_filter_mask,
                u32 event, int *new_nsid, int new_ifindex,
                int tgt_netnsid, gfp_t gfp)
{
    struct ifinfomsg *ifm;
    struct nlmsghdr *nlh;

    ASSERT_RTNL();
    nlh = nlmsg_put(skb, pid, seq, type, sizeof(*ifm), flags);
    if (nlh == NULL)
        return -EMSGSIZE;

    ifm = (struct ifinfomsg *)nlmsg_data(nlh);
    ifm->ifi_family = AF_UNSPEC;
    ifm->__ifi_pad = 0;
    ifm->ifi_type = dev->type;
    ifm->ifi_index = dev->ifindex;
    ifm->ifi_flags = dev_get_flags(dev);
    ifm->ifi_change = change;

    if (tgt_netnsid >= 0 && nla_put_s32(skb, IFLA_TARGET_NETNSID, tgt_netnsid))
        goto nla_put_failure;

    if (nla_put_string(skb, IFLA_IFNAME, dev->name) ||
        nla_put_u32(skb, IFLA_TXQLEN, dev->tx_queue_len) ||
        nla_put_u8(skb, IFLA_OPERSTATE,
               netif_running(dev) ? dev->operstate : IF_OPER_DOWN) ||
        nla_put_u8(skb, IFLA_LINKMODE, dev->link_mode) ||
        nla_put_u32(skb, IFLA_MTU, dev->mtu) ||
        nla_put_u32(skb, IFLA_MIN_MTU, dev->min_mtu) ||
        nla_put_u32(skb, IFLA_MAX_MTU, dev->max_mtu) ||
        nla_put_u32(skb, IFLA_GROUP, dev->group) ||
        nla_put_u32(skb, IFLA_PROMISCUITY, dev->promiscuity) ||
        nla_put_u32(skb, IFLA_NUM_TX_QUEUES, dev->num_tx_queues) ||
        nla_put_u32(skb, IFLA_GSO_MAX_SEGS, dev->gso_max_segs) ||
        nla_put_u32(skb, IFLA_GSO_MAX_SIZE, dev->gso_max_size) ||
#ifdef CONFIG_RPS
        nla_put_u32(skb, IFLA_NUM_RX_QUEUES, dev->num_rx_queues) ||
#endif
        put_master_ifindex(skb, dev) ||
        nla_put_u8(skb, IFLA_CARRIER, netif_carrier_ok(dev)) ||
//        (dev->qdisc &&
//         nla_put_string(skb, IFLA_QDISC, dev->qdisc->ops->id)) ||
        nla_put_ifalias(skb, dev) ||
        nla_put_u32(skb, IFLA_CARRIER_CHANGES,
            atomic_read(&dev->carrier_up_count) +
            atomic_read(&dev->carrier_down_count)) ||
        nla_put_u8(skb, IFLA_PROTO_DOWN, dev->proto_down) ||
        nla_put_u32(skb, IFLA_CARRIER_UP_COUNT,
            atomic_read(&dev->carrier_up_count)) ||
        nla_put_u32(skb, IFLA_CARRIER_DOWN_COUNT,
            atomic_read(&dev->carrier_down_count)))
        goto nla_put_failure;

    if (event != IFLA_EVENT_NONE) {
        if (nla_put_u32(skb, IFLA_EVENT, event))
            goto nla_put_failure;
    }

    if (rtnl_fill_link_ifmap(skb, dev))
        goto nla_put_failure;

    if (dev->addr_len) {
        if (nla_put(skb, IFLA_ADDRESS, dev->addr_len, dev->dev_addr) ||
            nla_put(skb, IFLA_BROADCAST, dev->addr_len, dev->broadcast))
            goto nla_put_failure;
    }

    if (rtnl_phys_port_id_fill(skb, dev))
        goto nla_put_failure;

    if (rtnl_phys_port_name_fill(skb, dev))
        goto nla_put_failure;

    if (rtnl_phys_switch_id_fill(skb, dev))
        goto nla_put_failure;

    if (rtnl_fill_stats(skb, dev))
        goto nla_put_failure;

    if (rtnl_fill_vf(skb, dev, ext_filter_mask))
        goto nla_put_failure;

    if (rtnl_port_fill(skb, dev, ext_filter_mask))
        goto nla_put_failure;

    if (rtnl_xdp_fill(skb, dev))
        goto nla_put_failure;

    if (dev->rtnl_link_ops || rtnl_have_link_slave_info(dev)) {
        if (rtnl_link_fill(skb, dev) < 0)
            goto nla_put_failure;
    }

    if (rtnl_fill_link_netnsid(skb, dev, src_net, gfp))
        goto nla_put_failure;

    if (new_nsid &&
        nla_put_s32(skb, IFLA_NEW_NETNSID, *new_nsid) < 0)
        goto nla_put_failure;
    if (new_ifindex &&
        nla_put_s32(skb, IFLA_NEW_IFINDEX, new_ifindex) < 0)
        goto nla_put_failure;

//    if (memchr_inv(dev->perm_addr, '\0', dev->addr_len) &&
//        nla_put(skb, IFLA_PERM_ADDRESS, dev->addr_len, dev->perm_addr))
//        goto nla_put_failure;

    rcu_read_lock();
    if (rtnl_fill_link_af(skb, dev, ext_filter_mask))
        goto nla_put_failure_rcu;
    rcu_read_unlock();

    if (rtnl_fill_prop_list(skb, dev))
        goto nla_put_failure;

    nlmsg_end(skb, nlh);
    return 0;

nla_put_failure_rcu:
    rcu_read_unlock();
nla_put_failure:
    nlmsg_cancel(skb, nlh);
    return -EMSGSIZE;
}


struct sk_buff *rtmsg_ifinfo_build_skb(int type, struct net_device *dev,
                       unsigned int change,
                       u32 event, gfp_t flags, int *new_nsid,
                       int new_ifindex)
{
    struct net *net = dev_net(dev);
    struct sk_buff *skb;
    int err = -ENOBUFS;
    size_t if_info_size;

    skb = nlmsg_new((if_info_size = if_nlmsg_size(dev, 0)), flags);
    if (skb == NULL)
        goto errout;

    err = rtnl_fill_ifinfo(skb, dev, dev_net(dev),
                   type, 0, 0, change, 0, 0, event,
                   new_nsid, new_ifindex, -1, flags);
    if (err < 0) {
        /* -EMSGSIZE implies BUG in if_nlmsg_size() */
        WARN_ON(err == -EMSGSIZE);
        kfree_skb(skb);
        goto errout;
    }
    return skb;
errout:
//    if (err < 0)
//        rtnl_set_sk_err(net, RTNLGRP_LINK, err);
    return NULL;
}


static void rtmsg_ifinfo_event(int type, struct net_device *dev,
                   unsigned int change, u32 event,
                   gfp_t flags, int *new_nsid, int new_ifindex)
{
    struct sk_buff *skb;

    if (dev->reg_state != NETREG_REGISTERED)
        return;

    skb = rtmsg_ifinfo_build_skb(type, dev, change, event, flags, new_nsid,
                     new_ifindex);
    if (skb)
        rtmsg_ifinfo_send(skb, dev, flags);
}

static u32 rtnl_get_event(unsigned long event)
{
    u32 rtnl_event_type = IFLA_EVENT_NONE;

    switch (event) {
    case NETDEV_REBOOT:
        rtnl_event_type = IFLA_EVENT_REBOOT;
        break;
    case NETDEV_FEAT_CHANGE:
        rtnl_event_type = IFLA_EVENT_FEATURES;
        break;
    case NETDEV_BONDING_FAILOVER:
        rtnl_event_type = IFLA_EVENT_BONDING_FAILOVER;
        break;
    case NETDEV_NOTIFY_PEERS:
        rtnl_event_type = IFLA_EVENT_NOTIFY_PEERS;
        break;
    case NETDEV_RESEND_IGMP:
        rtnl_event_type = IFLA_EVENT_IGMP_RESEND;
        break;
    case NETDEV_CHANGEINFODATA:
        rtnl_event_type = IFLA_EVENT_BONDING_OPTIONS;
        break;
    default:
        break;
    }

    return rtnl_event_type;
}

void rtmsg_ifinfo(int type, struct net_device *dev, unsigned int change,
          gfp_t flags)
{
    rtmsg_ifinfo_event(type, dev, change, rtnl_get_event(0), flags,
               NULL, 0);
}


void rtmsg_ifinfo_send(struct sk_buff *skb, struct net_device *dev, gfp_t flags)
{
    struct net *net = dev_net(dev);
    
    rtnl_notify(skb, net, 0, RTNLGRP_LINK, NULL, flags);
}

void rtnl_notify(struct sk_buff *skb, struct net *net, u32 pid, u32 group,
                 struct nlmsghdr *nlh, gfp_t flags)
{
    struct sock *rtnl = net->rtnl;
    int report = 0;
    
    if (nlh)
        report = nlmsg_report(nlh);
    
    nlmsg_notify(rtnl, skb, pid, group, report, flags);
}
EXPORT_SYMBOL(rtnl_notify);

