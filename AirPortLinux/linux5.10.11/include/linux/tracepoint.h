//
//  tracepoint.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef tracepoint_h
#define tracepoint_h

#define TP_printk(fmt, args...) fmt "\n", args

#define TP_PROTO(args...)    args
#define TP_ARGS(args...)    args
#define TP_STRUCT__entry(args...) args
#define TP_fast_assign(args...) args
#define TP_CONDITION(args...)    args


#endif /* tracepoint_h */
