#ifndef _NET_H
#define _NET_H

#include "list.h"

///////////
typedef list_t netlist_t;
typedef list_node_t netnode_t;

typedef struct {
    char* context;
    unsigned int context_len;
    unsigned char trysend_cnt;
}netdata_t;

extern netlist_t * pnetlist_send;
extern mutex_t gnet_sendlist_mutex;
DWORD WINAPI Thread_netproc(LPVOID p);


#endif