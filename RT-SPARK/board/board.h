/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include <stm32f4xx.h>
#include "drv_common.h"
#include "drv_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STM32_SRAM_SIZE        (128)
#define STM32_SRAM_END         (0x20000000 + STM32_SRAM_SIZE * 1024)

#define STM32_FLASH_START_ADRESS     ((uint32_t)0x08000000)
#define STM32_FLASH_SIZE             (1024 * 1024)
#define STM32_FLASH_END_ADDRESS      ((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE))

#define PKG_USING_ILI9341
#define PKG_ILI_9341_SPI_BUS_NAME "spi1"
#define PKG_ILI_9341_SPI_DEVICE_NAME "spi10"
#define PKG_ILI_9341_WIDTH 240
#define PKG_ILI_9341_HEIGHT 320
#define PKG_ILI_9341_DC_PIN 67 //GET_PIN(E, 3)
#define PKG_ILI_9341_RES_PIN 68 //GET_PIN(E, 4)
#define PKG_ILI_9341_CS_PIN 4 //GET_PIN(A, 4)
#define PKG_ILI_9341_BLK_PIN 66 //GET_PIN(E, 2)
#define PKG_USING_ILI9341_LATEST_VERSION

#if defined(__ARMCC_VERSION)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      ((void *)&__bss_end)
#endif

#define HEAP_END        STM32_SRAM_END

void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif

