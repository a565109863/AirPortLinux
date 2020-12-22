//
//  eth.c
//  AirPortLinux
//
//  Created by Zhong-MAC on 2020/12/22.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#include <linux/etherdevice.h>


/**
 * eth_prepare_mac_addr_change - prepare for mac change
 * @dev: network device
 * @p: socket address
 */
int eth_prepare_mac_addr_change(struct net_device *dev, void *p)
{
    struct sockaddr *addr = (struct sockaddr *)p;

    if (!(dev->priv_flags & IFF_LIVE_ADDR_CHANGE) && netif_running(dev))
        return -EBUSY;
    if (!is_valid_ether_addr((const u8 *)addr->sa_data))
        return -EADDRNOTAVAIL;
    return 0;
}
EXPORT_SYMBOL(eth_prepare_mac_addr_change);


/**
 * eth_commit_mac_addr_change - commit mac change
 * @dev: network device
 * @p: socket address
 */
void eth_commit_mac_addr_change(struct net_device *dev, void *p)
{
    struct sockaddr *addr = (struct sockaddr *)p;

    memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
}
EXPORT_SYMBOL(eth_commit_mac_addr_change);

/**
 * eth_mac_addr - set new Ethernet hardware address
 * @dev: network device
 * @p: socket address
 *
 * Change hardware address of device.
 *
 * This doesn't change hardware matching, so needs to be overridden
 * for most real devices.
 */
int eth_mac_addr(struct net_device *dev, void *p)
{
    int ret;

    ret = eth_prepare_mac_addr_change(dev, p);
    if (ret < 0)
        return ret;
    eth_commit_mac_addr_change(dev, p);
    return 0;
}
EXPORT_SYMBOL(eth_mac_addr);


const struct header_ops eth_header_ops ____cacheline_aligned = {
    .create        = eth_header,
    .parse        = eth_header_parse,
    .cache        = eth_header_cache,
    .cache_update    = eth_header_cache_update,
    .parse_protocol    = eth_header_parse_protocol,
};

/**
 * ether_setup - setup Ethernet network device
 * @dev: network device
 *
 * Fill in the fields of the device structure with Ethernet-generic values.
 */
void ether_setup(struct net_device *dev)
{
    dev->header_ops        = &eth_header_ops;
    dev->type        = ARPHRD_ETHER;
    dev->hard_header_len     = ETH_HLEN;
    dev->min_header_len    = ETH_HLEN;
    dev->mtu        = ETH_DATA_LEN;
    dev->min_mtu        = ETH_MIN_MTU;
    dev->max_mtu        = ETH_DATA_LEN;
    dev->addr_len        = ETH_ALEN;
    dev->tx_queue_len    = DEFAULT_TX_QUEUE_LEN;
    dev->flags        = IFF_BROADCAST|IFF_MULTICAST;
    dev->priv_flags        |= IFF_TX_SKB_SHARING;
    
    eth_broadcast_addr(dev->broadcast);
    
}
EXPORT_SYMBOL(ether_setup);


/**
 * eth_header - create the Ethernet header
 * @skb:    buffer to alter
 * @dev:    source device
 * @type:    Ethernet type field
 * @daddr: destination address (NULL leave destination address)
 * @saddr: source address (NULL use device source address)
 * @len:   packet length (<= skb->len)
 *
 *
 * Set the protocol type. For a packet of type ETH_P_802_3/2 we put the length
 * in here instead.
 */
int eth_header(struct sk_buff *skb, struct net_device *dev,
               unsigned short type,
               const void *daddr, const void *saddr, unsigned int len)
{
    struct ethhdr *eth = (struct ethhdr *)skb_push(skb, ETH_HLEN);
    
    if (type != ETH_P_802_3 && type != ETH_P_802_2)
        eth->h_proto = htons(type);
    else
        eth->h_proto = htons(len);
    
    /*
     *      Set the source hardware address.
     */
    
    if (!saddr)
        saddr = dev->dev_addr;
    memcpy(eth->h_source, saddr, ETH_ALEN);
    
    if (daddr) {
        memcpy(eth->h_dest, daddr, ETH_ALEN);
        return ETH_HLEN;
    }
    
    /*
     *      Anyway, the loopback-device should never use this function...
     */
    
    if (dev->flags & (IFF_LOOPBACK | IFF_NOARP)) {
        eth_zero_addr(eth->h_dest);
        return ETH_HLEN;
    }
    
    return -ETH_HLEN;
}
EXPORT_SYMBOL(eth_header);


/**
 * eth_header_parse - extract hardware address from packet
 * @skb: packet to extract header from
 * @haddr: destination buffer
 */
int eth_header_parse(const struct sk_buff *skb, unsigned char *haddr)
{
    const struct ethhdr *eth = (const struct ethhdr *)eth_hdr(skb);
    memcpy(haddr, eth->h_source, ETH_ALEN);
    return ETH_ALEN;
}
EXPORT_SYMBOL(eth_header_parse);

/**
 * eth_header_cache - fill cache entry from neighbour
 * @neigh: source neighbour
 * @hh: destination cache entry
 * @type: Ethernet type field
 *
 * Create an Ethernet header template from the neighbour.
 */
int eth_header_cache(const struct neighbour *neigh, struct hh_cache *hh, __be16 type)
{
    struct ethhdr *eth;
    const struct net_device *dev = neigh->dev;
    
    eth = (struct ethhdr *)
    (((u8 *) hh->hh_data) + (HH_DATA_OFF(sizeof(*eth))));
    
    if (type == htons(ETH_P_802_3))
        return -1;
    
    eth->h_proto = type;
    memcpy(eth->h_source, dev->dev_addr, ETH_ALEN);
    memcpy(eth->h_dest, neigh->ha, ETH_ALEN);
    
    /* Pairs with READ_ONCE() in neigh_resolve_output(),
     * neigh_hh_output() and neigh_update_hhs().
     */
//    smp_store_release(&hh->hh_len, ETH_HLEN);
    
    return 0;
}
EXPORT_SYMBOL(eth_header_cache);

/**
 * eth_header_cache_update - update cache entry
 * @hh: destination cache entry
 * @dev: network device
 * @haddr: new hardware address
 *
 * Called by Address Resolution module to notify changes in address.
 */
void eth_header_cache_update(struct hh_cache *hh,
                             const struct net_device *dev,
                             const unsigned char *haddr)
{
    memcpy(((u8 *) hh->hh_data) + HH_DATA_OFF(sizeof(struct ethhdr)),
           haddr, ETH_ALEN);
}
EXPORT_SYMBOL(eth_header_cache_update);

/**
 * eth_header_parser_protocol - extract protocol from L2 header
 * @skb: packet to extract protocol from
 */
__be16 eth_header_parse_protocol(const struct sk_buff *skb)
{
    const struct ethhdr *eth = eth_hdr(skb);
    
    return eth->h_proto;
}
EXPORT_SYMBOL(eth_header_parse_protocol);

