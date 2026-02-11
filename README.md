Это ветка с "черновиками" - прогой, которая пока не до конца правильно работает
Обновление: 11.02

-------------------- ✅ ЧТО УЖЕ РАБОТАЕТ:
UART (PD8/PD9) - полностью настроен, вывод в Putty есть
FreeRTOS - задачи создаются, планировщик работает
Heap - 122KB свободно, достаточно для LwIP
Ethernet PHY - LINK UP, кабель определяется
Прием пакетов (RX) - плата видит ARP-запросы от ПК
TCP сервер - создан, слушает порт 2001
IP-адрес - 10.0.0.100/8 (статический)
---------------- ⚠️ В ПРОЦЕССЕ:
Отправка пакетов (TX) - первый пакет уходит, потом дескрипторы заканчиваются
ARP-ответы - плата видит запросы для своего IP, но не может ответить

-------------------------- Вывод в путти:
=== USART3 INIT OK (PD8/PD9) ===

=== SYSTEM START ===
CPU Clock: 64000000 Hz
Starting FreeRTOS...

=== USART3 INIT OK (PD8/PD9) ===

=== SYSTEM START ===
CPU Clock: 64000000 Hz
Starting FreeRTOS...
Starting scheduler...

[ETH] Ethernet task started
[ETH] FreeRTOS heap free: 122760 bytes
[ETH] Startup delay 0...
[ETH] Startup delay 1...
[ETH] Startup delay 2...
[ETH] Initializing LWIP...
[LWIP] Starting tcpip_init...
[LWIP] tcpip_init complete
[LWIP] IP: 10.0.0.100
[ETH] Initializing Ethernet hardware...
[ETH] MAC: 00:80:E1:00:00:00
[TX] Transmit OK
[LWIP] Netif added
[LWIP] Netif up
[LWIP] Ethernet link thread created
[ETH] My IP: 10.0.0.100
[ETH] Starting TCP server on port 2001...
[TCP] Listening on port 2001
[ETH] TCP Server ready!
[ETH] Connect with: telnet 10.0.0.100 2001
[ETH] Received packet #1, len=60, tot_len=60
[ARP] Received packet, opcode: 1
[ARP] Request for IP: 10.0.0.2
[ETH] Received packet #2, len=110, tot_len=110
[ETH] Received packet #3, len=110, tot_len=110
[ETH] Received packet #4, len=110, tot_len=110
[ETH] Received packet #5, len=110, tot_len=110
[ETH] Received packet #6, len=110, tot_len=110
[ETH] Received packet #7, len=110, tot_len=110
[ETH] Received packet #8, len=110, tot_len=110
[ETH] Received packet #9, len=110, tot_len=110
[ETH] Received packet #10, len=110, tot_len=110
[ETH] Received packet #11, len=110, tot_len=110
[ETH] Received packet #12, len=110, tot_len=110
[ETH] Received packet #13, len=110, tot_len=110
[ETH] Received packet #14, len=110, tot_len=110
[ETH] Heartbeat 1 - Link UP
[ETH] Heartbeat 2 - Link UP
[ETH] Heartbeat 3 - Link UP
[ETH] Heartbeat 4 - Link UP
[ETH] Heartbeat 5 - Link UP
[ETH] Heartbeat 6 - Link UP
[ETH] Heartbeat 7 - Link UP
[ETH] Heartbeat 8 - Link UP
[ETH] Heartbeat 9 - Link UP
[ETH] Heartbeat 10 - Link UP
[ETH] Heartbeat 11 - Link UP
[ETH] Received packet #15, len=92, tot_len=92
[ETH] Received packet #16, len=92, tot_len=92
[ETH] Received packet #17, len=92, tot_len=92
[ETH] Heartbeat 12 - Link UP
[ETH] Received packet #18, len=92, tot_len=92
[ETH] Received packet #19, len=92, tot_len=92
[ETH] Received packet #20, len=92, tot_len=92
[ETH] Heartbeat 13 - Link UP
[ETH] Heartbeat 14 - Link UP
[ETH] Heartbeat 15 - Link UP
[ETH] Received packet #21, len=92, tot_len=92
[ETH] Received packet #22, len=92, tot_len=92
[ETH] Received packet #23, len=92, tot_len=92
[ETH] Heartbeat 16 - Link UP
[ETH] Received packet #24, len=92, tot_len=92
[ETH] Received packet #25, len=92, tot_len=92
[ETH] Received packet #26, len=92, tot_len=92
[ETH] Heartbeat 17 - Link UP
[ETH] Heartbeat 18 - Link UP
[ETH] Heartbeat 19 - Link UP
[ETH] Received packet #27, len=60, tot_len=60
[ARP] Received packet, opcode: 1
[ARP] Request for IP: 10.0.0.100
[ARP] !!! OUR IP - FORCING REPLY !!!
[ARP] Calling etharp_output...
[TX] Transmit OK
[ARP] etharp_output returned: 0


