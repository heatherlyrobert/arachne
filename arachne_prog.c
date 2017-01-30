/*============================----beg-of-source---============================*/

#include "arachne.h"

tACCESSOR   my;

tGAIT     gait;



char      verstring    [500];

char*      /* ---- : return library versioning information -------------------*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strncpy (t, "[cbang      ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, VER_NUM, VER_TXT);
   return verstring;
}

char       /*----: very first setup ------------------s-----------------------*/
PROG_init          (void)
{
   /*---(log header)------------------*/
   DEBUG_TOPS   yLOG_info     ("purpose" , "wickedly useful spider robot visualization");
   DEBUG_TOPS   yLOG_info     ("namesake", "beautiful young female master weaver transformed into a spider");
   DEBUG_TOPS   yLOG_info     ("arachne" , PROG_version    ());
   DEBUG_TOPS   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_TOPS   yLOG_info     ("yKINE"   , yKINE_version   ());
   DEBUG_TOPS   yLOG_info     ("yX11"    , yX11_version    ());
   DEBUG_TOPS   yLOG_info     ("yFONT"   , yFONT_version   ());
   DEBUG_TOPS   yLOG_info     ("yVIKEYS" , yVIKEYS_version ());
   DEBUG_TOPS   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_TOPS   yLOG_info     ("yLOG"    , yLOG_version    ());
   /*---(header)----------------------*/
   DEBUG_TOPS   yLOG_enter (__FUNCTION__);
   /*---(window configuration)-----------*/
   strlcpy (my.w_title, "arachne", LEN_STR);
   /*---(screen configuration)-----------*/
   my.scrn      = SCRN_NORM;
   my.report    = RPTG_NONE;
   /*---(widths)-------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window widths");
   my.s_wide = my.c_wide = my.p_wide = 1000;
   my.t_wide =  20;
   my.w_wide = my.s_wide + my.t_wide;
   /*---(lefts)--------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window left positions");
   my.t_left =   0;
   my.s_left = my.c_left = my.p_left = my.t_wide;
   /*---(talls)--------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window heights");
   my.c_tall =  20;  
   my.p_tall = 125;  
   my.s_tall = 580;
   my.w_tall = my.t_tall = my.c_tall + my.p_tall + my.s_tall;
   /*---(bottoms)------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window bottom positions");
   my.c_bott = my.t_bott =   0;  
   my.p_bott = my.c_tall;
   my.s_bott = my.p_bott + my.p_tall;
   /*---(progress ticker)----------------*/
   DEBUG_TOPS   yLOG_note  ("defaul progress ticker values");
   my.p_len     =  0.0;
   my.p_leg     =  0.0;
   my.p_cursec  =  0.0;
   my.p_endsec  = -1.0;
   my.p_quit    =  '-';
   my.p_dump    =  '-';
   /*---(fonss)--------------------------*/
   strlcpy (my.face  , "comfortaa"    , LEN_LABEL);
   strlcpy (my.t_text, "arachne, spider robot visualization and simulation"       , LEN_STR  );
   /*---(setup modes)--------------------*/
   DEBUG_TOPS   yLOG_note  ("prepare modes");
   yVIKEYS_mode_init    ();
   yVIKEYS_mode_enter   (MODE_GOD);
   /*---(time)---------------------------*/
   DEBUG_TOPS   yLOG_note  ("prepare scales");
   yVIKEYS_scale_set    ("d-", &my.p_inc);
   /*---(speed)--------------------------*/
   DEBUG_TOPS   yLOG_note  ("prepare speeds");
   yVIKEYS_speed_set    ("+1.00x", &my.p_waitns);
   yVIKEYS_speed_stop   (&my.p_waitns);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit  (__FUNCTION__);
   return 0;
}

