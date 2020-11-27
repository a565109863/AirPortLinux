//
//  netdevice.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef netdevice_h
#define netdevice_h


#include <linux/gfp.h>
#include <linux/workqueue.h>
#include <linux/device.h>
#include <linux/skbuff.h>

#include <linux/if_ether.h>
#include <linux/lockdep.h>
#include <linux/netdev_features.h>

#include <linux/fs.h>

#include <linux/u64_stats_sync.h>


#ifndef SPEED_UNKNOWN
#define SPEED_UNKNOWN  -1
#endif /* SPEED_UNKNOWN */

#ifndef DUPLEX_UNKNOWN
#define DUPLEX_UNKNOWN 0xff
#endif /* DUPLEX_UNKNOWN */

#ifndef ETHTOOL_FWVERS_LEN
#define ETHTOOL_FWVERS_LEN 32
#endif

typedef u64 netdev_features_t;


#define    NETDEV_ALIGN        32


/* hardware address assignment types */
#define NET_ADDR_PERM        0    /* address is permanent (default) */
#define NET_ADDR_RANDOM        1    /* address is generated randomly */
#define NET_ADDR_STOLEN        2    /* address is stolen from other device */
#define NET_ADDR_SET        3    /* address is set using
* dev_set_mac_address() */


enum netdev_tx {
    __NETDEV_TX_MIN     = INT_MIN,    /* make sure enum is signed */
    NETDEV_TX_OK     = 0x00,    /* driver took care of packet */
    NETDEV_TX_BUSY     = 0x10,    /* driver tx path was busy*/
};
typedef enum netdev_tx netdev_tx_t;



/* Init's network namespace */
extern struct net init_net;


typedef struct {
#ifdef CONFIG_NET_NS
    struct net *net;
#endif
} possible_net_t;


static inline void write_pnet(possible_net_t *pnet, struct net *net)
{
#ifdef CONFIG_NET_NS
    pnet->net = net;
#endif
}

static inline struct net *read_pnet(const possible_net_t *pnet)
{
#ifdef CONFIG_NET_NS
    return pnet->net;
#else
    return &init_net;
#endif
}


/*
 *    Old network device statistics. Fields are native words
 *    (unsigned long) so they can be read and written atomically.
 */

struct net_device_stats {
    unsigned long    rx_packets;
    unsigned long    tx_packets;
    unsigned long    rx_bytes;
    unsigned long    tx_bytes;
    unsigned long    rx_errors;
    unsigned long    tx_errors;
    unsigned long    rx_dropped;
    unsigned long    tx_dropped;
    unsigned long    multicast;
    unsigned long    collisions;
    unsigned long    rx_length_errors;
    unsigned long    rx_over_errors;
    unsigned long    rx_crc_errors;
    unsigned long    rx_frame_errors;
    unsigned long    rx_fifo_errors;
    unsigned long    rx_missed_errors;
    unsigned long    tx_aborted_errors;
    unsigned long    tx_carrier_errors;
    unsigned long    tx_fifo_errors;
    unsigned long    tx_heartbeat_errors;
    unsigned long    tx_window_errors;
    unsigned long    rx_compressed;
    unsigned long    tx_compressed;
};


#define MAX_ADDR_LEN 19

struct netdev_hw_addr {
    struct list_head    list;
    unsigned char        addr[MAX_ADDR_LEN];
    unsigned char        type;
#define NETDEV_HW_ADDR_T_LAN        1
#define NETDEV_HW_ADDR_T_SAN        2
#define NETDEV_HW_ADDR_T_SLAVE        3
#define NETDEV_HW_ADDR_T_UNICAST    4
#define NETDEV_HW_ADDR_T_MULTICAST    5
    bool            global_use;
    int            sync_cnt;
    int            refcount;
    int            synced;
    struct rcu_head        rcu_head;
};

struct netdev_hw_addr_list {
    struct list_head    list;
    int            count;
};


