#include "main.h"

void initTooling();

void TOOLING_USART1_UART_Init(void);
void TOOLING_USART2_UART_Init(void);


uint8_t testDTS();

uint8_t testND06();

void testIO();

void testInX();

typedef union {
    uint8_t toolingRaw; // Õû¸ö×Ö½Ú
    struct {
        uint8_t bit0RS485 : 1;
        uint8_t bit1UART : 1;
        uint8_t bit2OUT : 1;
        uint8_t bit3IN : 1;
        uint8_t bit4DTSdata : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
    } bits;
} ToolingBits;


extern ToolingBits toolingBits;
