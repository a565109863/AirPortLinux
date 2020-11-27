//
//  acpi.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef acpi_h
#define acpi_h

#include <linux/types.h>

/*
 * Miscellaneous types
 */
typedef u32 acpi_status;    /* All ACPI Exceptions */
typedef u32 acpi_name;        /* 4-byte ACPI name */
typedef char *acpi_string;    /* Null terminated ASCII string */
typedef void *acpi_handle;    /* Actually a ptr to a NS Node */

#endif /* acpi_h */
