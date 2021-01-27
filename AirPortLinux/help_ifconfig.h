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

void scan();
int ifscan(const char *ifname);



/* Structure used for parsing event streams, such as Wireless Events
 * and scan results */
typedef struct stream_descr
{
    char *    end;        /* End of the stream */
    char *    current;    /* Current event in stream of events */
    char *    value;        /* Current value in event */
} stream_descr;


/*
 * Scan state and meta-information, used to decode events...
 */
typedef struct iwscan_state
{
    /* State */
    int            ap_num;        /* Access Point number 1->N */
    int            val_index;    /* Value in table 0->(N-1) */
} iwscan_state;

/*
 * Bit to name mapping
 */
typedef struct iwmask_name
{
    unsigned int    mask;    /* bit mask for the value */
    const char *    name;    /* human readable name for the value */
} iwmask_name;

/*
 * Types of authentication parameters
 */
typedef struct iw_auth_descr
{
    int                value;        /* Type of auth value */
    const char *            label;        /* User readable version */
    const struct iwmask_name *    names;        /* Names for this value */
    const int            num_names;    /* Number of names */
} iw_auth_descr;



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


/* Shortcuts */
typedef struct iw_statistics    iwstats;
typedef struct iw_range        iwrange;
typedef struct iw_param        iwparam;
typedef struct iw_freq        iwfreq;
typedef struct iw_quality    iwqual;
typedef struct iw_priv_args    iwprivargs;

int
iw_get_range_info(int        skfd,
                  const char *    ifname,
                  iwrange *    range);
void
print_scanning_token(struct stream_descr *    stream,    /* Stream of events */
                     struct iw_event *        event,    /* Extracted token */
                     struct iwscan_state *    state,
                     struct iw_range *    iw_range,    /* Range info */
                     int        has_range);


void
iw_init_event_stream(struct stream_descr *    stream,    /* Stream of events */
                     char *            data,
                     int            len);

int
iw_extract_event_stream(struct stream_descr *    stream,    /* Stream of events */
                        struct iw_event *    iwe,    /* Extracted event */
                        int            we_version);


/* Modes as human readable strings */
extern const char * const    iw_operation_mode[];
#define IW_NUM_OPER_MODE    7
#define IW_NUM_OPER_MODE_EXT    8

#endif /* help_ifconfig_h */
