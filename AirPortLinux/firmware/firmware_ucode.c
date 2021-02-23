//
//  firmware_ucode.c
//  AirPortLinux

//  Created by Zhong-Mac on 2021/02/22.
//  Copyright © 2021 Zhong-Mac. All rights reserved.

#include <linux/firmware.h>

const struct firmware firmwares[] = {
    {FIRMWARE("iwlwifi-QuZ-a0-hr-b0-55.ucode", iwlwifi_QuZ_a0_hr_b0_55_ucode, iwlwifi_QuZ_a0_hr_b0_55_ucode_size)},
    {FIRMWARE("iwlwifi-2030-6.ucode", iwlwifi_2030_6_ucode, iwlwifi_2030_6_ucode_size)},
    {FIRMWARE("iwlwifi-9260-th-b0-jf-b0-46.ucode", iwlwifi_9260_th_b0_jf_b0_46_ucode, iwlwifi_9260_th_b0_jf_b0_46_ucode_size)},
    {FIRMWARE("iwlwifi-1000-5.ucode", iwlwifi_1000_5_ucode, iwlwifi_1000_5_ucode_size)},
    {FIRMWARE("iwlwifi-105-6.ucode", iwlwifi_105_6_ucode, iwlwifi_105_6_ucode_size)},
    {FIRMWARE("iwlwifi-6000-4.ucode", iwlwifi_6000_4_ucode, iwlwifi_6000_4_ucode_size)},
    {FIRMWARE("iwlwifi-3168-29.ucode", iwlwifi_3168_29_ucode, iwlwifi_3168_29_ucode_size)},
    {FIRMWARE("iwlwifi-cc-a0-55.ucode", iwlwifi_cc_a0_55_ucode, iwlwifi_cc_a0_55_ucode_size)},
    {FIRMWARE("iwlwifi-6000g2a-6.ucode", iwlwifi_6000g2a_6_ucode, iwlwifi_6000g2a_6_ucode_size)},
    {FIRMWARE("iwlwifi-QuZ-a0-jf-b0-55.ucode", iwlwifi_QuZ_a0_jf_b0_55_ucode, iwlwifi_QuZ_a0_jf_b0_55_ucode_size)},
    {FIRMWARE("iwlwifi-8000C-36.ucode", iwlwifi_8000C_36_ucode, iwlwifi_8000C_36_ucode_size)},
    {FIRMWARE("iwlwifi-8265-36.ucode", iwlwifi_8265_36_ucode, iwlwifi_8265_36_ucode_size)},
    {FIRMWARE("iwlwifi-6000g2b-6.ucode", iwlwifi_6000g2b_6_ucode, iwlwifi_6000g2b_6_ucode_size)},
    {FIRMWARE("iwlwifi-7265-17.ucode", iwlwifi_7265_17_ucode, iwlwifi_7265_17_ucode_size)},
    {FIRMWARE("iwlwifi-5150-2.ucode", iwlwifi_5150_2_ucode, iwlwifi_5150_2_ucode_size)},
    {FIRMWARE("iwlwifi-7265D-29.ucode", iwlwifi_7265D_29_ucode, iwlwifi_7265D_29_ucode_size)},
    {FIRMWARE("iwlwifi-4965-2.ucode", iwlwifi_4965_2_ucode, iwlwifi_4965_2_ucode_size)},
    {FIRMWARE("iwlwifi-5000-5.ucode", iwlwifi_5000_5_ucode, iwlwifi_5000_5_ucode_size)},
    {FIRMWARE("iwlwifi-100-5.ucode", iwlwifi_100_5_ucode, iwlwifi_100_5_ucode_size)},
    {FIRMWARE("iwlwifi-Qu-b0-jf-b0-55.ucode", iwlwifi_Qu_b0_jf_b0_55_ucode, iwlwifi_Qu_b0_jf_b0_55_ucode_size)},
    {FIRMWARE("iwlwifi-135-6.ucode", iwlwifi_135_6_ucode, iwlwifi_135_6_ucode_size)},
    {FIRMWARE("iwlwifi-Qu-b0-hr-b0-55.ucode", iwlwifi_Qu_b0_hr_b0_55_ucode, iwlwifi_Qu_b0_hr_b0_55_ucode_size)},
    {FIRMWARE("iwlwifi-6050-5.ucode", iwlwifi_6050_5_ucode, iwlwifi_6050_5_ucode_size)},
    {FIRMWARE("iwlwifi-3945-2.ucode", iwlwifi_3945_2_ucode, iwlwifi_3945_2_ucode_size)},
    {FIRMWARE("iwlwifi-7260-17.ucode", iwlwifi_7260_17_ucode, iwlwifi_7260_17_ucode_size)},
    {FIRMWARE("iwlwifi-2000-6.ucode", iwlwifi_2000_6_ucode, iwlwifi_2000_6_ucode_size)},
    {FIRMWARE("iwlwifi-Qu-c0-hr-b0-55.ucode", iwlwifi_Qu_c0_hr_b0_55_ucode, iwlwifi_Qu_c0_hr_b0_55_ucode_size)},
    {FIRMWARE("iwlwifi-3160-17.ucode", iwlwifi_3160_17_ucode, iwlwifi_3160_17_ucode_size)},
    {FIRMWARE("iwlwifi-9000-pu-b0-jf-b0-46.ucode", iwlwifi_9000_pu_b0_jf_b0_46_ucode, iwlwifi_9000_pu_b0_jf_b0_46_ucode_size)},
    {FIRMWARE("iwlwifi-Qu-c0-jf-b0-55.ucode", iwlwifi_Qu_c0_jf_b0_55_ucode, iwlwifi_Qu_c0_jf_b0_55_ucode_size)},
};
const int firmwares_total = 30;
