//
//  AirPortLinux_ioctl.cpp
//  AirPortLinux
//
//  Created by Mac-PC on 2020/3/18.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "AirPortLinux.hpp"

//extern struct ifnet *_ifp;

//static const u_int8_t empty_macaddr[IEEE80211_ADDR_LEN] = {
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//};

int chanspec2applechannel(int flags) {
    int ret = 0;
//    if (flags & IEEE80211_CHAN_2GHZ)    ret |= APPLE80211_C_FLAG_2GHZ;
//    if (flags & IEEE80211_CHAN_5GHZ)    ret |= APPLE80211_C_FLAG_5GHZ;
//    if (!(flags & IEEE80211_CHAN_PASSIVE))    ret |= APPLE80211_C_FLAG_ACTIVE;
//    if (flags & IEEE80211_CHAN_OFDM)    ret |= APPLE80211_C_FLAG_20MHZ; // XXX ??
//    if (flags & IEEE80211_CHAN_CCK)        ret |= APPLE80211_C_FLAG_10MHZ; // XXX ??
//    if (flags & IEEE80211_CHAN_VHT)        ret |= APPLE80211_C_FLAG_5GHZ; // XXX ??
    // 0x400 0x204 0x2  0x4 0x1 0x8 0x10 0x100
    return ret;
}

//
// MARK: 1 - SSID
//

IOReturn AirPortLinux::getSSID(IO80211Interface *interface,
                                    struct apple80211_ssid_data *ret) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == NULL)
//        return kIOReturnError;
//
//    if (ic->ic_state == IEEE80211_S_RUN) {
//        ret->version = APPLE80211_VERSION;
//        ret->ssid_len = ic->ic_bss->ni_esslen;
//        bcopy(ic->ic_bss->ni_essid, ret->ssid_bytes, ret->ssid_len);
//
//        return kIOReturnSuccess;
//    }
    return kIOReturnError;
}

//IOReturn AirPortLinux::setSSID(IO80211Interface *interface,
//                                    struct apple80211_ssid_data *sd) {
//    DebugLog("%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("%s: line = %d", __FUNCTION__, __LINE__);
//
//    return kIOReturnSuccess;
//}

//
// MARK: 2 - AUTH_TYPE
//

IOReturn AirPortLinux::getAUTH_TYPE(IO80211Interface *interface,
                                         struct apple80211_authtype_data *ad) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//       if (ic->ic_bss == NULL)
//           return kIOReturnError;
    
    u_int32_t    authtype_upper = APPLE80211_AUTHTYPE_NONE;
//    if (ic->ic_bss->ni_supported_rsnprotos & IEEE80211_PROTO_RSN && ic->ic_bss->ni_supported_rsnprotos & IEEE80211_PROTO_WPA) {
//        authtype_upper = APPLE80211_AUTHTYPE_WPA2_PSK;
//    } else if (ic->ic_bss->ni_supported_rsnprotos & IEEE80211_PROTO_RSN) {
//        authtype_upper = APPLE80211_AUTHTYPE_WPA2_PSK;
//    } else if (ic->ic_bss->ni_supported_rsnprotos & IEEE80211_PROTO_WPA) {
//        authtype_upper = APPLE80211_AUTHTYPE_WPA_PSK;
//    }
    
    ad->version = APPLE80211_VERSION;
    ad->authtype_lower = APPLE80211_AUTHTYPE_OPEN;
    ad->authtype_upper = authtype_upper; //1 << 19;
    // APPLE80211_AUTHTYPE_WPA_PSK;//APPLE80211_AUTHTYPE_NONE;
    // 0x0 无
    // 0x1 WPA企业级
    // 0x2 WPA个人级
    // 0x4 WPA2企业级
    // 0x8 WPA2个人级
    // 0x10 WPA2个人级
    // 0x20
    // 0x40
    // 0x80 WPA2企业级
    // 0x100 Wi-Fi保护设置
    // 0x200
    // 0x400 WPA2个人级
    // 0x800 WPA2企业级
    // 0x1000 WPA3个人级
    // 0x2000
    // 0x4000 不知道什么加密码类型
    // 0x8000 不知道什么加密码类型
    // 0x10000
    // 0x20000
    // 0x40000
    
    return kIOReturnSuccess;
}

IOReturn AirPortLinux::setCIPHER_KEY(IO80211Interface *interface, apple80211_key* key) {
    
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
    
    return kIOReturnSuccess;
}

//
// MARK: 4 - CHANNEL
//

IOReturn AirPortLinux::getCHANNEL(IO80211Interface *interface,
                                       struct apple80211_channel_data *cd_data) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == 0)
//        return kIOReturnError;
//    if (ic->ic_bss->ni_chan == 0)
//        return kIOReturnError;
//    cd->channel.version = APPLE80211_VERSION;
//    cd->channel.channel = ieee80211_chan2ieee(ic, ic->ic_bss->ni_chan);
//    cd->channel.flags = chanspec2applechannel(ic->ic_bss->ni_chan->ic_flags);
    
    
//    struct rx_radiotap_header *tap =(struct rx_radiotap_header *)_ifp->tap;
//    if (tap == 0)
//        return kIOReturnError;
//
//    if (tap->wr_chan_flags == 0) {
//        if (this->cd == NULL) {
//            return kIOReturnError;
//        } else {
//            bcopy(this->cd, cd_data, sizeof(struct apple80211_channel_data));
//            return kIOReturnSuccess;
//        }
//    }
//
//    if (this->cd == NULL) {
//        this->cd = new struct apple80211_channel_data;
//    }
//
//    this->cd->channel.version = APPLE80211_VERSION;
//    this->cd->channel.channel = ieee80211_mhz2ieee(tap->wr_chan_freq, tap->wr_chan_flags);
//    this->cd->channel.flags = chanspec2applechannel(tap->wr_chan_flags);
//
//    bcopy(this->cd, cd_data, sizeof(struct apple80211_channel_data));
    
    return kIOReturnSuccess;
}

//
// MARK: 5 - POWERSAVE
//

IOReturn AirPortLinux::getPOWERSAVE(IO80211Interface* interface, struct apple80211_powersave_data* ret) {
//    ret->version = APPLE80211_VERSION;
//    ret->powersave_level = _ifp->if_power_state;
    return kIOReturnSuccess;
}

//IOReturn AirPortLinux::setPOWERSAVE(IO80211Interface* interface, struct apple80211_powersave_data* ret) {
//    _ifp->if_power_state = ret->powersave_level; // FIXME
//    return kIOReturnSuccess;
//}


//
// MARK: 6 - PROTMODE
//

IOReturn AirPortLinux::getPROTMODE(IO80211Interface* interface, struct apple80211_protmode_data* ret) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    ret->protmode = APPLE80211_PROTMODE_AUTO;
//    ret->threshold = ic->ic_rtsthreshold; // XXX
    return kIOReturnSuccess;
}


//
// MARK: 7 - TXPOWER
//

