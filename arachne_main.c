/*============================----beg-of-source---============================*/

#include "arachne.h"


int
main (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int         updates     = 0;
   char        rc          = 0;
   char        x_savemode  = '-';
   tTSPEC      timer;
   XKeyEvent  *key_event;
   char        the_key[5];
   int         the_bytes;
   char        is_moved = 'n';
   char        x_draw   = '-';
   double      x_save   = 0.0;
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (argc, argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc >= 0)  rc = yURG_urgs    (argc, argv);
   if (rc >= 0)  rc = PROG_args    (argc, argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc >= 0)  rc = PROG_final   ();
   if (rc <  0)  {
      PROG_end     ();
      exit (-1);
   }
   /*---(arachne specific)---------------*/
   yKINE_script  (&my.p_len);
   yVIKEYS_prog_script (0.0, my.p_len);
   /*> if (my.report == RPTG_MOVES)  yKINE_moves_rpt ();                              <*/
   printf ("script length %8.3lf\n", my.p_len);
   TICK_init     ();
   TICK_draw     ();
   /*---(main loop)----------------------*/
   rc = yVIKEYS_main  ("10ms", "100ms", NULL);
   PROG_end();
   /*> if (is_test) printf("\npetal writing v03 ----------------------------------end---\n\n");   <*/
   return 0;
}



/*============================----end-of-source---============================*/
