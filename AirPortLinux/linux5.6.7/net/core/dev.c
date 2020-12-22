//
//  dev.c
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/22.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#include <linux/netdevice.h>
#include <linux/jhash.h>
#include <linux/rtnetlink.h>
#include <linux/ctype.h>

/**
 * eth_type_trans - determine the packet's protocol ID.
 * @skb: received socket data
 * @dev: receiving network device
 *
 * The rule here is that we
 * assume 802.3 if the type field is short enough to be a length.
 * This is normal practice and works for any 'now in use' protocol.
 */
__be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev)
{
    unsigned short _service_access_point;
    const unsigned short *sap;
    const struct ethhdr *eth;

    skb->dev = dev;
    skb_reset_mac_header(skb);

//    eth = (struct ethhdr *)skb->data;
//    skb_pull_inline(skb, ETH_HLEN);
//
//    if (unlikely(!ether_addr_equal_64bits(eth->h_dest,
//                          dev->dev_addr))) {
//        if (unlikely(is_multicast_ether_addr_64bits(eth->h_dest))) {
//            if (ether_addr_equal_64bits(eth->h_dest, dev->broadcast))
//                skb->pkt_type = PACKET_BROADCAST;
//            else
//                skb->pkt_type = PACKET_MULTICAST;
//        } else {
//            skb->pkt_type = PACKET_OTHERHOST;
//        }
//    }
//
//    /*
//     * Some variants of DSA tagging don't have an ethertype field
//     * at all, so we check here whether one of those tagging
//     * variants has been configured on the receiving interface,
//     * and if so, set skb->protocol without looking at the packet.
//     * The DSA tagging protocol may be able to decode some but not all
//     * traffic (for example only for management). In that case give it the
//     * option to filter the packets from which it can decode source port
//     * information.
//     */
//    if (unlikely(netdev_uses_dsa(dev)) && dsa_can_decode(skb, dev))
//        return htons(ETH_P_XDSA);
//
//    if (likely(eth_proto_is_802_3(eth->h_proto)))
//        return eth->h_proto;
//
//    /*
//     *      This is a magic hack to spot IPX packets. Older Novell breaks
//     *      the protocol design and runs IPX over 802.3 without an 802.2 LLC
//     *      layer. We look for FFFF which isn't a used 802.2 SSAP/DSAP. This
//     *      won't work for fault tolerant netware but does for the rest.
//     */
//    sap = skb_header_pointer(skb, 0, sizeof(*sap), &_service_access_point);
//    if (sap && *sap == 0xFFFF)
//        return htons(ETH_P_802_3);

    /*
     *      Real 802.2 LLC
     */
    return htons(ETH_P_802_2);
}
EXPORT_SYMBOL(eth_type_trans);



/**
 * dev_nit_active - return true if any network interface taps are in use
 *
 * @dev: network device to check for the presence of taps
 */
bool dev_nit_active(struct net_device *dev)
{
//    return !list_empty(&ptype_all) || !list_empty(&dev->ptype_all);
    return false;
}
EXPORT_SYMBOL_GPL(dev_nit_active);

/*
 *    Support routine. Sends outgoing frames to any network
 *    taps currently in use.
 */

