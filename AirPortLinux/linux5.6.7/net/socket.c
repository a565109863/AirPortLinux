// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * NET        An implementation of the SOCKET network access protocol.
 *
 * Version:    @(#)socket.c    1.1.93    18/02/95
 *
 * Authors:    Orest Zborowski, <obz@Kodak.COM>
 *        Ross Biro
 *        Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 * Fixes:
 *        Anonymous    :    NOTSOCK/BADF cleanup. Error fix in
 *                    shutdown()
 *        Alan Cox    :    verify_area() fixes
 *        Alan Cox    :    Removed DDI
 *        Jonathan Kamens    :    SOCK_DGRAM reconnect bug
 *        Alan Cox    :    Moved a load of checks to the very
 *                    top level.
 *        Alan Cox    :    Move address structures to/from user
 *                    mode above the protocol layers.
 *        Rob Janssen    :    Allow 0 length sends.
 *        Alan Cox    :    Asynchronous I/O support (cribbed from the
 *                    tty drivers).
 *        Niibe Yutaka    :    Asynchronous I/O for writes (4.4BSD style)
 *        Jeff Uphoff    :    Made max number of sockets command-line
 *                    configurable.
 *        Matti Aarnio    :    Made the number of sockets dynamic,
 *                    to be allocated when needed, and mr.
 *                    Uphoff's max is used as max to be
 *                    allowed to allocate.
 *        Linus        :    Argh. removed all the socket allocation
 *                    altogether: it's in the inode now.
 *        Alan Cox    :    Made sock_alloc()/sock_release() public
 *                    for NetROM and future kernel nfsd type
 *                    stuff.
 *        Alan Cox    :    sendmsg/recvmsg basics.
 *        Tom Dyas    :    Export net symbols.
 *        Marcin Dalecki    :    Fixed problems with CONFIG_NET="n".
 *        Alan Cox    :    Added thread locking to sys_* calls
 *                    for sockets. May have errors at the
 *                    moment.
 *        Kevin Buhr    :    Fixed the dumb errors in the above.
 *        Andi Kleen    :    Some small cleanups, optimizations,
 *                    and fixed a copy_from_user() bug.
 *        Tigran Aivazian    :    sys_send(args) calls sys_sendto(args, NULL, 0)
 *        Tigran Aivazian    :    Made listen(2) backlog sanity checks
 *                    protocol-independent
 *
 *    This module is effectively the top level interface to the BSD socket
 *    paradigm.
 *
 *    Based upon Swansea University Computer Society NET3.039
 */

#include <linux/mm.h>
#include <linux/socket.h>
//#include <linux/file.h>
#include <linux/net.h>
#include <linux/interrupt.h>
//#include <linux/thread_info.h>
#include <linux/rcupdate.h>
#include <linux/netdevice.h>
//#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mutex.h>
//#include <linux/if_bridge.h>
//#include <linux/if_frad.h>
#include <linux/if_vlan.h>
//#include <linux/ptp_classify.h>
#include <linux/init.h>
//#include <linux/poll.h>
//#include <linux/cache.h>
#include <linux/module.h>
#include <linux/highmem.h>
//#include <linux/mount.h>
//#include <linux/pseudo_fs.h>
//#include <linux/security.h>
//#include <linux/syscalls.h>
#include <linux/compat.h>
//#include <linux/kmod.h>
//#include <linux/audit.h>
#include <linux/wireless.h>
//#include <linux/nsproxy.h>
//#include <linux/magic.h>
#include <linux/slab.h>
//#include <linux/xattr.h>
//#include <linux/nospec.h>
//#include <linux/indirect_call_wrapper.h>

#include <linux/uaccess.h>
//#include <asm/unistd.h>

#include <net/compat.h>
#include <net/wext.h>
//#include <net/cls_cgroup.h>

#include <net/sock.h>
//#include <linux/netfilter.h>

//#include <linux/if_tun.h>
//#include <linux/ipv6_route.h>
//#include <linux/route.h>
//#include <linux/termios.h>
//#include <linux/sockios.h>
//#include <net/busy_poll.h>
//#include <linux/errqueue.h>



static long sock_do_ioctl(struct net *net, struct socket *sock,
              unsigned int cmd, unsigned long arg)
{
    int err;
    void __user *argp = (void __user *)arg;

//    err = sock->ops->ioctl(sock, cmd, arg);
//
//    /*
//     * If this ioctl is unknown try to hand it down
//     * to the NIC driver.
//     */
//    if (err != -ENOIOCTLCMD)
//        return err;

    if (cmd == SIOCGIFCONF) {
        struct ifconf ifc;
        if (copy_from_user(&ifc, argp, sizeof(struct ifconf)))
            return -EFAULT;
        rtnl_lock();
        err = dev_ifconf(net, &ifc, sizeof(struct _ifreq));
        rtnl_unlock();
        if (!err && copy_to_user(argp, &ifc, sizeof(struct ifconf)))
            err = -EFAULT;
    } else {
        struct _ifreq ifr;
        bool need_copyout;
        if (copy_from_user(&ifr, argp, sizeof(struct _ifreq)))
            return -EFAULT;
        err = dev_ioctl(net, cmd, &ifr, &need_copyout);
        if (!err && need_copyout)
            if (copy_to_user(argp, &ifr, sizeof(struct _ifreq)))
                return -EFAULT;
    }
    return err;
}

long sock_ioctl(int sk, unsigned cmd, unsigned long arg)
{
    struct socket *sock;
//    struct sock *sk;
    void __user *argp = (void __user *)arg;
    int pid, err;
    struct net *net;

//    sock = file->private_data;
//    sk = sock->sk;
    net = sock_net(sk);
    if (unlikely(cmd >= SIOCDEVPRIVATE && cmd <= (SIOCDEVPRIVATE + 15))) {
        struct _ifreq ifr;
        bool need_copyout;
        if (copy_from_user(&ifr, argp, sizeof(struct _ifreq)))
            return -EFAULT;
        err = dev_ioctl(net, cmd, &ifr, &need_copyout);
        if (!err && need_copyout)
            if (copy_to_user(argp, &ifr, sizeof(struct _ifreq)))
                return -EFAULT;
    } else
#ifdef CONFIG_WEXT_CORE
    if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST) {
        err = wext_handle_ioctl(net, cmd, argp);
    } else
#endif
        switch (cmd) {
        case FIOSETOWN:
        case SIOCSPGRP:
            err = -EFAULT;
//            if (get_user(pid, (int __user *)argp))
//                break;
//            err = f_setown(sock->file, pid, 1);
            break;
        case FIOGETOWN:
        case SIOCGPGRP:
//            err = put_user(f_getown(sock->file),
//                       (int __user *)argp);
            break;
        case SIOCGIFBR:
        case SIOCSIFBR:
        case SIOCBRADDBR:
        case SIOCBRDELBR:
            err = -ENOPKG;
//            if (!br_ioctl_hook)
//                request_module("bridge");
//
//            mutex_lock(&br_ioctl_mutex);
//            if (br_ioctl_hook)
//                err = br_ioctl_hook(net, cmd, argp);
//            mutex_unlock(&br_ioctl_mutex);
            break;
        case SIOCGIFVLAN:
        case SIOCSIFVLAN:
            err = -ENOPKG;
//            if (!vlan_ioctl_hook)
//                request_module("8021q");
//
//            mutex_lock(&vlan_ioctl_mutex);
//            if (vlan_ioctl_hook)
//                err = vlan_ioctl_hook(net, argp);
//            mutex_unlock(&vlan_ioctl_mutex);
            break;
        case SIOCADDDLCI:
        case SIOCDELDLCI:
            err = -ENOPKG;
//            if (!dlci_ioctl_hook)
//                request_module("dlci");
//
//            mutex_lock(&dlci_ioctl_mutex);
//            if (dlci_ioctl_hook)
//                err = dlci_ioctl_hook(cmd, argp);
//            mutex_unlock(&dlci_ioctl_mutex);
            break;
        case SIOCGSKNS:
            err = -EPERM;
//            if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
//                break;
//
//            err = open_related_ns(&net->ns, get_net_ns);
            break;
//        case SIOCGSTAMP_OLD:
//        case SIOCGSTAMPNS_OLD:
//            if (!sock->ops->gettstamp) {
//                err = -ENOIOCTLCMD;
//                break;
//            }
//            err = sock->ops->gettstamp(sock, argp,
//                           cmd == SIOCGSTAMP_OLD,
//                           !IS_ENABLED(CONFIG_64BIT));
//            break;
        case SIOCGSTAMP_NEW:
        case SIOCGSTAMPNS_NEW:
//            if (!sock->ops->gettstamp) {
//                err = -ENOIOCTLCMD;
//                break;
//            }
//            err = sock->ops->gettstamp(sock, argp,
//                           cmd == SIOCGSTAMP_NEW,
//                           false);
            break;
        default:
            err = sock_do_ioctl(net, sock, cmd, arg);
            break;
        }
    return err;
}
