//
//  bitmap.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/22.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef bitmap_h
#define bitmap_h


#define small_const_nbits(nbits) \
    ((nbits) <= BITS_PER_LONG)

static inline int bitmap_empty(const unsigned long *src, unsigned nbits)
{
    if (small_const_nbits(nbits))
        return ! (*src & BITMAP_LAST_WORD_MASK(nbits));

    return find_first_bit(src, nbits) == nbits;
}

static inline int bitmap_full(const unsigned long *src, unsigned int nbits)
{
    if (small_const_nbits(nbits))
        return ! (~(*src) & BITMAP_LAST_WORD_MASK(nbits));
    
    return find_first_zero_bit(src, nbits) == nbits;
}


static inline void bitmap_fill(unsigned long *dst, unsigned int nbits)
{
    unsigned int nlongs = BITS_TO_LONGS(nbits);
    if (!small_const_nbits(nbits)) {
        unsigned int len = (nlongs - 1) * sizeof(unsigned long);
        memset(dst, 0xff,  len);
    }
    dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
}

static void bitmap_clear(unsigned long *map, unsigned int start, int len)
{
    unsigned long *p = map + BIT_WORD(start);
    const unsigned int size = start + len;
    int bits_to_clear = BITS_PER_LONG - (start % BITS_PER_LONG);
    unsigned long mask_to_clear = BITMAP_FIRST_WORD_MASK(start);
    
    while (len - bits_to_clear >= 0) {
        *p &= ~mask_to_clear;
        len -= bits_to_clear;
        bits_to_clear = BITS_PER_LONG;
        mask_to_clear = ~0UL;
        p++;
    }
    if (len) {
        mask_to_clear &= BITMAP_LAST_WORD_MASK(size);
        *p &= ~mask_to_clear;
    }
}
EXPORT_SYMBOL(__bitmap_clear);



#endif /* bitmap_h */
