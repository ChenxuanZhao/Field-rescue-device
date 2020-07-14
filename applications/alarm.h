/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-13     Chenxuan     first version
 */

#ifndef __ALARM_H__
#define __ALARM_H__

#ifdef __cplusplus
extern "C"
{
#endif

extern int sos_startup(void);
extern int sos_delete(void);
extern void buzzer_on(void);
extern void buzzer_off(void);
extern void mute_on(void);
extern void mute_off(void);

#ifdef __cplusplus
}
#endif

#endif /* __ALARM_H__ */