enum rx_handler_result {
    RX_HANDLER_CONSUMED,
    RX_HANDLER_ANOTHER,
    RX_HANDLER_EXACT,
    RX_HANDLER_PASS,
};
typedef enum rx_handler_result rx_handler_result_t;
typedef rx_handler_result_t rx_handler_func_t(struct sk_buff **pskb);


#define TC_MAX_QUEUE    16
#define TC_BITMASK    15
/* HW offloaded queuing disciplines txq count and offset maps */
struct netdev_tc_txq {
    u16 count;
    u16 offset;
};


struct notifier_block;

typedef    int (*notifier_fn_t)(struct notifier_block *nb,
            unsigned long action, void *data);

struct notifier_block {
    notifier_fn_t notifier_call;
    struct notifier_block __rcu *next;
    int priority;
};

struct net_device {
    char            name[IFNAMSIZ];
    struct netdev_name_node    *name_node;
    struct dev_ifalias    __rcu *ifalias;
    /*
     *    I/O specific fields
     *    FIXME: Merge these and struct ifmap into one
     */
    unsigned long        mem_end;
    unsigned long        mem_start;
    unsigned long        base_addr;
    int            irq;

    /*
     *    Some hardware also needs these fields (state,dev_list,
     *    napi_list,unreg_list,close_list) but they are not
     *    part of the usual set specified in Space.c.
     */

    unsigned long        state;

    struct list_head    dev_list;
    struct list_head    napi_list;
    struct list_head    unreg_list;
    struct list_head    close_list;
    struct list_head    ptype_all;
    struct list_head    ptype_specific;

    struct {
        struct list_head upper;
        struct list_head lower;
    } adj_list;

    netdev_features_t    features;
    netdev_features_t    hw_features;
    netdev_features_t    wanted_features;
    netdev_features_t    vlan_features;
    netdev_features_t    hw_enc_features;
    netdev_features_t    mpls_features;
    netdev_features_t    gso_partial_features;

    int            ifindex;
    int            group;

    struct net_device_stats    stats;

    atomic_long_t        rx_dropped;
    atomic_long_t        tx_dropped;
    atomic_long_t        rx_nohandler;

    /* Stats to monitor link on/off, flapping */
    atomic_t        carrier_up_count;
    atomic_t        carrier_down_count;

#ifdef CONFIG_WIRELESS_EXT
    const struct iw_handler_def *wireless_handlers;
    struct iw_public_data    *wireless_data;
#endif
    const struct net_device_ops *netdev_ops;
    const struct ethtool_ops *ethtool_ops;
#ifdef CONFIG_NET_L3_MASTER_DEV
    const struct l3mdev_ops    *l3mdev_ops;
#endif
#if IS_ENABLED(CONFIG_IPV6)
    const struct ndisc_ops *ndisc_ops;
#endif

#ifdef CONFIG_XFRM_OFFLOAD
    const struct xfrmdev_ops *xfrmdev_ops;
#endif

#if IS_ENABLED(CONFIG_TLS_DEVICE)
    const struct tlsdev_ops *tlsdev_ops;
#endif

    const struct header_ops *header_ops;

    unsigned int        flags;
    unsigned int        priv_flags;

    unsigned short        gflags;
    unsigned short        padded;

    unsigned char        operstate;
    unsigned char        link_mode;

    unsigned char        if_port;
    unsigned char        dma;

    /* Note : dev->mtu is often read without holding a lock.
     * Writers usually hold RTNL.
     * It is recommended to use READ_ONCE() to annotate the reads,
     * and to use WRITE_ONCE() to annotate the writes.
     */
    unsigned int        mtu;
    unsigned int        min_mtu;
    unsigned int        max_mtu;
    unsigned short        type;
    unsigned short        hard_header_len;
    unsigned char        min_header_len;

    unsigned short        needed_headroom;
    unsigned short        needed_tailroom;

