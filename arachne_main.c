/*============================----beg-of-source---============================*/

#include "arachne.h"




int
main                    (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   /*---(debugging)----------------------*/
   rc = PROG_urgents (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("urgents"   , rc);
   if (rc <  0) { PROG_shutdown (); return -1; }
   /*---(initialization)-----------------*/
   rc = PROG_startup (a_argc, a_argv);
   DEBUG_PROG   yLOG_value    ("startup"   , rc);
   if (rc <  0) { PROG_shutdown (); return -2; }
   /*---(visual setup)-------------------*/
   rc = PROG_dawn    ();
   DEBUG_PROG   yLOG_value    ("dawn"      , rc);
   if (rc <  0) { PROG_shutdown (); return -3; }
   /*---(main-loop)----------------------*/
   rc = yVIOPENGL_main  ("10ms", "100ms", NULL);
   DEBUG_PROG   yLOG_value    ("main"      , rc);
   rc = PROG_dusk     ();
   DEBUG_PROG   yLOG_value    ("dusk"      , rc);
   /*---(wrap-up)------------------------*/
   rc = PROG_shutdown ();
   /*---(complete)-----------------------*/
   return 0;
}


int
main (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   /*---(initialize)---------------------*/
   /*> if (rc >= 0)  rc = yURG_logger  (argc, argv);                                  <*/
   /*> if (rc >= 0)  rc = PROG_init    ();                                            <*/
   /*> if (rc >= 0)  rc = yURG_urgs    (argc, argv);                                  <*/
   /*> if (rc >= 0)  rc = PROG_args    (argc, argv);                                  <*/
   /*> if (rc >= 0)  rc = PROG_begin   ();                                            <*/
   /*> if (rc >= 0)  rc = PROG_final   ();                                            <*/
   /*> if (rc <  0)  {                                                                <* 
    *>    PROG_end     ();                                                            <* 
    *>    exit (-1);                                                                  <* 
    *> }                                                                              <*/
   /*---(arachne specific)---------------*/
   /*> TICK_init       ();                                                                      <* 
    *> printf ("script length %8.3lf\n", my.p_len);                                             <* 
    *> yVIKEYS_progress_config (0.0, my.p_len, '-', my.p_nline, NULL, NULL, NULL, '-');         <* 
    *> /+> if (my.report == RPTG_MOVES)  yKINE_move_rpt  ();                              <+/   <* 
    *> yKINE_move_rpt  ();                                                                      <* 
    *> /+> TICK_draw     ();                                                              <+/   <*/
   /*---(main loop)----------------------*/
   /*> yURG_stage_check (YURG_STAGE_MAIN);                                            <* 
    *> rc = yVIKEYS_main  ("10ms", "100ms", NULL);                                    <* 
    *> yURG_stage_check (YURG_STAGE_WRAP);                                            <*/
   /*> PROG_end();                                                                    <*/
   /*> if (is_test) printf("\npetal writing v03 ----------------------------------end---\n\n");   <*/
   return 0;
}



/*============================----end-of-source---============================*/
