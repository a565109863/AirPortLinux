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

#include <net/net_namespace.h>

struct netpoll_info;
struct device;
struct phy_device;
struct dsa_port;

struct sfp_bus;
/* 802.11 specific */
struct wireless_dev;
/* 802.15.4 specific */
struct wpan_dev;
struct mpls_dev;
/* UDP Tunnel offloads */
struct udp_tunnel_info;
struct bpf_prog;
struct xdp_buff;

void netdev_set_default_ethtool_ops(struct net_device *dev,
                    const struct ethtool_ops *ops);

/* Backlog congestion levels */
#define NET_RX_SUCCESS        0    /* keep 'em coming, baby */
#define NET_RX_DROP        1    /* packet dropped */

#define MAX_NEST_DEV 8

/*
 * Transmit return codes: transmit return codes originate from three different
 * namespaces:
 *
 * - qdisc return codes
 * - driver transmit return codes
 * - errno values
 *
 * Drivers are allowed to return any one of those in their hard_start_xmit()
 * function. Real network devices commonly used with qdiscs should only return
 * the driver transmit return codes though - when qdiscs are used, the actual
 * transmission happens asynchronously, so the value is not propagated to
 * higher layers. Virtual network devices transmit synchronously; in this case
 * the driver transmit return codes are consumed by dev_queue_xmit(), and all
 * others are propagated to higher layers.
 */

/* qdisc ->enqueue() return codes. */
#define NET_XMIT_SUCCESS    0x00
#define NET_XMIT_DROP        0x01    /* skb dropped            */
#define NET_XMIT_CN        0x02    /* congestion notification    */
#define NET_XMIT_MASK        0x0f    /* qdisc flags in net/sch_generic.h */

/* NET_XMIT_CN is special. It does not guarantee that this packet is lost. It
 * indicates that the device will soon be dropping packets, or already drops
 * some packets of the same priority; prompting us to send less aggressively. */
#define net_xmit_eval(e)    ((e) == NET_XMIT_CN ? 0 : (e))
#define net_xmit_errno(e)    ((e) != NET_XMIT_CN ? -ENOBUFS : 0)

/* Driver transmit return codes */
#define NETDEV_TX_MASK        0xf0

enum netdev_tx {
    __NETDEV_TX_MIN     = INT_MIN,    /* make sure enum is signed */
    NETDEV_TX_OK     = 0x00,    /* driver took care of packet */
    NETDEV_TX_BUSY     = 0x10,    /* driver tx path was busy*/
};
typedef enum netdev_tx netdev_tx_t;

/*
 * Current order: NETDEV_TX_MASK > NET_XMIT_MASK >= 0 is significant;
 * hard_start_xmit() return < NET_XMIT_MASK means skb was consumed.
 */
static inline bool dev_xmit_complete(int rc)
{
    /*
     * Positive cases with an skb consumed by a driver:
     * - successful transmission (rc == NETDEV_TX_OK)
     * - error while transmitting (rc < 0)
     * - error while queueing to a different device (rc & NET_XMIT_MASK)
     */
    if (likely(rc < NET_XMIT_MASK))
        return true;

    return false;
}

/*
 *    Compute the worst-case header length according to the protocols
 *    used.
 */

#if defined(CONFIG_HYPERV_NET)
# define LL_MAX_HEADER 128
#elif defined(CONFIG_WLAN) || IS_ENABLED(CONFIG_AX25)
# if defined(CONFIG_MAC80211_MESH)
#  define LL_MAX_HEADER 128
# else
#  define LL_MAX_HEADER 96
# endif
#else
# define LL_MAX_HEADER 32
#endif

#if !IS_ENABLED(CONFIG_NET_IPIP) && !IS_ENABLED(CONFIG_NET_IPGRE) && \
    !IS_ENABLED(CONFIG_IPV6_SIT) && !IS_ENABLED(CONFIG_IPV6_TUNNEL)
#define MAX_HEADER LL_MAX_HEADER
#else
#define MAX_HEADER (LL_MAX_HEADER + 48)
#endif

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


/* These flag bits are private to the generic network queueing
 * layer; they may not be explicitly referenced by any other
 * code.
 */

enum netdev_state_t {
    __LINK_STATE_START,
    __LINK_STATE_PRESENT,
    __LINK_STATE_NOCARRIER,
    __LINK_STATE_LINKWATCH_PENDING,
    __LINK_STATE_DORMANT,
};


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

