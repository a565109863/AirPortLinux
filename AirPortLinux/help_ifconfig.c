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
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
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
    
//    scan();
    ifscan(ifname);
    
    DebugLog("--%s: line = %d end", __FUNCTION__, __LINE__);
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
    
    
//    if (bss->wdev_id_set)
////        return nla_put_u64(msg, NL80211_ATTR_WDEV, bss->wdev_id);
//    return nla_put_u32(msg, NL80211_ATTR_IFINDEX, bss->ifindex);
//
//
//    if (dev &&
//        (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
//         nla_put_string(msg, NL80211_ATTR_IFNAME, dev->name)))
//        goto nla_put_failure;
    
    genl_rcv(msg);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
}


int ifscan(const char *ifname)
{
    kprintf("--%s: line = %d", __FUNCTION__, __LINE__);
    struct _ifreq ifr = {};
    int skfd = 0, ret;

    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

//    sk = socket(PF_INET, SOCK_DGRAM, 0);
//    if (sk < 0)
//        return -1;
//
    
    struct iwreq        wrq;
    wrq.u.data.pointer = NULL;
    wrq.u.data.flags = 0;
    wrq.u.data.length = 0;

    unsigned char *    buffer = NULL;        /* Results */
    int            buflen = IW_SCAN_MAX_DATA; /* Min for compat WE<17 */

    if(iw_set_ext(skfd, ifname, SIOCSIWSCAN, &wrq) < 0)
    {
        return -1;
    }
//
//    unsigned char *    newbuf;
//
//    /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
//    newbuf = (unsigned char *)realloc(buffer, buflen);
//    if(newbuf == NULL)
//    {
//        if(buffer)
//            free(buffer, __GFP_ZERO, buflen);
////        fprintf(stderr, "%s: Allocation failed\n", __FUNCTION__);
//        return(-1);
//    }
//    buffer = newbuf;
//
//    /* Try to read the results */
//    wrq.u.data.pointer = buffer;
//    wrq.u.data.flags = 0;
//    wrq.u.data.length = buflen;
//    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
//    if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0)
//    {
//        return -1;
//    }
//
//    close(sk);
    
    
    kprintf("--%s: line = %d end", __FUNCTION__, __LINE__);
    return 0;
    IOSleep(10000);
    
    
    int            we_version = 1;
    
    /* Try to read the results */
    wrq.u.data.pointer = buffer;
    wrq.u.data.flags = 0;
    wrq.u.data.length = buflen;
    if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0)
    {
//        /* Check if buffer was too small (WE-17 only) */
//        if((errno == E2BIG) && (we_version > 16))
//        {
//            /* Some driver may return very large scan results, either
//             * because there are many cells, or because they have many
//             * large elements in cells (like IWEVCUSTOM). Most will
//             * only need the regular sized buffer. We now use a dynamic
//             * allocation of the buffer to satisfy everybody. Of course,
//             * as we don't know in advance the size of the array, we try
//             * various increasing sizes. Jean II */
//
//            /* Check if the driver gave us any hints. */
//            if(wrq.u.data.length > buflen)
//                buflen = wrq.u.data.length;
//            else
//                buflen *= 2;
//
//            /* Try again */
//            goto realloc;
//        }
//
//        /* Check if results not available yet */
//        if(errno == EAGAIN)
//        {
//            free(buffer);
//            /* Wait for only 100ms from now on */
//            return(100);    /* Wait 100 ms */
//        }
//
//        free(buffer);
//        /* Bad error, please don't come back... */
//        return(-1);
    }
    
    /* We have the results, process them */
    if(wrq.u.data.length)
    {
        struct iw_event        iwe;
        struct stream_descr    stream;
        struct wireless_scan *    wscan = NULL;
        int            ret;
#ifdef DEBUG
        /* Debugging code. In theory useless, because it's debugged ;-) */
        int    i;
        printf("Scan result [%02X", buffer[0]);
        for(i = 1; i < wrq.u.data.length; i++)
            printf(":%02X", buffer[i]);
        printf("]\n");
#endif
        
        /* Init */
        iw_init_event_stream(&stream, (char *) buffer, wrq.u.data.length);
        /* This is dangerous, we may leak user data... */
//        context->result = NULL;
        
        /* Look every token */
        do
        {
            /* Extract an event and print it */
            ret = iw_extract_event_stream(&stream, &iwe, we_version);
            if(ret > 0)
            {
                /* Convert to wireless_scan struct */
//                wscan = iw_process_scanning_token(&iwe, wscan);
//                /* Check problems */
//                if(wscan == NULL)
//                {
//                    free(buffer);
//                    errno = ENOMEM;
//                    return(-1);
//                }
//                /* Save head of list */
//                if(context->result == NULL)
//                    context->result = wscan;
            }
        }
        while(ret > 0);
    }
    
    
    
    return 0;
}




/************************ CONSTANTS & MACROS ************************/

/*
 * Constants fof WE-9->15
 */
#define IW15_MAX_FREQUENCIES    16
#define IW15_MAX_BITRATES    8
#define IW15_MAX_TXPOWER    8
#define IW15_MAX_ENCODING_SIZES    8
#define IW15_MAX_SPY        8
#define IW15_MAX_AP        8

/****************************** TYPES ******************************/

/*
 *    Struct iw_range up to WE-15
 */
struct    iw15_range
{
    __u32        throughput;
    __u32        min_nwid;
    __u32        max_nwid;
    __u16        num_channels;
    __u8        num_frequency;
    struct iw_freq    freq[IW15_MAX_FREQUENCIES];
    __s32        sensitivity;
    struct iw_quality    max_qual;
    __u8        num_bitrates;
    __s32        bitrate[IW15_MAX_BITRATES];
    __s32        min_rts;
    __s32        max_rts;
    __s32        min_frag;
    __s32        max_frag;
    __s32        min_pmp;
    __s32        max_pmp;
    __s32        min_pmt;
    __s32        max_pmt;
    __u16        pmp_flags;
    __u16        pmt_flags;
    __u16        pm_capa;
    __u16        encoding_size[IW15_MAX_ENCODING_SIZES];
    __u8        num_encoding_sizes;
    __u8        max_encoding_tokens;
    __u16        txpower_capa;
    __u8        num_txpower;
    __s32        txpower[IW15_MAX_TXPOWER];
    __u8        we_version_compiled;
    __u8        we_version_source;
    __u16        retry_capa;
    __u16        retry_flags;
    __u16        r_time_flags;
    __s32        min_retry;
    __s32        max_retry;
    __s32        min_r_time;
    __s32        max_r_time;
    struct iw_quality    avg_qual;
};

/*
 * Union for all the versions of iwrange.
 * Fortunately, I mostly only add fields at the end, and big-bang
 * reorganisations are few.
 */
union    iw_range_raw
{
    struct iw15_range    range15;    /* WE 9->15 */
    struct iw_range        range;        /* WE 16->current */
};

/*
 * Offsets in iw_range struct
 */
#define iwr15_off(f)    ( ((char *) &(((struct iw15_range *) NULL)->f)) - \
(char *) NULL)
#define iwr_off(f)    ( ((char *) &(((struct iw_range *) NULL)->f)) - \
(char *) NULL)


/* Disable runtime version warning in iw_get_range_info() */
int    iw_ignore_version = 0;

#define fprintf(x, arg...) kprintf(arg);

/* Maximum forward compatibility built in this version of WT */
#define WE_MAX_VERSION    22

