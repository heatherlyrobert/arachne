
/*============================----beg-of-source---============================*/

#include "arachne.h"

tDEBUG      debug;

/*---(title)-----------------------------*/
char      win_title[100]    = "arachne_full";

/*---(sizes)-----------------------------*/
/*> int       win_w  = 1024;                    /+ window width                   +/   <*/
int       win_w  =  700;                    /* window width                   */
/*> int       win_h  =  768;                    /+ window height                  +/   <*/
int       win_h  =  600;                    /* window height                  */


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

