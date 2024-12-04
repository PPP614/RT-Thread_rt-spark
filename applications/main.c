/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-5-10      ShiHao       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <./lcd/gui.h>
#include "./Function/function.h"
#include "./lcd/lcd_ili9341.h"
#include "drv_touch.h"
#include <./Key/key.h>
#include "./Uart/uart.h"
#include <drv_rs485.h>
#include "./Sensor485/sensor485.h"
#define DBG_TAG "main"
#define DBG_LVL         DBG_LOG
#include <rtdbg.h>
extern char ch;
extern char buf[128];

int main(void)
{
    Sensor485init();        // 初始化RS485设备
    Btns_Init(); // 初始化按键引脚和线程
    sensor_thread_init();   // 初始化并启动传感器线程
    thread_test_color_init();
    uart_init();

}


