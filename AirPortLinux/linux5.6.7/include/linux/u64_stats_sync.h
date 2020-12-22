//
//  u64_stats_sync.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/7/5.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef u64_stats_sync_h
#define u64_stats_sync_h


struct u64_stats_sync {
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
    seqcount_t    seq;
#endif
};


typedef struct {
    u64        v;
} u64_stats_t;


static inline void u64_stats_init(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG == 32 && defined(CONFIG_SMP)
//    seqcount_init(&syncp->seq);
#endif
}

static inline void u64_stats_update_begin(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
//    write_seqcount_begin(&syncp->seq);
#endif
}


static inline void u64_stats_update_end(struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
//    write_seqcount_end(&syncp->seq);
#endif
}


static inline unsigned int __u64_stats_fetch_begin(const struct u64_stats_sync *syncp)
{
//#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
//    return read_seqcount_begin(&syncp->seq);
//#else
//    return 0;
//#endif
    
    return 0;
}

static inline unsigned int u64_stats_fetch_begin(const struct u64_stats_sync *syncp)
{
#if BITS_PER_LONG==32 && !defined(CONFIG_SMP)
//    preempt_disable();
#endif
    return __u64_stats_fetch_begin(syncp);
}


static inline bool __u64_stats_fetch_retry(const struct u64_stats_sync *syncp,
                     unsigned int start)
{
//#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
//    return read_seqcount_retry(&syncp->seq, start);
//#else
//    return false;
//#endif
    
    return false;
}

static inline bool u64_stats_fetch_retry(const struct u64_stats_sync *syncp,
                     unsigned int start)
{
#if BITS_PER_LONG==32 && !defined(CONFIG_SMP)
//    preempt_enable();
#endif
    return __u64_stats_fetch_retry(syncp, start);
}

#endif /* u64_stats_sync_h */
