//
//  vmalloc.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef vmalloc_h
#define vmalloc_h

#include <linux/_malloc.h>

#define vmalloc(size) malloc(size)

#endif /* vmalloc_h */
