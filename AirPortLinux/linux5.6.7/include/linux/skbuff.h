//
//  skbuff.h
//  AppleIwlWiFi
//
//  Created by User-PC on 2019/11/7.
//  Copyright © 2019 User-PC. All rights reserved.
//

#ifndef skbuff_h
#define skbuff_h

#include <linux/types.h>
//#include <linux/kernel.h>
//#include <linux/spinlock.h>
//#include <linux/dma-mapping.h>
//#include <linux/mm.h>
//#include <linux/debugfs.h>

#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/time.h>
#include <linux/bug.h>
//#include <linux/bvec.h>
//#include <linux/cache.h>
#include <linux/rbtree.h>
#include <linux/socket.h>
//#include <linux/refcount.h>

#include <linux/atomic.h>
#include <asm/types.h>
#include <linux/spinlock.h>
//#include <linux/net.h>
//#include <linux/textsearch.h>
//#include <net/checksum.h>
#include <linux/rcupdate.h>
//#include <linux/hrtimer.h>
#include <linux/dma-mapping.h>
#include <linux/netdev_features.h>
#include <linux/sched.h>
//#include <linux/sched/clock.h>
#include <net/flow_dissector.h>
//#include <linux/splice.h>
#include <linux/in6.h>
//#include <linux/if_packet.h>
//#include <net/flow.h>

typedef unsigned char * sk_buff_data_t;


#define SKB_DATAREF_SHIFT 16
#define SKB_DATAREF_MASK ((1 << SKB_DATAREF_SHIFT) - 1)

#define L1_CACHE_SHIFT        5
#define L1_CACHE_BYTES        (1 << L1_CACHE_SHIFT)

#define SMP_CACHE_BYTES L1_CACHE_BYTES

/* Don't change this without changing skb_csum_unnecessary! */
#define CHECKSUM_NONE        0
#define CHECKSUM_UNNECESSARY    1
#define CHECKSUM_COMPLETE    2
#define CHECKSUM_PARTIAL    3

/* Maximum value in skb->csum_level */
#define SKB_MAX_CSUM_LEVEL    3

#define SKB_DATA_ALIGN(X)    ALIGN(X, SMP_CACHE_BYTES)
#define SKB_WITH_OVERHEAD(X)    \
((X) - SKB_DATA_ALIGN(sizeof(struct skb_shared_info)))
#define SKB_MAX_ORDER(X, ORDER) \
SKB_WITH_OVERHEAD((PAGE_SIZE << (ORDER)) - (X))
#define SKB_MAX_HEAD(X)        (SKB_MAX_ORDER((X), 0))
#define SKB_MAX_ALLOC        (SKB_MAX_ORDER(0, 2))

/* return minimum truesize of one skb containing X bytes of data */
#define SKB_TRUESIZE(X) ((X) +                        \
SKB_DATA_ALIGN(sizeof(struct sk_buff)) +    \
SKB_DATA_ALIGN(sizeof(struct skb_shared_info)))


#define SKB_ALLOC_FCLONE    0x01
#define SKB_ALLOC_RX        0x02
#define SKB_ALLOC_NAPI        0x04

struct skb_shared_hwtstamps {
    ktime_t    hwtstamp;
};


struct skb_frag_struct {
    struct {
        struct page *p;
    } page;
#if (BITS_PER_LONG > 32) || (PAGE_SIZE >= 65536)
    __u32 page_offset;
    __u32 size;
#else
    __u16 page_offset;
    __u16 size;
#endif
};

typedef struct skb_frag_struct skb_frag_t;

/* To allow 64K frame to be packed as single skb without frag_list we
 * require 64K/PAGE_SIZE pages plus 1 additional page to allow for
 * buffers which do not start on a page boundary.
 *
 * Since GRO uses frags we allocate at least 16 regardless of page
 * size.
 */
#if (65536/PAGE_SIZE + 1) < 16
#define MAX_SKB_FRAGS 16UL
#else
#define MAX_SKB_FRAGS (65536/PAGE_SIZE + 1)
#endif

/* This data is invariant across clones and lives at
 * the end of the header data, ie. at skb->end.
 */
struct skb_shared_info {
    __u8        __unused nouse;
    __u8        meta_len;
    __u8        nr_frags;
    __u8        tx_flags;
    unsigned short    gso_size;
    /* Warning: this field is not always filled in (UFO)! */
    unsigned short    gso_segs;
    struct sk_buff    *frag_list;
    struct skb_shared_hwtstamps hwtstamps;
    unsigned int    gso_type;
    u32        tskey;
    
