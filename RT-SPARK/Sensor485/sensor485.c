/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-15     彭竣       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_rs485.h>

#define DBG_TAG "main"
#define DBG_LVL         DBG_LOG
#include <rtdbg.h>

float humidity, temperature;

void Sensor485init(void)
{
    rs485_init();
}

void Sensor485data(void)
{
    char str[]={0x01,0x03,0x00,0x00,0x00,0x02,0xc4,0x0b};
       char recv_buffer[20]={};
       rt_int16_t thshi,thge;
       float data;
       rt_int8_t index0;
       rt_int8_t buffer;
       recv_buffer[index0++]=buffer;
       rt_err_t uart_function(){
        if(buffer=='\n'){
               recv_buffer[index0]='\0';
               rt_pin_write(BSP_RS485_RTS_PIN, PIN_HIGH);
               rt_device_write(rs485_serial, 0,recv_buffer, index0);
              rt_pin_write(BSP_RS485_RTS_PIN, PIN_LOW);

              index0=0;
           }
           return RT_EOK;
       }

            rs485_send_data(str, 8);
            rt_device_read(rs485_serial, 0, &recv_buffer, 9);

           // recv_buffer[index0++]=buffer;
            rt_kprintf("\nrecevi:");
                for(int i =0;i<9;i++)
                {
                    rt_kprintf("%d:%x ",i,recv_buffer[i]);
                }
                data = recv_buffer[5]*256+recv_buffer[6];
                humidity = data/10.0;
                rt_kprintf("\nhumidity is %f\n ",humidity);

                if(recv_buffer[3]>128)
                {
                    data=255-recv_buffer[3]*256+255-recv_buffer[4]+1;
                    temperature=data/10.0;
                    rt_kprintf("temperature is -%f\n",temperature);
                }
                else {
                    thshi=recv_buffer[3]/10;
                    thge=recv_buffer[3]%10;
                    data=thshi*4096+thge*256+recv_buffer[4];
                    temperature=data/10.0;
                    rt_kprintf("temperature is %f\n",temperature);
                    }

    // 清空接收缓冲区
    memset(recv_buffer, 0, sizeof(recv_buffer));
}

int sensor_thread_entry(void *parameter)
{
    while (1)
    {
        Sensor485data();
        rt_thread_mdelay(2000); // 延时2秒，可以根据需求调整
    }
    return RT_EOK;
}

void sensor_thread_init(void)
{
    rt_thread_t thread;
    thread = rt_thread_create("sensor", sensor_thread_entry, RT_NULL, 1024, 22, 10);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        rt_kprintf("Failed to create sensor thread.\n");
    }
}