    /* Interface address info. */
    unsigned char        perm_addr[MAX_ADDR_LEN];
    unsigned char        addr_assign_type;
    unsigned char        addr_len;
    unsigned char        upper_level;
    unsigned char        lower_level;
    unsigned short        neigh_priv_len;
    unsigned short          dev_id;
    unsigned short          dev_port;
    spinlock_t        addr_list_lock;
    unsigned char        name_assign_type;
    bool            uc_promisc;
    struct netdev_hw_addr_list    uc;
    struct netdev_hw_addr_list    mc;
    struct netdev_hw_addr_list    dev_addrs;

#ifdef CONFIG_SYSFS
    struct kset        *queues_kset;
#endif
    unsigned int        promiscuity;
    unsigned int        allmulti;


    /* Protocol-specific pointers */

#if IS_ENABLED(CONFIG_VLAN_8021Q)
    struct vlan_info __rcu    *vlan_info;
#endif
#if IS_ENABLED(CONFIG_NET_DSA)
    struct dsa_port        *dsa_ptr;
#endif
#if IS_ENABLED(CONFIG_TIPC)
    struct tipc_bearer __rcu *tipc_ptr;
#endif
#if IS_ENABLED(CONFIG_IRDA) || IS_ENABLED(CONFIG_ATALK)
    void             *atalk_ptr;
#endif
    struct in_device __rcu    *ip_ptr;
#if IS_ENABLED(CONFIG_DECNET)
    struct dn_dev __rcu     *dn_ptr;
#endif
    struct inet6_dev __rcu    *ip6_ptr;
#if IS_ENABLED(CONFIG_AX25)
    void            *ax25_ptr;
#endif
    struct wireless_dev    *ieee80211_ptr;
    struct wpan_dev        *ieee802154_ptr;
#if IS_ENABLED(CONFIG_MPLS_ROUTING)
    struct mpls_dev __rcu    *mpls_ptr;
#endif

/*
 * Cache lines mostly used on receive path (including eth_type_trans())
 */
    /* Interface address info used in eth_type_trans() */
    unsigned char        *dev_addr;

    struct netdev_rx_queue    *_rx;
    unsigned int        num_rx_queues;
    unsigned int        real_num_rx_queues;

    struct bpf_prog __rcu    *xdp_prog;
    unsigned long        gro_flush_timeout;
    rx_handler_func_t __rcu    *rx_handler;
    void __rcu        *rx_handler_data;

#ifdef CONFIG_NET_CLS_ACT
    struct mini_Qdisc __rcu    *miniq_ingress;
#endif
    struct netdev_queue __rcu *ingress_queue;
#ifdef CONFIG_NETFILTER_INGRESS
    struct nf_hook_entries __rcu *nf_hooks_ingress;
#endif

    unsigned char        broadcast[MAX_ADDR_LEN];
#ifdef CONFIG_RFS_ACCEL
    struct cpu_rmap        *rx_cpu_rmap;
#endif
    struct hlist_node    index_hlist;

/*
 * Cache lines mostly used on transmit path
 */
    struct netdev_queue    *_tx;
    unsigned int        num_tx_queues;
    unsigned int        real_num_tx_queues;
    struct Qdisc        *qdisc;
    unsigned int        tx_queue_len;
    spinlock_t        tx_global_lock;

    struct xdp_dev_bulk_queue __percpu *xdp_bulkq;

#ifdef CONFIG_XPS
    struct xps_dev_maps __rcu *xps_cpus_map;
    struct xps_dev_maps __rcu *xps_rxqs_map;
#endif
#ifdef CONFIG_NET_CLS_ACT
    struct mini_Qdisc __rcu    *miniq_egress;
#endif

#ifdef CONFIG_NET_SCHED
    DECLARE_HASHTABLE    (qdisc_hash, 4);
#endif
    /* These may be needed for future network-power-down code. */
    struct timer_list    watchdog_timer;
    int            watchdog_timeo;

    struct list_head    todo_list;
    int __percpu        *pcpu_refcnt;

    struct list_head    link_watch_list;