    /*
     * Warning : all fields before dataref are cleared in __alloc_skb()
     */
    atomic_t    dataref;
    
    /* Intermediate layers must ensure that destructor_arg
     * remains valid until skb destructor */
    void *        destructor_arg;
    
    /* must be last field, see pskb_expand_head() */
    skb_frag_t    frags[MAX_SKB_FRAGS];
};


#define kmemcheck_bitfield_begin(name)
#define kmemcheck_bitfield_end(name)


struct sk_buff {
    union {
        struct {
            /* These two members must be first. */
            struct sk_buff        *next;
            struct sk_buff        *prev;

            union {
                struct net_device    *dev;
                /* Some protocols might use this space to store information,
                 * while device pointer would be NULL.
                 * UDP receive path is one user.
                 */
                unsigned long        dev_scratch;
            };
        };
        struct rb_node        rbnode; /* used in netem, ip4 defrag, and tcp stack */
        struct list_head    list;
    };

    union {
        struct sock        *sk;
        int            ip_defrag_offset;
    };

    union {
        ktime_t        tstamp;
        u64        skb_mstamp_ns; /* earliest departure time */
    };
    /*
     * This is the control buffer. It is free to use for every
     * layer. Please put your private variables there. If you
     * want to keep them across layers you have to do a skb_clone()
     * first. This is owned by whoever has the skb queued ATM.
     */
    char            cb[48] __aligned(8);

    union {
        struct {
            unsigned long    _skb_refdst;
            void        (*destructor)(struct sk_buff *skb);
        };
        struct list_head    tcp_tsorted_anchor;
    };

#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
    unsigned long         _nfct;
#endif
    unsigned int        len,
                data_len;
    __u16            mac_len,
                hdr_len;

    /* Following fields are _not_ copied in __copy_skb_header()
     * Note that queue_mapping is here mostly to fill a hole.
     */
    __u16            queue_mapping;

/* if you move cloned around you also must adapt those constants */
#ifdef __BIG_ENDIAN_BITFIELD
#define CLONED_MASK    (1 << 7)
#else
#define CLONED_MASK    1
#endif
#define CLONED_OFFSET()        offsetof(struct sk_buff, __cloned_offset)

    /* private: */
    __u8            __cloned_offset[0];
    /* public: */
    __u8            cloned:1,
                nohdr:1,
                fclone:2,
                peeked:1,
                head_frag:1,
                pfmemalloc:1;
#ifdef CONFIG_SKB_EXTENSIONS
    __u8            active_extensions;
#endif
    /* fields enclosed in headers_start/headers_end are copied
     * using a single memcpy() in __copy_skb_header()
     */
    /* private: */
    __u32            headers_start[0];
    /* public: */

/* if you move pkt_type around you also must adapt those constants */
#ifdef __BIG_ENDIAN_BITFIELD
#define PKT_TYPE_MAX    (7 << 5)
#else
#define PKT_TYPE_MAX    7
#endif
#define PKT_TYPE_OFFSET()    offsetof(struct sk_buff, __pkt_type_offset)

    /* private: */
    __u8            __pkt_type_offset[0];
    /* public: */
    __u8            pkt_type:3;
    __u8            ignore_df:1;
    __u8            nf_trace:1;
    __u8            ip_summed:2;
    __u8            ooo_okay:1;

    __u8            l4_hash:1;
    __u8            sw_hash:1;
    __u8            wifi_acked_valid:1;
    __u8            wifi_acked:1;
    __u8            no_fcs:1;
    /* Indicates the inner headers are valid in the skbuff. */
    __u8            encapsulation:1;
    __u8            encap_hdr_csum:1;
    __u8            csum_valid:1;

#ifdef __BIG_ENDIAN_BITFIELD
#define PKT_VLAN_PRESENT_BIT    7
#else
#define PKT_VLAN_PRESENT_BIT    0
#endif
#define PKT_VLAN_PRESENT_OFFSET()    offsetof(struct sk_buff, __pkt_vlan_present_offset)
    /* private: */
    __u8            __pkt_vlan_present_offset[0];
    /* public: */
    __u8            vlan_present:1;
    __u8            csum_complete_sw:1;
    __u8            csum_level:2;
    __u8            csum_not_inet:1;
    __u8            dst_pending_confirm:1;
#ifdef CONFIG_IPV6_NDISC_NODETYPE
    __u8            ndisc_nodetype:2;
#endif

