//
//  module.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef module_h
#define module_h

#define request_module kprintf

#define module_param(x, y, z)
#define MODULE_PARM_DESC(x, y)

#define MODULE_DESCRIPTION(x);
#define MODULE_AUTHOR(x)
#define MODULE_LICENSE(x)
#define MODULE_SOFTDEP(x)

#define EXPORT_TRACEPOINT_SYMBOL(x)


//#define ___PASTE(a,b) a##b
//#define __PASTE(a,b) ___PASTE(a,b)
//
//#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)
//
//
//#define __MODULE_INFO(tag, name, info)                      \
//static const char __UNIQUE_ID(name)[]                      \
//  __used __attribute__((section(".modinfo"), unused, aligned(1)))      \
//  = __MODULE_INFO_PREFIX __stringify(tag) "=" info
//
//
///* Generic info of form tag = "info" */
//#define MODULE_INFO(tag, info) __MODULE_INFO(tag, tag, info)
//
//#define MODULE_FIRMWARE(_firmware) MODULE_INFO(firmware, _firmware)
#define MODULE_FIRMWARE(_firmware)


#define module_init(iwl_mvm_init)
#define module_exit(iwl_mvm_exit)
#define module_put(x)

#define KBUILD_MODNAME "intelwifi"
#define THIS_MODULE NULL


#endif /* module_h */
