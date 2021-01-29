//
//  rtnetlink.h
//  AirPortLinux
//
//  Created by Zhong-Mac on 2020/12/6.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef rtnetlink_h
#define rtnetlink_h

#include <net/netlink.h>


/* This struct should be in sync with struct rtnl_link_stats64 */
struct rtnl_link_stats {
    __u32    rx_packets;        /* total packets received    */
    __u32    tx_packets;        /* total packets transmitted    */
    __u32    rx_bytes;        /* total bytes received     */
    __u32    tx_bytes;        /* total bytes transmitted    */
    __u32    rx_errors;        /* bad packets received        */
    __u32    tx_errors;        /* packet transmit problems    */
    __u32    rx_dropped;        /* no space in linux buffers    */
    __u32    tx_dropped;        /* no space available in linux    */
    __u32    multicast;        /* multicast packets received    */
    __u32    collisions;

    /* detailed rx_errors: */
    __u32    rx_length_errors;
    __u32    rx_over_errors;        /* receiver ring buff overflow    */
    __u32    rx_crc_errors;        /* recved pkt with crc error    */
    __u32    rx_frame_errors;    /* recv'd frame alignment error */
    __u32    rx_fifo_errors;        /* recv'r fifo overrun        */
    __u32    rx_missed_errors;    /* receiver missed packet    */

    /* detailed tx_errors */
    __u32    tx_aborted_errors;
    __u32    tx_carrier_errors;
    __u32    tx_fifo_errors;
    __u32    tx_heartbeat_errors;
    __u32    tx_window_errors;

    /* for cslip etc */
    __u32    rx_compressed;
    __u32    tx_compressed;

    __u32    rx_nohandler;        /* dropped, no handler found    */
};

/* The main device statistics structure */
struct rtnl_link_stats64 {
    __u64    rx_packets;        /* total packets received    */
    __u64    tx_packets;        /* total packets transmitted    */
    __u64    rx_bytes;        /* total bytes received     */
    __u64    tx_bytes;        /* total bytes transmitted    */
    __u64    rx_errors;        /* bad packets received        */
    __u64    tx_errors;        /* packet transmit problems    */
    __u64    rx_dropped;        /* no space in linux buffers    */
    __u64    tx_dropped;        /* no space available in linux    */
    __u64    multicast;        /* multicast packets received    */
    __u64    collisions;

    /* detailed rx_errors: */
    __u64    rx_length_errors;
    __u64    rx_over_errors;        /* receiver ring buff overflow    */
    __u64    rx_crc_errors;        /* recved pkt with crc error    */
    __u64    rx_frame_errors;    /* recv'd frame alignment error */
    __u64    rx_fifo_errors;        /* recv'r fifo overrun        */
    __u64    rx_missed_errors;    /* receiver missed packet    */

    /* detailed tx_errors */
    __u64    tx_aborted_errors;
    __u64    tx_carrier_errors;
    __u64    tx_fifo_errors;
    __u64    tx_heartbeat_errors;
    __u64    tx_window_errors;

    /* for cslip etc */
    __u64    rx_compressed;
    __u64    tx_compressed;

    __u64    rx_nohandler;        /* dropped, no handler found    */
};

/* The struct should be in sync with struct ifmap */
struct rtnl_link_ifmap {
    __u64    mem_start;
    __u64    mem_end;
    __u64    base_addr;
    __u16    irq;
    __u8    dma;
    __u8    port;
};


struct rtnl_af_ops {
   struct list_head    list;
   int            family;

   int            (*fill_link_af)(struct sk_buff *skb,
                       const struct net_device *dev,
                       u32 ext_filter_mask);
   size_t            (*get_link_af_size)(const struct net_device *dev,
                           u32 ext_filter_mask);

   int            (*validate_link_af)(const struct net_device *dev,
                           const struct nlattr *attr);
   int            (*set_link_af)(struct net_device *dev,
                          const struct nlattr *attr);

   int            (*fill_stats_af)(struct sk_buff *skb,
                        const struct net_device *dev);
   size_t            (*get_stats_af_size)(const struct net_device *dev);
};

#endif /* rtnetlink_h */
