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
#include "raw_tcp_server.h"

extern struct netif gnetif;

/* Очередь для передачи данных Ethernet -> Core */
osMessageQueueId_t eth_to_core_queue = NULL;

void EthernetTask(void *argument)
{
    DebugUART_Print("\r\n[ETH] Ethernet task started\r\n");

    /* Проверка памяти */
    uint32_t free_heap = xPortGetFreeHeapSize();
    DebugUART_Print("[ETH] FreeRTOS heap free: %lu bytes\r\n", free_heap);

    /* Даем время системе */
    for(int i = 0; i < 3; i++) {
        DebugUART_Print("[ETH] Startup delay %d...\r\n", i);
        osDelay(1000);
    }

    /* Инициализируем lwIP */
    DebugUART_Print("[ETH] Initializing LWIP...\r\n");
    MX_LWIP_Init();

    /* Выводим IP адрес платы */
    DebugUART_Print("[ETH] My IP: %d.%d.%d.%d\r\n",
                   ip4_addr1(&gnetif.ip_addr),
                   ip4_addr2(&gnetif.ip_addr),
                   ip4_addr3(&gnetif.ip_addr),
                   ip4_addr4(&gnetif.ip_addr));

    /* Запускаем TCP сервер - НЕ ЖДЕМ ЛИНК! */
    DebugUART_Print("[ETH] Starting TCP server on port 2001...\r\n");
    RawTcpServer_Init();

    DebugUART_Print("[ETH] TCP Server ready!\r\n");
    DebugUART_Print("[ETH] Connect with: telnet 10.0.0.100 2001\r\n");

    /* Основной цикл */
    int counter = 0;
    for(;;) {
        osDelay(5000);

        if (netif_is_link_up(&gnetif)) {
            DebugUART_Print("[ETH] Heartbeat %d - Link UP\r\n", ++counter);
        } else {
            DebugUART_Print("[ETH] Heartbeat %d - Link DOWN (check cable)\r\n", ++counter);
        }
    }
}
