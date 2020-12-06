//
//  skbuff.c
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/27.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include "skbuff.h"


struct sk_buff *__netdev_alloc_skb(struct net_device *dev, unsigned int len,
                   gfp_t gfp_mask)
{
    return (struct sk_buff *)malloc(len);
}


struct sk_buff *__alloc_skb(unsigned int size,
                            gfp_t priority, int fclone, int node)
{
    return (struct sk_buff *)malloc(size);
}

/**
 *    skb_trim - remove end from a buffer
 *    @skb: buffer to alter
 *    @len: new length
 *
 *    Cut the length of a buffer down by removing data from the tail. If
 *    the buffer is already under the length specified it is not modified.
 *    The skb must be linear.
 */
void skb_trim(struct sk_buff *skb, unsigned int len)
{
    if (skb->len > len)
        __skb_trim(skb, len);
}
EXPORT_SYMBOL(skb_trim);


void *__pskb_pull_tail(struct sk_buff *skb, int delta)
{
    return NULL;
}


int ___pskb_trim(struct sk_buff *skb, unsigned int len)
{
    return 0;
}


void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
    unsigned long flags;

    spin_lock_irqsave(&list->lock, flags);
    __skb_unlink(skb, list);
    spin_unlock_irqrestore(&list->lock, flags);
}
EXPORT_SYMBOL(skb_unlink);


static struct sk_buff *__skb_clone(struct sk_buff *n, struct sk_buff *skb)
{
#define C(x) n->x = skb->x

    n->next = n->prev = NULL;
    n->sk = NULL;
//    __copy_skb_header(n, skb);

    C(len);
    C(data_len);
    C(mac_len);
    n->hdr_len = skb->nohdr ? skb_headroom(skb) : skb->hdr_len;
    n->cloned = 1;
    n->nohdr = 0;
    n->peeked = 0;
    C(pfmemalloc);
    n->destructor = NULL;
    C(tail);
    C(end);
    C(head);
    C(head_frag);
    C(data);
    C(truesize);
//    refcount_set(&n->users, 1);

    atomic_inc(&(skb_shinfo(skb)->dataref));
    skb->cloned = 1;

    return n;
#undef C
}


struct sk_buff *skb_clone(struct sk_buff *skb, gfp_t gfp_mask)
{
    struct sk_buff_fclones *fclones = container_of(skb,
                               struct sk_buff_fclones,
                               skb1);
    struct sk_buff *n;

//    if (skb_orphan_frags(skb, gfp_mask))
//        return NULL;

//    if (skb->fclone == SKB_FCLONE_ORIG &&
//        refcount_read(&fclones->fclone_ref) == 1) {
//        n = &fclones->skb2;
//        refcount_set(&fclones->fclone_ref, 2);
//    } else {
//        if (skb_pfmemalloc(skb))
//            gfp_mask |= __GFP_MEMALLOC;
//
//        n = kmem_cache_alloc(skbuff_head_cache, gfp_mask);
//        if (!n)
//            return NULL;
//
//        n->fclone = SKB_FCLONE_UNAVAILABLE;
//    }

    return __skb_clone(n, skb);
}

struct sk_buff *skb_clone_sk(struct sk_buff *skb)
{
    struct sock *sk = skb->sk;
    struct sk_buff *clone;

//    if (!sk || !refcount_inc_not_zero(&sk->sk_refcnt))
//        return NULL;

    clone = skb_clone(skb, GFP_ATOMIC);
    if (!clone) {
//        sock_put(sk);
        return NULL;
    }

    clone->sk = sk;
//    clone->destructor = sock_efree;

    return clone;
}
EXPORT_SYMBOL(skb_clone_sk);


void consume_skb(struct sk_buff *skb)
{
    if (!skb_unref(skb))
        return;

//    trace_consume_skb(skb);
    __kfree_skb(skb);
}
EXPORT_SYMBOL(consume_skb);


void __kfree_skb(struct sk_buff *skb)
{
//    skb_release_all(skb);
//    kfree_skbmem(skb);
}
EXPORT_SYMBOL(__kfree_skb);


void skb_queue_purge(struct sk_buff_head *list)
{
    struct sk_buff *skb;
    while ((skb = skb_dequeue(list)) != NULL)
        kfree_skb(skb);
}
EXPORT_SYMBOL(skb_queue_purge);



