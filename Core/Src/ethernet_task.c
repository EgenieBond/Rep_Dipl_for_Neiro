/*
 * ethernet_task.c
 *
 *  Created on: Feb 4, 2026
 *      Author: Egenie
 */

#include "ethernet_task.h"
#include "lwip.h"
#include "lwip/netif.h"
#include "debug_uart.h"
#include "cmsis_os.h"

extern struct netif gnetif;

void EthernetTask(void *argument)
{
    DebugUART_Print("[ETH] Ethernet task started\r\n");

    /* Initialize lwIP stack */
    MX_LWIP_Init();

    DebugUART_Print("[ETH] lwIP initialized\r\n");

    DebugUART_Print("[ETH] IP   : %s\r\n", ipaddr_ntoa(&gnetif.ip_addr));
    DebugUART_Print("[ETH] MASK : %s\r\n", ipaddr_ntoa(&gnetif.netmask));
    DebugUART_Print("[ETH] GW   : %s\r\n", ipaddr_ntoa(&gnetif.gw));

    for (;;)
    {
        /* lwIP работает в своем tcpip_thread */
        osDelay(1000);
    }
}
