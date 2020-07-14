/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-13     Chenxuan     first version
 */

#ifndef __KEY_H__
#define __KEY_H__

#include "flexible_button.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void btn_1_callback(flex_button_t *btn);
extern void btn_2_callback(flex_button_t *btn);
extern void btn_3_callback(flex_button_t *btn);
extern void btn_4_callback(flex_button_t *btn);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H__ */
