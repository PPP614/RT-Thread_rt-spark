/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-20     彭竣       the first version
 */
#ifndef W25Q64_W25Q64_H_
#define W25Q64_W25Q64_H_


#define W25Q80  0XEF13
#define W25Q16  0XEF14
#define W25Q32  0XEF15
#define W25Q64  0XEF16
#define W25Q128 0XEF17





void flash_init(void);
uint16_t  flash_read_id(void);               //读取FLASH ID
uint8_t   flash_read_sr(void);               //读取状态寄存器
void flash_write_sr(uint8_t sr);             //写状态寄存器
void flash_write_enable(void);          //写使能
void flash_write_disable(void);     //写保护
void flash_write_no_check(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void flash_read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void flash_write(uint8_t* pBuffer,uint16_t sectorNum,uint16_t NumByteToWrite);//写入flash
void flash_erase_chip(void);            //整片擦除
void flash_erase_sector(uint32_t Dst_Addr);  //扇区擦除
void flash_wait_busy(void);             //等待空闲
void flash_powerdown(void);         //进入掉电模式
void flash_wakeup(void);                //唤醒




#endif /* W25Q64_W25Q64_H_ */
