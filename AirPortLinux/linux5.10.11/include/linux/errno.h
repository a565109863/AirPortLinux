//
//  errno.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/7/12.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef errno_h
#define errno_h

#define NOTIFY_DONE        0x0000        /* Don't care */
#define NOTIFY_OK        0x0001        /* Suits me */
#define NOTIFY_STOP_MASK    0x8000        /* Don't call further */
#define NOTIFY_BAD        (NOTIFY_STOP_MASK|0x0002)
                        /* Bad/Veto action */
/*
 * Clean way to return from the notifier and stop further calls.
 */
#define NOTIFY_STOP        (NOTIFY_OK|NOTIFY_STOP_MASK)

#endif /* errno_h */
