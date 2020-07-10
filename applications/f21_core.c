/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-08     Chenxuan     first version
 */

#include "f21_core.h"

#include <rtthread.h>
#include <at.h>
#include <finsh.h>

static float latitude = 0;  /*< 经度 */
static float longitude = 0; /*< 纬度 */

static int f21_register(void)
{
    at_client_init(AT_SERVER_DEVICE, AT_SERVER_RECV_BUFF_LEN);
    /* 等待模块初始化完成 */
    at_client_wait_connect(RT_WAITING_FOREVER);

    return RT_EOK;
}

static int f21_set_position(void)
{
    /* 创建响应结构体，设置最大支持响应数据长度为 256 字节，响应数据行数无限制，超时时间为 5 秒 */
    at_response_t resp = at_create_resp(AT_SERVER_RECV_BUFF_LEN, 0, rt_tick_from_millisecond(5000));
    if (resp == RT_NULL)
    {
        rt_kprintf("\nNo memory for response structure!\n");
        return -RT_ENOMEM;
    }

    /* 发送 AT 命令并接收 AT Server 响应数据，数据及信息存放在 resp 结构体中 */
    if (at_exec_cmd(resp, "AT+QGNSSC=1") != RT_EOK)
    {
        rt_kprintf("AT client send commands failed, response error or timeout !\n");
        /* 删除响应结构体 */
        at_delete_resp(resp);
        return -RT_ERROR;
    }

    /* 删除响应结构体 */
    at_delete_resp(resp);
    return RT_EOK;
}

int f21_refresh_position(void)
{
    /* 创建响应结构体，设置最大支持响应数据长度为 256 字节，响应数据行数无限制，超时时间为 5 秒 */
    at_response_t resp = at_create_resp(AT_SERVER_RECV_BUFF_LEN, 0, rt_tick_from_millisecond(5000));
    if (resp == RT_NULL)
    {
        rt_kprintf("\nNo memory for response structure!\n");
        return -RT_ENOMEM;
    }

    /* 发送 AT 命令并接收 AT Server 响应数据，数据及信息存放在 resp 结构体中 */
    if (at_exec_cmd(resp, "AT+QGNSSRD=\"NMEA/RMC\"") != RT_EOK)
    {
        rt_kprintf("AT client send commands failed, response error or timeout !\n");
        /* 删除响应结构体 */
        at_delete_resp(resp);
        return -RT_ERROR;
    }
    /* 解析定位信息 */
    float tick = 0;
    char ch = 0;
    float _longitude = 0; /* 经度 */
    float _latitude = 0;  /* 纬度 */
    at_resp_parse_line_args_by_kw(resp, "$GNRMC", "$GNRMC,%f,%c,%f,N,%f,E,", &tick, &ch, &_latitude, &_longitude);
    if (ch != 'A')
    {
        rt_kprintf("没有获取到卫星信号!\n");
    }
    else
    {
        /* 保存坐标信息 */
        latitude = _latitude;
        longitude = _longitude;
    }

    /* 删除响应结构体 */
    at_delete_resp(resp);
    return RT_EOK;
}

int f21_cmd_base(const char *cmd_expr, const char *resp_expr)
{
    /* 创建响应结构体，设置最大支持响应数据长度为 256 字节，响应数据行数无限制，超时时间为 5 秒 */
    at_response_t resp = at_create_resp(AT_SERVER_RECV_BUFF_LEN, 0, rt_tick_from_millisecond(5000));
    if (resp == RT_NULL)
    {
        rt_kprintf("\nNo memory for response structure!\n");
        return -RT_ENOMEM;
    }

    /* 发送 AT 命令并接收 AT Server 响应数据，数据及信息存放在 resp 结构体中 */
    if (at_exec_cmd(resp, cmd_expr) != RT_EOK)
    {
        rt_kprintf("AT client send commands failed, response error or timeout !\n");
        /* 删除响应结构体 */
        at_delete_resp(resp);
        return -RT_ERROR;
    }

    /* 获取指定关键字的响应数据 */
    if (at_resp_get_line_by_kw(resp, resp_expr) == RT_NULL)
    {
        /* 删除响应结构体 */
        at_delete_resp(resp);
        return -RT_ERROR;
    }

    /* 删除响应结构体 */
    at_delete_resp(resp);
    return RT_EOK;
}

static int f21_hw_init(void)
{
    /* 注册模块 */
    f21_register();
    /* 开启 GPS定位 */
    f21_set_position();
    f21_cmd_base("AT+CMGF=1", "OK");
    f21_cmd_base("AT+CSCS=\"GSM\"", "OK");

    return RT_EOK;
}
INIT_APP_EXPORT(f21_hw_init);
