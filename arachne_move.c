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
   x_new->seq     = 0;
   strlcpy (x_new->label, "", LEN_LABEL);
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
      char       *a_label     ,   /* step label                               */
      int         a_line      ,   /* source line                              */
      float       a_deg       ,   /* end position                             */
      float       a_sec       )   /* duration                                 */
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   tMOVE      *x_move      = NULL;          /* new requires entry             */
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("a_type"    , a_type);
   DEBUG_DATA   yLOG_double  ("a_deg"     , a_deg);
   DEBUG_DATA   yLOG_double  ("a_sec"     , a_sec);
   /*---(defenses)-----------------------*/
   DEBUG_DATA   yLOG_point   ("a_servo"   , a_servo);
   --rce;  if (a_servo     == NULL) {
      DEBUG_DATA   yLOG_note    ("can not add a move to a null servo");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_info    ("name"      , a_servo->label);
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
      x_move->seq           = 1;
      x_move->sec_beg       = 0.0f;
      x_move->sec_end       = a_sec;
      x_move->deg_beg       = a_deg;
   } else {
      DEBUG_DATA   yLOG_note    ("add to tail");
      x_move->s_prev        = a_servo->tail;
      a_servo->tail->s_next = x_move;
      a_servo->tail         = x_move;
      ++(a_servo->count);
      DEBUG_DATA   yLOG_note    ("update sec/deg based on previous move");
      x_move->seq           = x_move->s_prev->seq + 1;
      x_move->sec_beg       = x_move->s_prev->sec_end;
      x_move->sec_end       = x_move->sec_beg + a_sec;
      x_move->deg_beg       = x_move->s_prev->deg_end;
   }
   /*---(display stats)------------------*/
   DEBUG_DATA   yLOG_value   ("count"     , a_servo->count);
   DEBUG_DATA   yLOG_value   ("seq"       , x_move->seq);
   DEBUG_DATA   yLOG_value   ("sec_beg"   , x_move->sec_beg);
   DEBUG_DATA   yLOG_value   ("sec_end"   , x_move->sec_end);
   DEBUG_DATA   yLOG_value   ("deg_beg"   , x_move->deg_beg);
   /*---(update globals)-----------------*/
   DEBUG_DATA   yLOG_double  ("my.p_len"  , my.p_len);
   if (x_move->sec_end > my.p_len) {
      my.p_len = x_move->sec_end;
      DEBUG_DATA   yLOG_note    ("end time greater than current length");
      DEBUG_DATA   yLOG_double  ("my.p_len"  , my.p_len);
   }
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
   DEBUG_DATA   yLOG_info    ("name"      , a_servo->label);
   /*---(assign basics)---------------*/
   DEBUG_DATA   yLOG_note    ("assign location values");
   a_servo->tail->x_pos    = a_xpos;
   a_servo->tail->y_pos    = a_ypos;
   a_servo->tail->z_pos    = a_zpos;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> copy an existing move to end ----------[ ------ [ ------ ]-*/
MOVE_copyappend    (
      /*----------+-----------+-----------------------------------------------*/
      tSERVO     *a_servo     ,   /* servo                                    */
      char       *a_label     ,   /* step label                               */
      int         a_count     ,   /* how many steps to copy (off end)         */
      int         a_times     )   /* how many times to copy                   */
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        x_label     [LEN_LABEL];
   tMOVE      *x_curr      = NULL;
   tMOVE      *x_moves     [100];
   int         i           = 0;
   int         j           = 0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_point   ("a_servo"   , a_servo);
   DEBUG_DATA   yLOG_info    ("a_label"   , a_label);
   DEBUG_DATA   yLOG_value   ("a_count"   , a_count);
   DEBUG_DATA   yLOG_value   ("a_times"   , a_times);
   /*---(defense)------------------------*/
   --rce;  if (a_servo        == NULL) {
      DEBUG_DATA   yLOG_note    ("not a valid servo");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_info    ("servo"     , a_servo->label);
   --rce;  if (a_servo->tail  == NULL) {
      DEBUG_DATA   yLOG_note    ("no moves on servo, nothing to repeat");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_DATA   yLOG_value   ("count"     , a_servo->count);
   --rce;  if (a_servo->count <  a_count) {
      DEBUG_DATA   yLOG_note    ("not enough moves on servo to repeat count");
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(build queue)--------------------*/
   for (i = 0; i < 100; ++i) {
      x_moves [i] = NULL;
   }
   x_curr = a_servo->tail;
   for (i = 1; i < a_count; ++i) {
      x_curr = x_curr->s_prev;
   }
   for (i = 0; i < a_count; ++i) {
      x_moves [i] = x_curr;
      x_curr      = x_curr->s_next;
   }
   /*---(add)----------------------------*/
   for (i = 0; i < a_times; ++i) {
      for (j = 0; j < a_count; ++j) {
         sprintf (x_label, "repeat %d.%d", i,j);
         MOVE_create (MOVE_SERVO, a_servo, x_label, x_moves [j]->line, x_moves [j]->deg_end, x_moves [j]->sec_dur);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      script position                         ----===*/
/*====================------------------------------------====================*/
static void      o___POSITION________________o (void) {;}

char         /*--> set the current move for a servo ------[ ------ [ ------ ]-*/
MOVE_curset        (int a_servo, float a_time)
{
   /*---(locals)-----------+-----------+-*/
   tMOVE      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   x_next = g_servos [a_servo].head;
   if (x_next == NULL) {
      DEBUG_DATA   yLOG_snote   ("no moves for servo");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return -10;
   }
   /*---(walk thru moves)----------------*/
   while (x_next != NULL) {
      DEBUG_DATA   yLOG_sint    (x_next->seq);
      if (x_next->sec_beg >  a_time) {
         DEBUG_DATA   yLOG_snote   ("too far");
         DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
         return -11;
      }
      if (x_next->sec_end <  a_time) {
         x_next = x_next->s_next;
         continue;
      }
      DEBUG_DATA   yLOG_snote   ("found it");
      g_servos [a_servo].curr = x_next;
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_snote   ("failed");
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return -12;
}

char         /*--> calc the current deg for a servo ------[ ------ [ ------ ]-*/
MOVE_curone        (int a_servo, float a_time)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =   0;           /* generic return code            */
   tMOVE      *x_curr      = NULL;
   float       x_beg       = 0.0;
   float       x_end       = 0.0;
   float       x_pct       = 0.0;
   float       x_degs      = 0.0;
   float       x_pos       = 0.0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_info    ("label"     , g_servos [a_servo].label);
   /*---(check for correct current)------*/
   if        (g_servos [a_servo].curr == NULL) {
      DEBUG_DATA   yLOG_note    ("servo current is not set");
      rc = MOVE_curset (a_servo, a_time);
      DEBUG_DATA   yLOG_value   ("rc"        , rc);
   }
   /*---(check return code)--------------*/
   if      (rc < 0) {
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(check for right times)----------*/
   x_curr  = g_servos [a_servo].curr;
   if        (a_time < x_curr->sec_beg) {
      DEBUG_DATA   yLOG_note    ("time is before current move beg");
      rc = MOVE_curset (a_servo, a_time);
      DEBUG_DATA   yLOG_value   ("rc"        , rc);
   } else if (a_time > x_curr->sec_end) {
      DEBUG_DATA   yLOG_note    ("time is after current move end");
      rc = MOVE_curset (a_servo, a_time);
      DEBUG_DATA   yLOG_value   ("rc"        , rc);
   }
   /*---(check return code)--------------*/
   if      (rc < 0) {
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(calc position)------------------*/
   x_curr  = g_servos [a_servo].curr;
   DEBUG_DATA   yLOG_double  ("sec_beg"   , x_curr->sec_beg);
   DEBUG_DATA   yLOG_double  ("sec_end"   , x_curr->sec_end);
   x_beg   = x_curr->sec_beg;
   x_end   = x_curr->sec_end;
   DEBUG_DATA   yLOG_double  ("deg_beg"   , x_curr->deg_beg);
   DEBUG_DATA   yLOG_double  ("deg_end"   , x_curr->deg_end);
   x_pct   = (a_time - x_beg) / (x_end - x_beg);
   if (x_end == 0.0)  x_pct = 0.0;
   DEBUG_DATA   yLOG_double  ("x_pct"     , x_pct);
   x_degs  = x_curr->deg_end - x_curr->deg_beg;
   DEBUG_DATA   yLOG_double  ("x_degs"    , x_degs);
   x_pos   = x_curr->deg_beg + (x_degs * x_pct);
   DEBUG_DATA   yLOG_double  ("x_pos"     , x_pos);
   g_servos [a_servo].deg = x_pos;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> calc current move/deg for all servos --[ ------ [ ------ ]-*/
MOVE_curall        (float a_time)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =   0;
   int         i           =   0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   for (i = 0; i < MAX_SERVO; ++i) {
      if (strcmp (g_servos [i].label, "end-of-list") == 0)  break;
      rc = MOVE_curone  (i, a_time);
   }
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

static tMOVE   *s_curr = NULL;

char         /*--> retrieve the first move ---------------[ ------ [ ------ ]-*/
MOVE_first         (int a_servo, float *a_sec, float *a_deg)
{
   /*---(locals)-----------+-----------+-*/
   tMOVE      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(get current)--------------------*/
   x_next = g_servos [a_servo].head;
   if (x_next == NULL) {
      s_curr  = NULL;
      *a_sec  = 0.0;
      *a_deg  = 0.0;
      DEBUG_DATA   yLOG_snote   ("no moves for servo");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return -10;
   }
   /*---(return values)------------------*/
   s_curr  = x_next;
   *a_sec  = s_curr->sec_end;
   *a_deg  = s_curr->deg_end;
   DEBUG_DATA   yLOG_snote   ("failed");
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*--> retrieve the next move ----------------[ ------ [ ------ ]-*/
MOVE_next          (float *a_sec, float *a_deg)
{
   /*---(locals)-----------+-----------+-*/
   tMOVE      *x_next      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   /*---(get current)--------------------*/
   x_next = s_curr;
   if (x_next == NULL) {
      s_curr  = NULL;
      *a_sec  = 0.0;
      *a_deg  = 0.0;
      DEBUG_DATA   yLOG_snote   ("no current move for servo");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return -10;
   }
   /*---(get next)-----------------------*/
   x_next = x_next->s_next;
   if (x_next == NULL) {
      s_curr  = NULL;
      *a_sec  = 0.0;
      *a_deg  = 0.0;
      DEBUG_DATA   yLOG_snote   ("no next move for servo");
      DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
      return -11;
   }
   /*---(return values)------------------*/
   s_curr  = x_next;
   *a_sec  = x_next->sec_end;
   *a_deg  = x_next->deg_end;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_snote   ("failed");
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}





/*============================----end-of-source---============================*/
