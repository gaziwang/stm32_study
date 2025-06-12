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
#define SECTOR_SIZE       512
#define FLASH_SECTOR_NUM  32768  // 16MB / 512
#define FLASH_BLOCK_SIZE  8      // 每个擦除块 4096B = 8 扇区
#define FLASH_BASE_ADDR   0x000000 // 你可以根据实际文件系统起始设置偏移
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv)
{
    if (pdrv == 0) return RES_OK;
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv)
{
    if (pdrv != 0) return STA_NOINIT;

    SPI2_Init(); // 初始化SPI硬件
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,
    BYTE *buff,
    LBA_t sector,
    UINT count)
{
    if (pdrv != 0 || !count) return RES_PARERR;

    uint32_t address = FLASH_BASE_ADDR + sector * SECTOR_SIZE;

    for (UINT i = 0; i < count; i++) {
        SPI2_FLASH_ReadData(address, buff, SECTOR_SIZE);
        address += SECTOR_SIZE;
        buff += SECTOR_SIZE;
    }

    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
    BYTE pdrv,
    const BYTE *buff,
    LBA_t sector,
    UINT count)
{
    if (pdrv != 0 || !count) return RES_PARERR;

    uint32_t address = FLASH_BASE_ADDR + sector * SECTOR_SIZE;

    for (UINT i = 0; i < count; i++) {
        // 🔁 每次都要重新写使能（必须）
        SPI2_FLASH_Write_Enable();
        SPI_FLASH_SectorErase(address);
        Delay_ms(100); // 等待擦除完成
        printf("Erasing sector at 0x%08X...\n", address);
        SPI2_FLASH_Write_Enable();  // 🔁 写入前也要再写使能
        SPI2_FLASH_WriteBytes(address, buff, SECTOR_SIZE);

        address += SECTOR_SIZE;
        buff += SECTOR_SIZE;
    }

    return RES_OK;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    if (pdrv != 0) return RES_PARERR;

    switch (cmd)
    {
        case CTRL_SYNC:
            return RES_OK;

        case GET_SECTOR_SIZE:
            *(WORD *)buff = SECTOR_SIZE;
            return RES_OK;

        case GET_BLOCK_SIZE:
            *(DWORD *)buff = FLASH_BLOCK_SIZE;
            return RES_OK;

        case GET_SECTOR_COUNT:
            *(DWORD *)buff = FLASH_SECTOR_NUM;
            return RES_OK;

        default:
            return RES_PARERR;
    }
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