void dev_queue_xmit_nit(struct sk_buff *skb, struct net_device *dev)
{
    struct packet_type *ptype;
    struct sk_buff *skb2 = NULL;
    struct packet_type *pt_prev = NULL;
//    struct list_head *ptype_list = &ptype_all;
//
//    rcu_read_lock();
//again:
//    list_for_each_entry_rcu(ptype, ptype_list, list) {
//        if (ptype->ignore_outgoing)
//            continue;
//
//        /* Never send packets back to the socket
//         * they originated from - MvS (miquels@drinkel.ow.org)
//         */
//        if (skb_loop_sk(ptype, skb))
//            continue;
//
//        if (pt_prev) {
//            deliver_skb(skb2, pt_prev, skb->dev);
//            pt_prev = ptype;
//            continue;
//        }
//
//        /* need to clone skb, done only once */
//        skb2 = skb_clone(skb, GFP_ATOMIC);
//        if (!skb2)
//            goto out_unlock;
//
//        net_timestamp_set(skb2);
//
//        /* skb->nh should be correctly
//         * set by sender, so that the second statement is
//         * just protection against buggy protocols.
//         */
//        skb_reset_mac_header(skb2);
//
//        if (skb_network_header(skb2) < skb2->data ||
//            skb_network_header(skb2) > skb_tail_pointer(skb2)) {
//            net_crit_ratelimited("protocol %04x is buggy, dev %s\n",
//                         ntohs(skb2->protocol),
//                         dev->name);
//            skb_reset_network_header(skb2);
//        }
//
//        skb2->transport_header = skb2->network_header;
//        skb2->pkt_type = PACKET_OUTGOING;
//        pt_prev = ptype;
//    }
//
//    if (ptype_list == &ptype_all) {
//        ptype_list = &dev->ptype_all;
//        goto again;
//    }
//out_unlock:
//    if (pt_prev) {
//        if (!skb_orphan_frags_rx(skb2, GFP_ATOMIC))
//            pt_prev->func(skb2, skb->dev, pt_prev, skb->dev);
//        else
//            kfree_skb(skb2);
//    }
//    rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(dev_queue_xmit_nit);


static int xmit_one(struct sk_buff *skb, struct net_device *dev,
            struct netdev_queue *txq, bool more)
{
    unsigned int len;
    int rc;

    if (dev_nit_active(dev))
        dev_queue_xmit_nit(skb, dev);

    len = skb->len;
//    trace_net_dev_start_xmit(skb, dev);
    rc = netdev_start_xmit(skb, dev, txq, more);
//    trace_net_dev_xmit(skb, rc, dev, len);

    return rc;
}

struct sk_buff *dev_hard_start_xmit(struct sk_buff *first, struct net_device *dev,
                    struct netdev_queue *txq, int *ret)
{
    struct sk_buff *skb = first;
    int rc = NETDEV_TX_OK;

    while (skb) {
        struct sk_buff *next = skb->next;

        skb_mark_not_on_list(skb);
        rc = xmit_one(skb, dev, txq, next != NULL);
        if (unlikely(!dev_xmit_complete(rc))) {
            skb->next = next;
            goto out;
        }

        skb = next;
        if (netif_tx_queue_stopped(txq) && skb) {
            rc = NETDEV_TX_BUSY;
            break;
        }
    }

out:
    *ret = rc;
    return skb;
}

/**
 *    __dev_queue_xmit - transmit a buffer
 *    @skb: buffer to transmit
 *    @sb_dev: suboordinate device used for L2 forwarding offload
 *
 *    Queue a buffer for transmission to a network device. The caller must
 *    have set the device and priority and built the buffer before calling
 *    this function. The function can be called from an interrupt.
 *
 *    A negative errno code is returned on a failure. A success does not
 *    guarantee the frame will be transmitted as it may be dropped due
 *    to congestion or traffic shaping.
 *
 * -----------------------------------------------------------------------------------
 *      I notice this method can also return errors from the queue disciplines,
 *      including NET_XMIT_DROP, which is a positive value.  So, errors can also
 *      be positive.
 *
 *      Regardless of the return value, the skb is consumed, so it is currently
 *      difficult to retry a send to this method.  (You can bump the ref count
 *      before sending to hold a reference for retry if you are careful.)
 *
 *      When calling this method, interrupts MUST be enabled.  This is because
 *      the BH enable code must have IRQs enabled so that it will not deadlock.
 *          --BLG
 */
static int __dev_queue_xmit(struct sk_buff *skb, struct net_device *sb_dev)
{
    struct net_device *dev = skb->dev;
    struct netdev_queue *txq;
    struct Qdisc *q;
    int rc = -ENOMEM;
    bool again = false;

    skb_reset_mac_header(skb);

//    if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_SCHED_TSTAMP))
//        __skb_tstamp_tx(skb, NULL, skb->sk, SCM_TSTAMP_SCHED);
//
//    /* Disable soft irqs for various locks below. Also
//     * stops preemption for RCU.
//     */
//    rcu_read_lock_bh();
//
//    skb_update_prio(skb);
//
//    qdisc_pkt_len_init(skb);
//#ifdef CONFIG_NET_CLS_ACT
//    skb->tc_at_ingress = 0;
//# ifdef CONFIG_NET_EGRESS
//    if (static_branch_unlikely(&egress_needed_key)) {
//        skb = sch_handle_egress(skb, &rc, dev);
//        if (!skb)
//            goto out;
//    }
//# endif
//#endif
//    /* If device/qdisc don't need skb->dst, release it right now while
//     * its hot in this cpu cache.
//     */
//    if (dev->priv_flags & IFF_XMIT_DST_RELEASE)
//        skb_dst_drop(skb);
//    else
//        skb_dst_force(skb);
//
//    txq = netdev_core_pick_tx(dev, skb, sb_dev);
//    q = rcu_dereference_bh(txq->qdisc);
//
//    trace_net_dev_queue(skb);
//    if (q->enqueue) {
//        rc = __dev_xmit_skb(skb, q, dev, txq);
//        goto out;
//    }

    /* The device has no queue. Common case for software devices:
     * loopback, all the sorts of tunnels...

     * Really, it is unlikely that netif_tx_lock protection is necessary
     * here.  (f.e. loopback and IP tunnels are clean ignoring statistics
     * counters.)
     * However, it is possible, that they rely on protection
     * made by us here.

     * Check this and shot the lock. It is not prone from deadlocks.
     *Either shot noqueue qdisc, it is even simpler 8)
     */
    if (dev->flags & IFF_UP) {
//        int cpu = smp_processor_id(); /* ok because BHs are off */
//
//        if (txq->xmit_lock_owner != cpu) {
//            if (dev_xmit_recursion())
//                goto recursion_alert;
//
//            skb = validate_xmit_skb(skb, dev, &again);
//            if (!skb)
//                goto out;
//
//            HARD_TX_LOCK(dev, txq, cpu);
//
//            if (!netif_xmit_stopped(txq)) {
//                dev_xmit_recursion_inc();
                skb = dev_hard_start_xmit(skb, dev, txq, &rc);
//                dev_xmit_recursion_dec();
//                if (dev_xmit_complete(rc)) {
//                    HARD_TX_UNLOCK(dev, txq);
//                    goto out;
//                }
//            }
//            HARD_TX_UNLOCK(dev, txq);
//            net_crit_ratelimited("Virtual device %s asks to queue packet!\n",
//                         dev->name);
//        } else {
//            /* Recursion is detected! It is possible,
//             * unfortunately
//             */
//recursion_alert:
//            net_crit_ratelimited("Dead loop on virtual device %s, fix it urgently!\n",
//                         dev->name);
//        }
    }

    rc = -ENETDOWN;
//    rcu_read_unlock_bh();
//
//    atomic_long_inc(&dev->tx_dropped);
//    kfree_skb_list(skb);
    return rc;
out:
//    rcu_read_unlock_bh();
    return rc;
}

int dev_queue_xmit(struct sk_buff *skb)
{
    return __dev_queue_xmit(skb, NULL);
}
EXPORT_SYMBOL(dev_queue_xmit);


static void netdev_init_one_queue(struct net_device *dev,
                  struct netdev_queue *queue, void *_unused)
{
    /* Initialize queue lock */
    spin_lock_init(&queue->_xmit_lock);
    lockdep_set_class(&queue->_xmit_lock, &dev->qdisc_xmit_lock_key);
    queue->xmit_lock_owner = -1;
    netdev_queue_numa_node_write(queue, NUMA_NO_NODE);
    queue->dev = dev;
#ifdef CONFIG_BQL
    dql_init(&queue->dql, HZ);
#endif
}

static int netif_alloc_netdev_queues(struct net_device *dev)
{
    unsigned int count = dev->num_tx_queues;
    struct netdev_queue *tx;
    size_t sz = count * sizeof(*tx);

    if (count < 1 || count > 0xffff)
        return -EINVAL;

    tx = (struct netdev_queue *)kvzalloc(sz, GFP_KERNEL | __GFP_RETRY_MAYFAIL);
    if (!tx)
        return -ENOMEM;

    dev->_tx = tx;

    netdev_for_each_tx_queue(dev, netdev_init_one_queue, NULL);
    spin_lock_init(&dev->tx_global_lock);

    return 0;
}

void netif_tx_stop_all_queues(struct net_device *dev)
{
    unsigned int i;
    
    for (i = 0; i < dev->num_tx_queues; i++) {
        struct netdev_queue *txq = netdev_get_tx_queue(dev, i);
        
        netif_tx_stop_queue(txq);
    }
}
EXPORT_SYMBOL(netif_tx_stop_all_queues);


#define REG_STATE_NEW        0x0
#define REG_STATE_REGISTERED    0x1
#define REG_STATE_UNREGISTERED    0x2
#define REG_STATE_UNUSED    0x3

enum xdp_mem_type {
    MEM_TYPE_PAGE_SHARED = 0, /* Split-page refcnt based model */
    MEM_TYPE_PAGE_ORDER0,     /* Orig XDP full page model */
    MEM_TYPE_PAGE_POOL,
    MEM_TYPE_ZERO_COPY,
    MEM_TYPE_MAX,
};

static void xdp_rxq_info_init(struct xdp_rxq_info *xdp_rxq)
{
    memset(xdp_rxq, 0, sizeof(*xdp_rxq));
}

static void mem_id_disconnect(int id)
{
//    struct xdp_mem_allocator *xa;
//
//    mutex_lock(&mem_id_lock);
//
//    xa = rhashtable_lookup_fast(mem_id_ht, &id, mem_id_rht_params);
//    if (!xa) {
//        mutex_unlock(&mem_id_lock);
//        WARN(1, "Request remove non-existing id(%d), driver bug?", id);
//        return;
//    }
//
//    trace_mem_disconnect(xa);
//
//    if (!rhashtable_remove_fast(mem_id_ht, &xa->node, mem_id_rht_params))
//        call_rcu(&xa->rcu, __xdp_mem_allocator_rcu_free);
//
//    mutex_unlock(&mem_id_lock);
}

void xdp_rxq_info_unreg_mem_model(struct xdp_rxq_info *xdp_rxq)
{
    struct xdp_mem_allocator *xa;
    int id = xdp_rxq->mem.id;

    if (xdp_rxq->reg_state != REG_STATE_REGISTERED) {
        WARN(1, "Missing register, driver bug");
        return;
    }

    if (id == 0)
        return;

    if (xdp_rxq->mem.type == MEM_TYPE_ZERO_COPY)
        return mem_id_disconnect(id);

    if (xdp_rxq->mem.type == MEM_TYPE_PAGE_POOL) {
        rcu_read_lock();
//        xa = rhashtable_lookup(mem_id_ht, &id, mem_id_rht_params);
//        page_pool_destroy(xa->page_pool);
        rcu_read_unlock();
    }
}
EXPORT_SYMBOL_GPL(xdp_rxq_info_unreg_mem_model);

void xdp_rxq_info_unreg(struct xdp_rxq_info *xdp_rxq)
{
    /* Simplify driver cleanup code paths, allow unreg "unused" */
    if (xdp_rxq->reg_state == REG_STATE_UNUSED)
        return;

    WARN(!(xdp_rxq->reg_state == REG_STATE_REGISTERED), "Driver BUG");

    xdp_rxq_info_unreg_mem_model(xdp_rxq);

    xdp_rxq->reg_state = REG_STATE_UNREGISTERED;
    xdp_rxq->dev = NULL;

    /* Reset mem info to defaults */
    xdp_rxq->mem.id = 0;
    xdp_rxq->mem.type = 0;
}
EXPORT_SYMBOL_GPL(xdp_rxq_info_unreg);

/* Returns 0 on success, negative on failure */
int xdp_rxq_info_reg(struct xdp_rxq_info *xdp_rxq,
             struct net_device *dev, u32 queue_index)
{
    if (xdp_rxq->reg_state == REG_STATE_UNUSED) {
        WARN(1, "Driver promised not to register this");
        return -EINVAL;
    }

    if (xdp_rxq->reg_state == REG_STATE_REGISTERED) {
        WARN(1, "Missing unregister, handled but fix driver");
        xdp_rxq_info_unreg(xdp_rxq);
    }

    if (!dev) {
        WARN(1, "Missing net_device from driver");
        return -ENODEV;
    }

    /* State either UNREGISTERED or NEW */
    xdp_rxq_info_init(xdp_rxq);
    xdp_rxq->dev = dev;
    xdp_rxq->queue_index = queue_index;

    xdp_rxq->reg_state = REG_STATE_REGISTERED;
    return 0;
}
EXPORT_SYMBOL_GPL(xdp_rxq_info_reg);


static int netif_alloc_rx_queues(struct net_device *dev)
{
    unsigned int i, count = dev->num_rx_queues;
    struct netdev_rx_queue *rx;
    size_t sz = count * sizeof(*rx);
    int err = 0;

    BUG_ON(count < 1);

    rx = (struct netdev_rx_queue *)kvzalloc(sz, GFP_KERNEL | __GFP_RETRY_MAYFAIL);
    if (!rx)
        return -ENOMEM;

    dev->_rx = rx;

    for (i = 0; i < count; i++) {
        rx[i].dev = dev;

        /* XDP RX-queue setup */
        err = xdp_rxq_info_reg(&rx[i].xdp_rxq, dev, i);
        if (err < 0)
            goto err_rxq_info;
    }
    return 0;

err_rxq_info:
    /* Rollback successful reg's and free other resources */
    while (i--)
        xdp_rxq_info_unreg(&rx[i].xdp_rxq);
    kvfree(dev->_rx);
    dev->_rx = NULL;
    return err;
}


static const struct ethtool_ops default_ethtool_ops = {};

void netdev_set_default_ethtool_ops(struct net_device *dev,
                    const struct ethtool_ops *ops)
{
    if (dev->ethtool_ops == &default_ethtool_ops)
        dev->ethtool_ops = ops;
}
EXPORT_SYMBOL_GPL(netdev_set_default_ethtool_ops);

void netdev_freemem(struct net_device *dev)
{
    char *addr = (char *)dev - dev->padded;
    
    kvfree(addr);
}


/**
 * alloc_netdev_mqs - allocate network device
 * @sizeof_priv: size of private data to allocate space for
 * @name: device name format string
 * @name_assign_type: origin of device name
 * @setup: callback to initialize device
 * @txqs: the number of TX subqueues to allocate
 * @rxqs: the number of RX subqueues to allocate
 *
 * Allocates a struct net_device with private data area for driver use
 * and performs basic initialization.  Also allocates subqueue structs
 * for each queue on the device.
 */
struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name,
        unsigned char name_assign_type,
        void (*setup)(struct net_device *),
        unsigned int txqs, unsigned int rxqs)
{
    struct net_device *dev;
    unsigned int alloc_size;
    struct net_device *p;

    BUG_ON(strlen(name) >= sizeof(dev->name));

    if (txqs < 1) {
        pr_err("alloc_netdev: Unable to allocate device with zero queues\n");
        return NULL;
    }

    if (rxqs < 1) {
        pr_err("alloc_netdev: Unable to allocate device with zero RX queues\n");
        return NULL;
    }

    alloc_size = sizeof(struct net_device);
    if (sizeof_priv) {
        /* ensure 32-byte alignment of private area */
        alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
        alloc_size += sizeof_priv;
    }
    /* ensure 32-byte alignment of whole construct */
    alloc_size += NETDEV_ALIGN - 1;

    p = (struct net_device *)kvzalloc(alloc_size, GFP_KERNEL | __GFP_RETRY_MAYFAIL);
    if (!p)
        return NULL;

    dev = PTR_ALIGN(p, NETDEV_ALIGN);
    dev->padded = (char *)dev - (char *)p;

    dev->pcpu_refcnt = (int *)alloc_percpu(int);
    if (!dev->pcpu_refcnt)
        goto free_dev;

    if (dev_addr_init(dev))
        goto free_pcpu;

    dev_mc_init(dev);
    dev_uc_init(dev);

    dev_net_set(dev, &init_net);

//    netdev_register_lockdep_key(dev);

    dev->gso_max_size = GSO_MAX_SIZE;
    dev->gso_max_segs = GSO_MAX_SEGS;
    dev->upper_level = 1;
    dev->lower_level = 1;

    INIT_LIST_HEAD(&dev->napi_list);
    INIT_LIST_HEAD(&dev->unreg_list);
    INIT_LIST_HEAD(&dev->close_list);
    INIT_LIST_HEAD(&dev->link_watch_list);
    INIT_LIST_HEAD(&dev->adj_list.upper);
    INIT_LIST_HEAD(&dev->adj_list.lower);
    INIT_LIST_HEAD(&dev->ptype_all);
    INIT_LIST_HEAD(&dev->ptype_specific);
    INIT_LIST_HEAD(&dev->net_notifier_list);
#ifdef CONFIG_NET_SCHED
    hash_init(dev->qdisc_hash);
#endif
    dev->priv_flags = IFF_XMIT_DST_RELEASE | IFF_XMIT_DST_RELEASE_PERM;
    setup(dev);

#define DEFAULT_TX_QUEUE_LEN    1000
    if (!dev->tx_queue_len) {
        dev->priv_flags |= IFF_NO_QUEUE;
        dev->tx_queue_len = DEFAULT_TX_QUEUE_LEN;
    }

    dev->num_tx_queues = txqs;
    dev->real_num_tx_queues = txqs;
    if (netif_alloc_netdev_queues(dev))
        goto free_all;

    dev->num_rx_queues = rxqs;
    dev->real_num_rx_queues = rxqs;
    if (netif_alloc_rx_queues(dev))
        goto free_all;

    strncpy(dev->name, name, IFNAMSIZ);
    dev->name_assign_type = name_assign_type;
    dev->group = INIT_NETDEV_GROUP;
    if (!dev->ethtool_ops)
        dev->ethtool_ops = &default_ethtool_ops;

//    nf_hook_ingress_init(dev);

    return dev;

free_all:
    free_netdev(dev);
    return NULL;

free_pcpu:
    free_percpu(dev->pcpu_refcnt);
free_dev:
    netdev_freemem(dev);
    return NULL;
}
EXPORT_SYMBOL(alloc_netdev_mqs);


