//
//  AirPortLinux_Util.cpp
//  AirPortLinux
//
//  Created by User-PC on 2020/8/10.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "AirPortLinux.hpp"
#include <linux/types.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
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

void AirPortLinux::scanDone(OSObject *owner, ...)
{
//    return ;
//    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("--%s: line = %d", __FUNCTION__, __LINE__);
    struct device *dev = (struct device *)&_pdev->dev;
//    const char *ifname = dev->dev->ifname;
//    int skfd = 0, ret;
//    struct iw_range    range;
//    int            has_range;
//
//    struct iwreq        wrq;
//    wrq.u.data.pointer = NULL;
//    wrq.u.data.flags = 0;
//    wrq.u.data.length = 0;
//
//
//    /* Get range stuff */
//    has_range = (iw_get_range_info(skfd, ifname, &range) >= 0);
//
//
//
//    unsigned char *    buffer;
//    int            buflen = IW_SCAN_MAX_DATA; /* Min for compat WE<17 */
//
//    /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
//    buffer = (unsigned char *)realloc(buffer, buflen);
//
//    /* Try to read the results */
//    wrq.u.data.pointer = buffer;
//    wrq.u.data.flags = 0;
//    wrq.u.data.length = buflen;
//    if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0)
//    {
//        DebugLog("--%s: line = %d error", __FUNCTION__, __LINE__);
//    }
    
    
    DebugLog("--%s: line = %d end", __FUNCTION__, __LINE__);
    
//    if(wrq.u.data.length)
//    {
//        struct iw_event        iwe;
//        struct stream_descr    stream;
//        struct iwscan_state    state = { .ap_num = 1, .val_index = 0 };
//        int            ret;
//
//#ifdef DEBUG
//        /* Debugging code. In theory useless, because it's debugged ;-) */
//        int    i;
//        printf("Scan result %d [%02X", wrq.u.data.length, buffer[0]);
//        for(i = 1; i < wrq.u.data.length; i++)
//            printf(":%02X", buffer[i]);
//        printf("]\n");
//#endif
//        printf("%-8.16s  Scan completed :\n", ifname);
//        iw_init_event_stream(&stream, (char *) buffer, wrq.u.data.length);
//        do
//        {
//            /* Extract an event and print it */
//            ret = iw_extract_event_stream(&stream, &iwe,
//                                          range.we_version_compiled);
//            if(ret > 0)
//                print_scanning_token(&stream, &iwe, &state,
//                                     &range, has_range);
//        }
//        while(ret > 0);
//        printf("\n");
//    }
//    else
//        printf("%-8.16s  No scan results\n\n", ifname);

    

    //    dev->dev->scanFreeResults();
//    dev->dev->scanComplete();
#ifndef Ethernet
    dev->dev->iface->postMessage(APPLE80211_M_SCAN_DONE);
    dev->dev->iface->postMessage(APPLE80211_M_COUNTRY_CODE_CHANGED);
#endif
}

//OSString *AirPortLinux::getNVRAMProperty(char *name)
//{
//    OSString *value = OSString::withCString("");
//    char val[120];
//    int ret = 0;
//    IOLog("Start Get NARAM value %s", name);
//    if (IORegistryEntry *nvram = OSDynamicCast(IORegistryEntry, IODTNVRAM::fromPath("/options", gIODTPlane)))
//    {
//        if (OSData *buf = OSDynamicCast(OSData, nvram->getProperty(name))) {
//            buf->appendByte('\0', 1);
//
//            ret = snprintf(val, buf->getLength(), "%s", buf->getBytesNoCopy());
//            IOLog("Get NARAM value %s=%s successed ret = %d", name,val, ret);
//            if (ret == -1)
//                return value;
//            value = OSString::withCString(val);
//
//            IOLog("Get NARAM value %s=%s successed", name,val);
//        }else
//        {
//            IOLog("Get NARAM value %s failed: Cna't getProperty %s", name, name);
//        }
//        nvram->release();
//    } else {
//        IOLog("Get NARAM value %s failed: Can't get /options", name);
//    }
//    return value;
//}
