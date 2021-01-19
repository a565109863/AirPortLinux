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


/*------------------------------------------------------------------*/
/*
 * Wrapper to push some Wireless Parameter in the driver
 */
static inline int
iw_set_ext(int            skfd,        /* Socket to the kernel */
       const char *        ifname,        /* Device name */
       int            request,    /* WE ID */
       struct iwreq *    pwrq)        /* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}

/*------------------------------------------------------------------*/
/*
 * Wrapper to extract some Wireless Parameter out of the driver
 */
static inline int
iw_get_ext(int            skfd,        /* Socket to the kernel */
       const char *        ifname,        /* Device name */
       int            request,    /* WE ID */
       struct iwreq *    pwrq)        /* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}



int ioctl(int sk, unsigned int cmd, void *ifr)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
    int ret = sock_ioctl(sk, cmd, (unsigned long)ifr);
    if (ret) {
        return -1;
    }
    return 0;
}

void scan();
int ifscan(const char *ifname);

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
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    ret = ioctl(sk, SIOCGIFFLAGS, &ifr);
    if (ret) {
//        close(sk);
        return -1;
    }
//
    ifr.ifr_flags |= IFF_UP;
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    ret = ioctl(sk, SIOCSIFFLAGS, &ifr);
    if (ret) {
//        close(sk);
        return -1;
    }
    
    DebugLog("--%s: line = %d up", __FUNCTION__, __LINE__);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    
//    scan();
    ifscan(ifname);
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
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

#include <net/genetlink.h>
#include <linux/nl80211.h>

void scan()
{
    struct sk_buff *msg;
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    msg = nlmsg_new(sizeof(struct nlmsghdr), 123);
    if (!msg)
        return;
    
    
    const struct genl_family *res = NULL;
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    
    res = genl_family_find_byname(NL80211_GENL_NAME);
    if (res == NULL) {
        DebugLog("--%s: line = %d, genl_family = NULL", __FUNCTION__, __LINE__);
        return;
    }
    DebugLog("--%s: line = %d, family->id = %d", __FUNCTION__, __LINE__, res->id);
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    
    genlmsg_put(msg, 0, 0, res,
                NLM_F_REQUEST, NL80211_CMD_TRIGGER_SCAN);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    
    genl_rcv(msg);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
}


int ifscan(const char *ifname)
{
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    struct _ifreq ifr = {};
    int skfd = 0, ret;

    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

//    sk = socket(PF_INET, SOCK_DGRAM, 0);
//    if (sk < 0)
//        return -1;
//
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    struct iwreq        wrq;
    wrq.u.data.pointer = NULL;
    wrq.u.data.flags = 0;
    wrq.u.data.length = 0;
    
    unsigned char *    buffer = NULL;        /* Results */
    int            buflen = IW_SCAN_MAX_DATA; /* Min for compat WE<17 */
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    if(iw_set_ext(skfd, ifname, SIOCSIWSCAN, &wrq) < 0)
    {
        return -1;
    }
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    unsigned char *    newbuf;

    /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
    newbuf = (unsigned char *)realloc(buffer, buflen);
    if(newbuf == NULL)
    {
        if(buffer)
            free(buffer, __GFP_ZERO, buflen);
//        fprintf(stderr, "%s: Allocation failed\n", __FUNCTION__);
        return(-1);
    }
    buffer = newbuf;
    
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    /* Try to read the results */
    wrq.u.data.pointer = buffer;
    wrq.u.data.flags = 0;
    wrq.u.data.length = buflen;
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
//    if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0)
//    {
//        return -1;
//    }
//
//    close(sk);
    return 0;
}