static int __dev_alloc_name(struct net *net, const char *name, char *buf)
{
    int i = 0;
    const char *p;
    const int max_netdevices = 8*PAGE_SIZE;
    unsigned long *inuse;
    struct net_device *d;

    if (!dev_valid_name(name))
        return -EINVAL;

//    p = strchr(name, '%');
//    if (p) {
//        /*
//         * Verify the string as this thing may have come from
//         * the user.  There must be either one "%d" and no other "%"
//         * characters.
//         */
//        if (p[1] != 'd' || strchr(p + 2, '%'))
//            return -EINVAL;
//
//        /* Use one page as a bit array of possible slots */
//        inuse = (unsigned long *) get_zeroed_page(GFP_ATOMIC);
//        if (!inuse)
//            return -ENOMEM;
//
//        for_each_netdev(net, d) {
//            if (!sscanf(d->name, name, &i))
//                continue;
//            if (i < 0 || i >= max_netdevices)
//                continue;
//
//            /*  avoid cases where sscanf is not exact inverse of printf */
//            snprintf(buf, IFNAMSIZ, name, i);
//            if (!strncmp(buf, d->name, IFNAMSIZ))
//                set_bit(i, inuse);
//        }
//
//        i = find_first_zero_bit(inuse, max_netdevices);
//        free_page((unsigned long) inuse);
//    }

    snprintf(buf, IFNAMSIZ, name, i);
    if (!__dev_get_by_name(net, buf))
        return i;

    /* It is possible to run out of possible slots
     * when the name is long and there isn't enough space left
     * for the digits, or if all bits are used.
     */
    return -ENFILE;
}

static int dev_alloc_name_ns(struct net *net,
                 struct net_device *dev,
                 const char *name)
{
    char buf[IFNAMSIZ];
    int ret;

    BUG_ON(!net);
    ret = __dev_alloc_name(net, name, buf);
    if (ret >= 0)
        strlcpy(dev->name, buf, IFNAMSIZ);
    return ret;
}

int dev_alloc_name(struct net_device *dev, const char *name)
{
    return dev_alloc_name_ns(dev_net(dev), dev, name);
}
EXPORT_SYMBOL(dev_alloc_name);


/**
 *    dev_valid_name - check if name is okay for network device
 *    @name: name string
 *
 *    Network device names need to be valid file names to
 *    to allow sysfs to work.  We also disallow any kind of
 *    whitespace.
 */
bool dev_valid_name(const char *name)
{
    if (*name == '\0')
        return false;
    if (strnlen(name, IFNAMSIZ) == IFNAMSIZ)
        return false;
    if (!strcmp(name, ".") || !strcmp(name, ".."))
        return false;

    while (*name) {
        if (*name == '/' || *name == ':' || isspace(*name))
            return false;
        name++;
    }
    return true;
}
EXPORT_SYMBOL(dev_valid_name);


/* Return the hash of a string of known length */
unsigned int full_name_hash(const void *salt, const char *name, unsigned int len)
{
    unsigned long hash = init_name_hash(salt);
    while (len--)
        hash = partial_name_hash((unsigned char)*name++, hash);
    return end_name_hash(hash);
}
EXPORT_SYMBOL(full_name_hash);

static inline struct hlist_head *dev_name_hash(struct net *net, const char *name)
{
    unsigned int hash = full_name_hash(net, name, strnlen(name, IFNAMSIZ));

    return &net->dev_name_head[hash_32(hash, NETDEV_HASHBITS)];
}

static struct netdev_name_node *netdev_name_node_lookup(struct net *net,
                            const char *name)
{
    struct hlist_head *head = dev_name_hash(net, name);
    struct netdev_name_node *name_node;

    hlist_for_each_entry(name_node, head, hlist)
        if (!strcmp(name_node->name, name))
            return name_node;
    return NULL;
}

struct net_device *__dev_get_by_name(struct net *net, const char *name)
{
    struct netdev_name_node *node_name;

    node_name = netdev_name_node_lookup(net, name);
    return node_name ? node_name->dev : NULL;
}
EXPORT_SYMBOL(__dev_get_by_name);


static int dev_get_valid_name(struct net *net, struct net_device *dev,
                  const char *name)
{
    BUG_ON(!net);

    if (!dev_valid_name(name))
        return -EINVAL;

    if (strchr(name, '%'))
        return dev_alloc_name_ns(net, dev, name);
    else if (__dev_get_by_name(net, name))
        return -EEXIST;
    else if (dev->name != name)
        strlcpy(dev->name, name, IFNAMSIZ);

    return 0;
}


static inline struct hlist_head *dev_index_hash(struct net *net, int ifindex)
{
    return &net->dev_index_head[ifindex & (NETDEV_HASHENTRIES - 1)];
}

struct net_device *__dev_get_by_index(struct net *net, int ifindex)
{
    struct net_device *dev;
    struct hlist_head *head = dev_index_hash(net, ifindex);

    hlist_for_each_entry(dev, head, index_hlist)
        if (dev->ifindex == ifindex)
            return dev;

    return NULL;
}
EXPORT_SYMBOL(__dev_get_by_index);

static int dev_new_index(struct net *net)
{
   int ifindex = net->ifindex;

   for (;;) {
       if (++ifindex <= 0)
           ifindex = 1;
       if (!__dev_get_by_index(net, ifindex))
           return net->ifindex = ifindex;
   }
}

static netdev_features_t netdev_sync_upper_features(struct net_device *lower,
    struct net_device *upper, netdev_features_t features)
{
    netdev_features_t upper_disables = NETIF_F_UPPER_DISABLES;
    netdev_features_t feature;
    int feature_bit;

    for_each_netdev_feature(upper_disables, feature_bit) {
        feature = __NETIF_F_BIT(feature_bit);
        if (!(upper->wanted_features & feature)
            && (features & feature)) {
            netdev_dbg(lower, "Dropping feature %pNF, upper dev %s has it off.\n",
                   &feature, upper->name);
            features &= ~feature;
        }
    }

    return features;
}

static void netdev_sync_lower_features(struct net_device *upper,
    struct net_device *lower, netdev_features_t features)
{
    netdev_features_t upper_disables = NETIF_F_UPPER_DISABLES;
    netdev_features_t feature;
    int feature_bit;

    for_each_netdev_feature(upper_disables, feature_bit) {
        feature = __NETIF_F_BIT(feature_bit);
        if (!(features & feature) && (lower->features & feature)) {
            netdev_dbg(upper, "Disabling feature %pNF on lower dev %s.\n",
                   &feature, lower->name);
            lower->wanted_features &= ~feature;
            netdev_update_features(lower);

            if (unlikely(lower->features & feature))
                netdev_WARN(upper, "failed to disable %pNF on %s!\n",
                        &feature, lower->name);
        }
    }
}

