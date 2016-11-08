/*============================----beg-of-source---============================*/

#include "arachne.h"


int
main (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int         updates     = 0;
   char        rc          = 0;
   char        x_savemode  = '-';
   /*---(initialize)---------------------*/
   if (rc == 0)  rc = PROG_logger  (argc, argv);
   if (rc == 0)  rc = PROG_init    ();
   if (rc == 0)  rc = PROG_urgs    (argc, argv);
   if (rc == 0)  rc = PROG_args    (argc, argv);
   if (rc == 0)  rc = PROG_begin   ();
   if (rc != 0)  {
      PROG_end     ();
      exit (-1);
   }
   SCRP_main   ();
   draw_main   ();
   TICK_init   ();
   TICK_draw   ();
   /*> if (is_test) printf("\nspider--------------------------------------------begin---\n\n");   <*/
   /*> strcpy(its_text, " ");                                                         <*/
   /*> counter = 0;                                                                   <*/
   struct timespec timer;
   XKeyEvent *key_event;
   char  the_key[5];
   int   the_bytes;
   char  is_moved = 'n';
   /*> if (is_test) printf("handling the event loop...\n");                           <*/
   /*> printf("pre-while   : gk[0][CORE].cy = %8.1f, fk[0][CORE].cy = %8.1f\n", gk[0][CORE].cy, fk[0][CORE].cy);   <*/
   yVIKEYS_mode_mesg (my.message, "");
   while (1) {
      while (XPending(DISP)) {
         /*---(start processing event)---*/
         XNextEvent(DISP, &EVNT);
         ++updates;
         switch(EVNT.type) {
         case Expose:
            /*> if (is_test_inp) printf("   - exposed  %d times\n", EVNT.xexpose.count);   <*/
            is_moved = 'y';
            break;
         case ConfigureNotify:
            if (EVNT.xconfigure.width != X || EVNT.xconfigure.height != Y) {
               /*> if (is_test_inp) printf("   - moved    x=%4d, y=%4d", X, Y);   <*/
               X = EVNT.xconfigure.width;
               Y = EVNT.xconfigure.height;
               /*> if (is_test_inp) printf("  TO  x=%4d, y=%4d\n", X, Y);             <*/
               /*> draw_main();                                                       <*/
            }
            if (EVNT.xconfigure.width != (int) WIDTH || EVNT.xconfigure.height != (int) HEIGHT) {
               /*> if (is_test_inp) printf("   - resized  w=%3d, h=%3d\n",            <* 
                *>       EVNT.xconfigure.width, EVNT.xconfigure.height);        <*/
               glx_resize(EVNT.xconfigure.width, EVNT.xconfigure.height);
            }
            is_moved = 'y';
            break;
         case KeyPress:
            /*---(prepare)---------------*/
            key_event  = (XKeyEvent *) &EVNT;
            the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
            if (the_bytes < 1) break;
            /*---(handle)----------------*/
            my.cch      = the_key [0];
            x_savemode  = yVIKEYS_mode_curr ();
            switch (x_savemode) {
            case MODE_GOD      : rc = MODE_god      (my.sch, my.cch); break;
            case MODE_PROGRESS : rc = MODE_progress (my.sch, my.cch); break;
                                 /*> case MODE_MAP      : rc = MODE_map      (my.sch, my.cch); break;            <* 
                                  *> case MODE_VISUAL   : rc = VISU_mode     (my.sch, my.cch); break;            <* 
                                  *> case MODE_SOURCE   : rc = MODE_source   (my.sch, my.cch); break;            <* 
                                  *> case MODE_INPUT    : rc = MODE_input    (my.sch, my.cch); break;            <* 
                                  *> case MODE_COMMAND  : rc = MODE_command  (' ', my.cch); break;            <* 
                                  *> case SMOD_ERROR    : rc = SMOD_error    (my.sch, my.cch); break;            <* 
                                  *> case SMOD_SELECT   : rc = SELC_mode     (my.sch, my.cch); break;            <* 
                                  *> case SMOD_TEXTREG  : rc = TREG_mode     (my.sch, my.cch); break;            <* 
                                  *> case SMOD_REPLACE  : rc = SMOD_replace  (my.sch, my.cch); break;            <* 
                                  *> case SMOD_FORMAT   : rc = SMOD_format   (' ', my.cch); break;            <* 
                                  *> case SMOD_BUFFER   : rc = SMOD_buffer   (' ', my.cch); break;            <* 
                                  *> case SMOD_WANDER   : rc = SMOD_wander   (' ', my.cch); break;            <* 
                                  *> case SMOD_REGISTER : rc = REG_mode      (my.sch, my.cch); break;            <* 
                                  *> case SMOD_MARK     : rc = MARK_mode     (my.sch, my.cch); break;            <* 
                                  *> case SMOD_MENUS    : rc = SMOD_menus    (my.sch, my.cch); break;            <*/
            default            : rc = MODE_god      (my.sch, my.cch); break;
            }
            /*---(setup for next keystroke)----*/
            if      (rc == 0)    my.sch = ' ';
            else if (rc >  0)    my.sch = rc;
            else               { my.sch = ' ';  my.sta_error = 'y'; }
            /*---(setup status line)-----*/
            if   (x_savemode != yVIKEYS_mode_curr() || yVIKEYS_mode_curr() == MODE_COMMAND) {
               yVIKEYS_mode_mesg (my.message, "");
            }
            /*---(progress)-------------*/
            /*> if (my_mode == 'o') {                                                 <* 
             *>    switch (the_key[0]) {                                              <* 
             *>    case '+': SCALE_smaller ();   /+ in  +/     break;                 <* 
             *>    case '-': SCALE_larger  ();   /+ out +/     break;                 <* 
             *>    case ',': my.p_moving  = 'y';                    break;                 <* 
             *>    case '.': my.p_moving  = 'n';                    break;                 <* 
             *>    case '>': my.p_moving  = 'n'; my_pos +=  my.p_inc;          break;      <* 
             *>    case '<': my.p_moving  = 'n'; my_pos -=  my.p_inc;          break;      <* 
             *>    case ')': my.p_moving  = 'n'; my_pos +=  my.p_inc * 5;      break;      <* 
             *>    case '(': my.p_moving  = 'n'; my_pos -=  my.p_inc * 5;      break;      <* 
             *>    }                                                                  <* 
             *> }                                                                     <*/
            /*---(unknown)--------------*/
            /*> case 'a': if (flag_annotate == 'y') flag_annotate = 'n'; else flag_annotate = 'y'; break;   <*/
            /*> case 'w': ++flag_view; if (flag_view > 4) flag_view = 0; break;    <*/
            /*> case 'u': view_unit();      break;                                    <*/
            /*> if (my_mode == 'p') {                                                 <* 
             *>    switch (the_key[0]) {                                              <* 
             *>    case 'o': my_mode = 'o';  break;                                   <* 
             *>    case '>': my_inc *= 2.0;  break;                                   <* 
             *>    case 'n': my_inc  = 1.0;  break;                                   <* 
             *>    case '<': my_inc /= 2.0;  break;                                   <* 
             *>    case '0': my_run  = 0  ; my_pos = 0;       break;                  <* 
             *>    case '$': my_run  = 0  ; my_pos = my_len;  break;                  <* 
             *>    case 'l': my_run  = 0  ; ++my_pos;         break;                  <* 
             *>    case 'h': my_run  = 0  ; --my_pos;         break;                  <* 
             *>    case 'Q': exit(0);        break;                                   <* 
             *>    }                                                                  <* 
             *> }                                                                     <*/
            if (the_key[0] == 'Q')  exit(0);
            is_moved = 'y';
            break;
         }
      }
      /*> printf ("my_pos = %6.1f, my_ppos = %6.1f\n", my_pos, my_ppos);              <*/
      /*---(check boundaries)------------*/
      /*> if (my.p_moving == 'y')  my_pos += my.p_adv;                                <*/
      yVIKEYS_speed_adv  (&my_pos);
      if (my_pos <  0.0f)   {
         yVIKEYS_speed_stop (&my.p_waitns);
         my_pos = 0.0;
      }
      if (my_pos >= my.p_len) {
         yVIKEYS_speed_stop (&my.p_waitns);
         my_pos = my.p_len;
      }
      gait.pos = my_pos;
      /*---(check boundaries)------------*/
      stat_masscenter();
      draw_main();
      if (umake_leg  == 'y')  unit_FK();
      is_moved = 'n';
      my_ppos = my_pos;
      /*---(wait)-----------------------------------------*/
      timer.tv_sec  = 0;
      timer.tv_nsec =  my.p_waitns;
      nanosleep (&timer, NULL);
   }
   /*> if (is_test) printf("   - done\n\n");                                          <*/
   PROG_end();
   /*> if (is_test) printf("\npetal writing v03 ----------------------------------end---\n\n");   <*/
   return 0;
}

/*============================----end-of-source---============================*/
