/*
 * debug_uart.c
 *
 *  Created on: Jan 27, 2026
 *      Author: Egenie
 */

#include "main.h"
#include <stdarg.h>
#include <stdio.h>

extern UART_HandleTypeDef huart3;

void DebugUART_Print(const char *fmt, ...)
{
    char buffer[256];
    va_list args;

    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (len > 0)
    {
        HAL_UART_Transmit(&huart3, (uint8_t*)buffer, len, 1000);
    }
}
