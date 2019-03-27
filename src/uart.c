
/*
//uart.c 
//
//
*/


#include <stdio.h>
#include <string.h>
#include "log.h"
#include "list.h"
#include "error.h"
#include "common.h"
#include <windows.h>
#include "uart.h"
#include "net.h"
#include "common.h"


//micro
#ifndef MODULE_NAME
#define MODULE_NAME ("uart")
#endif

#define VALID_MSGLEN  (3) 
#define COND_JUDGE(X,Y,Z)    (3*X+Y+Z) //+ (#Y)//*(#Y)//( (#X-5)*(#Y+3)*(#Z*2) ) 
#define FIND_MSGPOS(X,Y)   { for( ;(X[Y] != '*')&&(Y < BUFF_LEN - 10);Y ++); }


//target special: "*1234#1231234567890##"  begin:*  end:##
char guart_recvbuff[BUFF_LEN] = {0};
mutex_t guart_rbuf_mutex = mutex_unlock;


/*
thread  task:
 check uart_recvbuff and prase
 compose netlist node, push it to netlist  
  *  check net_recvlist and prase,send it to uart_sendbuff 
  *
*/

//find the valid msg in uart_recvbuff  valid format:*----#---------##, *:head # interval ##:end
static char *extract_validmsg(void)
{    
    static unsigned int search_pos = 0;
    char *ret = NULL, *ptmp = NULL;

find_msghead:    
    //for( ;(guart_recvbuff[search_pos] != '*')&&(search_pos < BUFF_LEN - 10);search_pos ++);  //10: protect area  margin
    FIND_MSGPOS(guart_recvbuff, search_pos);
    
    if(guart_recvbuff[search_pos] == '*')
    {
        ptmp = &guart_recvbuff[search_pos];
        //sys_Log(LOG_DEBUG, "%s:111:%s", MODULE_NAME, ptmp);
        
        if(strlen(ptmp) <= VALID_MSGLEN)
        {
            //// tail tag"##" pos  format  invald,re-search
            search_pos += strlen(ptmp);
            goto find_msghead; 
        }
        else{
             if(strcmp(ptmp+(strlen(ptmp) - 2), "##") != 0) //tail != ##
             {
                 search_pos += strlen(ptmp);
                 goto find_msghead; 
             }
             else{
                   //pass the format check do nothing 
             }
        }
           
        ret = ptmp;
        search_pos += strlen(ptmp);  //move search_pos for next search 
    }
    
    //get to the buff_tail
    if(search_pos >= BUFF_LEN - 10)
    {
        ret = NULL;        
        //clear the recv_buff
        sys_trygetlock(guart_rbuf_mutex);
        memset(guart_recvbuff, 0, sizeof(guart_recvbuff));        
        sys_releaselock(guart_rbuf_mutex); 

        search_pos = 0; //reset the pos
    }
    
    return ret;
}

//get data  from "*----#---------##"msg
static char *extract_data(char *phead)
{
    char *ptmp;
    RETURN_IF_NIL(phead, NULL);
    // chk the interval sympol
    RETURN_IF_NIL((ptmp = strchr(phead, '#')), NULL);
    //sys_Log(LOG_INFO, "%s:extract_data 1:%s", MODULE_NAME, ptmp);
    //check the case: "*----###"msg  (no valid data)
    RETURN_ON_COND((*(ptmp + 1) == '#'), NULL)
    //move ptmp point to data-area
    return ++ptmp;          
}

void prase_msg(char *pmsg)
{
    //no chk pmsg 
    //get msg type
    /*
     *    switch(msg_type)
     *       case  ctrl_type
     *          prase data 
     *       case  netdata_type
     *          compose netlist node, push
     *       default:
     *           msg_type err  log
     * 
     * */
}

void fill_netdata(const char *pdata, netdata_t **ppnet)
{
    int data_len = strlen(pdata) - 2;
    
    *ppnet = malloc(sizeof(netdata_t)); // in net handle thread will free it
    (*ppnet)->context = (char *)malloc(data_len + 1);
    
    if((*ppnet)->context  == NULL)
    {
       //err  handle
    }
                
    strncpy((*ppnet)->context, pdata, data_len);
    (*ppnet)->context[data_len] = '\0';
    (*ppnet)->trysend_cnt = 3;  // initial val
}

void uart_uplink_proc(void)
{
    char *ptmp = NULL;
    netdata_t *pdata;
    
    sys_trygetlock(guart_rbuf_mutex);
    while((ptmp = extract_validmsg()) != NULL)
    {
        //ptmp = convert_msg(phead);  
        //sys_Log(LOG_INFO, "%s:uart_recvbuf:%s", MODULE_NAME, phead);
        ptmp = extract_data(ptmp);
        if(NULL == ptmp){
            continue;  //data_invalid,sorry to drop it
        } 
        sys_Log(LOG_INFO, "%s:data:%s", MODULE_NAME, ptmp);
             
        fill_netdata(ptmp, &pdata);
        
        sys_trygetlock(gnet_sendlist_mutex);
        netlist_addnode(pnetlist_send, pdata);
        sys_releaselock(gnet_sendlist_mutex);
        //prase the buff,compose netlist node, push it
        //ret = prase_msg(phead);
                //ctrl_msg\netdata_msg

    }//end of while
    sys_releaselock(guart_rbuf_mutex);
    //recv_buff prase done,clr already
    
} 


DWORD WINAPI Thread_uartproc(LPVOID p)
{
    sys_Log(LOG_INFO, "%s:%s start in", MODULE_NAME, __func__);
    
    while(1)
    {
        //check guart_recvbuff,if not empty   
        //prase,compose netlist_node until get every msg from buff  uart_uplink_proc() 
        uart_uplink_proc();
        
        
        //get nodefrom net_recvlist
        
        sleep(1);
    }
    return 0;
}




