//
//  socket.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/7/1.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef socket_h
#define socket_h

/* Supported address families. */
#define AF_UNSPEC    0
#define AF_UNIX        1    /* Unix domain sockets         */
#define AF_LOCAL    1    /* POSIX name for AF_UNIX    */
#define AF_INET        2    /* Internet IP Protocol     */
#define AF_AX25        3    /* Amateur Radio AX.25         */
#define AF_IPX        4    /* Novell IPX             */
#define AF_APPLETALK    5    /* AppleTalk DDP         */
#define AF_NETROM    6    /* Amateur Radio NET/ROM     */
#define AF_BRIDGE    7    /* Multiprotocol bridge     */
#define AF_ATMPVC    8    /* ATM PVCs            */
#define AF_X25        9    /* Reserved for X.25 project     */
#define AF_INET6    10    /* IP version 6            */
#define AF_ROSE        11    /* Amateur Radio X.25 PLP    */
#define AF_DECnet    12    /* Reserved for DECnet project    */
#define AF_NETBEUI    13    /* Reserved for 802.2LLC project*/
#define AF_SECURITY    14    /* Security callback pseudo AF */
#define AF_KEY        15      /* PF_KEY key management API */
#define AF_NETLINK    16
#define AF_ROUTE    AF_NETLINK /* Alias to emulate 4.4BSD */
#define AF_PACKET    17    /* Packet family        */
#define AF_ASH        18    /* Ash                */
#define AF_ECONET    19    /* Acorn Econet            */
#define AF_ATMSVC    20    /* ATM SVCs            */
#define AF_RDS        21    /* RDS sockets             */
#define AF_SNA        22    /* Linux SNA Project (nutters!) */
#define AF_IRDA        23    /* IRDA sockets            */
#define AF_PPPOX    24    /* PPPoX sockets        */
#define AF_WANPIPE    25    /* Wanpipe API Sockets */
#define AF_LLC        26    /* Linux LLC            */
#define AF_IB        27    /* Native InfiniBand address    */
#define AF_MPLS        28    /* MPLS */
#define AF_CAN        29    /* Controller Area Network      */
#define AF_TIPC        30    /* TIPC sockets            */
#define AF_BLUETOOTH    31    /* Bluetooth sockets         */
#define AF_IUCV        32    /* IUCV sockets            */
#define AF_RXRPC    33    /* RxRPC sockets         */
#define AF_ISDN        34    /* mISDN sockets         */
#define AF_PHONET    35    /* Phonet sockets        */
#define AF_IEEE802154    36    /* IEEE802154 sockets        */
#define AF_CAIF        37    /* CAIF sockets            */
#define AF_ALG        38    /* Algorithm sockets        */
#define AF_NFC        39    /* NFC sockets            */
#define AF_VSOCK    40    /* vSockets            */
#define AF_KCM        41    /* Kernel Connection Multiplexor*/
#define AF_QIPCRTR    42    /* Qualcomm IPC Router          */
#define AF_SMC        43    /* smc sockets: reserve number for
* PF_SMC protocol family that
* reuses AF_INET address family
*/
#define AF_XDP        44    /* XDP sockets            */

#define AF_MAX        45    /* For now.. */

/* Protocol families, same as address families. */
#define PF_UNSPEC    AF_UNSPEC
#define PF_UNIX        AF_UNIX
#define PF_LOCAL    AF_LOCAL
#define PF_INET        AF_INET
#define PF_AX25        AF_AX25
#define PF_IPX        AF_IPX
#define PF_APPLETALK    AF_APPLETALK
#define    PF_NETROM    AF_NETROM
#define PF_BRIDGE    AF_BRIDGE
#define PF_ATMPVC    AF_ATMPVC
#define PF_X25        AF_X25
#define PF_INET6    AF_INET6
#define PF_ROSE        AF_ROSE
#define PF_DECnet    AF_DECnet
#define PF_NETBEUI    AF_NETBEUI
#define PF_SECURITY    AF_SECURITY
#define PF_KEY        AF_KEY
#define PF_NETLINK    AF_NETLINK
#define PF_ROUTE    AF_ROUTE
#define PF_PACKET    AF_PACKET
#define PF_ASH        AF_ASH
#define PF_ECONET    AF_ECONET
#define PF_ATMSVC    AF_ATMSVC
#define PF_RDS        AF_RDS
#define PF_SNA        AF_SNA
#define PF_IRDA        AF_IRDA
#define PF_PPPOX    AF_PPPOX
#define PF_WANPIPE    AF_WANPIPE
#define PF_LLC        AF_LLC
#define PF_IB        AF_IB
#define PF_MPLS        AF_MPLS
#define PF_CAN        AF_CAN
#define PF_TIPC        AF_TIPC
#define PF_BLUETOOTH    AF_BLUETOOTH
#define PF_IUCV        AF_IUCV
#define PF_RXRPC    AF_RXRPC
#define PF_ISDN        AF_ISDN
#define PF_PHONET    AF_PHONET
#define PF_IEEE802154    AF_IEEE802154
#define PF_CAIF        AF_CAIF
#define PF_ALG        AF_ALG
#define PF_NFC        AF_NFC
#define PF_VSOCK    AF_VSOCK
#define PF_KCM        AF_KCM
#define PF_QIPCRTR    AF_QIPCRTR
#define PF_SMC        AF_SMC
#define PF_XDP        AF_XDP
#define PF_MAX        AF_MAX


