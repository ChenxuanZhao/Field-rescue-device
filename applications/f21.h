/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-12     Chenxuan     first version
 */

#ifndef __F21_H__
#define __F21_H__

#include <rtthread.h>
#include <at.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern float f21_get_gps_parameter_e(void);
extern float f21_get_gps_parameter_n(void);

extern bool f21_set_faker_position(float _latitude, float _longitude);
extern bool f21_set_phone_num(uint8_t cnt, const char *phone_num);

extern int f21_cmd_base(const char *cmd_expr, const char *resp_expr);
extern int f21_refresh_position(void);

extern rt_err_t f21_send_message(uint8_t phone_cnt, const char *msg);
extern rt_err_t f21_delete_message(void);
extern rt_err_t f21_phone_call(uint8_t phone_cnt);

extern void f21_urc_ring_callback(struct at_client *client, const char *data, rt_size_t size);
extern void f21_urc_recv_callback(struct at_client *client, const char *data, rt_size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __F21_H__ */