IOReturn AirPortLinux::getTXPOWER(IO80211Interface *interface,
                                       struct apple80211_txpower_data *txd) {

//    struct ieee80211_txpower power;
//    if (_ifp->if_ioctl(_ifp, SIOCG80211TXPOWER, (caddr_t)&power) == -1)
//        return kIOReturnError;
//
//    txd->version = APPLE80211_VERSION;
//    txd->txpower = power.i_val;
//    txd->txpower_unit = APPLE80211_UNIT_DBM;
    return kIOReturnSuccess;
}


//IOReturn AirPortLinux::setTXPOWER(IO80211Interface *interface,
//                                       struct apple80211_txpower_data *txd) {
//
////    struct ieee80211_txpower power = {NULL, 0, txd->txpower};
////    if (_ifp->if_ioctl(_ifp, SIOCS80211TXPOWER, (caddr_t)&power) == -1)
////        return kIOReturnError;
//
//    return kIOReturnSuccess;
//}

//
// MARK: 8 - RATE
//

IOReturn AirPortLinux::getRATE(IO80211Interface *interface, struct apple80211_rate_data *rd) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == NULL)
//        return kIOReturnError;
//
//    rd->num_radios = ic->ic_bss->ni_rates.rs_nrates > APPLE80211_MAX_RADIO ? APPLE80211_MAX_RADIO : ic->ic_bss->ni_rates.rs_nrates;
//    for (int i=0;i < rd->num_radios; i++)
//    {
//        rd->rate[i] = ic->ic_bss->ni_rates.rs_rates[i];
//    }
    return kIOReturnSuccess;
}


bool scanResultsAreSimilar
( const apple80211_scan_result* a, const apple80211_scan_result* b )
{
    /* This function will check for "equality" of two scan results.
     * The definition of equality is debatable.
     */
    
    /* First test: SSID should be same */
    if (strncmp((char*) a->asr_ssid,
            (char*) b->asr_ssid,
            min(a->asr_ssid_len, b->asr_ssid_len)) != 0)
        return false;
    
    /* Next, BSSID should match */
    if (strncmp((char*) a->asr_bssid.octet,
                (char*) b->asr_bssid.octet, 6) != 0)
        return false;
    
    /* Check if it's the same channel */
    if (a->asr_channel.channel != b->asr_channel.channel)
        return false;
    
    /* .. with the same flags */
    if (a->asr_channel.flags != b->asr_channel.flags)
        return false;
    
    /* Check capability flags */
    if (a->asr_cap != b->asr_cap)
        return false;
    
    /* Channel, SSID, BSSID and Cap flags match. Lastly match on no. of supported rates */
    if (a->asr_nrates != b->asr_nrates)
        return false;
    
    /* At this stage, no. of supported rates are the same, so the last thing we'll do is match those */
    for (int i = 0; i < a->asr_nrates; i++)
        if (a->asr_rates[i] != b->asr_rates[i])
            return false;
    
    /* Channel, SSID, BSSID, cap flags and supported rates ALL match then it's the same goddamn thing */
    return true;
}

//
// MARK: 9 - BSSID
//

IOReturn AirPortLinux::getBSSID(IO80211Interface *interface,
                                     struct apple80211_bssid_data *bd) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == NULL)
//        return kIOReturnError;
//
//    bd->version = APPLE80211_VERSION;
//    bcopy(ic->ic_bss->ni_bssid, bd->bssid.octet, APPLE80211_ADDR_LEN);
//
//
////    printf("--%s: line = %d bd->bssid.octet = %s", __FUNCTION__, __LINE__,
////        ether_sprintf(bd->bssid.octet));
    
    return kIOReturnSuccess;
}


//static IOReturn scanAction(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3) {
//    IOSleep(1000);
//
//    _ifp->iface->postMessage(APPLE80211_M_SCAN_DONE);
//    return kIOReturnSuccess;
//}

//
// MARK: 10 - SCAN_REQ
//
IOReturn AirPortLinux::setSCAN_REQ(IO80211Interface *interface,
                                        struct apple80211_scan_data *sd) {

    IOLog("AirPortLinux. Scan requested. Type: %u\n"
          "BSS Type: %u\n"
          "PHY Mode: %u\n"
          "Dwell time: %u\n"
          "Rest time: %u\n"
          "Num channels: %u\n",
          sd->scan_type,
          sd->bss_type,
          sd->phy_mode,
          sd->dwell_time,
          sd->rest_time,
          sd->num_channels);

//    _ifp->fCommandGate->runAction(scanAction, interface);
    

//    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//
//    if (fScanResult)
//        return kIOReturnError;
//
////    struct ieee80211_nodereq_all na;
////    struct ieee80211_nodereq nr[512];
////    struct ifreq ifr;
//    int i;
//
//    if ((_ifp->if_flags & IFF_UP) == 0) {
//        printf("\t\tcannot scan, interface is down\n");
//        return kIOReturnError;
//    }
//
//    bzero(&na, sizeof(na));
//    bzero(&nr, sizeof(nr));
//    na.na_node = nr;
//    na.na_size = sizeof(nr);
//    strlcpy(na.na_ifname, __FUNCTION__, sizeof(na.na_ifname));
//
//    if (_ifp->if_ioctl(_ifp, SIOCG80211ALLNODES, (caddr_t)&na) != 0) {
//        return kIOReturnError;
//    }
//
//    if (!na.na_nodes)
//        printf("\t\tnone\n");
////    else
////        qsort(nr, na.na_nodes, sizeof(*nr), rssicmp);
//
//    for (i = 0; i < na.na_nodes; i++) {
//
//        DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//
//        struct ieee80211_nodereq *na_node = &na.na_node[i];
//        // 过滤空
//        if (memcmp(na_node->nr_bssid, empty_macaddr, IEEE80211_ADDR_LEN) == 0)
//        {
//            DebugLog("--%s: AP %s ", __FUNCTION__,
//            ether_sprintf(na_node->nr_bssid));
//            DebugLog("--%s: AP %s ", __FUNCTION__,
//            ether_sprintf(na_node->nr_bssid));
//            DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//            continue;
//        }
//
//        if (_scanResults->getCount() > 512) {
//            continue;
//        }
//
//        apple80211_scan_result* oneResult = new apple80211_scan_result;
//
//        oneResult->version = APPLE80211_VERSION;
//
//        oneResult->asr_channel.version = APPLE80211_VERSION;
//        oneResult->asr_channel.channel = na_node->nr_channel;
//        oneResult->asr_channel.flags = chanspec2applechannel(na_node->nr_chan_flags);
//
////        oneResult->asr_unk = APPLE80211_AUTHTYPE_WPA2_PSK;
//        oneResult->asr_noise = 0;
//        oneResult->asr_snr = 0;
//        oneResult->asr_rssi = na_node->nr_rssi;
//        oneResult->asr_beacon_int = na_node->nr_intval;
//        oneResult->asr_cap = na_node->nr_capinfo;
//        bcopy(na_node->nr_bssid, oneResult->asr_bssid, IEEE80211_ADDR_LEN);
//        oneResult->asr_nrates = na_node->nr_nrates;
//        for (int r = 0; r < oneResult->asr_nrates; r++)
//            oneResult->asr_rates[r] = na_node->nr_rates[r];
//    //    oneResult->asr_nr_unk = 8;
//        oneResult->asr_ssid_len = na_node->nr_nwid_len;
//        bcopy(na_node->nr_nwid, oneResult->asr_ssid, oneResult->asr_ssid_len);
//
//
//
//        DebugLog("---%s: line = %d oneResult->asr_ssid_len = %d", __FUNCTION__, __LINE__, oneResult->asr_ssid_len);
//
//        DebugLog("--%s: AP %s ", __FUNCTION__,
//        ether_sprintf(na_node->nr_bssid));
//
//        DebugLog("---%s: line = %d _resultsPending = %d", __FUNCTION__, __LINE__, _resultsPending);
//
//    //    oneResult->unk = 0;
//    //    oneResult->unk2 = 8;
//
//        oneResult->asr_age = 0;
//        oneResult->asr_ie_len = 0;
//        if (na_node->nr_rsnie != NULL) {
//            oneResult->asr_ie_len = na_node->nr_rsnie[1];
//            oneResult->asr_ie_data = &na_node->nr_rsnie[2];
//        }
//
//        DebugLog("---%s: line = %d na_node->nr_nwid = %s", __FUNCTION__, __LINE__,na_node->nr_nwid);
//        DebugLog("--%s: AP %s ", __FUNCTION__,
//        ether_sprintf(na_node->nr_bssid));
//
//        OSData* scanresult = OSData::withBytes(oneResult, sizeof(apple80211_scan_result));
//        _scanResults->setObject(scanresult);
//        _resultsPending = _scanResults->getCount();
//    }
//
//    DebugLog("---%s: line = %d data.na_nodes = %d", __FUNCTION__, __LINE__, na.na_nodes);
    
    
    return kIOReturnSuccess;
}

