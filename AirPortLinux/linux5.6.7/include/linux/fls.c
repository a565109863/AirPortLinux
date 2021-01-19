//
//  fls.c
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/27.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#include <linux/bitops.h>


int fls(int x)
{
    return fls64(x);
}
