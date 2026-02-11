Это ветка с "черновиками" - прогой, которая пока не до конца правильно работает
Обновление: 11.02

компьютер (192.168.50.50) пытается отправить пинг на 192.168.50.100, но не получает ответа от STM32. 
Вместо этого ответ приходит от самого компьютера (192.168.50.50).

Интерфейс: 192.168.50.50 --- 0xe  (это ваш Wi-Fi)
  192.168.0.100         00-80-e1-00-00-00     статический  ← ОШИБКА!
                          (правильный MAC STM32, но НЕПРАВИЛЬНЫЙ IP!)

ЧТО БЫЛО ИСПРАВЛЕНО:
1. Было: Конфликт пинов USART3 - пытались настроить PB10/PD9, но не работало
Стало: Вернулись на PD8(TX)/PD9(RX) - РАБОЧАЯ КОНФИГУРАЦИЯ
Исправление: Ручная настройка GPIO в MX_GPIO_Init() с AF7_USART3

2. ПАМЯТЬ FREERTOS. Heap free вырос с 32KB до 122KB
3. Закомментирован //MPU_Config(); в main.c

- Текущий вывод в путти:
=== USART3 INIT OK (PD8/PD9) ===

=== SYSTEM START ===
CPU Clock: 64000000 Hz
Starting FreeRTOS...
Starting scheduler...

[ETH] Ethernet task started
[ETH] FreeRTOS heap free: 122752 bytes
[ETH] Startup delay 0...
[ETH] Startup delay 1...
[ETH] Startup delay 2...
[ETH] Initializing LWIP...
[LWIP] Starting tcpip_init...
[ETH] Initializing Ethernet hardware...
[ETH] MAC: 00:80:E1:00:00:00
[LWIP] Ethernet link thread created
[ETH] Waiting for Ethernet hardware...
[ETH] Waiting for link: 2 sec
[ETH] Waiting for link: 4 sec
[ETH] Waiting for link: 6 sec
[ETH] Waiting for link: 8 sec
[ETH] Waiting for link: 10 sec
[ETH] Waiting for link: 12 sec
[ETH] Waiting for link: 14 sec
[ETH] Waiting for link: 16 sec
[ETH] Waiting for link: 18 sec
[ETH] Waiting for link: 20 sec
[ETH] Waiting for link: 22 sec
[ETH] Waiting for link: 24 sec
[ETH] Waiting for link: 26 sec
[ETH] Waiting for link: 28 sec
[ETH] Waiting for link: 30 sec
[ETH] Starting TCP server on port 2001...
[TCP] Listening on port 2001
[ETH] TCP Server ready!
[ETH] Heartbeat 1 - Link UP
[ETH] Heartbeat 2 - Link UP
