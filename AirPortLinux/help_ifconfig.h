//
//  help_ifconfig.h
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/29.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#ifndef help_ifconfig_h
#define help_ifconfig_h

#include <net/core/dev_ioctl.h>

extern struct net init_net;


int ioctl(struct net *net, unsigned int cmd, struct _ifreq *ifr, bool *need_copyout);

int ifup(const char *ifname);

#endif /* help_ifconfig_h */