//
// MARK: 11 - SCAN_RESULT
//
IOReturn AirPortLinux::getSCAN_RESULT(IO80211Interface *interface,
                                           struct apple80211_scan_result **sr) {

//    fScanResult = true;
//
//    if (_resultsPending <= 0) {
//        _resultsPending = _scanResults->getCount(); // wrap back for later
//        fScanResult = false;
//        return kIOReturnError; // XXX
//    } else {
//        OSObject* resultobj = _scanResults->getObject(--_resultsPending);
//        if (!resultobj) { // no results - error condition
//            _resultsPending = _scanResults->getCount(); // wrap for later
//            fScanResult = false;
//            return kIOReturnError;
//        }
//        DebugLog("---%s: line = %d _resultsPending = %d", __FUNCTION__, __LINE__, _resultsPending);
//
//        OSData*    scanresult = OSDynamicCast(OSData, resultobj);
//        apple80211_scan_result* oneResult = (struct apple80211_scan_result *)scanresult->getBytesNoCopy();
////        struct ieee80211_nodereq *na_node = (struct ieee80211_nodereq *)scanresult->getBytesNoCopy();
//
//
//        DebugLog("---%s: line = %d oneResult->asr_ssid = %s", __FUNCTION__, __LINE__, oneResult->asr_ssid);
//        DebugLog("--%s: AP %s ", __FUNCTION__,
//        ether_sprintf(oneResult->asr_bssid));
//
////        DebugLog("--%s: AP %s ", __FUNCTION__,
////                 ether_sprintf(na_node->nr_bssid));
////
////        if (memcmp(na_node->nr_bssid, empty_macaddr, IEEE80211_ADDR_LEN) == 0)
////        {
////            fScanResult = false;
////            return kIOReturnError;
////        }
//
//        *sr = oneResult;
//
//        fScanResult = false;
//        return kIOReturnSuccess;
//    }
//
//
//    return kIOReturnError;
    
//    int x = splnet();
////    DebugLog("---%s: line = %d", __FUNCTION__, __LINE__);
//
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//
//    if (ni == NULL) { // start at beginning if we're not in the middle
//        if (fScanResultWrapping) {
//            fScanResultWrapping = false;
//            splx(x);
//            return -1; // XXX no more results
//        } else {
//            ni = RBT_MIN(ieee80211_tree, &ic->ic_tree);
//            if (ni == NULL) { // start at beginning if we're not in the middle
//                splx(x);
//                return -1; // XXX no more results
//            }
//        }
//    }
//
////    DPRINTF(("Sending scan result for essid %s\n", ni->ni_essid));
//    apple80211_scan_result* oneResult = new apple80211_scan_result; // FIXME: huge memory leak here
//
//    oneResult->version = APPLE80211_VERSION;
//
//    oneResult->asr_channel.version = APPLE80211_VERSION;
//    oneResult->asr_channel.channel = ieee80211_chan2ieee(ic, ni->ni_chan);
//    oneResult->asr_channel.flags = chanspec2applechannel(ni->ni_chan->ic_flags);
//
//    oneResult->asr_unk = 8;
//    oneResult->asr_noise = 0;
//    oneResult->asr_snr = 8;
//    oneResult->asr_rssi = ni->ni_rssi;
//    oneResult->asr_beacon_int = ni->ni_intval;
//    oneResult->asr_cap = ni->ni_capinfo;
//    bcopy(ni->ni_bssid, oneResult->asr_bssid.octet, IEEE80211_ADDR_LEN);
//    oneResult->asr_nrates = ni->ni_rates.rs_nrates;
//    for (int r = 0; r < oneResult->asr_nrates; r++)
//        oneResult->asr_rates[r] = ni->ni_rates.rs_rates[r];
////    oneResult->asr_nr_unk = 8;
//    oneResult->asr_ssid_len = ni->ni_esslen;
//    bcopy(ni->ni_essid, oneResult->asr_ssid, ni->ni_esslen);
//
//    oneResult->unk = 8;
////    oneResult->unk2 = 8;
//
//
//    oneResult->asr_age = 0;
//    oneResult->asr_ie_len = 0;
//    if (ni->ni_rsnie != NULL) {
//        oneResult->asr_ie_len = ni->ni_rsnie[1];
//        oneResult->asr_ie_data = &ni->ni_rsnie[2];
//    }
//
////    kprintf("--%s: essid %s, ni->ni_capinfo = 0x%x, 0x%x", __FUNCTION__, ni->ni_essid, ni->ni_capinfo, ni->ni_rsncaps);
//
//
//    *sr = oneResult;
//
//    // done sending this one, now move to next for subsequent request
//    ni = RBT_NEXT(ieee80211_tree, ni);
//    if (ni == 0) // if there is no next one then wrap next time
//        fScanResultWrapping = true; // XXX next time signal that we got no more
//    splx(x);

    return kIOReturnSuccess;
}

//
// MARK: 12 - CARD_CAPABILITIES
//

