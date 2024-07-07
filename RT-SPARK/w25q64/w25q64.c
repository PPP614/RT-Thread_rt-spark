///*
// * Copyright (c) 2006-2021, RT-Thread Development Team
// *
// * SPDX-License-Identifier: Apache-2.0
// *
// * Change Logs:
// * Date           Author       Notes
// * 2024-06-20     彭竣       the first version
// */
//#include "w25q64.h"
//#include "drv_spi.h"
//#define LOG_TAG      "flash"
//#define LOG_LVL       ELOG_LVL_DEBUG
//
//
//uint8_t const W25X_WriteEnable=      0x06 ;
//uint8_t const W25X_WriteDisable=     0x04 ;
//uint8_t const W25X_ReadStatusReg=    0x05 ;
//uint8_t const W25X_WriteStatusReg=   0x01 ;
//uint8_t const W25X_ReadData=         0x03 ;
//uint8_t const W25X_FastReadData=     0x0B ;
//uint8_t const W25X_FastReadDual=     0x3B ;
//uint8_t const W25X_PageProgram=      0x02 ;
//uint8_t const W25X_BlockErase=       0xD8 ;
//uint8_t const W25X_SectorErase=      0x20 ;
//uint8_t const W25X_ChipErase =       0xC7 ;
//uint8_t const W25X_PowerDown =       0xB9 ;
//uint8_t const W25X_ReleasePowerDown= 0xAB ;
//uint8_t const W25X_DeviceID  =       0xAB ;
//uint8_t const W25X_ManufactDeviceID= 0x90 ;
//uint8_t const W25X_JedecDeviceID=    0x9F ;
//
//
//static struct rt_spi_device *spi_dev_w25q;
//
//
////读取W25QXX的状态寄存器
////BIT7  6   5   4   3   2   1   0
////SPR   RV  TB BP2 BP1 BP0 WEL BUSY
////SPR:默认0,状态寄存器保护位,配合WP使用
////TB,BP2,BP1,BP0:FLASH区域写保护设置
////WEL:写使能锁定
////BUSY:忙标记位(1,忙;0,空闲)
////默认:0x00
//uint8_t flash_read_sr(void)
//{
//    uint8_t byte=0;
//    rt_spi_send_then_recv(spi_dev_w25q,&W25X_ReadStatusReg,1,&byte,1);
//    return byte;
//}
////写W25QXX状态寄存器
////只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
//void flash_write_sr(uint8_t sr)
//{
//
//rt_spi_send_then_send(spi_dev_w25q,&W25X_WriteStatusReg,1,&sr,1);   //发送写取状态寄存器命令
//
//}
////W25QXX写使能
////将WEL置位
//void flash_write_enable(void)
//{
//    rt_spi_send(spi_dev_w25q,&W25X_WriteEnable,1);      //发送写使能
//}
////W25QXX写禁止
////将WEL清零
//void flash_write_disable(void)
//{
//    rt_spi_send(spi_dev_w25q,&W25X_WriteDisable,1);     //发送写禁止指令
//
//}
////读取SPI FLASH
////在指定地址开始读取指定长度的数据
////pBuffer:数据存储区
////ReadAddr:开始读取的地址(24bit)
////NumByteToRead:要读取的字节数(最大65535)
//void flash_read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
//{
//    uint32_t ReadCmd =  (W25X_ReadData)|(ReadAddr<<8);
//    rt_spi_send_then_recv(spi_dev_w25q,&ReadCmd,4,pBuffer,NumByteToRead);//发送读取命令
//}
//
//
//
//
////SPI在一页(0~65535)内写入少于255个字节的数据
////在指定地址开始写入最大255字节的数据
////pBuffer:数据存储区
////WriteAddr:开始写入的地址(24bit)
////NumByteToWrite:要写入的字节数(最大255),该数不应该超过该页的剩余字节数!!!
//void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint8_t NumByteToWrite)
//{
//    uint32_t WriteCmd =  (W25X_PageProgram)|(WriteAddr<<8);
//    flash_write_enable();                  //SET WEL
//
//    rt_spi_send_then_send(spi_dev_w25q,&WriteCmd,4,pBuffer,NumByteToWrite%256);      //发送写页命令
//    flash_wait_busy();                          //等待写入结束
//}
////无检验写SPI FLASH
////必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
////具有自动换页功能
////在指定地址开始写入指定长度的数据,但是要确保地址不越界!
////pBuffer:数据存储区
////WriteAddr:开始写入的地址(24bit)
////NumByteToWrite:要写入的字节数(最大65535)
////CHECK OK
//void flash_write_no_check(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
//{
//    uint16_t pageremain;
//    pageremain=256-WriteAddr%256; //单页剩余的字节数
//    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
//    while(1)
//    {
//        W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
//        if(NumByteToWrite==pageremain)break;//写入结束了
//        else //NumByteToWrite>pageremain
//        {
//            pBuffer+=pageremain;
//            WriteAddr+=pageremain;
//
//            NumByteToWrite-=pageremain; //减去已经写入了的字节数
//            if(NumByteToWrite>255)pageremain=255; //一次可以写入256个字节
//            else pageremain=NumByteToWrite;       //不够256个字节了
//        }
//    };
//}
////写SPI FLASH
////在指定地址开始写入指定长度的数据
////该函数带擦除操作!
////pBuffer:数据存储区
////WriteAddr:开始写入的地址(24bit)
////NumByteToWrite:要写入的字节数(最大65535)
//void flash_write(uint8_t* pBuffer,uint16_t sectorNum,uint16_t NumByteToWrite)
//{
//    flash_erase_sector(sectorNum);//擦除这个扇区
//    flash_write_no_check(pBuffer,sectorNum,NumByteToWrite);//写入整个扇区
//
//}
////擦除整个芯片
////等待时间超长...
//void flash_erase_chip(void)
//{
//    flash_write_enable();                  //SET WEL
//    flash_wait_busy();
//
//    rt_spi_send(spi_dev_w25q,&W25X_ChipErase,1);        //发送片擦除命令
//    flash_wait_busy();                     //等待芯片擦除结束
//}
////擦除一个扇区
////Dst_Addr:扇区地址 根据实际容量设置
////擦除一个山区的最少时间:150ms
//void flash_erase_sector(uint32_t Dst_Addr)
//{
//    uint32_t EraseSectorCmd =  (W25X_SectorErase)|(Dst_Addr<<8);
//    rt_kprintf("Erase_Sector:%x\r\n",Dst_Addr);//监视falsh擦除情况,测试用
//    Dst_Addr*=4096;
//    flash_write_enable();                  //SET WEL
//    flash_wait_busy();
//
//    rt_spi_send(spi_dev_w25q,&EraseSectorCmd,4);      //发送扇区擦除指令
//    flash_wait_busy();                     //等待擦除完成
//}
////等待空闲
//void flash_wait_busy(void)
//{
//    while((flash_read_sr()&0x01)==0x01);   // 等待BUSY位清空
//}
////进入掉电模式
//void flash_powerdown(void)
//{
//    rt_spi_send(spi_dev_w25q,&W25X_PowerDown,1);        //发送掉电命令
//    rt_hw_us_delay(3);                               //等待TPD
//}
////唤醒
//void flash_wakeup(void)
//{
//    rt_spi_send(spi_dev_w25q,&W25X_ReleasePowerDown,1);   //  send W25X_PowerDown command 0xAB
//
//    rt_hw_us_delay(3);                               //等待TRES1
//}
//
//
//
//void flash_init()
//{
//
//    rt_hw_spi_device_attach("spi2", "spi flash", GPIOI, GPIO_PIN_0);
//
//    spi_dev_w25q = (struct rt_spi_device *)rt_device_find("spi flash");
//    if (!spi_dev_w25q)
//    {
//        rt_kprintf("spi sample run failed! can't find %s device!\n","spi2");
//    }
//    else
//    {
//        struct rt_spi_configuration cfg;
//        cfg.data_width = 8;
//        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB;
//        cfg.max_hz = 50 * 1000 *1000;                           /* 50MHz ,no more than 80M */
//        rt_spi_configure(spi_dev_w25q, &cfg);
//
//    }
//}
//
//
//
//
