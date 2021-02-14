//
//  random.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/27.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef random_h
#define random_h

#include <linux/types.h>
#include <sys/random.h>

static u32 get_random_u32(void)
{
    u_int32_t ret;
    read_random(&ret, sizeof(ret));
    return ret;
}


#endif /* random_h */