IOReturn AirPortLinux::getCARD_CAPABILITIES(IO80211Interface *interface,
                                                 struct apple80211_capability_data *cd) {
    cd->version = APPLE80211_VERSION;
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//
//    uint32_t caps = 0;
//
//    if (ic->ic_caps & IEEE80211_C_WEP)        caps |= APPLE80211_CAP_WEP;
//    if (ic->ic_caps & IEEE80211_C_RSN)        caps |=  APPLE80211_CAP_WPA | APPLE80211_CAP_WPA1 | APPLE80211_CAP_WPA2 ; // TODO: add others
//    if (ic->ic_caps & IEEE80211_C_MONITOR)        caps |= APPLE80211_CAP_MONITOR;
//    if (ic->ic_caps & IEEE80211_C_SHSLOT)        caps |= APPLE80211_CAP_SHSLOT;
//    if (ic->ic_caps & IEEE80211_C_SHPREAMBLE)    caps |= APPLE80211_CAP_SHPREAMBLE;
//    //    if (ic->ic_caps & IEEE80211_C_AHDEMO)        caps |= APPLE80211_CAP_IBSS;
////    if (ic->ic_caps & IEEE80211_C_PMGT)        caps |= APPLE80211_CAP_PMGT;
////    if (ic->ic_caps & IEEE80211_C_TXPMGT)        caps |= APPLE80211_CAP_TXPMGT;
////    if (ic->ic_caps & IEEE80211_C_QOS)        caps |= APPLE80211_CAP_WME;
//
//    cd->capabilities[0] = (caps & 0xff);
//    cd->capabilities[1] = (caps & 0xff00) >> 8;
//    cd->capabilities[2] = (caps & 0xff0000) >> 16;
    

//    cd->capabilities[0] = APPLE80211_CAP_WEP;
//    cd->capabilities[1] = APPLE80211_CAP_IBSS;
//    cd->capabilities[2] = APPLE80211_CAP_MONITOR;
//    cd->capabilities[3] = APPLE80211_CAP_WPA;
    
    
//    cd->capabilities[0] |= 0xab;
//    cd->capabilities[1] |= 0x7e;
//    cd->capabilities[2] |= 0x3 | 0x13 | 0x20 | 0x28 | 0x4 | 0x80 | 0xc0;
//    cd->capabilities[3] |= 0x2 | 0x23;
//    cd->capabilities[4] |= 0x1;
//    cd->capabilities[5] |= 0x80;
//    cd->capabilities[6] |= 0x84;

    return kIOReturnSuccess;
}

//
// MARK: 13 - STATE
//

IOReturn AirPortLinux::getSTATE(IO80211Interface *interface,
                                     struct apple80211_state_data *ret) {

//    ret->version = APPLE80211_VERSION;
//
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    ret->state = ic->ic_state;
    return kIOReturnSuccess;
}

//IOReturn AirPortLinux::setSTATE(IO80211Interface *interface,
//                                     struct apple80211_state_data *sd) {
//    IOLog("Black82011: Setting state: %u", sd->state);
////    dev->setState(sd->state);
//    return kIOReturnSuccess;
//}

//
// MARK: 14 - PHY_MODE
//

IOReturn AirPortLinux::getPHY_MODE(IO80211Interface *interface,
                                        struct apple80211_phymode_data *pd) {
    pd->version = APPLE80211_VERSION;
//    pd->phy_mode = APPLE80211_MODE_11A
//                 | APPLE80211_MODE_11B
//                 | APPLE80211_MODE_11G;
//                 | APPLE80211_MODE_11N;
//                 | APPLE80211_MODE_11AC;
//    pd->active_phy_mode = APPLE80211_MODE_AUTO;
//
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    u_int32_t ic_modecaps= ic->ic_modecaps;
//    u_int32_t phy_mode  = APPLE80211_MODE_UNKNOWN;
//
//    if (ic_modecaps & 1<<IEEE80211_MODE_AUTO)       phy_mode |= APPLE80211_MODE_AUTO;
//    if (ic_modecaps & 1<<IEEE80211_MODE_11A)        phy_mode |= APPLE80211_MODE_11A;
//    if (ic_modecaps & 1<<IEEE80211_MODE_11B)        phy_mode |= APPLE80211_MODE_11B;
//    if (ic_modecaps & 1<<IEEE80211_MODE_11G)        phy_mode |= APPLE80211_MODE_11G;
//    if (ic_modecaps & 1<<IEEE80211_MODE_11N)        phy_mode |= APPLE80211_MODE_11N;
//    if (ic_modecaps & 1<<IEEE80211_MODE_11AC)       phy_mode |= APPLE80211_MODE_11AC;
//
//
//    u_int32_t ic_curmode= ic->ic_curmode;
//    u_int32_t active_phy_mode  = APPLE80211_MODE_UNKNOWN;
//
//    switch (ic_curmode) {
//        case IEEE80211_MODE_11A:
//            active_phy_mode = APPLE80211_MODE_11A;
//            break;
//        case IEEE80211_MODE_11B:
//            active_phy_mode = APPLE80211_MODE_11B;
//            break;
//        case IEEE80211_MODE_11G:
//            active_phy_mode = APPLE80211_MODE_11G;
//            break;
//        case IEEE80211_MODE_11N:
//            active_phy_mode = APPLE80211_MODE_11N;
//            break;
//        case IEEE80211_MODE_11AC:
//            active_phy_mode = APPLE80211_MODE_11AC;
//            break;
//        default:
//            active_phy_mode = APPLE80211_MODE_AUTO;
//            break;
//    }
//
//    pd->phy_mode = phy_mode;
//    pd->active_phy_mode = active_phy_mode;
    return kIOReturnSuccess;
}

//
// MARK: 15 - OP_MODE
//

IOReturn AirPortLinux::getOP_MODE(IO80211Interface *interface,
                                       struct apple80211_opmode_data *od) {
    od->version = APPLE80211_VERSION;
    od->op_mode = APPLE80211_M_STA;
    return kIOReturnSuccess;
}

//
// MARK: 16 - RSSI
//

IOReturn AirPortLinux::getRSSI(IO80211Interface *interface,
                                    struct apple80211_rssi_data *rd_data) {
    
//    struct rx_radiotap_header *tap =(struct rx_radiotap_header *)_ifp->tap;
//    if (tap == 0)
//        return kIOReturnError;
//
//    if (tap->wr_dbm_antsignal == 0) {
//        if (this->rd == NULL) {
//            return kIOReturnError;
//        } else {
//            bcopy(this->rd, rd_data, sizeof(struct apple80211_rssi_data));
//            return kIOReturnSuccess;
//        }
//    }
//
//    if (this->rd == NULL) {
//        this->rd = new struct apple80211_rssi_data;
//    }
//
//    this->rd->version = APPLE80211_VERSION;
//    this->rd->num_radios = 1;
//    this->rd->rssi_unit    = APPLE80211_UNIT_DBM;
//    this->rd->rssi[0]
//    = this->rd->aggregate_rssi
//    = this->rd->rssi_ext[0]
//    = this->rd->aggregate_rssi_ext
//    = tap->wr_dbm_antsignal;
//
//    bcopy(this->rd, rd_data, sizeof(struct apple80211_rssi_data));
//
    return kIOReturnSuccess;
}

//
// MARK: 17 - NOISE
//