/*------------------------------------------------------------------*/
/*
 * Get the range information out of the driver
 */
int
iw_get_range_info(int        skfd,
                  const char *    ifname,
                  iwrange *    range)
{
    struct iwreq        wrq;
    char            buffer[sizeof(iwrange) * 2];    /* Large enough */
    union iw_range_raw *    range_raw;
    
    /* Cleanup */
    bzero(buffer, sizeof(buffer));
    
    wrq.u.data.pointer = (caddr_t) buffer;
    wrq.u.data.length = sizeof(buffer);
    wrq.u.data.flags = 0;
    if(iw_get_ext(skfd, ifname, SIOCGIWRANGE, &wrq) < 0)
        return(-1);
    
    /* Point to the buffer */
    range_raw = (union iw_range_raw *) buffer;
    
    /* For new versions, we can check the version directly, for old versions
     * we use magic. 300 bytes is a also magic number, don't touch... */
    if(wrq.u.data.length < 300)
    {
        /* That's v10 or earlier. Ouch ! Let's make a guess...*/
        range_raw->range.we_version_compiled = 9;
    }
    
    /* Check how it needs to be processed */
    if(range_raw->range.we_version_compiled > 15)
    {
        /* This is our native format, that's easy... */
        /* Copy stuff at the right place, ignore extra */
        memcpy((char *) range, buffer, sizeof(iwrange));
    }
    else
    {
        /* Zero unknown fields */
        bzero((char *) range, sizeof(struct iw_range));
        
        /* Initial part unmoved */
        memcpy((char *) range,
               buffer,
               iwr15_off(num_channels));
        /* Frequencies pushed futher down towards the end */
        memcpy((char *) range + iwr_off(num_channels),
               buffer + iwr15_off(num_channels),
               iwr15_off(sensitivity) - iwr15_off(num_channels));
        /* This one moved up */
        memcpy((char *) range + iwr_off(sensitivity),
               buffer + iwr15_off(sensitivity),
               iwr15_off(num_bitrates) - iwr15_off(sensitivity));
        /* This one goes after avg_qual */
        memcpy((char *) range + iwr_off(num_bitrates),
               buffer + iwr15_off(num_bitrates),
               iwr15_off(min_rts) - iwr15_off(num_bitrates));
        /* Number of bitrates has changed, put it after */
        memcpy((char *) range + iwr_off(min_rts),
               buffer + iwr15_off(min_rts),
               iwr15_off(txpower_capa) - iwr15_off(min_rts));
        /* Added encoding_login_index, put it after */
        memcpy((char *) range + iwr_off(txpower_capa),
               buffer + iwr15_off(txpower_capa),
               iwr15_off(txpower) - iwr15_off(txpower_capa));
        /* Hum... That's an unexpected glitch. Bummer. */
        memcpy((char *) range + iwr_off(txpower),
               buffer + iwr15_off(txpower),
               iwr15_off(avg_qual) - iwr15_off(txpower));
        /* Avg qual moved up next to max_qual */
        memcpy((char *) range + iwr_off(avg_qual),
               buffer + iwr15_off(avg_qual),
               sizeof(struct iw_quality));
    }
    
    /* We are now checking much less than we used to do, because we can
     * accomodate more WE version. But, there are still cases where things
     * will break... */
    if(!iw_ignore_version)
    {
        /* We don't like very old version (unfortunately kernel 2.2.X) */
        if(range->we_version_compiled <= 10)
        {
            fprintf(stderr, "Warning: Driver for device %s has been compiled with an ancient version\n", ifname);
            fprintf(stderr, "of Wireless Extension, while this program support version 11 and later.\n");
            fprintf(stderr, "Some things may be broken...\n\n");
        }
        
        /* We don't like future versions of WE, because we can't cope with
         * the unknown */
        if(range->we_version_compiled > WE_MAX_VERSION)
        {
            fprintf(stderr, "Warning: Driver for device %s has been compiled with version %d\n", ifname, range->we_version_compiled);
            fprintf(stderr, "of Wireless Extension, while this program supports up to version %d.\n", WE_MAX_VERSION);
            fprintf(stderr, "Some things may be broken...\n\n");
        }
        
        /* Driver version verification */
        if((range->we_version_compiled > 10) &&
           (range->we_version_compiled < range->we_version_source))
        {
            fprintf(stderr, "Warning: Driver for device %s recommend version %d of Wireless Extension,\n", ifname, range->we_version_source);
            fprintf(stderr, "but has been compiled with version %d, therefore some driver features\n", range->we_version_compiled);
            fprintf(stderr, "may not be available...\n\n");
        }
        /* Note : we are only trying to catch compile difference, not source.
         * If the driver source has not been updated to the latest, it doesn't
         * matter because the new fields are set to zero */
    }
    
    /* Don't complain twice.
     * In theory, the test apply to each individual driver, but usually
     * all drivers are compiled from the same kernel. */
    iw_ignore_version = 1;
    
    return(0);
}


/*------------------------------------------------------------------*/
/*
 * Initialise the struct stream_descr so that we can extract
 * individual events from the event stream.
 */
void
iw_init_event_stream(struct stream_descr *    stream,    /* Stream of events */
                     char *            data,
                     int            len)
{
    /* Cleanup */
    memset((char *) stream, '\0', sizeof(struct stream_descr));
    
    /* Set things up */
    stream->current = data;
    stream->end = data + len;
}


/* -------------------------- CONSTANTS -------------------------- */

/* Type of headers we know about (basically union iwreq_data) */
#define IW_HEADER_TYPE_NULL    0    /* Not available */
#define IW_HEADER_TYPE_CHAR    2    /* char [IFNAMSIZ] */
#define IW_HEADER_TYPE_UINT    4    /* __u32 */
#define IW_HEADER_TYPE_FREQ    5    /* struct iw_freq */
#define IW_HEADER_TYPE_ADDR    6    /* struct sockaddr */
#define IW_HEADER_TYPE_POINT    8    /* struct iw_point */
#define IW_HEADER_TYPE_PARAM    9    /* struct iw_param */
#define IW_HEADER_TYPE_QUAL    10    /* struct iw_quality */

/* Handling flags */
/* Most are not implemented. I just use them as a reminder of some
 * cool features we might need one day ;-) */
#define IW_DESCR_FLAG_NONE    0x0000    /* Obvious */
/* Wrapper level flags */
#define IW_DESCR_FLAG_DUMP    0x0001    /* Not part of the dump command */
#define IW_DESCR_FLAG_EVENT    0x0002    /* Generate an event on SET */
#define IW_DESCR_FLAG_RESTRICT    0x0004    /* GET : request is ROOT only */
/* SET : Omit payload from generated iwevent */
#define IW_DESCR_FLAG_NOMAX    0x0008    /* GET : no limit on request size */
/* Driver level flags */
#define IW_DESCR_FLAG_WAIT    0x0100    /* Wait for driver event */

/* ---------------------------- TYPES ---------------------------- */

/*
 * Describe how a standard IOCTL looks like.
 */
struct iw_ioctl_description
{
    __u8    header_type;        /* NULL, iw_point or other */
    __u8    token_type;        /* Future */
    __u16    token_size;        /* Granularity of payload */
    __u16    min_tokens;        /* Min acceptable token number */
    __u16    max_tokens;        /* Max acceptable token number */
    __u32    flags;            /* Special handling of the request */
};

/* -------------------------- VARIABLES -------------------------- */

