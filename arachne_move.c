/*============================----beg-of-source---============================*/

#include  "arachne.h"


tMOVE      *m_head;
tMOVE      *m_tail;
int         m_count;


/*====================------------------------------------====================*/
/*===----                       memory allocation                      ----===*/
/*====================------------------------------------====================*/
static void      o___MALLOC__________________o (void) {;}

   char        type;
   float       sec_dur;
   float       deg_beg;
   float       deg_end;
   float       sec_beg;
   float       sec_end;
   float       x_pos;
   float       y_pos;
   float       z_pos;
   tMOVE      *m_prev;
   tMOVE      *m_next;
   tMOVE      *s_prev;
   tMOVE      *s_next;

tMOVE*       /*--> create a new, blank move --------------[ leaf   [ ------ ]-*/
MOVE__new          (tSERVO  *a_parent)
{  /*---(design notes)--------------------------------------------------------*/
   /*
    *  creates a new move object which is completely blank except for being
    *  linked into the master move doubly-linked list wich allows a single
    *  point of control over all moves, regardless of type or assignment
    */
   /*---(locals)-----------+-----------+-*/
   tMOVE      *x_new       = NULL;
   int         x_tries     = 0;
   /*---(create)-------------------------*/
   while (x_new == NULL && x_tries < 10) {
      x_new = (tMOVE *) malloc (sizeof (tMOVE));
      ++x_tries;
   }
   if (x_new == NULL)    return NULL;
   /*---(overall)------------------------*/
   x_new->parent  = a_parent;
   x_new->type    = MOVE_NULL;
   /*---(position)-----------------------*/
   x_new->deg_beg =  0.0f;
   x_new->deg_end =  0.0f;
   /*---(timing)-------------------------*/
   x_new->sec_dur =  0.0f;
   x_new->sec_beg =  0.0f;
   x_new->sec_end =  0.0f;
   /*---(location)-----------------------*/
   x_new->x_pos   =  0.0f;
   x_new->y_pos   =  0.0f;
   x_new->z_pos   =  0.0f;
   /*---(master linked list)-------------*/
   x_new->m_prev  = NULL;
   x_new->m_next  = NULL;
   if (m_tail == NULL) {
      m_head         = x_new;
      m_tail         = x_new;
   } else {
      x_new->m_prev  = m_tail;
      m_tail->m_next = x_new;
      m_tail         = x_new;
   }
   ++m_count;
   /*---(move linked list)---------------*/
   x_new->s_prev  = NULL;
   x_new->s_next  = NULL;
   /*---(complete)-----------------------*/
   return x_new;
}



/*============================----end-of-source---============================*/