IOReturn AirPortLinux::getNOISE(IO80211Interface *interface,
                                     struct apple80211_noise_data *nd_data) {
//
//    struct rx_radiotap_header *tap =(struct rx_radiotap_header *)_ifp->tap;
//    if (tap == 0)
//        return kIOReturnError;
//
//    if (tap->wr_dbm_antnoise == 0) {
//        if (this->nd == NULL) {
//            return kIOReturnError;
//        } else {
//            bcopy(this->nd, nd_data, sizeof(struct apple80211_noise_data));
//            return kIOReturnSuccess;
//        }
//    }
//
//    if (this->nd == NULL) {
//        this->nd = new struct apple80211_noise_data;
//    }
//
//    this->nd->version = APPLE80211_VERSION;
//    this->nd->num_radios = 1;
//    this->nd->noise[0] = tap->wr_dbm_antnoise;
//    this->nd->aggregate_noise = tap->wr_dbm_antnoise;
//    this->nd->noise_unit = APPLE80211_UNIT_DBM;
//
//    bcopy(this->nd, nd_data, sizeof(struct apple80211_noise_data));
    
    return kIOReturnSuccess;
}

//
// MARK: 18 - INT_MIT
//
IOReturn AirPortLinux::getINT_MIT(IO80211Interface* interface,
                                       struct apple80211_intmit_data* imd) {
    imd->version = APPLE80211_VERSION;
    imd->int_mit = APPLE80211_INT_MIT_AUTO;
    return kIOReturnSuccess;
}


//
// MARK: 19 - POWER
//

IOReturn AirPortLinux::getPOWER(IO80211Interface *interface,
                                     struct apple80211_power_data *ret) {

//    ret->version = APPLE80211_VERSION;
//    ret->num_radios = 1;
//    ret->power_state[0] = _ifp->if_power_state;

    return kIOReturnSuccess;
}

IOReturn AirPortLinux::setPOWER(IO80211Interface *interface,
                                     struct apple80211_power_data *pd) {

//    if (pd->num_radios > 0) {
//        _ifp->if_power_state = pd->power_state[0];
//    }
//
//    switch (_ifp->if_power_state) {
//        case APPLE80211_POWER_ON:
//            DPRINTF(("Setting power on\n"));
//            _ifp->if_ioctl(_ifp, SIOCSIFADDR, NULL);
//            if (_ifp->iface)
//                _ifp->iface->postMessage(APPLE80211_M_POWER_CHANGED);
////            if (fOutputQueue)
////                fOutputQueue->setCapacity(200); // FIXME
//            this->fWatchdogTimer->setTimeoutMS(kTimeoutMS);
//            return kIOReturnSuccess;
//
//        case APPLE80211_POWER_OFF:
//            DPRINTF(("Setting power off\n"));
//            this->fWatchdogTimer->cancelTimeout();
//            _ifp->if_flags &= ~IFF_UP;
//            _ifp->if_ioctl(_ifp, SIOCSIFFLAGS, NULL);
//            if (_ifp->iface)
//                _ifp->iface->postMessage(APPLE80211_M_POWER_CHANGED);
////            if (fOutputQueue) {
////                fOutputQueue->stop();
////                fOutputQueue->flush();
////                fOutputQueue->setCapacity(0);
////            }
//            return kIOReturnSuccess;
//
//        default:
//            return kIOReturnError;
//    };

    return kIOReturnSuccess;
}

//
// MARK: 20 - ASSOCIATE
//

IOReturn AirPortLinux::setASSOCIATE(IO80211Interface *interface,
                                         struct apple80211_assoc_data *ad) {
    
//    kprintf("%s: line = %d ad->ad_auth_upper = %d, ad->ad_ssid = %s, ad->ad_key = %s", __FUNCTION__, __LINE__, ad->ad_auth_upper, ad->ad_ssid, ad->ad_key.key);
//
//    kprintf("%s: line = %d ad->ad_auth_upper = %d", __FUNCTION__, __LINE__, ad->ad_auth_upper);
//
//    kprintf("%s: line = %d ad->ad_ssid = %s", __FUNCTION__, __LINE__, ad->ad_ssid);
//
//    kprintf("%s: line = %d ad->ad_key = %s", __FUNCTION__, __LINE__, ad->ad_key.key);
//
//    kprintf("%s: line = %d ad->key_len = %d", __FUNCTION__, __LINE__, ad->ad_key.key_len);
//
//    char key[32];
//    bcopy(ad->ad_key.key, key, ad->ad_key.key_len);
//    printf("--%s: line = %d key = %s", __FUNCTION__, __LINE__, key);
//
//
//    printf("\n%s:line = %d ad->ad_key.key = \n", __FUNCTION__, __LINE__);
//    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n",ad->ad_key.key[0], ad->ad_key.key[1], ad->ad_key.key[2], ad->ad_key.key[3], ad->ad_key.key[4],ad->ad_key.key[5], ad->ad_key.key[6], ad->ad_key.key[7]);
//    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", ad->ad_key.key[8], ad->ad_key.key[9],ad->ad_key.key[10], ad->ad_key.key[11], ad->ad_key.key[12], ad->ad_key.key[13], ad->ad_key.key[14],ad->ad_key.key[15]);
//    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n", ad->ad_key.key[16], ad->ad_key.key[17], ad->ad_key.key[18], ad->ad_key.key[19],ad->ad_key.key[20], ad->ad_key.key[21], ad->ad_key.key[22], ad->ad_key.key[23]);
//    printf("%02x %02x %02x %02x %02x %02x %02x %02x \n\n", ad->ad_key.key[24],ad->ad_key.key[25], ad->ad_key.key[26], ad->ad_key.key[27], ad->ad_key.key[28], ad->ad_key.key[29], ad->ad_key.key[30], ad->ad_key.key[31]);
//
//printf("--%s: line = %d", __FUNCTION__, __LINE__);
//
////    const char *configStr = "nwid KIMAX wpakey 126abc!@ABC wpaprotos wpa1,wpa2";
//    char configStr[32];
//    snprintf((char*)configStr, sizeof(configStr), "nwid %s wpakey 126abc!@ABC wpaprotos wpa1,wpa2", ad->ad_ssid);
//
////    ifconfig(configStr);
//
//    if (ad->ad_ssid_len > IEEE80211_NWID_LEN)
//        return kIOReturnInvalid;
    
//    ifnet *ifp = &if_softc.sc_ic.ic_if;
//    ifp->iface->postMessage(APPLE80211_M_BSSID_CHANGED);
    
//    BSSID = OSString::withCString((const char *)ad->ad_ssid);
//    PWD = OSString::withCString((const char *)ad->ad_key.key);
//
//    kprintf("%s: line = %d BSSID = %s, PWD = %s", __FUNCTION__, __LINE__, BSSID->getCStringNoCopy(), PWD->getCStringNoCopy());
//
//    kprintf("%s: line = %d", __FUNCTION__, __LINE__);
//
//    ifconfig();
//
////    ifp->iface->postMessage(APPLE80211_M_ASSOC_DONE);
//    bzero(ad, sizeof(*ad));

    return kIOReturnSuccess;
}

