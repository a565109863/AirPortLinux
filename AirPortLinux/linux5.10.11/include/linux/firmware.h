//* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FIRMWARE_H
#define _LINUX_FIRMWARE_H

#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/gfp.h>

#include "firmware_ucode.h"

struct firmware {
    const char *name;
    size_t size;
    const u8 *data;
//    struct page **pages;
//
//    /* firmware loader private fields */
//    void *priv;
};

#define FIRMWARE(_name, _data, _size) \
    .name = _name, .data = _data, .size = _size

static int request_firmware(const struct firmware **fw, const char *name,
struct device *device)
{
    return 0;
}

static int firmware_request_nowarn(const struct firmware **fw, const char *name,
struct device *device)
{
    return -EIO;
}

int request_firmware_nowait(
    struct module *module, bool uevent,
    const char *name, struct device *device, gfp_t gfp, void *context,
    void (*cont)(const struct firmware *fw, void *context));

void release_firmware(const struct firmware *fw);

#endif /* linux_firmware_h */
