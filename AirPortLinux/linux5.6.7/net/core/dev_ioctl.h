//
//  dev_ioctl.h
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/27.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef dev_ioctl_h
#define dev_ioctl_h


#include <linux/types.h>



/**
 * struct hwtstamp_config - %SIOCGHWTSTAMP and %SIOCSHWTSTAMP parameter
 *
 * @flags:    no flags defined right now, must be zero for %SIOCSHWTSTAMP
 * @tx_type:    one of HWTSTAMP_TX_*
 * @rx_filter:    one of HWTSTAMP_FILTER_*
 *
 * %SIOCGHWTSTAMP and %SIOCSHWTSTAMP expect a &struct ifreq with a
 * ifr_data pointer to this structure.  For %SIOCSHWTSTAMP, if the
 * driver or hardware does not support the requested @rx_filter value,
 * the driver may use a more general filter mode.  In this case
 * @rx_filter will indicate the actual mode on return.
 */
struct hwtstamp_config {
    int flags;
    int tx_type;
    int rx_filter;
};

/* possible values for hwtstamp_config->tx_type */
enum hwtstamp_tx_types {
    /*
     * No outgoing packet will need hardware time stamping;
     * should a packet arrive which asks for it, no hardware
     * time stamping will be done.
     */
    HWTSTAMP_TX_OFF,

    /*
     * Enables hardware time stamping for outgoing packets;
     * the sender of the packet decides which are to be
     * time stamped by setting %SOF_TIMESTAMPING_TX_SOFTWARE
     * before sending the packet.
     */
    HWTSTAMP_TX_ON,

    /*
     * Enables time stamping for outgoing packets just as
     * HWTSTAMP_TX_ON does, but also enables time stamp insertion
     * directly into Sync packets. In this case, transmitted Sync
     * packets will not received a time stamp via the socket error
     * queue.
     */
    HWTSTAMP_TX_ONESTEP_SYNC,

    /*
     * Same as HWTSTAMP_TX_ONESTEP_SYNC, but also enables time
     * stamp insertion directly into PDelay_Resp packets. In this
     * case, neither transmitted Sync nor PDelay_Resp packets will
     * receive a time stamp via the socket error queue.
     */
    HWTSTAMP_TX_ONESTEP_P2P,
};

/* possible values for hwtstamp_config->rx_filter */
enum hwtstamp_rx_filters {
    /* time stamp no incoming packet at all */
    HWTSTAMP_FILTER_NONE,

    /* time stamp any incoming packet */
    HWTSTAMP_FILTER_ALL,

    /* return value: time stamp all packets requested plus some others */
    HWTSTAMP_FILTER_SOME,

    /* PTP v1, UDP, any kind of event packet */
    HWTSTAMP_FILTER_PTP_V1_L4_EVENT,
    /* PTP v1, UDP, Sync packet */
    HWTSTAMP_FILTER_PTP_V1_L4_SYNC,
    /* PTP v1, UDP, Delay_req packet */
    HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ,
    /* PTP v2, UDP, any kind of event packet */
    HWTSTAMP_FILTER_PTP_V2_L4_EVENT,
    /* PTP v2, UDP, Sync packet */
    HWTSTAMP_FILTER_PTP_V2_L4_SYNC,
    /* PTP v2, UDP, Delay_req packet */
    HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ,

    /* 802.AS1, Ethernet, any kind of event packet */
    HWTSTAMP_FILTER_PTP_V2_L2_EVENT,
    /* 802.AS1, Ethernet, Sync packet */
    HWTSTAMP_FILTER_PTP_V2_L2_SYNC,
    /* 802.AS1, Ethernet, Delay_req packet */
    HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ,

    /* PTP v2/802.AS1, any layer, any kind of event packet */
    HWTSTAMP_FILTER_PTP_V2_EVENT,
    /* PTP v2/802.AS1, any layer, Sync packet */
    HWTSTAMP_FILTER_PTP_V2_SYNC,
    /* PTP v2/802.AS1, any layer, Delay_req packet */
    HWTSTAMP_FILTER_PTP_V2_DELAY_REQ,

    /* NTP, UDP, all versions and packet modes */
    HWTSTAMP_FILTER_NTP_ALL,
};


#define SOCK_IOC_TYPE    0x89

/*
 * the timeval/timespec data structure layout is defined by libc,
 * so we need to cover both possible versions on 32-bit.
 */