//
IOReturn AirPortLinux::getASSOCIATE_RESULT(IO80211Interface* interface,struct apple80211_assoc_result_data* ard)
{
//    printf("--%s: line = %d", __FUNCTION__, __LINE__);
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == 0)
//        return kIOReturnError;
//
//    ard->version = APPLE80211_VERSION;
//    ard->result = APPLE80211_RESULT_UNAVAILABLE;
//    if (ic->ic_state & IEEE80211_S_ASSOC)
//    {
//        ard->result = APPLE80211_RESULT_SUCCESS;
//    }
    return kIOReturnSuccess;
}


//
// MARK: 22 - DISASSOCIATE
//

IOReturn AirPortLinux::setDISASSOCIATE(IO80211Interface* interface)
{
    printf("--%s: line = %d", __FUNCTION__, __LINE__);
//    ifnet *ifp = &if_softc.sc_ic.ic_if;
//    ifp->if_ioctl(ifp, SIOCSIFADDR, NULL);
    return kIOReturnSuccess;
}

//
// MARK: 23 - STATUS_DEV_NAME
//

//IOReturn AirPortLinux::getSTATUS_DEV_NAME(IO80211Interface *interface,
//                                       struct apple80211_status_dev_data *hv) {
////    hv->version = APPLE80211_VERSION;
////    sprintf((char*)hv->dev_name,"%s%d" , _ifp->iface->getNamePrefix(), _ifp->iface->getUnitNumber());
////    _ifp->iface->setEnabledBySystem(true);
////    _ifp->iface->setPoweredOnByUser(true);
//    return kIOReturnSuccess;
//}

//
// MARK: 27 - SUPPORTED_CHANNELS
//

IOReturn AirPortLinux::getSUPPORTED_CHANNELS(IO80211Interface *interface,
                                                  struct apple80211_sup_channel_data *ad) {
    ad->version = APPLE80211_VERSION;
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    struct ieee80211_channel *c;
//
//    ad->num_channels = 0;
//    for (int i = 0; i <= IEEE80211_CHAN_MAX; i++) {
//        c = &ic->ic_channels[i];
//        if (c->ic_flags) {
//            /*
//             * Verify driver passed us valid data.
//             */
//            if (i != ieee80211_chan2ieee(ic, c)) {
//                c->ic_flags = 0;    /* NB: remove */
//                continue;
//            }
//
//            ad->supported_channels[ad->num_channels].version = APPLE80211_VERSION;
//            ad->supported_channels[ad->num_channels].channel = i;
//            ad->supported_channels[ad->num_channels].flags = chanspec2applechannel(c->ic_flags);
//            ad->num_channels++;
//        }
//    }

    return kIOReturnSuccess;
}

//
// MARK: 28 - LOCALE
//

IOReturn AirPortLinux::getLOCALE(IO80211Interface *interface,
                                      struct apple80211_locale_data *ld) {
    ld->version = APPLE80211_VERSION;
    ld->locale  = APPLE80211_LOCALE_ROW;

    return kIOReturnSuccess;
}

//
// MARK: 29 - DEAUTH
//

IOReturn AirPortLinux::getDEAUTH(IO80211Interface *interface,
                                   struct apple80211_deauth_data *ret) {
    
//    printf("--%s: line = %d", __FUNCTION__, __LINE__);
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    ret->deauth_reason = APPLE80211_REASON_UNSPECIFIED; // FIXME
    return kIOReturnSuccess;
}

//
// MARK: 31 - FRAG_THRESHOLD
//

//IOReturn AirPortLinux::getFRAG_THRESHOLD(IO80211Interface *interface,
//                                       apple80211_frag_threshold_data *ret) {
//
////    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
////    ret->version = APPLE80211_VERSION;
////    ret->threshold = ic->ic_fragthreshold;
//    return kIOReturnSuccess;
//}


//
// MARK: 32 - RATE_SET
//

IOReturn AirPortLinux::getRATE_SET(IO80211Interface *interface, struct apple80211_rate_set_data *dd) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == 0)
//        return kIOReturnError;
//
//    dd->version = APPLE80211_VERSION;
//    dd->num_rates = ic->ic_bss->ni_rates.rs_nrates;
//    for (int i=0; i<APPLE80211_MAX_RATES; i++)
//    {
//        dd->rates[i].rate = ic->ic_bss->ni_rates.rs_rates[i];
//        dd->rates[i].flags = APPLE80211_RATE_FLAG_NONE;
//    }

    return kIOReturnSuccess;
}

//
// MARK: 37 - TX_ANTENNA
//
IOReturn AirPortLinux::getTX_ANTENNA(IO80211Interface *interface,
                                          apple80211_antenna_data *ad) {
    ad->version = APPLE80211_VERSION;
    ad->num_radios = 1;
    ad->antenna_index[0] = 1;
    return kIOReturnSuccess;
}

//
// MARK: 39 - ANTENNA_DIVERSITY
//

IOReturn AirPortLinux::getANTENNA_DIVERSITY(IO80211Interface *interface,
                                                 apple80211_antenna_data *ad) {
    ad->version = APPLE80211_VERSION;
    ad->num_radios = 1;
    ad->antenna_index[0] = 1;
    return kIOReturnSuccess;
}

//
// MARK: 40 - ROM
//

//IOReturn AirPortLinux::getROM(IO80211Interface* interface, apple80211_rom_data* rd) {
//    rd->version = APPLE80211_VERSION;
//    rd->rom_len = 1;
//    rd->rom[0] = 0.1;
//    return kIOReturnSuccess;
//}

//
// MARK: 42 - STATION_LIST
//

//IOReturn AirPortLinux::getSTATION_LIST(IO80211Interface *interface, apple80211_sta_data *sd)
//{
//    int i;
//    IOLog("Feeding a list of stations to the driver...\n");
//    sd->num_stations = 4;
//
//    for(i=0; i < 4; i++) {
//        struct apple80211_station *sta = &(sd->station_list[i]);
//
//        sta->version = APPLE80211_VERSION;
//
//        memset(&(sta->sta_mac), i, sizeof(sta->sta_mac));
//        sta->sta_rssi = 1;
//    }
//
//    return kIOReturnSuccess;
//}

//
// MARK: 43 - DRIVER_VERSION
//

IOReturn AirPortLinux::getDRIVER_VERSION(IO80211Interface *interface,
                                              struct apple80211_version_data *hv) {
    hv->version = APPLE80211_VERSION;
    strncpy(hv->string, "Driver 1.0", sizeof(hv->string));
    hv->string_len = strlen("Driver 1.0");
    return kIOReturnSuccess;
}

//
// MARK: 44 - HARDWARE_VERSION
//

IOReturn AirPortLinux::getHARDWARE_VERSION(IO80211Interface *interface,
                                                struct apple80211_version_data *hv) {
    hv->version = APPLE80211_VERSION;
//    strncpy(hv->string, if_softc.sc_fwver, sizeof(if_softc.sc_fwver));
//    hv->string_len = strlen(if_softc.sc_fwver);
    return kIOReturnSuccess;
}

//
// MARK: 46 - RSN_IE
//
IOReturn AirPortLinux::getRSN_IE(IO80211Interface *interface, struct apple80211_rsn_ie_data *ret) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == NULL)
//        return kIOReturnError;
//    ret->len = ic->ic_bss->ni_rsnie[1]; // XXX
//    bcopy(ic->ic_bss->ni_rsnie + 2, ret->ie, ret->len);
    return kIOReturnSuccess;
}

