#include "stmflash.h"
#include "main.h"


uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t*)faddr;
}


void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;
    for(i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr + i * 4); // ????4??
    }
}


static uint8_t stmflash_wait_done(uint32_t time)
{
    uint32_t wait_time = 0;
    
    while(FLASH->SR & FLASH_SR_BSY) // ??Flash????
    {
        wait_time++;
        if(wait_time > time) return 1; // ??
    }
    
    if(FLASH->SR & FLASH_SR_EOP)      // ??????
    {
        FLASH->SR = FLASH_SR_EOP;     // ????
    }
    
    return 0;
}


static void stmflash_unlock(void)
{
    if(FLASH->CR & FLASH_CR_LOCK)     // ???????
    {
        FLASH->KEYR = 0x45670123;     // ????1
        FLASH->KEYR = 0xCDEF89AB;     // ????2
    }
}


static void stmflash_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;       // ??Flash
}


static uint8_t stmflash_erase_page(uint32_t paddr)
{
    uint8_t result = 0;
    
    stmflash_unlock();                // ??Flash
    
    FLASH->CR |= FLASH_CR_PER;        // ?????
    FLASH->AR = paddr;                // ?????
    FLASH->CR |= FLASH_CR_STRT;       // ????
    
    result = stmflash_wait_done(FLASH_WAITETIME); // ??????
    
    FLASH->CR &= ~FLASH_CR_PER;       // ???????
    stmflash_lock();                  // ??Flash
    
    return result;
}


static uint8_t stmflash_write_word(uint32_t waddr, uint32_t wdata)
{
    uint8_t result = 0;
    
    stmflash_unlock();                // ??Flash
    
    FLASH->CR |= FLASH_CR_PG;         // ????
    
    *(volatile uint32_t*)waddr = wdata; // ????
    result = stmflash_wait_done(FLASH_WAITETIME); // ??????
    
    FLASH->CR &= ~FLASH_CR_PG;        // ??????
    stmflash_lock();                  // ??Flash
    
    return result;
}


void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;
    uint32_t current_addr = waddr;
    
    // ????????
    uint32_t start_page = (waddr - STM32_FLASH_BASE) / FLASH_PAGE_SIZE;
    uint32_t end_page = (waddr + length * 4 - 1 - STM32_FLASH_BASE) / FLASH_PAGE_SIZE;
    
    // ????????
    for(uint32_t page = start_page; page <= end_page; page++)
    {
        uint32_t page_addr = STM32_FLASH_BASE + page * FLASH_PAGE_SIZE;
        stmflash_erase_page(page_addr);
    }
    
    // ????
    for(i = 0; i < length; i++)
    {
        stmflash_write_word(current_addr, pbuf[i]);
        current_addr += 4; // ????4??
    }
}


void stmflash_test_write(uint32_t waddr, uint32_t wdata)
{
    stmflash_write_word(waddr, wdata);
}