/*standard_ioctl_num
 * Meta-data about all the standard Wireless Extension request we
 * know about.
 */
static const struct iw_ioctl_description standard_ioctl_descr[] = {
    [SIOCSIWCOMMIT    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },
    [SIOCGIWNAME    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_CHAR,
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWNWID    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
        .flags        = IW_DESCR_FLAG_EVENT,
    },
    [SIOCGIWNWID    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWFREQ    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_FREQ,
        .flags        = IW_DESCR_FLAG_EVENT,
    },
    [SIOCGIWFREQ    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_FREQ,
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWMODE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_UINT,
        .flags        = IW_DESCR_FLAG_EVENT,
    },
    [SIOCGIWMODE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_UINT,
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWSENS    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWSENS    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCSIWRANGE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },
    [SIOCGIWRANGE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = sizeof(struct iw_range),
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWPRIV    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },
    [SIOCGIWPRIV    - SIOCIWFIRST] = { /* (handled directly by us) */
        .header_type    = IW_HEADER_TYPE_NULL,
    },
    [SIOCSIWSTATS    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },
    [SIOCGIWSTATS    - SIOCIWFIRST] = { /* (handled directly by us) */
        .header_type    = IW_HEADER_TYPE_NULL,
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWSPY    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = sizeof(struct sockaddr),
        .max_tokens    = IW_MAX_SPY,
    },
    [SIOCGIWSPY    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = sizeof(struct sockaddr) +
        sizeof(struct iw_quality),
        .max_tokens    = IW_MAX_SPY,
    },
    [SIOCSIWTHRSPY    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = sizeof(struct iw_thrspy),
        .min_tokens    = 1,
        .max_tokens    = 1,
    },
    [SIOCGIWTHRSPY    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = sizeof(struct iw_thrspy),
        .min_tokens    = 1,
        .max_tokens    = 1,
    },
    [SIOCSIWAP    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },
    [SIOCGIWAP    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWMLME    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .min_tokens    = sizeof(struct iw_mlme),
        .max_tokens    = sizeof(struct iw_mlme),
    },
    [SIOCGIWAPLIST    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = sizeof(struct sockaddr) +
        sizeof(struct iw_quality),
        .max_tokens    = IW_MAX_AP,
        .flags        = IW_DESCR_FLAG_NOMAX,
    },
    [SIOCSIWSCAN    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .min_tokens    = 0,
        .max_tokens    = sizeof(struct iw_scan_req),
    },
    [SIOCGIWSCAN    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_SCAN_MAX_DATA,
        .flags        = IW_DESCR_FLAG_NOMAX,
    },
    [SIOCSIWESSID    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_ESSID_MAX_SIZE + 1,
        .flags        = IW_DESCR_FLAG_EVENT,
    },
    [SIOCGIWESSID    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_ESSID_MAX_SIZE + 1,
        .flags        = IW_DESCR_FLAG_DUMP,
    },
    [SIOCSIWNICKN    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_ESSID_MAX_SIZE + 1,
    },
    [SIOCGIWNICKN    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_ESSID_MAX_SIZE + 1,
    },
    [SIOCSIWRATE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWRATE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCSIWRTS    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWRTS    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCSIWFRAG    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWFRAG    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCSIWTXPOW    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWTXPOW    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCSIWRETRY    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWRETRY    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCSIWENCODE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_ENCODING_TOKEN_MAX,
        .flags        = IW_DESCR_FLAG_EVENT | IW_DESCR_FLAG_RESTRICT,
    },
    [SIOCGIWENCODE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_ENCODING_TOKEN_MAX,
        .flags        = IW_DESCR_FLAG_DUMP | IW_DESCR_FLAG_RESTRICT,
    },
    [SIOCSIWPOWER    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWPOWER    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    //    [SIOCSIWMODUL    - SIOCIWFIRST] = {
    //        .header_type    = IW_HEADER_TYPE_PARAM,
    //    },
    //    [SIOCGIWMODUL    - SIOCIWFIRST] = {
    //        .header_type    = IW_HEADER_TYPE_PARAM,
    //    },
    [SIOCSIWGENIE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_GENERIC_IE_MAX,
    },
    [SIOCGIWGENIE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_GENERIC_IE_MAX,
    },
    [SIOCSIWAUTH    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCGIWAUTH    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },
    [SIOCSIWENCODEEXT - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .min_tokens    = sizeof(struct iw_encode_ext),
        .max_tokens    = sizeof(struct iw_encode_ext) +
        IW_ENCODING_TOKEN_MAX,
    },
    [SIOCGIWENCODEEXT - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .min_tokens    = sizeof(struct iw_encode_ext),
        .max_tokens    = sizeof(struct iw_encode_ext) +
        IW_ENCODING_TOKEN_MAX,
    },
    [SIOCSIWPMKSA - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .min_tokens    = sizeof(struct iw_pmksa),
        .max_tokens    = sizeof(struct iw_pmksa),
    },
};
static const unsigned int standard_ioctl_num = (sizeof(standard_ioctl_descr) /
                                                sizeof(struct iw_ioctl_description));

/*
 * Meta-data about all the additional standard Wireless Extension events
 * we know about.
 */
static const struct iw_ioctl_description standard_event_descr[] = {
    [IWEVTXDROP    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },
    [IWEVQUAL    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_QUAL,
    },
    [IWEVCUSTOM    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_CUSTOM_MAX,
    },
    [IWEVREGISTERED    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },
    [IWEVEXPIRED    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },
    [IWEVGENIE    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_GENERIC_IE_MAX,
    },
    [IWEVMICHAELMICFAILURE    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = sizeof(struct iw_michaelmicfailure),
    },
    [IWEVASSOCREQIE    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_GENERIC_IE_MAX,
    },
    [IWEVASSOCRESPIE    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = IW_GENERIC_IE_MAX,
    },
    [IWEVPMKIDCAND    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size    = 1,
        .max_tokens    = sizeof(struct iw_pmkid_cand),
    },
};
static const unsigned int standard_event_num = (sizeof(standard_event_descr) /
                                                sizeof(struct iw_ioctl_description));

/* Size (in bytes) of various events */
static const int event_type_size[] = {
    IW_EV_LCP_PK_LEN,    /* IW_HEADER_TYPE_NULL */
    0,
    IW_EV_CHAR_PK_LEN,    /* IW_HEADER_TYPE_CHAR */
    0,
    IW_EV_UINT_PK_LEN,    /* IW_HEADER_TYPE_UINT */
    IW_EV_FREQ_PK_LEN,    /* IW_HEADER_TYPE_FREQ */
    IW_EV_ADDR_PK_LEN,    /* IW_HEADER_TYPE_ADDR */
    0,
    IW_EV_POINT_PK_LEN,    /* Without variable payload */
    IW_EV_PARAM_PK_LEN,    /* IW_HEADER_TYPE_PARAM */
    IW_EV_QUAL_PK_LEN,    /* IW_HEADER_TYPE_QUAL */
};


/*------------------------------------------------------------------*/
/*
 * Extract the next event from the event stream.
 */
int
iw_extract_event_stream(struct stream_descr *    stream,    /* Stream of events */
                        struct iw_event *    iwe,    /* Extracted event */
                        int            we_version)
{
    const struct iw_ioctl_description *    descr = NULL;
    int        event_type = 0;
    unsigned int    event_len = 1;        /* Invalid */
    char *    pointer;
    /* Don't "optimise" the following variable, it will crash */
    unsigned    cmd_index;        /* *MUST* be unsigned */
    
    /* Check for end of stream */
    if((stream->current + IW_EV_LCP_PK_LEN) > stream->end)
        return(0);
    
#ifdef DEBUG
    printf("DBG - stream->current = %p, stream->value = %p, stream->end = %p\n",
           stream->current, stream->value, stream->end);
#endif
    
    /* Extract the event header (to get the event id).
     * Note : the event may be unaligned, therefore copy... */
    memcpy((char *) iwe, stream->current, IW_EV_LCP_PK_LEN);
    
#ifdef DEBUG
    printf("DBG - iwe->cmd = 0x%X, iwe->len = %d\n",
           iwe->cmd, iwe->len);
#endif
    
    /* Check invalid events */
    if(iwe->len <= IW_EV_LCP_PK_LEN)
        return(-1);
    
    /* Get the type and length of that event */
    if(iwe->cmd <= SIOCIWLAST)
    {
        cmd_index = iwe->cmd - SIOCIWFIRST;
        if(cmd_index < standard_ioctl_num)
            descr = &(standard_ioctl_descr[cmd_index]);
    }
    else
    {
        cmd_index = iwe->cmd - IWEVFIRST;
        if(cmd_index < standard_event_num)
            descr = &(standard_event_descr[cmd_index]);
    }
    if(descr != NULL)
        event_type = descr->header_type;
    /* Unknown events -> event_type=0 => IW_EV_LCP_PK_LEN */
    event_len = event_type_size[event_type];
    /* Fixup for earlier version of WE */
    if((we_version <= 18) && (event_type == IW_HEADER_TYPE_POINT))
        event_len += IW_EV_POINT_OFF;
    
    /* Check if we know about this event */
    if(event_len <= IW_EV_LCP_PK_LEN)
    {
        /* Skip to next event */
        stream->current += iwe->len;
        return(2);
    }
    event_len -= IW_EV_LCP_PK_LEN;
    
    /* Set pointer on data */
    if(stream->value != NULL)
        pointer = stream->value;            /* Next value in event */
    else
        pointer = stream->current + IW_EV_LCP_PK_LEN;    /* First value in event */
    
#ifdef DEBUG
    printf("DBG - event_type = %d, event_len = %d, pointer = %p\n",
           event_type, event_len, pointer);
#endif
    
    /* Copy the rest of the event (at least, fixed part) */
    if((pointer + event_len) > stream->end)
    {
        /* Go to next event */
        stream->current += iwe->len;
        return(-2);
    }
    /* Fixup for WE-19 and later : pointer no longer in the stream */
    /* Beware of alignement. Dest has local alignement, not packed */
    if((we_version > 18) && (event_type == IW_HEADER_TYPE_POINT))
        memcpy((char *) iwe + IW_EV_LCP_LEN + IW_EV_POINT_OFF,
               pointer, event_len);
    else
        memcpy((char *) iwe + IW_EV_LCP_LEN, pointer, event_len);
    
    /* Skip event in the stream */
    pointer += event_len;
    
    /* Special processing for iw_point events */
    if(event_type == IW_HEADER_TYPE_POINT)
    {
        /* Check the length of the payload */
        unsigned int    extra_len = iwe->len - (event_len + IW_EV_LCP_PK_LEN);
        if(extra_len > 0)
        {
            /* Set pointer on variable part (warning : non aligned) */
            iwe->u.data.pointer = pointer;
            
            /* Check that we have a descriptor for the command */
            if(descr == NULL)
            /* Can't check payload -> unsafe... */
                iwe->u.data.pointer = NULL;    /* Discard paylod */
            else
            {
                /* Those checks are actually pretty hard to trigger,
                 * because of the checks done in the kernel... */
                
                unsigned int    token_len = iwe->u.data.length * descr->token_size;
                
                /* Ugly fixup for alignement issues.
                 * If the kernel is 64 bits and userspace 32 bits,
                 * we have an extra 4+4 bytes.
                 * Fixing that in the kernel would break 64 bits userspace. */
                if((token_len != extra_len) && (extra_len >= 4))
                {
                    __u16        alt_dlen = *((__u16 *) pointer);
                    unsigned int    alt_token_len = alt_dlen * descr->token_size;
                    if((alt_token_len + 8) == extra_len)
                    {
#ifdef DEBUG
                        printf("DBG - alt_token_len = %d\n", alt_token_len);
#endif
                        /* Ok, let's redo everything */
                        pointer -= event_len;
                        pointer += 4;
                        /* Dest has local alignement, not packed */
                        memcpy((char *) iwe + IW_EV_LCP_LEN + IW_EV_POINT_OFF,
                               pointer, event_len);
                        pointer += event_len + 4;
                        iwe->u.data.pointer = pointer;
                        token_len = alt_token_len;
                    }
                }
                
                /* Discard bogus events which advertise more tokens than
                 * what they carry... */
                if(token_len > extra_len)
                    iwe->u.data.pointer = NULL;    /* Discard paylod */
                /* Check that the advertised token size is not going to
                 * produce buffer overflow to our caller... */
                if((iwe->u.data.length > descr->max_tokens)
                   && !(descr->flags & IW_DESCR_FLAG_NOMAX))
                    iwe->u.data.pointer = NULL;    /* Discard paylod */
                /* Same for underflows... */
                if(iwe->u.data.length < descr->min_tokens)
                    iwe->u.data.pointer = NULL;    /* Discard paylod */
#ifdef DEBUG
                printf("DBG - extra_len = %d, token_len = %d, token = %d, max = %d, min = %d\n",
                       extra_len, token_len, iwe->u.data.length, descr->max_tokens, descr->min_tokens);
#endif
            }
        }
        else
        /* No data */
            iwe->u.data.pointer = NULL;
        
        /* Go to next event */
        stream->current += iwe->len;
    }
    else
    {
        /* Ugly fixup for alignement issues.
         * If the kernel is 64 bits and userspace 32 bits,
         * we have an extra 4 bytes.
         * Fixing that in the kernel would break 64 bits userspace. */
        if((stream->value == NULL)
           && ((((iwe->len - IW_EV_LCP_PK_LEN) % event_len) == 4)
               || ((iwe->len == 12) && ((event_type == IW_HEADER_TYPE_UINT) ||
                                        (event_type == IW_HEADER_TYPE_QUAL))) ))
        {
#ifdef DEBUG
            printf("DBG - alt iwe->len = %d\n", iwe->len - 4);
#endif
            pointer -= event_len;
            pointer += 4;
            /* Beware of alignement. Dest has local alignement, not packed */
            memcpy((char *) iwe + IW_EV_LCP_LEN, pointer, event_len);
            pointer += event_len;
        }
        
        /* Is there more value in the event ? */
        if((pointer + event_len) <= (stream->current + iwe->len))
        /* Go to next value */
            stream->value = pointer;
        else
        {
            /* Go to next event */
            stream->value = NULL;
            stream->current += iwe->len;
        }
    }
    return(1);
}



/*------------------------------------------------------------------*/
/*
 * Display an Ethernet address in readable format.
 */
void
iw_ether_ntop(const struct ether_addr *    eth,
              char *            buf)
{
    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
            eth->ether_addr_octet[0], eth->ether_addr_octet[1],
            eth->ether_addr_octet[2], eth->ether_addr_octet[3],
            eth->ether_addr_octet[4], eth->ether_addr_octet[5]);
}

