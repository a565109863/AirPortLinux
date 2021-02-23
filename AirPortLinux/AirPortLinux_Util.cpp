//
//  AirPortLinux_Util.cpp
//  AirPortLinux
//
//  Created by User-PC on 2020/8/10.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "AirPortLinux.hpp"
#include <linux/types.h>
#include <linux/_malloc.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/ieee80211.h>
//
//void AirPortLinux::loadfirmware( OSKextRequestTag requestTag, OSReturn result, const void *resourceData, uint32_t resourceDataLength, void *context) {
//    AirPortLinux *dev = (AirPortLinux *)context;
//    if(result == kOSReturnSuccess) {
//        dev->firmwareData = OSData::withBytes(resourceData, resourceDataLength);
//    } else
//        printf("firmwareLoadComplete FAILURE: %08x.\n", result);
//    IOLockWakeup(dev->fwLoadLock, dev, true);
//}
//
////void AirPortLinux::loadfirmware(const char* name) {
////    for (int i = 0; i < firmwares_total; i++) {
////        if (strcmp(firmwares[i].name, name) == 0) {
////            firmware fw = firmwares[i];
////            this->firmwareData = OSData::withBytes(fw.data, fw.size);
////            return;
////        }
////    }
////}
//
//int AirPortLinux::loadfirmware(const char *firmware_name, u_char **bufp, size_t *buflen)
//{
//    //    IOLockLock(this->fwLoadLock);
//    //
//    //    OSReturn ret = OSKextRequestResource(OSKextGetCurrentIdentifier(),
//    //                                         firmware_name,
//    //                                         firmwareLoadComplete,
//    //                                         this,
//    //                                         NULL);
//    //    if(ret != kOSReturnSuccess) {
//    //        IOLog("%s Unable to load firmware file %08x\n", __FUNCTION__, ret);
//    //        IOLockUnlock(this->fwLoadLock);
//    //        return 1;
//    //    }
//    //    IOLockSleep(this->fwLoadLock, this, THREAD_INTERRUPTIBLE);
//    //    IOLockUnlock(this->fwLoadLock);
//
//    //    FWFamily = new AirPortLinuxFWFamily();
//    //    this->firmwareData = this->fw->loadfirmware(firmware_name);
//
//    *buflen = this->firmwareData->getLength();
//    *bufp = (u_char *)malloc(*buflen, M_DEVBUF, M_NOWAIT);
//    memcpy(*bufp , (u_char*)this->firmwareData->getBytesNoCopy(), *buflen);
//
//    this->firmwareData->release();
//    this->firmwareData = NULL;
//
//    //    *bufp = (u_char *)this->firmwareData->getBytesNoCopy();
//
//    return 0;
//}
//
//void AirPortLinux::if_input(struct ifnet* ifp, struct mbuf_list *ml)
//{
//    int packets = 0;
//    mbuf_t m;
//    while ((m = ml_dequeue(ml)) != NULL) {
//        if (ifp->iface == NULL) {
//            panic("%s ifq->iface == NULL!!!\n", __FUNCTION__);
//            break;
//        }
//        //        ifp->fInterface->inputPacket(m, mbuf_len(m), IONetworkInterface::kInputOptionQueuePacket, 0);
//        int err = this->enqueueInputPacket2(m);
//        if (err != kIOReturnSuccess)
//            continue;
//
//        packets ++;
//        if (ifp->netStat != NULL) {
//            ifp->if_ipackets++;
//        }
//    }
//
//    if (packets)
//        this->flushInputQueue2();
//}
//
//int AirPortLinux::chanspec2applechannel(int flags)
//{
//    int ret = 0;
//    if (flags & IEEE80211_CHAN_2GHZ)    ret |= APPLE80211_C_FLAG_2GHZ;
//    if (flags & IEEE80211_CHAN_5GHZ)    ret |= APPLE80211_C_FLAG_5GHZ;
//    if (!(flags & IEEE80211_CHAN_PASSIVE))    ret |= APPLE80211_C_FLAG_ACTIVE;
//    if (flags & IEEE80211_CHAN_OFDM)    ret |= APPLE80211_C_FLAG_20MHZ; // XXX ??
//    if (flags & IEEE80211_CHAN_CCK)        ret |= APPLE80211_C_FLAG_10MHZ; // XXX ??
//    if (flags & IEEE80211_CHAN_VHT)        ret |= APPLE80211_C_FLAG_5GHZ; // XXX ??
//    // 0x400 0x204 0x2  0x4 0x1 0x8 0x10 0x100
//    return ret;//0x400 |0x204| 0x2|  0x4| 0x1| 0x8| 0x10| 0x100 | 0x20 | 0x40 | 0x80;
//}
//
//
//bool AirPortLinux::isConnected()
//{
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//
//    return ic->ic_state == IEEE80211_S_RUN &&
//    (ic->ic_opmode != IEEE80211_M_STA ||
//     !(ic->ic_flags & IEEE80211_F_RSNON) ||
//     ic->ic_bss->ni_port_valid);
//}
//
//bool AirPortLinux::isRun80211X()
//{
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//
//    return ic->ic_state == IEEE80211_S_RUN &&
//    (ic->ic_opmode != IEEE80211_M_STA || (ic->ic_rsnakms & IEEE80211_AKM_8021X || ic->ic_rsnakms & IEEE80211_AKM_SHA256_8021X));
//}
//
//struct ieee80211_nodereq* AirPortLinux::findScanResult(apple80211_assoc_data* ad)
//{
//    // find the scan result corresponding to the given assoc params
//    if (!ad) return NULL;
//
//    //    scanFreeResults();
//    scanComplete();
//
//    if (scanResults->getCount() == 0) return NULL;
//
//    bool emptyBSSID = (strncmp((char*)&ad->ad_bssid, (char *)empty_macaddr, APPLE80211_ADDR_LEN) == 0);
//
//    for (int i = 0; i < scanResults->getCount(); i++) {
//        OSObject* scanObj = scanResults->getObject(scanIndex++);
//        if (scanObj == NULL) {
//            continue;
//        }
//
//        OSData* scanresult = OSDynamicCast(OSData, scanObj);
//        struct ieee80211_nodereq *na_node = (struct ieee80211_nodereq *)scanresult->getBytesNoCopy();
//
//        if (!emptyBSSID && strncmp((char*) &ad->ad_bssid, (char*) na_node->nr_bssid, APPLE80211_ADDR_LEN) == 0) {
//            return na_node;
//        }
//
//        if (strncmp((char*) na_node->nr_nwid,
//                    (char*) ad->ad_ssid,
//                    min(ad->ad_ssid_len, na_node->nr_nwid_len)) != 0)
//        {
//            continue;
//        } else {
//            return na_node;
//        }
//    }
//    return NULL;
//}
//
//IOReturn AirPortLinux::scanConvertResult(struct ieee80211_nodereq *nr, struct apple80211_scan_result* oneResult)
//{
//    bzero(oneResult, sizeof(apple80211_scan_result));
//
//    oneResult->version = APPLE80211_VERSION;
//
//    oneResult->asr_channel.version = APPLE80211_VERSION;
//    oneResult->asr_channel.channel = nr->nr_channel;
//    oneResult->asr_channel.flags = chanspec2applechannel(nr->nr_chan_flags);
//
//    oneResult->asr_noise = 0;
//    oneResult->asr_rssi = nr->nr_rssi - 100;
//    oneResult->asr_snr = oneResult->asr_rssi - oneResult->asr_noise;
//    oneResult->asr_beacon_int = nr->nr_intval;
//    oneResult->asr_cap = nr->nr_capinfo;
//    bcopy(nr->nr_bssid, oneResult->asr_bssid.octet, IEEE80211_ADDR_LEN);
//    oneResult->asr_nrates = nr->nr_nrates;
//    for (int r = 0; r < oneResult->asr_nrates; r++)
//        oneResult->asr_rates[r] = nr->nr_rates[r];
//    oneResult->asr_ssid_len = nr->nr_nwid_len;
//    bcopy(nr->nr_nwid, oneResult->asr_ssid, oneResult->asr_ssid_len);
//
//    oneResult->asr_age = 0;
//    oneResult->asr_ie_len = 0;
//    if (nr->nr_ie != NULL && nr->nr_ie_len > 0) {
//        oneResult->asr_ie_len = nr->nr_ie_len;
//        oneResult->asr_ie_data = nr->nr_ie;
//        //        oneResult->asr_ie_data = IOMalloc(oneResult->asr_ie_len);
//        //        bcopy(nr->nr_ie, oneResult->asr_ie_data, oneResult->asr_ie_len);
//    }
//
//    return 0;
//}
//
//void AirPortLinux::scanComplete()
//{
//    int i;
//    struct ieee80211_nodereq_all *na = (struct ieee80211_nodereq_all *)IOMalloc(sizeof(struct ieee80211_nodereq_all));
//    struct ieee80211_nodereq *nr = (struct ieee80211_nodereq *)IOMalloc(512 * sizeof(struct ieee80211_nodereq));
//
//    na->na_node = nr;
//    na->na_size = 512 * sizeof(struct ieee80211_nodereq);
//    strlcpy(na->na_ifname, "AirPortLinux", strlen("AirPortLinux"));
//
//    if (_ifp->if_ioctl(_ifp, SIOCG80211ALLNODES, (caddr_t)na) != 0) {
//        return;
//    }
//
//    for (i = 0; i < na->na_nodes; i++) {
//        struct ieee80211_nodereq *na_node = na->na_node + i;
//
//        if (strcmp((char *)na_node->nr_nwid, "") == 0)
//        {
//            continue;
//        }
//
//        OSData* scanresult = OSData::withBytes(na_node, sizeof(*na_node));
//        if (!scanresult) {
//            continue;
//        }
//
//        scanResults->setObject(scanresult);
//
//        RELEASE(scanresult);
//    }
//
//    IOFree(na, sizeof(struct ieee80211_nodereq_all));
//    IOFree(nr, 512 * sizeof(struct ieee80211_nodereq));
//}

