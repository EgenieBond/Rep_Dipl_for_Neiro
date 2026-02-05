/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "lwip.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "debug_uart.h"

struct netif gnetif;

void MX_LWIP_Init(void)
{
    DebugUART_Print("[LWIP] tcpip_init\r\n");

    tcpip_init(NULL, NULL);

    ip4_addr_t ipaddr, netmask, gw;
    IP4_ADDR(&ipaddr,  192,168,1,50);
    IP4_ADDR(&netmask, 255,255,255,0);
    IP4_ADDR(&gw,      192,168,1,1);

    netif_add(&gnetif, &ipaddr, &netmask, &gw,
              NULL, ethernetif_init, tcpip_input);

    netif_set_default(&gnetif);

    DebugUART_Print("[LWIP] Netif added\r\n");
}

