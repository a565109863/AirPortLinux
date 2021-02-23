//
//  firmware.c
//  AirPortLinux
//
//  Created by Zhong-MAC on 2021/2/22.
//  Copyright © 2021 User-Mac. All rights reserved.
//

#include <linux/capability.h>
#include <linux/device.h>
//#include <linux/kernel_read_file.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/bitops.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/highmem.h>
#include <linux/firmware.h>
#include <linux/slab.h>
#include <linux/sched.h>
//#include <linux/file.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/pm.h>

/* Async support */
struct firmware_work {
    struct work_struct work;
    struct module *module;
    const char *name;
    struct device *device;
    void *context;
    void (*cont)(const struct firmware *fw, void *context);
    u32 opt_flags;
};

/* called from request_firmware() and request_firmware_work_func() */
static int
_request_firmware(const struct firmware **firmware_p, const char *name,
                  struct device *device, void *buf, size_t size,
                  size_t offset, u32 opt_flags)
{
    struct firmware *fw = NULL;
    int ret = 1;
    
    for (int i = 0; i < firmwares_total; i++) {
        if (strcmp(firmwares[i].name, name) == 0) {
            fw = (struct firmware *)vmalloc(sizeof(struct firmware));
            fw->name = firmwares[i].name;
            fw->size = firmwares[i].size;
            fw->data = (const u8 *)vmalloc(fw->size);
            memcpy((void*)fw->data, firmwares[i].data, fw->size);
            
            ret = 0;
            break;
        }
    }
    
    *firmware_p = fw;
    return ret;
}

static void request_firmware_work_func(struct work_struct *work)
{
    struct firmware_work *fw_work;
    const struct firmware *fw;
    
    fw_work = container_of(work, struct firmware_work, work);
    
    _request_firmware(&fw, fw_work->name, fw_work->device, NULL, 0, 0,
                      fw_work->opt_flags);
    fw_work->cont(fw, fw_work->context);
    //    put_device(fw_work->device); /* taken in request_firmware_nowait() */
    
    module_put(fw_work->module);
    //    kfree_const(fw_work->name);
    kfree(fw_work);
}

void firmwareLoadComplete(OSKextRequestTag requestTag, OSReturn result,
                          const void *resourceData,
                          uint32_t resourceDataLength,
                          void *context) {
    
    //    OSData *firmwareData = OSData::withBytes(resourceData, resourceDataLength);
    struct firmware* fw = (struct firmware *)vmalloc(sizeof(struct firmware));
    if(result == kOSReturnSuccess) {
        fw->size = resourceDataLength;
        fw->data = (const u8 *)vmalloc(fw->size);
        memcpy((void*)fw->data, resourceData, fw->size);
    }
    
    //    kprintf("--%s: line = %d, fw->size = %lu", __FUNCTION__, __LINE__, fw->size);
    struct firmware_work *fw_work = (struct firmware_work *)context;
    fw_work->cont(fw, context);
}

int request_firmware_nowait(
                            struct module *module, bool uevent,
                            const char *name, struct device *device, gfp_t gfp, void *context,
                            void (*cont)(const struct firmware *fw, void *context))
{
    struct firmware_work *fw_work;
    
    fw_work = (typeof fw_work)kzalloc(sizeof(struct firmware_work), gfp);
    if (!fw_work)
        return -ENOMEM;
    
    fw_work->module = module;
    fw_work->name = name;
    if (!fw_work->name) {
        kfree(fw_work);
        return -ENOMEM;
    }
    fw_work->device = device;
    fw_work->context = context;
    fw_work->cont = cont;
//   fw_work->opt_flags = FW_OPT_NOWAIT |
//       (uevent ? FW_OPT_UEVENT : FW_OPT_USERHELPER);
//
//   if (!uevent && fw_cache_is_setup(device, name)) {
//       kfree_const(fw_work->name);
//       kfree(fw_work);
//       return -EOPNOTSUPP;
//   }
//
//   if (!try_module_get(module)) {
//       kfree_const(fw_work->name);
//       kfree(fw_work);
//       return -EFAULT;
//   }
//
//   get_device(fw_work->device);
    INIT_WORK(&fw_work->work, request_firmware_work_func);
    schedule_work(&fw_work->work);
    return 0;
    
//    OSReturn ret = OSKextRequestResource(OSKextGetCurrentIdentifier(),
//                                          name,
//                                          firmwareLoadComplete,
//                                          fw_work,
//                                          NULL);
//    if (ret != kIOReturnSuccess) {
//        return 1;
//    }
//    return 0;

}
EXPORT_SYMBOL(request_firmware_nowait);

void release_firmware(const struct firmware *fw)
{
    if(fw)
        free((void *)fw);
}