static netdev_features_t netdev_fix_features(struct net_device *dev,
    netdev_features_t features)
{
    /* Fix illegal checksum combinations */
    if ((features & NETIF_F_HW_CSUM) &&
        (features & (NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM))) {
        netdev_warn(dev, "mixed HW and IP checksum settings.\n");
        features &= ~(NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM);
    }

    /* TSO requires that SG is present as well. */
    if ((features & NETIF_F_ALL_TSO) && !(features & NETIF_F_SG)) {
        netdev_dbg(dev, "Dropping TSO features since no SG feature.\n");
        features &= ~NETIF_F_ALL_TSO;
    }

    if ((features & NETIF_F_TSO) && !(features & NETIF_F_HW_CSUM) &&
                    !(features & NETIF_F_IP_CSUM)) {
        netdev_dbg(dev, "Dropping TSO features since no CSUM feature.\n");
        features &= ~NETIF_F_TSO;
        features &= ~NETIF_F_TSO_ECN;
    }

    if ((features & NETIF_F_TSO6) && !(features & NETIF_F_HW_CSUM) &&
                     !(features & NETIF_F_IPV6_CSUM)) {
        netdev_dbg(dev, "Dropping TSO6 features since no CSUM feature.\n");
        features &= ~NETIF_F_TSO6;
    }

    /* TSO with IPv4 ID mangling requires IPv4 TSO be enabled */
    if ((features & NETIF_F_TSO_MANGLEID) && !(features & NETIF_F_TSO))
        features &= ~NETIF_F_TSO_MANGLEID;

    /* TSO ECN requires that TSO is present as well. */
    if ((features & NETIF_F_ALL_TSO) == NETIF_F_TSO_ECN)
        features &= ~NETIF_F_TSO_ECN;

    /* Software GSO depends on SG. */
    if ((features & NETIF_F_GSO) && !(features & NETIF_F_SG)) {
        netdev_dbg(dev, "Dropping NETIF_F_GSO since no SG feature.\n");
        features &= ~NETIF_F_GSO;
    }

    /* GSO partial features require GSO partial be set */
    if ((features & dev->gso_partial_features) &&
        !(features & NETIF_F_GSO_PARTIAL)) {
        netdev_dbg(dev,
               "Dropping partially supported GSO features since no GSO partial.\n");
        features &= ~dev->gso_partial_features;
    }

    if (!(features & NETIF_F_RXCSUM)) {
        /* NETIF_F_GRO_HW implies doing RXCSUM since every packet
         * successfully merged by hardware must also have the
         * checksum verified by hardware.  If the user does not
         * want to enable RXCSUM, logically, we should disable GRO_HW.
         */
        if (features & NETIF_F_GRO_HW) {
            netdev_dbg(dev, "Dropping NETIF_F_GRO_HW since no RXCSUM feature.\n");
            features &= ~NETIF_F_GRO_HW;
        }
    }

    /* LRO/HW-GRO features cannot be combined with RX-FCS */
    if (features & NETIF_F_RXFCS) {
        if (features & NETIF_F_LRO) {
            netdev_dbg(dev, "Dropping LRO feature since RX-FCS is requested.\n");
            features &= ~NETIF_F_LRO;
        }

        if (features & NETIF_F_GRO_HW) {
            netdev_dbg(dev, "Dropping HW-GRO feature since RX-FCS is requested.\n");
            features &= ~NETIF_F_GRO_HW;
        }
    }

    return features;
}

struct netdev_adjacent {
    struct net_device *dev;

    /* upper master flag, there can only be one master device per list */
    bool master;

    /* lookup ignore flag */
    bool ignore;

    /* counter for the number of times this device was added to us */
    u16 ref_nr;

    /* private field for the users */
    void *_private;

    struct list_head list;
    struct rcu_head rcu;
};

/**
 * netdev_lower_get_next - Get the next device from the lower neighbour
 *                         list
 * @dev: device
 * @iter: list_head ** of the current position
 *
 * Gets the next netdev_adjacent from the dev's lower neighbour
 * list, starting from iter position. The caller must hold RTNL lock or
 * its own locking that guarantees that the neighbour lower
 * list will remain unchanged.
 */
void *netdev_lower_get_next(struct net_device *dev, struct list_head **iter)
{
    struct netdev_adjacent *lower;

    lower = list_entry(*iter, struct netdev_adjacent, list);

    if (&lower->list == &dev->adj_list.lower)
        return NULL;

    *iter = lower->list.next;

    return lower->dev;
}
EXPORT_SYMBOL(netdev_lower_get_next);


int __netdev_update_features(struct net_device *dev)
{
    struct net_device *upper, *lower;
    netdev_features_t features;
    struct list_head *iter;
    int err = -1;

    ASSERT_RTNL();

    features = netdev_get_wanted_features(dev);

    if (dev->netdev_ops->ndo_fix_features)
        features = dev->netdev_ops->ndo_fix_features(dev, features);

    /* driver might be less strict about feature dependencies */
    features = netdev_fix_features(dev, features);

    /* some features can't be enabled if they're off an an upper device */
    netdev_for_each_upper_dev_rcu(dev, upper, iter)
        features = netdev_sync_upper_features(dev, upper, features);

    if (dev->features == features)
        goto sync_lower;

    netdev_dbg(dev, "Features changed: %pNF -> %pNF\n",
        &dev->features, &features);

    if (dev->netdev_ops->ndo_set_features)
        err = dev->netdev_ops->ndo_set_features(dev, features);
    else
        err = 0;

    if (unlikely(err < 0)) {
        netdev_err(dev,
            "set_features() failed (%d); wanted %pNF, left %pNF\n",
            err, &features, &dev->features);
        /* return non-0 since some features might have changed and
         * it's better to fire a spurious notification than miss it
         */
        return -1;
    }

sync_lower:
    /* some features must be disabled on lower devices when disabled
     * on an upper device (think: bonding master or bridge)
     */
    netdev_for_each_lower_dev(dev, lower, iter)
        netdev_sync_lower_features(dev, lower, features);

    if (!err) {
        netdev_features_t diff = features ^ dev->features;

        if (diff & NETIF_F_RX_UDP_TUNNEL_PORT) {
            /* udp_tunnel_{get,drop}_rx_info both need
             * NETIF_F_RX_UDP_TUNNEL_PORT enabled on the
             * device, or they won't do anything.
             * Thus we need to update dev->features
             * *before* calling udp_tunnel_get_rx_info,
             * but *after* calling udp_tunnel_drop_rx_info.
             */
            if (features & NETIF_F_RX_UDP_TUNNEL_PORT) {
                dev->features = features;
//                udp_tunnel_get_rx_info(dev);
            } else {
//                udp_tunnel_drop_rx_info(dev);
            }
        }

        if (diff & NETIF_F_HW_VLAN_CTAG_FILTER) {
            if (features & NETIF_F_HW_VLAN_CTAG_FILTER) {
                dev->features = features;
//                err |= vlan_get_rx_ctag_filter_info(dev);
            } else {
//                vlan_drop_rx_ctag_filter_info(dev);
            }
        }

        if (diff & NETIF_F_HW_VLAN_STAG_FILTER) {
            if (features & NETIF_F_HW_VLAN_STAG_FILTER) {
                dev->features = features;
//                err |= vlan_get_rx_stag_filter_info(dev);
            } else {
//                vlan_drop_rx_stag_filter_info(dev);
            }
        }

        dev->features = features;
    }

    return err < 0 ? 0 : 1;
}

/**
 *    netdev_features_change - device changes features
 *    @dev: device to cause notification
 *
 *    Called to indicate a device has changed features.
 */
void netdev_features_change(struct net_device *dev)
{
    call_netdevice_notifiers(NETDEV_FEAT_CHANGE, dev);
}
EXPORT_SYMBOL(netdev_features_change);

/**
 *    netdev_update_features - recalculate device features
 *    @dev: the device to check
 *
 *    Recalculate dev->features set and send notifications if it
 *    has changed. Should be called after driver or hardware dependent
 *    conditions might have changed that influence the features.
 */
void netdev_update_features(struct net_device *dev)
{
    if (__netdev_update_features(dev))
        netdev_features_change(dev);
}
EXPORT_SYMBOL(netdev_update_features);


static struct netdev_name_node *netdev_name_node_alloc(struct net_device *dev,
                                                       const char *name)
{
    struct netdev_name_node *name_node;
    
    name_node = (struct netdev_name_node *)kmalloc(sizeof(*name_node), GFP_KERNEL);
    if (!name_node)
        return NULL;
    INIT_HLIST_NODE(&name_node->hlist);
    name_node->dev = dev;
    name_node->name = name;
    return name_node;
}

static struct netdev_name_node *
netdev_name_node_head_alloc(struct net_device *dev)
{
    struct netdev_name_node *name_node;
    
    name_node = netdev_name_node_alloc(dev, dev->name);
    if (!name_node)
        return NULL;
    INIT_LIST_HEAD(&name_node->list);
    return name_node;
}

static void netdev_name_node_free(struct netdev_name_node *name_node)
{
    kfree(name_node);
}

static void netdev_name_node_add(struct net *net,
                                 struct netdev_name_node *name_node)
{
    hlist_add_head_rcu(&name_node->hlist,
                       dev_name_hash(net, name_node->name));
}

static void netdev_name_node_del(struct netdev_name_node *name_node)
{
    hlist_del_rcu(&name_node->hlist);
}


/* Device list insertion */
static void list_netdevice(struct net_device *dev)
{
    struct net *net = dev_net(dev);

    ASSERT_RTNL();

//    write_lock_bh(&dev_base_lock);
    list_add_tail_rcu(&dev->dev_list, &net->dev_base_head);
    netdev_name_node_add(net, dev->name_node);
    hlist_add_head_rcu(&dev->index_hlist,
               dev_index_hash(net, dev->ifindex));
//    write_unlock_bh(&dev_base_lock);

//    dev_base_seq_inc(net);
}

#define NOTIFY_DONE        0x0000        /* Don't care */
#define NOTIFY_OK        0x0001        /* Suits me */
#define NOTIFY_STOP_MASK    0x8000        /* Don't call further */
#define NOTIFY_BAD        (NOTIFY_STOP_MASK|0x0002)
/* Bad/Veto action */

/* Restore (negative) errno value from notify return value. */
static inline int notifier_to_errno(int ret)
{
    ret &= ~NOTIFY_STOP_MASK;
    return ret > NOTIFY_OK ? NOTIFY_OK - ret : 0;
}


