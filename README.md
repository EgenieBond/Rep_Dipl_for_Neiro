Последняя версия (04.02)

57600   - скорость в Путти
--------
- cd C:\Users\Egenie\Desktop
- py simple_test.py   - запуск клиента
- py test_client.py   - запуск теста

вот такой вывод в путти: 
=== System start === 
Starting scheduler... 
[ETH] Ethernet task started 
[LWIP] tcpip_init 
[LWIP] Netif up 
[ETH] lwIP initialized 
[ETH] IP : 192.168.1.50 
[ETH] MASK : 255.255.255.0 
[ETH] GW : 192.168.1.1

✔ FreeRTOS стартовал корректно
✔ EthernetTask реально выполняется
✔ tcpip_thread создан (tcpip_init)
✔ сетевой интерфейс поднят (Netif up)
✔ статический IP применился
✔ lwIP живёт в правильном режиме NO_SYS = 0
✔ архитектура полностью соответствует ВКР

сервер пока не сделан

Инициализация стека lwIP выполняется внутри выделенной задачи FreeRTOS. После запуска системного потока tcpip_thread сетевой интерфейс переводится в активное состояние и получает заранее заданные параметры статического IP-адреса.