    __u8            ipvs_property:1;
    __u8            inner_protocol_type:1;
    __u8            remcsum_offload:1;
#ifdef CONFIG_NET_SWITCHDEV
    __u8            offload_fwd_mark:1;
    __u8            offload_l3_fwd_mark:1;
#endif
#ifdef CONFIG_NET_CLS_ACT
    __u8            tc_skip_classify:1;
    __u8            tc_at_ingress:1;
#endif
#ifdef CONFIG_NET_REDIRECT
    __u8            redirected:1;
    __u8            from_ingress:1;
#endif
#ifdef CONFIG_TLS_DEVICE
    __u8            decrypted:1;
#endif

#ifdef CONFIG_NET_SCHED
    __u16            tc_index;    /* traffic control index */
#endif

    union {
        __wsum        csum;
        struct {
            __u16    csum_start;
            __u16    csum_offset;
        };
    };
    __u32            priority;
    int            skb_iif;
    __u32            hash;
    __be16            vlan_proto;
    __u16            vlan_tci;
#if defined(CONFIG_NET_RX_BUSY_POLL) || defined(CONFIG_XPS)
    union {
        unsigned int    napi_id;
        unsigned int    sender_cpu;
    };
#endif
#ifdef CONFIG_NETWORK_SECMARK
    __u32        secmark;
#endif

    union {
        __u32        mark;
        __u32        reserved_tailroom;
    };

    union {
        __be16        inner_protocol;
        __u8        inner_ipproto;
    };

    __u16            inner_transport_header;
    __u16            inner_network_header;
    __u16            inner_mac_header;

    __be16            protocol;
    __u16            transport_header;
    __u16            network_header;
    __u16            mac_header;

    /* private: */
    __u32            headers_end[0];
    /* public: */

    /* These elements must be at the end, see alloc_skb() for details.  */
    sk_buff_data_t        tail;
    sk_buff_data_t        end;
    unsigned char        *head,
                *data;
    unsigned int        truesize;
//    refcount_t        users;

#ifdef CONFIG_SKB_EXTENSIONS
    /* only useable after checking ->active_extensions != 0 */
    struct skb_ext        *extensions;
#endif
};

struct sk_buff_head {
    /* These two members must be first. */
    struct sk_buff  *next;
    struct sk_buff  *prev;
    
    __u32           qlen;
    spinlock_t      lock;
};


/* Layout of fast clones : [skb1][skb2][fclone_ref] */
struct sk_buff_fclones {
    struct sk_buff    skb1;

    struct sk_buff    skb2;

//    refcount_t    fclone_ref;
};


/* Definitions for tx_flags in struct skb_shared_info */
enum {
    /* generate hardware time stamp */
    SKBTX_HW_TSTAMP = 1 << 0,

    /* generate software time stamp when queueing packet to NIC */
    SKBTX_SW_TSTAMP = 1 << 1,

    /* device driver is going to provide hardware time stamp */
    SKBTX_IN_PROGRESS = 1 << 2,

    /* device driver supports TX zero-copy buffers */
    SKBTX_DEV_ZEROCOPY = 1 << 3,

    /* generate wifi status information (where possible) */
    SKBTX_WIFI_STATUS = 1 << 4,

    /* This indicates at least one fragment might be overwritten
     * (as in vmsplice(), sendfile() ...)
     * If we need to compute a TX checksum, we'll need to copy
     * all frags to avoid possible bad checksum
     */
    SKBTX_SHARED_FRAG = 1 << 5,

    /* generate software time stamp when entering packet scheduling */
    SKBTX_SCHED_TSTAMP = 1 << 6,
};


/**
 * skb_pfmemalloc - Test if the skb was allocated from PFMEMALLOC reserves
 * @skb: buffer
 */
static inline bool skb_pfmemalloc(const struct sk_buff *skb)
{
    return unlikely(skb->pfmemalloc);
}

static inline bool skb_is_nonlinear(struct sk_buff *skb)
{
    return skb->data_len;
}

static inline int skb_mac_header_was_set(const struct sk_buff *skb)
{
    return skb->mac_header != (typeof(skb->mac_header))~0U;
}

static inline unsigned char *skb_network_header(const struct sk_buff *skb)
{
    return skb->head + skb->network_header;
}

static inline unsigned char *skb_inner_network_header(const struct sk_buff *skb)
{
    return skb->head + skb->inner_network_header;
}

static inline u32 skb_network_header_len(const struct sk_buff *skb)
{
    return skb->transport_header - skb->network_header;
}


static inline unsigned char *skb_transport_header(const struct sk_buff *skb)
{
    return skb->head + skb->transport_header;
}

