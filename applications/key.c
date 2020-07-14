/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-13     Chenxuan     first version
 */

#include "key.h"

#include <rtdevice.h>
#include <rtthread.h>

#include "drv_gpio.h"

#define KEY1_PIN GET_PIN(A, 5)
#define KEY2_PIN GET_PIN(A, 4)
#define KEY3_PIN GET_PIN(A, 7)
#define KEY4_PIN GET_PIN(A, 6)

typedef enum
{
    USER_BUTTON_1 = 0,
    USER_BUTTON_2,
    USER_BUTTON_3,
    USER_BUTTON_4,
    USER_BUTTON_MAX
} user_button_t;

static flex_button_t user_button[USER_BUTTON_MAX] = {0};

static void button_scan(void *parameter)
{
    while (1)
    {
        flex_button_scan();
        rt_thread_mdelay(20);
    }
}

static uint8_t button_key1_read(void)
{
    return rt_pin_read(KEY1_PIN);
}

static uint8_t button_key2_read(void)
{
    return rt_pin_read(KEY2_PIN);
}

static uint8_t button_key3_read(void)
{
    return rt_pin_read(KEY3_PIN);
}

static uint8_t button_key4_read(void)
{
    return rt_pin_read(KEY4_PIN);
}

RT_WEAK void btn_1_callback(flex_button_t *btn)
{
    rt_kprintf("btn_1_callback\n");
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_DOWN:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_DOWN]\n");
        break;
    case FLEX_BTN_PRESS_CLICK:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_DOUBLE_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_START:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_SHORT_START]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_UP:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_SHORT_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_START:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_LONG_START]\n");
        break;
    case FLEX_BTN_PRESS_LONG_UP:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_LONG_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_LONG_HOLD]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        rt_kprintf("btn_1_callback [FLEX_BTN_PRESS_LONG_HOLD_UP]\n");
        break;
    }
}

RT_WEAK void btn_2_callback(flex_button_t *btn)
{
    rt_kprintf("btn_2_callback\n");
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_DOWN:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_DOWN]\n");
        break;
    case FLEX_BTN_PRESS_CLICK:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_DOUBLE_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_START:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_SHORT_START]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_UP:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_SHORT_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_START:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_LONG_START]\n");
        break;
    case FLEX_BTN_PRESS_LONG_UP:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_LONG_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_LONG_HOLD]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        rt_kprintf("btn_2_callback [FLEX_BTN_PRESS_LONG_HOLD_UP]\n");
        break;
    }
}

RT_WEAK void btn_3_callback(flex_button_t *btn)
{
    rt_kprintf("btn_3_callback\n");
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_DOWN:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_DOWN]\n");
        break;
    case FLEX_BTN_PRESS_CLICK:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_DOUBLE_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_START:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_SHORT_START]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_UP:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_SHORT_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_START:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_LONG_START]\n");
        break;
    case FLEX_BTN_PRESS_LONG_UP:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_LONG_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_LONG_HOLD]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        rt_kprintf("btn_3_callback [FLEX_BTN_PRESS_LONG_HOLD_UP]\n");
        break;
    }
}

RT_WEAK void btn_4_callback(flex_button_t *btn)
{
    rt_kprintf("btn_4_callback\n");
    switch (btn->event)
    {
    case FLEX_BTN_PRESS_DOWN:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_DOWN]\n");
        break;
    case FLEX_BTN_PRESS_CLICK:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_DOUBLE_CLICK]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_START:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_SHORT_START]\n");
        break;
    case FLEX_BTN_PRESS_SHORT_UP:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_SHORT_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_START:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_LONG_START]\n");
        break;
    case FLEX_BTN_PRESS_LONG_UP:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_LONG_UP]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_LONG_HOLD]\n");
        break;
    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        rt_kprintf("btn_4_callback [FLEX_BTN_PRESS_LONG_HOLD_UP]\n");
        break;
    }
}

static void user_button_init(void)
{
    user_button[USER_BUTTON_1].usr_button_read = button_key1_read;
    user_button[USER_BUTTON_1].cb = (flex_button_response_callback)btn_1_callback;

    user_button[USER_BUTTON_2].usr_button_read = button_key2_read;
    user_button[USER_BUTTON_2].cb = (flex_button_response_callback)btn_2_callback;

    user_button[USER_BUTTON_3].usr_button_read = button_key3_read;
    user_button[USER_BUTTON_3].cb = (flex_button_response_callback)btn_3_callback;

    user_button[USER_BUTTON_4].usr_button_read = button_key4_read;
    user_button[USER_BUTTON_4].cb = (flex_button_response_callback)btn_4_callback;

    rt_pin_mode(KEY1_PIN, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(KEY2_PIN, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(KEY3_PIN, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(KEY4_PIN, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */

    for (int i = 0; i < USER_BUTTON_MAX; i++)
    {
        user_button[i].pressed_logic_level = 0;
        user_button[i].click_start_tick = 20;
        user_button[i].short_press_start_tick = 50;
        user_button[i].long_press_start_tick = 100;
        user_button[i].long_hold_start_tick = 200;

        flex_button_register(&user_button[i]);
    }
}

int flex_button_main(void)
{
    user_button_init();

    /* Create background ticks thread */
    rt_thread_t tid = rt_thread_create("flex_btn", button_scan, RT_NULL, 1024, 10, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return 0;
}
#ifdef FINSH_USING_MSH
INIT_APP_EXPORT(flex_button_main);
#endif