/* Get stamp (timeval) */
#define SIOCGSTAMP_NEW     _IOR(SOCK_IOC_TYPE, 0x06, long long[2])
/* Get stamp (timespec) */
#define SIOCGSTAMPNS_NEW _IOR(SOCK_IOC_TYPE, 0x07, long long[2])

#if __BITS_PER_LONG == 64 || (defined(__x86_64__) && defined(__ILP32__))
/* on 64-bit and x32, avoid the ?: operator */
#define SIOCGSTAMP    SIOCGSTAMP_OLD
#define SIOCGSTAMPNS    SIOCGSTAMPNS_OLD
#else
#define SIOCGSTAMP    ((sizeof(struct timeval))  == 8 ? \
             SIOCGSTAMP_OLD   : SIOCGSTAMP_NEW)
#define SIOCGSTAMPNS    ((sizeof(struct timespec)) == 8 ? \
             SIOCGSTAMPNS_OLD : SIOCGSTAMPNS_NEW)
#endif

/* Routing table calls. */
#define SIOCADDRT    0x890B        /* add routing table entry    */
#define SIOCDELRT    0x890C        /* delete routing table entry    */
#define SIOCRTMSG    0x890D        /* unused            */

/* Socket configuration controls. */
#define SIOCGIFNAME    0x8910        /* get iface name        */
#define SIOCSIFLINK    0x8911        /* set iface channel        */
#define SIOCGIFCONF    0x8912        /* get iface list        */
#undef SIOCGIFFLAGS
#define SIOCGIFFLAGS    0x8913        /* get flags            */
#undef SIOCSIFFLAGS
#define SIOCSIFFLAGS    0x8914        /* set flags            */
#undef SIOCGIFADDR
#define SIOCGIFADDR    0x8915        /* get PA address        */
#undef SIOCSIFADDR
#define SIOCSIFADDR    0x8916        /* set PA address        */
#undef SIOCGIFDSTADDR
#define SIOCGIFDSTADDR    0x8917        /* get remote PA address    */
#undef SIOCSIFDSTADDR
#define SIOCSIFDSTADDR    0x8918        /* set remote PA address    */
#undef SIOCGIFBRDADDR
#define SIOCGIFBRDADDR    0x8919        /* get broadcast PA address    */
#undef SIOCSIFBRDADDR
#define SIOCSIFBRDADDR    0x891a        /* set broadcast PA address    */
#undef SIOCGIFNETMASK
#define SIOCGIFNETMASK    0x891b        /* get network PA mask        */
#undef SIOCSIFNETMASK
#define SIOCSIFNETMASK    0x891c        /* set network PA mask        */
#undef SIOCGIFMETRIC
#define SIOCGIFMETRIC    0x891d        /* get metric            */
#undef SIOCSIFMETRIC
#define SIOCSIFMETRIC    0x891e        /* set metric            */
#define SIOCGIFMEM    0x891f        /* get memory address (BSD)    */
#define SIOCSIFMEM    0x8920        /* set memory address (BSD)    */
#undef SIOCGIFMTU
#define SIOCGIFMTU    0x8921        /* get MTU size            */
#undef SIOCSIFMTU
#define SIOCSIFMTU    0x8922        /* set MTU size            */
#define SIOCSIFNAME    0x8923        /* set interface name */
#define    SIOCSIFHWADDR    0x8924        /* set hardware address     */
#define SIOCGIFENCAP    0x8925        /* get/set encapsulations       */
#define SIOCSIFENCAP    0x8926
#define SIOCGIFHWADDR    0x8927        /* Get hardware address        */
#define SIOCGIFSLAVE    0x8929        /* Driver slaving support    */
#define SIOCSIFSLAVE    0x8930
#undef SIOCADDMULTI
#define SIOCADDMULTI    0x8931        /* Multicast address lists    */
#undef SIOCDELMULTI
#define SIOCDELMULTI    0x8932
#define SIOCGIFINDEX    0x8933        /* name -> if_index mapping    */
#define SIOGIFINDEX    SIOCGIFINDEX    /* misprint compatibility :-)    */
#define SIOCSIFPFLAGS    0x8934        /* set/get extended flags set    */
#define SIOCGIFPFLAGS    0x8935
#undef SIOCDIFADDR
#define SIOCDIFADDR    0x8936        /* delete PA address        */
#define    SIOCSIFHWBROADCAST    0x8937    /* set hardware broadcast addr    */
#define SIOCGIFCOUNT    0x8938        /* get number of devices */