static inline int skb_network_offset(const struct sk_buff *skb)
{
    return skb_network_header(skb) - skb->data;
}

static inline unsigned char *skb_inner_transport_header(const struct sk_buff
                                                        *skb)
{
    return skb->head + skb->inner_transport_header;
}

static inline int skb_transport_offset(const struct sk_buff *skb)
{
    return skb_transport_header(skb) - skb->data;
}

static inline struct sk_buff *skb_peek_tail(const struct sk_buff_head *list_)
{
    struct sk_buff *skb = list_->prev;
    
    if (skb == (struct sk_buff *)list_)
        skb = NULL;
    return skb;
    
}




struct sk_buff *__alloc_skb(unsigned int size,
                            gfp_t priority, int fclone, int node);

static inline struct sk_buff *alloc_skb(unsigned int size, gfp_t priority)
{
    return __alloc_skb(size, priority, 0, -1);
}

static inline unsigned char *skb_tail_pointer(const struct sk_buff *skb)
{
    return skb->head;// + skb->tail;
}


static int skb_headlen(const struct sk_buff *skb) {
//    return (int)mbuf_len(skb->mac_data);
    return 0;
}

static inline void *skb_push(struct sk_buff *skb, unsigned int len)
{
    skb->data -= len;
    skb->len  += len;
    return skb->data;
}

static inline void skb_reset_tail_pointer(struct sk_buff *skb)
{
    skb->tail = (sk_buff_data_t)(skb->data - skb->head);
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
    skb_reset_tail_pointer(skb);
    skb->tail += offset;
}

static inline void *skb_pull(struct sk_buff *skb, unsigned int len)
{
    skb->len -= len;
    BUG_ON(skb->len < skb->data_len);
    return skb->data += len;
}


static void *skb_put(struct sk_buff *skb, unsigned int len)
{
    return NULL;
}


static inline void *skb_put_zero(struct sk_buff *skb, unsigned int len)
{
    void *tmp = skb_put(skb, len);

    memset(tmp, 0, len);

    return tmp;
}

static inline void *skb_put_data(struct sk_buff *skb, const void *data,
                 unsigned int len)
{
    void *tmp = skb_put(skb, len);

    memcpy(tmp, data, len);

    return tmp;
}


int ___pskb_trim(struct sk_buff *skb, unsigned int len);

static inline void __skb_set_length(struct sk_buff *skb, unsigned int len)
{
    if (WARN_ON(skb_is_nonlinear(skb)))
        return;
    skb->len = len;
//    skb_set_tail_pointer(skb, len);
}

static inline void __skb_trim(struct sk_buff *skb, unsigned int len)
{
    __skb_set_length(skb, len);
}

void skb_trim(struct sk_buff *skb, unsigned int len);

struct sk_buff *skb_clone_sk(struct sk_buff *skb);
struct sk_buff *skb_clone(struct sk_buff *skb, gfp_t priority);
void consume_skb(struct sk_buff *skb);
void  __kfree_skb(struct sk_buff *skb);
void skb_queue_purge(struct sk_buff_head *list);

static inline int __pskb_trim(struct sk_buff *skb, unsigned int len)
{
    if (skb->data_len)
        return ___pskb_trim(skb, len);
    __skb_trim(skb, len);
    return 0;
}

static inline int pskb_trim(struct sk_buff *skb, unsigned int len)
{
    return (len < skb->len) ? __pskb_trim(skb, len) : 0;
}


void *__pskb_pull_tail(struct sk_buff *skb, int delta);

static inline int __skb_linearize(struct sk_buff *skb)
{
    return __pskb_pull_tail(skb, skb->data_len) ? 0 : -ENOMEM;
}

/**
 *    skb_linearize - convert paged skb to linear one
 *    @skb: buffer to linarize
 *
 *    If there is no free memory -ENOMEM is returned, otherwise zero
 *    is returned and the old skb data released.
 */
static inline int skb_linearize(struct sk_buff *skb)
{
    return skb_is_nonlinear(skb) ? __skb_linearize(skb) : 0;
}


static inline bool pskb_may_pull(struct sk_buff *skb, unsigned int len)
{
    if (likely(len <= skb_headlen(skb)))
        return true;
    if (unlikely(len > skb->len))
        return false;
    return __pskb_pull_tail(skb, len - skb_headlen(skb)) != NULL;
}


