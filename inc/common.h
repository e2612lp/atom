#ifndef _COMMON_H
#define _COMMON_H

#define  RETURN_IF_NIL(p1,E) \
    {if( !p1 ) return E;}

#define  RETURN_IF_NIL2(p1,p2,E) \
    {if( !p1 ||!p2 ) return E;}

#define  RETURN_IF_NIL3(p1,p2,p3,E) \
    {if( !p1 ||!p2 || !p3 ) return E;}

#define RETURN_ON_COND(COND,ret) \
    {if (COND)return ret;}
    

typedef char bool;
#define TRUE  (1)
#define FALSE (0)

#define mutex_lock  (1)
#define mutex_unlock  (0)


typedef volatile char mutex_t;


 


#endif