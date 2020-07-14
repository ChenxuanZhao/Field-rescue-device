/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-09     Chenxuan     first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include <stdio.h>
#include <stdbool.h>

#include "alarm.h"
#include "flash.h"
#include "f21.h"
#include "key.h"

/* defined the LED0 pin: PB2 */
#define LED0_PIN GET_PIN(B, 2)

static rt_sem_t call_sem = RT_NULL;

/**
 * @brief 主函数
 * 
 * @return int 
 */
int main(int argc, const char *argv[])
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    call_sem = rt_sem_create("call_sem", 0, RT_IPC_FLAG_FIFO);
    if (call_sem == RT_NULL)
    {
        rt_kprintf("create call_sem semaphore failed.\n");
    }
    buzzer_on();
    rt_thread_mdelay(200);
    buzzer_off();
    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

/**
 * @brief 按键 1 回调函数 (声光报警)
 * 
 * @param btn 按键状态
 */
void btn_1_callback(flex_button_t *btn)
{
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_CLICK:
        sos_delete();
        break;
    case FLEX_BTN_PRESS_LONG_START:
        sos_startup();
        break;
    default:
        break;
    }
}

/**
 * @brief 按键 2 回调函数 (发送求救短信)
 * 
 * @param btn 按键状态
 */
void btn_2_callback(flex_button_t *btn)
{
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_LONG_START:
        f21_refresh_position();
        rt_thread_mdelay(100);
        char buf[128] = {0};
        sprintf(buf, "S.O.S.\nposition:\n%.4f E\n%.4f N", f21_get_gps_parameter_e(), f21_get_gps_parameter_n());
        f21_send_message(0, buf);
        f21_send_message(1, buf);
        rt_kprintf("发送求救短信\n");
        break;
    default:
        break;
    }
}

/**
 * @brief 按键 3 回调函数 (拨打电话 1 / 挂断)
 * 
 * @param btn 按键状态
 */
void btn_3_callback(flex_button_t *btn)
{
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_CLICK:
        if (rt_sem_trytake(call_sem) == RT_EOK)
        {
            f21_cmd_base("ATA", "OK");
            rt_kprintf("接电话\n");
            mute_off();
            while (rt_sem_trytake(call_sem) == RT_EOK)
            {
                ;
            }
        }
        else
        {
            f21_cmd_base("ATH", "OK");
            rt_kprintf("挂断电话\n");
            mute_on();
        }

        break;
    case FLEX_BTN_PRESS_LONG_HOLD:
        mute_off();
        f21_phone_call(0);
        break;
    default:
        break;
    }
}

/**
 * @brief 按键 4 回调函数 (拨打电话 2 / 挂断)
 * 
 * @param btn 按键状态
 */
void btn_4_callback(flex_button_t *btn)
{
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_CLICK:
        if (rt_sem_trytake(call_sem) == RT_EOK)
        {
            f21_cmd_base("ATA", "OK");
            rt_kprintf("接电话\n");
            mute_off();
            while (rt_sem_trytake(call_sem) == RT_EOK)
            {
                ;
            }
        }
        else
        {
            f21_cmd_base("ATH", "OK");
            rt_kprintf("挂断电话\n");
            mute_on();
        }

        break;
    case FLEX_BTN_PRESS_LONG_HOLD:
        mute_off();
        f21_phone_call(1);
        break;
    default:
        break;
    }
}

/**
 * @brief 有电话
 */
void f21_urc_ring_callback(struct at_client *client, const char *data, rt_size_t size)
{
    mute_off();
    rt_sem_release(call_sem);
}

/**
 * @brief 有短信
 */
void f21_urc_recv_callback(struct at_client *client, const char *data, rt_size_t size)
{
    char *p = RT_NULL;
    p = rt_strstr(data, "set phone");
    if (p != RT_NULL)
    {
        char buff[12] = {0};
        int cnt = 0;
        sscanf(p, "set phone%d: %s", &cnt, buff);
        if (1 <= cnt && cnt <= 2)
        {
            f21_set_phone_num(cnt - 1, buff);
            strncpy(sys_env.env.phone_num[cnt - 1], buff, 11);
            flash_save(sys_env);
            rt_kprintf("保存环境参数\n");
            char buf[100] = {0};
            sprintf(buf, "Emergency Contact: %d, has been changed to %s.", cnt, buff);
            f21_send_message(0, buf);
            f21_send_message(1, buf);
        }
        return;
    }
    p = rt_strstr(data, "set position");
    if (p != RT_NULL)
    {
        float _longitude = 0; /* 经度 */
        float _latitude = 0;  /* 纬度 */
        sscanf(p, "set position: %f,%f", &_longitude, &_latitude);
        sys_env.env.longitude = _longitude;
        sys_env.env.latitude = _latitude;
        flash_save(sys_env);
        f21_set_faker_position(_latitude, _longitude);
    }
    f21_delete_message();
}

/**
 * @brief 有定位信息
 */
void f21_urc_gps_callback(struct at_client *client, const char *data, rt_size_t size)
{
    /* 解析定位信息 */
    float tick = 0;
    char ch = 0;
    float _longitude = 0; /* 经度 */
    float _latitude = 0;  /* 纬度 */
    sscanf(data, "$GNRMC,%f,%c,%f,N,%f,E,", &tick, &ch, &_latitude, &_longitude);
    if (ch != 'A')
    {
        rt_kprintf("没有获取到卫星信号!\n");
    }
    else
    {
        /* 保存坐标信息 */
        sys_env.env.longitude = _longitude;
        sys_env.env.latitude = _latitude;
        flash_save(sys_env);
        f21_set_faker_position(_latitude, _longitude);
    }
}