/*------------------------------------------------------------------*/
/*
 * Display an Ethernet Socket Address in readable format.
 */
static inline char *
iw_saether_ntop(const struct sockaddr *sap, char* bufp)
{
    iw_ether_ntop((const struct ether_addr *) sap->sa_data, bufp);
    return bufp;
}


/*------------------------------------------------------------------*/
/*
 * Convert our internal representation of frequencies to a floating point.
 */
double
iw_freq2float(const iwfreq *    in)
{
#ifdef WE_NOLIBM
    /* Version without libm : slower */
    int        i;
    double    res = (double) in->m;
    for(i = 0; i < in->e; i++)
        res *= 10;
    return(res);
#else    /* WE_NOLIBM */
    /* Version with libm : faster */
    return ((double) in->m) * pow(10,in->e);
#endif    /* WE_NOLIBM */
}



/* Some usefull constants */
#define KILO    1e3
#define MEGA    1e6
#define GIGA    1e9

/*------------------------------------------------------------------*/
/*
 * Convert a frequency to a channel (negative -> error)
 */
int
iw_freq_to_channel(double            freq,
                   const struct iw_range *    range)
{
    double    ref_freq;
    int        k;
    
    /* Check if it's a frequency or not already a channel */
    if(freq < KILO)
        return(-1);
    
    /* We compare the frequencies as double to ignore differences
     * in encoding. Slower, but safer... */
    for(k = 0; k < range->num_frequency; k++)
    {
        ref_freq = iw_freq2float(&(range->freq[k]));
        if(freq == ref_freq)
            return(range->freq[k].i);
    }
    /* Not found */
    return(-2);
}