/*
 * Desired design of maximum size and alignment (see RFC2553)
 */
#define _K_SS_MAXSIZE    128    /* Implementation specific max size */
#define _K_SS_ALIGNSIZE    (__alignof__ (struct sockaddr *))
                /* Implementation specific desired alignment */

typedef unsigned short __kernel_sa_family_t;

struct __kernel_sockaddr_storage {
    __kernel_sa_family_t    ss_family;        /* address family */
    /* Following field(s) are implementation specific */
    char        __data[_K_SS_MAXSIZE - sizeof(unsigned short)];
                /* space to achieve desired size, */
                /* _SS_MAXSIZE value minus size of ss_family */
} __attribute__ ((aligned(_K_SS_ALIGNSIZE)));    /* force desired alignment */


#define __sockaddr_check_size(size)    \
    BUILD_BUG_ON(((size) > sizeof(struct __kernel_sockaddr_storage)))




/* Setsockoptions(2) level. Thanks to BSD these must match IPPROTO_xxx */
#define SOL_IP        0
/* #define SOL_ICMP    1    No-no-no! Due to Linux :-) we cannot use SOL_ICMP=1 */
#define SOL_TCP        6
#define SOL_UDP        17
#define SOL_IPV6    41
#define SOL_ICMPV6    58
#define SOL_SCTP    132
#define SOL_UDPLITE    136     /* UDP-Lite (RFC 3828) */
#define SOL_RAW        255
#define SOL_IPX        256
#define SOL_AX25    257
#define SOL_ATALK    258
#define SOL_NETROM    259
#define SOL_ROSE    260
#define SOL_DECNET    261
#define    SOL_X25        262
#define SOL_PACKET    263
#define SOL_ATM        264    /* ATM layer (cell level) */
#define SOL_AAL        265    /* ATM Adaption Layer (packet level) */
#define SOL_IRDA        266
#define SOL_NETBEUI    267
#define SOL_LLC        268
#define SOL_DCCP    269
#define SOL_NETLINK    270
#define SOL_TIPC    271
#define SOL_RXRPC    272
#define SOL_PPPOL2TP    273
#define SOL_BLUETOOTH    274
#define SOL_PNPIPE    275
#define SOL_RDS        276
#define SOL_IUCV    277
#define SOL_CAIF    278
#define SOL_ALG        279
#define SOL_NFC        280
#define SOL_KCM        281
#define SOL_TLS        282
#define SOL_XDP        283

/* IPX options */
#define IPX_TYPE    1

//extern int move_addr_to_kernel(void __user *uaddr, int ulen, struct sockaddr_storage *kaddr);
extern int put_cmsg(struct msghdr*, int level, int type, int len, void *data);


//int netlink_send(struct sock *sock, int group, u16 type, void *msg, int len,
//struct net_device *dev);
//
//#define call_int_hook(FUNC, g, sk, skb) (netlink_send(FUNC, IRC, __VA_ARGS__))

static inline int security_netlink_send(struct sock *sk, struct sk_buff *skb)
{
//    return call_int_hook(netlink_send, 0, sk, skb);
    return 0;
}


static inline void sock_prot_inuse_add(struct net *net, struct proto *prot,
        int inc)
{
}

#endif /* socket_h */