    enum { NETREG_UNINITIALIZED=0,
           NETREG_REGISTERED,    /* completed register_netdevice */
           NETREG_UNREGISTERING,    /* called unregister_netdevice */
           NETREG_UNREGISTERED,    /* completed unregister todo */
           NETREG_RELEASED,        /* called free_netdev */
           NETREG_DUMMY,        /* dummy device for NAPI poll */
    } reg_state:8;

    bool dismantle;

    enum {
        RTNL_LINK_INITIALIZED,
        RTNL_LINK_INITIALIZING,
    } rtnl_link_state:16;

    bool needs_free_netdev;
    void (*priv_destructor)(struct net_device *dev);

#ifdef CONFIG_NETPOLL
    struct netpoll_info __rcu    *npinfo;
#endif

    possible_net_t            nd_net;

    /* mid-layer private */
    union {
        void                    *ml_priv;
        struct pcpu_lstats __percpu        *lstats;
        struct pcpu_sw_netstats __percpu    *tstats;
        struct pcpu_dstats __percpu        *dstats;
    };

#if IS_ENABLED(CONFIG_GARP)
    struct garp_port __rcu    *garp_port;
#endif
#if IS_ENABLED(CONFIG_MRP)
    struct mrp_port __rcu    *mrp_port;
#endif

    struct device        dev;
    const struct attribute_group *sysfs_groups[4];
    const struct attribute_group *sysfs_rx_queue_group;

    const struct rtnl_link_ops *rtnl_link_ops;

    /* for setting kernel sock attribute on TCP connection setup */
#define GSO_MAX_SIZE        65536
    unsigned int        gso_max_size;
#define GSO_MAX_SEGS        65535
    u16            gso_max_segs;

#ifdef CONFIG_DCB
    const struct dcbnl_rtnl_ops *dcbnl_ops;
#endif
    s16            num_tc;
    struct netdev_tc_txq    tc_to_txq[TC_MAX_QUEUE];
    u8            prio_tc_map[TC_BITMASK + 1];

#if IS_ENABLED(CONFIG_FCOE)
    unsigned int        fcoe_ddp_xid;
#endif
#if IS_ENABLED(CONFIG_CGROUP_NET_PRIO)
    struct netprio_map __rcu *priomap;
#endif
    struct phy_device    *phydev;
    struct sfp_bus        *sfp_bus;
    struct lock_class_key    qdisc_tx_busylock_key;
    struct lock_class_key    qdisc_running_key;
    struct lock_class_key    qdisc_xmit_lock_key;
    struct lock_class_key    addr_list_lock_key;
    bool            proto_down;
    unsigned        wol_enabled:1;

    struct list_head    net_notifier_list;
};
#define to_net_dev(d) container_of(d, struct net_device, dev)


struct gro_list {
    struct list_head    list;
    int            count;
};


struct hrtimer {
//    struct timerqueue_node        node;
//    ktime_t                _softexpires;
//    enum hrtimer_restart        (*function)(struct hrtimer *);
//    struct hrtimer_clock_base    *base;
    u8                state;
    u8                is_rel;
    u8                is_soft;
    u8                is_hard;
};

/*
 * size of gro hash buckets, must less than bit number of
 * napi_struct::gro_bitmask
 */
#define GRO_HASH_BUCKETS    8

/*
 * Structure for NAPI scheduling similar to tasklet but with weighting
 */
struct napi_struct {
    /* The poll_list must only be managed by the entity which
     * changes the state of the NAPI_STATE_SCHED bit.  This means
     * whoever atomically sets that bit can add this napi_struct
     * to the per-CPU poll_list, and whoever clears that bit
     * can remove from the list right before clearing the bit.
     */
    struct list_head    poll_list;

    unsigned long        state;
    int            weight;
    unsigned long        gro_bitmask;
    int            (*poll)(struct napi_struct *, int);
#ifdef CONFIG_NETPOLL
    int            poll_owner;
#endif
    struct net_device    *dev;
    struct gro_list        gro_hash[GRO_HASH_BUCKETS];
    struct sk_buff        *skb;
    struct list_head    rx_list; /* Pending GRO_NORMAL skbs */
    int            rx_count; /* length of rx_list */
    struct hrtimer        timer;
    struct list_head    dev_list;
    struct hlist_node    napi_hash_node;
    unsigned int        napi_id;
};