/*------------------------------------------------------------------*/
/*
 * Output a frequency with proper scaling
 */
void
iw_print_freq_value(char *    buffer,
                    int        buflen,
                    double    freq)
{
    if(freq < KILO)
        snprintf(buffer, buflen, "%g", freq);
    else
    {
        char    scale;
        int    divisor;
        
        if(freq >= GIGA)
        {
            scale = 'G';
            divisor = GIGA;
        }
        else
        {
            if(freq >= MEGA)
            {
                scale = 'M';
                divisor = MEGA;
            }
            else
            {
                scale = 'k';
                divisor = KILO;
            }
        }
        snprintf(buffer, buflen, "%g %cHz", freq / divisor, scale);
    }
}

/*------------------------------------------------------------------*/
/*
 * Output a frequency with proper scaling
 */
void
iw_print_freq(char *    buffer,
              int    buflen,
              double    freq,
              int    channel,
              int    freq_flags)
{
    char    sep = ((freq_flags & IW_FREQ_FIXED) ? '=' : ':');
    char    vbuf[16];
    
    /* Print the frequency/channel value */
    iw_print_freq_value(vbuf, sizeof(vbuf), freq);
    
    /* Check if channel only */
    if(freq < KILO)
        snprintf(buffer, buflen, "Channel%c%s", sep, vbuf);
    else
    {
        /* Frequency. Check if we have a channel as well */
        if(channel >= 0)
            snprintf(buffer, buflen, "Frequency%c%s (Channel %d)",
                     sep, vbuf, channel);
        else
            snprintf(buffer, buflen, "Frequency%c%s", sep, vbuf);
    }
}

/* Modes as human readable strings */
const char * const iw_operation_mode[] = { "Auto",
    "Ad-Hoc",
    "Managed",
    "Master",
    "Repeater",
    "Secondary",
    "Monitor",
    "Unknown/bug" };

/*********************** ENCODING SUBROUTINES ***********************/

/*------------------------------------------------------------------*/
/*
 * Output the encoding key, with a nice formating
 */
void
iw_print_key(char *            buffer,
             int            buflen,
             const unsigned char *    key,        /* Must be unsigned */
             int            key_size,
             int            key_flags)
{
    int    i;
    
    /* Check buffer size -> 1 bytes => 2 digits + 1/2 separator */
    if((key_size * 3) > buflen)
    {
        snprintf(buffer, buflen, "<too big>");
        return;
    }
    
    /* Is the key present ??? */
    if(key_flags & IW_ENCODE_NOKEY)
    {
        /* Nope : print on or dummy */
        if(key_size <= 0)
            strncpy(buffer, "on", sizeof("on"));            /* Size checked */
        else
        {
            strncpy(buffer, "**", sizeof("**"));            /* Size checked */
            buffer +=2;
            for(i = 1; i < key_size; i++)
            {
                if((i & 0x1) == 0)
                    strncpy(buffer++, "-", sizeof("-"));        /* Size checked */
                strncpy(buffer, "**", sizeof("**"));        /* Size checked */
                buffer +=2;
            }
        }
    }
    else
    {
        /* Yes : print the key */
        sprintf(buffer, "%.2X", key[0]);        /* Size checked */
        buffer +=2;
        for(i = 1; i < key_size; i++)
        {
            if((i & 0x1) == 0)
                strncpy(buffer++, "-", sizeof("-"));        /* Size checked */
            sprintf(buffer, "%.2X", key[i]);    /* Size checked */
            buffer +=2;
        }
    }
}


/*------------------------------------------------------------------*/
/*
 * Output the link statistics, taking care of formating
 */
