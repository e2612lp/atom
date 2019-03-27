#ifndef _LIST_H
#define _LIST_H

#include "common.h"
#include "error.h"

typedef struct list_t list_t;
typedef struct list_node_t  list_node_t;

struct list_node_t 
{
    unsigned int id;
	void     *data;
	struct list_node_t *next;
};

struct list_t
{
    list_node_t *next;
    list_node_t *tail;
    unsigned int node_cnt;
    //mutex??
};

list_t *list_creat(void);
//ret: success
void list_destory(list_t *plist);

//
unsigned int list_get_nodecnt(list_t *plist); 
//
bool list_isempty(list_t *plist);

//insert node:  return: ERR_OK/ERR_LIST_INSERT_FAIL
ERR_T list_insert_head(list_t *plist, void *data, unsigned int id);
ERR_T list_insert_tail(list_t *plist, void *data, unsigned int id);

//only get node,not delete it  
list_node_t *list_getnode_head(list_t *plist);
list_node_t *list_getnode_tail(list_t *plist);
list_node_t *list_getnode_id(list_t *plist, unsigned int id);

//remove node  remove the node from the list
list_node_t *list_rmnode_head(list_t *plist);
list_node_t *list_rmnode_tail(list_t *plist);
list_node_t *list_rmnode_id(list_t *plist, unsigned int id);  

ERR_T  node_destroy(list_node_t *node);

void list_travel(list_t *plist);

void list_test(void);


#endif