int register_netdevice(struct net_device *dev)
{
    int ret;
    struct net *net = dev_net(dev);

    BUILD_BUG_ON(sizeof(netdev_features_t) * BITS_PER_BYTE <
             NETDEV_FEATURE_COUNT);
//    BUG_ON(dev_boot_phase);
    ASSERT_RTNL();

    might_sleep();

    /* When net_device's are persistent, this will be fatal. */
    BUG_ON(dev->reg_state != NETREG_UNINITIALIZED);
    BUG_ON(!net);

    spin_lock_init(&dev->addr_list_lock);
    lockdep_set_class(&dev->addr_list_lock, &dev->addr_list_lock_key);

    ret = dev_get_valid_name(net, dev, dev->name);
    if (ret < 0)
        goto out;

    ret = -ENOMEM;
    dev->name_node = netdev_name_node_head_alloc(dev);
    if (!dev->name_node)
        goto out;

    /* Init, if this function is available */
    if (dev->netdev_ops->ndo_init) {
        ret = dev->netdev_ops->ndo_init(dev);
        if (ret) {
            if (ret > 0)
                ret = -EIO;
            goto err_free_name;
        }
    }

    if (((dev->hw_features | dev->features) &
         NETIF_F_HW_VLAN_CTAG_FILTER) &&
        (!dev->netdev_ops->ndo_vlan_rx_add_vid ||
         !dev->netdev_ops->ndo_vlan_rx_kill_vid)) {
        netdev_WARN(dev, "Buggy VLAN acceleration in driver!\n");
        ret = -EINVAL;
        goto err_uninit;
    }

    ret = -EBUSY;
    if (!dev->ifindex)
        dev->ifindex = dev_new_index(net);
    else if (__dev_get_by_index(net, dev->ifindex))
        goto err_uninit;

    /* Transfer changeable features to wanted_features and enable
     * software offloads (GSO and GRO).
     */
    dev->hw_features |= (NETIF_F_SOFT_FEATURES | NETIF_F_SOFT_FEATURES_OFF);
    dev->features |= NETIF_F_SOFT_FEATURES;

    if (dev->netdev_ops->ndo_udp_tunnel_add) {
        dev->features |= NETIF_F_RX_UDP_TUNNEL_PORT;
        dev->hw_features |= NETIF_F_RX_UDP_TUNNEL_PORT;
    }

    dev->wanted_features = dev->features & dev->hw_features;

    if (!(dev->flags & IFF_LOOPBACK))
        dev->hw_features |= NETIF_F_NOCACHE_COPY;

    /* If IPv4 TCP segmentation offload is supported we should also
     * allow the device to enable segmenting the frame with the option
     * of ignoring a static IP ID value.  This doesn't enable the
     * feature itself but allows the user to enable it later.
     */
    if (dev->hw_features & NETIF_F_TSO)
        dev->hw_features |= NETIF_F_TSO_MANGLEID;
    if (dev->vlan_features & NETIF_F_TSO)
        dev->vlan_features |= NETIF_F_TSO_MANGLEID;
    if (dev->mpls_features & NETIF_F_TSO)
        dev->mpls_features |= NETIF_F_TSO_MANGLEID;
    if (dev->hw_enc_features & NETIF_F_TSO)
        dev->hw_enc_features |= NETIF_F_TSO_MANGLEID;

    /* Make NETIF_F_HIGHDMA inheritable to VLAN devices.
     */
    dev->vlan_features |= NETIF_F_HIGHDMA;

    /* Make NETIF_F_SG inheritable to tunnel devices.
     */
    dev->hw_enc_features |= NETIF_F_SG | NETIF_F_GSO_PARTIAL;

    /* Make NETIF_F_SG inheritable to MPLS.
     */
    dev->mpls_features |= NETIF_F_SG;

    ret = call_netdevice_notifiers(NETDEV_POST_INIT, dev);
    ret = notifier_to_errno(ret);
    if (ret)
        goto err_uninit;

//    ret = netdev_register_kobject(dev);
//    if (ret) {
//        dev->reg_state = NETREG_UNREGISTERED;
//        goto err_uninit;
//    }
    dev->reg_state = NETREG_REGISTERED;

    __netdev_update_features(dev);

    /*
     *    Default initial state at registry is that the
     *    device is present.
     */

    set_bit(__LINK_STATE_PRESENT, &dev->state);

    linkwatch_init_dev(dev);

//    dev_init_scheduler(dev);
    dev_hold(dev);
    list_netdevice(dev);
//    add_device_randomness(dev->dev_addr, dev->addr_len);

    /* If the device has permanent device address, driver should
     * set dev_addr and also addr_assign_type should be set to
     * NET_ADDR_PERM (default value).
     */
    if (dev->addr_assign_type == NET_ADDR_PERM)
        memcpy(dev->perm_addr, dev->dev_addr, dev->addr_len);

    /* Notify protocols, that a new device appeared. */
    ret = call_netdevice_notifiers(NETDEV_REGISTER, dev);
    ret = notifier_to_errno(ret);
    if (ret) {
//        rollback_registered(dev);
//        rcu_barrier();

        dev->reg_state = NETREG_UNREGISTERED;
    }
    /*
     *    Prevent userspace races by waiting until the network
     *    device is fully setup before sending notifications.
     */
//    if (!dev->rtnl_link_ops ||
//        dev->rtnl_link_state == RTNL_LINK_INITIALIZED)
//        rtmsg_ifinfo(RTM_NEWLINK, dev, ~0U, GFP_KERNEL);

out:
    return ret;

err_uninit:
    if (dev->netdev_ops->ndo_uninit)
        dev->netdev_ops->ndo_uninit(dev);
    if (dev->priv_destructor)
        dev->priv_destructor(dev);
err_free_name:
//    netdev_name_node_free(dev->name_node);
    goto out;
}
EXPORT_SYMBOL(register_netdevice);


int register_netdevice_notifier(struct notifier_block *nb)
{
//    struct net *net;
//    int err;
//
//    /* Close race with setup_net() and cleanup_net() */
//    down_write(&pernet_ops_rwsem);
//    rtnl_lock();
//    err = raw_notifier_chain_register(&netdev_chain, nb);
//    if (err)
//        goto unlock;
//    if (dev_boot_phase)
//        goto unlock;
//    for_each_net(net) {
//        err = call_netdevice_register_net_notifiers(nb, net);
//        if (err)
//            goto rollback;
//    }
//
//unlock:
//    rtnl_unlock();
//    up_write(&pernet_ops_rwsem);
//    return err;
//
//rollback:
//    for_each_net_continue_reverse(net)
//        call_netdevice_unregister_net_notifiers(nb, net);
//
//    raw_notifier_chain_unregister(&netdev_chain, nb);
//    goto unlock;
    return 0;
}
EXPORT_SYMBOL(register_netdevice_notifier);

static void flush_gro_hash(struct napi_struct *napi)
{
    int i;
    
    for (i = 0; i < GRO_HASH_BUCKETS; i++) {
        struct sk_buff *skb, *n;
        
        list_for_each_entry_safe(skb, n, &napi->gro_hash[i].list, list)
        kfree_skb(skb);
        napi->gro_hash[i].count = 0;
    }
}

/* Warning : caller is responsible to make sure rcu grace period
 * is respected before freeing memory containing @napi
 */
bool napi_hash_del(struct napi_struct *napi)
{
    bool rcu_sync_needed = false;
    
//    spin_lock(&napi_hash_lock);
    
    if (test_and_clear_bit(NAPI_STATE_HASHED, &napi->state)) {
        rcu_sync_needed = true;
        hlist_del_rcu(&napi->napi_hash_node);
    }
//    spin_unlock(&napi_hash_lock);
    return rcu_sync_needed;
}
EXPORT_SYMBOL_GPL(napi_hash_del);


/* Must be called in process context */
void netif_napi_del(struct napi_struct *napi)
{
    might_sleep();
    if (napi_hash_del(napi))
        synchronize_net();
    list_del_init(&napi->dev_list);
    napi_free_frags(napi);
    
    flush_gro_hash(napi);
    napi->gro_bitmask = 0;
}
EXPORT_SYMBOL(netif_napi_del);


void free_netdev(struct net_device *dev)
{
    struct napi_struct *p, *n;
    
    might_sleep();
//    netif_free_tx_queues(dev);
//    netif_free_rx_queues(dev);
    
    kfree(rcu_dereference_protected(dev->ingress_queue, 1));
    
    /* Flush device addresses */
    dev_addr_flush(dev);
    
    list_for_each_entry_safe(p, n, &dev->napi_list, dev_list)
    netif_napi_del(p);
    
    free_percpu(dev->pcpu_refcnt);
    dev->pcpu_refcnt = NULL;
    free_percpu(dev->xdp_bulkq);
    dev->xdp_bulkq = NULL;
    
//    netdev_unregister_lockdep_key(dev);
    
    /*  Compatibility with error handling in drivers */
    if (dev->reg_state == NETREG_UNINITIALIZED) {
        netdev_freemem(dev);
        return;
    }
    
    BUG_ON(dev->reg_state != NETREG_UNREGISTERED);
    dev->reg_state = NETREG_RELEASED;
    
    /* will free via device release */
//    put_device(&dev->dev);
}
EXPORT_SYMBOL(free_netdev);

/**
 *    synchronize_net -  Synchronize with packet receive processing
 *
 *    Wait for packets currently being received to be done.
 *    Does not block later packets from starting.
 */
void synchronize_net(void)
{
//    might_sleep();
//    if (rtnl_is_locked())
//        synchronize_rcu_expedited();
//    else
        synchronize_rcu();
}
EXPORT_SYMBOL(synchronize_net);

void synchronize_rcu(void)
{
    might_sleep();
    //    if (rtnl_is_locked())
    //        synchronize_rcu_expedited();
    //    else
    //        synchronize_rcu();
}



/* Delayed registration/unregisteration */
static LIST_HEAD(net_todo_list);
//DECLARE_WAIT_QUEUE_HEAD(netdev_unregistering_wq);

static void net_set_todo(struct net_device *dev)
{
    list_add_tail(&dev->todo_list, &net_todo_list);
    dev_net(dev)->dev_unreg_count++;
}

static void __netdev_name_node_alt_destroy(struct netdev_name_node *name_node)
{
    list_del(&name_node->list);
    netdev_name_node_del(name_node);
    kfree(name_node->name);
    netdev_name_node_free(name_node);
}

int netdev_name_node_alt_destroy(struct net_device *dev, const char *name)
{
    struct netdev_name_node *name_node;
    struct net *net = dev_net(dev);
    
    name_node = netdev_name_node_lookup(net, name);
    if (!name_node)
        return -ENOENT;
    /* lookup might have found our primary name or a name belonging
     * to another device.
     */
    if (name_node == dev->name_node || name_node->dev != dev)
        return -EINVAL;
    
    __netdev_name_node_alt_destroy(name_node);
    
    return 0;
}
EXPORT_SYMBOL(netdev_name_node_alt_destroy);

static void netdev_name_node_alt_flush(struct net_device *dev)
{
    struct netdev_name_node *name_node, *tmp;
    
    list_for_each_entry_safe(name_node, tmp, &dev->name_node->list, list)
    __netdev_name_node_alt_destroy(name_node);
}