void AirPortLinux::scanFreeResults()
{
    scanResults->flushCollection();
    scanIndex = 0;
    return;
}

/**
 * wpa_printf - conditional printf
 * @level: priority level (MSG_*) of the message
 * @fmt: printf format string, followed by optional arguments
 *
 * This function is used to print conditional debugging and error messages. The
 * output may be directed to stdout, stderr, and/or syslog based on
 * configuration.
 *
 * Note: New line '\n' is added to the end of the text when printing to stdout.
 */
//void wpa_printf(int level, const char *fmt, ...)
//{
//    va_list ap;
//
//    va_start(ap, fmt);
//    vprintf(fmt, ap);
//    printf("\n");
//    va_end(ap);
//}

#define wpa_printf(msg, arg...) kprintf(arg)

static u8 * wpa_driver_wext_giwscan(size_t *len)
{
    struct device *dev = (struct device *)&_pdev->dev;
    const char *ifname = dev->dev->ifname;
    int skfd = 0, errno;
    
    struct iwreq iwr;
    u8 *res_buf;
    size_t res_buf_len;
    
    res_buf_len = IW_SCAN_MAX_DATA;
    for (;;) {
        res_buf = (typeof res_buf)malloc(res_buf_len);
        if (res_buf == NULL)
            return NULL;
        memset(&iwr, 0, sizeof(iwr));
        strlcpy(iwr.ifr_name, ifname, IFNAMSIZ);
        iwr.u.data.pointer = res_buf;
        iwr.u.data.length = res_buf_len;
        
        
        errno = ioctl(skfd, SIOCGIWSCAN, &iwr);
        if (errno == 0 || res_buf_len >= 65535)
            break;
        
        if (-errno == E2BIG && res_buf_len < 65535) {
            free(res_buf, 1, res_buf_len);
            res_buf = NULL;
            res_buf_len *= 2;
            if (res_buf_len > 65535)
                res_buf_len = 65535; /* 16-bit length field */
            wpa_printf(MSG_DEBUG, "Scan results did not fit - "
                       "trying larger buffer (%lu bytes)",
                       (unsigned long) res_buf_len);
        } else {
            wpa_printf(MSG_ERROR, "ioctl[SIOCGIWSCAN]: %d",
                       errno);
            free(res_buf, 1, res_buf_len);
            return NULL;
        }
    }
    
    if (iwr.u.data.length > res_buf_len) {
        free(res_buf, 1, res_buf_len);
        return NULL;
    }
    *len = iwr.u.data.length;
    
    DebugLog("--%s: line = %d, *len = %d", __FUNCTION__, __LINE__, *len);
    
    return res_buf;
}

