//
//  if_vlan.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/7/5.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef if_vlan_h
#define if_vlan_h

#include <linux/types.h>
#include <linux/if_ether.h>

#define VLAN_HLEN    4        /* The additional bytes required by VLAN
                     * (in addition to the Ethernet header)
                     */
#define VLAN_ETH_HLEN    18        /* Total octets in header.     */
#define VLAN_ETH_ZLEN    64        /* Min. octets in frame sans FCS */

/*
 * According to 802.3ac, the packet can be 4 bytes longer. --Klika Jan
 */
#define VLAN_ETH_DATA_LEN    1500    /* Max. octets in payload     */
#define VLAN_ETH_FRAME_LEN    1518    /* Max. octets in frame sans FCS */

/*
 *     struct vlan_hdr - vlan header
 *     @h_vlan_TCI: priority and VLAN ID
 *    @h_vlan_encapsulated_proto: packet type ID or len
 */
struct vlan_hdr {
    __be16    h_vlan_TCI;
    __be16    h_vlan_encapsulated_proto;
};

/**
 *    struct vlan_ethhdr - vlan ethernet header (ethhdr + vlan_hdr)
 *    @h_dest: destination ethernet address
 *    @h_source: source ethernet address
 *    @h_vlan_proto: ethernet protocol
 *    @h_vlan_TCI: priority and VLAN ID
 *    @h_vlan_encapsulated_proto: packet type ID or len
 */
struct vlan_ethhdr {
    unsigned char    h_dest[ETH_ALEN];
    unsigned char    h_source[ETH_ALEN];
    __be16        h_vlan_proto;
    __be16        h_vlan_TCI;
    __be16        h_vlan_encapsulated_proto;
};

#endif /* if_vlan_h */
