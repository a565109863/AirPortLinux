#!/bin/sh

#  fw2bin.sh
#  AirPortLinux
#
#  Created by Zhong-Mac on 2021/02/22.
#  Copyright © 2021 Zhong-Mac. All rights reserved.

target_path="${PROJECT_DIR}/AirPortLinux/firmware/"
fw_files=$(find "${PROJECT_DIR}/AirPortLinux/sources/firmware" ! -name ".*" -maxdepth 1 -type f | tr " " "\?")

for fw in ${fw_files}; do
    fw_file_name=`basename $(echo ${fw} | tr " " "-") `
    
    fw_var_name=${fw_file_name//./_}
    fw_var_name=${fw_var_name//-/_}
    target_file="${target_path}${fw_var_name}.c"
    rm -rf $target_file
    echo "//\n//  ${fw_var_name}.c\n//  AirPortLinux\n\n//  Created by Zhong-Mac on 2021/02/22.\n//  Copyright © 2021 Zhong-Mac. All rights reserved." >$target_file
    
    echo "">>$target_file
    echo "const unsigned char ${fw_var_name}[] = {">>$target_file
    xxd -i <$fw >>$target_file
    echo "};">>$target_file
    echo "">>$target_file
    echo "const unsigned int ${fw_var_name}_size = sizeof(${fw_var_name});">>$target_file
done


target_file="${target_path}firmware_ucode.h"

rm -rf $target_file

echo "">>$target_file
echo "//\n//  firmwarevar.h\n//  AirPortLinux\n\n//  Created by Zhong-Mac on 2021/02/22.\n//  Copyright © 2021 Zhong-Mac. All rights reserved." >$target_file
echo "">>$target_file
echo "#ifndef firmwarevar_h\n#define firmwarevar_h">>$target_file
echo "">>$target_file

echo "extern const struct firmware firmwares[];">>$target_file
echo "extern const int firmwares_total;">>$target_file

for fw in ${fw_files}; do
    fw_file_name=`basename $(echo ${fw} | tr " " "-") `
    
    fw_var_name=${fw_file_name//./_}
    fw_var_name=${fw_var_name//-/_}
    
    echo "">>$target_file
    echo "extern const unsigned char ${fw_var_name}[];">>$target_file
    echo "extern const unsigned int ${fw_var_name}_size;">>$target_file
done

echo "">>$target_file
echo "#endif">>$target_file


target_file="${target_path}firmware_ucode.c"

rm -rf $target_file

echo "">>$target_file
echo "//\n//  firmware_ucode.c\n//  AirPortLinux\n\n//  Created by Zhong-Mac on 2021/02/22.\n//  Copyright © 2021 Zhong-Mac. All rights reserved." >$target_file
echo "\n#include <linux/firmware.h>">>$target_file


echo "">>$target_file
echo "const struct firmware firmwares[] = {">>$target_file
i=0;
for fw in $fw_files; do
    fw_file_name=`basename $(echo ${fw} | tr " " "-") `
    fw_var_name=${fw_file_name//./_}
    fw_var_name=${fw_var_name//-/_}
    echo "    {FIRMWARE(\"$fw_file_name\", $fw_var_name, ${fw_var_name}_size)},">>$target_file
    let i+=1
done
echo "};">>$target_file
echo "const int firmwares_total = $i;">>$target_file
