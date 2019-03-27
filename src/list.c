#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"
#include "list.h"
#include "error.h"
#include "stdlib.h"
#include "common.h"


//
//  return new list_t *   head point
list_t *list_creat(void)
{
    list_t *plist = (list_t *)malloc(sizeof(list_t));
	RETURN_IF_NIL(plist, NULL);

	plist->next = plist->tail = NULL;
	plist->node_cnt = 0;
    
	return  plist;
}

// creat list node
static list_node_t *node_create(void *data, unsigned int id)
{
	list_node_t *node = NULL;
	RETURN_IF_NIL(data, NULL);
	
    node = (list_node_t *)malloc(sizeof(list_node_t));
	RETURN_IF_NIL(node, NULL);
	
    node->data = data;
    node->id = id;  //in list,have same id nodes??
	node->next = NULL;
	return node;
}

//before use this, should free the data 
ERR_T  node_destroy(list_node_t *node)
{
    RETURN_IF_NIL(node, ERR_LIST_PARAM_INVALID);
    free(node);
    return ERR_OK;
}
//list empty judge
bool list_isempty(list_t *plist)
{
    return (plist->node_cnt == 0)?(TRUE):(FALSE);
}



//insert a node after the list head
ERR_T list_insert_head(list_t *plist, void *data, unsigned int id)
{
    list_node_t *node = NULL;
    RETURN_IF_NIL2(plist, data, ERR_LIST_PARAM_INVALID);
      
    node = node_create(data, id);
    RETURN_IF_NIL(node, ERR_LIST_NOMEM);
    /* list is empty */
	if (list_isempty(plist) == TRUE) {
		plist->next= node ;
		plist->tail = node ;
	}
    else {  // not empty
		node->next = plist->next;
		plist->next = node;
	}
    
    plist->node_cnt++;
    return ERR_OK;
}


ERR_T list_insert_tail(list_t *plist, void *data, unsigned int id)
{
    list_node_t *node = NULL;
    RETURN_IF_NIL2(plist, data, ERR_LIST_PARAM_INVALID);
      
    node = node_create(data, id);
    RETURN_IF_NIL(node, ERR_LIST_NOMEM);
    
    if (list_isempty(plist) == TRUE) {
		plist->next= node ;
		plist->tail = node ;
	}
    else {  // not empty
		plist->tail->next = node;
        plist->tail = node;
	}
    
    plist->node_cnt++;
    return ERR_OK;
}


//
//
list_node_t *list_getnode_head(list_t *plist)
{
    return plist->next;
}

list_node_t *list_getnode_tail(list_t *plist)
{
    return plist->tail;
}

//
list_node_t *list_rmnode_head(list_t *plist)
{
    list_node_t *node = NULL;
    RETURN_IF_NIL(plist, NULL);
    
    if (list_isempty(plist) != TRUE) {
        node = plist->next;
        plist->next = plist->next->next;
        
        if(plist->node_cnt == 1) {
            plist->tail = plist->next;   // adjust tail NULL
        }
        
        plist->node_cnt--;
	}
    else {
        //do nothing, keep node NULL
    }
    
    return node;
}

//
list_node_t *list_rmnode_tail(list_t *plist)
{
    list_node_t *node,*ptmp = NULL;
    RETURN_IF_NIL(plist, NULL);
        
    if (list_isempty(plist) != TRUE) {

        if(plist->node_cnt == 1){
            node = plist->tail;
            plist->tail = plist->next = NULL;
        }
        else{
            //find the pre_tail node
            for(ptmp = plist->next; ptmp->next != plist->tail; ptmp =ptmp->next);
            
            plist->tail= ptmp; //adjust tail 
            node = ptmp->next;
        }
        
        plist->node_cnt--;
	}
    else {
        //do nothing, keep node NULL
    }
    
    return node;
}


void list_travel(list_t *plist)
{
    RETURN_IF_NIL(plist, NULL);
    
    sys_Log(LOG_INFO, "%s start", __func__);
    
    unsigned int i; list_node_t *node;
  
    for(i = 0, node = plist->next; i < plist->node_cnt; i++, node = node->next)
    {
        sys_Log(LOG_INFO, "list %d node, data:%s", node->id, node->data);
    }
}

void list_test(void)
{
    sys_Log(LOG_INFO, "%s start  head insert", __func__);
    
    char *test_data_1 = "123123";
    char *test_data_2 = "234234";
    char *test_data_3 = "345345";
    char *test_data_4 = "hello";
    list_t *test_list = NULL;
    
    test_list = list_creat();
    list_insert_head(test_list, NULL, 1);
    list_insert_head(test_list, test_data_2, 2);
    list_insert_head(test_list, test_data_3, 3);
    
    list_travel(test_list);
    
    list_rmnode_head(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);
    
    list_rmnode_head(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);
    
    list_rmnode_head(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);
    
    list_rmnode_head(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);
    
    sys_Log(LOG_INFO, "tail insert node start");
    
    list_insert_tail(test_list, test_data_1, 1);
    list_insert_tail(test_list, test_data_2, 2);
    list_insert_tail(test_list, test_data_3, 3);
    
    
    list_travel(test_list);
    
    list_rmnode_tail(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);
    
    list_rmnode_tail(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);
    
    list_rmnode_tail(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);
    
    list_rmnode_tail(test_list);
    sys_Log(LOG_INFO, "rm 1 node");
    list_travel(test_list);    
}