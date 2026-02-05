Это ветка с "черновиками" - прогой, которая пока не до конца правильно работает

Текущая проблема: 
Первый проход — НОРМАЛЬНЫЙ
✔ PHY инициализировался
✔ Link поднялся
✔ DHCP стартовал
Второй проход — ОШИБОЧНЫЙ
❌ PHY init FAILED
❌ PHY state = 4294967291
❌ low_level_init() вызывается ещё раз
КЛЮЧЕВАЯ ПРОБЛЕМА: low_level_init() вызывается ПОВТОРНО

[LWIP] >>> ENTER MX_LWIP_Init <<<
[LWIP] MX_LWIP_Init @ 0x8007161, file=../LWIP/App/lwip.c
[ETH] low_level_init()
[ETH] LAN8742 init...
[ETH] PHY init OK
[LWIP] Init done, waiting for link UP
ETH link: DOWN
IP: 0.0.0.0
MASK: 0.0.0.0
GW: 0.0.0.0
[ETH] Link UP -> start MAC
[DHCP] dhcp_start()
[ETH] low_level_init()
[ETH] MAC started
[ETH] PHY init FAILED
[LWIP] Init done, waiting for link UP
ETH link: DOWN
IP: 0.0.0.0
MASK: 0.0.0.0
GW: 0.0.0.0
[ETH] Link UP (PHY state=4294967291)
[ETH] Link UP -> netif UP + DHCP