/**
 * enum net_device_priv_flags - &struct net_device priv_flags
 *
 * These are the &struct net_device, they are only set internally
 * by drivers and used in the kernel. These flags are invisible to
 * userspace; this means that the order of these flags can change
 * during any kernel release.
 *
 * You should have a pretty good reason to be extending these flags.
 *
 * @IFF_802_1Q_VLAN: 802.1Q VLAN device
 * @IFF_EBRIDGE: Ethernet bridging device
 * @IFF_BONDING: bonding master or slave
 * @IFF_ISATAP: ISATAP interface (RFC4214)
 * @IFF_WAN_HDLC: WAN HDLC device
 * @IFF_XMIT_DST_RELEASE: dev_hard_start_xmit() is allowed to
 *    release skb->dst
 * @IFF_DONT_BRIDGE: disallow bridging this ether dev
 * @IFF_DISABLE_NETPOLL: disable netpoll at run-time
 * @IFF_MACVLAN_PORT: device used as macvlan port
 * @IFF_BRIDGE_PORT: device used as bridge port
 * @IFF_OVS_DATAPATH: device used as Open vSwitch datapath port
 * @IFF_TX_SKB_SHARING: The interface supports sharing skbs on transmit
 * @IFF_UNICAST_FLT: Supports unicast filtering
 * @IFF_TEAM_PORT: device used as team port
 * @IFF_SUPP_NOFCS: device supports sending custom FCS
 * @IFF_LIVE_ADDR_CHANGE: device supports hardware address
 *    change when it's running
 * @IFF_MACVLAN: Macvlan device
 * @IFF_XMIT_DST_RELEASE_PERM: IFF_XMIT_DST_RELEASE not taking into account
 *    underlying stacked devices
 * @IFF_L3MDEV_MASTER: device is an L3 master device
 * @IFF_NO_QUEUE: device can run without qdisc attached
 * @IFF_OPENVSWITCH: device is a Open vSwitch master
 * @IFF_L3MDEV_SLAVE: device is enslaved to an L3 master device
 * @IFF_TEAM: device is a team device
 * @IFF_RXFH_CONFIGURED: device has had Rx Flow indirection table configured
 * @IFF_PHONY_HEADROOM: the headroom value is controlled by an external
 *    entity (i.e. the master device for bridged veth)
 * @IFF_MACSEC: device is a MACsec device
 * @IFF_NO_RX_HANDLER: device doesn't support the rx_handler hook
 * @IFF_FAILOVER: device is a failover master device
 * @IFF_FAILOVER_SLAVE: device is lower dev of a failover master device
 * @IFF_L3MDEV_RX_HANDLER: only invoke the rx handler of L3 master device
 * @IFF_LIVE_RENAME_OK: rename is allowed while device is up and running
 */
enum netdev_priv_flags {
    IFF_802_1Q_VLAN            = 1<<0,
    IFF_EBRIDGE            = 1<<1,
    IFF_BONDING            = 1<<2,
    IFF_ISATAP            = 1<<3,
    IFF_WAN_HDLC            = 1<<4,
    IFF_XMIT_DST_RELEASE        = 1<<5,
    IFF_DONT_BRIDGE            = 1<<6,
    IFF_DISABLE_NETPOLL        = 1<<7,
    IFF_MACVLAN_PORT        = 1<<8,
    IFF_BRIDGE_PORT            = 1<<9,
    IFF_OVS_DATAPATH        = 1<<10,
    IFF_TX_SKB_SHARING        = 1<<11,
    IFF_UNICAST_FLT            = 1<<12,
    IFF_TEAM_PORT            = 1<<13,
    IFF_SUPP_NOFCS            = 1<<14,
    IFF_LIVE_ADDR_CHANGE        = 1<<15,
    IFF_MACVLAN            = 1<<16,
    IFF_XMIT_DST_RELEASE_PERM    = 1<<17,
    IFF_L3MDEV_MASTER        = 1<<18,
    IFF_NO_QUEUE            = 1<<19,
    IFF_OPENVSWITCH            = 1<<20,
    IFF_L3MDEV_SLAVE        = 1<<21,
    IFF_TEAM            = 1<<22,
    IFF_RXFH_CONFIGURED        = 1<<23,
    IFF_PHONY_HEADROOM        = 1<<24,
    IFF_MACSEC            = 1<<25,
    IFF_NO_RX_HANDLER        = 1<<26,
    IFF_FAILOVER            = 1<<27,
    IFF_FAILOVER_SLAVE        = 1<<28,
    IFF_L3MDEV_RX_HANDLER        = 1<<29,
    IFF_LIVE_RENAME_OK        = 1<<30,
};

