/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"     /* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */
#include "SPI.h"    /* SPI functions for RAM disk */
#include <string.h>
#include <stdio.h>
/* Definitions of physical drive number for each drive */
#define SPI2_FLASH  0 /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC     1 /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB     2 /* Example: Map USB MSD to physical drive 2 */
#define SECTOR_SIZE 4096
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;

    switch (pdrv) {
        case SPI2_FLASH:
            result = SPI2_FLASH_ReadID();
            if (result == 0xEF4018) {
                stat = RES_OK; // Assume initialization is successful
            } else {
                stat = STA_NOINIT; // Initialization failed
            }
            // translate the reslut code here

            return stat;

            // case DEV_MMC:
            //     result = MMC_disk_status();

            //     // translate the reslut code here

            //     return stat;

            // case DEV_USB:
            //     result = USB_disk_status();

            //     // translate the reslut code here

            //     return stat;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    switch (pdrv) {
        case SPI2_FLASH:
            SPI2_Init();
            if (disk_status(SPI2_FLASH) == 0) {
                stat = RES_OK; // Assume initialization is successful
            } else {
                stat = STA_NOINIT; // Initialization failed
            }
            return stat;

            // case DEV_MMC:
            //     result = MMC_disk_initialize();

            //     // translate the reslut code here

            //     return stat;

            // case DEV_USB:
            //     result = USB_disk_initialize();

            //     // translate the reslut code here

            //     return stat;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,    /* Physical drive nmuber to identify the drive */
    BYTE *buff,   /* Data buffer to store read data */
    LBA_t sector, /* Start sector in LBA */
    UINT count    /* Number of sectors to read */
)
{
    DRESULT res;

    switch (pdrv) {
        case SPI2_FLASH:
            uint32_t address = sector * SECTOR_SIZE;
            for (UINT i = 0; i < count; i++) {
                SPI_FLASH_ReadData(address, buff, SECTOR_SIZE);
                address += SECTOR_SIZE;
            }
            res = RES_OK; // Assume read is successful
            return res;

            // case DEV_MMC:
            //     // translate the arguments here

            //     result = MMC_disk_read(buff, sector, count);

            //     // translate the reslut code here

            //     return res;

            // case DEV_USB:
            //     // translate the arguments here

            //     result = USB_disk_read(buff, sector, count);

            //     // translate the reslut code here

            //     return res;
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
    BYTE pdrv,        /* Physical drive nmuber to identify the drive */
    const BYTE *buff, /* Data to be written */
    LBA_t sector,     /* Start sector in LBA */
    UINT count        /* Number of sectors to write */
)
{
    DRESULT res;
    switch (pdrv) {

        case SPI2_FLASH:
            uint32_t address = sector * SECTOR_SIZE;
            for (UINT i = 0; i < count; i++) {
                // SPI Flash 不能直接写，需要先擦除再写入
                SPI_FLASH_SectorErase(address); // 擦除当前扇区（通常4KB）

                // 写入一个扇区数据
                SPI2_FLASH_WriteBytes(address, buff, SECTOR_SIZE);

                // 更新地址与指针
                address += SECTOR_SIZE;
                buff += SECTOR_SIZE;
            }
            res = RES_OK;
            return res;

            // case DEV_MMC:
            //     // translate the arguments here

            //     result = MMC_disk_write(buff, sector, count);

            //     // translate the reslut code here

            //     return res;

            // case DEV_USB:
            //     // translate the arguments here

            //     result = USB_disk_write(buff, sector, count);

            //     // translate the reslut code here

            //     return res;
    }

    return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
    BYTE pdrv, /* Physical drive nmuber (0..) */
    BYTE cmd,  /* Control code */
    void *buff /* Buffer to send/receive control data */
)
{
    DRESULT res;

    switch (pdrv) {
        case SPI2_FLASH:
            switch (cmd) {
                case CTRL_SYNC:

                    break;

                case GET_SECTOR_COUNT:
                    *(DWORD *)buff = 4096;
                    break;
                case GET_SECTOR_SIZE:
                    *(DWORD *)buff = SECTOR_SIZE;
                    break;
                case GET_BLOCK_SIZE:
                    *(DWORD *)buff = 16;
                    ;
                    break;
            }
            // Process of the command for the RAM drive

            return res;

            // case DEV_MMC:

            //     // Process of the command for the MMC/SD card

            //     return res;

            // case DEV_USB:

            //     // Process of the command the USB drive

            //     return res;
    }

    return RES_PARERR;
}
DWORD get_fattime(void) {
    const char *months[] = {"Jan","Feb","Mar","Apr","May","Jun",
                            "Jul","Aug","Sep","Oct","Nov","Dec"};
    
    // 编译器预定义的日期时间宏
    const char *date = __DATE__;  // 格式: "Jun 10 2025"
    const char *time = __TIME__;  // 格式: "12:30:00"
    
    char month_str[4];
    int year, day, hour, min, sec, month = 1;
    
    sscanf(date, "%3s %d %d", month_str, &day, &year);
    sscanf(time, "%d:%d:%d", &hour, &min, &sec);
    
    // 转换月份字符串为数字
    for (int i = 0; i < 12; i++) {
        if (strncmp(month_str, months[i], 3) == 0) {
            month = i + 1;
            break;
        }
    }
    
    return ((DWORD)(year - 1980) << 25)
         | ((DWORD)month << 21)
         | ((DWORD)day << 16)
         | ((DWORD)hour << 11)
         | ((DWORD)min << 5)
         | ((DWORD)(sec / 2));  // 秒数除以2
}