void
iw_print_stats(char *        buffer,
               int        buflen,
               const iwqual *    qual,
               const iwrange *    range,
               int        has_range)
{
    int        len;
    
    /* People are very often confused by the 8 bit arithmetic happening
     * here.
     * All the values here are encoded in a 8 bit integer. 8 bit integers
     * are either unsigned [0 ; 255], signed [-128 ; +127] or
     * negative [-255 ; 0].
     * Further, on 8 bits, 0x100 == 256 == 0.
     *
     * Relative/percent values are always encoded unsigned, between 0 and 255.
     * Absolute/dBm values are always encoded between -192 and 63.
     * (Note that up to version 28 of Wireless Tools, dBm used to be
     *  encoded always negative, between -256 and -1).
     *
     * How do we separate relative from absolute values ?
     * The old way is to use the range to do that. As of WE-19, we have
     * an explicit IW_QUAL_DBM flag in updated...
     * The range allow to specify the real min/max of the value. As the
     * range struct only specify one bound of the value, we assume that
     * the other bound is 0 (zero).
     * For relative values, range is [0 ; range->max].
     * For absolute values, range is [range->max ; 63].
     *
     * Let's take two example :
     * 1) value is 75%. qual->value = 75 ; range->max_qual.value = 100
     * 2) value is -54dBm. noise floor of the radio is -104dBm.
     *    qual->value = -54 = 202 ; range->max_qual.value = -104 = 152
     *
     * Jean II
     */
    
    /* Just do it...
     * The old way to detect dBm require both the range and a non-null
     * level (which confuse the test). The new way can deal with level of 0
     * because it does an explicit test on the flag. */
    if(has_range && ((qual->level != 0)
                     || (qual->updated & (IW_QUAL_DBM | IW_QUAL_RCPI))))
    {
        /* Deal with quality : always a relative value */
        if(!(qual->updated & IW_QUAL_QUAL_INVALID))
        {
            len = snprintf(buffer, buflen, "Quality%c%d/%d  ",
                           qual->updated & IW_QUAL_QUAL_UPDATED ? '=' : ':',
                           qual->qual, range->max_qual.qual);
            buffer += len;
            buflen -= len;
        }
        
        /* Check if the statistics are in RCPI (IEEE 802.11k) */
        if(qual->updated & IW_QUAL_RCPI)
        {
            /* Deal with signal level in RCPI */
            /* RCPI = int{(Power in dBm +110)*2} for 0dbm > Power > -110dBm */
            if(!(qual->updated & IW_QUAL_LEVEL_INVALID))
            {
                double    rcpilevel = (qual->level / 2.0) - 110.0;
                len = snprintf(buffer, buflen, "Signal level%c%g dBm  ",
                               qual->updated & IW_QUAL_LEVEL_UPDATED ? '=' : ':',
                               rcpilevel);
                buffer += len;
                buflen -= len;
            }
            
            /* Deal with noise level in dBm (absolute power measurement) */
            if(!(qual->updated & IW_QUAL_NOISE_INVALID))
            {
                double    rcpinoise = (qual->noise / 2.0) - 110.0;
                len = snprintf(buffer, buflen, "Noise level%c%g dBm",
                               qual->updated & IW_QUAL_NOISE_UPDATED ? '=' : ':',
                               rcpinoise);
            }
        }
        else
        {
            /* Check if the statistics are in dBm */
            if((qual->updated & IW_QUAL_DBM)
               || (qual->level > range->max_qual.level))
            {
                /* Deal with signal level in dBm  (absolute power measurement) */
                if(!(qual->updated & IW_QUAL_LEVEL_INVALID))
                {
                    int    dblevel = qual->level;
                    /* Implement a range for dBm [-192; 63] */
                    if(qual->level >= 64)
                        dblevel -= 0x100;
                    len = snprintf(buffer, buflen, "Signal level%c%d dBm  ",
                                   qual->updated & IW_QUAL_LEVEL_UPDATED ? '=' : ':',
                                   dblevel);
                    buffer += len;
                    buflen -= len;
                }
                
                /* Deal with noise level in dBm (absolute power measurement) */
                if(!(qual->updated & IW_QUAL_NOISE_INVALID))
                {
                    int    dbnoise = qual->noise;
                    /* Implement a range for dBm [-192; 63] */
                    if(qual->noise >= 64)
                        dbnoise -= 0x100;
                    len = snprintf(buffer, buflen, "Noise level%c%d dBm",
                                   qual->updated & IW_QUAL_NOISE_UPDATED ? '=' : ':',
                                   dbnoise);
                }
            }
            else
            {
                /* Deal with signal level as relative value (0 -> max) */
                if(!(qual->updated & IW_QUAL_LEVEL_INVALID))
                {
                    len = snprintf(buffer, buflen, "Signal level%c%d/%d  ",
                                   qual->updated & IW_QUAL_LEVEL_UPDATED ? '=' : ':',
                                   qual->level, range->max_qual.level);
                    buffer += len;
                    buflen -= len;
                }
                
                /* Deal with noise level as relative value (0 -> max) */
                if(!(qual->updated & IW_QUAL_NOISE_INVALID))
                {
                    len = snprintf(buffer, buflen, "Noise level%c%d/%d",
                                   qual->updated & IW_QUAL_NOISE_UPDATED ? '=' : ':',
                                   qual->noise, range->max_qual.noise);
                }
            }
        }
    }
    else
    {
        /* We can't read the range, so we don't know... */
        snprintf(buffer, buflen,
                 "Quality:%d  Signal level:%d  Noise level:%d",
                 qual->qual, qual->level, qual->noise);
    }
}

/*------------------------------------------------------------------*/
/*
 * Output a bitrate with proper scaling
 */
void
iw_print_bitrate(char *    buffer,
                 int    buflen,
                 int    bitrate)
{
    double    rate = bitrate;
    char        scale;
    int        divisor;
    
    if(rate >= GIGA)
    {
        scale = 'G';
        divisor = GIGA;
    }
    else
    {
        if(rate >= MEGA)
        {
            scale = 'M';
            divisor = MEGA;
        }
        else
        {
            scale = 'k';
            divisor = KILO;
        }
    }
    snprintf(buffer, buflen, "%g %cb/s", rate / divisor, scale);
}

/*------------------------------------------------------------------*/
/*
 * Print the name corresponding to a value, with overflow check.
 */
static void
iw_print_value_name(unsigned int        value,
                    const char *        names[],
                    const unsigned int        num_names)
{
    if(value >= num_names)
        printf(" unknown (%d)", value);
    else
        printf(" %s", names[value]);
}

/*------------------------------------------------------------------*/
/*
 * Parse, and display the results of an unknown IE.
 *
 */
static void
iw_print_ie_unknown(unsigned char *    iebuf,
                    int            buflen)
{
    int    ielen = iebuf[1] + 2;
    int    i;
    
    if(ielen > buflen)
        ielen = buflen;
    
    printf("Unknown: ");
    for(i = 0; i < ielen; i++)
        printf("%02X", iebuf[i]);
    printf("\n");
}

/* ------------------------ WPA CAPA NAMES ------------------------ */
/*
 * This is the user readable name of a bunch of WPA constants in wireless.h
 * Maybe this should go in iwlib.c ?
 */

#ifndef WE_ESSENTIAL
#define IW_ARRAY_LEN(x) (sizeof(x)/sizeof((x)[0]))

//static const struct iwmask_name iw_enc_mode_name[] = {
//  { IW_ENCODE_RESTRICTED,    "restricted" },
//  { IW_ENCODE_OPEN,        "open" },
//};
//#define    IW_ENC_MODE_NUM        IW_ARRAY_LEN(iw_enc_mode_name)

static const struct iwmask_name iw_auth_capa_name[] = {
    { IW_ENC_CAPA_WPA,        "WPA" },
    { IW_ENC_CAPA_WPA2,        "WPA2" },
    { IW_ENC_CAPA_CIPHER_TKIP,    "CIPHER-TKIP" },
    { IW_ENC_CAPA_CIPHER_CCMP,    "CIPHER-CCMP" },
};
#define    IW_AUTH_CAPA_NUM    IW_ARRAY_LEN(iw_auth_capa_name)

static const struct iwmask_name iw_auth_cypher_name[] = {
    { IW_AUTH_CIPHER_NONE,    "none" },
    { IW_AUTH_CIPHER_WEP40,    "WEP-40" },
    { IW_AUTH_CIPHER_TKIP,    "TKIP" },
    { IW_AUTH_CIPHER_CCMP,    "CCMP" },
    { IW_AUTH_CIPHER_WEP104,    "WEP-104" },
};
#define    IW_AUTH_CYPHER_NUM    IW_ARRAY_LEN(iw_auth_cypher_name)

static const struct iwmask_name iw_wpa_ver_name[] = {
    { IW_AUTH_WPA_VERSION_DISABLED,    "disabled" },
    { IW_AUTH_WPA_VERSION_WPA,        "WPA" },
    { IW_AUTH_WPA_VERSION_WPA2,        "WPA2" },
};
#define    IW_WPA_VER_NUM        IW_ARRAY_LEN(iw_wpa_ver_name)

static const struct iwmask_name iw_auth_key_mgmt_name[] = {
    { IW_AUTH_KEY_MGMT_802_1X,    "802.1x" },
    { IW_AUTH_KEY_MGMT_PSK,    "PSK" },
};
#define    IW_AUTH_KEY_MGMT_NUM    IW_ARRAY_LEN(iw_auth_key_mgmt_name)

