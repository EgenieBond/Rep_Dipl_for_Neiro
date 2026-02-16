/*
 * raw_tcp_server.c
 *
 * TCP RAW server (single client)
 */

#include "lwip/tcp.h"
#include "debug_uart.h"
#include <string.h>

#define TCP_SERVER_PORT 2001
#define RX_BUFFER_SIZE 256

static struct tcp_pcb *server_pcb = NULL;
static struct tcp_pcb *client_pcb = NULL;
static char rx_buffer[RX_BUFFER_SIZE];
static int rx_buffer_pos = 0;

/* ===== CALLBACKS ===== */

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
    if (!p)
    {
        DebugUART_Print("[TCP] Client disconnected\r\n");
        tcp_close(tpcb);
        client_pcb = NULL;
        rx_buffer_pos = 0;  // Сбрасываем буфер
        return ERR_OK;
    }

    if (err != ERR_OK)
    {
        pbuf_free(p);
        return err;
    }

    tcp_recved(tpcb, p->tot_len);

    /* Обрабатываем полученные данные */
    uint8_t *data = (uint8_t *)p->payload;
    uint16_t len = p->len;

    for (uint16_t i = 0; i < len; i++)
    {
        if (rx_buffer_pos < RX_BUFFER_SIZE - 1)
        {
            rx_buffer[rx_buffer_pos++] = data[i];

            /* Если получили \r (CR), то команда завершена */
            if (data[i] == '\r')
            {
                rx_buffer[rx_buffer_pos] = '\0';  // Завершаем строку
                DebugUART_Print("[TCP] Received command: %s", rx_buffer);

                /* TODO: Здесь нужно отправить команду в очередь eth_to_core_queue */
                /* Пока просто эхо */
                tcp_write(tpcb, rx_buffer, rx_buffer_pos, TCP_WRITE_FLAG_COPY);
                tcp_output(tpcb);

                rx_buffer_pos = 0;  // Сбрасываем буфер для следующей команды
            }
        }
        else
        {
            DebugUART_Print("[TCP] RX buffer overflow!\r\n");
            rx_buffer_pos = 0;  // Сбрасываем буфер
            break;
        }
    }

    pbuf_free(p);
    return ERR_OK;
}

static void tcp_server_error(void *arg, err_t err)
{
    DebugUART_Print("[TCP] Error %d\r\n", err);
    client_pcb = NULL;
    rx_buffer_pos = 0;
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    if (client_pcb)
    {
        DebugUART_Print("[TCP] Reject second client (only one allowed)\r\n");
        tcp_close(newpcb);
        return ERR_ABRT;
    }

    client_pcb = newpcb;
    rx_buffer_pos = 0;  // Сбрасываем буфер при новом подключении

    DebugUART_Print("[TCP] Client connected from %d.%d.%d.%d:%d\r\n",
                   ip4_addr1(&newpcb->remote_ip),
                   ip4_addr2(&newpcb->remote_ip),
                   ip4_addr3(&newpcb->remote_ip),
                   ip4_addr4(&newpcb->remote_ip),
                   newpcb->remote_port);

    tcp_arg(newpcb, NULL);
    tcp_recv(newpcb, tcp_server_recv);
    tcp_err(newpcb, tcp_server_error);
    tcp_poll(newpcb, NULL, 4);

    /* Отправляем приветственное сообщение */
    const char *welcome = "CAN-Ethernet Gateway ready. Use SLCAN commands.\r\n";
    tcp_write(newpcb, welcome, strlen(welcome), TCP_WRITE_FLAG_COPY);
    tcp_output(newpcb);

    return ERR_OK;
}

/* ===== INIT ===== */

void RawTcpServer_Init(void)
{
    if (server_pcb)
    {
        tcp_close(server_pcb);
        server_pcb = NULL;
    }

    server_pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!server_pcb)
    {
        DebugUART_Print("[TCP] tcp_new failed\r\n");
        return;
    }

    err_t err = tcp_bind(server_pcb, IP_ANY_TYPE, TCP_SERVER_PORT);
    if (err != ERR_OK)
    {
        DebugUART_Print("[TCP] bind failed: %d\r\n", err);
        tcp_close(server_pcb);
        server_pcb = NULL;
        return;
    }

    server_pcb = tcp_listen(server_pcb);
    if (!server_pcb)
    {
        DebugUART_Print("[TCP] listen failed\r\n");
        return;
    }

    tcp_arg(server_pcb, NULL);
    tcp_accept(server_pcb, tcp_server_accept);

    DebugUART_Print("[TCP] Listening on port %d\r\n", TCP_SERVER_PORT);
}
