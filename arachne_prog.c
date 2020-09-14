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
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

char       /*----: very first setup ------------------s-----------------------*/
PROG_init          (void)
{
   /*---(log header)------------------*/
   DEBUG_TOPS   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_TOPS   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_TOPS   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_TOPS   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_TOPS   yLOG_info     ("reason"  , P_REASON);
   DEBUG_TOPS   yLOG_info     ("arachne" , PROG_version    ());
   DEBUG_TOPS   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_TOPS   yLOG_info     ("yKINE"   , yKINE_version   ());
   DEBUG_TOPS   yLOG_info     ("yGOLEM"  , yGOLEM_version  ());
   DEBUG_TOPS   yLOG_info     ("yX11"    , yX11_version    ());
   DEBUG_TOPS   yLOG_info     ("yFONT"   , yFONT_version   ());
   DEBUG_TOPS   yLOG_info     ("yGLTEX"  , yGLTEX_version  ());
   DEBUG_TOPS   yLOG_info     ("yVIKEYS" , yVIKEYS_version ());
   DEBUG_TOPS   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_TOPS   yLOG_info     ("yLOG"    , yLOGS_version   ());
   /*---(stage check)-----------------*/
   yURG_stage_check (YURG_STAGE_INIT);
   /*---(header)----------------------*/
   DEBUG_TOPS   yLOG_enter (__FUNCTION__);
   /*---(window configuration)-----------*/
   strlcpy (my.w_title, "arachne", LEN_STR);
   my.verify    = '-';
   /*---(screen configuration)-----------*/
   my.scrn      = SCRN_NORM;
   my.report    = RPTG_NONE;
   /*---(widths)-------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window widths");
   my.t_wide =  20;
   my.s_wide = 800;
   my.a_wide = 200;
   my.c_wide = my.p_wide =  my.s_wide + my.a_wide;
   my.w_wide = my.p_wide + my.t_wide;
   /*---(lefts)--------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window left positions");
   my.t_left =   0;
   my.s_left = my.c_left = my.p_left = my.t_wide;
   my.a_left = my.p_left + my.s_wide;
   /*---(talls)--------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window heights");
   my.c_tall =  20;  
   my.p_tall = 100;  
   my.s_tall = my.a_tall = 500;
   my.w_tall = my.t_tall = my.c_tall + my.p_tall + my.s_tall;
   /*---(bottoms)------------------------*/
   DEBUG_TOPS   yLOG_note  ("set window bottom positions");
   my.c_bott = my.t_bott =   0;  
   my.p_bott = my.c_tall;
   my.s_bott = my.a_bott = my.p_bott + my.p_tall;
   /*---(main window)--------------------*/
   my.m_xmin    =  -400;
   my.m_xmax    =   400;
   my.m_ymin    =  -250;
   my.m_ymax    =   250;
   my.m_zmin    = -1000;
   my.m_zmax    =  1000;
   /*---(progress ticker)----------------*/
   DEBUG_TOPS   yLOG_note  ("default progress ticker values");
   my.p_len     =  0.0;
   my.p_line    =  0.0;
   my.p_leg     =  0.0;
   my.p_cur     =  0.0;
   my.p_endsec  = -1.0;
   my.p_quit    =  '-';
   my.p_dump    =  '-';
   /*---(fonss)--------------------------*/
   strlcpy (my.face_pretty, "comfortaa"    , LEN_LABEL);
   strlcpy (my.face_fixed , "hack"         , LEN_LABEL);
   /*> strlcpy (my.t_text     , "arachne, spider robot visualization and simulation"       , LEN_STR  );   <*/
   /*---(setup yVIKEYS)------------------*/
   DEBUG_TOPS   yLOG_note  ("prepare modes");
   yVIKEYS_init   (MODE_GOD);
   FILE_init      ();
   yGOLEM_init    ();
   /*> yVIKEYS_mode_init    ();                                                       <* 
    *> yVIKEYS_mode_enter   (MODE_GOD);                                               <*/
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit  (__FUNCTION__);
   return 0;
}

