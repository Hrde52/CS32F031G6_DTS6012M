
#ifndef __STMFLASH_H
#define __STMFLASH_H

#include <main.h>


/* FLASH��ʼ��ַ */
#define STM32_FLASH_BASE        0x08000000                                  /* STM32 FLASH ��ʼ��ַ */
#define STM32_FLASH_SIZE        0x00008000 //0x200000                       /* STM32 FLASH �ܴ�С */
#define FLASH_WAITETIME         50000                                       /* Flash�ȴ���ʱʱ�� */



uint32_t stmflash_read_word(uint32_t faddr);                                /* ������ */
HAL_StatusTypeDef  stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length);       /* ��ָ����ַ��ʼд��ָ�����ȵ����� */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length);        /* ��ָ����ַ��ʼ����ָ�����ȵ����� */
void stmflash_test_write(uint32_t waddr, uint32_t wdata);                   /* ����д�� */

#endif











