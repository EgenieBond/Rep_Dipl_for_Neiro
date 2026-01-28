#ifndef INC_DEBUG_UART_H_
#define INC_DEBUG_UART_H_

#include <stdint.h>

#pragma once
#include <stdarg.h>

void DebugUART_Init(void);
void DebugUART_Print(const char *fmt, ...);

#endif /* INC_DEBUG_UART_H_ */