#define SIOCGIFBR    0x8940        /* Bridging support        */
#define SIOCSIFBR    0x8941        /* Set bridging options     */

#define SIOCGIFTXQLEN    0x8942        /* Get the tx queue length    */
#define SIOCSIFTXQLEN    0x8943        /* Set the tx queue length     */

/* SIOCGIFDIVERT was:    0x8944        Frame diversion support */
/* SIOCSIFDIVERT was:    0x8945        Set frame diversion options */

#define SIOCETHTOOL    0x8946        /* Ethtool interface        */

#define SIOCGMIIPHY    0x8947        /* Get address of MII PHY in use. */
#define SIOCGMIIREG    0x8948        /* Read MII PHY register.    */
#define SIOCSMIIREG    0x8949        /* Write MII PHY register.    */

#define SIOCWANDEV    0x894A        /* get/set netdev parameters    */

#define SIOCOUTQNSD    0x894B        /* output queue size (not sent only) */
#define SIOCGSKNS    0x894C        /* get socket network namespace */

/* ARP cache control calls. */
            /*  0x8950 - 0x8952  * obsolete calls, don't re-use */
#define SIOCDARP    0x8953        /* delete ARP table entry    */
#define SIOCGARP    0x8954        /* get ARP table entry        */
#define SIOCSARP    0x8955        /* set ARP table entry        */

/* RARP cache control calls. */
#define SIOCDRARP    0x8960        /* delete RARP table entry    */
#define SIOCGRARP    0x8961        /* get RARP table entry        */
#define SIOCSRARP    0x8962        /* set RARP table entry        */

/* Driver configuration calls */

#define SIOCGIFMAP    0x8970        /* Get device parameters    */
#define SIOCSIFMAP    0x8971        /* Set device parameters    */

/* DLCI configuration calls */

#define SIOCADDDLCI    0x8980        /* Create new DLCI device    */
#define SIOCDELDLCI    0x8981        /* Delete DLCI device        */

#define SIOCGIFVLAN    0x8982        /* 802.1Q VLAN support        */
#define SIOCSIFVLAN    0x8983        /* Set 802.1Q VLAN options     */

/* bonding calls */

#define SIOCBONDENSLAVE    0x8990        /* enslave a device to the bond */
#define SIOCBONDRELEASE 0x8991        /* release a slave from the bond*/
#define SIOCBONDSETHWADDR      0x8992    /* set the hw addr of the bond  */
#define SIOCBONDSLAVEINFOQUERY 0x8993   /* rtn info about slave state   */
#define SIOCBONDINFOQUERY      0x8994    /* rtn info about bond state    */
#define SIOCBONDCHANGEACTIVE   0x8995   /* update to a new active slave */
            
/* bridge calls */
#define SIOCBRADDBR     0x89a0        /* create new bridge device     */
#define SIOCBRDELBR     0x89a1        /* remove bridge device         */
#define SIOCBRADDIF    0x89a2        /* add interface to bridge      */
#define SIOCBRDELIF    0x89a3        /* remove interface from bridge */

/* hardware time stamping: parameters in linux/net_tstamp.h */
#define SIOCSHWTSTAMP    0x89b0        /* set and get config        */
#define SIOCGHWTSTAMP    0x89b1        /* get config            */

/* Device private ioctl calls */

/*
 *    These 16 ioctls are available to devices via the do_ioctl() device
 *    vector. Each device should include this file and redefine these names
 *    as their own. Because these are device dependent it is a good idea
 *    _NOT_ to issue them to random objects and hope.
 *
 *    THESE IOCTLS ARE _DEPRECATED_ AND WILL DISAPPEAR IN 2.5.X -DaveM
 */
 
#define SIOCDEVPRIVATE    0x89F0    /* to 89FF */

/*
 *    These 16 ioctl calls are protocol private
 */
 
#define SIOCPROTOPRIVATE 0x89E0 /* to 89EF */




//#define AF_MAX        45    /* For now.. */

#define NPROTO        AF_MAX

#define __UAPI_DEF_IF_IFREQ     1
#define __UAPI_DEF_IF_IFCONF    1
#define __UAPI_DEF_IF_IFMAP     1