/**
 *    skb_copy_bits - copy bits from skb to kernel buffer
 *    @skb: source skb
 *    @offset: offset in source
 *    @to: destination buffer
 *    @len: number of bytes to copy
 *
 *    Copy the specified number of bytes from the source skb to the
 *    destination buffer.
 *
 *    CAUTION ! :
 *        If its prototype is ever changed,
 *        check arch/{*}/net/{*}.S files,
 *        since it is called from BPF assembly code.
 */
int skb_copy_bits(const struct sk_buff *skb, int offset, void *to, int len)
{
    int start = skb_headlen(skb);
    struct sk_buff *frag_iter;
    int i, copy;

    if (offset > (int)skb->len - len)
        goto fault;

    /* Copy header. */
    if ((copy = start - offset) > 0) {
        if (copy > len)
            copy = len;
//        skb_copy_from_linear_data_offset(skb, offset, to, copy);
        if ((len -= copy) == 0)
            return 0;
        offset += copy;
        to     = (char *)to + copy;
    }

    for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
        int end;
        skb_frag_t *f = &skb_shinfo(skb)->frags[i];

        WARN_ON(start > offset + len);

        end = start + skb_frag_size(f);
        if ((copy = end - offset) > 0) {
            u32 p_off, p_len, copied;
            struct page *p;
            u8 *vaddr;

            if (copy > len)
                copy = len;

//            skb_frag_foreach_page(f,
//                          skb_frag_off(f) + offset - start,
//                          copy, p, p_off, p_len, copied) {
//                vaddr = kmap_atomic(p);
//                memcpy(to + copied, vaddr + p_off, p_len);
//                kunmap_atomic(vaddr);
//            }

            if ((len -= copy) == 0)
                return 0;
            offset += copy;
            to     = (char *)to + copy;
        }
        start = end;
    }

    skb_walk_frags(skb, frag_iter) {
        int end;

        WARN_ON(start > offset + len);

        end = start + frag_iter->len;
        if ((copy = end - offset) > 0) {
            if (copy > len)
                copy = len;
            if (skb_copy_bits(frag_iter, offset - start, to, copy))
                goto fault;
            if ((len -= copy) == 0)
                return 0;
            offset += copy;
            to     = (char *)to + copy;
        }
        start = end;
    }

    if (!len)
        return 0;

fault:
    return -EFAULT;
}
EXPORT_SYMBOL(skb_copy_bits);


static inline int skb_alloc_rx_flag(const struct sk_buff *skb)
{
    if (skb_pfmemalloc(skb))
        return SKB_ALLOC_RX;
    return 0;
}


static inline void __skb_dst_copy(struct sk_buff *nskb, unsigned long refdst)
{
    nskb->_skb_refdst = refdst;
//    if (!(nskb->_skb_refdst & SKB_DST_NOREF))
//        dst_clone(skb_dst(nskb));
}

static inline void skb_dst_copy(struct sk_buff *nskb, const struct sk_buff *oskb)
{
    __skb_dst_copy(nskb, oskb->_skb_refdst);
}

/* Make sure a field is enclosed inside headers_start/headers_end section */
#define CHECK_SKB_FIELD(field) \
    BUILD_BUG_ON(offsetof(struct sk_buff, field) <        \
             offsetof(struct sk_buff, headers_start));    \
    BUILD_BUG_ON(offsetof(struct sk_buff, field) >        \
             offsetof(struct sk_buff, headers_end));    \