char               /* PURPOSE : process the command line arguments            */
PROG_args          (int argc, char *argv[])
{
   /*---(locals)-------------------------*/
   int         i           = 0;
   char       *a           = NULL;
   int         x_total     = 0;
   int         x_args      = 0;
   /*---(header)-------------------------*/
   /*> printf ("entering PROG_args\n");                                               <*/
   DEBUG_ARGS  yLOG_enter   (__FUNCTION__);
   /*---(begin)--------------------------*/
   strlcpy (my.f_base  , FILE_BLANK , LEN_STR);
   strlcpy (my.f_suffix, FILE_SUFFIX, LEN_STR);
   strlcpy (my.f_name  , ""         , LEN_STR);
   /*---(process)------------------------*/
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      ++x_total;
      if (a[0] == '@')  continue;
      DEBUG_ARGS  yLOG_info    ("cli arg", a);
      ++x_args;
      if      (strcmp(a, "--play"       ) == 0)  yVIKEYS_speed_play   (&my.p_waitns);
      else if (strcmp(a, "--pause"      ) == 0)  yVIKEYS_speed_stop   (&my.p_waitns);
      else if (strcmp(a, "--quit"       ) == 0)  my.p_quit = 'y';
      else if (strcmp(a, "--RR"         ) == 0)  my.p_leg  = 0.0;
      else if (strcmp(a, "--RM"         ) == 0)  my.p_leg  = 1.0;
      else if (strcmp(a, "--RF"         ) == 0)  my.p_leg  = 2.0;
      else if (strcmp(a, "--LF"         ) == 0)  my.p_leg  = 3.0;
      else if (strcmp(a, "--LM"         ) == 0)  my.p_leg  = 4.0;
      else if (strcmp(a, "--LR"         ) == 0)  my.p_leg  = 5.0;
      else if (strcmp(a, "--dump"       ) == 0)  my.p_dump = 'c';
      else if (strcmp(a, "--dumpall"    ) == 0)  my.p_dump = 'a';
      else if (strcmp(a, "--moves_rpt"  ) == 0)  my.report = RPTG_MOVES;
      else if (strcmp(a, "--begsec"     ) == 0) {
         if (i + 1 <  argc)  my.p_cursec = atof (argv[++i]);
      }
      else if (strcmp(a, "--endsec"     ) == 0) {
         if (i + 1 <  argc)  my.p_endsec = atof (argv[++i]);
      }
      else if (strcmp(a, "--scale"      ) == 0) {
         if (i + 1 <  argc) {
            yVIKEYS_scale_set    (argv[++i], &my.p_inc);
         }
      }
      else if (strcmp(a, "--progress"   ) == 0) {
         yVIKEYS_mode_enter  (MODE_PROGRESS);
      }
      else if (a[0] != '-'                     ) {
         DEBUG_ARGS  yLOG_note   ("found a file name");
         strncpy (my.f_base , a        , LEN_STR);
      }
   }
   DEBUG_ARGS  yLOG_value  ("entries"   , x_total);
   DEBUG_ARGS  yLOG_value  ("arguments" , x_args);
   if (x_args == 0) {
      DEBUG_ARGS  yLOG_note   ("no arguments identified");
   }
   /*---(update title)-------------------*/
   if (strcmp (my.f_base, FILE_BLANK) != 0) {
      sprintf (my.f_name  , "%s.%s", my.f_base, my.f_suffix);
   } else {
      sprintf (my.f_name  , "%s", FILE_STDIN);
   }
   DEBUG_ARGS  yLOG_info   ("file base" , my.f_base);
   DEBUG_ARGS  yLOG_info   ("file suf"  , my.f_suffix);
   DEBUG_ARGS  yLOG_info   ("file name" , my.f_name);
   /*---(complete)-----------------------*/
   DEBUG_ARGS  yLOG_exit  (__FUNCTION__);
   return 0;
}


char       /*----: drive program setup activities ----------------------------*/
PROG_begin         (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   stat_init    (model_name);
   KINE_begin   ();
   DEBUG_ARGS  yLOG_info   ("title"     , my.w_title);
   DEBUG_ARGS  yLOG_value  ("width"     , my.w_wide);
   DEBUG_ARGS  yLOG_value  ("height"    , my.w_tall);
   yXINIT_start (my.w_title, my.w_wide, my.w_tall, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   DRAW_begin   ();
   draw_setup   ();
   font_load    ();
   dlist_begin  ();
   yGOD_start();
   gait.dmax   = 100;
   /*> stat_masscenter();                                                             <*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*----: process the xwindows event stream -------------------------*/
prog_event         (void)
{
   return 0;
}

char       /*----: drive the program closure activities ----------------------*/
PROG_end           (void)
{
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   KINE_end    ();
   font_delete ();
   dlist_end   ();
   DRAW_end    ();
   yXINIT_end  ();
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   DEBUG_TOPS   yLOG_end     ();
   return 0;
}


/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
font_load          (void)
{
   my.font  = yFONT_load (my.face);
   if (my.font <  0) {
      fprintf(stderr, "Problem loading %s\n", my.face);
      exit(1);
   }
   return 0;
}

char
font_delete        (void)
{
   yFONT_free(my.font);
   return 0;
}