/* Note: This doesn't put any conntrack info in dst. */
static inline void __nf_copy(struct sk_buff *dst, const struct sk_buff *src,
                 bool copy)
{
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
    dst->_nfct = src->_nfct;
    nf_conntrack_get(skb_nfct(src));
#endif
#if IS_ENABLED(CONFIG_NETFILTER_XT_TARGET_TRACE) || defined(CONFIG_NF_TABLES)
    if (copy)
        dst->nf_trace = src->nf_trace;
#endif
}

static inline void skb_ext_put(struct sk_buff *skb)
{
//    if (skb->active_extensions)
//        __skb_ext_put(skb->extensions);
}

static inline void __skb_ext_copy(struct sk_buff *dst,
                  const struct sk_buff *src)
{
//    dst->active_extensions = src->active_extensions;
//
//    if (src->active_extensions) {
//        struct skb_ext *ext = src->extensions;
//
//        refcount_inc(&ext->refcnt);
//        dst->extensions = ext;
//    }
}

static inline void skb_ext_copy(struct sk_buff *dst, const struct sk_buff *src)
{
    skb_ext_put(dst);
    __skb_ext_copy(dst, src);
}


static void kfree_skb(struct sk_buff *skb)
{
    __kfree_skb(skb);
}

static void dev_kfree_skb(struct sk_buff *skb)
{
    // todo
}

static void dev_kfree_skb_any(struct sk_buff *skb)
{
    // todo
}

static struct page *alloc_pages(gfp_t gtp, size_t size)
{
    // todo
    return NULL;
}
#define alloc_page(gfp_mask) alloc_pages(gfp_mask, 0)

static void kfree_skb(struct page *page)
{
    
}

#define __free_pages(a,b) kfree_skb(a)
#define __free_page(a) kfree_skb(a)

#define __skb_linearize(x) 1

#ifndef skb_walk_frags
#define skb_walk_frags(skb, iter)    \
for (iter = skb_shinfo(skb)->frag_list; iter; iter = iter->next)
#endif


#define skb_queue_walk(queue, skb) \
for (skb = (queue)->next;                    \
skb != (struct sk_buff *)(queue);                \
skb = skb->next)

static inline unsigned char *skb_end_pointer(const struct sk_buff *skb)
{
    return skb->end;
}

#define skb_shinfo(SKB)    ((struct skb_shared_info *)(skb_end_pointer(SKB)))

static inline bool skb_has_frag_list(const struct sk_buff *skb)
{
    return skb_shinfo(skb)->frag_list != NULL;
}


static inline void __skb_queue_head_init(struct sk_buff_head *list)
{
    //  spin_lock_init(&list->lock);
    list->prev = list->next = (struct sk_buff *)list;
    list->qlen = 0;
}

static inline void skb_queue_head_init(struct sk_buff_head *list)
{
    __skb_queue_head_init(list);
}

static inline void skb_copy_from_linear_data_offset(const struct sk_buff *skb,
                            const int offset, void *to,
                            const unsigned int len)
{
    memcpy(to, skb->data + offset, len);
}

/**
 *    skb_queue_empty - check if a queue is empty
 *    @list: queue head
 *
 *    Returns true if the queue is empty, false otherwise.
 */
static inline int skb_queue_empty(const struct sk_buff_head *list)
{
    return list->next == (struct sk_buff *)list;
}

static inline struct sk_buff *__skb_dequeue(struct sk_buff_head *list)
{
    struct sk_buff *next, *prev, *result;
    
    prev = (struct sk_buff *) list;
    next = prev->next;
    result = NULL;
    if(next != prev) {
        result       = next;
        next         = next->next;
        list->qlen--;
        next->prev   = prev;
        prev->next   = next;
        result->next = result->prev = NULL;
    }
    return result;
}

static struct sk_buff *skb_dequeue(struct sk_buff_head *list)
{
    unsigned long flags;
    struct sk_buff *result;

    spin_lock_irqsave(&list->lock, flags);
    result = __skb_dequeue(list);
    spin_unlock_irqrestore(&list->lock, flags);
    return result;
}

static inline void __skb_queue_tail(struct sk_buff_head *list,struct sk_buff *newsk)
{
    struct sk_buff *prev, *next;
    
    list->qlen++;
    next = (struct sk_buff *)list;
    prev = next->prev;
    newsk->next = next;
    newsk->prev = prev;
    next->prev  = prev->next = newsk;
}

static void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
//    unsigned long flags;

//    spin_lock_irqsave(&list->lock, flags);
    __skb_queue_tail(list, newsk);
//    spin_unlock_irqrestore(&list->lock, flags);
}
EXPORT_SYMBOL(skb_queue_tail);

