//
//  firmware.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef firmware_h
#define firmware_h

#include <linux/types.h>
#include <linux/gfp.h>

struct firmware {
    size_t size;
    const u8 *data;
//    struct page **pages;

    /* firmware loader private fields */
    void *priv;
};

static int request_firmware(const struct firmware **fw, const char *name,
struct device *device)
{
    return 0;
}

static int firmware_request_nowarn(const struct firmware **fw, const char *name,
struct device *device)
{
    return 0;
}

int request_firmware_nowait(
void *, bool uevent,
const char *name, struct device *device, gfp_t gfp, void *context,
void (*cont)(const struct firmware *fw, void *context));

static void release_firmware(const struct firmware *fw)
{
    if( fw )
    IOFree((void *)fw, sizeof(struct firmware));
}

#endif /* firmware_h */
