/*
 * sys_arch.c
 *
 *  Created on: Jan 27, 2026
 *      Author: Egenie
 */

#include "lwip/sys.h"
#include "stm32h7xx_hal.h"

sys_prot_t sys_arch_protect(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

void sys_arch_unprotect(sys_prot_t pval)
{
    if (pval == 0)
    {
        __enable_irq();
    }
}

