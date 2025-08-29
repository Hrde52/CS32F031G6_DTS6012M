#include "stmflash.h"


/**
 * @brief       从指定地址读取一个字 (32位数据)
 * @param       faddr   : 读取地址 (此地址必须为4倍数!!)
 * @retval      读取到的数据 (32位)
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       从指定地址开始读出指定长度的数据
 * @param       raddr : 起始地址
 * @param       pbuf  : 数据指针
 * @param       length: 要读取的字(32位)数,即4个字节的整数倍
 * @retval      无
 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t  i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr);/* 读取4个字节 */
        raddr += 4;                         /* 偏移4个字节 */
    }
}

/**
 * @brief       获取某个地址所在的flash扇区
 * @param       addr: flash地址
 * @retval      指定地址所在的flash扇区
 */
static uint32_t stmflash_get_flash_sector(uint32_t addr)
{
    uint32_t sector = 0;

    if ((addr >= FLASH_BASE) && (addr < FLASH_BASE + FLASH_BANK_SIZE))
    {
        sector = (addr & ~FLASH_BASE) / FLASH_SECTOR_SIZE;
    }
    else if ((addr >= FLASH_BASE + FLASH_BANK_SIZE) && (addr < FLASH_BASE + FLASH_SIZE))
    {
        sector = ((addr & ~FLASH_BASE) - FLASH_BANK_SIZE) / FLASH_SECTOR_SIZE;
    }
    else
    {
        sector = 0xFFFFFFFF;    /* 地址溢出 */
    }

    return sector;
}

/**
 * @brief       获取某个地址所在的flash bank
 * @param       addr: flash地址
 * @retval      返回值是：FLASH_BANK_1、FLASH_BANK_2、0xFFFFFFFF（表示地址溢出）
 */
static uint32_t stmflash_get_flash_bank(uint32_t addr)
{
    uint32_t bank = 0;

//    if((addr >= FLASH_BASE) && (addr < FLASH_BASE + FLASH_BANK_SIZE))
//    {
//        bank = FLASH_BANK_1;
//    }
//    else if ((addr >= FLASH_BASE + FLASH_BANK_SIZE) && (addr < FLASH_BASE + FLASH_SIZE))
//    {
//        bank = FLASH_BANK_2;
//    }
//    else
//    {
//        bank = 0xFFFFFFFF;      /* 地址溢出 */
//    }

    return bank;
}

/**
 * @brief       向指定地址写入指定长度的数据
 * @param       waddr : 指定写入数据的起始地址
 * @param       pbuf  : 保存写入数据的起始地址
 * @param       length: 指定写入数据的长度，单位：字
 * @retval      无
 */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t addrx;
    uint32_t endaddr;
    uint32_t first_sector = 0;
    uint32_t num_sectors = 0;
    uint32_t bank_number = 0;
    uint32_t erase_addr;                                                                    /* 擦除错误，这个值为发生错误的扇区地址 */
    FLASH_EraseInitTypeDef flash_erase_init = {0};
    HAL_StatusTypeDef status = HAL_OK;

    if ((waddr < STM32_FLASH_BASE) ||                                                       /* 指定地址小于flash的起始地址 */
        (waddr > (STM32_FLASH_BASE + STM32_FLASH_SIZE)) ||                                  /* 指定地址大于flash的末地址 */
         waddr % 4)                                                                         /* 指定地址没有按4字节对齐 */
    {
        return;                                                                             /* 非法地址 */
    }

//    HAL_ICACHE_Disable();                                                                   /* 禁用指令缓存 */
//    HAL_FLASH_Unlock();                                                                     /* FLASH解锁 */

//    addrx = waddr;                                                                          /* 数据写入的起始地址 */
//    endaddr = waddr + length * 4;                                                           /* 数据写入的结束地址 */

//    first_sector = stmflash_get_flash_sector(addrx);                                        /* 获取要擦除的第一个扇区 */
//    num_sectors = stmflash_get_flash_sector(endaddr) - first_sector + 1;                    /* 获取要擦除的扇区数 */
//    bank_number = stmflash_get_flash_bank(addrx);                                           /* 获取给定地址的bank */

//    if (addrx < 0x0800FFFF)                                                                 /* 只有主存储区，才需要进行擦除操作 */
//    {
//        while (addrx < endaddr)                                                             /* 扫清一切障碍(对非FFFFFFFF的地方,先擦除) */
//        {
//            if ((uint32_t)stmflash_read_word(addrx) != 0xFFFFFFFF)                          /* 存在非0xFFFFFFFF */
//            {
//                flash_erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;                       /* 以扇区擦除的方式 */
//                flash_erase_init.Banks = bank_number;                                       /* 擦除所在的bank */
//                flash_erase_init.Sector = first_sector;                                     /* 要擦除的第一个扇区 */
//                flash_erase_init.NbSectors = num_sectors;                                   /* 要擦除的扇区数 */
//                status = HAL_FLASHEx_Erase( &flash_erase_init, &erase_addr);

//                if (status == HAL_OK)                                                       /* 擦除成功 */
//                {
//                    break;
//                }
//            }
//            else                                                                            /* 无需擦除 */
//            {
//                addrx += 4;
//            }
//        }
//    }
//    
//   
//		if (status == HAL_OK)                                                                   /* 擦除扇区没有错误 */
//    {
//        for(int i = 0; i < length; i++)
//        {
//            HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, waddr, (uint32_t)&pbuf[i]);
//            waddr += 4;                                                                     /* 指向下一个半字 */
//        }
//    }
//    
//    HAL_FLASH_Lock();                                                                       /* 上锁 */
//    HAL_ICACHE_Enable();                                                                    /* 启用指令缓存 */
}

/***************************************************************************************************************************************/
/* 测试用代码 */

/**
 * @brief       测试写数据(写1个字)
 * @param       waddr : 起始地址
 * @param       wdata : 要写入的数据
 * @retval      读取到的数据
 */
void stmflash_test_write(uint32_t waddr, uint32_t wdata)
{
    stmflash_write(waddr, &wdata, 1);                                                       /* 写入一个字 */
}












