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