enum tc_setup_type {
    TC_SETUP_QDISC_MQPRIO,
    TC_SETUP_CLSU32,
    TC_SETUP_CLSFLOWER,
    TC_SETUP_CLSMATCHALL,
    TC_SETUP_CLSBPF,
    TC_SETUP_BLOCK,
    TC_SETUP_QDISC_CBS,
    TC_SETUP_QDISC_RED,
    TC_SETUP_QDISC_PRIO,
    TC_SETUP_QDISC_MQ,
    TC_SETUP_QDISC_ETF,
    TC_SETUP_ROOT_QDISC,
    TC_SETUP_QDISC_GRED,
    TC_SETUP_QDISC_TAPRIO,
    TC_SETUP_FT,
    TC_SETUP_QDISC_ETS,
    TC_SETUP_QDISC_TBF,
};

struct net_device_ops {
    int            (*ndo_init)(struct net_device *dev);
    void            (*ndo_uninit)(struct net_device *dev);
    int            (*ndo_open)(struct net_device *dev);
    int            (*ndo_stop)(struct net_device *dev);
    netdev_tx_t        (*ndo_start_xmit)(struct sk_buff *skb,
                          struct net_device *dev);
    netdev_features_t    (*ndo_features_check)(struct sk_buff *skb,
                              struct net_device *dev,
                              netdev_features_t features);
    u16            (*ndo_select_queue)(struct net_device *dev,
                            struct sk_buff *skb,
                            struct net_device *sb_dev);
    void            (*ndo_change_rx_flags)(struct net_device *dev,
                               int flags);
    void            (*ndo_set_rx_mode)(struct net_device *dev);
    int            (*ndo_set_mac_address)(struct net_device *dev,
                               void *addr);
    int            (*ndo_validate_addr)(struct net_device *dev);
    int            (*ndo_do_ioctl)(struct net_device *dev,
                            struct ifreq *ifr, int cmd);
    int            (*ndo_set_config)(struct net_device *dev,
                              struct ifmap *map);
    int            (*ndo_change_mtu)(struct net_device *dev,
                          int new_mtu);
    int            (*ndo_neigh_setup)(struct net_device *dev,
                           struct neigh_parms *);
    void            (*ndo_tx_timeout) (struct net_device *dev,
                           unsigned int txqueue);

    void            (*ndo_get_stats64)(struct net_device *dev,
                           struct rtnl_link_stats64 *storage);
    bool            (*ndo_has_offload_stats)(const struct net_device *dev, int attr_id);
    int            (*ndo_get_offload_stats)(int attr_id,
                             const struct net_device *dev,
                             void *attr_data);
    struct net_device_stats* (*ndo_get_stats)(struct net_device *dev);