/**
 * netdev_has_any_upper_dev - Check if device is linked to some device
 * @dev: device
 *
 * Find out if a device is linked to an upper device and return true in case
 * it is. The caller must hold the RTNL lock.
 */
bool netdev_has_any_upper_dev(struct net_device *dev)
{
    ASSERT_RTNL();
    
    return !list_empty(&dev->adj_list.upper);
}
EXPORT_SYMBOL(netdev_has_any_upper_dev);

/**
 * netdev_has_any_lower_dev - Check if device is linked to some device
 * @dev: device
 *
 * Find out if a device is linked to a lower device and return true in case
 * it is. The caller must hold the RTNL lock.
 */
static bool netdev_has_any_lower_dev(struct net_device *dev)
{
    ASSERT_RTNL();
    
    return !list_empty(&dev->adj_list.lower);
}

/**
 * netdev_upper_get_next_dev_rcu - Get the next dev from upper list
 * @dev: device
 * @iter: list_head ** of the current position
 *
 * Gets the next device from the dev's upper list, starting from iter
 * position. The caller must hold RCU read lock.
 */
struct net_device *netdev_upper_get_next_dev_rcu(struct net_device *dev,
                                                 struct list_head **iter)
{
    struct netdev_adjacent *upper;
    
//    WARN_ON_ONCE(!rcu_read_lock_held() && !lockdep_rtnl_is_held());
    
    upper = list_entry((*iter)->next, struct netdev_adjacent, list);
    
    if (&upper->list == &dev->adj_list.upper)
        return NULL;
    
    *iter = &upper->list;
    
    return upper->dev;
}
EXPORT_SYMBOL(netdev_upper_get_next_dev_rcu);


static void rollback_registered_many(struct list_head *head)
{
    struct net_device *dev, *tmp;
    LIST_HEAD(close_head);
    
//    BUG_ON(dev_boot_phase);
    ASSERT_RTNL();
    
    list_for_each_entry_safe(dev, tmp, head, unreg_list) {
        /* Some devices call without registering
         * for initialization unwind. Remove those
         * devices and proceed with the remaining.
         */
        if (dev->reg_state == NETREG_UNINITIALIZED) {
            pr_debug("unregister_netdevice: device %s/%p never was registered\n",
                     dev->name, dev);
            
            WARN_ON(1);
            list_del(&dev->unreg_list);
            continue;
        }
        dev->dismantle = true;
        BUG_ON(dev->reg_state != NETREG_REGISTERED);
    }
    
    /* If device is running, close it first. */
    list_for_each_entry(dev, head, unreg_list)
    list_add_tail(&dev->close_list, &close_head);
    dev_close_many(&close_head, true);
    
    list_for_each_entry(dev, head, unreg_list) {
        /* And unlink it from device chain. */
//        unlist_netdevice(dev);
        
        dev->reg_state = NETREG_UNREGISTERING;
    }
//    flush_all_backlogs();
    
    synchronize_net();
    
    list_for_each_entry(dev, head, unreg_list) {
        struct sk_buff *skb = NULL;
        
        /* Shutdown queueing discipline. */
//        dev_shutdown(dev);
//
//        dev_xdp_uninstall(dev);
        
        /* Notify protocols, that we are about to destroy
         * this device. They should clean all the things.
         */
        call_netdevice_notifiers(NETDEV_UNREGISTER, dev);
        
//        if (!dev->rtnl_link_ops ||
//            dev->rtnl_link_state == RTNL_LINK_INITIALIZED)
//            skb = rtmsg_ifinfo_build_skb(RTM_DELLINK, dev, ~0U, 0,
//                                         GFP_KERNEL, NULL, 0);
        
        /*
         *    Flush the unicast and multicast chains
         */
        dev_uc_flush(dev);
        dev_mc_flush(dev);
        
        netdev_name_node_alt_flush(dev);
        netdev_name_node_free(dev->name_node);
        
        if (dev->netdev_ops->ndo_uninit)
            dev->netdev_ops->ndo_uninit(dev);
        
//        if (skb)
//            rtmsg_ifinfo_send(skb, dev, GFP_KERNEL);
        
        /* Notifier chain MUST detach us all upper devices. */
        WARN_ON(netdev_has_any_upper_dev(dev));
        WARN_ON(netdev_has_any_lower_dev(dev));
        
        /* Remove entries from kobject tree */
//        netdev_unregister_kobject(dev);
#ifdef CONFIG_XPS
        /* Remove XPS queueing entries */
        netif_reset_xps_queues_gt(dev, 0);
#endif
    }
    
    synchronize_net();
    
    list_for_each_entry(dev, head, unreg_list)
    dev_put(dev);
}

static void rollback_registered(struct net_device *dev)
{
    LIST_HEAD(single);
    
    list_add(&dev->unreg_list, &single);
    rollback_registered_many(&single);
    list_del(&single);
}

/**
 *    unregister_netdevice_queue - remove device from the kernel
 *    @dev: device
 *    @head: list
 *
 *    This function shuts down a device interface and removes it
 *    from the kernel tables.
 *    If head not NULL, device is queued to be unregistered later.
 *
 *    Callers must hold the rtnl semaphore.  You may want
 *    unregister_netdev() instead of this.
 */

void unregister_netdevice_queue(struct net_device *dev, struct list_head *head)
{
    ASSERT_RTNL();
    
    if (head) {
        list_move_tail(&dev->unreg_list, head);
    } else {
        rollback_registered(dev);
        /* Finish processing unregister after unlock */
        net_set_todo(dev);
    }
}
EXPORT_SYMBOL(unregister_netdevice_queue);


static void dev_change_rx_flags(struct net_device *dev, int flags)
{
    const struct net_device_ops *ops = dev->netdev_ops;
    
    if (ops->ndo_change_rx_flags)
        ops->ndo_change_rx_flags(dev, flags);
}


/**
 *    call_netdevice_notifiers_info - call all network notifier blocks
 *    @val: value passed unmodified to notifier function
 *    @info: notifier information data
 *
 *    Call all network notifier blocks.  Parameters and return value
 *    are as for raw_notifier_call_chain().
 */

static int call_netdevice_notifiers_info(unsigned long val,
                                         struct netdev_notifier_info *info)
{
    //    struct net *net = dev_net(info->dev);
    //    int ret;
    //
    //    ASSERT_RTNL();
    //
    //    /* Run per-netns notifier block chain first, then run the global one.
    //     * Hopefully, one day, the global one is going to be removed after
    //     * all notifier block registrators get converted to be per-netns.
    //     */
    //    ret = raw_notifier_call_chain(&net->netdev_chain, val, info);
    //    if (ret & NOTIFY_STOP_MASK)
    //        return ret;
    //    return raw_notifier_call_chain(&netdev_chain, val, info);
    return 0;
}


void __dev_notify_flags(struct net_device *dev, unsigned int old_flags,
                        unsigned int gchanges)
{
    unsigned int changes = dev->flags ^ old_flags;
    
//    if (gchanges)
//        rtmsg_ifinfo(RTM_NEWLINK, dev, gchanges, GFP_ATOMIC);
    
    if (changes & IFF_UP) {
        if (dev->flags & IFF_UP)
            call_netdevice_notifiers(NETDEV_UP, dev);
        else
            call_netdevice_notifiers(NETDEV_DOWN, dev);
    }
    
    if (dev->flags & IFF_UP &&
        (changes & ~(IFF_UP | IFF_PROMISC | IFF_ALLMULTI))) {
        struct netdev_notifier_change_info change_info = {
            .info = {
                .dev = dev,
            },
            .flags_changed = changes,
        };
        
        call_netdevice_notifiers_info(NETDEV_CHANGE, &change_info.info);
    }
}

static int __dev_set_promiscuity(struct net_device *dev, int inc, bool notify)
{
    unsigned int old_flags = dev->flags;
    kuid_t uid;
//    kgid_t gid;
    
    ASSERT_RTNL();
    
    dev->flags |= IFF_PROMISC;
    dev->promiscuity += inc;
    if (dev->promiscuity == 0) {
        /*
         * Avoid overflow.
         * If inc causes overflow, untouch promisc and return error.
         */
        if (inc < 0)
            dev->flags &= ~IFF_PROMISC;
        else {
            dev->promiscuity -= inc;
            pr_warn("%s: promiscuity touches roof, set promiscuity failed. promiscuity feature of device might be broken.\n",
                    dev->name);
            return -EOVERFLOW;
        }
    }
    if (dev->flags != old_flags) {
        pr_info("device %s %s promiscuous mode\n",
                dev->name,
                dev->flags & IFF_PROMISC ? "entered" : "left");
//        if (audit_enabled) {
//            current_uid_gid(&uid, &gid);
//            audit_log(audit_context(), GFP_ATOMIC,
//                      AUDIT_ANOM_PROMISCUOUS,
//                      "dev=%s prom=%d old_prom=%d auid=%u uid=%u gid=%u ses=%u",
//                      dev->name, (dev->flags & IFF_PROMISC),
//                      (old_flags & IFF_PROMISC),
//                      from_kuid(&init_user_ns, audit_get_loginuid(current)),
//                      from_kuid(&init_user_ns, uid),
//                      from_kgid(&init_user_ns, gid),
//                      audit_get_sessionid(current));
//        }
        
        dev_change_rx_flags(dev, IFF_PROMISC);
    }
    if (notify)
        __dev_notify_flags(dev, old_flags, IFF_PROMISC);
    return 0;
}

/*
 *    Upload unicast and multicast address lists to device and
 *    configure RX filtering. When the device doesn't support unicast
 *    filtering it is put in promiscuous mode while unicast addresses
 *    are present.
 */