//
// MARK: 50 - ASSOCIATION_STATUS
//

IOReturn AirPortLinux::getASSOCIATION_STATUS(IO80211Interface* interface, struct apple80211_assoc_status_data* ret) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic == NULL)
        return kIOReturnError;
//
//    if (ic->ic_state == IEEE80211_S_RUN) {
//        ret->status = APPLE80211_STATUS_SUCCESS;
//        return kIOReturnSuccess;
//    } else if (ic->ic_state == IEEE80211_S_AUTH || ic->ic_state == IEEE80211_S_ASSOC) {
//        ret->status = APPLE80211_STATUS_UNAVAILABLE;
//        return kIOReturnBusy;
//    } else {
//        ret->status = APPLE80211_STATUS_UNSPECIFIED_FAILURE; // TODO: use reason code
//        return kIOReturnSuccess;
//    }
}

//
// MARK: 51 - COUNTRY_CODE
//

IOReturn AirPortLinux::getCOUNTRY_CODE(IO80211Interface *interface,
                                            struct apple80211_country_code_data *cd) {
    cd->version = APPLE80211_VERSION;
//    strncpy((char*) cd->cc, if_softc.sc_fw_mcc, sizeof(if_softc.sc_fw_mcc)); // FIXME
    strncpy((char*)cd->cc, "IN", sizeof(cd->cc));
    return kIOReturnSuccess;
}

//
// MARK: 53 - LAST_RX_PKT_DATA
//

#define toMbps(x)        (((x) & 0x7f) / 2)

//IOReturn AirPortLinux::getLAST_RX_PKT_DATA(IO80211Interface *interface,
//                                         struct apple80211_last_rx_pkt_data *ret) {
//
////    struct rx_radiotap_header *tap =(struct rx_radiotap_header *)_ifp->tap;
////    if (tap == 0)
////        return kIOReturnError;
////
////    if (tap->wr_dbm_antsignal == 0)
////        return kIOReturnError;
////
////    ret->rate = toMbps(tap->wr_rate);
////    ret->rssi = tap->wr_dbm_antsignal;//ic->ic_bss->ni_rssi;
////    ret->num_streams = 1; // only changes for 11n mode
////    // TODO: we should ideally fill in ret->sa too, but ignoring for now
//    return kIOReturnSuccess;
//}

//
// MARK: 54 - ADIO_INFO
//

IOReturn AirPortLinux::getRADIO_INFO(IO80211Interface* interface, struct apple80211_radio_info_data* md)
{
    md->version = 1;
    md->count = 1;
    return kIOReturnSuccess;
}

//
// MARK: 57 - MCS
//
IOReturn AirPortLinux::getMCS(IO80211Interface* interface, struct apple80211_mcs_data* ret) {
    
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == NULL)
//        return kIOReturnError;
//
//    ret->version = APPLE80211_VERSION;
//    ret->index = ic->ic_bss->ni_txmcs;
    return kIOReturnSuccess;
}

//
// MARK: 66 - MCS_INDEX_SET
//
IOReturn AirPortLinux::getMCS_INDEX_SET(IO80211Interface* interface, struct apple80211_mcs_index_set_data* md)
{
//    struct ieee80211com *ic = (struct ieee80211com *)_ifp;
//    if (ic->ic_bss == NULL)
//        return kIOReturnError;
//    md->version = APPLE80211_VERSION;
//    int offset;
//    for(offset=0; offset<sizeof(md->mcs_set_map); offset++) {
//        md->mcs_set_map[offset] = ic->ic_sup_mcs[offset];
//    }

    return kIOReturnSuccess;
}


//
// MARK: 80 - ROAM_THRESH
//

IOReturn AirPortLinux::getROAM_THRESH(IO80211Interface* interface, struct apple80211_roam_threshold_data* rtd) {
    
    rtd->count = 0;
    rtd->threshold = 1000;
    return kIOReturnSuccess;
}


//
// MARK: 112 - FACTORY_MODE
//

//IOReturn AirPortLinux::getFACTORY_MODE(IO80211Interface* interface, struct apple80211_factory_mode_data* fmd) {
//
//    fmd->version = 1;
//    fmd->mode1 = 1;
//    fmd->mode2 = APPLE80211_LOCALE_ROW;
//    fmd->mode3 = 1;
//    return kIOReturnSuccess;
//}







