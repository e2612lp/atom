
/*
//main.c 
//
//
*/
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "list.h"
#include "error.h"
#include "common.h"
#include <windows.h>
#include "uart.h"
#include "net.h"
//using namespace std;

//micro 
#ifndef MODULE_NAME
#define MODULE_NAME ("sys")
#endif

#define TEST_STR1  ("*1234#12312345991188990045##")
#define TEST_STR2  ("##&&123*2349901*123")
#define TEST_STR3  ("*7575#56789123890880##")
#define TEST_STR4  ("*5678#!!littlejhfi9090123AB##")

typedef struct {
    char head_pad[4];
    char *data;  // should be string
    char tail_pad[4];
}test_stritem_st;

//local val
int   count   =0; 
HANDLE hThread_A, hThread_B, hThread_C, hThread_D;
DWORD  threadId_A,threadId_B, threadId_C, threadId_D;

//local function
void mutex_init(void);
void sys_trygetlock(mutex_t pmutex);
void sys_releaselock(mutex_t pmutex);

// test part  start
char test_tmp[20] = {'*','*','\0','*','(','#','\0'};
test_stritem_st  test_stritem[] = {{.data = "123"}, {.data="456"}};
#define TEST_STRITEM_NUM (sizeof(test_stritem) / sizeof(test_stritem_st))
// test part  end

//timer test
void CALLBACK OnTimer(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
//void TimerProc(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
//void OnTimer()
{   
	count++;     
	printf("WM_TIMER   in   work   thread   count=%d\n",count); 
}


void fill_uartrecvbuff_test(void)
{
    memset(guart_recvbuff, 0, sizeof(guart_recvbuff));
    //recvbuff_simulate_test_in();
    snprintf(guart_recvbuff, sizeof(TEST_STR1), TEST_STR1); 
    //strcat(guart_recvbuff, TEST_STR2);
    snprintf((char*)guart_recvbuff + sizeof(TEST_STR1)+ 5, sizeof(TEST_STR3), TEST_STR3); 
    memcpy((char *)guart_recvbuff + 96, test_tmp, sizeof(test_tmp));
    snprintf((char*)guart_recvbuff + 164, sizeof(TEST_STR2), TEST_STR2);    
    snprintf((char*)guart_recvbuff + 256, sizeof(TEST_STR4), TEST_STR4);
}

DWORD WINAPI Thread_uarttest(LPVOID p)
{   
    //printf("I am child, pid = %d\n", GetCurrentThreadId());   //输出子线程pid
    sys_Log(LOG_INFO, "%s:%s start in", MODULE_NAME, __func__);
    int loop_cnt = 0;
    
    while(1)
    {
        //uart_input test
        sys_Log(LOG_INFO, "%s:%s loop", MODULE_NAME, __func__);
        if(loop_cnt % 3 == 0)
        {
            sys_trygetlock(guart_rbuf_mutex);
            fill_uartrecvbuff_test();       
            sys_releaselock(guart_rbuf_mutex);
        }
        //uart_output print
        
        //
        sleep(1);
        loop_cnt++;
    }
    
    return 0;
}





int main(int argc, char **argv)
{
	printf("hello world\n");
    
    g_sys_loglevel = LOG_INFO;
    //HWND tm_hand;
    
    sys_Log(LOG_INFO, "app start,M-name:%s\n", "main");
    //sys_Log(LOG_DEBUG, "app start111\n");
    //list_test();
    
    unsigned int  timerid = SetTimer(NULL, 10, 1000, OnTimer);
    
    sys_Log(LOG_INFO, "app start,M-name:%s, timeid:%d\n", "main", timerid);
    //file_test();
    
     hThread_A = CreateThread(NULL, 0, Thread_uarttest, 0, 0, &threadId_A); // 创建线程
     hThread_B = CreateThread(NULL, 0, Thread_uartproc, 0, 0, &threadId_B);
     hThread_C = CreateThread(NULL, 0, Thread_netproc, 0, 0, &threadId_C);
     
    //KillTimer(NULL,timerid);  
    while(1);
	return 0;
    
}

//file test
void file_test(void)
{
    //HANDLE hsrcfile = CreateFile("./test1", GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
    char *wr_buff = "file test 123123123";
    char *rd_buff[64] = {0};
    int wr_bytecnt = 0;
    HANDLE hdestfile = CreateFile("./test2", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    
    if(hdestfile == INVALID_HANDLE_VALUE)
    {
        sys_Log(LOG_INFO, "invalid  hdestfile");
    }
    
    //WriteFile(hdestfile, wr_buff, strlen(wr_buff), &wr_bytecnt, 0);
   
    //ReadFile(hdestfile, rd_buff, 10, &wr_bytecnt, 0);
    
    sys_Log(LOG_INFO, "rd_buff is %s", rd_buff);

}

void sys_trygetlock(mutex_t pmutex)
{
    //wait until pmutex = unlock
    while(pmutex == mutex_lock);
    
    pmutex = mutex_lock;
}

void sys_releaselock(mutex_t pmutex)
{
    pmutex = mutex_unlock;
}
