//
//  _string.c
//  AirPortLinux
//
//  Created by User-Mac on 2020/12/29.
//  Copyright © 2020 User-Mac. All rights reserved.
//

#include <linux/_string.h>

void *memchr(const void *s, int c, size_t count)
{
    const unsigned char *p = (const unsigned char *)s;
    
    while (count--)
        if ((unsigned char)c == *p++)
            return (void *)(p - 1);
    return NULL;
}
