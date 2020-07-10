/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-08     Chenxuan     first version
 */

#ifndef __F21_CORE_H__
#define __F21_CORE_H__

#ifdef __cplusplus
extern "C"
{
#endif

extern int f21_refresh_position(void);
extern int f21_cmd_base(const char *cmd_expr, const char *resp_expr);

#ifdef __cplusplus
}
#endif

#endif /* __F21_CORE_H__ */
