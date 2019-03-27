/*
//net.c 
//
//
*/

#include <stdio.h>
#include "log.h"
#include "list.h"
#include "error.h"
#include "common.h"
#include <windows.h>
#include "net.h"

//micro 
#ifndef MODULE_NAME
#define MODULE_NAME ("net")
#endif

//local val
//class list_node_t;
netlist_t * pnetlist_send;
mutex_t gnet_sendlist_mutex = mutex_unlock;

void netsend_proc(void)
{      
    //while net is OK && loc_staging list is not empty\ netsend is not empty
       /* get one item from local or netsend list every loop
        *     send one item (stage one, ret random)
        *          send OK? 
        *              remove it from the list
        *              continue
        *          send fail?
        *              trynum out?
        *                  resp handle
        */            
    
    
}

void netrecv_proc(void)
{
    
}

//ERR_T list_insert_tail(list_t *plist, void *data, unsigned int id);
ERR_T netlist_addnode(netlist_t *plist, netdata_t *data)
{
    unsigned int msg_id = rand();
    //sys_Log(LOG_INFO, "%s:%s dbg %d in", MODULE_NAME, __func__, msg_id);
    return list_insert_tail(plist, (void *)data, msg_id);
}

//list_node_t *list_getnode_head(list_t *plist);
//list_node_t *list_rmnode_head(list_t *plist);

netnode_t * netlist_getnode(netlist_t *plist)
{
    //netnode_t *pnode = NULL;
    return (netnode_t *)list_rmnode_head(plist);
}

void netnode_free(netnode_t *pnode)
{
    free( ((netdata_t *)(pnode->data))->context );
    ((netdata_t *)(pnode->data))->context = NULL;
    
    free(pnode->data);
    pnode->data = NULL;
    
    free( pnode );
    pnode = NULL;
}


DWORD WINAPI Thread_netproc(LPVOID p)
{
    sys_Log(LOG_INFO, "%s:%s start in", MODULE_NAME, __func__);
    
    pnetlist_send = (netlist_t *)list_creat();
    
    while(1)
    {
        //netlist_send &local staging handle
        sys_trygetlock(gnet_sendlist_mutex);
        while(list_isempty(pnetlist_send) != TRUE)
        {    
            netnode_t *pnode = netlist_getnode(pnetlist_send);
            sys_Log(LOG_INFO, "%s:%s dbg %s in", MODULE_NAME, __func__, ((netdata_t *)(pnode->data))->context);
            netnode_free(pnode);
        }
        sys_releaselock(gnet_sendlist_mutex);
        
        //netrecv_buff  handle
        //sleep(1);
    }
    return 0;
}