//static inline void __skb_queue_splice(const struct sk_buff_head *list,
//                                      struct sk_buff *prev,
//                                      struct sk_buff *next)
//{
//    struct sk_buff *first = list->next;
//    struct sk_buff *last = list->prev;
//
//    first->prev = prev;
//    prev->next = first;
//
//    last->next = next;
//    next->prev = last;
//}

static inline void __skb_queue_splice(const struct sk_buff_head *list,
                      struct sk_buff *prev,
                      struct sk_buff *next)
{
    struct sk_buff *first = list->next;
    struct sk_buff *last = list->prev;

    WRITE_ONCE(first->prev, prev);
    WRITE_ONCE(prev->next, first);

    WRITE_ONCE(last->next, next);
    WRITE_ONCE(next->prev, last);
}

/**
 *    skb_queue_splice - join two skb lists, this is designed for stacks
 *    @list: the new list to add
 *    @head: the place to add it in the first list
 */
static inline void skb_queue_splice(const struct sk_buff_head *list,
                    struct sk_buff_head *head)
{
    if (!skb_queue_empty(list)) {
        __skb_queue_splice(list, (struct sk_buff *) head, head->next);
        head->qlen += list->qlen;
    }
}


/**
 *    skb_queue_splice_init - join two skb lists and reinitialise the emptied list
 *    @list: the new list to add
 *    @head: the place to add it in the first list
 *
 *    The list at @list is reinitialised
 */
static inline void skb_queue_splice_init(struct sk_buff_head *list,
                                         struct sk_buff_head *head)
{
    if (!skb_queue_empty(list)) {
        __skb_queue_splice(list, (struct sk_buff *) head, head->next);
        head->qlen += list->qlen;
        __skb_queue_head_init(list);
    }
}

static inline void skb_queue_splice_tail(const struct sk_buff_head *list,
                     struct sk_buff_head *head)
{
    if (!skb_queue_empty(list)) {
        __skb_queue_splice(list, head->prev, (struct sk_buff *) head);
        head->qlen += list->qlen;
    }
}

static inline unsigned int skb_frag_size(const skb_frag_t *frag)
{
    return frag->size;
}


/**
 * skb_frag_page - retrieve the page referred to by a paged fragment
 * @frag: the paged fragment
 *
 * Returns the &struct page associated with @frag.
 */
static inline struct page *skb_frag_page(const skb_frag_t *frag)
{
    return frag->page.p;
}

/**
 * skb_frag_dma_map - maps a paged fragment via the DMA API
 * @dev: the device to map the fragment to
 * @frag: the paged fragment to map
 * @offset: the offset within the fragment (starting at the
 *          fragment's own offset)
 * @size: the number of bytes to map
 * @dir: the direction of the mapping (``PCI_DMA_*``)
 *
 * Maps the page associated with @frag to @device.
 */
static inline dma_addr_t skb_frag_dma_map(struct device *dev,
                                          const skb_frag_t *frag,
                                          size_t offset, size_t size,
                                          enum dma_data_direction dir)
{
    return dma_map_page(dev, skb_frag_page(frag),
                        frag->page_offset + offset, size, dir);
}


/**
 * skb_frag_address - gets the address of the data contained in a paged fragment
 * @frag: the paged fragment buffer
 *
 * Returns the address of the data within @frag. The page must already
 * be mapped.
 */
static inline void *skb_frag_address(const skb_frag_t *frag)
{
    return (char *)page_address(skb_frag_page(frag)) + frag->page_offset;
}

/**
 *    skb_headroom - bytes at buffer head
 *    @skb: buffer to check
 *
 *    Return the number of bytes of free space at the head of an &sk_buff.
 */
static inline unsigned int skb_headroom(const struct sk_buff *skb)
{
    return skb->data - skb->head;
}

static inline int skb_checksum_start_offset(const struct sk_buff *skb)
{
    return skb->csum_start - skb_headroom(skb);
}


static void skb_reserve(struct sk_buff *skb, int len) {
//    void *data = (UInt8*)mbuf_data(skb->mac_data) + len;
//    mbuf_setdata(skb->mac_data,data, mbuf_len(skb->mac_data));// m_len is not changed.
}


static int skb_tailroom(const struct sk_buff *skb) {
//    return (int)mbuf_trailingspace(skb->mac_data);
    return 0;
}


//static int skb_put_data(const struct sk_buff *skb, void *data, size_t len) {
//    mbuf_setdata(skb->mac_data,data,len);
//    mbuf_pkthdr_setlen(skb->mac_data,len);
//    mbuf_setlen(skb->mac_data,len);
//    return 0;
//}


