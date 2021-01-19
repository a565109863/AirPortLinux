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
#include <linux/wireless.h>

extern struct net init_net;

int ioctl(int sk, unsigned int cmd, void *);
long sock_ioctl(int sk, unsigned cmd, unsigned long arg);

int ifup(const char *ifname);
int ifdown(const char *ifname);

#endif /* help_ifconfig_h */