struct wpa_scan_res {
    unsigned int flags;
    u8 bssid[ETH_ALEN];
    int freq;
    u16 beacon_int;
    u16 caps;
    int qual;
    int noise;
    int level;
    u64 tsf;
    unsigned int age;
    unsigned int est_throughput;
    int snr;
    u64 parent_tsf;
    u8 tsf_bssid[ETH_ALEN];
    size_t ie_len;
    size_t beacon_ie_len;
    /* Followed by ie_len + beacon_ie_len octets of IE data */
};

/*
 * Data structure for collecting WEXT scan results. This is needed to allow
 * the various methods of reporting IEs to be combined into a single IE buffer.
 */
struct wext_scan_data {
    struct wpa_scan_res res;
    u8 *ie;
    size_t ie_len;
    u8 ssid[APPLE80211_MAX_SSID_LEN];
    size_t ssid_len;
    int maxrate;
};

/**
 * struct wpa_scan_results - Scan results
 * @res: Array of pointers to allocated variable length scan result entries
 * @num: Number of entries in the scan result array
 * @fetch_time: Time when the results were fetched from the driver
 */
struct wpa_scan_results {
    struct wpa_scan_res **res;
    size_t num;
//    struct reltime fetch_time;
};

#define IEEE80211_CAP_ESS    0x0001
#define IEEE80211_CAP_IBSS    0x0002
#define IEEE80211_CAP_PRIVACY    0x0010
#define IEEE80211_CAP_RRM    0x1000

/* DMG (60 GHz) IEEE 802.11ad */
/* type - bits 0..1 */
#define IEEE80211_CAP_DMG_MASK    0x0003
#define IEEE80211_CAP_DMG_IBSS    0x0001 /* Tx by: STA */
#define IEEE80211_CAP_DMG_PBSS    0x0002 /* Tx by: PCP */
#define IEEE80211_CAP_DMG_AP    0x0003 /* Tx by: AP */

#define WPA_SCAN_QUAL_INVALID        BIT(0)
#define WPA_SCAN_NOISE_INVALID        BIT(1)
#define WPA_SCAN_LEVEL_INVALID        BIT(2)
#define WPA_SCAN_LEVEL_DBM        BIT(3)
#define WPA_SCAN_ASSOCIATED        BIT(5)

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

/*
 * Compact form for string representation of MAC address
 * To be used, e.g., for constructing dbus paths for P2P Devices
 */
#define COMPACT_MACSTR "%02x%02x%02x%02x%02x%02x"

enum {
    MSG_EXCESSIVE, MSG_MSGDUMP, MSG_DEBUG, MSG_INFO, MSG_WARNING, MSG_ERROR
};

static void wext_get_scan_mode(struct iw_event *iwe,
                               struct wext_scan_data *res)
{
    if (iwe->u.mode == IW_MODE_ADHOC)
        res->res.caps |= IEEE80211_CAP_IBSS;
    else if (iwe->u.mode == IW_MODE_MASTER || iwe->u.mode == IW_MODE_INFRA)
        res->res.caps |= IEEE80211_CAP_ESS;
}


static void wext_get_scan_ssid(struct iw_event *iwe,
                               struct wext_scan_data *res, char *custom,
                               char *end)
{
    int ssid_len = iwe->u.essid.length;
    if (ssid_len > end - custom)
        return;
    if (iwe->u.essid.flags &&
        ssid_len > 0 &&
        ssid_len <= IW_ESSID_MAX_SIZE) {
        memcpy(res->ssid, custom, ssid_len);
        res->ssid_len = ssid_len;
    }
}


static void wext_get_scan_freq(struct iw_event *iwe,
                               struct wext_scan_data *res)
{
    int divi = 1000000, i;
    
    if (iwe->u.freq.e == 0) {
        /*
         * Some drivers do not report frequency, but a channel.
         * Try to map this to frequency by assuming they are using
         * IEEE 802.11b/g.  But don't overwrite a previously parsed
         * frequency if the driver sends both frequency and channel,
         * since the driver may be sending an A-band channel that we
         * don't handle here.
         */
        
        if (res->res.freq)
            return;
        
        if (iwe->u.freq.m >= 1 && iwe->u.freq.m <= 13) {
            res->res.freq = 2407 + 5 * iwe->u.freq.m;
            return;
        } else if (iwe->u.freq.m == 14) {
            res->res.freq = 2484;
            return;
        }
    }
    
    if (iwe->u.freq.e > 6) {
        wpa_printf(MSG_DEBUG, "Invalid freq in scan results (BSSID="
                   MACSTR " m=%d e=%d)",
                   MAC2STR(res->res.bssid), iwe->u.freq.m,
                   iwe->u.freq.e);
        return;
    }
    
    for (i = 0; i < iwe->u.freq.e; i++)
        divi /= 10;
    res->res.freq = iwe->u.freq.m / divi;
}


