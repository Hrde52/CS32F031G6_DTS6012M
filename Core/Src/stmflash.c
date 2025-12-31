#include "stmflash.h"
#include "main.h"
#include "stm32f0xx_hal.h"

#define FLASH_TYPEPROGRAM_WORD (0x02U)

/* FLASH */
#define STM32_FLASH_BASE 0x08000000
#define STM32_FLASH_SIZE 0x00008000
#define FLASH_PAGE_SIZE 0x400U

#define IS_FLASH_ADDRESS(addr) (((addr) >= STM32_FLASH_BASE) &&                          \
                                ((addr) <= (STM32_FLASH_BASE + STM32_FLASH_SIZE - 4)) && \
                                ((addr) % 4 == 0))

uint32_t stmflash_read_word(uint32_t faddr)
{
    if (!IS_FLASH_ADDRESS(faddr))
        return 0xFFFFFFFF;
    return *(__IO uint32_t *)faddr;
}

HAL_StatusTypeDef stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    HAL_StatusTypeDef status = HAL_OK;
    FLASH_EraseInitTypeDef erase_init;
    uint32_t page_error = 0;
    uint32_t programmed = 0;
    uint32_t page_start, page_end;

    /* ??????? */
    if (!IS_FLASH_ADDRESS(waddr) || !IS_FLASH_ADDRESS(waddr + length * 4 - 4))
        return HAL_ERROR;

    /* ?????????? */
    page_start = (waddr - STM32_FLASH_BASE) / FLASH_PAGE_SIZE;
    page_end = (waddr + length * 4 - 1 - STM32_FLASH_BASE) / FLASH_PAGE_SIZE;

    /* ??Flash */
    HAL_FLASH_Unlock();

    /* ????Flash???? */
    //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

    /* ?????? */
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init.PageAddress = STM32_FLASH_BASE + page_start * FLASH_PAGE_SIZE;
    erase_init.NbPages = page_end - page_start + 1;

    /* ?????? */
    status = HAL_FLASHEx_Erase(&erase_init, &page_error);
    if (status != HAL_OK)
    {
        HAL_FLASH_Lock();
        return status;
    }

    /* ????? */
    for (programmed = 0; programmed < length; programmed++)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                                   waddr + programmed * 4,
                                   pbuf[programmed]);
        if (status != HAL_OK)
            break;
    }

    /* ??Flash */
    HAL_FLASH_Lock();

    return status;
}

void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;

    /* ??????? */
    if (!IS_FLASH_ADDRESS(raddr) || !IS_FLASH_ADDRESS(raddr + length * 4 - 4))
        return;

    /* ????? */
    for (i = 0; i < length; i++)
    {
        pbuf[i] = *(__IO uint32_t *)(raddr + i * 4);
    }
}
