Это ветка с "черновиками" - прогой, которая пока не до конца правильно работает

Изменения:
1. Удален sys_arch.c
2. Добавлены файлы FreeRTOS:
   - freertos.c, FreeRTOSConfig.h
3. Добавлена Ethernet задача:
   - ethernet_task.c, ethernet_task.h
4. Обновлены основные файлы проекта
5. Обновлена конфигурация LWIP

Проблема (вывод в путти):

=== System start ===
Starting scheduler...
[ETH] Ethernet task started
[LWIP] tcpip_init
[LWIP] Netif added
[ETH] Netif UP
[ETH] IP   : 192.168.1.50

=== System start ===
Starting scheduler...
[ETH] Ethernet task started
[LWIP] tcpip_init
[LWIP] Netif added