static void wext_get_scan_qual(struct iw_event *iwe,
                               struct wext_scan_data *res)
{
    res->res.qual = iwe->u.qual.qual;
    res->res.noise = iwe->u.qual.noise;
    res->res.level = iwe->u.qual.level;
    if (iwe->u.qual.updated & IW_QUAL_QUAL_INVALID)
        res->res.flags |= WPA_SCAN_QUAL_INVALID;
    if (iwe->u.qual.updated & IW_QUAL_LEVEL_INVALID)
        res->res.flags |= WPA_SCAN_LEVEL_INVALID;
    if (iwe->u.qual.updated & IW_QUAL_NOISE_INVALID)
        res->res.flags |= WPA_SCAN_NOISE_INVALID;
    if (iwe->u.qual.updated & IW_QUAL_DBM)
        res->res.flags |= WPA_SCAN_LEVEL_DBM;
    if ((iwe->u.qual.updated & IW_QUAL_DBM) ||
        ((iwe->u.qual.level != 0) &&
         (iwe->u.qual.level > 60))) {
            if (iwe->u.qual.level >= 64)
                res->res.level -= 0x100;
            if (iwe->u.qual.noise >= 64)
                res->res.noise -= 0x100;
        }
}


static void wext_get_scan_encode(struct iw_event *iwe,
                                 struct wext_scan_data *res)
{
    if (!(iwe->u.data.flags & IW_ENCODE_DISABLED))
        res->res.caps |= IEEE80211_CAP_PRIVACY;
}


static void wext_get_scan_rate(struct iw_event *iwe,
                               struct wext_scan_data *res, char *pos,
                               char *end)
{
    int maxrate;
    char *custom = pos + IW_EV_LCP_LEN;
    struct iw_param p;
    size_t clen;
    
    clen = iwe->len;
    if (clen > (size_t) (end - custom))
        return;
    maxrate = 0;
    while (((ssize_t) clen) >= (ssize_t) sizeof(struct iw_param)) {
        /* Note: may be misaligned, make a local, aligned copy */
        memcpy(&p, custom, sizeof(struct iw_param));
        if (p.value > maxrate)
            maxrate = p.value;
        clen -= sizeof(struct iw_param);
        custom += sizeof(struct iw_param);
    }
    
    /* Convert the maxrate from WE-style (b/s units) to
     * 802.11 rates (500000 b/s units).
     */
    res->maxrate = maxrate / 500000;
}


static void wext_get_scan_iwevgenie(struct iw_event *iwe,
                                    struct wext_scan_data *res, char *custom,
                                    char *end)
{
    char *genie, *gpos, *gend;
    u8 *tmp;
    
    if (iwe->u.data.length == 0)
        return;
    
    gpos = genie = custom;
    gend = genie + iwe->u.data.length;
    if (gend > end) {
        wpa_printf(MSG_INFO, "IWEVGENIE overflow");
        return;
    }
    
    tmp = (typeof tmp)realloc(res->ie, res->ie_len + gend - gpos);
    if (tmp == NULL)
        return;
    memcpy(tmp + res->ie_len, gpos, gend - gpos);
    res->ie = tmp;
    res->ie_len += gend - gpos;
}


static int hex2num(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}


int hex2byte(const char *hex)
{
    int a, b;
    a = hex2num(*hex++);
    if (a < 0)
        return -1;
    b = hex2num(*hex++);
    if (b < 0)
        return -1;
    return (a << 4) | b;
}


/**
 * hexstr2bin - Convert ASCII hex string into binary data
 * @hex: ASCII hex string (e.g., "01ab")
 * @buf: Buffer for the binary data
 * @len: Length of the text to convert in bytes (of buf); hex will be double
 * this size
 * Returns: 0 on success, -1 on failure (invalid hex string)
 */
int hexstr2bin(const char *hex, u8 *buf, size_t len)
{
    size_t i;
    int a;
    const char *ipos = hex;
    u8 *opos = buf;
    
    for (i = 0; i < len; i++) {
        a = hex2byte(ipos);
        if (a < 0)
            return -1;
        *opos++ = a;
        ipos += 2;
    }
    return 0;
}

static inline u64 WPA_GET_BE64(const u8 *a)
{
    return (((u64) a[0]) << 56) | (((u64) a[1]) << 48) |
    (((u64) a[2]) << 40) | (((u64) a[3]) << 32) |
    (((u64) a[4]) << 24) | (((u64) a[5]) << 16) |
    (((u64) a[6]) << 8) | ((u64) a[7]);
}

