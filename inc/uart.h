#ifndef _UART_H
#define _UART_H


#define BUFF_LEN (2048)



//extern work_thread
//netlist_push
//netlist_pop
//
//
//


extern mutex_t guart_rbuf_mutex;
extern char guart_recvbuff[BUFF_LEN];


DWORD WINAPI Thread_uartproc(LPVOID p);

#endif