void __dev_set_rx_mode(struct net_device *dev)
{
    const struct net_device_ops *ops = dev->netdev_ops;
    
    /* dev_open will call this function so the list will stay sane. */
    if (!(dev->flags&IFF_UP))
        return;
    
    if (!netif_device_present(dev))
        return;
    
    if (!(dev->priv_flags & IFF_UNICAST_FLT)) {
        /* Unicast addresses changes may only happen under the rtnl,
         * therefore calling __dev_set_promiscuity here is safe.
         */
        if (!netdev_uc_empty(dev) && !dev->uc_promisc) {
            __dev_set_promiscuity(dev, 1, false);
            dev->uc_promisc = true;
        } else if (netdev_uc_empty(dev) && dev->uc_promisc) {
            __dev_set_promiscuity(dev, -1, false);
            dev->uc_promisc = false;
        }
    }
    
    if (ops->ndo_set_rx_mode)
        ops->ndo_set_rx_mode(dev);
}

void dev_set_rx_mode(struct net_device *dev)
{
    netif_addr_lock_bh(dev);
    __dev_set_rx_mode(dev);
    netif_addr_unlock_bh(dev);
}


void __netdev_watchdog_up(struct net_device *dev)
{
    if (dev->netdev_ops->ndo_tx_timeout) {
        if (dev->watchdog_timeo <= 0)
            dev->watchdog_timeo = 5*HZ;
        if (!mod_timer(&dev->watchdog_timer,
                       round_jiffies(jiffies + dev->watchdog_timeo)))
            dev_hold(dev);
    }
}

static void dev_watchdog_up(struct net_device *dev)
{
    __netdev_watchdog_up(dev);
}

void dev_activate(struct net_device *dev)
{
    int need_watchdog;
    
    /* No queueing discipline is attached to device;
     * create default one for devices, which need queueing
     * and noqueue_qdisc for virtual interfaces
     */
    
//    if (dev->qdisc == &noop_qdisc)
//        attach_default_qdiscs(dev);
    
    if (!netif_carrier_ok(dev))
    /* Delay activation until next carrier-on event */
        return;
    
    need_watchdog = 0;
//    netdev_for_each_tx_queue(dev, transition_one_qdisc, &need_watchdog);
//    if (dev_ingress_queue(dev))
//        transition_one_qdisc(dev, dev_ingress_queue(dev), NULL);
    
    if (need_watchdog) {
        netif_trans_update(dev);
        dev_watchdog_up(dev);
    }
}
EXPORT_SYMBOL(dev_activate);


static int __dev_open(struct net_device *dev, struct netlink_ext_ack *extack)
{
    const struct net_device_ops *ops = dev->netdev_ops;
    int ret;
    
    ASSERT_RTNL();
    
    if (!netif_device_present(dev))
        return -ENODEV;
    
    /* Block netpoll from trying to do any rx path servicing.
     * If we don't do this there is a chance ndo_poll_controller
     * or ndo_poll may be running while we open the device
     */
//    netpoll_poll_disable(dev);
//
//    ret = call_netdevice_notifiers_extack(NETDEV_PRE_UP, dev, extack);
//    ret = notifier_to_errno(ret);
//    if (ret)
//        return ret;
    
    set_bit(__LINK_STATE_START, &dev->state);
    
    if (ops->ndo_validate_addr)
        ret = ops->ndo_validate_addr(dev);
    
    if (!ret && ops->ndo_open)
        ret = ops->ndo_open(dev);
    
//    netpoll_poll_enable(dev);
    
    if (ret)
        clear_bit(__LINK_STATE_START, &dev->state);
    else {
        dev->flags |= IFF_UP;
        dev_set_rx_mode(dev);
        dev_activate(dev);
//        add_device_randomness(dev->dev_addr, dev->addr_len);
    }
    
    return ret;
}

/**
 *    dev_open    - prepare an interface for use.
 *    @dev: device to open
 *    @extack: netlink extended ack
 *
 *    Takes a device from down to up state. The device's private open
 *    function is invoked and then the multicast lists are loaded. Finally
 *    the device is moved into the up state and a %NETDEV_UP message is
 *    sent to the netdev notifier chain.
 *
 *    Calling this function on an active interface is a nop. On a failure
 *    a negative errno code is returned.
 */
int dev_open(struct net_device *dev, struct netlink_ext_ack *extack)
{
    int ret;
    
    if (dev->flags & IFF_UP)
        return 0;
    
    ret = __dev_open(dev, extack);
    if (ret < 0)
        return ret;
    
//    rtmsg_ifinfo(RTM_NEWLINK, dev, IFF_UP|IFF_RUNNING, GFP_KERNEL);
    call_netdevice_notifiers(NETDEV_UP, dev);
    
    return ret;
}
EXPORT_SYMBOL(dev_open);

static void __dev_close_many(struct list_head *head)
{
    struct net_device *dev;
    
    ASSERT_RTNL();
    might_sleep();
    
    list_for_each_entry(dev, head, close_list) {
        /* Temporarily disable netpoll until the interface is down */
//        netpoll_poll_disable(dev);
        
        call_netdevice_notifiers(NETDEV_GOING_DOWN, dev);
        
        clear_bit(__LINK_STATE_START, &dev->state);
        
        /* Synchronize to scheduled poll. We cannot touch poll list, it
         * can be even on different cpu. So just clear netif_running().
         *
         * dev->stop() will invoke napi_disable() on all of it's
         * napi_struct instances on this device.
         */
//        smp_mb__after_atomic(); /* Commit netif_running(). */
    }
    
//    dev_deactivate_many(head);
    
    list_for_each_entry(dev, head, close_list) {
        const struct net_device_ops *ops = dev->netdev_ops;
        
        /*
         *    Call the device specific close. This cannot fail.
         *    Only if device is UP
         *
         *    We allow it to be called even after a DETACH hot-plug
         *    event.
         */
        if (ops->ndo_stop)
            ops->ndo_stop(dev);
        
        dev->flags &= ~IFF_UP;
//        netpoll_poll_enable(dev);
    }
}

static void __dev_close(struct net_device *dev)
{
    LIST_HEAD(single);
    
    list_add(&dev->close_list, &single);
    __dev_close_many(&single);
    list_del(&single);
}

void dev_close_many(struct list_head *head, bool unlink)
{
    struct net_device *dev, *tmp;
    
    /* Remove the devices that don't need to be closed */
    list_for_each_entry_safe(dev, tmp, head, close_list)
    if (!(dev->flags & IFF_UP))
        list_del_init(&dev->close_list);
    
    __dev_close_many(head);
    
    list_for_each_entry_safe(dev, tmp, head, close_list) {
//        rtmsg_ifinfo(RTM_NEWLINK, dev, IFF_UP|IFF_RUNNING, GFP_KERNEL);
        call_netdevice_notifiers(NETDEV_DOWN, dev);
        if (unlink)
            list_del_init(&dev->close_list);
    }
}
EXPORT_SYMBOL(dev_close_many);

/**
 *    dev_change_tx_queue_len - Change TX queue length of a netdevice
 *    @dev: device
 *    @new_len: new tx queue length
 */
int dev_change_tx_queue_len(struct net_device *dev, unsigned long new_len)
{
    unsigned int orig_len = dev->tx_queue_len;
    int res;
    
    if (new_len != (unsigned int)new_len)
        return -ERANGE;
    
    if (new_len != orig_len) {
        dev->tx_queue_len = new_len;
        res = call_netdevice_notifiers(NETDEV_CHANGE_TX_QUEUE_LEN, dev);
//        res = notifier_to_errno(res);
//        if (res)
//            goto err_rollback;
//        res = dev_qdisc_change_tx_queue_len(dev);
//        if (res)
//            goto err_rollback;
    }
    
    return 0;
    
err_rollback:
    netdev_err(dev, "refused to change device tx_queue_len\n");
    dev->tx_queue_len = orig_len;
    return res;
}

/**
 *    dev_set_group - Change group this device belongs to
 *    @dev: device
 *    @new_group: group this device should belong to
 */
void dev_set_group(struct net_device *dev, int new_group)
{
    dev->group = new_group;
}
EXPORT_SYMBOL(dev_set_group);


/**
 *    dev_pre_changeaddr_notify - Call NETDEV_PRE_CHANGEADDR.
 *    @dev: device
 *    @addr: new address
 *    @extack: netlink extended ack
 */
int dev_pre_changeaddr_notify(struct net_device *dev, const char *addr,
                              struct netlink_ext_ack *extack)
{
    struct netdev_notifier_pre_changeaddr_info info = {
        .info.dev = dev,
        .info.extack = extack,
        .dev_addr = (const unsigned char *)addr,
    };
    int rc;
    
    rc = call_netdevice_notifiers_info(NETDEV_PRE_CHANGEADDR, &info.info);
    return notifier_to_errno(rc);
    
}
EXPORT_SYMBOL(dev_pre_changeaddr_notify);

/**
 *    dev_set_mac_address - Change Media Access Control Address
 *    @dev: device
 *    @sa: new address
 *    @extack: netlink extended ack
 *
 *    Change the hardware (MAC) address of the device
 */
int dev_set_mac_address(struct net_device *dev, struct sockaddr *sa,
                        struct netlink_ext_ack *extack)
{
    const struct net_device_ops *ops = dev->netdev_ops;
    int err;
    
    if (!ops->ndo_set_mac_address)
        return -EOPNOTSUPP;
    if (sa->sa_family != dev->type)
        return -EINVAL;
    if (!netif_device_present(dev))
        return -ENODEV;
    err = dev_pre_changeaddr_notify(dev, sa->sa_data, extack);
    if (err)
        return err;
    err = ops->ndo_set_mac_address(dev, sa);
    if (err)
        return err;
    dev->addr_assign_type = NET_ADDR_SET;
    call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
//    add_device_randomness(dev->dev_addr, dev->addr_len);
    return 0;
}
EXPORT_SYMBOL(dev_set_mac_address);



gro_result_t napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
    gro_result_t ret;
    
//    skb_mark_napi_id(skb, napi);
//    trace_napi_gro_receive_entry(skb);
//    
//    skb_gro_reset_offset(skb);
//    
//    ret = napi_skb_finish(napi, skb, dev_gro_receive(napi, skb));
//    trace_napi_gro_receive_exit(ret);
    
    return ret;
}
EXPORT_SYMBOL(napi_gro_receive);


