//
//  help_ifconfig.c
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/29.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#include "help_ifconfig.h"
#include <linux/netdevice.h>
#include <net/core/dev_ioctl.h>


int ioctl(struct net *net, unsigned int cmd, struct _ifreq *ifr, bool *need_copyout)
{
    int ret = dev_ioctl(net, cmd, ifr, need_copyout);
    if (ret) {
        return -1;
    }
    return 0;
}

int ifup(const char *ifname)
{
    struct _ifreq ifr = {};
    int sk, ret;

    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

//    sk = socket(PF_INET, SOCK_DGRAM, 0);
//    if (sk < 0)
//        return -1;
//
    ret = ioctl(&init_net, SIOCGIFFLAGS, &ifr, NULL);
    if (ret) {
//        close(sk);
        return -1;
    }
//
    ifr.ifr_flags |= IFF_UP;
    ret = ioctl(&init_net, SIOCSIFFLAGS, &ifr, NULL);
    if (ret) {
//        close(sk);
        return -1;
    }
//
//    close(sk);
    return 0;
}
