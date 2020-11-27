//
//  crc32table.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/6/27.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef crc32table_h
#define crc32table_h

#define ENTRIES_PER_LINE 4

#if CRC_LE_BITS > 8
# define LE_TABLE_ROWS (CRC_LE_BITS/8)
# define LE_TABLE_SIZE 256
#else
# define LE_TABLE_ROWS 1
# define LE_TABLE_SIZE (1 << CRC_LE_BITS)
#endif

#if CRC_BE_BITS > 8
# define BE_TABLE_ROWS (CRC_BE_BITS/8)
# define BE_TABLE_SIZE 256
#else
# define BE_TABLE_ROWS 1
# define BE_TABLE_SIZE (1 << CRC_BE_BITS)
#endif

static uint32_t crc32table_le[LE_TABLE_ROWS][256];
static uint32_t crc32table_be[BE_TABLE_ROWS][256];
static uint32_t crc32ctable_le[LE_TABLE_ROWS][256];

#endif /* crc32table_h */
