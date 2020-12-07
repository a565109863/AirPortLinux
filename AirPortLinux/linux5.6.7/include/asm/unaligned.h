//
//  unaligned.h
//  AppleIntelWiFi
//
//  Created by Zhong-Mac on 2020/4/28.
//  Copyright © 2020 Zhong-Mac. All rights reserved.
//

#ifndef unaligned_h
#define unaligned_h

#include <asm/types.h>

#define __swab32 swab32
#define __swab64 swab64

static inline u16 get_unaligned_le16(const void *p)
{
    return le16_to_cpup((__le16 *)p);
}

static inline u32 get_unaligned_le32(const void *p)
{
    return le32_to_cpup((__le32 *)p);
}

static inline u64 get_unaligned_le64(const void *p)
{
    return le64_to_cpup((__le64 *)p);
}

static inline u16 get_unaligned_be16(const void *p)
{
    return be16_to_cpup((__be16 *)p);
}

static inline u32 get_unaligned_be32(const void *p)
{
    return be32_to_cpup((__be32 *)p);
}

static inline u64 get_unaligned_be64(const void *p)
{
    return be64_to_cpup((__be64 *)p);
}

static inline void put_unaligned_le16(u16 val, void *p)
{
    *((__le16 *)p) = cpu_to_le16(val);
}

static inline void put_unaligned_le32(u32 val, void *p)
{
    *((__le32 *)p) = cpu_to_le32(val);
}

static inline void put_unaligned_le64(u64 val, void *p)
{
    *((__le64 *)p) = cpu_to_le64(val);
}

static inline void put_unaligned_be16(u16 val, void *p)
{
    *((__be16 *)p) = cpu_to_be16(val);
}

static inline void put_unaligned_be32(u32 val, void *p)
{
    *((__be32 *)p) = cpu_to_be32(val);
}

static inline void put_unaligned_be64(u64 val, void *p)
{
    *((__be64 *)p) = cpu_to_be64(val);
}

//
//
///*
// * The C64x+ can do unaligned word and dword accesses in hardware
// * using special load/store instructions.
// */
//
//static inline u16 get_unaligned_le16(const void *p)
//{
//    const u8 *_p = (const u8 *)p;
//    return _p[0] | _p[1] << 8;
//}
//
//static inline u16 get_unaligned_be16(const void *p)
//{
//    const u8 *_p = (const u8 *)p;
//    return _p[0] << 8 | _p[1];
//}
//
//static inline void put_unaligned_le16(u16 val, void *p)
//{
//    u8 *_p = (u8 *)p;
//    _p[0] = val;
//    _p[1] = val >> 8;
//}
//
//static inline void put_unaligned_be16(u16 val, void *p)
//{
//    u8 *_p = (u8 *)p;
//    _p[0] = val >> 8;
//    _p[1] = val;
//}

static inline u32 get_unaligned32(const void *p)
{
    const u32 *val = (const u32 *) p;
//    asm (" ldnw    .d1t1    *%0,%0\n"
//         " nop     4\n"
//         : "+a"(val));
    return *val;
}

static inline void put_unaligned32(u32 val, void *p)
{
//    asm volatile (" stnw    .d2t1    %0,*%1\n"
//              : : "a"(val), "b"(p) : "memory");
}

static inline u64 get_unaligned64(const void *p)
{
    u64 val;
//    asm volatile (" ldndw    .d1t1    *%1,%0\n"
//              " nop     4\n"
//              : "=a"(val) : "a"(p));
    return val;
}

static inline void put_unaligned64(u64 val, const void *p)
{
//    asm volatile (" stndw    .d2t1    %0,*%1\n"
//              : : "a"(val), "b"(p) : "memory");
}

static inline u32 get_unaligned_le32(const u8 *p)
{
        return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}


#ifdef CONFIG_CPU_BIG_ENDIAN

//#define get_unaligned_le32(p)     __swab32(get_unaligned32(p))
#define get_unaligned_le64(p)     __swab64(get_unaligned64(p))
#define get_unaligned_be32(p)     get_unaligned32(p)
#define get_unaligned_be64(p)     get_unaligned64(p)
#define put_unaligned_le32(v, p) put_unaligned32(__swab32(v), (p))
#define put_unaligned_le64(v, p) put_unaligned64(__swab64(v), (p))
#define put_unaligned_be32(v, p) put_unaligned32((v), (p))
#define put_unaligned_be64(v, p) put_unaligned64((v), (p))
#define get_unaligned    __get_unaligned_be
#define put_unaligned    __put_unaligned_be

#else

//#define get_unaligned_le32(p)     get_unaligned32(p)
#define get_unaligned_le64(p)     get_unaligned64(p)
#define get_unaligned_be32(p)     __swab32(get_unaligned32(p))
#define get_unaligned_be64(p)     __swab64(get_unaligned64(p))
#define put_unaligned_le32(v, p) put_unaligned32((v), (p))
#define put_unaligned_le64(v, p) put_unaligned64((v), (p))
#define put_unaligned_be32(v, p) put_unaligned32(__swab32(v), (p))
#define put_unaligned_be64(v, p) put_unaligned64(__swab64(v), (p))
#define get_unaligned    __get_unaligned_le
#define put_unaligned    __put_unaligned_le

#endif

#define __bad_unaligned_access_size(void)


#define __put_unaligned_le(val, ptr) ({                    \
    void *__gu_p = (ptr);                        \
    switch (sizeof(*(ptr))) {                    \
    case 1:                                \
        *(u8 *)__gu_p = (__force u8)(val);            \
        break;                            \
    case 2:                                \
        put_unaligned_le16((__force u16)(val), __gu_p);        \
        break;                            \
    case 4:                                \
        put_unaligned_le32((__force u32)(val), __gu_p);        \
        break;                            \
    case 8:                                \
        put_unaligned_le64((__force u64)(val), __gu_p);        \
        break;                            \
    default:                            \
        __bad_unaligned_access_size();                \
        break;                            \
    }                                \
    (void)0; })


#endif /* unaligned_h */
