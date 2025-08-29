
#ifndef __STMFLASH_H
#define __STMFLASH_H

#include <main.h>


/* FLASH起始地址 */
#define STM32_FLASH_BASE        0x08000000                                  /* STM32 FLASH 起始地址 */
#define STM32_FLASH_SIZE        0x00008000 //0x200000                                    /* STM32 FLASH 总大小 */
#define FLASH_WAITETIME         50000                                       /* Flash等待超时时间 */


/* 函数声明 */
uint32_t stmflash_read_word(uint32_t faddr);                                /* 读出字 */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length);       /* 从指定地址开始写入指定长度的数据 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length);        /* 从指定地址开始读出指定长度的数据 */
void stmflash_test_write(uint32_t waddr, uint32_t wdata);                   /* 测试写入 */

#endif











