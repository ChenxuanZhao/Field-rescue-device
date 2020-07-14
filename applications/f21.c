/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-12     Chenxuan     first version
 */

#include "f21.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

static float latitude = 3126.0409;   /*< 纬度 */
static float longitude = 12074.6826; /*< 经度 */

static char phone_base[][12] = {{"13771731568"}, {"18006201663"}};

#define PHONE_BASE_SIZE (sizeof(phone_base) / sizeof(phone_base[0]))

static rt_mutex_t f21_lock = RT_NULL;

bool f21_set_faker_position(float _latitude, float _longitude)
{
    latitude = _latitude;
    longitude = _longitude;
    return true;
}

bool f21_set_phone_num(uint8_t cnt, const char *phone_num)
{
    if (cnt >= 2 || phone_num == RT_NULL || strlen(phone_num) != 11)
    {
        return false;
    }

    strncpy(phone_base[cnt], phone_num, 11);

    return true;
}

float f21_get_gps_parameter_e(void)
{
    return longitude;
}

float f21_get_gps_parameter_n(void)
{
    return latitude;
}

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
    if (at_exec_cmd(resp, "AT+QGNSSC=1") < 0)
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
    const char *p = "AT+QGNSSRD=\"NMEA/RMC\"";
    at_client_send(p, strlen(p));
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
    if (at_exec_cmd(resp, cmd_expr) < 0)
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

rt_err_t f21_send_message(uint8_t phone_cnt, const char *msg)
{
    static rt_err_t err;

    if (msg == RT_NULL || phone_cnt >= PHONE_BASE_SIZE)
    {
        return RT_ERROR;
    }
    rt_mutex_take(f21_lock, RT_WAITING_FOREVER);
    char p[22] = {0};
    err = f21_cmd_base("AT+CMGF=1", "OK");
    f21_cmd_base("AT+CSCS=\"GSM\"", "OK");
    strcat(p, "AT+CMGS=\"");
    strcat(p, phone_base[phone_cnt]);
    strcat(p, "\"");
    f21_cmd_base(p, ">");
    err = at_client_send(msg, strlen(msg));
    char eof = 0x1A;
    err = at_client_send(&eof, 1);
    rt_mutex_release(f21_lock);

    return err;
}

rt_err_t f21_phone_call(uint8_t phone_cnt)
{
    if (phone_cnt >= PHONE_BASE_SIZE)
    {
        return RT_ERROR;
    }
    char buf[16] = {0};
    strcat(buf, "ATD");
    strcat(buf, phone_base[phone_cnt]);
    strcat(buf, ";");
    rt_kprintf("send: %s\n", buf);
    f21_cmd_base(buf, "OK");
    return RT_EOK;
}

rt_err_t f21_delete_message(void)
{
    rt_err_t err;

    rt_mutex_take(f21_lock, RT_WAITING_FOREVER);
    err = f21_cmd_base("AT+CPMS?", "OK");
    err = f21_cmd_base("AT+CMGL=\"ALL\"", "OK");
    err = f21_cmd_base("AT+CMGD=0", "OK");
    err = f21_cmd_base("AT+CMGF=1", "OK");
    rt_mutex_release(f21_lock);

    return err;
}

RT_WEAK void f21_urc_ring_callback(struct at_client *client, const char *data, rt_size_t size)
{
    /* 有电话 */
}

RT_WEAK void f21_urc_recv_callback(struct at_client *client, const char *data, rt_size_t size)
{
    /* 有短信 */
}

RT_WEAK void f21_urc_gps_callback(struct at_client *client, const char *data, rt_size_t size)
{
    /* 定位信息 */
}

static struct at_urc urc_table[] = {
    {"RING", "\r\n", f21_urc_ring_callback},
    {"+CMT", "\r\n", f21_urc_recv_callback},
    {"$GNRMC", "E,", f21_urc_gps_callback}};

int at_client_port_init(void)
{
    /* 添加多种 URC 数据至 URC 列表中，当接收到同时匹配 URC 前缀和后缀的数据，执行 URC 函数  */
    at_set_urc_table(urc_table, sizeof(urc_table) / sizeof(urc_table[0]));
    return RT_EOK;
}

static int f21_hw_init(void)
{
    f21_cmd_base("AT&F", "OK");
    /* 注册模块 */
    f21_register();
    /* 开启 GPS定位 */
    f21_set_position();
    at_client_port_init();
    f21_cmd_base("AT+CSQ", "OK");
    f21_cmd_base("AT+CPIN?", "OK");
    f21_cmd_base("AT+COPS?", "OK");
    f21_cmd_base("AT+CMGF=1", "OK");
    f21_cmd_base("AT+CSMP=17,167,0,8", "OK");
    f21_lock = rt_mutex_create("f21_lock", RT_IPC_FLAG_FIFO);
    if (f21_lock == RT_NULL)
    {
        rt_kprintf("create f21_lock mutex failed.\n");
        return RT_ERROR;
    }
    f21_delete_message();

    return RT_EOK;
}
INIT_APP_EXPORT(f21_hw_init);