char               /* PURPOSE : process the command line arguments            */
PROG_args          (int argc, char *argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char       *a           = NULL;
   int         x_total     =    0;
   int         x_args      =    0;
   uchar       x_name      [LEN_FULL]  = "";
   uchar       t           [LEN_FULL]  = "";
   /*---(header)-------------------------*/
   /*> printf ("entering PROG_args\n");                                               <*/
   DEBUG_ARGS  yLOG_enter   (__FUNCTION__);
   /*---(begin)--------------------------*/
   strlcpy (my.f_base  , FILE_BLANK , LEN_STR);
   strlcpy (my.f_suffix, P_SUFFIX   , LEN_STR);
   strlcpy (my.f_name  , ""         , LEN_STR);
   /*---(process)------------------------*/
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      ++x_total;
      if (a[0] == '@')  continue;
      DEBUG_ARGS  yLOG_info    ("cli arg", a);
      ++x_args;
      if      (strcmp(a, "--verify"     ) == 0)  my.verify = 'y';
      /*> else if (strcmp(a, "--play"       ) == 0)  yVIKEYS_speed_play   ();                   <* 
       *> else if (strcmp(a, "--pause"      ) == 0)  yVIKEYS_speed_stop   ();                   <* 
       *> else if (strcmp(a, "--quit"       ) == 0)  my.p_quit = 'y';                           <* 
       *> else if (strcmp(a, "--RR"         ) == 0)  my.p_leg  = 0.0;                           <* 
       *> else if (strcmp(a, "--RM"         ) == 0)  my.p_leg  = 1.0;                           <* 
       *> else if (strcmp(a, "--RF"         ) == 0)  my.p_leg  = 2.0;                           <* 
       *> else if (strcmp(a, "--LF"         ) == 0)  my.p_leg  = 3.0;                           <* 
       *> else if (strcmp(a, "--LM"         ) == 0)  my.p_leg  = 4.0;                           <* 
       *> else if (strcmp(a, "--LR"         ) == 0)  my.p_leg  = 5.0;                           <* 
       *> else if (strcmp(a, "--dump"       ) == 0)  my.p_dump = 'c';                           <* 
       *> else if (strcmp(a, "--dumpall"    ) == 0)  my.p_dump = 'a';                           <* 
       *> else if (strcmp(a, "--moves_rpt"  ) == 0)  my.report = RPTG_MOVES;                    <* 
       *> else if (strcmp(a, "--begsec"     ) == 0) {                                           <* 
       *>    if (i + 1 <  argc)  my.p_cur = atof (argv[++i]);                                   <* 
       *> }                                                                                     <* 
       *> else if (strcmp(a, "--endsec"     ) == 0) {                                           <* 
       *>    if (i + 1 <  argc)  my.p_endsec = atof (argv[++i]);                                <* 
       *> }                                                                                     <* 
       *> else if (strcmp(a, "--progress"   ) == 0) {                                           <* 
       *>    /+> yVIKEYS_mode_enter  (MODE_PROGRESS);                                     <+/   <* 
       *> }                                                                                     <* 
       *> else if (a[0] != '-'                     ) {                                          <* 
       *>    DEBUG_ARGS  yLOG_note   ("found a file name");                                     <* 
       *>    strncpy (my.f_base , a        , LEN_STR);                                          <* 
       *> }                                                                                     <*/
      else if (a[0] != '-'                     )   strlcpy (x_name , argv[i]  , LEN_RECD);
   }
   DEBUG_ARGS  yLOG_value  ("entries"   , x_total);
   DEBUG_ARGS  yLOG_value  ("arguments" , x_args);
   if (x_args == 0) {
      DEBUG_ARGS  yLOG_note   ("no arguments identified");
   }
   /*---(update title)-------------------*/
   if (strlen (x_name) > 0) {
      sprintf (t, ":file %s", x_name);
      yVIKEYS_cmds_direct (t);
   }
   /*---(update title)-------------------*/
   /*> if (strcmp (my.f_base, FILE_BLANK) != 0) {                                     <* 
    *>    sprintf (my.f_name  , "%s.%s", my.f_base, my.f_suffix);                     <* 
    *> } else {                                                                       <* 
    *>    sprintf (my.f_name  , "%s", FILE_STDIN);                                    <* 
    *> }                                                                              <*/
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
   char        rc          =    0;
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_TOPS   yLOG_info    ("mode_name" , model_name);
   rc = stat_init    (model_name);
   DEBUG_TOPS   yLOG_value   ("stat_init" , rc);
   rc = KINE_begin   ();
   DEBUG_TOPS   yLOG_value   ("KINE_begin", rc);
   DEBUG_ARGS  yLOG_info   ("title"     , my.w_title);
   DEBUG_ARGS  yLOG_value  ("width"     , my.w_wide);
   DEBUG_ARGS  yLOG_value  ("height"    , my.w_tall);
   /*> yXINIT_start (my.w_title, my.w_wide, my.w_tall, YX_FOCUSABLE, YX_FIXED, YX_SILENT);   <*/
   /*> my.height    = 139.7;                                                          <*/
   my.std_height    = yKINE_seglen (YKINE_TIBI) + yKINE_seglen (YKINE_FOOT);
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*----: process the xwindows event stream -------------------------*/
PROG_final         (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   api_yvikeys_init      ();
   yVIKEYS_cmds_direct (":read");
   yKINE_tick_load ();
   yURG_stage_check (YURG_STAGE_INIT);
   DRAW_init    ();
   draw_setup   ();
   font_load    ();
   dlist_begin  ();
   yGOD_start   ();
   gait.dmax   = 100;
   /*> stat_masscenter();                                                             <*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*----: drive the program closure activities ----------------------*/
PROG_end           (void)
{
   /*---(shutdown)--------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   KINE_end    ();
   font_delete ();
   dlist_end   ();
   DRAW_end    ();
   /*> yXINIT_end  ();                                                                <*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   DEBUG_TOPS   yLOGS_end    ();
   return 0;
}


/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
font_load          (void)
{
   my.fixed  = yFONT_load (my.face_fixed);
   if (my.fixed <  0) {
      fprintf(stderr, "Problem loading %s\n", my.face_fixed);
      exit(1);
   }
   my.pretty  = yFONT_load (my.face_pretty);
   if (my.pretty <  0) {
      fprintf(stderr, "Problem loading %s\n", my.face_pretty);
      exit(1);
   }
   return 0;
}

char
font_delete        (void)
{
   yFONT_free (my.fixed);
   yFONT_free (my.pretty);
   return 0;
}

