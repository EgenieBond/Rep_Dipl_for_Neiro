/*
 * raw_tcp_server.c
 *
 *  Created on: Jan 27, 2026
 *      Author: Egenie
 */


#include "raw_tcp_server.h"
#include "lwip/tcp.h"
#include <string.h>

#define TCP_SERVER_PORT 5000

static struct tcp_pcb *server_pcb;

/* ===== CALLBACKS ===== */

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        return ERR_OK;
    }

    /* Сообщаем lwIP, что данные приняты */
    tcp_recved(tpcb, p->tot_len);

    /* Echo обратно */
    tcp_write(tpcb, p->payload, p->len, TCP_WRITE_FLAG_COPY);

    pbuf_free(p);
    return ERR_OK;
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    tcp_recv(newpcb, tcp_server_recv);
    tcp_nagle_disable(newpcb);

    return ERR_OK;
}

/* ===== INIT ===== */

void RawTcpServer_Init(void)
{
    server_pcb = tcp_new();
    if (!server_pcb)
        return;

    if (tcp_bind(server_pcb, IP_ADDR_ANY, TCP_SERVER_PORT) != ERR_OK)
        return;

    server_pcb = tcp_listen(server_pcb);
    tcp_accept(server_pcb, tcp_server_accept);
}
