/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-13     Chenxuan     first version
 */

#include "alarm.h"

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_gpio.h>

#define BUZZER_PIN GET_PIN(B, 0)
#define MUTE_PIN GET_PIN(B, 1)

void buzzer_on(void)
{
    rt_pin_write(BUZZER_PIN, PIN_HIGH);
}

void buzzer_off(void)
{
    rt_pin_write(BUZZER_PIN, PIN_LOW);
}

void mute_on(void)
{
    rt_pin_write(MUTE_PIN, PIN_LOW);
}

void mute_off(void)
{
    rt_pin_write(MUTE_PIN, PIN_HIGH);
}

void buzzer_sos_entry(void *parameter)
{
    while (1)
    {
        for (size_t i = 0; i < 3; i++)
        {
            buzzer_on();
            rt_thread_mdelay(200);
            buzzer_off();
            rt_thread_mdelay(200);
        }
        rt_thread_mdelay(300);
        for (size_t i = 0; i < 3; i++)
        {
            buzzer_on();
            rt_thread_mdelay(400);
            buzzer_off();
            rt_thread_mdelay(400);
        }
        rt_thread_mdelay(100);
        for (size_t i = 0; i < 3; i++)
        {
            buzzer_on();
            rt_thread_mdelay(200);
            buzzer_off();
            rt_thread_mdelay(200);
        }
        rt_thread_mdelay(1000);
    }
}

static rt_thread_t sos_tid = RT_NULL;

int sos_startup(void)
{
    if (sos_tid != NULL)
    {
        return RT_ERROR;
    }
    sos_tid = rt_thread_create("s.o.s.", buzzer_sos_entry, RT_NULL, 140, 31, 20);
    if (sos_tid != RT_NULL)
    {
        rt_thread_startup(sos_tid);
    }
    return RT_EOK;
}

int sos_delete(void)
{
    buzzer_off();
    if (sos_tid != RT_NULL)
    {
        rt_thread_delete(sos_tid);
        sos_tid = RT_NULL;
        return RT_EOK;
    }
    return RT_ERROR;
}

static int rt_hw_alarm_init(void)
{
    rt_pin_mode(BUZZER_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(MUTE_PIN, PIN_MODE_OUTPUT_OD);
    buzzer_off();
    mute_on();

    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_alarm_init);
