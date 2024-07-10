/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-14     彭竣       the first version
 */
#include "drv_touch.h"
#include "rtdevice.h"
#include "drv_common.h"
#include "stm32f4xx.h"
#include "drv_spi.h"
#include "rtdbg.h"
#define DBG_TAG "touch"

#define X_LEN 320 // 屏幕分辨率
#define Y_LEN 240 // 屏幕分辨率

#define X_MAX 7508 // 实际x 方向最大采样值
#define X_MIN 384  // 实际y 方向最大采样值
#define Y_MAX 7632 // 实际x 方向最小采样值
#define Y_MIN 448  // 实际x 方向最小采样值


#define T_SPI_BUS "spi1" // 使用spi1 作为屏幕接口, 速度更快 触摸也挂上去

#define T_SPI_DEVICE "spi11"


#define IRQ_PIN GET_PIN(F, 6) // 笔中断引脚
#define TCS_PIN GET_PIN(F, 7)
#define TCS_PIN_CLR rt_pin_write(TCS_PIN, PIN_LOW)
#define TCS_PIN_SET rt_pin_write(TCS_PIN, PIN_HIGH)

static struct rt_spi_device *spi_dev_t;


void touch_init(void)
{
    rt_pin_mode(IRQ_PIN, PIN_MODE_INPUT_PULLDOWN); // 设置感应笔引脚
    rt_pin_mode(TCS_PIN, PIN_MODE_OUTPUT); // 设置感应笔引脚

          rt_err_t res = RT_EOK;

          spi_device_attach(T_SPI_BUS, T_SPI_DEVICE,TCS_PIN );
          spi_dev_t = (struct rt_spi_device *)rt_device_find(T_SPI_DEVICE);
              if (spi_dev_t != RT_NULL)
          {
          /* config spi */
          {
              struct rt_spi_configuration cfg;
              cfg.data_width = 8;
              cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0| RT_SPI_MSB;
              cfg.max_hz = 10 * 1000 * 1000; /* 42M,SPI max 42MHz,lcd 4-wire spi */

              rt_spi_configure(spi_dev_t, &cfg);
          }

          }  //  return RT_EOK;
}
INIT_APP_EXPORT(touch_init);

int8_t sendbuf[3],recv_buf[3];



uint8_t is_preass(void)
{
uint8_t res = rt_pin_read(IRQ_PIN);
}


void get_x_y_raw(uint16_t *x,uint16_t *y) // 获取 坐标的采样值 , 用来校准用
{
uint16_t res;

    sendbuf[0] =  0x90; //  x
    TCS_PIN_CLR;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);
    rt_hw_us_delay(50);// rt_thread_delay(1);
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);
    TCS_PIN_SET;

    res = (recv_buf[0]<<8) + recv_buf[1];

    *x = res>>2;




    sendbuf[0] =  0xd0; //  y
    TCS_PIN_CLR;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);
   // rt_thread_delay(1);
    rt_hw_us_delay(50);
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);
    TCS_PIN_SET;

    res = (recv_buf[0]<<8) + recv_buf[1];

    *y = res>>2;


}
static uint16_t x_buf[3],y_buf[3];
static uint8_t x_cnt = 0,y_cnt = 0;  // 求平均滤波

void get_x_y(uint16_t *x,uint16_t *y)
{
uint16_t res;
x_cnt = 0,y_cnt = 0;

    sendbuf[0] =  0x90; //  x
    TCS_PIN_CLR;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);
    rt_hw_us_delay(50);// rt_t        hread_delay(1);
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);    res = (recv_buf[0]<<8) + recv_buf[1];x_buf[x_cnt%3] = res >> 2; x_cnt ++;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);    res = (recv_buf[0]<<8) + recv_buf[1];x_buf[x_cnt%3] = res >> 2; x_cnt ++;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);    res = (recv_buf[0]<<8) + recv_buf[1];x_buf[x_cnt%3] = res >> 2; x_cnt ++;
    res = (recv_buf[0]<<8) + recv_buf[1];
    TCS_PIN_SET;


    *x = (x_buf[0] + x_buf[1] + x_buf[2])/3;

  //  *x =( X_MAX - *x ) * X_LEN / (X_MAX - X_MIN); //
    *x =( *x - X_MIN ) * X_LEN / (X_MAX - X_MIN); // 取反操作
    if(*x > X_LEN)
        *x = 320;
    if(*x < 0 )
        *x = 0;



    sendbuf[0] =  0xd0; //  y
    TCS_PIN_CLR;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2);
   // rt_thread_delay(1);
    rt_hw_us_delay(50);
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2); res = (recv_buf[0]<<8) + recv_buf[1];y_buf[y_cnt%3] = res >> 2;y_cnt ++;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2); res = (recv_buf[0]<<8) + recv_buf[1];y_buf[y_cnt%3] = res >> 2;y_cnt ++;
    rt_spi_send_then_recv(spi_dev_t,sendbuf,1,recv_buf,2); res = (recv_buf[0]<<8) + recv_buf[1];y_buf[y_cnt%3] = res >> 2;y_cnt ++;
    TCS_PIN_SET;



    *y = (y_buf[0] + y_buf[1] + y_buf[2])/3;

    *y =( Y_MAX - *y ) * Y_LEN / (Y_MAX - Y_MIN);

    if(*y > Y_LEN)
        *y = 240;
    if(*y < 0 )
        *y = 0;
}
