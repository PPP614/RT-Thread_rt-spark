/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-14     彭竣       the first version
 */
#ifndef LCD_DRV_TOUCH_H_
#define LCD_DRV_TOUCH_H_


#include <stdint.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drivers/spi.h"


void touch_init(void);
uint8_t is_preass(void);
void get_x_y_raw(uint16_t *x,uint16_t *y);  // 获取 坐标的采样值 , 用来校准用
void get_x_y(uint16_t *x,uint16_t *y);

#endif /* LCD_DRV_TOUCH_H_ */
