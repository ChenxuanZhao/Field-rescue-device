/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-08     Chenxuan     first version
 */

#ifndef __F21_MESSAGE_H__
#define __F21_MESSAGE_H__

#include <stddef.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern rt_err_t sms_send_message(uint8_t phone_cnt, const char *msg);
extern rt_err_t sms_delete_message(void);

#ifdef __cplusplus
}
#endif

#endif /* __F21_MESSAGE_H__ */
