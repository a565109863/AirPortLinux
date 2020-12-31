//
//  AirPortLinux_80211.cpp
//  AirPortLinux
//
//  Created by Mac-PC on 2020/7/17.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "AirPortLinux.hpp"

//extern struct ifnet *_ifp;


int AirPortLinux::bpfOutputPacket(IOInterface* interface, UInt dltType, mbuf_t m)
{
//    DebugLog("--%s: 80211 line = %d", __FUNCTION__, __LINE__);
    if (dltType == DLT_IEEE802_11_RADIO || dltType == DLT_IEEE802_11) {
        return bpfOutput80211Radio(interface, m);
    }
    if (dltType == DLT_RAW) {
        return outputActionFrame(interface, m);
    }
    mbuf_freem(m);
    return 1;
}

int AirPortLinux::outputRaw80211Packet(IOInterface* interface, mbuf_t m)
{
//    DebugLog("--%s: 80211 line = %d", __FUNCTION__, __LINE__);
    mbuf_freem(m);
    return kIOReturnOutputDropped;
}

int AirPortLinux::outputActionFrame(IOInterface *interface, mbuf_t m)
{
//    DebugLog("--%s: 80211 line = %d", __FUNCTION__, __LINE__);
    mbuf_freem(m);
    return kIOReturnOutputDropped;
}

int AirPortLinux::bpfOutput80211Radio(IOInterface *interface, mbuf_t m)
{
//    DebugLog("--%s: 80211 line = %d", __FUNCTION__, __LINE__);
    mbuf_freem(m);
    return 0;
}

bool AirPortLinux::useAppleRSNSupplicant(IOInterface *interface)
{
    return false;
}

//bool AirPortLinux::useAppleRSNSupplicant(IO80211VirtualInterface *interface)
//{
//    return false;
//}
