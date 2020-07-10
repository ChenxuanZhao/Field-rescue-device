/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-08     Chenxuan     first version
 */

#include "f21_message.h"

#include <stdint.h>
#include <string.h>
#include <at.h>

#include "f21_core.h"

static char phone_base[][12] = {{"13771731568"}, {"18549982432"}};

#define PHONE_BASE_SIZE (sizeof(phone_base) / sizeof(phone_base[0]))

rt_err_t sms_send_message(uint8_t phone_cnt, const char *msg)
{
    static rt_err_t err;

    if (msg == RT_NULL || phone_cnt >= PHONE_BASE_SIZE)
    {
        return RT_ERROR;
    }
    char *p = (char *)rt_calloc(140, sizeof(char));
    if (p == RT_NULL)
    {
        return RT_ENOMEM;
    }
    strcat(p, "AT+CMGS=\"");
    strcat(p, phone_base[phone_cnt]);
    strcat(p, "\"");
    err = f21_cmd_base(p, ">");
    at_client_send(msg, strlen(msg));
    char eof = 0x1A;
    at_client_send(&eof, 1);
    rt_free(p);

    return err;
}

rt_err_t sms_delete_message(void)
{
    rt_err_t err;

    err = f21_cmd_base("AT+CPMS?", "OK");
    err = f21_cmd_base("AT+CMGF=0", "OK");
    err = f21_cmd_base("AT+CMGDA=6", "OK");
    err = f21_cmd_base("AT+CMGF=1", "OK");

    return err;
}
