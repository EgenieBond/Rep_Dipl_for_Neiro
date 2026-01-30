Последняя версия (30.01)

57600   - скорость в Путти

- реализован статический IP

[LWIP] >>> ENTER MX_LWIP_Init <<< [LWIP] Static IP configured IP: 192.168.1.50 MASK: 255.255.255.0 GW: 192.168.1.1 ETH link: UP IP: 192.168.1.50 MASK: 255.255.255.0 GW: 192.168.1.1 [LWIP] >>> ENTER MX_LWIP_Init <<< [LWIP] Static IP configured IP: 192.168.1.50 MASK: 255.255.255.0 GW: 192.168.1.1 ETH link: DOWN IP: 192.168.1.50 MASK: 255.255.255.0 GW: 192.168.1.1 [ETH] Link UP

На момент main() линк уже поднят. Это значит:
- PHY жив
- auto-negotiation прошла
- MAC стартовал
- netif уже link_up

- 👉 Проблем с LWIP / Ethernet больше НЕТ

- Пока просто настроила сервер, не запускала его еще
