/*
 * debug_uart.c
 *
 *  Created on: Jan 27, 2026
 *      Author: Egenie
 */

#include "debug_uart.h"
#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <stdarg.h>

extern UART_HandleTypeDef huart3;

void DebugUART_Init(void)
{
    // UART3 уже инициализирован Cube'ом
    DebugUART_Print("\r\n[DEBUG] UART ready\r\n");
}

void DebugUART_Print(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len > 0)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)buf, len, HAL_MAX_DELAY);
    }
}

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}