static void __copy_skb_header(struct sk_buff *_new, const struct sk_buff *old)
{
    _new->tstamp        = old->tstamp;
    /* We do not copy old->sk */
    _new->dev        = old->dev;
    memcpy(_new->cb, old->cb, sizeof(old->cb));
    skb_dst_copy(_new, old);
    __skb_ext_copy(_new, old);
    __nf_copy(_new, old, false);

    /* Note : this field could be in headers_start/headers_end section
     * It is not yet because we do not want to have a 16 bit hole
     */
    _new->queue_mapping = old->queue_mapping;

    memcpy(&_new->headers_start, &old->headers_start,
           offsetof(struct sk_buff, headers_end) -
           offsetof(struct sk_buff, headers_start));
    CHECK_SKB_FIELD(protocol);
    CHECK_SKB_FIELD(csum);
    CHECK_SKB_FIELD(hash);
    CHECK_SKB_FIELD(priority);
    CHECK_SKB_FIELD(skb_iif);
    CHECK_SKB_FIELD(vlan_proto);
    CHECK_SKB_FIELD(vlan_tci);
    CHECK_SKB_FIELD(transport_header);
    CHECK_SKB_FIELD(network_header);
    CHECK_SKB_FIELD(mac_header);
    CHECK_SKB_FIELD(inner_protocol);
    CHECK_SKB_FIELD(inner_transport_header);
    CHECK_SKB_FIELD(inner_network_header);
    CHECK_SKB_FIELD(inner_mac_header);
    CHECK_SKB_FIELD(mark);
#ifdef CONFIG_NETWORK_SECMARK
    CHECK_SKB_FIELD(secmark);
#endif
#ifdef CONFIG_NET_RX_BUSY_POLL
    CHECK_SKB_FIELD(napi_id);
#endif
#ifdef CONFIG_XPS
    CHECK_SKB_FIELD(sender_cpu);
#endif
#ifdef CONFIG_NET_SCHED
    CHECK_SKB_FIELD(tc_index);
#endif

}

void skb_headers_offset_update(struct sk_buff *skb, int off)
{
    /* Only adjust this if it actually is csum_start rather than csum */
    if (skb->ip_summed == CHECKSUM_PARTIAL)
        skb->csum_start += off;
    /* {transport,network,mac}_header and tail are relative to skb->head */
    skb->transport_header += off;
    skb->network_header   += off;
    if (skb_mac_header_was_set(skb))
        skb->mac_header += off;
    skb->inner_transport_header += off;
    skb->inner_network_header += off;
    skb->inner_mac_header += off;
}
EXPORT_SYMBOL(skb_headers_offset_update);


void skb_copy_header(struct sk_buff *_new, const struct sk_buff *old)
{
    __copy_skb_header(_new, old);

    skb_shinfo(_new)->gso_size = skb_shinfo(old)->gso_size;
    skb_shinfo(_new)->gso_segs = skb_shinfo(old)->gso_segs;
    skb_shinfo(_new)->gso_type = skb_shinfo(old)->gso_type;
}
EXPORT_SYMBOL(skb_copy_header);


/**
 *    skb_copy_expand    -    copy and expand sk_buff
 *    @skb: buffer to copy
 *    @newheadroom: new free bytes at head
 *    @newtailroom: new free bytes at tail
 *    @gfp_mask: allocation priority
 *
 *    Make a copy of both an &sk_buff and its data and while doing so
 *    allocate additional space.
 *
 *    This is used when the caller wishes to modify the data and needs a
 *    private copy of the data to alter as well as more space for new fields.
 *    Returns %NULL on failure or the pointer to the buffer
 *    on success. The returned buffer has a reference count of 1.
 *
 *    You must pass %GFP_ATOMIC as the allocation priority if this function
 *    is called from an interrupt.
 */
struct sk_buff *skb_copy_expand(const struct sk_buff *skb,
                int newheadroom, int newtailroom,
                gfp_t gfp_mask)
{
    /*
     *    Allocate the copy buffer
     */
    struct sk_buff *n = __alloc_skb(newheadroom + skb->len + newtailroom,
                    gfp_mask, skb_alloc_rx_flag(skb),
                    NUMA_NO_NODE);
    int oldheadroom = skb_headroom(skb);
    int head_copy_len, head_copy_off;

    if (!n)
        return NULL;

    skb_reserve(n, newheadroom);

    /* Set the tail pointer and length */
    skb_put(n, skb->len);

    head_copy_len = oldheadroom;
    head_copy_off = 0;
    if (newheadroom <= head_copy_len)
        head_copy_len = newheadroom;
    else
        head_copy_off = newheadroom - head_copy_len;

    /* Copy the linear header and data. */
    BUG_ON(skb_copy_bits(skb, -head_copy_len, n->head + head_copy_off,
                 skb->len + head_copy_len));

    skb_copy_header(n, skb);

    skb_headers_offset_update(n, newheadroom - oldheadroom);

    return n;
}
EXPORT_SYMBOL(skb_copy_expand);