static inline u32 WPA_GET_BE32(const u8 *a)
{
    return ((u32) a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

static void wext_get_scan_custom(struct iw_event *iwe,
                                 struct wext_scan_data *res, char *custom,
                                 char *end)
{
    size_t clen;
    u8 *tmp;
    
    clen = iwe->u.data.length;
    if (clen > (size_t) (end - custom))
        return;
    
    if (clen > 7 && strncmp(custom, "wpa_ie=", 7) == 0) {
        char *spos;
        int bytes;
        spos = custom + 7;
        bytes = custom + clen - spos;
        if (bytes & 1 || bytes == 0)
            return;
        bytes /= 2;
        tmp = (typeof tmp)realloc(res->ie, res->ie_len + bytes);
        if (tmp == NULL)
            return;
        res->ie = tmp;
        if (hexstr2bin(spos, tmp + res->ie_len, bytes) < 0)
            return;
        res->ie_len += bytes;
    } else if (clen > 7 && strncmp(custom, "rsn_ie=", 7) == 0) {
        char *spos;
        int bytes;
        spos = custom + 7;
        bytes = custom + clen - spos;
        if (bytes & 1 || bytes == 0)
            return;
        bytes /= 2;
        tmp = (typeof tmp)realloc(res->ie, res->ie_len + bytes);
        if (tmp == NULL)
            return;
        res->ie = tmp;
        if (hexstr2bin(spos, tmp + res->ie_len, bytes) < 0)
            return;
        res->ie_len += bytes;
    } else if (clen > 4 && strncmp(custom, "tsf=", 4) == 0) {
        char *spos;
        int bytes;
        u8 bin[8];
        spos = custom + 4;
        bytes = custom + clen - spos;
        if (bytes != 16) {
            wpa_printf(MSG_INFO, "Invalid TSF length (%d)", bytes);
            return;
        }
        bytes /= 2;
        if (hexstr2bin(spos, bin, bytes) < 0) {
            wpa_printf(MSG_DEBUG, "WEXT: Invalid TSF value");
            return;
        }
        res->res.tsf += WPA_GET_BE64(bin);
    }
}

/**
 * get_ie - Fetch a specified information element from IEs buffer
 * @ies: Information elements buffer
 * @len: Information elements buffer length
 * @eid: Information element identifier (WLAN_EID_*)
 * Returns: Pointer to the information element (id field) or %NULL if not found
 *
 * This function returns the first matching information element in the IEs
 * buffer or %NULL in case the element is not found.
 */
const u8 * get_ie(const u8 *ies, size_t len, u8 eid)
{
    const struct element *elem;
    
    if (!ies)
        return NULL;
    
    for_each_element_id(elem, eid, ies, len)
    return &elem->id;
    
    return NULL;
}

static const u8 * wpa_scan_get_ie(const struct wpa_scan_res *res, u8 ie)
{
    return get_ie((const u8 *) (res + 1), res->ie_len, ie);
}

static void wpa_driver_wext_add_scan_entry(struct wpa_scan_results *res,
                                           struct wext_scan_data *data)
{
    struct wpa_scan_res **tmp;
    struct wpa_scan_res *r;
    size_t extra_len;
    u8 *pos, *end, *ssid_ie = NULL, *rate_ie = NULL;
    
    /* Figure out whether we need to fake any IEs */
    pos = data->ie;
    end = pos + data->ie_len;
    while (pos && end - pos > 1) {
        if (2 + pos[1] > end - pos)
            break;
        if (pos[0] == WLAN_EID_SSID)
            ssid_ie = pos;
        else if (pos[0] == WLAN_EID_SUPP_RATES)
            rate_ie = pos;
        else if (pos[0] == WLAN_EID_EXT_SUPP_RATES)
            rate_ie = pos;
        pos += 2 + pos[1];
    }
    
    extra_len = 0;
    if (ssid_ie == NULL)
        extra_len += 2 + data->ssid_len;
    if (rate_ie == NULL && data->maxrate)
        extra_len += 3;
    
    r = (typeof r)zalloc(sizeof(*r) + extra_len + data->ie_len);
    if (r == NULL)
        return;
    memcpy(r, &data->res, sizeof(*r));
    r->ie_len = extra_len + data->ie_len;
    pos = (u8 *) (r + 1);
    if (ssid_ie == NULL) {
        /*
         * Generate a fake SSID IE since the driver did not report
         * a full IE list.
         */
        *pos++ = WLAN_EID_SSID;
        *pos++ = data->ssid_len;
        memcpy(pos, data->ssid, data->ssid_len);
        pos += data->ssid_len;
    }
    if (rate_ie == NULL && data->maxrate) {
        /*
         * Generate a fake Supported Rates IE since the driver did not
         * report a full IE list.
         */
        *pos++ = WLAN_EID_SUPP_RATES;
        *pos++ = 1;
        *pos++ = data->maxrate;
    }
    if (data->ie)
        memcpy(pos, data->ie, data->ie_len);
    
    tmp = (typeof tmp)realloc_array(res->res, res->num + 1,
                           sizeof(struct wpa_scan_res *));
    if (tmp == NULL) {
        free(r, 1, sizeof(*r) + extra_len + data->ie_len);
        return;
    }
    tmp[res->num++] = r;
    res->res = tmp;
}


#define wpa_dbg(wpa, msg, arg...) kprintf(arg)

/**
 * wpa_scan_get_vendor_ie - Fetch vendor information element from a scan result
 * @res: Scan result entry
 * @vendor_type: Vendor type (four octets starting the IE payload)
 * Returns: Pointer to the information element (id field) or %NULL if not found
 *
 * This function returns the first matching information element in the scan
 * result.
 */
const u8 * wpa_scan_get_vendor_ie(const struct wpa_scan_res *res,
                                  u32 vendor_type)
{
    const u8 *end, *pos;
    
    pos = (const u8 *) (res + 1);
    end = pos + res->ie_len;
    
    while (end - pos > 1) {
        if (2 + pos[1] > end - pos)
            break;
        if (pos[0] == WLAN_EID_VENDOR_SPECIFIC && pos[1] >= 4 &&
            vendor_type == WPA_GET_BE32(&pos[2]))
            return pos;
        pos += 2 + pos[1];
    }
    
    return NULL;
}


/**
 * wpa_bss_update_scan_res - Update a BSS table entry based on a scan result
 * @wpa_s: Pointer to wpa_supplicant data
 * @res: Scan result
 * @fetch_time: Time when the result was fetched from the driver
 *
 * This function updates a BSS table entry (or adds one) based on a scan result.
 * This is called separately for each scan result between the calls to
 * wpa_bss_update_start() and wpa_bss_update_end().
 */
void wpa_bss_update_scan_res(struct wpa_scan_res *res)
{
    const u8 *ssid, *p2p, *mesh;
    
    ssid = wpa_scan_get_ie(res, WLAN_EID_SSID);
    if (ssid == NULL) {
        wpa_dbg(wpa_s, MSG_DEBUG, "BSS: No SSID IE included for "
                MACSTR, MAC2STR(res->bssid));
        return;
    }
    if (ssid[1] > APPLE80211_MAX_SSID_LEN) {
        wpa_dbg(wpa_s, MSG_DEBUG, "BSS: Too long SSID IE included for "
                MACSTR, MAC2STR(res->bssid));
        return;
    }
    
//    p2p = wpa_scan_get_vendor_ie(res, P2P_IE_VENDOR_TYPE);
//#ifdef CONFIG_P2P
//    if (p2p == NULL &&
//        wpa_s->p2p_group_interface != NOT_P2P_GROUP_INTERFACE) {
//        /*
//         * If it's a P2P specific interface, then don't update
//         * the scan result without a P2P IE.
//         */
//        wpa_printf(MSG_DEBUG, "BSS: No P2P IE - skipping BSS " MACSTR
//                   " update for P2P interface", MAC2STR(res->bssid));
//        return;
//    }
//#endif /* CONFIG_P2P */
//    if (p2p && ssid[1] == P2P_WILDCARD_SSID_LEN &&
//        memcmp(ssid + 2, P2P_WILDCARD_SSID, P2P_WILDCARD_SSID_LEN) == 0)
//        return; /* Skip P2P listen discovery results here */
    
    /* TODO: add option for ignoring BSSes we are not interested in
     * (to save memory) */
    
    mesh = wpa_scan_get_ie(res, WLAN_EID_MESH_ID);
    if (mesh && mesh[1] <= APPLE80211_MAX_SSID_LEN)
        ssid = mesh;
    
//    bss = wpa_bss_get(wpa_s, res->bssid, ssid + 2, ssid[1]);
//    if (bss == NULL)
//        bss = wpa_bss_add(wpa_s, ssid + 2, ssid[1], res, fetch_time);
//    else {
//        bss = wpa_bss_update(wpa_s, bss, res, fetch_time);
//        if (wpa_s->last_scan_res) {
//            unsigned int i;
//            for (i = 0; i < wpa_s->last_scan_res_used; i++) {
//                if (bss == wpa_s->last_scan_res[i]) {
//                    /* Already in the list */
//                    return;
//                }
//            }
//        }
//    }
//
//    if (bss == NULL)
//        return;
//    if (wpa_s->last_scan_res_used >= wpa_s->last_scan_res_size) {
//        struct wpa_bss **n;
//        unsigned int siz;
//        if (wpa_s->last_scan_res_size == 0)
//            siz = 32;
//        else
//            siz = wpa_s->last_scan_res_size * 2;
//        n = realloc_array(wpa_s->last_scan_res, siz,
//                             sizeof(struct wpa_bss *));
//        if (n == NULL)
//            return;
//        wpa_s->last_scan_res = n;
//        wpa_s->last_scan_res_size = siz;
//    }
//
//    if (wpa_s->last_scan_res)
//        wpa_s->last_scan_res[wpa_s->last_scan_res_used++] = bss;
}


static int wext_19_iw_point(u16 cmd)
{
    return (cmd == SIOCGIWESSID || cmd == SIOCGIWENCODE ||
     cmd == IWEVGENIE || cmd == IWEVCUSTOM);
}

/**
 * wpa_driver_wext_get_scan_results - Fetch the latest scan results
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * Returns: Scan results on success, -1 on failure
 */
struct wpa_scan_results *wpa_driver_wext_get_scan_results()
{
    size_t len;
    int first;
    u8 *res_buf;
    struct iw_event iwe_buf, *iwe = &iwe_buf;
    char *pos, *end, *custom;
    struct wpa_scan_results *res;
    struct wext_scan_data data;
    
    struct device *dev = (struct device *)&_pdev->dev;
    
    res_buf = wpa_driver_wext_giwscan(&len);
    if (res_buf == NULL) {
        if (dev->dev->fTimerEventSource) {
            dev->dev->fTimerEventSource->cancelTimeout();
            dev->dev->fTimerEventSource->setAction(&AirPortLinux::scanDone);
            dev->dev->fTimerEventSource->setTimeoutMS(200);
        }
        return NULL;
    }
    
    
//#ifndef Ethernet
//    dev->dev->iface->postMessage(APPLE80211_M_SCAN_DONE);
//    dev->dev->iface->postMessage(APPLE80211_M_COUNTRY_CODE_CHANGED);
//#endif
//
//    free(res_buf, 1, len);
//    return NULL;
    
    first = 1;
    
    res = (typeof res)zalloc(sizeof(*res));
    if (res == NULL) {
        free(res_buf, 1, len);
        return NULL;
    }
    
    pos = (char *) res_buf;
    end = (char *) res_buf + len;
    memset(&data, 0, sizeof(data));
    
    while ((size_t) (end - pos) >= IW_EV_LCP_LEN) {
        /* Event data may be unaligned, so make a local, aligned copy
         * before processing. */
        memcpy(&iwe_buf, pos, IW_EV_LCP_LEN);
        if (iwe->len <= IW_EV_LCP_LEN || iwe->len > end - pos)
            break;
        
        custom = pos + IW_EV_POINT_LEN;
        if (wext_19_iw_point(iwe->cmd)) {
            /* WE-19 removed the pointer from struct iw_point */
            char *dpos = (char *) &iwe_buf.u.data.length;
            int dlen = dpos - (char *) &iwe_buf;
            memcpy(dpos, pos + IW_EV_LCP_LEN,
                   sizeof(struct iw_event) - dlen);
        } else {
            memcpy(&iwe_buf, pos, sizeof(struct iw_event));
            custom += IW_EV_POINT_OFF;
        }
        
        switch (iwe->cmd) {
            case SIOCGIWAP:
                if (!first)
                    wpa_driver_wext_add_scan_entry(res, &data);
                first = 0;
                free(data.ie, 1, data.ie_len);
                memset(&data, 0, sizeof(data));
                memcpy(data.res.bssid,
                          iwe->u.ap_addr.sa_data, ETH_ALEN);
                break;
            case SIOCGIWMODE:
                wext_get_scan_mode(iwe, &data);
                break;
            case SIOCGIWESSID:
                wext_get_scan_ssid(iwe, &data, custom, end);
                break;
            case SIOCGIWFREQ:
                wext_get_scan_freq(iwe, &data);
                break;
            case IWEVQUAL:
                wext_get_scan_qual(iwe, &data);
                break;
            case SIOCGIWENCODE:
                wext_get_scan_encode(iwe, &data);
                break;
            case SIOCGIWRATE:
                wext_get_scan_rate(iwe, &data, pos, end);
                break;
            case IWEVGENIE:
                wext_get_scan_iwevgenie(iwe, &data, custom, end);
                break;
            case IWEVCUSTOM:
                wext_get_scan_custom(iwe, &data, custom, end);
                break;
        }
        
        pos += iwe->len;
    }
    free(res_buf, 1, len);
    res_buf = NULL;
    if (!first)
        wpa_driver_wext_add_scan_entry(res, &data);
    free(data.ie, 1, data.ie_len);
    
    wpa_printf(MSG_DEBUG, "Received %lu bytes of scan results (%lu BSSes)",
               (unsigned long) len, (unsigned long) res->num);
    
    return res;
}

void free_wpa_scan_res(struct wpa_scan_res *_res)
{
    free(_res);
}

void free_wpa_scan_results(struct wpa_scan_results *res)
{
    free(res, 1, sizeof(struct wpa_scan_results));
}

void AirPortLinux::scanDone(OSObject *owner, ...)
{
    struct device *dev = (struct device *)&_pdev->dev;
    
    dev->dev->res = wpa_driver_wext_get_scan_results();
    if (dev->dev->res == NULL) {
        return;
    }
    
//    dev->dev->scanResults->flushCollection();
//
//    int i = 0;
//    while (i < res->num) {
//        struct wpa_scan_res *_res = res->res[i++];
//        OSData* scanresult = OSData::withBytes(_res, sizeof(*_res));
//        if (!scanresult) {
//            continue;
//        }
//
//        dev->dev->scanResults->setObject(scanresult);
//
//        RELEASE(scanresult);
//
//        free_wpa_scan_res(_res);
//    }
//
//    IOSleep(1000);
    
    //    dev->dev->scanFreeResults();
    //    dev->dev->scanComplete();
#ifndef Ethernet
    dev->dev->iface->postMessage(APPLE80211_M_SCAN_DONE);
    dev->dev->iface->postMessage(APPLE80211_M_COUNTRY_CODE_CHANGED);
#endif
    return;
}


/**
 * enum hostapd_hw_mode - Hardware mode
 */
enum hostapd_hw_mode {
    HOSTAPD_MODE_IEEE80211B,
    HOSTAPD_MODE_IEEE80211G,
    HOSTAPD_MODE_IEEE80211A,
    HOSTAPD_MODE_IEEE80211AD,
    HOSTAPD_MODE_IEEE80211ANY,
    NUM_HOSTAPD_MODES
};

/* VHT channel widths */
#define CHANWIDTH_USE_HT    0
#define CHANWIDTH_80MHZ        1
#define CHANWIDTH_160MHZ    2
#define CHANWIDTH_80P80MHZ    3

/**
 * ieee80211_freq_to_channel_ext - Convert frequency into channel info
 * for HT40 and VHT. DFS channels are not covered.
 * @freq: Frequency (MHz) to convert
 * @sec_channel: 0 = non-HT40, 1 = sec. channel above, -1 = sec. channel below
 * @vht: VHT channel width (CHANWIDTH_*)
 * @op_class: Buffer for returning operating class
 * @channel: Buffer for returning channel number
 * Returns: hw_mode on success, NUM_HOSTAPD_MODES on failure
 */
enum hostapd_hw_mode ieee80211_freq_to_channel_ext(unsigned int freq,
                                                   int sec_channel, int vht,
                                                   u8 *op_class, u8 *channel)
{
    u8 vht_opclass;
    
    /* TODO: more operating classes */
    
    if (sec_channel > 1 || sec_channel < -1)
        return NUM_HOSTAPD_MODES;
    
    if (freq >= 2412 && freq <= 2472) {
        if ((freq - 2407) % 5)
            return NUM_HOSTAPD_MODES;
        
        if (vht)
            return NUM_HOSTAPD_MODES;
        
        /* 2.407 GHz, channels 1..13 */
        if (sec_channel == 1)
            *op_class = 83;
            else if (sec_channel == -1)
                *op_class = 84;
                else
                    *op_class = 81;
                    
                    *channel = (freq - 2407) / 5;
                    
                    return HOSTAPD_MODE_IEEE80211G;
    }
    
    if (freq == 2484) {
        if (sec_channel || vht)
            return NUM_HOSTAPD_MODES;
        
        *op_class = 82; /* channel 14 */
        *channel = 14;
        
        return HOSTAPD_MODE_IEEE80211B;
    }
    
    if (freq >= 4900 && freq < 5000) {
        if ((freq - 4000) % 5)
            return NUM_HOSTAPD_MODES;
        *channel = (freq - 4000) / 5;
        *op_class = 0; /* TODO */
        return HOSTAPD_MODE_IEEE80211A;
    }
    
    switch (vht) {
        case CHANWIDTH_80MHZ:
            vht_opclass = 128;
            break;
        case CHANWIDTH_160MHZ:
            vht_opclass = 129;
            break;
        case CHANWIDTH_80P80MHZ:
            vht_opclass = 130;
            break;
        default:
            vht_opclass = 0;
            break;
    }
    
    /* 5 GHz, channels 36..48 */
    if (freq >= 5180 && freq <= 5240) {
        if ((freq - 5000) % 5)
            return NUM_HOSTAPD_MODES;
        
        if (vht_opclass)
            *op_class = vht_opclass;
            else if (sec_channel == 1)
                *op_class = 116;
                else if (sec_channel == -1)
                    *op_class = 117;
                    else
                        *op_class = 115;
                        
                        *channel = (freq - 5000) / 5;
                        
                        return HOSTAPD_MODE_IEEE80211A;
    }
    
    /* 5 GHz, channels 52..64 */
    if (freq >= 5260 && freq <= 5320) {
        if ((freq - 5000) % 5)
            return NUM_HOSTAPD_MODES;
        
        if (vht_opclass)
            *op_class = vht_opclass;
            else if (sec_channel == 1)
                *op_class = 119;
                else if (sec_channel == -1)
                    *op_class = 120;
                    else
                        *op_class = 118;
                        
                        *channel = (freq - 5000) / 5;
                        
                        return HOSTAPD_MODE_IEEE80211A;
    }
    
    /* 5 GHz, channels 149..169 */
    if (freq >= 5745 && freq <= 5845) {
        if ((freq - 5000) % 5)
            return NUM_HOSTAPD_MODES;
        
        if (vht_opclass)
            *op_class = vht_opclass;
            else if (sec_channel == 1)
                *op_class = 126;
                else if (sec_channel == -1)
                    *op_class = 127;
                    else if (freq <= 5805)
                        *op_class = 124;
                        else
                            *op_class = 125;
                            
                            *channel = (freq - 5000) / 5;
                            
                            return HOSTAPD_MODE_IEEE80211A;
    }
    
    /* 5 GHz, channels 100..140 */
    if (freq >= 5000 && freq <= 5700) {
        if ((freq - 5000) % 5)
            return NUM_HOSTAPD_MODES;
        
        if (vht_opclass)
            *op_class = vht_opclass;
            else if (sec_channel == 1)
                *op_class = 122;
                else if (sec_channel == -1)
                    *op_class = 123;
                    else
                        *op_class = 121;
                        
                        *channel = (freq - 5000) / 5;
                        
                        return HOSTAPD_MODE_IEEE80211A;
    }
    
    if (freq >= 5000 && freq < 5900) {
        if ((freq - 5000) % 5)
            return NUM_HOSTAPD_MODES;
        *channel = (freq - 5000) / 5;
        *op_class = 0; /* TODO */
        return HOSTAPD_MODE_IEEE80211A;
    }
    
    /* 56.16 GHz, channel 1..4 */
    if (freq >= 56160 + 2160 * 1 && freq <= 56160 + 2160 * 4) {
        if (sec_channel || vht)
            return NUM_HOSTAPD_MODES;
        
        *channel = (freq - 56160) / 2160;
        *op_class = 180;
        
        return HOSTAPD_MODE_IEEE80211AD;
    }
    
    return NUM_HOSTAPD_MODES;
}

IOReturn AirPortLinux::scanConvertResult(struct apple80211_scan_result* oneResult)
{
    bzero(oneResult, sizeof(apple80211_scan_result));
    
    if (this->res->num == 0) {
        this->scanFlag = false;
        return 0x0C;
    }
    
    DebugLog("--%s: line = %d, scanIndex = %d", __FUNCTION__, __LINE__, scanIndex);
    
//    while (this->scanIndex < scanResults->getCount()) {
//        OSObject* scanObj = scanResults->getObject(this->scanIndex++);
//        if (scanObj == NULL) {
//            continue;
//        }
//
//        OSData* scanresult = OSDynamicCast(OSData, scanObj);
//        struct wpa_scan_res *_res = (struct wpa_scan_res *)scanresult->getBytesNoCopy();
    
    while (this->scanIndex < this->res->num) {
        struct wpa_scan_res *_res = this->res->res[this->scanIndex++];
        
        const u8 *ssid_ie = wpa_scan_get_ie(_res, WLAN_EID_SSID);
        uint8_t ssid_len = ssid_ie[1];
        
        u_int8_t        ni_essid[APPLE80211_MAX_SSID_LEN];
        memset(ni_essid, 0, APPLE80211_MAX_SSID_LEN * sizeof(u_int8_t));
        memcpy(ni_essid, ((u8 *)ssid_ie + 2), ssid_ie[1]);
//        ni_essid[ssid_ie[1]] = '\0';

        oneResult->version = APPLE80211_VERSION;

        oneResult->asr_channel.version = APPLE80211_VERSION;
        
        u8 current, chan;
        ieee80211_freq_to_channel_ext(_res->freq, 0, CHANWIDTH_USE_HT,
                                      &current, &chan);
        
        oneResult->asr_channel.channel = chan;
        oneResult->asr_channel.flags = _res->flags;

        oneResult->asr_noise = _res->noise;
        oneResult->asr_rssi = - 50;
        oneResult->asr_snr = _res->snr;
        oneResult->asr_beacon_int = _res->beacon_int;
        oneResult->asr_cap = _res->caps;
        bcopy(_res->bssid, oneResult->asr_bssid.octet, ETH_ALEN);
        
        const u8 *ie;
        int i;
        int         nrates = 0;
        uint32_t    rates[APPLE80211_MAX_RATES];
        
        ie = wpa_scan_get_ie(_res, WLAN_EID_SUPP_RATES);
        for (i = 0; ie && i < ie[1]; i++) {
            if (nrates < APPLE80211_MAX_RATES)
                rates[nrates++] = ie[i + 2] & 0x7f;
        }
        
        ie = wpa_scan_get_ie(_res, WLAN_EID_EXT_SUPP_RATES);
        for (i = 0; ie && i < ie[1]; i++) {
            if (nrates < APPLE80211_MAX_RATES)
                rates[nrates++] = ie[i + 2] & 0x7f;
        }
        
        oneResult->asr_nrates = nrates;
        for (int r = 0; r < oneResult->asr_nrates; r++)
            oneResult->asr_rates[r] = rates[r];
        oneResult->asr_ssid_len = ssid_len;
        bcopy(ni_essid, oneResult->asr_ssid, oneResult->asr_ssid_len);

        oneResult->asr_age = _res->age;
        oneResult->asr_ie_len = 0;
        if ((_res + 1) != NULL && _res->ie_len > 0) {
            oneResult->asr_ie_len = _res->ie_len;
//            oneResult->asr_ie_data = (_res + 1);
            oneResult->asr_ie_data = IOMalloc(oneResult->asr_ie_len);
            bcopy((_res + 1), oneResult->asr_ie_data, oneResult->asr_ie_len);
        }
        
        free_wpa_scan_res(_res);
        
        DebugLog("--%s: line = %d channel: %u,"
              "flags: %u,"
              "asr_noise: %u,"
              "asr_rssi: %u,"
              "asr_snr: %u,"
              "nrates: %u,"
              "asr_age: %u,"
              "asr_beacon_int: %u,"
              "BSSID: %s,"
              "SSID: %s",
              __FUNCTION__,
              __LINE__,
              oneResult->asr_channel.channel,
              oneResult->asr_channel.flags,
              oneResult->asr_noise,
              oneResult->asr_rssi,
              oneResult->asr_snr,
              oneResult->asr_nrates,
              oneResult->asr_age,
              oneResult->asr_beacon_int,
              ether_sprintf(oneResult->asr_bssid.octet),
              oneResult->asr_ssid);

        return kIOReturnSuccess;
    }
    
    this->scanFlag = false;
    
    this->scanIndex = 0;
    
    return 0x05;
}

