#include "stmflash.h"
#include "main.h"


#include "stm32f0xx_hal.h"

//#define FLASH_TYPEPROGRAM_HALFWORD   (0x01U)
//#define FLASH_TYPEPROGRAM_WORD       (0x02U)
//#define FLASH_TYPEPROGRAM_DOUBLEWORD (0x03U)

#define STM32_FLASH_BASE        0x08000000
#define STM32_FLASH_SIZE        0x00008000
#define FLASH_WAITETIME         50000


//#define FLASH_PAGE_SIZE         0x400U
#define FLASH_PAGE_NUM          (STM32_FLASH_SIZE / FLASH_PAGE_SIZE)


#define GET_FLASH_PAGE(addr)    (((addr) - STM32_FLASH_BASE) / FLASH_PAGE_SIZE)

#define IS_FLASH_ADDRESS(addr)  (((addr) >= STM32_FLASH_BASE) && \
                                ((addr) <= (STM32_FLASH_BASE + STM32_FLASH_SIZE - 4)))

uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(__IO uint32_t*)faddr;
}


void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    HAL_StatusTypeDef status = HAL_OK;
    FLASH_EraseInitTypeDef erase_init;
    uint32_t page_error = 0;
    uint32_t programmed = 0;
    
    if (!IS_FLASH_ADDRESS(waddr) || !IS_FLASH_ADDRESS(waddr + length * 4 - 1))
        return;
    
    HAL_FLASH_Unlock();
      
    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init.PageAddress = waddr;
    erase_init.NbPages = 1;
    
    
    status = HAL_FLASHEx_Erase(&erase_init, &page_error);
    if (status != HAL_OK)
    {
        HAL_FLASH_Lock();
        return;
    }
    
    for (programmed = 0; programmed < length; programmed++)
    {
        status = HAL_FLASH_Program(FLASH_PROC_PROGRAMWORD/*FLASH_TYPEPROGRAM_WORD*/, 
                                 waddr + programmed * 4, 
                                 pbuf[programmed]);
        if (status != HAL_OK)
            break;
    }
    
    HAL_FLASH_Lock();
}


void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;
    
    
    if (!IS_FLASH_ADDRESS(raddr) || !IS_FLASH_ADDRESS(raddr + length * 4 - 1))
        return;
    
    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr + i * 4);
    }
}

//uint32_t stmflash_read_word(uint32_t faddr)
//{
//    return *(volatile uint32_t*)faddr;
//}


//void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
//{
//    uint32_t i;
//    for(i = 0; i < length; i++)
//    {
//        pbuf[i] = stmflash_read_word(raddr + i * 4); 
//    }
//}


//static uint8_t stmflash_wait_done(uint32_t time)
//{
//    uint32_t wait_time = 0;
//    
//    while(FLASH->SR & FLASH_SR_BSY) 
//    {
//        wait_time++;
//        if(wait_time > time) return 1; 
//    }
//    
//    if(FLASH->SR & FLASH_SR_EOP)      
//    {
//        FLASH->SR = FLASH_SR_EOP;     
//    }
//    
//    return 0;
//}


//static void stmflash_unlock(void)
//{
//    if(FLASH->CR & FLASH_CR_LOCK)     
//    {
//        FLASH->KEYR = FLASH_KEY1;   // 0x45670123;     
//        FLASH->KEYR = FLASH_KEY2;   // 0xCDEF89AB;     
//    }
//}


//static void stmflash_lock(void)
//{
//    FLASH->CR |= FLASH_CR_LOCK;       
//}


//static uint8_t stmflash_erase_page(uint32_t paddr)
//{
//    uint8_t result = 0;
//    HAL_StatusTypeDef flashStatus = FLASH_WaitForLastOperation(100);
//    stmflash_unlock();                
//    if(flashStatus == HAL_OK)
//		{
//			FLASH->CR |= FLASH_CR_PER;        
//			FLASH->AR = paddr;                
//			FLASH->CR |= FLASH_CR_STRT; 
//		}
//          
//    result = stmflash_wait_done(FLASH_WAITETIME); 
//    
//    FLASH->CR &= ~FLASH_CR_PER;       
//    stmflash_lock();                  
//    
//    return result;
//}


//static uint8_t stmflash_write_word(uint32_t waddr, uint32_t wdata)
//{
//    uint8_t result = 0;
//    
//    stmflash_unlock();                
//    
//    FLASH->CR |= FLASH_CR_PG;         
//    
//    *(volatile uint32_t*)waddr = wdata; 
//    result = stmflash_wait_done(FLASH_WAITETIME); 
//    
//    FLASH->CR &= ~FLASH_CR_PG;        
//    stmflash_lock();                  
//    
//    return result;
//}


//void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
//{
//    uint32_t i;
//    uint32_t current_addr = waddr;
//		
//		if ((waddr < STM32_FLASH_BASE) ||                                                       /* 指定地址小于flash的起始地址 */
//        (waddr > (STM32_FLASH_BASE + STM32_FLASH_SIZE)) ||                                  /* 指定地址大于flash的末地址 */
//         waddr % 4)                                                                         /* 指定地址没有按4字节对齐 */
//    {
//        return;                                                                             /* 非法地址 */
//    }
//		
//		//& ~

//    // compute erase page number
//    uint32_t start_page = (waddr  - STM32_FLASH_BASE) / FLASH_PAGE_SIZE;  //(waddr - 0x08000000
//    uint32_t end_page = (waddr + length * 4 - 1  - STM32_FLASH_BASE) / FLASH_PAGE_SIZE;
//    
//    for(uint32_t page = start_page; page <= end_page; page++)
//    {
//        uint32_t page_addr = STM32_FLASH_BASE + page * FLASH_PAGE_SIZE;
//        stmflash_erase_page(page_addr);
//    }
//    
//    for(i = 0; i < length; i++)
//    {
//        //stmflash_write_word(current_addr, pbuf[i]);
//			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, current_addr, (uint32_t)&pbuf[i]);
//      current_addr += 4; 
//    }
//		stmflash_lock();
//		return;
//}


//void stmflash_test_write(uint32_t waddr, uint32_t wdata)
//{
//    stmflash_write_word(waddr, wdata);
//}