static const struct iwmask_name iw_auth_alg_name[] = {
    { IW_AUTH_ALG_OPEN_SYSTEM,    "open" },
    { IW_AUTH_ALG_SHARED_KEY,    "shared-key" },
    { IW_AUTH_ALG_LEAP,        "LEAP" },
};
#define    IW_AUTH_ALG_NUM        IW_ARRAY_LEN(iw_auth_alg_name)

static const struct iw_auth_descr    iw_auth_settings[] = {
    { IW_AUTH_WPA_VERSION, "WPA version", iw_wpa_ver_name, IW_WPA_VER_NUM },
    { IW_AUTH_KEY_MGMT, "Key management", iw_auth_key_mgmt_name, IW_AUTH_KEY_MGMT_NUM },
    { IW_AUTH_CIPHER_PAIRWISE, "Pairwise cipher", iw_auth_cypher_name, IW_AUTH_CYPHER_NUM },
    { IW_AUTH_CIPHER_GROUP, "Pairwise cipher", iw_auth_cypher_name, IW_AUTH_CYPHER_NUM },
    { IW_AUTH_TKIP_COUNTERMEASURES, "TKIP countermeasures", NULL, 0 },
    { IW_AUTH_DROP_UNENCRYPTED, "Drop unencrypted", NULL, 0 },
    { IW_AUTH_80211_AUTH_ALG, "Authentication algorithm", iw_auth_alg_name, IW_AUTH_ALG_NUM },
    { IW_AUTH_RX_UNENCRYPTED_EAPOL, "Receive unencrypted EAPOL", NULL, 0 },
    { IW_AUTH_ROAMING_CONTROL, "Roaming control", NULL, 0 },
    { IW_AUTH_PRIVACY_INVOKED, "Privacy invoked", NULL, 0 },
};
#define    IW_AUTH_SETTINGS_NUM        IW_ARRAY_LEN(iw_auth_settings)

/* Values for the IW_ENCODE_ALG_* returned by SIOCSIWENCODEEXT */
static const char *    iw_encode_alg_name[] = {
    "none",
    "WEP",
    "TKIP",
    "CCMP",
    "unknown"
};
#define    IW_ENCODE_ALG_NUM        IW_ARRAY_LEN(iw_encode_alg_name)

#ifndef IW_IE_CIPHER_NONE
/* Cypher values in GENIE (pairwise and group) */
#define IW_IE_CIPHER_NONE    0
#define IW_IE_CIPHER_WEP40    1
#define IW_IE_CIPHER_TKIP    2
#define IW_IE_CIPHER_WRAP    3
#define IW_IE_CIPHER_CCMP    4
#define IW_IE_CIPHER_WEP104    5
/* Key management in GENIE */
#define IW_IE_KEY_MGMT_NONE    0
#define IW_IE_KEY_MGMT_802_1X    1
#define IW_IE_KEY_MGMT_PSK    2
#endif    /* IW_IE_CIPHER_NONE */

/* Values for the IW_IE_CIPHER_* in GENIE */
static const char *    iw_ie_cypher_name[] = {
    "none",
    "WEP-40",
    "TKIP",
    "WRAP",
    "CCMP",
    "WEP-104",
};
#define    IW_IE_CYPHER_NUM    IW_ARRAY_LEN(iw_ie_cypher_name)

/* Values for the IW_IE_KEY_MGMT_* in GENIE */
static const char *    iw_ie_key_mgmt_name[] = {
    "none",
    "802.1x",
    "PSK",
};
#define    IW_IE_KEY_MGMT_NUM    IW_ARRAY_LEN(iw_ie_key_mgmt_name)

#endif    /* WE_ESSENTIAL */


/*------------------------------------------------------------------*/
/*
 * Parse, and display the results of a WPA or WPA2 IE.
 *
 */
static inline void
iw_print_ie_wpa(unsigned char *    iebuf,
                int        buflen)
{
    int            ielen = iebuf[1] + 2;
    int            offset = 2;    /* Skip the IE id, and the length. */
    unsigned char        wpa1_oui[3] = {0x00, 0x50, 0xf2};
    unsigned char        wpa2_oui[3] = {0x00, 0x0f, 0xac};
    unsigned char *    wpa_oui;
    int            i;
    uint16_t        ver = 0;
    uint16_t        cnt = 0;
    
    if(ielen > buflen)
        ielen = buflen;
    
#ifdef DEBUG
    /* Debugging code. In theory useless, because it's debugged ;-) */
    printf("IE raw value %d [%02X", buflen, iebuf[0]);
    for(i = 1; i < buflen; i++)
        printf(":%02X", iebuf[i]);
    printf("]\n");
#endif
    
    switch(iebuf[0])
    {
        case 0x30:        /* WPA2 */
            /* Check if we have enough data */
            if(ielen < 4)
            {
                iw_print_ie_unknown(iebuf, buflen);
                return;
            }
            
            wpa_oui = wpa2_oui;
            break;
            
        case 0xdd:        /* WPA or else */
            wpa_oui = wpa1_oui;
            
            /* Not all IEs that start with 0xdd are WPA.
             * So check that the OUI is valid. Note : offset==2 */
            if((ielen < 8)
               || (memcmp(&iebuf[offset], wpa_oui, 3) != 0)
               || (iebuf[offset + 3] != 0x01))
            {
                iw_print_ie_unknown(iebuf, buflen);
                return;
            }
            
            /* Skip the OUI type */
            offset += 4;
            break;
            
        default:
            return;
    }
    
    /* Pick version number (little endian) */
    ver = iebuf[offset] | (iebuf[offset + 1] << 8);
    offset += 2;
    
    if(iebuf[0] == 0xdd)
        printf("WPA Version %d\n", ver);
    if(iebuf[0] == 0x30)
        printf("IEEE 802.11i/WPA2 Version %d\n", ver);
    
    /* From here, everything is technically optional. */
    
    /* Check if we are done */
    if(ielen < (offset + 4))
    {
        /* We have a short IE.  So we should assume TKIP/TKIP. */
        printf("                        Group Cipher : TKIP\n");
        printf("                        Pairwise Cipher : TKIP\n");
        return;
    }
    
    /* Next we have our group cipher. */
    if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
    {
        printf("                        Group Cipher : Proprietary\n");
    }
    else
    {
        printf("                        Group Cipher :");
        iw_print_value_name(iebuf[offset+3],
                            iw_ie_cypher_name, IW_IE_CYPHER_NUM);
        printf("\n");
    }
    offset += 4;
    
    /* Check if we are done */
    if(ielen < (offset + 2))
    {
        /* We don't have a pairwise cipher, or auth method. Assume TKIP. */
        printf("                        Pairwise Ciphers : TKIP\n");
        return;
    }
    
    /* Otherwise, we have some number of pairwise ciphers. */
    cnt = iebuf[offset] | (iebuf[offset + 1] << 8);
    offset += 2;
    printf("                        Pairwise Ciphers (%d) :", cnt);
    
    if(ielen < (offset + 4*cnt))
        return;
    
    for(i = 0; i < cnt; i++)
    {
        if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
        {
            printf(" Proprietary");
        }
        else
        {
            iw_print_value_name(iebuf[offset+3],
                                iw_ie_cypher_name, IW_IE_CYPHER_NUM);
        }
        offset+=4;
    }
    printf("\n");
    
    /* Check if we are done */
    if(ielen < (offset + 2))
        return;
    
    /* Now, we have authentication suites. */
    cnt = iebuf[offset] | (iebuf[offset + 1] << 8);
    offset += 2;
    printf("                        Authentication Suites (%d) :", cnt);
    
    if(ielen < (offset + 4*cnt))
        return;
    
    for(i = 0; i < cnt; i++)
    {
        if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
        {
            printf(" Proprietary");
        }
        else
        {
            iw_print_value_name(iebuf[offset+3],
                                iw_ie_key_mgmt_name, IW_IE_KEY_MGMT_NUM);
        }
        offset+=4;
    }
    printf("\n");
    
    /* Check if we are done */
    if(ielen < (offset + 1))
        return;
    
    /* Otherwise, we have capabilities bytes.
     * For now, we only care about preauth which is in bit position 1 of the
     * first byte.  (But, preauth with WPA version 1 isn't supposed to be
     * allowed.) 8-) */
    if(iebuf[offset] & 0x01)
    {
        printf("                       Preauthentication Supported\n");
    }
}


