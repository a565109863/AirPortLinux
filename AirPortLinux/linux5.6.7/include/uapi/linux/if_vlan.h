//
//  if_vlan.h
//  AirPortLinux
//
//  Created by Zhong-Mac on 2020/12/1.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef if_vlan_h
#define if_vlan_h



/* VF ports management section
 *
 *    Nested layout of set/get msg is:
 *
 *        [IFLA_NUM_VF]
 *        [IFLA_VF_PORTS]
 *            [IFLA_VF_PORT]
 *                [IFLA_PORT_*], ...
 *            [IFLA_VF_PORT]
 *                [IFLA_PORT_*], ...
 *            ...
 *        [IFLA_PORT_SELF]
 *            [IFLA_PORT_*], ...
 */

enum {
    IFLA_VF_PORT_UNSPEC,
    IFLA_VF_PORT,            /* nest */
    __IFLA_VF_PORT_MAX,
};

#define IFLA_VF_PORT_MAX (__IFLA_VF_PORT_MAX - 1)

enum {
    IFLA_PORT_UNSPEC,
    IFLA_PORT_VF,            /* __u32 */
    IFLA_PORT_PROFILE,        /* string */
    IFLA_PORT_VSI_TYPE,        /* 802.1Qbg (pre-)standard VDP */
    IFLA_PORT_INSTANCE_UUID,    /* binary UUID */
    IFLA_PORT_HOST_UUID,        /* binary UUID */
    IFLA_PORT_REQUEST,        /* __u8 */
    IFLA_PORT_RESPONSE,        /* __u16, output only */
    __IFLA_PORT_MAX,
};

#define IFLA_PORT_MAX (__IFLA_PORT_MAX - 1)

#define PORT_PROFILE_MAX    40
#define PORT_UUID_MAX        16
#define PORT_SELF_VF        -1



enum {
    IFLA_UNSPEC,
    IFLA_ADDRESS,
    IFLA_BROADCAST,
    IFLA_IFNAME,
    IFLA_MTU,
    IFLA_LINK,
    IFLA_QDISC,
    IFLA_STATS,
    IFLA_COST,
#define IFLA_COST IFLA_COST
    IFLA_PRIORITY,
#define IFLA_PRIORITY IFLA_PRIORITY
    IFLA_MASTER,
#define IFLA_MASTER IFLA_MASTER
    IFLA_WIRELESS,        /* Wireless Extension event - see wireless.h */
#define IFLA_WIRELESS IFLA_WIRELESS
    IFLA_PROTINFO,        /* Protocol specific information for a link */
#define IFLA_PROTINFO IFLA_PROTINFO
    IFLA_TXQLEN,
#define IFLA_TXQLEN IFLA_TXQLEN
    IFLA_MAP,
#define IFLA_MAP IFLA_MAP
    IFLA_WEIGHT,
#define IFLA_WEIGHT IFLA_WEIGHT
    IFLA_OPERSTATE,
    IFLA_LINKMODE,
    IFLA_LINKINFO,
#define IFLA_LINKINFO IFLA_LINKINFO
    IFLA_NET_NS_PID,
    IFLA_IFALIAS,
    IFLA_NUM_VF,        /* Number of VFs if device is SR-IOV PF */
    IFLA_VFINFO_LIST,
    IFLA_STATS64,
    IFLA_VF_PORTS,
    IFLA_PORT_SELF,
    IFLA_AF_SPEC,
    IFLA_GROUP,        /* Group the device belongs to */
    IFLA_NET_NS_FD,
    IFLA_EXT_MASK,        /* Extended info mask, VFs, etc */
    IFLA_PROMISCUITY,    /* Promiscuity count: > 0 means acts PROMISC */
#define IFLA_PROMISCUITY IFLA_PROMISCUITY
    IFLA_NUM_TX_QUEUES,
    IFLA_NUM_RX_QUEUES,
    IFLA_CARRIER,
    IFLA_PHYS_PORT_ID,
    IFLA_CARRIER_CHANGES,
    IFLA_PHYS_SWITCH_ID,
    IFLA_LINK_NETNSID,
    IFLA_PHYS_PORT_NAME,
    IFLA_PROTO_DOWN,
    IFLA_GSO_MAX_SEGS,
    IFLA_GSO_MAX_SIZE,
    IFLA_PAD,
    IFLA_XDP,
    IFLA_EVENT,
    IFLA_NEW_NETNSID,
    IFLA_IF_NETNSID,
    IFLA_TARGET_NETNSID = IFLA_IF_NETNSID, /* new alias */
    IFLA_CARRIER_UP_COUNT,
    IFLA_CARRIER_DOWN_COUNT,
    IFLA_NEW_IFINDEX,
    IFLA_MIN_MTU,
    IFLA_MAX_MTU,
    IFLA_PROP_LIST,
    IFLA_ALT_IFNAME, /* Alternative ifname */
    IFLA_PERM_ADDRESS,
    __IFLA_MAX
};

enum {
    IFLA_EVENT_NONE,
    IFLA_EVENT_REBOOT,        /* internal reset / reboot */
    IFLA_EVENT_FEATURES,        /* change in offload features */
    IFLA_EVENT_BONDING_FAILOVER,    /* change in active slave */
    IFLA_EVENT_NOTIFY_PEERS,    /* re-sent grat. arp/ndisc */
    IFLA_EVENT_IGMP_RESEND,        /* re-sent IGMP JOIN */
    IFLA_EVENT_BONDING_OPTIONS,    /* change in bonding options */
};

enum {
    IFLA_INFO_UNSPEC,
    IFLA_INFO_KIND,
    IFLA_INFO_DATA,
    IFLA_INFO_XSTATS,
    IFLA_INFO_SLAVE_KIND,
    IFLA_INFO_SLAVE_DATA,
    __IFLA_INFO_MAX,
};

#endif /* if_vlan_h */
