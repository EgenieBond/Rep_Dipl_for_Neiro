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

    if (free_heap < 30000) {
        DebugUART_Print("[ETH] WARNING: Low memory! LWIP may fail!\r\n");
    }

    /* Даем время системе */
    for(int i = 0; i < 3; i++) {
        DebugUART_Print("[ETH] Startup delay %d...\r\n", i);
        osDelay(1000);
    }

    /* Инициализируем lwIP */
    DebugUART_Print("[ETH] Initializing LWIP...\r\n");
    MX_LWIP_Init();

    /* Ждем инициализацию Ethernet */
    DebugUART_Print("[ETH] Waiting for Ethernet hardware...\r\n");

    /* Проверяем линк каждые 500ms */
    int link_wait_time = 0;
    while(link_wait_time < 60) {  // Максимум 30 секунд
        osDelay(500);
        link_wait_time++;

        if ((link_wait_time % 4) == 0) {  // Каждые 2 секунды
            DebugUART_Print("[ETH] Waiting for link: %d sec\r\n", link_wait_time/2);
        }
    }

    /* Запускаем TCP сервер независимо от линка */
    DebugUART_Print("[ETH] Starting TCP server on port 2001...\r\n");
    RawTcpServer_Init();

    DebugUART_Print("[ETH] TCP Server ready!\r\n");
    //DebugUART_Print("[ETH] Connect with: telnet 10.0.0.100 2001\r\n");
    //DebugUART_Print("[ETH] Or use python: vs_can_lib.VSCAN('10.0.0.100:2001')\r\n");

    /* Основной цикл */
    int counter = 0;
    for(;;) {
        osDelay(5000);

        /* Проверяем статус линка */
        if (netif_is_link_up(&gnetif)) {
            DebugUART_Print("[ETH] Heartbeat %d - Link UP\r\n", ++counter);
        } else {
            DebugUART_Print("[ETH] Heartbeat %d - Link DOWN (check cable)\r\n", ++counter);
        }
    }
}
