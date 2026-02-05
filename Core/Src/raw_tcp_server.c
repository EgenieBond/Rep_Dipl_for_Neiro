/*
 * raw_tcp_server.c
 *
 * TCP RAW server (single client)
 */

#include "lwip/tcp.h"
#include "debug_uart.h"
#include <string.h>

#define TCP_SERVER_PORT 2001

static struct tcp_pcb *server_pcb = NULL;
static struct tcp_pcb *client_pcb = NULL;

/* ===== CALLBACKS ===== */

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
    if (!p)
    {
        DebugUART_Print("[TCP] Client disconnected\r\n");
        tcp_close(tpcb);
        client_pcb = NULL;
        return ERR_OK;
    }

    tcp_recved(tpcb, p->tot_len);

    DebugUART_Print("[TCP] RX %d bytes\r\n", p->tot_len);

    /* echo back (для отладки) */
    tcp_write(tpcb, p->payload, p->len, TCP_WRITE_FLAG_COPY);

    pbuf_free(p);
    return ERR_OK;
}

static void tcp_server_error(void *arg, err_t err)
{
    DebugUART_Print("[TCP] Error %d\r\n", err);
    client_pcb = NULL;
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    if (client_pcb)
    {
        DebugUART_Print("[TCP] Reject second client\r\n");
        tcp_close(newpcb);
        return ERR_ABRT;
    }

    client_pcb = newpcb;

    DebugUART_Print("[TCP] Client connected\r\n");

    tcp_recv(newpcb, tcp_server_recv);
    tcp_err(newpcb, tcp_server_error);

    return ERR_OK;
}

/* ===== INIT ===== */

void RawTcpServer_Init(void)
{
    if (server_pcb)
        return;  // уже создан

    server_pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!server_pcb)
    {
        DebugUART_Print("[TCP] tcp_new failed\r\n");
        return;
    }

    if (tcp_bind(server_pcb, IP_ANY_TYPE, TCP_SERVER_PORT) != ERR_OK)
    {
        DebugUART_Print("[TCP] bind failed\r\n");
        tcp_close(server_pcb);
        server_pcb = NULL;
        return;
    }

    server_pcb = tcp_listen(server_pcb);
    tcp_accept(server_pcb, tcp_server_accept);

    DebugUART_Print("[TCP] Listening on port %d\r\n", TCP_SERVER_PORT);
}

