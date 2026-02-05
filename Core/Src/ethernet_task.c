/*
 * ethernet_task.c
 *
 *  Created on: Feb 4, 2026
 *      Author: Egenie
 */

#include "ethernet_task.h"
#include "lwip.h"
#include "lwip/netif.h"
#include "cmsis_os.h"
#include "debug_uart.h"

extern struct netif gnetif;

void EthernetTask(void *argument)
{
    DebugUART_Print("[ETH] Ethernet task started\r\n");

    /* Инициализация lwIP (создаёт tcpip_thread) */
    MX_LWIP_Init();

    /* Ждём физический линк */
    while (!netif_is_link_up(&gnetif))
        osDelay(100);

    netif_set_up(&gnetif);

    DebugUART_Print("[ETH] Netif UP\r\n");
    DebugUART_Print("[ETH] IP   : %s\r\n",
                    ipaddr_ntoa(&gnetif.ip_addr));

    /*
     * ВАЖНО:
     * TCP-сервер НЕ запускается здесь!
     * Он запускается ТОЛЬКО из ethernet_link_thread
     * через tcpip_callback().
     */

    for (;;)
    {
        osDelay(1000);
    }
}
