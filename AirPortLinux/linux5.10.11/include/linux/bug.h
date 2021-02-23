//
//  bug.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef bug_h
#define bug_h


#define DebugLogSleep(x, args...) \
if(1) { \
    thread_t new_thread = current_thread(); \
    uint64_t new_thread_id = thread_tid(new_thread); \
    kprintf(x " tid = %llu", args, new_thread_id); \
    IOSleep(1000); \
}

#define DebugLog(x, args...) \
if(1) { \
    thread_t new_thread = current_thread(); \
    uint64_t new_thread_id = thread_tid(new_thread); \
    kprintf(x " tid = %llu", args, new_thread_id); \
}


#endif /* bug_h */
