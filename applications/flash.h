/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-11     Chenxuan     first version
 */

#ifndef __FLASH_H__
#define __FLASH_H__

#include "fal.h"
#include <stdint.h>

union flash_backup
{
    uint8_t index[128];
    struct
    {
        char phone_num[2][12];
        float latitude;
        float longitude;
        int checksum;
    } env;
};

extern union flash_backup sys_env;

#ifdef __cplusplus
extern "C"
{
#endif

extern int rt_hw_flash_init(void);
extern int flash_save(union flash_backup env);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H__ */