/* for compatibility with glibc net/if.h */
#if __UAPI_DEF_IF_IFMAP
struct ifmap {
    unsigned long mem_start;
    unsigned long mem_end;
    unsigned short base_addr;
    unsigned char irq;
    unsigned char dma;
    unsigned char port;
    /* 3 bytes spare */
};
#endif /* __UAPI_DEF_IF_IFMAP */

struct if_settings {
    unsigned int type;    /* Type of physical device or protocol */
    unsigned int size;    /* Size of the data allocated by the caller */
//    union {
//        /* {atm/eth/dsl}_settings anyone ? */
//        raw_hdlc_proto        __user *raw_hdlc;
//        cisco_proto        __user *cisco;
//        fr_proto        __user *fr;
//        fr_proto_pvc        __user *fr_pvc;
//        fr_proto_pvc_info    __user *fr_pvc_info;
//        x25_hdlc_proto        __user *x25;
//
//        /* interface settings */
//        sync_serial_settings    __user *sync;
//        te1_settings        __user *te1;
//    } ifs_ifsu;
};


/* for compatibility with glibc net/if.h */
#if __UAPI_DEF_IF_IFREQ
struct _ifreq {
#define IFHWADDRLEN    6
    union
    {
        char    ifrn_name[IFNAMSIZ];        /* if name, e.g. "en0" */
    } ifr_ifrn;
    
    union {
        struct    sockaddr ifru_addr;
        struct    sockaddr ifru_dstaddr;
        struct    sockaddr ifru_broadaddr;
        struct    sockaddr ifru_netmask;
        struct  sockaddr ifru_hwaddr;
        short    ifru_flags;
        int    ifru_ivalue;
        int    ifru_mtu;
        struct  ifmap ifru_map;
        char    ifru_slave[IFNAMSIZ];    /* Just fits the size */
        char    ifru_newname[IFNAMSIZ];
        void __user *    ifru_data;
        struct    if_settings ifru_settings;
    } ifr_ifru;
};
#endif /* __UAPI_DEF_IF_IFREQ */

#define ifr_name    ifr_ifrn.ifrn_name    /* interface name     */
#define ifr_hwaddr    ifr_ifru.ifru_hwaddr    /* MAC address         */
#define    ifr_addr    ifr_ifru.ifru_addr    /* address        */
#define    ifr_dstaddr    ifr_ifru.ifru_dstaddr    /* other end of p-p lnk    */
#define    ifr_broadaddr    ifr_ifru.ifru_broadaddr    /* broadcast address    */
#define    ifr_netmask    ifr_ifru.ifru_netmask    /* interface net mask    */
#define    ifr_flags    ifr_ifru.ifru_flags    /* flags        */
#undef ifr_metric
#define    ifr_metric    ifr_ifru.ifru_ivalue    /* metric        */
#define    ifr_mtu        ifr_ifru.ifru_mtu    /* mtu            */
#define ifr_map        ifr_ifru.ifru_map    /* device map        */
#define ifr_slave    ifr_ifru.ifru_slave    /* slave device        */
#define    ifr_data    ifr_ifru.ifru_data    /* for use by interface    */
#define ifr_ifindex    ifr_ifru.ifru_ivalue    /* interface index    */
#define ifr_bandwidth    ifr_ifru.ifru_ivalue    /* link bandwidth    */
#define ifr_qlen    ifr_ifru.ifru_ivalue    /* Queue length     */
#define ifr_newname    ifr_ifru.ifru_newname    /* New name        */
#define ifr_settings    ifr_ifru.ifru_settings    /* Device/proto settings*/

/*
 * Structure used in SIOCGIFCONF request.
 * Used to retrieve interface configuration
 * for machine (useful for programs which
 * must know all networks accessible).
 */

/* for compatibility with glibc net/if.h */
#if __UAPI_DEF_IF_IFCONF
struct ifconf  {
    int    ifc_len;            /* size of buffer    */
    union {
        char __user *ifcu_buf;
        struct _ifreq __user *ifcu_req;
    } ifc_ifcu;
};
#endif /* __UAPI_DEF_IF_IFCONF */

#define    ifc_buf    ifc_ifcu.ifcu_buf        /* buffer address    */
#define    ifc_req    ifc_ifcu.ifcu_req        /* array of structures    */


#endif /* dev_ioctl_h */
