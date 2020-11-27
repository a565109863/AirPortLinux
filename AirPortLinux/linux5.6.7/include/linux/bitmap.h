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

#endif /* bitmap_h */