    int            (*ndo_vlan_rx_add_vid)(struct net_device *dev,
                               __be16 proto, u16 vid);
    int            (*ndo_vlan_rx_kill_vid)(struct net_device *dev,
                                __be16 proto, u16 vid);
#ifdef CONFIG_NET_POLL_CONTROLLER
    void                    (*ndo_poll_controller)(struct net_device *dev);
    int            (*ndo_netpoll_setup)(struct net_device *dev,
                             struct netpoll_info *info);
    void            (*ndo_netpoll_cleanup)(struct net_device *dev);
#endif
    int            (*ndo_set_vf_mac)(struct net_device *dev,
                          int queue, u8 *mac);
    int            (*ndo_set_vf_vlan)(struct net_device *dev,
                           int queue, u16 vlan,
                           u8 qos, __be16 proto);
    int            (*ndo_set_vf_rate)(struct net_device *dev,
                           int vf, int min_tx_rate,
                           int max_tx_rate);
    int            (*ndo_set_vf_spoofchk)(struct net_device *dev,
                               int vf, bool setting);
    int            (*ndo_set_vf_trust)(struct net_device *dev,
                            int vf, bool setting);
    int            (*ndo_get_vf_config)(struct net_device *dev,
                             int vf,
                             struct ifla_vf_info *ivf);
    int            (*ndo_set_vf_link_state)(struct net_device *dev,
                             int vf, int link_state);
    int            (*ndo_get_vf_stats)(struct net_device *dev,
                            int vf,
                            struct ifla_vf_stats
                            *vf_stats);
    int            (*ndo_set_vf_port)(struct net_device *dev,
                           int vf,
                           struct nlattr *port[]);
    int            (*ndo_get_vf_port)(struct net_device *dev,
                           int vf, struct sk_buff *skb);
    int            (*ndo_get_vf_guid)(struct net_device *dev,
                           int vf,
                           struct ifla_vf_guid *node_guid,
                           struct ifla_vf_guid *port_guid);
    int            (*ndo_set_vf_guid)(struct net_device *dev,
                           int vf, u64 guid,
                           int guid_type);
    int            (*ndo_set_vf_rss_query_en)(
                           struct net_device *dev,
                           int vf, bool setting);
    int            (*ndo_setup_tc)(struct net_device *dev,
                        enum tc_setup_type type,
                        void *type_data);
#if IS_ENABLED(CONFIG_FCOE)
    int            (*ndo_fcoe_enable)(struct net_device *dev);
    int            (*ndo_fcoe_disable)(struct net_device *dev);
    int            (*ndo_fcoe_ddp_setup)(struct net_device *dev,
                              u16 xid,
                              struct scatterlist *sgl,
                              unsigned int sgc);
    int            (*ndo_fcoe_ddp_done)(struct net_device *dev,
                             u16 xid);
    int            (*ndo_fcoe_ddp_target)(struct net_device *dev,
                               u16 xid,
                               struct scatterlist *sgl,
                               unsigned int sgc);
    int            (*ndo_fcoe_get_hbainfo)(struct net_device *dev,
                            struct netdev_fcoe_hbainfo *hbainfo);
#endif

#if IS_ENABLED(CONFIG_LIBFCOE)
#define NETDEV_FCOE_WWNN 0
#define NETDEV_FCOE_WWPN 1
    int            (*ndo_fcoe_get_wwn)(struct net_device *dev,
                            u64 *wwn, int type);
#endif

#ifdef CONFIG_RFS_ACCEL
    int            (*ndo_rx_flow_steer)(struct net_device *dev,
                             const struct sk_buff *skb,
                             u16 rxq_index,
                             u32 flow_id);
#endif
    int            (*ndo_add_slave)(struct net_device *dev,
                         struct net_device *slave_dev,
                         struct netlink_ext_ack *extack);
    int            (*ndo_del_slave)(struct net_device *dev,
                         struct net_device *slave_dev);
    netdev_features_t    (*ndo_fix_features)(struct net_device *dev,
                            netdev_features_t features);
    int            (*ndo_set_features)(struct net_device *dev,
                            netdev_features_t features);
    int            (*ndo_neigh_construct)(struct net_device *dev,
                               struct neighbour *n);
    void            (*ndo_neigh_destroy)(struct net_device *dev,
                             struct neighbour *n);

