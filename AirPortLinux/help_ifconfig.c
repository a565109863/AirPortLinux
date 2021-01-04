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


int ioctl(int sk, unsigned int cmd, void *ifr)
{
    int ret = sock_ioctl(sk, cmd, (unsigned long)ifr);
    if (ret) {
        return -1;
    }
    return 0;
}

int ifup(const char *ifname)
{
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    struct _ifreq ifr = {};
    int sk, ret;

    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

//    sk = socket(PF_INET, SOCK_DGRAM, 0);
//    if (sk < 0)
//        return -1;
//
    ret = ioctl(sk, SIOCGIFFLAGS, &ifr);
    if (ret) {
//        close(sk);
        return -1;
    }
//
    ifr.ifr_flags |= IFF_UP;
    ret = ioctl(sk, SIOCSIFFLAGS, &ifr);
    if (ret) {
//        close(sk);
        return -1;
    }
//
//    close(sk);
    return 0;
}


int ifdown(const char *ifname)
{
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    struct _ifreq ifr = {};
    int sk, ret;
    
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
    
    //    sk = socket(PF_INET, SOCK_DGRAM, 0);
    //    if (sk < 0)
    //        return -1;
    //
    ret = ioctl(sk, SIOCGIFFLAGS, &ifr);
    if (ret) {
        //        close(sk);
        return -1;
    }
    //
    ifr.ifr_flags = ~IFF_UP;
    ret = ioctl(sk, SIOCSIFFLAGS, &ifr);
    if (ret) {
        //        close(sk);
        return -1;
    }
    //
    //    close(sk);
    return 0;
}