static void skb_add_rx_frag(struct sk_buff *skb, int start, struct page *page, size_t offset, size_t len, size_t true_len)
{
//    mbuf_copyback(skb->mac_data,  offset, len, page->mac_data, MBUF_DONTWAIT);
}

static struct sk_buff *skb_copy( struct sk_buff *skb, gfp_t gfp_mask)
{
    struct sk_buff *skb_copy = (struct sk_buff *)IOMalloc(sizeof(struct sk_buff));
//    mbuf_copym(skb->mac_data, 0, mbuf_len(skb->mac_data), 1, &skb_copy->mac_data);
//    skb_copy->intf = skb->intf;
    return skb_copy;//need to check for prev, next
}

static inline bool skb_is_gso(const struct sk_buff *skb)
{
    return skb_shinfo(skb)->gso_size;
}


static inline void __skb_queue_purge(struct sk_buff_head *list)
{
    struct sk_buff *skb;
    while ((skb = __skb_dequeue(list)) != NULL)
        kfree_skb(skb);
}

static
int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail, gfp_t gfp_mask)
{
    return 0;
}


struct sk_buff *__netdev_alloc_skb(struct net_device *dev, unsigned int length,
                   gfp_t gfp_mask);

/**
 *    netdev_alloc_skb - allocate an skbuff for rx on a specific device
 *    @dev: network device to receive on
 *    @length: length to allocate
 *
 *    Allocate a new &sk_buff and assign it a usage count of one. The
 *    buffer has unspecified headroom built in. Users should allocate
 *    the headroom they think they need without accounting for the
 *    built in space. The built in space is used for optimisations.
 *
 *    %NULL is returned if there is no free memory. Although this function
 *    allocates memory it can be called from an interrupt.
 */
static inline struct sk_buff *netdev_alloc_skb(struct net_device *dev,
                           unsigned int length)
{
    return __netdev_alloc_skb(dev, length, GFP_ATOMIC);
}

/* legacy helper around netdev_alloc_skb() */
static inline struct sk_buff *dev_alloc_skb(unsigned int length)
{
    return netdev_alloc_skb(NULL, length);
}



static inline u16 skb_get_queue_mapping(const struct sk_buff *skb)
{
    return skb->queue_mapping;
}

static inline void skb_set_queue_mapping(struct sk_buff *skb, u16 queue_mapping)
{
    skb->queue_mapping = queue_mapping;
}

/**
 *    skb_queue_len    - get queue length
 *    @list_: list to measure
 *
 *    Return the length of an &sk_buff queue.
 */
static inline __u32 skb_queue_len(const struct sk_buff_head *list_)
{
    return list_->qlen;
}


static inline void skb_copy_queue_mapping(struct sk_buff *to, const struct sk_buff *from)
{
    to->queue_mapping = from->queue_mapping;
}

/**
 *    skb_queue_is_last - check if skb is the last entry in the queue
 *    @list: queue head
 *    @skb: buffer
 *
 *    Returns true if @skb is the last buffer on the list.
 */
static inline bool skb_queue_is_last(const struct sk_buff_head *list,
                     const struct sk_buff *skb)
{
    return skb->next == (const struct sk_buff *) list;
}


static inline void skb_reset_network_header(struct sk_buff *skb)
{
    skb->network_header = skb->data - skb->head;
}

static inline void skb_set_network_header(struct sk_buff *skb, const int offset)
{
    skb_reset_network_header(skb);
    skb->network_header += offset;
}


/**
 *    __skb_peek - peek at the head of a non-empty &sk_buff_head
 *    @list_: list to peek at
 *
 *    Like skb_peek(), but the caller knows that the list is not empty.
 */
static inline struct sk_buff *__skb_peek(const struct sk_buff_head *list_)
{
    return list_->next;
}

static inline struct sk_buff *skb_peek(const struct sk_buff_head *list_)
{
    struct sk_buff *skb = list_->next;

    if (skb == (struct sk_buff *)list_)
        skb = NULL;
    return skb;
}


static inline struct sk_buff *skb_queue_next(const struct sk_buff_head *list,
                         const struct sk_buff *skb)
{
    /* This BUG_ON may seem severe, but if we just return then we
     * are going to dereference garbage.
     */
    BUG_ON(skb_queue_is_last(list, skb));
    return skb->next;
}

