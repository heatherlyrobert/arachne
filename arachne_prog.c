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
   DEBUG_TOPS   yLOG_info     ("yGLTEX"  , yGLTEX_version  ());
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
   /*---(progress ticker)----------------*/
   DEBUG_TOPS   yLOG_note  ("defaul progress ticker values");
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
   yVIKEYS_init  ();
   /*> yVIKEYS_mode_init    ();                                                       <* 
    *> yVIKEYS_mode_enter   (MODE_GOD);                                               <*/
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
      /*> if      (strcmp(a, "--play"       ) == 0)  yVIKEYS_speed_play   ();       <* 
       *> else if (strcmp(a, "--pause"      ) == 0)  yVIKEYS_speed_stop   ();       <* 
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
       *>    if (i + 1 <  argc)  my.p_cur = atof (argv[++i]);                                <* 
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
   /*> yXINIT_start (my.w_title, my.w_wide, my.w_tall, YX_FOCUSABLE, YX_FIXED, YX_SILENT);   <*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*----: process the xwindows event stream -------------------------*/
PROG_final         (void)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(window and panels)--------------*/
   yVIKEYS_view_config   ("arachne, hexapod visualization and simulation", VER_NUM, YVIKEYS_OPENGL, 800, 500, 0);
   yVIKEYS_view_setup    (YVIKEYS_MAIN     , YVIKEYS_DEPTH, YVIKEYS_MIDCEN, -400, 400, 250, -250, -1000, 1000, YCOLOR_BAS    , DRAW_primary);
   yVIKEYS_view_simple   (YVIKEYS_PROGRESS , YCOLOR_BAS   , TICK_show   );
   yVIKEYS_cmds_direct   (":xaxis    disable");
   yVIKEYS_cmds_direct   (":yaxis    disable");
   yVIKEYS_cmds_direct   (":ribbon   disable");
   yVIKEYS_cmds_direct   (":nav      disable");
   yVIKEYS_cmds_direct   (":alt      disable");
   yVIKEYS_cmds_direct   (":formula  disable");
   yVIKEYS_cmds_direct   (":layout   work");
   yVIKEYS_cmds_direct   (":progress show");
   yVIKEYS_cmds_direct   (":buffer   show");
   /*---(colors)-------------------------*/
   yVIKEYS_cmds_direct   (":palette 100 rcomp pale earthy");
   yVIKEYS_view_colors   (YCOLOR_POS, YCOLOR_BAS, YCOLOR_NEG, YCOLOR_POS);
   yVIKEYS_cmds_add ('a', "p_cond"      , ""    , ""     , KINE_unitcond        , "write out a unit testing condition for yKINE"    );

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
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   KINE_end    ();
   font_delete ();
   dlist_end   ();
   DRAW_end    ();
   /*> yXINIT_end  ();                                                                <*/
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

