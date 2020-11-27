//
//  string.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef string_h
#define string_h

#include <linux/kernel.h>

static int match_string(const char * const *array, size_t n, const char *string)
{
    int index;
    const char *item;

    for (index = 0; index < n; index++) {
        item = array[index];
        if (!item)
            break;
        if (!strcmp(item, string))
            return index;
    }

    return -EINVAL;
}

static inline char tolower(char x) {
    if (x >= 'A' && x <= 'Z')
        return x - 'A' + 'a';
    return x;
}

static inline char toupper(char x) {
    if (x >= 'a' && x <= 'z')
        return x - 'a' + 'A';
    return x;
}

static int __hex2bin(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - '0';
    ch = tolower(ch);
    if ((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    return -1;
}

static int hex2bin(uint8_t *dst, const char *src, size_t count)
{
    while (count--) {
        int hi = __hex2bin(*src++);
        int lo = __hex2bin(*src++);

        if ((hi < 0) || (lo < 0))
            return -1;
        *dst++ = (hi << 4) | lo;
    }
    return 0;
}

#endif /* string_h */
