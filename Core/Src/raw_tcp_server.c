/*
 * raw_tcp_server.c
 *  Created on: Jan 27, 2026	Author: Egenie
 */

#include "raw_tcp_server.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "debug_uart.h"

#define TCP_SERVER_PORT 2001

static struct tcp_pcb *server_pcb;

/* ===== Receive callback ===== */
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
{
  if (p == NULL)
  {
    DebugUART_Print("[TCP] Client disconnected\r\n");
    tcp_close(tpcb);
    return ERR_OK;
  }

  tcp_recved(tpcb, p->tot_len);

  /* Echo received data */
  tcp_write(tpcb, p->payload, p->len, TCP_WRITE_FLAG_COPY);

  pbuf_free(p);
  return ERR_OK;
}

/* ===== Accept callback ===== */
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  DebugUART_Print("[TCP] Client connected\r\n");

  tcp_recv(newpcb, tcp_server_recv);
  tcp_nagle_disable(newpcb);

  return ERR_OK;
}

/* ===== Init inside tcpip_thread ===== */
static void tcp_server_init_cb(void *arg)
{
  LWIP_UNUSED_ARG(arg);

  server_pcb = tcp_new();
  if (server_pcb == NULL)
  {
    DebugUART_Print("[TCP] tcp_new failed\r\n");
    return;
  }

  tcp_bind(server_pcb, IP_ADDR_ANY, TCP_SERVER_PORT);
  server_pcb = tcp_listen(server_pcb);
  tcp_accept(server_pcb, tcp_server_accept);

  DebugUART_Print("[TCP] Server listening on port 2001\r\n");
}

/* ===== Public init ===== */
void RawTcpServer_Init(void)
{
  tcpip_callback(tcp_server_init_cb, NULL);
}
