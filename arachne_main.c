/*============================----beg-of-source---============================*/

#include "arachne.h"


/*---(title)-----------------------------*/
char      win_title[100]    = "arachne_full";

/*---(sizes)-----------------------------*/
/*> int       win_w  = 1024;                    /+ window width                   +/   <*/
int       win_w  =  700;                    /* window width                   */
/*> int       win_h  =  768;                    /+ window height                  +/   <*/
int       win_h  =  600;                    /* window height                  */


int
main (int argc, char *argv[])
{
   /*---(locals)--------------------------------*/
   int rc = 0;
   /*---(run four stages)-----------------------*/
   if (rc == 0)  rc = prog_args(argc, argv);
   if (rc == 0)  rc = prog_begin();
   /*> if (is_test) printf("\nspider--------------------------------------------begin---\n\n");   <*/
   /*> strcpy(its_text, " ");                                                         <*/
   /*> counter = 0;                                                                   <*/
   struct timespec timer, remain;
   XKeyEvent *key_event;
   char  the_key[5];
   int   the_bytes;
   char  is_moved = 'n';
   /*> if (is_test) printf("handling the event loop...\n");                           <*/
   /*> printf("pre-while   : gk[0][CORE].cy = %8.1f, fk[0][CORE].cy = %8.1f\n", gk[0][CORE].cy, fk[0][CORE].cy);   <*/
   while (1) {
      while (XPending(DISP)) {
         XNextEvent(DISP, &EVNT);
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
            key_event  = (XKeyEvent *) &EVNT;
            the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
            if (the_bytes < 1) break;
            if (my_mode == 'o') {
               /*> printf("key = %3d\n", the_key[0]);                                 <*/
               switch (the_key[0]) {
               case ',': moving  = 'y';                    break;
               case '.': moving  = 'n';                    break;
               case '-': my_inc -= 1.0;                    break;
               case '+': my_inc += 1.0;                    break;
               case '_': moving  = 'n'; my_pos = 0;        break;
               case '$': moving  = 'n'; my_pos = my_len;   break;
               case '>': moving  = 'n'; ++my_pos;          break;
               case '<': moving  = 'n'; --my_pos;          break;
               case ')': my_pos += 10; moving = 'n';       break;
               case '(': my_pos -= 10; moving = 'n';       break;
               case 'a': if (flag_annotate == 'y') flag_annotate = 'n'; else flag_annotate = 'y'; break;
               case 'w': ++flag_view; if (flag_view > 4) flag_view = 0; break;
               case 'Q': exit(0);         break;
               case '\e': my_mode = 'p';  break;
               /* crab  /horz  */ case 'h': case 'l': case 'H': case 'L': 
               /* boom  /vert  */ case 'j': case 'k': case 'J': case 'K':
               /* dolly /zoom  */ case 'i': case 'I': case 'o': case 'O':
               /* x-axis/vant  */ case 'p': case 'P': case 'v': case 'V':
               /* y-axis/orbit */ case 'y': case 'Y': case 'c': case 'C':
               /* z-axis/spin  */ case 'r': case 'R': case 't': case 'T':
               case '0': case '1': case '2': case '3':
               case '4': case '5': case '6': case '7': case '8': case '9':
                  yGOD_key(the_key[0]);
                  break;
               case 'u': view_unit();      break;
               }
               if      (my_pitch  >   180.0) my_pitch  -= 360.0;
               else if (my_pitch  <= -180.0) my_pitch  += 360.0;
               if      (my_roll   >   180.0) my_roll   -= 360.0;
               else if (my_roll   <= -180.0) my_roll   += 360.0;
               if      (my_yaw    >   180.0) my_yaw    -= 360.0;
               else if (my_yaw    <= -180.0) my_yaw    += 360.0;
               if      (my_vpitch >   180.0) my_vpitch -= 360.0;
               else if (my_vpitch <= -180.0) my_vpitch += 360.0;
               if      (my_vroll  >   180.0) my_vroll  -= 360.0;
               else if (my_vroll  <= -180.0) my_vroll  += 360.0;
               if      (my_vyaw   >   180.0) my_vyaw   -= 360.0;
               else if (my_vyaw   <= -180.0) my_vyaw   += 360.0;
            } else if (my_mode == 'p') {
               switch (the_key[0]) {
               case 'o': my_mode = 'o';  break;
               case '>': my_inc *= 2.0;  break;
               case 'n': my_inc  = 1.0;  break;
               case '<': my_inc /= 2.0;  break;
               case '0': my_run  = 0  ; my_pos = 0;       break;
               case '$': my_run  = 0  ; my_pos = my_len;  break;
               case 'l': my_run  = 0  ; ++my_pos;         break;
               case 'h': my_run  = 0  ; --my_pos;         break;
               case 'Q': exit(0);        break;
               }
            }
            is_moved = 'y';
            break;
         }
      }
      /*> printf ("my_pos = %6.1f, my_ppos = %6.1f\n", my_pos, my_ppos);              <*/
      /*---(fix inc value)---------------*/
      if (my_inc >  8.0) my_inc =  8.0;
      if (my_inc < -8.0) my_inc = -8.0;
      /*---(check boundaries)------------*/
      if (my_pos <  0.0f)   {
         moving = 'n';
         my_pos = 0;
      }
      if (my_pos >= my_len) {
         moving = 'n';
         my_pos = my_len - 1;
      }
      gait.pos = my_pos;
      /*---(check boundaries)------------*/
      if (moving == 'y')  my_pos += my_inc;
      if (moving == 'y')  my_pos += 1;
      /*> scrp_vertical (my_pos);                                                     <*/
      /*> scrp_hulahoop (my_pos);                                                     <*/
      /*> scrp_sideside (my_pos);                                                     <*/
      /*> scrp_tripod   (my_pos);                                                     <*/
      /*> scrp_wave     (my_pos);                                                     <*/
      /*> scrp_quad     (my_pos);                                                     <*/
      /*> scrp_position (my_pos);                                                     <*/
      stat_masscenter();
      draw_main();
      if (umake_leg  == 'y')  unit_FK();
      is_moved = 'n';
      my_ppos = my_pos;
      /*---(wait)-----------------------------------------*/
      timer.tv_sec  = 0;
      /*> timer.tv_nsec = 100000000;   /+---(0.1 sec)---+/                            <*/
      /*> timer.tv_nsec =  50000000;   /+---(0.05 sec)---+/                           <*/
      /*> timer.tv_nsec =  25000000;   /+---(0.025 sec)---+/                      <*/
      /*> timer.tv_nsec =      50000000 * my_inc;   /+---(0.005 sec)---+/             <*/
      timer.tv_nsec =      50000000;            /*---(0.005 sec)---*/
      nanosleep(&timer, &remain);
   }
   /*> if (is_test) printf("   - done\n\n");                                          <*/
   prog_end();
   /*> if (is_test) printf("\npetal writing v03 ----------------------------------end---\n\n");   <*/
   return 0;
}

