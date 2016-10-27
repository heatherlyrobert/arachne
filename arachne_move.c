/*============================----beg-of-source---============================*/

#include  "arachne.h"


tMOVE      *m_head;
tMOVE      *m_tail;
int         m_count;



/*====================------------------------------------====================*/
/*===----                       memory allocation                      ----===*/
/*====================------------------------------------====================*/
static void      o___MALLOC__________________o (void) {;}

tMOVE*       /*--> create a new, blank move --------------[ leaf   [ ------ ]-*/
MOVE__new          (void)
{  /*---(design notes)--------------------------------------------------------*/
   /*
    *  creates a new move object which is completely blank except for being
    *  linked into the master move doubly-linked list wich allows a single
    *  point of control over all moves, regardless of type or assignment
    */
   /*---(locals)-----------+-----------+-*/
   tMOVE      *x_new       = NULL;
   int         x_tries     = 0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(create)-------------------------*/
   while (x_new == NULL && x_tries < 10) {
      x_new = (tMOVE *) malloc (sizeof (tMOVE));
      ++x_tries;
   }
   DEBUG_DATA   yLOG_value   ("tries"     , x_tries);
   DEBUG_DATA   yLOG_value   ("x_new"     , x_new);
   if (x_new == NULL) {
      DEBUG_DATA   yLOG_note    ("could not malloc a new move object");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   /*---(overall)------------------------*/
   DEBUG_DATA   yLOG_note    ("default overall values");
   x_new->servo   = NULL;
   x_new->type    = MOVE_NULL;
   /*---(position)-----------------------*/
   DEBUG_DATA   yLOG_note    ("default positions");
   x_new->deg_beg =  0.0f;
   x_new->deg_end =  0.0f;
   /*---(timing)-------------------------*/
   DEBUG_DATA   yLOG_note    ("default timings");
   x_new->sec_dur =  0.0f;
   x_new->sec_beg =  0.0f;
   x_new->sec_end =  0.0f;
   /*---(location)-----------------------*/
   DEBUG_DATA   yLOG_note    ("default locations");
   x_new->x_pos   =  0.0f;
   x_new->y_pos   =  0.0f;
   x_new->z_pos   =  0.0f;
   /*---(master linked list)-------------*/
   DEBUG_DATA   yLOG_note    ("tie into master doubly-linked move list");
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
   /*---(servo linked list)--------------*/
   DEBUG_DATA   yLOG_note    ("default servo linked list");
   x_new->s_prev  = NULL;
   x_new->s_next  = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return x_new;
}

tMOVE*       /*--> destroy a single move -----------------[ leaf   [ ------ ]-*/
MOVE__free         (
      /*----------+-----------+-----------------------------------------------*/
      tMOVE      *a_move)
{  /*---(design notes)--------------------------------------------------------*/
   /*
    *  clears and destroys a single move entry as well as removing it from the
    *  master move doubly-linked list.
    */
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_point   ("a_move"    , a_move);
   /*---(defenses)-----------------------*/
   if (a_move      == NULL) {
      DEBUG_DATA   yLOG_note    ("can not free a null move");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   /*---(remove from dependency list)----*/
   DEBUG_DATA   yLOG_note    ("remove from backwards/prev direction");
   if (a_move->m_next != NULL) a_move->m_next->m_prev = a_move->m_prev;
   else                        m_tail                 = a_move->m_prev;
   DEBUG_DATA   yLOG_note    ("remove from forewards/next direction");
   if (a_move->m_prev != NULL) a_move->m_prev->m_next = a_move->m_next;
   else                        m_head                 = a_move->m_next;
   DEBUG_DATA   yLOG_note    ("decrement count");
   --m_count;
   /*---(free)---------------------------*/
   DEBUG_DATA   yLOG_note    ("free move object");
   free (a_move);
   a_move = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return  a_move;
}



/*====================------------------------------------====================*/
/*===----                        move objects                          ----===*/
/*====================------------------------------------====================*/
static void      o___OBJECTS_________________o (void) {;}

char         /*--> create a new move ---------------------[ ------ [ ------ ]-*/
MOVE_create        (
      /*----------+-----------+-----------------------------------------------*/
      char        a_type      ,   /* type of move (pause, servo, ...)         */
      tSERVO     *a_servo     ,   /* servo                                    */
      float       a_deg       ,   /* end position                             */
      float       a_sec       )   /* duration                                 */
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tMOVE      *x_move      = NULL;          /* new requires entry             */
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("a_type"    , a_type);
   DEBUG_DATA   yLOG_value   ("a_deg"     , a_deg);
   DEBUG_DATA   yLOG_value   ("a_sec"     , a_sec);
   /*---(defenses)-----------------------*/
   DEBUG_DATA   yLOG_point   ("a_servo"   , a_servo);
   --rce;  if (a_servo     == NULL) {
      DEBUG_DATA   yLOG_note    ("can not add a move to a null servo");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_info    ("name"      , a_servo->name);
   /*---(create a new requires)-------*/
   DEBUG_DATA   yLOG_note    ("add a new move object");
   x_move          = MOVE__new ();
   --rce;  if (x_move == NULL)   {
      DEBUG_DATA yLOG_exit      (__FUNCTION__);
      return rce; 
   }
   /*---(assign basics)---------------*/
   DEBUG_DATA   yLOG_note    ("assign basic values");
   x_move->type     = a_type;
   x_move->servo    = a_servo;
   x_move->deg_end  = a_deg;
   x_move->sec_dur  = a_sec;
   /*---(hook it up to servo)---------*/
   DEBUG_DATA   yLOG_note    ("hook up move into servo doubly-linked list");
   if (a_servo->tail == NULL) {
      DEBUG_DATA   yLOG_note    ("first entry in list");
      a_servo->head         = x_move;
      a_servo->tail         = x_move;
      a_servo->count        = 1;
      DEBUG_DATA   yLOG_note    ("update sec/deg based on being first move");
      x_move->sec_beg       = 0.0f;
      x_move->sec_end       = a_sec;
      x_move->deg_beg       = 0.0f;
   } else {
      DEBUG_DATA   yLOG_note    ("add to tail");
      x_move->s_prev        = a_servo->tail;
      a_servo->tail->s_next = x_move;
      a_servo->tail         = x_move;
      ++(a_servo->count);
      DEBUG_DATA   yLOG_note    ("update sec/deg based on previous move");
      x_move->sec_beg       = x_move->s_prev->sec_end;
      x_move->sec_end       = x_move->sec_beg + a_sec;
      x_move->deg_beg       = x_move->s_prev->deg_end;
   }
   /*---(display stats)------------------*/
   DEBUG_DATA   yLOG_value   ("count"     , a_servo->count);
   DEBUG_DATA   yLOG_value   ("sec_beg"   , x_move->sec_beg);
   DEBUG_DATA   yLOG_value   ("sec_end"   , x_move->sec_end);
   DEBUG_DATA   yLOG_value   ("deg_beg"   , x_move->deg_beg);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> add a location to a move object -------[ ------ [ ------ ]-*/
MOVE_addloc        (
      /*----------+-----------+-----------------------------------------------*/
      tSERVO     *a_servo     ,   /* servo                                    */
      float       a_xpos      ,   /* end location                             */
      float       a_ypos      ,   /* end location                             */
      float       a_zpos      )   /* end location                             */
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_value   ("a_xpos"    , a_xpos);
   DEBUG_DATA   yLOG_value   ("a_ypos"    , a_ypos);
   DEBUG_DATA   yLOG_value   ("a_zpos"    , a_zpos);
   /*---(defenses)-----------------------*/
   DEBUG_DATA   yLOG_point   ("a_servo"   , a_servo);
   --rce;  if (a_servo     == NULL) {
      DEBUG_DATA   yLOG_note    ("can not add location to a null servo");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_info    ("name"      , a_servo->name);
   /*---(assign basics)---------------*/
   DEBUG_DATA   yLOG_note    ("assign location values");
   a_servo->tail->x_pos    = a_xpos;
   a_servo->tail->y_pos    = a_ypos;
   a_servo->tail->z_pos    = a_zpos;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================----end-of-source---============================*/