/*------------------------------------------------------------------*/
/*
 * Process a generic IE and display the info in human readable form
 * for some of the most interesting ones.
 * For now, we only decode the WPA IEs.
 */
static inline void
iw_print_gen_ie(unsigned char *    buffer,
                int        buflen)
{
    int offset = 0;
    
    /* Loop on each IE, each IE is minimum 2 bytes */
    while(offset <= (buflen - 2))
    {
        printf("                    IE: ");
        
        /* Check IE type */
        switch(buffer[offset])
        {
            case 0xdd:    /* WPA1 (and other) */
            case 0x30:    /* WPA2 */
                iw_print_ie_wpa(buffer + offset, buflen);
                break;
            default:
                iw_print_ie_unknown(buffer + offset, buflen);
        }
        /* Skip over this IE to the next one in the list. */
        offset += buffer[offset+1] + 2;
    }
}

/*------------------------------------------------------------------*/
/*
 * Print one element from the scanning results
 */
void
print_scanning_token(struct stream_descr *    stream,    /* Stream of events */
                     struct iw_event *        event,    /* Extracted token */
                     struct iwscan_state *    state,
                     struct iw_range *    iw_range,    /* Range info */
                     int        has_range)
{
    char        buffer[128];    /* Temporary buffer */
    
    /* Now, let's decode the event */
    switch(event->cmd)
    {
        case SIOCGIWAP:
            printf("          Cell %02d - Address: %s\n", state->ap_num,
                   iw_saether_ntop(&event->u.ap_addr, buffer));
            state->ap_num++;
            break;
        case SIOCGIWNWID:
            if(event->u.nwid.disabled)
                printf("                    NWID:off/any\n");
            else
                printf("                    NWID:%X\n", event->u.nwid.value);
            break;
        case SIOCGIWFREQ:
        {
            double        freq;            /* Frequency/channel */
            int        channel = -1;        /* Converted to channel */
            freq = iw_freq2float(&(event->u.freq));
            /* Convert to channel if possible */
            if(has_range)
                channel = iw_freq_to_channel(freq, iw_range);
            iw_print_freq(buffer, sizeof(buffer),
                          freq, channel, event->u.freq.flags);
            printf("                    %s\n", buffer);
        }
            break;
        case SIOCGIWMODE:
            /* Note : event->u.mode is unsigned, no need to check <= 0 */
            if(event->u.mode >= IW_NUM_OPER_MODE)
                event->u.mode = IW_NUM_OPER_MODE;
            printf("                    Mode:%s\n",
                   iw_operation_mode[event->u.mode]);
            break;
        case SIOCGIWNAME:
            printf("                    Protocol:%-1.16s\n", event->u.name);
            break;
        case SIOCGIWESSID:
        {
            char essid[IW_ESSID_MAX_SIZE+1];
            memset(essid, '\0', sizeof(essid));
            if((event->u.essid.pointer) && (event->u.essid.length))
                memcpy(essid, event->u.essid.pointer, event->u.essid.length);
            if(event->u.essid.flags)
            {
                /* Does it have an ESSID index ? */
                if((event->u.essid.flags & IW_ENCODE_INDEX) > 1)
                    printf("                    ESSID:\"%s\" [%d]\n", essid,
                           (event->u.essid.flags & IW_ENCODE_INDEX));
                else
                    printf("                    ESSID:\"%s\"\n", essid);
            }
            else
                printf("                    ESSID:off/any/hidden\n");
        }
            break;
        case SIOCGIWENCODE:
        {
            unsigned char    key[IW_ENCODING_TOKEN_MAX];
            if(event->u.data.pointer)
                memcpy(key, event->u.data.pointer, event->u.data.length);
            else
                event->u.data.flags |= IW_ENCODE_NOKEY;
            printf("                    Encryption key:");
            if(event->u.data.flags & IW_ENCODE_DISABLED)
                printf("off\n");
            else
            {
                /* Display the key */
                iw_print_key(buffer, sizeof(buffer), key, event->u.data.length,
                             event->u.data.flags);
                printf("%s", buffer);
                
                /* Other info... */
                if((event->u.data.flags & IW_ENCODE_INDEX) > 1)
                    printf(" [%d]", event->u.data.flags & IW_ENCODE_INDEX);
                if(event->u.data.flags & IW_ENCODE_RESTRICTED)
                    printf("   Security mode:restricted");
                if(event->u.data.flags & IW_ENCODE_OPEN)
                    printf("   Security mode:open");
                printf("\n");
            }
        }
            break;
        case SIOCGIWRATE:
            if(state->val_index == 0)
                printf("                    Bit Rates:");
            else
                if((state->val_index % 5) == 0)
                    printf("\n                              ");
                else
                    printf("; ");
            iw_print_bitrate(buffer, sizeof(buffer), event->u.bitrate.value);
            printf("%s", buffer);
            /* Check for termination */
            if(stream->value == NULL)
            {
                printf("\n");
                state->val_index = 0;
            }
            else
                state->val_index++;
            break;
//        case SIOCGIWMODUL:
//        {
//            unsigned int    modul = event->u.param.value;
//            int        i;
//            int        n = 0;
//            printf("                    Modulations :");
//            for(i = 0; i < IW_SIZE_MODUL_LIST; i++)
//            {
//                if((modul & iw_modul_list[i].mask) == iw_modul_list[i].mask)
//                {
//                    if((n++ % 8) == 7)
//                        printf("\n                        ");
//                    else
//                        printf(" ; ");
//                    printf("%s", iw_modul_list[i].cmd);
//                }
//            }
//            printf("\n");
//        }
//            break;
        case IWEVQUAL:
            iw_print_stats(buffer, sizeof(buffer),
                           &event->u.qual, iw_range, has_range);
            printf("                    %s\n", buffer);
            break;
#ifndef WE_ESSENTIAL
        case IWEVGENIE:
            /* Informations Elements are complex, let's do only some of them */
            iw_print_gen_ie((unsigned char *)event->u.data.pointer, event->u.data.length);
            break;
#endif    /* WE_ESSENTIAL */
        case IWEVCUSTOM:
        {
            char custom[IW_CUSTOM_MAX+1];
            if((event->u.data.pointer) && (event->u.data.length))
                memcpy(custom, event->u.data.pointer, event->u.data.length);
            custom[event->u.data.length] = '\0';
            printf("                    Extra:%s\n", custom);
        }
            break;
        default:
            printf("                    (Unknown Wireless Token 0x%04X)\n",
                   event->cmd);
    }    /* switch(event->cmd) */
}