static inline void skb_queue_splice_tail_init(struct sk_buff_head *list,
                          struct sk_buff_head *head)
{
    if (!skb_queue_empty(list)) {
        __skb_queue_splice(list, head->prev, (struct sk_buff *) head);
        head->qlen += list->qlen;
        __skb_queue_head_init(list);
    }
}

void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list);
static inline void __skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
    struct sk_buff *next, *prev;

    WRITE_ONCE(list->qlen, list->qlen - 1);
    next       = skb->next;
    prev       = skb->prev;
    skb->next  = skb->prev = NULL;
    WRITE_ONCE(next->prev, prev);
    WRITE_ONCE(prev->next, next);
}


static inline int skb_cloned(const struct sk_buff *skb)
{
    return skb->cloned &&
           (atomic_read(&skb_shinfo(skb)->dataref) & SKB_DATAREF_MASK) != 1;
}

static inline int skb_header_cloned(const struct sk_buff *skb)
{
    int dataref;

    if (!skb->cloned)
        return 0;

    dataref = atomic_read(&skb_shinfo(skb)->dataref);
    dataref = (dataref & SKB_DATAREF_MASK) - (dataref >> SKB_DATAREF_SHIFT);
    return dataref != 1;
}

static inline int skb_clone_writable(const struct sk_buff *skb, unsigned int len)
{
    return !skb_header_cloned(skb) &&
           skb_headroom(skb) + len <= skb->hdr_len;
}

static inline void skb_reset_mac_header(struct sk_buff *skb)
{
    skb->mac_header = skb->data - skb->head;
}

static inline void skb_set_mac_header(struct sk_buff *skb, const int offset)
{
    skb_reset_mac_header(skb);
    skb->mac_header += offset;
}

static
bool __skb_flow_dissect(const struct net *net,
            const struct sk_buff *skb,
            struct flow_dissector *flow_dissector,
            void *target_container,
            void *data, __be16 proto, int nhoff, int hlen,
                        unsigned int flags) {
    return true;
}

static inline bool
skb_flow_dissect_flow_keys_basic(const struct net *net,
                 const struct sk_buff *skb,
                 struct flow_keys_basic *flow, void *data,
                 __be16 proto, int nhoff, int hlen,
                 unsigned int flags)
{
    memset(flow, 0, sizeof(*flow));
    return __skb_flow_dissect(net, skb, &flow_keys_basic_dissector, flow,
                  data, proto, nhoff, hlen, flags);
}

static inline bool skb_transport_header_was_set(const struct sk_buff *skb)
{
    return skb->transport_header != (typeof(skb->transport_header))~0U;
}

static inline void skb_reset_transport_header(struct sk_buff *skb)
{
    skb->transport_header = skb->data - skb->head;
}

static inline void skb_set_transport_header(struct sk_buff *skb,
                        const int offset)
{
    skb_reset_transport_header(skb);
    skb->transport_header += offset;
}


static inline void skb_probe_transport_header(struct sk_buff *skb)
{
    struct flow_keys_basic keys;

    if (skb_transport_header_was_set(skb))
        return;

    if (skb_flow_dissect_flow_keys_basic(NULL, skb, &keys,
                         NULL, 0, 0, 0, 0))
        skb_set_transport_header(skb, keys.control.thoff);
}

static inline int skb_shared(const struct sk_buff *skb)
{
//    return refcount_read(&skb->users) != 1;
    return 0;
}

static inline bool skb_unref(struct sk_buff *skb)
{
    if (unlikely(!skb))
        return false;
//    if (likely(refcount_read(&skb->users) == 1))
//        smp_rmb();
//    else if (likely(!refcount_dec_and_test(&skb->users)))
//        return false;

    return true;
}





#define skb_queue_walk_safe(queue, skb, tmp)                    \
        for (skb = (queue)->next, tmp = skb->next;            \
             skb != (struct sk_buff *)(queue);                \
             skb = tmp, tmp = skb->next)




int skb_copy_bits(const struct sk_buff *skb, int offset, void *to, int len);


static inline void *__pskb_pull(struct sk_buff *skb, unsigned int len)
{
    if (len > skb_headlen(skb) &&
        !__pskb_pull_tail(skb, len - skb_headlen(skb)))
        return NULL;
    skb->len -= len;
    return skb->data += len;
}

static inline void *pskb_pull(struct sk_buff *skb, unsigned int len)
{
    return unlikely(len > skb->len) ? NULL : __pskb_pull(skb, len);
}


struct sk_buff *skb_copy_expand(const struct sk_buff *skb, int newheadroom,
                int newtailroom, gfp_t priority);

#endif /* skbuff_h */