    int            (*ndo_fdb_add)(struct ndmsg *ndm,
                           struct nlattr *tb[],
                           struct net_device *dev,
                           const unsigned char *addr,
                           u16 vid,
                           u16 flags,
                           struct netlink_ext_ack *extack);
    int            (*ndo_fdb_del)(struct ndmsg *ndm,
                           struct nlattr *tb[],
                           struct net_device *dev,
                           const unsigned char *addr,
                           u16 vid);
    int            (*ndo_fdb_dump)(struct sk_buff *skb,
                        struct netlink_callback *cb,
                        struct net_device *dev,
                        struct net_device *filter_dev,
                        int *idx);
    int            (*ndo_fdb_get)(struct sk_buff *skb,
                           struct nlattr *tb[],
                           struct net_device *dev,
                           const unsigned char *addr,
                           u16 vid, u32 portid, u32 seq,
                           struct netlink_ext_ack *extack);
    int            (*ndo_bridge_setlink)(struct net_device *dev,
                              struct nlmsghdr *nlh,
                              u16 flags,
                              struct netlink_ext_ack *extack);
    int            (*ndo_bridge_getlink)(struct sk_buff *skb,
                              u32 pid, u32 seq,
                              struct net_device *dev,
                              u32 filter_mask,
                              int nlflags);
    int            (*ndo_bridge_dellink)(struct net_device *dev,
                              struct nlmsghdr *nlh,
                              u16 flags);
    int            (*ndo_change_carrier)(struct net_device *dev,
                              bool new_carrier);
    int            (*ndo_get_phys_port_id)(struct net_device *dev,
                            struct netdev_phys_item_id *ppid);
    int            (*ndo_get_port_parent_id)(struct net_device *dev,
                              struct netdev_phys_item_id *ppid);
    int            (*ndo_get_phys_port_name)(struct net_device *dev,
                              char *name, size_t len);
    void            (*ndo_udp_tunnel_add)(struct net_device *dev,
                              struct udp_tunnel_info *ti);
    void            (*ndo_udp_tunnel_del)(struct net_device *dev,
                              struct udp_tunnel_info *ti);
    void*            (*ndo_dfwd_add_station)(struct net_device *pdev,
                            struct net_device *dev);
    void            (*ndo_dfwd_del_station)(struct net_device *pdev,
                            void *priv);

    int            (*ndo_set_tx_maxrate)(struct net_device *dev,
                              int queue_index,
                              u32 maxrate);
    int            (*ndo_get_iflink)(const struct net_device *dev);
    int            (*ndo_change_proto_down)(struct net_device *dev,
                             bool proto_down);
    int            (*ndo_fill_metadata_dst)(struct net_device *dev,
                               struct sk_buff *skb);
    void            (*ndo_set_rx_headroom)(struct net_device *dev,
                               int needed_headroom);
    int            (*ndo_bpf)(struct net_device *dev,
                       struct netdev_bpf *bpf);
    int            (*ndo_xdp_xmit)(struct net_device *dev, int n,
                        struct xdp_frame **xdp,
                        u32 flags);
    int            (*ndo_xsk_wakeup)(struct net_device *dev,
                          u32 queue_id, u32 flags);
    struct devlink_port *    (*ndo_get_devlink_port)(struct net_device *dev);
};


static
void synchronize_net(void)
{
    might_sleep();
//    if (rtnl_is_locked())
//        synchronize_rcu_expedited();
//    else
//        synchronize_rcu();
}
EXPORT_SYMBOL(synchronize_net);


#define net_ratelimit() 0



#define netdev_hw_addr_list_count(l) ((l)->count)
#define netdev_hw_addr_list_empty(l) (netdev_hw_addr_list_count(l) == 0)
#define netdev_hw_addr_list_for_each(ha, l) \
list_for_each_entry(ha, &(l)->list, list)

#define netdev_uc_count(dev) netdev_hw_addr_list_count(&(dev)->uc)
#define netdev_uc_empty(dev) netdev_hw_addr_list_empty(&(dev)->uc)
#define netdev_for_each_uc_addr(ha, dev) \
netdev_hw_addr_list_for_each(ha, &(dev)->uc)

#define netdev_mc_count(dev) netdev_hw_addr_list_count(&(dev)->mc)
#define netdev_mc_empty(dev) netdev_hw_addr_list_empty(&(dev)->mc)
#define netdev_for_each_mc_addr(ha, dev) \
netdev_hw_addr_list_for_each(ha, &(dev)->mc)


static struct sk_buff *__skb_gso_segment(struct sk_buff *skb,
                  netdev_features_t features, bool tx_path)
{
    return NULL;
}


static inline
struct sk_buff *skb_gso_segment(struct sk_buff *skb, netdev_features_t features)
{
    return __skb_gso_segment(skb, features, true);
}

#endif /* netdevice_h */
