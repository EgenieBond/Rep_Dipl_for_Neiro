Последняя версия (27.01)

57600   - скорость в Путти

Что точно работает:
1. PHY и MAC — полностью ОК
  [ETH] PHY: 100M FULL
  [ETH] MAC started, netif UP

✔ PHY инициализируется
✔ Автосогласование есть
✔ MAC стартует
✔ netif поднимается
👉 Железо и RMII — не проблема

-----------------------------
2. DHCP реально запускается
  [DHCP] dhcp_start() called

✔ dhcp_start() вызывается
✔ DHCP state меняется

-----------------------------
DHCP доходит до SELECTING
  [DHCP] state=6 tries=1
  state = 6 → DHCP_SELECTING
значит:
DHCP DISCOVER уходит
tries увеличился (tries=1)
таймеры работают

-----------------------------
❗ Но DHCP OFFER не принимается
Нет ни одного RX-пакета. В логах НИ РАЗУ нет чего-то типа:
[ETH] RX packet len = ...

👉 Значит:
ответы от DHCP-сервера физически приходят, но не попадают в lwIP
Не работает приём Ethernet кадров (RX path). DHCP OFFER не доходит до ethernetif_input()