static void skb_warn_bad_offload(const struct sk_buff *skb)
{
//    static const netdev_features_t null_features;
//    struct net_device *dev = skb->dev;
//    const char *name = "";
//
//    if (!net_ratelimit())
//        return;
//
//    if (dev) {
//        if (dev->dev.parent)
//            name = dev_driver_string(dev->dev.parent);
//        else
//            name = netdev_name(dev);
//    }
//    skb_dump(KERN_WARNING, skb, false);
//    WARN(1, "%s: caps=(%pNF, %pNF)\n",
//         name, dev ? &dev->features : &null_features,
//         skb->sk ? &skb->sk->sk_route_caps : &null_features);
}

/* openvswitch calls this on rx path, so we need a different check.
 */
static inline bool skb_needs_check(struct sk_buff *skb, bool tx_path)
{
    if (tx_path)
        return skb->ip_summed != CHECKSUM_PARTIAL &&
        skb->ip_summed != CHECKSUM_UNNECESSARY;
    
    return skb->ip_summed == CHECKSUM_NONE;
}

/**
 * vlan_get_protocol - get protocol EtherType.
 * @skb: skbuff to query
 * @type: first vlan protocol
 * @depth: buffer to store length of eth and vlan tags in bytes
 *
 * Returns the EtherType of the packet, regardless of whether it is
 * vlan encapsulated (normal or hardware accelerated) or not.
 */
static inline __be16 __vlan_get_protocol(struct sk_buff *skb, __be16 type,
                                         int *depth)
{
    unsigned int vlan_depth = skb->mac_len;
    
//    /* if type is 802.1Q/AD then the header should already be
//     * present at mac_len - VLAN_HLEN (if mac_len > 0), or at
//     * ETH_HLEN otherwise
//     */
//    if (eth_type_vlan(type)) {
//        if (vlan_depth) {
//            if (WARN_ON(vlan_depth < VLAN_HLEN))
//                return 0;
//            vlan_depth -= VLAN_HLEN;
//        } else {
//            vlan_depth = ETH_HLEN;
//        }
//        do {
//            struct vlan_hdr *vh;
//
//            if (unlikely(!pskb_may_pull(skb,
//                                        vlan_depth + VLAN_HLEN)))
//                return 0;
//
//            vh = (struct vlan_hdr *)(skb->data + vlan_depth);
//            type = vh->h_vlan_encapsulated_proto;
//            vlan_depth += VLAN_HLEN;
//        } while (eth_type_vlan(type));
//    }
    
    if (depth)
        *depth = vlan_depth;
    
    return type;
}

__be16 skb_network_protocol(struct sk_buff *skb, int *depth)
{
    __be16 type = skb->protocol;
    
    /* Tunnel gso handlers can set protocol to ethernet. */
    if (type == htons(ETH_P_TEB)) {
        struct ethhdr *eth;
        
        if (unlikely(!pskb_may_pull(skb, sizeof(struct ethhdr))))
            return 0;
        
        eth = (struct ethhdr *)skb->data;
        type = eth->h_proto;
    }
    
    return __vlan_get_protocol(skb, type, depth);
}

static struct list_head offload_base __read_mostly;

/**
 *    skb_mac_gso_segment - mac layer segmentation handler.
 *    @skb: buffer to segment
 *    @features: features for the output path (see dev->features)
 */
struct sk_buff *skb_mac_gso_segment(struct sk_buff *skb,
                                    netdev_features_t features)
{
    struct sk_buff *segs = (struct sk_buff *)ERR_PTR(-EPROTONOSUPPORT);
    struct packet_offload *ptype;
    int vlan_depth = skb->mac_len;
    __be16 type = skb_network_protocol(skb, &vlan_depth);
    
    if (unlikely(!type))
        return (struct sk_buff *)ERR_PTR(-EINVAL);
    
    __skb_pull(skb, vlan_depth);
    
    rcu_read_lock();
    list_for_each_entry_rcu(ptype, &offload_base, list) {
        if (ptype->type == type && ptype->callbacks.gso_segment) {
            segs = ptype->callbacks.gso_segment(skb, features);
            break;
        }
    }
    rcu_read_unlock();
    
    __skb_push(skb, skb->data - skb_mac_header(skb));
    
    return segs;
}
EXPORT_SYMBOL(skb_mac_gso_segment);


/**
 *    __skb_gso_segment - Perform segmentation on skb.
 *    @skb: buffer to segment
 *    @features: features for the output path (see dev->features)
 *    @tx_path: whether it is called in TX path
 *
 *    This function segments the given skb and returns a list of segments.
 *
 *    It may return NULL if the skb requires no segmentation.  This is
 *    only possible when GSO is used for verifying header integrity.
 *
 *    Segmentation preserves SKB_SGO_CB_OFFSET bytes of previous skb cb.
 */
struct sk_buff *__skb_gso_segment(struct sk_buff *skb,
                                  netdev_features_t features, bool tx_path)
{
    struct sk_buff *segs;
    
    if (unlikely(skb_needs_check(skb, tx_path))) {
        int err;
        
        /* We're going to init ->check field in TCP or UDP header */
        err = skb_cow_head(skb, 0);
        if (err < 0)
            return (struct sk_buff *)ERR_PTR(err);
    }
    
    /* Only report GSO partial support if it will enable us to
     * support segmentation on this frame without needing additional
     * work.
     */
    if (features & NETIF_F_GSO_PARTIAL) {
        netdev_features_t partial_features = NETIF_F_GSO_ROBUST;
        struct net_device *dev = skb->dev;
        
        partial_features |= dev->features & dev->gso_partial_features;
        if (!skb_gso_ok(skb, features | partial_features))
            features &= ~NETIF_F_GSO_PARTIAL;
    }
    
    BUILD_BUG_ON(SKB_SGO_CB_OFFSET +
                 sizeof(*SKB_GSO_CB(skb)) > sizeof(skb->cb));
    
    SKB_GSO_CB(skb)->mac_offset = skb_headroom(skb);
    SKB_GSO_CB(skb)->encap_level = 0;
    
    skb_reset_mac_header(skb);
    skb_reset_mac_len(skb);
    
    segs = skb_mac_gso_segment(skb, features);
    
    if (segs != skb && unlikely(skb_needs_check(skb, tx_path) && !IS_ERR(segs)))
        skb_warn_bad_offload(skb);
    
    return segs;
}
EXPORT_SYMBOL(__skb_gso_segment);


void linkwatch_init_dev(struct net_device *dev)
{
//    /* Handle pre-registration link state changes */
//    if (!netif_carrier_ok(dev) || netif_dormant(dev))
//        rfc2863_policy(dev);
}

void __dev_kfree_skb_any(struct sk_buff *skb, enum skb_free_reason reason)
{
//    if (in_irq() || irqs_disabled())
//        __dev_kfree_skb_irq(skb, reason);
//    else
        dev_kfree_skb(skb);
}
EXPORT_SYMBOL(__dev_kfree_skb_any);


void linkwatch_fire_event(struct net_device *dev)
{
//    bool urgent = linkwatch_urgent_event(dev);
//
//    if (!test_and_set_bit(__LINK_STATE_LINKWATCH_PENDING, &dev->state)) {
//        linkwatch_add_event(dev);
//    } else if (!urgent)
//        return;
//
//    linkwatch_schedule_work(urgent);
}
EXPORT_SYMBOL(linkwatch_fire_event);









/**
 *    netif_carrier_on - set carrier
 *    @dev: network device
 *
 * Device has detected acquisition of carrier.
 */
void netif_carrier_on(struct net_device *dev)
{
    if (test_and_clear_bit(__LINK_STATE_NOCARRIER, &dev->state)) {
        if (dev->reg_state == NETREG_UNINITIALIZED)
            return;
        atomic_inc(&dev->carrier_up_count);
        linkwatch_fire_event(dev);
        if (netif_running(dev))
            __netdev_watchdog_up(dev);
    }
}
EXPORT_SYMBOL(netif_carrier_on);

/**
 *    netif_carrier_off - clear carrier
 *    @dev: network device
 *
 * Device has detected loss of carrier.
 */
void netif_carrier_off(struct net_device *dev)
{
    if (!test_and_set_bit(__LINK_STATE_NOCARRIER, &dev->state)) {
        if (dev->reg_state == NETREG_UNINITIALIZED)
            return;
        atomic_inc(&dev->carrier_down_count);
        linkwatch_fire_event(dev);
    }
}
EXPORT_SYMBOL(netif_carrier_off);


/**
 *    netif_receive_skb - process receive buffer from network
 *    @skb: buffer to process
 *
 *    netif_receive_skb() is the main receive data processing function.
 *    It always succeeds. The buffer may be dropped during processing
 *    for congestion control or by the protocol layers.
 *
 *    This function may only be called from softirq context and interrupts
 *    should be enabled.
 *
 *    Return values (usually ignored):
 *    NET_RX_SUCCESS: no congestion
 *    NET_RX_DROP: packet was dropped
 */
int netif_receive_skb(struct sk_buff *skb)
{
    int ret;

//    trace_netif_receive_skb_entry(skb);
//
//    ret = netif_receive_skb_internal(skb);
//    trace_netif_receive_skb_exit(ret);

//    return ret;
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
    return 0;
}
EXPORT_SYMBOL(netif_receive_skb);

static int call_netdevice_notifiers_extack(unsigned long val,
                                           struct net_device *dev,
                                           struct netlink_ext_ack *extack)
{
    struct netdev_notifier_info info = {
        .dev = dev,
        .extack = extack,
    };
    
    return call_netdevice_notifiers_info(val, &info);
}

/**
 *    call_netdevice_notifiers - call all network notifier blocks
 *      @val: value passed unmodified to notifier function
 *      @dev: net_device pointer passed unmodified to notifier function
 *
 *    Call all network notifier blocks.  Parameters and return value
 *    are as for raw_notifier_call_chain().
 */

int call_netdevice_notifiers(unsigned long val, struct net_device *dev)
{
    return call_netdevice_notifiers_extack(val, dev, NULL);
}
EXPORT_SYMBOL(call_netdevice_notifiers);


int netif_rx(struct sk_buff *skb)
{
    int ret;
    
//    trace_netif_rx_entry(skb);
//
//    ret = netif_rx_internal(skb);
//    trace_netif_rx_exit(ret);
    
    return ret;
}
EXPORT_SYMBOL(netif_rx);