char       /* PURPOSE : process the command line arguments                    */
prog_args          (int argc, char *argv[])
{
   char     *a         = NULL;
   int       i         = 0;
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if      (strcmp(a, "--moving"     ) == 0)  moving       = 'y';
      else if (strcmp(a, "--sizing"     ) == 0)  debug_sizing = 'y';
      else if (strcmp(a, "--setup"      ) == 0)  umake_setup  = 'y';
      else if (strcmp(a, "--init"       ) == 0)  umake_init   = 'y';
      else if (strcmp(a, "--model"      ) == 0) {
         if (i + 1 <  argc)  strncpy (model_name, argv[++i], 25);
         printf("model     = %s\n", model_name);
      }
      else if (strcmp(a, "--y"          ) == 0) {
         if (i + 1 <  argc)  arg_y    = atof(argv[++i]);
      }
      else if (strcmp(a, "--thor"       ) == 0) {
         if (i + 1 <  argc)  arg_thor = atof(argv[++i]);
      }
      else if (strcmp(a, "--FK"         ) == 0) {
         if (i + 4 <  argc)  {
            arg_thor = atof(argv[++i]);
            arg_femu = atof(argv[++i]);
            arg_pate = atof(argv[++i]);
            arg_tibi = atof(argv[++i]);
            umake_leg    = 'y';
         }
      }
   }
   return 0;
}

char       /*----: drive program setup activities ----------------------------*/
prog_begin         (void)
{
   stat_init    (model_name);
   kine_init    ();
   yXINIT_start (win_title, win_w, win_h, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   draw_begin   ();
   draw_setup   ();
   font_load    ();
   dlist_begin  ();
   if (umake_init == 'y') {
      unit_init();
      exit(0);
   }
   yGOD_start();
   kinetics_scripter();
   gait.dmax   = 100;
   draw_main      ();
   /*> scrp_init      ();                                                             <*/
   stat_masscenter();
   return 0;
}

char       /*----: process the xwindows event stream -------------------------*/
prog_event         (void)
{
   return 0;
}

char       /*----: drive the program closure activities ----------------------*/
prog_end           (void)
{
   font_delete ();
   dlist_end   ();
   draw_end    ();
   yXINIT_end  ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
font_load          (void)
{
   txf_bg  = yFONT_load(face_bg);
   if (txf_bg <  0) {
      fprintf(stderr, "Problem loading %s\n", face_bg);
      exit(1);
   }
   txf_sm  = yFONT_load(face_sm);
   if (txf_sm <  0) {
      fprintf(stderr, "Problem loading %s\n", face_sm);
      exit(1);
   }
   return 0;
}

char
font_delete        (void)
{
   yFONT_free(txf_bg);
   yFONT_free(txf_sm);
   return 0;
}


/*============================----end-of-source---============================*/