IOReturn AirPortLinux::apple80211Request(UInt32 request_type,
                                            int request_number,
                                            IO80211Interface* interface,
                                            void* data) {
    if (request_type != SIOCGA80211 && request_type != SIOCSA80211) {
        IOLog("AirPortLinux: Invalid IOCTL request type: %u", request_type);
        return kIOReturnError;
    }
    
    IOReturn ret = 0;
    
    bool isGet = (request_type == SIOCGA80211);
    
#define IOCTL(REQ_TYPE, REQ, DATA_TYPE) \
if (REQ_TYPE == SIOCGA80211) { \
ret = get##REQ(interface, (struct DATA_TYPE* )data); \
} else { \
ret = set##REQ(interface, (struct DATA_TYPE* )data); \
}
    
#define IOCTL_GET(REQ_TYPE, REQ, DATA_TYPE) \
if (REQ_TYPE == SIOCGA80211) { \
ret = get##REQ(interface, (struct DATA_TYPE* )data); \
}
#define IOCTL_SET(REQ_TYPE, REQ, DATA_TYPE) \
if (REQ_TYPE == SIOCSA80211) { \
ret = set##REQ(interface, (struct DATA_TYPE* )data); \
}
    
//    IOLog("--%s: IOCTL %s(%d)", __FUNCTION__,
//          isGet ? "get" : "set",
//          request_number);
//    IOLog("%s: IOCTL %s(%d) %s", __FUNCTION__,
//          isGet ? "get" : "set",
//          request_number,
//          IOCTL_NAMES[request_number]);
    
    switch (request_number) {
        case APPLE80211_IOC_SSID: // 1
            IOCTL_GET(request_type, SSID, apple80211_ssid_data);
            break;
        case APPLE80211_IOC_AUTH_TYPE: // 2
            IOCTL_GET(request_type, AUTH_TYPE, apple80211_authtype_data);
            break;
        case APPLE80211_IOC_CIPHER_KEY: // 3
            IOCTL_SET(request_type, CIPHER_KEY, apple80211_key);
            break;
        case APPLE80211_IOC_CHANNEL: // 4
            IOCTL_GET(request_type, CHANNEL, apple80211_channel_data);
            break;
        case APPLE80211_IOC_POWERSAVE: // 5
            IOCTL_GET(request_type, POWERSAVE, apple80211_powersave_data);
            break;
        case APPLE80211_IOC_PROTMODE: // 6
            IOCTL_GET(request_type, PROTMODE, apple80211_protmode_data);
            break;
        case APPLE80211_IOC_TXPOWER: // 7
            IOCTL_GET(request_type, TXPOWER, apple80211_txpower_data);
            break;
        case APPLE80211_IOC_RATE: // 8
            IOCTL_GET(request_type, RATE, apple80211_rate_data);
            break;
        case APPLE80211_IOC_BSSID: // 9
            IOCTL_GET(request_type, BSSID, apple80211_bssid_data);
            break;
        case APPLE80211_IOC_SCAN_REQ: // 10
            IOCTL_SET(request_type, SCAN_REQ, apple80211_scan_data);
            break;
        case APPLE80211_IOC_SCAN_RESULT: // 11
            IOCTL_GET(request_type, SCAN_RESULT, apple80211_scan_result*);
            break;
        case APPLE80211_IOC_CARD_CAPABILITIES: // 12
            IOCTL_GET(request_type, CARD_CAPABILITIES, apple80211_capability_data);
            break;
        case APPLE80211_IOC_STATE: // 13
            IOCTL_GET(request_type, STATE, apple80211_state_data);
            break;
        case APPLE80211_IOC_PHY_MODE: // 14
            IOCTL_GET(request_type, PHY_MODE, apple80211_phymode_data);
            break;
        case APPLE80211_IOC_OP_MODE: // 15
            IOCTL_GET(request_type, OP_MODE, apple80211_opmode_data);
            break;
        case APPLE80211_IOC_RSSI: // 16
            IOCTL_GET(request_type, RSSI, apple80211_rssi_data);
            break;
        case APPLE80211_IOC_NOISE: // 17
            IOCTL_GET(request_type, NOISE, apple80211_noise_data);
            break;
        case APPLE80211_IOC_INT_MIT: // 18
            IOCTL_GET(request_type, INT_MIT, apple80211_intmit_data);
            break;
        case APPLE80211_IOC_POWER: // 19
            IOCTL(request_type, POWER, apple80211_power_data);
            break;
        case APPLE80211_IOC_ASSOCIATE: // 20
            IOCTL_SET(request_type, ASSOCIATE, apple80211_assoc_data);
            break;
        case APPLE80211_IOC_ASSOCIATE_RESULT: // 21
            IOCTL_GET(request_type, ASSOCIATE_RESULT, apple80211_assoc_result_data);
            break;
        case APPLE80211_IOC_DISASSOCIATE: // 22
            setDISASSOCIATE(interface);
            break;
//        case APPLE80211_IOC_STATUS_DEV_NAME: // 23
//            IOCTL_GET(request_type, STATUS_DEV_NAME, apple80211_status_dev_data);
//            break;
        case APPLE80211_IOC_SUPPORTED_CHANNELS: // 27
            IOCTL_GET(request_type, SUPPORTED_CHANNELS, apple80211_sup_channel_data);
            break;
        case APPLE80211_IOC_LOCALE: // 28
            IOCTL_GET(request_type, LOCALE, apple80211_locale_data);
            break;
        case APPLE80211_IOC_DEAUTH: // 29
            IOCTL_GET(request_type, DEAUTH, apple80211_deauth_data);
            break;
//        case APPLE80211_IOC_FRAG_THRESHOLD: // 31
//            IOCTL_GET(request_type, FRAG_THRESHOLD, apple80211_frag_threshold_data);
//            break;
        case APPLE80211_IOC_RATE_SET: // 32
            IOCTL_GET(request_type, RATE_SET, apple80211_rate_set_data);
            break;
        case APPLE80211_IOC_TX_ANTENNA: // 37
            IOCTL_GET(request_type, TX_ANTENNA, apple80211_antenna_data);
            break;
        case APPLE80211_IOC_ANTENNA_DIVERSITY: // 39
            IOCTL_GET(request_type, ANTENNA_DIVERSITY, apple80211_antenna_data);
            break;
//        case APPLE80211_IOC_ROM:
//            IOCTL_GET(request_type, ROM, apple80211_rom_data);
//            break;
        case APPLE80211_IOC_DRIVER_VERSION: // 43
            IOCTL_GET(request_type, DRIVER_VERSION, apple80211_version_data);
            break;
        case APPLE80211_IOC_HARDWARE_VERSION: // 44
            IOCTL_GET(request_type, HARDWARE_VERSION, apple80211_version_data);
            break;
        case APPLE80211_IOC_RSN_IE: // 46
            IOCTL_GET(request_type, RSN_IE, apple80211_rsn_ie_data);
            break;
        case APPLE80211_IOC_ASSOCIATION_STATUS: // 50
            IOCTL_GET(request_type, ASSOCIATION_STATUS, apple80211_assoc_status_data);
            break;
        case APPLE80211_IOC_COUNTRY_CODE: // 51
            IOCTL_GET(request_type, COUNTRY_CODE, apple80211_country_code_data);
            break;
//        case APPLE80211_IOC_LAST_RX_PKT_DATA: // 53
//            IOCTL_GET(request_type, LAST_RX_PKT_DATA, apple80211_last_rx_pkt_data);
//            break;
        case APPLE80211_IOC_RADIO_INFO: // 54
            IOCTL_GET(request_type, RADIO_INFO, apple80211_radio_info_data);
            break;
        case APPLE80211_IOC_MCS: // 57
            IOCTL_GET(request_type, MCS, apple80211_mcs_data);
            break;
        case APPLE80211_IOC_MCS_INDEX_SET: // 66
            IOCTL_GET(request_type, MCS_INDEX_SET, apple80211_mcs_index_set_data);
            break;
//        case APPLE80211_IOC_WOW_PARAMETERS: // 69
//            break;
        case APPLE80211_IOC_ROAM_THRESH: // 80
            IOCTL_GET(request_type, ROAM_THRESH, apple80211_roam_threshold_data);
            break;
////        case APPLE80211_IOC_TX_CHAIN_POWER: // 108
////            break;
////        case APPLE80211_IOC_THERMAL_THROTTLING: // 111
////            break;
//        case APPLE80211_IOC_FACTORY_MODE: // 112
//            IOCTL_GET(request_type, FACTORY_MODE, apple80211_factory_mode_data);
//            break;
        default:
//            DPRINTF(("Unhandled Airport GET request %u\n", request_number));
            return kIOReturnUnsupported;
    }
#undef IOCTL
    
    return ret;
}

bool AirPortLinux::addMediumType(UInt32 type, UInt32 speed, UInt32 code, char* name) {
    bool ret = false;
    
    IONetworkMedium* medium = IONetworkMedium::medium(type, speed, 0, code, name);
    if (medium) {
        ret = IONetworkMedium::addMedium(mediumDict, medium);
        if (ret)
            mediumTable[code] = medium;
        medium->release();
    }
    return ret;
}

//IOReturn AirPortLinux::setSTA_WPA_Key(apple80211_key* key)
//{
//    kprintf("%s: line = %d", __FUNCTION__, __LINE__);
//    kprintf("%s: line = %d", __FUNCTION__, __LINE__);
//    kprintf("%s: line = %d", __FUNCTION__, __LINE__);
//    kprintf("%s: line = %d key = %s, key_cipher_type = %u, key_rsc = %s", __FUNCTION__, __LINE__, key->key, key->key_cipher_type, key->key_rsc);
//
//    return kIOReturnSuccess;
//}
