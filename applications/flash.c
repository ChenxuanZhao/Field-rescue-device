/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-11     Chenxuan     first version
 */

#include "flash.h"

#include <string.h>
#include <rtthread.h>

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define CHECK_SUM 233

union flash_backup sys_env;

int rt_hw_flash_init(void)
{
    fal_init();
    fal_partition_t flash_param = (fal_partition_t)fal_partition_find("param");
    if (flash_param == RT_NULL)
    {
        rt_kprintf("No partitions found\n");
        return RT_ERROR;
    }
    // 读取环境变量
    union flash_backup tmp = {0};
    fal_partition_read(flash_param, 0, tmp.index, ARR_SIZE(tmp.index));
    rt_kprintf("Phone1: %s\n", tmp.env.phone_num[0]);
    rt_kprintf("Phone2: %s\n", tmp.env.phone_num[1]);
    rt_kprintf("latitude: %d\n", (int)tmp.env.latitude);
    rt_kprintf("longitude: %d\n", (int)tmp.env.longitude);
    rt_kprintf("checksum: %d\n", tmp.env.checksum);
    if (tmp.env.checksum != CHECK_SUM)
    {
        // 校验和出错, 恢复默认设置
        fal_partition_erase(flash_param, 0, ARR_SIZE(sys_env.index));
        sys_env.env.checksum = CHECK_SUM;
        sys_env.env.latitude = 3126.0409;
        sys_env.env.longitude = 12074.6826;
        strncpy(sys_env.env.phone_num[0], "13771731568", strlen("13771731568"));
        strncpy(sys_env.env.phone_num[1], "18006201663", strlen("18006201663"));
        fal_partition_write(flash_param, 0, sys_env.index, ARR_SIZE(sys_env.index));
    }
    else
    {
        sys_env = tmp;
    }
    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_flash_init);

int flash_save(union flash_backup env)
{
    fal_partition_t flash_param = (fal_partition_t)fal_partition_find("param");
    if (flash_param == RT_NULL)
    {
        rt_kprintf("No partitions found\n");
        return RT_ERROR;
    }
    fal_partition_erase(flash_param, 0, ARR_SIZE(env.index));
    env.env.checksum = CHECK_SUM;
    fal_partition_write(flash_param, 0, env.index, ARR_SIZE(env.index));
    return RT_EOK;
}