#define IFF_802_1Q_VLAN            IFF_802_1Q_VLAN
#define IFF_EBRIDGE            IFF_EBRIDGE
#define IFF_BONDING            IFF_BONDING
#define IFF_ISATAP            IFF_ISATAP
#define IFF_WAN_HDLC            IFF_WAN_HDLC
#define IFF_XMIT_DST_RELEASE        IFF_XMIT_DST_RELEASE
#define IFF_DONT_BRIDGE            IFF_DONT_BRIDGE
#define IFF_DISABLE_NETPOLL        IFF_DISABLE_NETPOLL
#define IFF_MACVLAN_PORT        IFF_MACVLAN_PORT
#define IFF_BRIDGE_PORT            IFF_BRIDGE_PORT
#define IFF_OVS_DATAPATH        IFF_OVS_DATAPATH
#define IFF_TX_SKB_SHARING        IFF_TX_SKB_SHARING
#define IFF_UNICAST_FLT            IFF_UNICAST_FLT
#define IFF_TEAM_PORT            IFF_TEAM_PORT
#define IFF_SUPP_NOFCS            IFF_SUPP_NOFCS
#define IFF_LIVE_ADDR_CHANGE        IFF_LIVE_ADDR_CHANGE
#define IFF_MACVLAN            IFF_MACVLAN
#define IFF_XMIT_DST_RELEASE_PERM    IFF_XMIT_DST_RELEASE_PERM
#define IFF_L3MDEV_MASTER        IFF_L3MDEV_MASTER
#define IFF_NO_QUEUE            IFF_NO_QUEUE
#define IFF_OPENVSWITCH            IFF_OPENVSWITCH
#define IFF_L3MDEV_SLAVE        IFF_L3MDEV_SLAVE
#define IFF_TEAM            IFF_TEAM
#define IFF_RXFH_CONFIGURED        IFF_RXFH_CONFIGURED
#define IFF_MACSEC            IFF_MACSEC
#define IFF_NO_RX_HANDLER        IFF_NO_RX_HANDLER
#define IFF_FAILOVER            IFF_FAILOVER
#define IFF_FAILOVER_SLAVE        IFF_FAILOVER_SLAVE
#define IFF_L3MDEV_RX_HANDLER        IFF_L3MDEV_RX_HANDLER
#define IFF_LIVE_RENAME_OK        IFF_LIVE_RENAME_OK


static
void synchronize_rcu(void)
{
    might_sleep();
//    if (rtnl_is_locked())
//        synchronize_rcu_expedited();
//    else
//        synchronize_rcu();
}

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


/**
 *    netif_running - test if up
 *    @dev: network device
 *
 *    Test if the device has been brought up.
 */
static inline bool netif_running(const struct net_device *dev)
{
    return test_bit(__LINK_STATE_START, &dev->state);
}


static int netif_receive_skb(struct sk_buff *skb)
{
    int ret = 0;
//
//    trace_netif_receive_skb_entry(skb);
//
//    ret = netif_receive_skb_internal(skb);
//    trace_netif_receive_skb_exit(ret);

    return ret;
}


enum gro_result {
    GRO_MERGED,
    GRO_MERGED_FREE,
    GRO_HELD,
    GRO_NORMAL,
    GRO_DROP,
    GRO_CONSUMED,
};
typedef enum gro_result gro_result_t;

static gro_result_t napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
    gro_result_t ret = GRO_MERGED;

//    skb_mark_napi_id(skb, napi);
//    trace_napi_gro_receive_entry(skb);
//
//    skb_gro_reset_offset(skb);
//
//    ret = napi_skb_finish(napi, skb, dev_gro_receive(napi, skb));
//    trace_napi_gro_receive_exit(ret);

    return ret;
}

/**
 * eth_type_trans - determine the packet's protocol ID.
 * @skb: received socket data
 * @dev: receiving network device
 *
 * The rule here is that we
 * assume 802.3 if the type field is short enough to be a length.
 * This is normal practice and works for any 'now in use' protocol.
 */
static __be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev)
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


static int dev_queue_xmit(struct sk_buff *skb)
{
//    return __dev_queue_xmit(skb, NULL);
    return 0;
}

static int netif_rx(struct sk_buff *skb)
{
//    int ret;
//
//    trace_netif_rx_entry(skb);
//
//    ret = netif_rx_internal(skb);
//    trace_netif_rx_exit(ret);
//
//    return ret;
    return 0;
}
EXPORT_SYMBOL(netif_rx);


#endif /* netdevice_h */
