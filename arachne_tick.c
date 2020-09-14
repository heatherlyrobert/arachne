/*============================----beg-of-source---============================*/

#include "arachne.h"

/*===[[ METIS ]]==============================================================*/
/*
 * metis  tw2#·  add large seconds indicator to top half of ticker line
 * metis  tw1#·  scale/speed indicators are not updating on header of ticker lines
 * metis  dw2#·  progress servo lines go to zero at panel ends rather than true values
 *
 *
 *
 */




/*====================------------------------------------====================*/
/*===----                       adjusted values                        ----===*/
/*====================------------------------------------====================*/
static void      o___ADJUSTED________________o (void) {;}

/*---1----- -----2----- -----3----- -----4-----  ---------comments------------*/
typedef     struct      cPOS        tPOS;
struct      cPOS {
   /*---(special)------------------------*/
   char        used;
   char        exact;
   char       *label;
   char        cell;
   /*---(planned angles)-----------------*/
   float       o_femu;
   float       o_pate;
   float       o_tibi;
   /*---(planned endpoint)---------------*/
   float       o_xpos;
   float       o_zpos;
   float       o_ypos;
   /*---(target endpoint)----------------*/
   float       t_xpos;
   float       t_zpos;
   float       t_ypos;
   /*---(revised angles)-----------------*/
   float       r_femu;
   float       r_pate;
   float       r_tibi;
   /*---(revised endpoint)---------------*/
   float       r_xpos;
   float       r_zpos;
   float       r_ypos;
   /*---(revised success)----------------*/
   char        r_rc;
   float       r_error;
   /*---(opengl endpoint)----------------*/
   float       g_xpos;
   float       g_zpos;
   float       g_ypos;
   float       g_error;
   /*---(done)---------------------------*/
};


#define     MAX_PANEL   5
#define     MAX_WIDE    400
typedef     struct      cTICKER     tTICKER;
struct      cTICKER {
   char        seq;                              /* true order for auditing   */
   uint        fbo;                              /* framebuffer               */
   uint        depth;                            /* depth buffer              */
   uint        tex;                              /* texture array             */
   float       beg;                              /* begining script position  */
   int         sect;                             /* each panel section assign */
   tPOS        detail [MAX_WIDE];                /* exact positions           */
};
static       tTICKER    s_ticker [MAX_PANEL];
static       tTICKER   *s_order  [MAX_PANEL];


/*---1----- -----2----- -----3----- -----4-----  ---------comments------------*/
typedef     struct      cPANEL      tPANEL;
struct      cPANEL {
   char        seq;                              /* true order for auditing   */
   uint        fbo;                              /* framebuffer               */
   uint        depth;                            /* depth buffer              */
   uint        tex;                              /* texture array             */
   float       beg;                              /* begining script position  */
   int         sect;                             /* each panel section assign */
   int         fill;                             /* how man drawn             */
   tPOS        detail [YKINE_MAX_LEGS][MAX_WIDE];/* exact positions           */
   char        act;                              /* action, like snapshot     */
};
/*---1----- -----2----- -----3----- -----4-----  ---------comments------------*/
static      tPANEL      s_panel     [MAX_PANEL];
static      tPANEL     *p_back      = NULL;
static      tPANEL     *p_pref      = NULL;
static      tPANEL     *p_curr      = NULL;
static      tPANEL     *p_suff      = NULL;
static      tPANEL     *p_draw      = NULL;      /* panel with current pos    */
static      tPANEL     *p_actv      = NULL;      /* panel being drawn         */

/*---1----- -----2----- -----3----- -----4-----  ---------comments------------*/
/*---(horizontal)---------------------*/
static      int         s_wide      =   4000;    /* texture panel width       */
static      int         s_xinc      =     10;    /* horizontal increment      */
static      char        s_anchor    =    'c';    /* horz scroll anchor point  */
/*---(vertical)-----------------------*/
static      int         s_title     =     30;    /* title at top of line      */
static      int         s_top       =    115;    /* positive part of each line*/
static      int         s_bot       =    -85;    /* negative part of each line*/
static      int         s_yinc      =    200;    /* vertical size of line     */
static      int         s_tall      =      0;    /* texture panel height      */
/*---(durations)----------------------*/
static      int         s_nsec      =      0;    /* total number of sections  */             
static      float       s_len       =    0.0;
/*---(debugging)----------------------*/
static      char        s_debug     =    '-';


#define        UNIT2PANEL       40.0


#define     MAX_LINES      20
typedef   struct cLINES  tLINES;
struct cLINES {
   char        name        [LEN_LABEL];
   char        active;
   char        content;
   int         leg;
};
static tLINES  s_line_info  [MAX_LINES] = {
   { "center/zero"      , 'y' , 'z' , YKINE_BODY  },
   { "orientation"      , 'y' , 'o' , YKINE_BODY  },
   { "right rear"       , 'y' , 'l' , YKINE_RR    },
   { "right middle"     , 'y' , 'l' , YKINE_RM    },
   { "right front"      , 'y' , 'l' , YKINE_RF    },
   { "left front"       , 'y' , 'l' , YKINE_LF    },
   { "left middle"      , 'y' , 'l' , YKINE_LM    },
   { "left rear"        , 'y' , 'l' , YKINE_LR    },
   { "right posterior"  , '-' , 'l' , YKINE_RP    },
   { "right anterior"   , '-' , 'l' , YKINE_RA    },
   { "left anterior"    , '-' , 'l' , YKINE_LA    },
   { "left posterior"   , '-' , 'l' , YKINE_LP    },
   { "beak"             , 'y' , 'p' , YKINE_BEAK  },
   { "cam offset"       , 'y' , 'p' , YKINE_CAMOF },
   { "cam orient"       , 'y' , 'p' , YKINE_CAMOR },
   { "---"              , 'x' , '-' , -1          },
};

/*---(single leg vars)----------------*/
static float       s_textop    =   0.0;    /* single leg top percent in texture   */
static float       s_texbot    =   0.0;    /* single leg bot percent in texture   */
static float       s_texavail  =   0.0;    /* tex width available in progress      */
static float       s_texpct    =   0.0;    /* tex width avail in percent of panel */
static float       s_tsec      =   0.0;         /* texture length of a second     */
/*> static float       s_tnsec     =   0.0;         /+ texture number of secs shown   +/   <*/
/*> static float       s_tsecp     =   0.0;         /+ width pct of a texture sec     +/   <*/
static float       s_plenp     =   0.0;         /* total length in texture pct    */
static float       s_texbeg    =   0.0;
static float       s_texend    =   0.0;
static float       s_curp      =   0.0;         /* cur pos in texture pct         */
static float       s_cur       =   0.0;

static float       s_start     =   0.0;

static int         s_section   =     0;         /* section of script          */
static char        s_sectext   [5];             /* section of script          */
static float       s_texbeg1   =   0.0;
static float       s_texend1   =   0.0;
static float       s_texpct1   =   0.0;
static float       s_texbeg2   =   0.0;
static float       s_texend2   =   0.0;
static float       s_texpct2   =   0.0;

static float       s_lowest    =   0.0;

static int       s_debug_leg;
static float     s_debug_sec;


char    TICK_curr (void);



/*====================------------------------------------====================*/
/*===----                         progress lines                       ----===*/
/*====================------------------------------------====================*/
static void      o___LINES___________________o (void) {;}

char
TICK_line_count         (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;          /* loop iterator                  */
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_senter  (__FUNCTION__);
   my.p_nline  =    0;
   /*---(cycle)--------------------------*/
   for (i = 0; i < MAX_LINES; ++i) {
      if (s_line_info [i].active == 'x')  break;
      /*> if (s_line_info [i].active == '-')  continue;                                   <*/
      ++my.p_nline;
   }
   DEBUG_GRAF   yLOG_sint    (my.p_nline);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
TICK_line_find          (int a_num)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;          /* loop iterator                  */
   int         c           =    0;
   char        rc          =   -1;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_senter  (__FUNCTION__);
   /*---(cycle)--------------------------*/
   for (i = 0; i < my.p_nline; ++i) {
      if (s_line_info [i].active == 'x')  break;
      /*> if (s_line_info [i].active == '-')  continue;                                   <*/
      if (c == a_num) {
         rc = i;
         break;
      }
      ++c;
   }
   DEBUG_GRAF   yLOG_sint    (rc);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                          background                          ----===*/
/*====================------------------------------------====================*/
static void      o___BACKGROUND______________o (void) {;}

char
TICK_back_vert          (void)
{
   /*---(locals)-----------+-----------+-*/
   int       x_pos         =    0;          /* loop iterator                  */
   int       x_line        =    0;          /* loop iterator                  */
   float     x_bot         =  0.0;
   float     x_top         =  0.0;
   char      x_msg         [100];
   int       x_small       =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   yVIKEYS_view_color (YCOLOR_NEG_MUT, 1.00);
   glLineWidth (2.0);
   /*---(vertical lines)-----------------*/
   for (x_line = 0; x_line < my.p_nline; ++x_line) {
      x_top = (x_line + 1) * s_yinc;
      x_bot = (x_line    ) * s_yinc;
      /*---(horizontal move)-------------*/
      for (x_pos = 0; x_pos <= s_wide; x_pos += s_xinc) {
         /*---(10.0x bars)---------------*/
         if (x_pos % (s_xinc * 100) == 0) {
            glBegin         (GL_POLYGON); {
               glVertex3f   (x_pos - s_xinc / 3.0, x_top, 0.0);
               glVertex3f   (x_pos + s_xinc / 3.0, x_top, 0.0);
               glVertex3f   (x_pos + s_xinc / 3.0, x_bot, 0.0);
               glVertex3f   (x_pos - s_xinc / 3.0, x_bot, 0.0);
            } glEnd   ();
            x_small = 0;
         }
         /*---(1.00x bars)---------------*/
         else if (x_pos % (s_xinc *  10) == 0) {
            glBegin         (GL_POLYGON); {
               glVertex3f  (x_pos - 1, x_top,     0.0);
               glVertex3f  (x_pos + 1, x_top,     0.0);
               glVertex3f  (x_pos + 1, x_bot,     0.0);
               glVertex3f  (x_pos - 1, x_bot,     0.0);
            } glEnd   ();
            ++x_small;
            sprintf (x_msg, "%d", x_small);
            glPushMatrix(); {
               glTranslatef (x_pos + 10.0, x_bot +  12.0,    10.0  );
               yFONT_print  (my.fixed,  15, YF_BOTLEF, x_msg);
            } glPopMatrix();
         }
         /*---(0.50x bars)---------------*/
         else if (x_pos % (s_xinc *   5) == 0) {
            glBegin         (GL_LINES); {
               glVertex3f  (x_pos, x_top     ,     0.0);
               glVertex3f  (x_pos, x_top - 50,     0.0);
               glVertex3f  (x_pos, x_bot     ,     0.0);
               glVertex3f  (x_pos, x_bot + 20,     0.0);
            } glEnd   ();
         }
         /*---(0.10x bars)---------------*/
         else {
            glBegin         (GL_LINES); {
               glVertex3f  (x_pos, x_top     ,     0.0);
               glVertex3f  (x_pos, x_top - 40,     0.0);
               glVertex3f  (x_pos, x_bot     ,     0.0);
               glVertex3f  (x_pos, x_bot + 10,     0.0);
            } glEnd   ();
         }
         /*---(done)---------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture background ---------------[ ------ [ ------ ]-*/
TICK_back_horz     (void)
{
   /*---(locals)-----------+-----------+-*/
   int       x_line        =    0;          /* loop iterator                  */
   int       x_yinc        =  0.0;
   float     x_ypos        =  0.0;
   float     x_top         =  0.0;
   /*> x_bar  = s_top - s_bot;                                                  <*/
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   yVIKEYS_view_color (YCOLOR_NEG_MUT, 1.00);
   /*---(long bars)----------------------*/
   for (x_line = 0; x_line < my.p_nline; ++x_line) {
      x_top = (x_line + 1) * s_yinc;
      /*---(title bar)-------------------*/
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0   , x_top             ,   10.0);
         glVertex3f  (s_wide, x_top             ,   10.0);
         glVertex3f  (s_wide, x_top - s_title   ,   10.0);
         glVertex3f  (0.0   , x_top - s_title   ,   10.0);
      } glEnd   ();
      /*---(middle bar)------------------*/
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0   , x_top - 110       ,   10.0);
         glVertex3f  (s_wide, x_top - 110       ,   10.0);
         glVertex3f  (s_wide, x_top - 120       ,   10.0);
         glVertex3f  (0.0   , x_top - 120       ,   10.0);
      } glEnd   ();
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_back_label         (void)
{
   /*---(locals)-----------+-----------+-*/
   int       x_line        =    0;
   float     x_top         =  0.0;
   float     x_bot         =  0.0;
   int       x_pos         =    0;
   int       x_ref         =    0;
   char      x_msg         [100];
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   yVIKEYS_view_color (YCOLOR_BAS_ACC, 1.00);
   /*---(vertical lines)-----------------*/
   DEBUG_GRAF   yLOG_value   ("p_nline"   , my.p_nline);
   for (x_line = 0; x_line < my.p_nline; ++x_line) {
      DEBUG_GRAF   yLOG_value   ("x_line"    , x_line);
      x_top = (my.p_nline - x_line    ) * s_yinc;
      x_bot = (my.p_nline - x_line - 1) * s_yinc;
      x_ref = TICK_line_find (x_line);
      DEBUG_GRAF   yLOG_info    ("name"      , s_line_info [x_ref].name);
      /*---(horizontal move)-------------*/
      for (x_pos = 0; x_pos <= s_wide; x_pos += s_xinc * 100) {
         DEBUG_GRAF   yLOG_value   ("x_pos"     , x_pos);
         /*---(line)---------------------*/
         sprintf (x_msg, "%s", s_line_info [x_ref].name);
         glPushMatrix(); {
            glTranslatef (x_pos + 500.0, x_top -  14.0,    60.0);
            yFONT_print  (my.fixed,  18, YF_MIDCEN, x_msg);
         } glPopMatrix();
         /*---(index)--------------------*/
         sprintf (x_msg, "%d", x_ref);
         glPushMatrix(); {
            glTranslatef (x_pos + 250.0, x_top - 14.0,    60.0);
            yFONT_print  (my.fixed,  16, YF_MIDCEN, x_msg);
         } glPopMatrix();
         /*---(scale)--------------------*/
         yVIKEYS_scale_brief (x_msg);
         glPushMatrix(); {
            glTranslatef (x_pos +  15.0, x_top -  14.0,    60.0  );
            yFONT_print  (my.fixed,  14, YF_MIDLEF, x_msg);
         } glPopMatrix();
         /*---(speed)--------------------*/
         yVIKEYS_speed_brief (x_msg);
         glPushMatrix(); {
            glTranslatef (x_pos + 985.0, x_top -  14.0,    60.0  );
            yFONT_print  (my.fixed,  14, YF_MIDRIG, x_msg);
         } glPopMatrix();
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture background ---------------[ ------ [ ------ ]-*/
TICK_back_draw          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_note    ("initializing background (tex, fbo, depth)");
   /*---(free)------------------------*/
   if (p_back->tex != 0) {
      rc = yGLTEX_free (&(p_back->tex), &(p_back->fbo), &(p_back->depth));
      if (rc < 0) {
         DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
   }
   /*---(create)----------------------*/
   rc = yGLTEX_new  (&(p_back->tex), &(p_back->fbo), &(p_back->depth), s_wide, s_tall);
   DEBUG_GRAF   yLOG_value   ("new"       , rc);
   if (rc < 0) {
      DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(draw)---------------------------*/
   DEBUG_GRAF   yLOG_note    ("draw background");
   yGLTEX_draw       (p_back->fbo, YGLTEX_BOTLEF, s_wide, s_tall, 1.0);
   TICK_back_vert    ();
   TICK_back_horz    ();
   TICK_back_label   ();
   yGLTEX_done       (p_back->tex);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_back_copy_label    (tPANEL *a_panel)
{
   /*---(locals)-----------+-----------+-*/
   int       x_line        =    0;
   float     x_top         =  0.0;
   float     x_bot         =  0.0;
   int       x_pos         =    0;
   int       x_ref         =    0;
   char      x_msg         [100];
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(vertical lines)-----------------*/
   DEBUG_GRAF   yLOG_value   ("p_nline"   , my.p_nline);
   for (x_line = 0; x_line < my.p_nline; ++x_line) {
      DEBUG_GRAF   yLOG_value   ("x_line"    , x_line);
      x_top = (my.p_nline - x_line    ) * s_yinc;
      x_bot = (my.p_nline - x_line - 1) * s_yinc;
      x_ref = TICK_line_find (x_line);
      DEBUG_GRAF   yLOG_info    ("name"      , s_line_info [x_ref].name);
      /*---(horizontal move)-------------*/
      for (x_pos = 0; x_pos <= s_wide; x_pos += s_xinc * 100) {
         DEBUG_GRAF   yLOG_value   ("x_pos"     , x_pos);
         /*---(panel)--------------------*/
         yVIKEYS_view_color (YCOLOR_BAS_MED, 1.00);
         sprintf (x_msg, "%d%c/%d (%d)", a_panel->sect, x_pos / (s_xinc * 100) + 'a', s_nsec, a_panel->seq);
         glPushMatrix(); {
            glTranslatef (x_pos + 750.0, x_top - 14.0,    60.0);
            yFONT_print  (my.fixed,  16, YF_MIDCEN, x_msg);
         } glPopMatrix();
         yVIKEYS_view_color (YCOLOR_NEG_MUT, 1.00);
         sprintf (x_msg, "%d", a_panel->sect * 4 + x_pos / (s_xinc * 100));
         glPushMatrix(); {
            glTranslatef (x_pos + 50.0, x_top - 75.0,    60.0);
            yFONT_print  (my.fixed, 32, YF_MIDCEN, x_msg);
         } glPopMatrix();
         /*---(done)---------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture background ---------------[ ------ [ ------ ]-*/
TICK_back_copy          (tPANEL *a_panel)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;          /* generic return code            */
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(copy)---------------------------*/
   glBindTexture   (GL_TEXTURE_2D, p_back->tex);
   glBegin(GL_POLYGON); {
      /*---(top beg)--------*/
      glTexCoord2f (0.0   , 1.0   );
      glVertex3f   (0.0   , s_tall,     0.00f);
      /*---(top end)--------*/
      glTexCoord2f (1.0   , 1.0   );
      glVertex3f   (s_wide, s_tall,     0.00f);
      /*---(bottom end)-----*/
      glTexCoord2f (1.0   , 0.0   );
      glVertex3f   (s_wide, 0.0   ,     0.00f);
      /*---(bottom beg)-----*/
      glTexCoord2f (0.0   , 0.0   );
      glVertex3f   (0.0   , 0.0   ,     0.00f);
      /*---(done)-----------*/
   } glEnd();
   glBindTexture   (GL_TEXTURE_2D, 0);
   TICK_back_copy_label (a_panel);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           major labels                       ----===*/
/*====================------------------------------------====================*/
static void      o___LABELS__________________o (void) {;}

char*
TICK_sectext       (int a_section)
{
   /*---(locals)-----------+-----------+-*/
   int         a           = 0;
   int         b           = 0;
   int         c           = 0;
   int         d           = 0;
   int         x_rem       = 0;
   /*---(parse)--------------------------*/
   /*> printf ("\n");                                                                 <* 
    *> printf ("TICK_sectext\n");                                                     <* 
    *> printf ("s =      ,      ,      , %5d\n", a_section);                          <*/
   if (a_section < 0) {
      sprintf (s_sectext, "%c%c", '-', '-');
   } else {
      x_rem  = a_section;
      a      = trunc (x_rem / (26 * 26 * 26));
      x_rem -= a * 26 * 26 * 26;
      /*> printf ("a = %5d, %5d, %5d, %5d, %5d\n"    , a, a * 26, a * 26 * 26, a * 26 * 26 * 26, x_rem);   <*/
      b      = trunc (x_rem / (26 * 26));
      x_rem -= b * 26 * 26;
      /*> printf ("b = %5d, %5d,      , %5d, %5d\n"  , b, b * 26, b * 26 * 26, x_rem);   <*/
      c      = trunc (x_rem / (26     ));
      x_rem -= c * 26;
      /*> printf ("c = %5d,      ,      , %5d, %5d\n", c, c * 26, x_rem);             <*/
      d      = x_rem % 26;
      x_rem -= d;
      /*> printf ("d = %5d,      ,      , %5d, %5d\n", d, d, x_rem);                  <*/
      if      (a >  0)  sprintf (s_sectext, "%c%c%c%c", a + 'a', b + 'a', c + 'a', d + 'a');
      else if (b >  0)  sprintf (s_sectext, "%c%c%c"  , b + 'a', c + 'a', d + 'a');
      else              sprintf (s_sectext, "%c%c"    , c + 'a', d + 'a');
   }
   /*> printf (">   %s\n", s_sectext);                                                <*/
   /*---(complete)-----------------------*/
   return s_sectext;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_front_label        (int a_panel)
{
   /*---(locals)-----------+-----------+-*/
   int       i;                             /* loop iterator                  */
   int       j;                             /* loop iterator                  */
   float     x_yinc        =  0.0;
   float     x_top         =  0.0;
   float     x_bot         =  0.0;
   float     x_beg         =  0.0;
   float     x_end         = s_wide;
   float     x_pos         =  0.0;
   char      x_msg         [100];
   char      x_part        [100];
   /*> int       x_labelper    =    0;                                                <*/
   /*> int       x_secbeg1     =    0;                                                <*/
   /*> int       x_secbeg2     =    0;                                                <*/
   char      x_label1      [10];
   char      x_label2      [10];
   float     x_sec         = 0.0;
   int       x_lowcnt      = 0;
   char      rc            = 0;

   int       x_line        =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_scale_base (&my.p_multi, &my.p_base);                                  <*/
   /*> x_labelper  = (s_wide / 10.0) * my.p_multi;                                 <*/
   /*> printf ("%8d  %8.3lf\n", s_wide, my.p_multi);                               <*/
   /*> x_secbeg1   = x_labelper * s_section;                                          <*/
   /*> x_secbeg2   = x_secbeg1 + x_labelper;                                          <*/
   strlcpy (x_label1, TICK_sectext (s_section    ), 10);
   strlcpy (x_label2, TICK_sectext (s_section + 1), 10);
   /*> printf ("%8d  %8d  %8d\n", x_labelper, x_secbeg1, x_secbeg2);                  <*/
   /*---(vertical lines)-----------------*/
   yVIKEYS_view_color (YCOLOR_BAS_ACC, 1.00);
   for (x_line = 0; x_line < my.p_nline; ++x_line) {
      x_top = (x_line + 1) * s_yinc;
      x_bot = (x_line    ) * s_yinc;
      /*---(horizontal move)-------------*/
      for (x_pos = 0; x_pos <= s_wide; x_pos += s_xinc * 100) {
      }
   }
   /*---(leg labels)---------------------*/
   for (i = x_beg; i < x_end; i += s_xinc * 100) {
      for (j = 0; j < my.p_nline; ++j) {
         /*---(percent)------------------*/
         /*---(leg)----------------------*/
         x_pos = s_tall - (j * s_yinc);
         sprintf (x_msg, " %d  %s", j % 6, legs_name [j % 6]);
         yVIKEYS_view_color (YCOLOR_BAS_ACC, 1.00);
         glPushMatrix(); {
            glTranslatef ( i + 500.0 , x_pos    -  115.0    ,    60.0  );
            glRotatef  ( 90.0  , 0.0f, 0.0f, 1.0f);
            yFONT_print  (my.fixed,  25, YF_MIDCEN, x_msg);
         } glPopMatrix();
         /*---(hint)---------------------*/
         /*> if (j >= 6)  strlcpy (x_msg, x_label1, LEN_STR);                         <* 
          *> else         strlcpy (x_msg, x_label2, LEN_STR);                         <* 
          *> glColor4f    (0.25f, 0.25f, 0.25f, 1.0f);                                <* 
          *> glPushMatrix(); {                                                        <* 
          *>    glTranslatef ( i +  10.0 , x_pos    -  150.0    ,    60.0  );         <* 
          *>    yFONT_print  (my.fixed,  18, YF_TOPLEF, x_msg);                       <* 
          *> } glPopMatrix();                                                         <*/
         /*---(done)---------------------*/
      }
   }
   /*---(scale labels)-------------------*/
   /*> glPushMatrix(); {                                                                                                                             <* 
    *>    glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                                                                                  <* 
    *>    for (i = 0; i < s_wide; i += s_xinc) {                                                                                                     <* 
    *>       /+---(labels)-------------------+/                                                                                                      <* 
    *>       /+> if (i % 100 == 0) {                                                                                               <*                <* 
    *>        *>    for (j = 0; j < 12; ++j) {                                                                                     <*                <* 
    *>        *>       x_pos = (j + 1) * s_yinc;                                                                                   <*                <* 
    *>        *>       /+> glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                          <+/                                  <*                <* 
    *>        *>       if (j < 6)  snprintf     (x_msg, 50, "%d%c"  , (int) ((i / s_xinc) * my.p_multi) + x_secbeg1, my.p_base);   <*                <* 
    *>        *>       else        snprintf     (x_msg, 50, "%d%c"  , (int) ((i / s_xinc) * my.p_multi) + x_secbeg2, my.p_base);   <*                <* 
    *>        *>       glPushMatrix(); {                                                                                           <*                <* 
    *>        *>          glTranslatef ( i , x_pos + 25.0 ,    60.0  );                                                            <*                <* 
    *>        *>          yFONT_print  (my.fixed,  14, YF_TOPLEF, x_msg);                                                          <*                <* 
    *>        *>       } glPopMatrix();                                                                                            <*                <* 
    *>        *>    }                                                                                                              <*                <* 
    *>        *> }                                                                                                                 <+/               <* 
    *>       /+---(accuracy heatmap)---------+/                                                                                                      <* 
    *>       /+> for (j = 0; j < 12; ++j) {                                                                                                    <*    <* 
    *>        *>    if (j < 6) {                                                                                                               <*    <* 
    *>        *>       x_pos = (6 - j) * s_yinc;                                                                                               <*    <* 
    *>        *>       x_sec = (((float) (i) / s_xinc) * my.p_multi) + x_secbeg1;                                                              <*    <* 
    *>        *>       if (j == 0) {                                                                                                           <*    <* 
    *>        *>          rc = yKINE_phys_flat   (YKINE_FK, x_sec, &s_lowest, &x_lowcnt);                                                      <*    <* 
    *>        *>          /+> printf ("i=%5d, j=%2d, x_sec=%8.3lf, s_lowest=%8.1lf, x_lowcnt=%2d\n", i, j, x_sec, s_lowest, x_lowcnt);   <+/   <*    <* 
    *>        *>       }                                                                                                                       <*    <* 
    *>        *>       /+> TICK_height   (i, x_pos);                                          <+/                                              <*    <* 
    *>        *>       if (x_sec >= 0.0 && x_sec <= my.p_len)  TICK__heat_column (j    , x_sec, i, x_pos - 35.0);                              <*    <* 
    *>        *>    } else {                                                                                                                   <*    <* 
    *>        *>       x_pos = (12 + (6 - j)) * s_yinc;                                                                                        <*    <* 
    *>        *>       x_sec = (((float) (i) / s_xinc) * my.p_multi) + x_secbeg2;                                                              <*    <* 
    *>        *>       if (j == 6) {                                                                                                           <*    <* 
    *>        *>          yKINE_phys_flat   (YKINE_FK, x_sec, &s_lowest, &x_lowcnt);                                                           <*    <* 
    *>        *>          /+> printf ("i=%5d, j=%2d, x_sec=%8.3lf, s_lowest=%8.1lf, x_lowcnt=%2d\n", i, j, x_sec, s_lowest, x_lowcnt);   <+/   <*    <* 
    *>        *>       }                                                                                                                       <*    <* 
    *>        *>       /+> TICK_height   (i, x_pos);                                          <+/                                              <*    <* 
    *>        *>       if (x_sec >= 0.0 && x_sec <= my.p_len)  TICK__heat_column (j - 6, x_sec, i, x_pos - 35.0);                              <*    <* 
    *>        *>    }                                                                                                                          <*    <* 
    *>        *> }                                                                                                                             <+/   <* 
    *>    }                                                                                                                                          <* 
    *> } glPopMatrix();                                                                                                                              <*/
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     new drawing routines                     ----===*/
/*====================------------------------------------====================*/
static void      o___NEW_____________________o (void) {;}

char
tick_panel_wipe         (tPANEL *a_panel, char a_init)
{
   int         i           =    0;
   int         j           =    0;
   for (i = 0; i < MAX_WIDE; ++i) {
      for (j = 0; j < YKINE_MAX_LEGS; ++j) {
         /*---(special)------------------------*/
         a_panel->detail [j][i].used        = '-';
         a_panel->detail [j][i].exact       = '-';
         if (a_init != 'y' && a_panel->detail [j][i].label != NULL) {
            free (a_panel->detail [j][i].label);
         }
         a_panel->detail [j][i].label       = NULL;
         a_panel->detail [j][i].cell        = '-';
         /*---(planned angles)-----------------*/
         a_panel->detail [j][i].o_femu      = 0.0;
         a_panel->detail [j][i].o_pate      = 0.0;
         a_panel->detail [j][i].o_tibi      = 0.0;
         /*---(planned endpoint)---------------*/
         a_panel->detail [j][i].o_xpos      = 0.0;
         a_panel->detail [j][i].o_zpos      = 0.0;
         a_panel->detail [j][i].o_ypos      = 0.0;
         /*---(target endpoint)----------------*/
         a_panel->detail [j][i].t_xpos      = 0.0;
         a_panel->detail [j][i].t_zpos      = 0.0;
         a_panel->detail [j][i].t_ypos      = 0.0;
         /*---(revised angles)-----------------*/
         a_panel->detail [j][i].r_femu      = 0.0;
         a_panel->detail [j][i].r_pate      = 0.0;
         a_panel->detail [j][i].r_tibi      = 0.0;
         /*---(revised endpoint)---------------*/
         a_panel->detail [j][i].r_xpos      = 0.0;
         a_panel->detail [j][i].r_zpos      = 0.0;
         a_panel->detail [j][i].r_ypos      = 0.0;
         /*---(revised success)----------------*/
         a_panel->detail [j][i].r_rc        =   0;
         a_panel->detail [j][i].r_error     = 0.0;
         /*---(opengl endpoint)----------------*/
         a_panel->detail [j][i].g_xpos      = 0.0;
         a_panel->detail [j][i].g_zpos      = 0.0;
         a_panel->detail [j][i].g_ypos      = 0.0;
         a_panel->detail [j][i].g_error     = 0.0;
         /*---(done)---------------------------*/
      }
   }
   return 0;
}

/*> char         /+--> set values for progress ticker --------[ ------ [ ------ ]-+/                                                                                    <* 
 *> tick_panel_init         (void)                                                                                                                                      <* 
 *> {                                                                                                                                                                   <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                                                         <* 
 *>    char        rc          =    0;                                                                                                                                  <* 
 *>    int         i           =    0;                                                                                                                                  <* 
 *>    /+---(header)-------------------------+/                                                                                                                         <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                                                                                                        <* 
 *>    /+---(prepare)------------------------+/                                                                                                                         <* 
 *>    rc = yGLTEX_config ();                                                                                                                                           <* 
 *>    DEBUG_GRAF   yLOG_value   ("yGLTEX"    , rc);                                                                                                                    <* 
 *>    if (rc < 0)  return rc;                                                                                                                                          <* 
 *>    /+> rc = yVIKEYS_cmds_add ('c', "p_snap"      , ""    , ""     , TICK_snap            , "save png image of full ticker/progress"                      );   <*    <* 
 *>     *> DEBUG_GRAF   yLOG_value   ("p_snap"    , rc);                                                                                                          <*    <* 
 *>     *> if (rc < 0)  return rc;                                                                                                                                <+/   <* 
 *>    /+---(sizes)--------------------------+/                                                                                                                         <* 
 *>    DEBUG_GRAF   yLOG_note    ("setting sizes (widths and heights)");                                                                                                <* 
 *>    TICK_line_count ();                                                                                                                                              <* 
 *>    s_tall = s_yinc;                                                                                                                                                 <* 
 *>    s_wide = MAX_WIDE;                                                                                                                                               <* 
 *>    DEBUG_GRAF   yLOG_value   ("s_tall"    , s_tall);                                                                                                                <* 
 *>    /+---(handles)------------------------+/                                                                                                                         <* 
 *>    DEBUG_GRAF   yLOG_note    ("initializing handles (tex, fbo, depth)");                                                                                            <* 
 *>    for (i = 0; i < MAX_PANEL; ++i) {                                                                                                                                <* 
 *>       s_ticker [i].tex = s_ticker [i].fbo = s_ticker [i].depth = 0;                                                                                                 <* 
 *>    }                                                                                                                                                                <* 
 *>    /+---(secs per panel)-----------------+/                                                                                                                         <* 
 *>    rc  = yVIKEYS_prog_cur (&s_anchor, &my.p_cur, &my.p_scale, &my.p_inc, &my.p_line);                                                                               <* 
 *>    DEBUG_GRAF   yLOG_double  ("p_cur"     , my.p_cur);                                                                                                              <* 
 *>    DEBUG_GRAF   yLOG_double  ("p_scale"   , my.p_scale);                                                                                                            <* 
 *>    s_len       = my.p_scale * UNIT2PANEL;                                                                                                                           <* 
 *>    DEBUG_GRAF   yLOG_double  ("s_len"     , s_len);                                                                                                                 <* 
 *>    /+---(pointers)-----------------------+/                                                                                                                         <* 
 *>    for (i = 0; i < MAX_PANEL; ++i) {                                                                                                                                <* 
 *>       s_order  [i] = &(s_ticker [i]);                                                                                                                               <* 
 *>       s_ticker [i].seq  = i - 2;                                                                                                                                    <* 
 *>       s_ticker [i].beg  = (i - 2) * s_len;                                                                                                                          <* 
 *>       s_ticker [i].sect = i - 2;                                                                                                                                    <* 
 *>    }                                                                                                                                                                <* 
 *>    /+---(complete)-----------------------+/                                                                                                                         <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                                                                                                        <* 
 *>    return 0;                                                                                                                                                        <* 
 *> }                                                                                                                                                                   <*/

/*> char                                                                                      <* 
 *> tick_panel__start       (void)                                                            <* 
 *> {                                                                                         <* 
 *>    char        rc          =    0;                                                        <* 
 *>    /+---(prepare)------------------------+/                                               <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                              <* 
 *>    /+---(free)------------------------+/                                                  <* 
 *>    if (p_back->tex != 0) {                                                                <* 
 *>       rc = yGLTEX_free (&(p_back->tex), &(p_back->fbo), &(p_back->depth));                <* 
 *>       if (rc < 0) {                                                                       <* 
 *>          DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);                                    <* 
 *>          return rc;                                                                       <* 
 *>       }                                                                                   <* 
 *>    }                                                                                      <* 
 *>    /+---(create)----------------------+/                                                  <* 
 *>    rc = yGLTEX_new  (&(p_back->tex), &(p_back->fbo), &(p_back->depth), s_wide, s_tall);   <* 
 *>    DEBUG_GRAF   yLOG_value   ("new"       , rc);                                          <* 
 *>    if (rc < 0) {                                                                          <* 
 *>       DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);                                       <* 
 *>       return rc;                                                                          <* 
 *>    }                                                                                      <* 
 *>    /+---(draw)---------------------------+/                                               <* 
 *>    DEBUG_GRAF   yLOG_note    ("draw background");                                         <* 
 *>    yGLTEX_draw       (p_back->fbo, YGLTEX_BOTLEF, s_wide, s_tall, 1.0);                   <* 
 *>    /+---(complete)-----------------------+/                                               <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                              <* 
 *>    return 0;                                                                              <* 
 *> }                                                                                         <*/

/*> char                                                                              <* 
 *> tick_panel__back        (int a_pos)                                               <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----------+-+/                                       <* 
 *>    float     x_big         =  0.0;                                                <* 
 *>    float     x_sml         =  0.0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(prepare)------------------------+/                                       <* 
 *>    yVIKEYS_view_color (YCOLOR_NEG_MUT, 1.00);                                     <* 
 *>    glLineWidth (2.0);                                                             <* 
 *>    /+---(locators)-----------------+/                                             <* 
 *>    x_big  = s_xinc / 3.0;                                                         <* 
 *>    x_sml  = 1.0;                                                                  <* 
 *>    /+---(title bar)-------------------+/                                          <* 
 *>    glBegin (GL_POLYGON); {                                                        <* 
 *>       glVertex3f (a_pos         , s_yinc             ,   10.0);                   <* 
 *>       glVertex3f (a_pos + s_xinc, s_yinc             ,   10.0);                   <* 
 *>       glVertex3f (a_pos + s_xinc, s_yinc - s_title   ,   10.0);                   <* 
 *>       glVertex3f (a_pos         , s_yinc - s_title   ,   10.0);                   <* 
 *>    } glEnd ();                                                                    <* 
 *>    /+---(middle bar)------------------+/                                          <* 
 *>    glBegin (GL_POLYGON); {                                                        <* 
 *>       glVertex3f (a_pos         , s_yinc - 110       ,   10.0);                   <* 
 *>       glVertex3f (a_pos + s_xinc, s_yinc - 110       ,   10.0);                   <* 
 *>       glVertex3f (a_pos + s_xinc, s_yinc - 120       ,   10.0);                   <* 
 *>       glVertex3f (a_pos         , s_yinc - 120       ,   10.0);                   <* 
 *>    } glEnd ();                                                                    <* 
 *>    /+---(10.0x bars)---------------+/                                             <* 
 *>    if (a_pos % (s_xinc * 100) == 0) {                                             <* 
 *>       glBegin (GL_POLYGON); {                                                     <* 
 *>          glVertex3f (a_pos - x_big, s_yinc, 0.0);                                 <* 
 *>          glVertex3f (a_pos + x_big, s_yinc, 0.0);                                 <* 
 *>          glVertex3f (a_pos + x_big, 0.0   , 0.0);                                 <* 
 *>          glVertex3f (a_pos - x_big, 0.0   , 0.0);                                 <* 
 *>       } glEnd ();                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(1.00x bars)---------------+/                                             <* 
 *>    else if (a_pos % (s_xinc *  10) == 0) {                                        <* 
 *>       glBegin (GL_POLYGON); {                                                     <* 
 *>          glVertex3f (a_pos - x_sml, s_yinc, 0.0);                                 <* 
 *>          glVertex3f (a_pos + x_sml, s_yinc, 0.0);                                 <* 
 *>          glVertex3f (a_pos + x_sml, 0.0   , 0.0);                                 <* 
 *>          glVertex3f (a_pos - x_sml, 0.0   , 0.0);                                 <* 
 *>       } glEnd ();                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(0.50x bars)---------------+/                                             <* 
 *>    else if (a_pos % (s_xinc *   5) == 0) {                                        <* 
 *>       glBegin (GL_LINES); {                                                       <* 
 *>          glVertex3f (a_pos, s_yinc       , 0.0);                                  <* 
 *>          glVertex3f (a_pos, s_yinc - 50.0, 0.0);                                  <* 
 *>          glVertex3f (a_pos, 0.0          , 0.0);                                  <* 
 *>          glVertex3f (a_pos, 20.0         , 0.0);                                  <* 
 *>       } glEnd ();                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(0.10x bars)---------------+/                                             <* 
 *>    else {                                                                         <* 
 *>       glBegin (GL_LINES); {                                                       <* 
 *>          glVertex3f (a_pos, s_yinc       , 0.0);                                  <* 
 *>          glVertex3f (a_pos, s_yinc - 40.0, 0.0);                                  <* 
 *>          glVertex3f (a_pos, 0.0          , 0.0);                                  <* 
 *>          glVertex3f (a_pos, 10.0         , 0.0);                                  <* 
 *>       } glEnd ();                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                                                         <* 
 *> tick_panel__line   (int a_pos, int a_line, char a_type, float a_base, float a_scale, int a_off, float a_z)   <* 
 *> {                                                                                                            <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                  <* 
 *>    int       i             =    0;                                                                           <* 
 *>    float     y_beg         =  0.0;                                                                           <* 
 *>    float     y_end         =  0.0;                                                                           <* 
 *>    switch (a_type) {                                                                                         <* 
 *>    case 'F' :  /+ femur original    +/                                                                       <* 
 *>    case 'X' :  /+ xpos original     +/                                                                       <* 
 *>       glColor4f    (0.70f, 0.00f, 0.00f, 1.0f);                                                              <* 
 *>       glLineWidth  (15.0f);                                                                                  <* 
 *>       break;                                                                                                 <* 
 *>    case 'f' :  /+ femur adapted     +/                                                                       <* 
 *>       glColor4f    (0.50f, 0.00f, 0.00f, 1.0f);                                                              <* 
 *>       glPointSize  ( 5.0f);                                                                                  <* 
 *>       break;                                                                                                 <* 
 *>    case 'P' :  /+ patella original  +/                                                                       <* 
 *>    case 'Z' :  /+ zpos original     +/                                                                       <* 
 *>       glColor4f    (0.20f, 0.20f, 0.80f, 1.0f);                                                              <* 
 *>       glLineWidth  (10.0f);                                                                                  <* 
 *>       break;                                                                                                 <* 
 *>    case 'p' :  /+ patella adapted   +/                                                                       <* 
 *>       glColor4f    (0.10f, 0.10f, 0.50f, 1.0f);                                                              <* 
 *>       glPointSize  ( 5.0f);                                                                                  <* 
 *>       break;                                                                                                 <* 
 *>    case 'T' :  /+ tibia original    +/                                                                       <* 
 *>    case 'Y' :  /+ ypos original     +/                                                                       <* 
 *>       glColor4f    (0.60f, 0.60f, 0.00f, 1.0f);                                                              <* 
 *>       glLineWidth  ( 4.0f);                                                                                  <* 
 *>       break;                                                                                                 <* 
 *>    case 't' :  /+ tibia adapted     +/                                                                       <* 
 *>       glColor4f    (0.40f, 0.40f, 0.00f, 1.0f);                                                              <* 
 *>       glPointSize  ( 5.0f);                                                                                  <* 
 *>       break;                                                                                                 <* 
 *>    }                                                                                                         <* 
 *>    /+---(lines)-------------------------+/                                                                   <* 
 *>    if (strchr ("FPTXZY", a_type) != NULL) {                                                                  <* 
 *>       switch (a_type) {                                                                                      <* 
 *>       case 'F' :                                                                                             <* 
 *>          y_beg = p_actv->detail [a_line][a_pos + 0].o_femu;                                                  <* 
 *>          y_end = p_actv->detail [a_line][a_pos + 1].o_femu;                                                  <* 
 *>          glColor4f    (0.70f, 0.00f, 0.00f, 1.0f);                                                           <* 
 *>          glLineWidth  (15.0f);                                                                               <* 
 *>          break;                                                                                              <* 
 *>       case 'P' :                                                                                             <* 
 *>          y_beg = p_actv->detail [a_line][a_pos + 0].o_pate;                                                  <* 
 *>          y_end = p_actv->detail [a_line][a_pos + 1].o_pate;                                                  <* 
 *>          glColor4f    (0.20f, 0.20f, 0.80f, 1.0f);                                                           <* 
 *>          glLineWidth  (10.0f);                                                                               <* 
 *>          break;                                                                                              <* 
 *>       case 'T' :                                                                                             <* 
 *>          y_beg = p_actv->detail [a_line][a_pos + 0].o_tibi;                                                  <* 
 *>          y_end = p_actv->detail [a_line][a_pos + 1].o_tibi;                                                  <* 
 *>          glColor4f    (0.60f, 0.60f, 0.00f, 1.0f);                                                           <* 
 *>          glLineWidth  ( 4.0f);                                                                               <* 
 *>          break;                                                                                              <* 
 *>       case 'X' :                                                                                             <* 
 *>          y_beg = p_actv->detail [a_line][a_pos + 0].o_xpos;                                                  <* 
 *>          y_end = p_actv->detail [a_line][a_pos + 1].o_xpos;                                                  <* 
 *>          glColor4f    (0.70f, 0.00f, 0.00f, 1.0f);                                                           <* 
 *>          glLineWidth  (15.0f);                                                                               <* 
 *>          break;                                                                                              <* 
 *>       case 'Z' :                                                                                             <* 
 *>          y_beg = p_actv->detail [a_line][a_pos + 0].o_zpos;                                                  <* 
 *>          y_end = p_actv->detail [a_line][a_pos + 1].o_zpos;                                                  <* 
 *>          glColor4f    (0.20f, 0.20f, 0.80f, 1.0f);                                                           <* 
 *>          glLineWidth  (10.0f);                                                                               <* 
 *>          break;                                                                                              <* 
 *>       case 'Y' :                                                                                             <* 
 *>          y_beg = p_actv->detail [a_line][a_pos + 0].o_ypos;                                                  <* 
 *>          y_end = p_actv->detail [a_line][a_pos + 1].o_ypos;                                                  <* 
 *>          glColor4f    (0.60f, 0.60f, 0.00f, 1.0f);                                                           <* 
 *>          glLineWidth  ( 4.0f);                                                                               <* 
 *>          break;                                                                                              <* 
 *>       }                                                                                                      <* 
 *>       glBegin (GL_LINES); {                                                                                  <* 
 *>          glVertex3f  ((a_pos + 0) * 10, a_base + (y_beg * a_scale + a_off), a_z);                            <* 
 *>          glVertex3f  ((a_pos + 1) * 10, a_base + (y_end * a_scale + a_off), a_z);                            <* 
 *>       } glEnd ();                                                                                            <* 
 *>    }                                                                                                         <* 
 *>    /+---(points)------------------------+/                                                                   <* 
 *>    else {                                                                                                    <* 
 *>       glBegin (GL_POINTS); {                                                                                 <* 
 *>          switch (a_type) {                                                                                   <* 
 *>          case 'f' :                                                                                          <* 
 *>             y_beg = p_actv->detail [a_line][a_pos].r_femu;                                                   <* 
 *>             glColor4f    (0.50f, 0.00f, 0.00f, 1.0f);                                                        <* 
 *>             glPointSize  ( 5.0f);                                                                            <* 
 *>             break;                                                                                           <* 
 *>          case 'p' :                                                                                          <* 
 *>             y_beg = p_actv->detail [a_line][a_pos].r_pate;                                                   <* 
 *>             glColor4f    (0.10f, 0.10f, 0.50f, 1.0f);                                                        <* 
 *>             glPointSize  ( 5.0f);                                                                            <* 
 *>             break;                                                                                           <* 
 *>          case 't' :                                                                                          <* 
 *>             y_beg = p_actv->detail [a_line][a_pos].r_tibi;                                                   <* 
 *>             glColor4f    (0.40f, 0.40f, 0.00f, 1.0f);                                                        <* 
 *>             glPointSize  ( 5.0f);                                                                            <* 
 *>             break;                                                                                           <* 
 *>             /+> case 'x' : y_beg = p_actv->detail [a_line][a_pos].r_xpos;   break;       <*                  <* 
 *>              *> case 'z' : y_beg = p_actv->detail [a_line][a_pos].r_zpos;   break;       <*                  <* 
 *>              *> case 'y' : y_beg = p_actv->detail [a_line][a_pos].r_ypos;   break;       <+/                 <* 
 *>          }                                                                                                   <* 
 *>          glVertex3f  (a_pos * 10, a_base + (y_beg * a_scale + a_off), a_z);                                  <* 
 *>       } glEnd ();                                                                                            <* 
 *>    }                                                                                                         <* 
 *>    /+---(complete)----------------------+/                                                                   <* 
 *>    return 0;                                                                                                 <* 
 *> }                                                                                                            <*/

/*> char         /+--> draw texture labels -------------------[ ------ [ ------ ]-+/                         <* 
 *> tick_panel__pos    (int a_pos, int a_line, float a_scale)                                                <* 
 *> {                                                                                                        <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                              <* 
 *>    float     x_base        =  0.0;                                                                       <* 
 *>    /+---(prepare)------------------------+/                                                              <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                                             <* 
 *>    x_base    = (s_tall - (s_yinc * a_line)) - s_top;                                                     <* 
 *>    /+---(x-pos)--------------------------+/                                                              <* 
 *>    glPushMatrix(); {                                                                                     <* 
 *>       tick_panel__line (a_pos, a_line, 'X', x_base, a_scale,  3, 230);                                   <* 
 *>       tick_panel__line (a_pos, a_line, 'X', x_base, a_scale, -3, 230);                                   <* 
 *>    } glPopMatrix();                                                                                      <* 
 *>    /+---(z-pos)--------------------------+/                                                              <* 
 *>    glPushMatrix(); {                                                                                     <* 
 *>       tick_panel__line (a_pos, a_line, 'Z', x_base, a_scale,  1, 240);                                   <* 
 *>       tick_panel__line (a_pos, a_line, 'Z', x_base, a_scale, -1, 240);                                   <* 
 *>    } glPopMatrix();                                                                                      <* 
 *>    /+---(y-pos)--------------------------+/                                                              <* 
 *>    glPushMatrix(); {                                                                                     <* 
 *>       tick_panel__line (a_pos, a_line, 'Y', x_base, a_scale,  0, 250);                                   <* 
 *>    } glPopMatrix();                                                                                      <* 
 *>    /+---(draw end)-----------------------+/                                                              <* 
 *>    /+> glPushMatrix(); {                                                                           <*    <* 
 *>     *>    TICK_servoline  (a_pos, 'e', x_base, my.p_len, my.p_len, s_top - 30.0, s_bot, x_unit);   <*    <* 
 *>     *> } glPopMatrix();                                                                            <+/   <* 
 *>    /+---(complete)-----------------------+/                                                              <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                                             <* 
 *>    return 0;                                                                                             <* 
 *> }                                                                                                        <*/

/*> char         /+--> draw texture labels -------------------[ ------ [ ------ ]-+/                                                                                                                                                                                                                                                                                                                               <* 
 *> tick_panel__deg    (int a_pos, int a_line, float a_scale)                                                                                                                                                                                                                                                                                                                                                      <* 
 *> {                                                                                                                                                                                                                                                                                                                                                                                                              <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                                                                                                                                                                                                                                                                                                    <* 
 *>    float     x_base        =  0.0;                                                                                                                                                                                                                                                                                                                                                                             <* 
 *>    /+---(prepare)------------------------+/                                                                                                                                                                                                                                                                                                                                                                    <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                                                                                                                                                                                                                                                                                                                                                   <* 
 *>    x_base    = (s_tall - (s_yinc * a_line)) - s_top;                                                                                                                                                                                                                                                                                                                                                           <* 
 *>    /+---(femur)--------------------------+/                                                                                                                                                                                                                                                                                                                                                                    <* 
 *>    glPushMatrix(); {                                                                                                                                                                                                                                                                                                                                                                                           <* 
 *>       tick_panel__line (a_pos, a_line, 'F', x_base, a_scale,  3, 230);                                                                                                                                                                                                                                                                                                                                         <* 
 *>       tick_panel__line (a_pos, a_line, 'F', x_base, a_scale, -3, 230);                                                                                                                                                                                                                                                                                                                                         <* 
 *>       tick_panel__line (a_pos, a_line, 'f', x_base, a_scale,  0, 330);                                                                                                                                                                                                                                                                                                                                         <* 
 *>    } glPopMatrix();                                                                                                                                                                                                                                                                                                                                                                                            <* 
 *>    /+---(patella)------------------------+/                                                                                                                                                                                                                                                                                                                                                                    <* 
 *>    glPushMatrix(); {                                                                                                                                                                                                                                                                                                                                                                                           <* 
 *>       tick_panel__line (a_pos, a_line, 'P', x_base, a_scale,  1, 240);                                                                                                                                                                                                                                                                                                                                         <* 
 *>       tick_panel__line (a_pos, a_line, 'P', x_base, a_scale, -1, 240);                                                                                                                                                                                                                                                                                                                                         <* 
 *>       tick_panel__line (a_pos, a_line, 'p', x_base, a_scale,  0, 340);                                                                                                                                                                                                                                                                                                                                         <* 
 *>    } glPopMatrix();                                                                                                                                                                                                                                                                                                                                                                                            <* 
 *>    /+---(tibia)--------------------------+/                                                                                                                                                                                                                                                                                                                                                                    <* 
 *>    glPushMatrix(); {                                                                                                                                                                                                                                                                                                                                                                                           <* 
 *>       tick_panel__line (a_pos, a_line, 'T', x_base, a_scale,  0, 250);                                                                                                                                                                                                                                                                                                                                         <* 
 *>       tick_panel__line (a_pos, a_line, 't', x_base, a_scale,  0, 350);                                                                                                                                                                                                                                                                                                                                         <* 
 *>       /+> printf ("%d  %3d  :: %2d   %8.1ff %8.1fp %8.1ft   %8.1fx %8.1fz %8.1fy    %8.1fx %8.1fy \n", p_actv->sect, i, a_line, p_actv->detail [a_line][i].o_femu, p_actv->detail [a_line][i].o_pate, p_actv->detail [a_line][i].o_tibi, p_actv->detail [a_line][i].o_xpos, p_actv->detail [a_line][i].o_zpos, p_actv->detail [a_line][i].o_ypos, i * 10, x_base + p_actv->detail [a_line][i].o_tibi);   <+/   <* 
 *>    } glPopMatrix();                                                                                                                                                                                                                                                                                                                                                                                            <* 
 *>    /+---(draw end)-----------------------+/                                                                                                                                                                                                                                                                                                                                                                    <* 
 *>    /+> glPushMatrix(); {                                                                               <*                                                                                                                                                                                                                                                                                                      <* 
 *>     *>    TICK_servoline     (p_actv, 'e', x_base, my.p_len, my.p_len, s_top - 30.0, s_bot, x_unit);   <*                                                                                                                                                                                                                                                                                                      <* 
 *>     *> } glPopMatrix();                                                                                <+/                                                                                                                                                                                                                                                                                                     <* 
 *>    /+---(complete)-----------------------+/                                                                                                                                                                                                                                                                                                                                                                    <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                                                                                                                                                                                                                                                                                                                                                   <* 
 *>    return 0;                                                                                                                                                                                                                                                                                                                                                                                                   <* 
 *> }                                                                                                                                                                                                                                                                                                                                                                                                              <*/

/*> char         /+--> draw texture for progress ticker ------[ ------ [ ------ ]-+/     <* 
 *> tick_panel__one         (int a_pos)                                                  <* 
 *> {                                                                                    <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                          <* 
 *>    char        rc          =    0;                                                   <* 
 *>    int         i;                             /+ loop iterator                  +/   <* 
 *>    int         x, y;                                                                 <* 
 *>    int         x_ref       =    0;                                                   <* 
 *>    /+---(prepare)------------------------+/                                          <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                         <* 
 *>    DEBUG_GRAF   yLOG_value   ("a_pos"     , a_pos);                                  <* 
 *>    /+---(draw)------------------------+/                                             <* 
 *>    DEBUG_GRAF   yLOG_value   ("p_line"    , my.p_line);                              <* 
 *>    DEBUG_GRAF   yLOG_char    ("content"   , s_line_info [my.p_line].content);        <* 
 *>    tick_panel__back   (a_pos);                                                       <* 
 *>    if (s_line_info [my.p_line].active == 'x') {                                      <* 
 *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                      <* 
 *>       return 0;                                                                      <* 
 *>    }                                                                                 <* 
 *>    switch (s_line_info [my.p_line].content) {                                        <* 
 *>    case 'z' : tick_panel__pos  (a_pos, my.p_line, 1.00);   break;                    <* 
 *>    case 'o' : tick_panel__deg  (a_pos, my.p_line, 2.00);   break;                    <* 
 *>    case 'l' : tick_panel__deg  (a_pos, my.p_line, 1.00);   break;                    <* 
 *>    }                                                                                 <* 
 *>    /+---(complete)-----------------------+/                                          <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                         <* 
 *>    return 0;                                                                         <* 
 *> }                                                                                    <*/



/*====================------------------------------------====================*/
/*===----                      setup and teardown                      ----===*/
/*====================------------------------------------====================*/
static void      o___GLOBAL__________________o (void) {;}

char         /*--> set values for progress ticker --------[ ------ [ ------ ]-*/
TICK_init          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yGLTEX_config ();
   DEBUG_GRAF   yLOG_value   ("yGLTEX"    , rc);
   if (rc < 0)  return rc;
   rc = yVIKEYS_cmds_add ('c', "p_snap"      , ""    , ""     , TICK_snap            , "save png image of full ticker/progress"                      );
   DEBUG_GRAF   yLOG_value   ("p_snap"    , rc);
   if (rc < 0)  return rc;
   /*---(sizes)--------------------------*/
   DEBUG_GRAF   yLOG_note    ("setting sizes (widths and heights)");
   TICK_line_count ();
   s_tall = my.p_nline * s_yinc;
   DEBUG_GRAF   yLOG_value   ("s_tall"    , s_tall);
   /*---(handles)------------------------*/
   DEBUG_GRAF   yLOG_note    ("initializing handles (tex, fbo, depth)");
   for (i = 0; i < MAX_PANEL; ++i) {
      s_panel [i].tex = s_panel [i].fbo = s_panel [i].depth = 0;
   }
   /*---(secs per panel)-----------------*/
   rc  = yVIKEYS_prog_cur (&s_anchor, &my.p_cur, &my.p_scale, &my.p_inc, &my.p_line);
   DEBUG_GRAF   yLOG_double  ("p_cur"     , my.p_cur);
   DEBUG_GRAF   yLOG_double  ("p_scale"   , my.p_scale);
   s_len       = my.p_scale * UNIT2PANEL;
   DEBUG_GRAF   yLOG_double  ("s_len"     , s_len);
   /*---(back)---------------------------*/
   p_back = &(s_panel [0]);
   p_back->seq   =    0;
   p_back->beg   =  0.0;
   p_back->sect  =    0;
   p_back->act   =  '-';
   tick_panel_wipe (p_back, 'y');
   /*---(prefix panel)-------------------*/
   p_pref = &(s_panel [1]);
   p_pref->seq   =    1;
   p_pref->beg   =  0.0 - s_len;
   p_pref->sect  =   -1;
   p_pref->act   =  '-';
   tick_panel_wipe (p_pref, 'y');
   /*---(main panel)---------------------*/
   p_curr = &(s_panel [2]);
   p_curr->seq   =    2;
   p_curr->beg   =  0.0;
   p_curr->sect  =    0;
   p_curr->act   =  '-';
   tick_panel_wipe (p_curr, 'y');
   /*---(suffix panel)-------------------*/
   p_suff = &(s_panel [3]);
   p_suff->seq   =    3;
   p_suff->beg   =  0.0 + s_len;
   p_suff->sect  =    1;
   p_suff->act   =  '-';
   tick_panel_wipe (p_suff, 'y');
   /*---(drawing position)---------------*/
   p_draw        = p_curr;
   my.p_tick     =    0;
   /*---(working)------------------------*/
   DEBUG_GRAF   yLOG_note    ("initializing working variables");
   /*---(update colors)------------------*/
   /*> s_ncolor = yCOLOR_init     (YCOLOR_BROMAU2);                                   <* 
    *> yCOLOR_scale    (YCOLOR_PARABOLIC,  0.0,  30.0);                               <*/
   /*---(draw template)------------------*/
   TICK_back_draw ();
   TICK_draw_all  ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> set values for progress ticker --------[ ------ [ ------ ]-*/
TICK_wrap          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(handles)------------------------*/
   DEBUG_GRAF   yLOG_note    ("destroying handles (tex, fbo, depth)");
   for (i = 0; i < MAX_PANEL; ++i) {
      rc = yGLTEX_free (&(s_panel [i].tex), &(s_panel [i].fbo), &(s_panel [i].depth));
      DEBUG_GRAF   yLOG_value   ("rc"        , rc);
      if (rc < 0) {
         DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         servo lines                          ----===*/
/*====================------------------------------------====================*/
static void      o___SERVOS__________________o (void) {;}

/*> char                                                                                                         <* 
 *> TICK_line          (float a_x1, float a_y1, float a_x2, float a_y2, float a_xinc, float a_yinc, float a_z)   <* 
 *> {                                                                                                            <* 
 *>    DEBUG_GRAF   yLOG_snote   (__FUNCTION__);                                                                 <* 
 *>    /+---(top line)-----------------------+/                                                                  <* 
 *>    glBegin(GL_LINE_STRIP); {                                                                                 <* 
 *>       glVertex3f  (a_x1 - a_xinc, a_y1 + a_yinc, a_z);                                                       <* 
 *>       glVertex3f  (a_x2 - a_xinc, a_y2 + a_yinc, a_z);                                                       <* 
 *>    } glEnd   ();                                                                                             <* 
 *>    /+---(bottom line)--------------------+/                                                                  <* 
 *>    glBegin(GL_LINE_STRIP); {                                                                                 <* 
 *>       glVertex3f  (a_x1 + a_xinc, a_y1 - a_yinc, a_z);                                                       <* 
 *>       glVertex3f  (a_x2 + a_xinc, a_y2 - a_yinc, a_z);                                                       <* 
 *>    } glEnd   ();                                                                                             <* 
 *>    /+---(complete)-----------------------+/                                                                  <* 
 *>    return 0;                                                                                                 <* 
 *> }                                                                                                            <*/

/*> char                                                                                                                                    <* 
 *> TICK_servoline     (tPANEL *a_panel, char a_type, float a_base, float a_bsec, float a_esec, float a_bdeg, float a_edeg, float a_unit)   <* 
 *> {                                                                                                                                       <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                                             <* 
 *>    float       x_beg       =  0.0;                                                                                                      <* 
 *>    float       x_end       =  0.0;                                                                                                      <* 
 *>    float       y_beg       =  0.0;                                                                                                      <* 
 *>    float       y_end       =  0.0;                                                                                                      <* 
 *>    float       y_inc       =  0.0;                                                                                                      <* 
 *>    float       x_inc       =  0.0;                                                                                                      <* 
 *>    float       z_pos       =  0.0;                                                                                                      <* 
 *>    /+---(header)-------------------------+/                                                                                             <* 
 *>    DEBUG_GRAF   yLOG_senter  (__FUNCTION__);                                                                                            <* 
 *>    /+---(prepare beg)--------------------+/                                                                                             <* 
 *>    DEBUG_GRAF   yLOG_sint    (a_panel->seq);                                                                                            <* 
 *>    DEBUG_GRAF   yLOG_sint    (a_panel->sect);                                                                                           <* 
 *>    DEBUG_GRAF   yLOG_sdouble (a_panel->beg);                                                                                            <* 
 *>    DEBUG_GRAF   yLOG_sdouble (s_len);                                                                                                   <* 
 *>    DEBUG_GRAF   yLOG_sdouble (a_panel->beg + s_len);                                                                                    <* 
 *>    DEBUG_GRAF   yLOG_snote   ("---");                                                                                                   <* 
 *>    DEBUG_GRAF   yLOG_sdouble (a_bsec);                                                                                                  <* 
 *>    x_beg   = a_bsec - a_panel->beg;                                                                                                     <* 
 *>    DEBUG_GRAF   yLOG_sdouble (x_beg);                                                                                                   <* 
 *>    if (x_beg > s_len) {                                                                                                                 <* 
 *>       DEBUG_GRAF   yLOG_snote   ("beyond panel end");                                                                                   <* 
 *>       DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);                                                                                         <* 
 *>       return 0;                                                                                                                         <* 
 *>    }                                                                                                                                    <* 
 *>    x_beg  *= a_unit;                                                                                                                    <* 
 *>    DEBUG_GRAF   yLOG_sdouble (x_beg);                                                                                                   <* 
 *>    /+---(prepare end)--------------------+/                                                                                             <* 
 *>    DEBUG_GRAF   yLOG_snote   ("---");                                                                                                   <* 
 *>    DEBUG_GRAF   yLOG_sdouble (a_esec);                                                                                                  <* 
 *>    x_end   = a_esec - a_panel->beg;                                                                                                     <* 
 *>    DEBUG_GRAF   yLOG_sdouble (x_end);                                                                                                   <* 
 *>    if (x_end <  0.0     ) {                                                                                                             <* 
 *>       DEBUG_GRAF   yLOG_snote   ("before panel beg");                                                                                   <* 
 *>       DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);                                                                                         <* 
 *>       return 0;                                                                                                                         <* 
 *>    }                                                                                                                                    <* 
 *>    x_end  *= a_unit;                                                                                                                    <* 
 *>    DEBUG_GRAF   yLOG_sdouble (x_end);                                                                                                   <* 
 *>    /+---(prepare y-pos)------------------+/                                                                                             <* 
 *>    y_beg   = a_base + a_bdeg;                                                                                                           <* 
 *>    y_end   = a_base + a_edeg;                                                                                                           <* 
 *>    /+---(prepare adjustments)------------+/                                                                                             <* 
 *>    DEBUG_GRAF   yLOG_snote   ("GOOD");                                                                                                  <* 
 *>    switch (a_type) {                                                                                                                    <* 
 *>    case 'f' :   /+ femur         +/                                                                                                     <* 
 *>       y_inc = 4;                                                                                                                        <* 
 *>       z_pos = 230.0;                                                                                                                    <* 
 *>       glColor4f    (0.50f, 0.00f, 0.00f, 1.0f);                                                                                         <* 
 *>       glLineWidth  (15.0f);                                                                                                             <* 
 *>       break;                                                                                                                            <* 
 *>    case 'p' :   /+ patella       +/                                                                                                     <* 
 *>       y_inc = 2;                                                                                                                        <* 
 *>       z_pos = 233.0;                                                                                                                    <* 
 *>       glColor4f    (0.20f, 0.20f, 0.70f, 1.0f);                                                                                         <* 
 *>       glLineWidth  (10.0f);                                                                                                             <* 
 *>       break;                                                                                                                            <* 
 *>    case 't' :   /+ tibia         +/                                                                                                     <* 
 *>       y_inc = 1;                                                                                                                        <* 
 *>       z_pos = 236.0;                                                                                                                    <* 
 *>       glColor4f    (0.60f, 0.60f, 0.00f, 1.0f);                                                                                         <* 
 *>       glLineWidth  ( 4.0f);                                                                                                             <* 
 *>       break;                                                                                                                            <* 
 *>    case 'e' :   /+ end point     +/                                                                                                     <* 
 *>       x_inc = 5;                                                                                                                        <* 
 *>       z_pos = 239.0;                                                                                                                    <* 
 *>       glColor4f    (0.50f, 0.00f, 0.50f, 1.0f);                                                                                         <* 
 *>       glLineWidth  (10.0f);                                                                                                             <* 
 *>       break;                                                                                                                            <* 
 *>    case 'c' :   /+ current       +/                                                                                                     <* 
 *>       x_inc = 0;                                                                                                                        <* 
 *>       z_pos = 239.0;                                                                                                                    <* 
 *>       glColor4f    (0.00f, 0.00f, 1.00f, 1.0f);                                                                                         <* 
 *>       glLineWidth  ( 5.0f);                                                                                                             <* 
 *>       break;                                                                                                                            <* 
 *>    }                                                                                                                                    <* 
 *>    /+---(draw)--------+/                                                                                                                <* 
 *>    TICK_line (x_beg, y_beg, x_end, y_end, x_inc, y_inc, z_pos);                                                                         <* 
 *>    /+---(complete)-----------------------+/                                                                                             <* 
 *>    DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);                                                                                            <* 
 *>    return 0;                                                                                                                            <* 
 *> }                                                                                                                                       <*/

char
TICK_servo_prep         (char a_type)
{
   switch (a_type) {
   case 'F' :  /* femur original    */
      glColor4f    (0.70f, 0.00f, 0.00f, 1.0f);
      glLineWidth  (15.0f);
   case 'f' :  /* femur adapted     */
      glColor4f    (0.50f, 0.00f, 0.00f, 1.0f);
      glPointSize  ( 5.0f);
      break;
   case 'P' :  /* patella original  */
      glColor4f    (0.20f, 0.20f, 0.80f, 1.0f);
      glLineWidth  (10.0f);
      break;
   case 'p' :  /* patella adapted   */
      glColor4f    (0.10f, 0.10f, 0.50f, 1.0f);
      glPointSize  ( 5.0f);
      break;
   case 'T' :  /* tibia original    */
      glColor4f    (0.60f, 0.60f, 0.00f, 1.0f);
      glLineWidth  ( 4.0f);
      break;
   case 't' :  /* tibia adapted     */
      glColor4f    (0.40f, 0.40f, 0.00f, 1.0f);
      glPointSize  ( 5.0f);
      break;
   case 'X' :  /* xpos original     */
      glColor4f    (0.70f, 0.35f, 0.18f, 1.0f);
      glLineWidth  (15.0f);
      break;
   case 'x' :  /* xpos adapted      */
      glColor4f    (0.50f, 0.25f, 0.12f, 1.0f);
      glPointSize  ( 5.0f);
      break;
   case 'Z' :  /* zpos original     */
      glColor4f    (0.20f, 0.60f, 0.20f, 1.0f);
      glLineWidth  (10.0f);
      break;
   case 'z' :  /* zpos adapted      */
      glColor4f    (0.15f, 0.40f, 0.15f, 1.0f);
      glPointSize  ( 5.0f);
      break;
   case 'Y' :  /* ypos original     */
      glColor4f    (0.60f, 0.00f, 0.40f, 1.0f);
      glLineWidth  ( 4.0f);
      break;
   case 'y' :  /* ypos adapted      */
      glColor4f    (0.40f, 0.00f, 0.30f, 1.0f);
      glPointSize  ( 5.0f);
      break;
   }
   return 0;
}

char
TICK_servo_data    (tPANEL *a_panel, int a_row, int a_col, char a_type, float *b, float *e)
{
   if (b == NULL)  return -1;
   if (e == NULL)  return -2;
   if (a_panel->detail [a_row][a_col + 0].used != 'y')  return -3;
   if (a_panel->detail [a_row][a_col + 1].used != 'y')  --a_col;
   switch (a_type) {
   case 'F' :
      *b = a_panel->detail [a_row][a_col + 0].o_femu;
      *e = a_panel->detail [a_row][a_col + 1].o_femu;
      break;
   case 'P' :
      *b = a_panel->detail [a_row][a_col + 0].o_pate;
      *e = a_panel->detail [a_row][a_col + 1].o_pate;
      break;
   case 'T' :
      *b = a_panel->detail [a_row][a_col + 0].o_tibi;
      *e = a_panel->detail [a_row][a_col + 1].o_tibi;
      break;
   case 'X' :
      *b = a_panel->detail [a_row][a_col + 0].o_xpos;
      *e = a_panel->detail [a_row][a_col + 1].o_xpos;
      break;
   case 'Z' :
      *b = a_panel->detail [a_row][a_col + 0].o_zpos;
      *e = a_panel->detail [a_row][a_col + 1].o_zpos;
      break;
   case 'Y' :
      *b = a_panel->detail [a_row][a_col + 0].o_ypos;
      *e = a_panel->detail [a_row][a_col + 1].o_ypos;
      break;
   case 'f' :
      *b = a_panel->detail [a_row][a_col + 0].r_femu;
      *e = a_panel->detail [a_row][a_col + 1].r_femu;
      break;
   case 'p' :
      *b = a_panel->detail [a_row][a_col + 0].r_pate;
      *e = a_panel->detail [a_row][a_col + 1].r_pate;
      break;
   case 't' :
      *b = a_panel->detail [a_row][a_col + 0].r_tibi;
      *e = a_panel->detail [a_row][a_col + 1].r_tibi;
      break;
   case 'x' :
      *b = a_panel->detail [a_row][a_col + 0].r_xpos;
      *e = a_panel->detail [a_row][a_col + 1].r_xpos;
      break;
   case 'z' :
      *b = a_panel->detail [a_row][a_col + 0].r_zpos;
      *e = a_panel->detail [a_row][a_col + 1].r_zpos;
      break;
   case 'y' :
      *b = a_panel->detail [a_row][a_col + 0].r_ypos;
      *e = a_panel->detail [a_row][a_col + 1].r_ypos;
      break;
   }
   if (tolower (a_type) == a_type) {
      if (a_panel->detail [a_row][a_col + 0].r_rc < 0)  return -5;
   }
   return 0;
}

char
TICK_servo__solid  (tPANEL *a_panel, int a_line, char a_type, float a_base, float a_scale, int a_off, float a_z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;
   int         x_pos       =    0;
   int         x_one       =    0;
   int         x_two       =    0;
   float       y_beg       =  0.0;
   float       y_end       =  0.0;
   char        x_rc        =    0;
   float       z           =  0.0;
   for (i = 0; i < MAX_WIDE; ++i) {
      x_pos = a_panel->beg * 10 + i;
      x_one = (i + 0) * 10;
      x_two = (i + 1) * 10;
      /*> rc = TICK_servo_data (a_panel, a_line, i, a_type, &y_beg, &y_end);          <*/
      rc = yKINE_ticker (a_line, x_pos, a_type, &y_beg, &y_end, &x_rc);
      if (rc < 0)   continue;
      z = a_z;
      glBegin(GL_LINES); {
         glVertex3f  (x_one, a_base + (y_beg * a_scale + a_off), z);
         glVertex3f  (x_two, a_base + (y_end * a_scale + a_off), z);
      } glEnd   ();
   }
   return 0;
}

char
TICK_servo__dotted (tPANEL *a_panel, int a_line, char a_type, float a_base, float a_scale, int a_off, float a_z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;
   int         x_pos       =    0;
   int         x_one       =    0;
   int         x_two       =    0;
   float       y_beg       =  0.0;
   float       y_end       =  0.0;
   char        x_rc        =    0;
   float       z           =  0.0;
   for (i = 0; i < MAX_WIDE; i += 2) {
      x_pos = a_panel->beg * 10 + i;
      x_one = (i + 0) * 10;
      x_two = (i + 1) * 10;
      /*> rc = TICK_servo_data (a_panel, a_line, i, a_type, &y_beg, &y_end);          <*/
      rc = yKINE_ticker (a_line, x_pos, a_type, &y_beg, &y_end, &x_rc);
      if (rc < 0)   continue;
      z = a_z - 5.0;
      glBegin(GL_LINES); {
         glVertex3f  (x_one, a_base + (y_beg * a_scale + 1.0), z);
         glVertex3f  (x_two, a_base + (y_end * a_scale + 1.0), z);
         glVertex3f  (x_one, a_base + (y_beg * a_scale - 1.0), z);
         glVertex3f  (x_two, a_base + (y_end * a_scale - 1.0), z);
      } glEnd   ();
   }
   glColor4f     (0.0, 0.0, 0.0, 1.0);
   for (i = 1; i < MAX_WIDE; i += 2) {
      x_pos = a_panel->beg * 10 + i;
      x_one = (i + 0) * 10;
      x_two = (i + 1) * 10;
      /*> rc = TICK_servo_data (a_panel, a_line, i, a_type, &y_beg, &y_end);          <*/
      rc = yKINE_ticker (a_line, x_pos, a_type, &y_beg, &y_end, &x_rc);
      if (rc < 0)   continue;
      z = a_z - 5.0;
      glBegin(GL_LINES); {
         glVertex3f  (x_one, a_base + (y_beg * a_scale + 1.0), z);
         glVertex3f  (x_two, a_base + (y_end * a_scale + 1.0), z);
         glVertex3f  (x_one, a_base + (y_beg * a_scale - 1.0), z);
         glVertex3f  (x_two, a_base + (y_end * a_scale - 1.0), z);
      } glEnd   ();
   }
   return 0;
}

/*> char                                                                                             <* 
 *> TICK_servo__accel  (tPANEL *a_panel, int a_line, float a_base, float a_z)                        <* 
 *> {                                                                                                <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                      <* 
 *>    int         i           =    0;                                                               <* 
 *>    for (i = 1; i < MAX_WIDE; ++i) {                                                              <* 
 *>       if (a_panel->detail [a_line][i].used  != 'y')   continue;                                  <* 
 *>       if (a_panel->detail [a_line][i].cell  == '-')   continue;                                  <* 
 *>       if (a_panel->detail [a_line][i].cell  == a_panel->detail [a_line][i - 1].cell) continue;   <* 
 *>       glColor4f     (0.0, 0.0, 0.0, 1.0);                                                        <* 
 *>       glBegin(GL_POINTS); {                                                                      <* 
 *>          glVertex3f  (i * 10, a_base - 45.0, a_z);                                               <* 
 *>       }                                                                                          <* 
 *>       glEnd   ();                                                                                <* 
 *>    }                                                                                             <* 
 *>    return 0;                                                                                     <* 
 *> }                                                                                                <*/

/*> char                                                                                                                    <* 
 *> TICK_servo__labels (tPANEL *a_panel, int a_line, float a_base, float a_z)                                               <* 
 *> {                                                                                                                       <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                                                             <* 
 *>    int         i           =    0;                                                                                      <* 
 *>    char        x_show      =  '-';                                                                                      <* 
 *>    for (i = 0; i < MAX_WIDE; ++i) {                                                                                     <* 
 *>       if (a_panel->detail [a_line][i].used  != 'y')   continue;                                                         <* 
 *>       if (a_panel->detail [a_line][i].label == NULL)  continue;                                                         <* 
 *>       x_show = '-';                                                                                                     <* 
 *>       if (i - 1 < 0)                                      x_show = 'y';                                                 <* 
 *>       if (a_panel->detail [a_line][i - 1].label == NULL)  x_show = 'y';                                                 <* 
 *>       else if (strcmp (a_panel->detail [a_line][i - 1].label, a_panel->detail [a_line][i].label) != 0)  x_show = 'y';   <* 
 *>       if (x_show != 'y')  continue;                                                                                     <* 
 *>       glColor4f     (0.0, 0.0, 0.0, 1.0);                                                                               <* 
 *>       if (a_panel->detail [a_line][i + 1].label != NULL) {                                                              <* 
 *>          glPushMatrix(); {                                                                                              <* 
 *>             glTranslatef  ((i + 0) * 10, a_base - 35.0, a_z);                                                           <* 
 *>             yFONT_print  (my.fixed,  15, YF_BOTLEF, a_panel->detail [a_line][i + 1].label);                             <* 
 *>          } glPopMatrix();                                                                                               <* 
 *>       }                                                                                                                 <* 
 *>       /+> if (a_panel->detail [a_line][i + 1].cell != '-') {                          <*                                <* 
 *>        *>    glBegin(GL_POINTS); {                                                    <*                                <* 
 *>        *>       glVertex3f  (i * 10, a_base - 45.0, a_z);                             <*                                <* 
 *>        *>    }                                                                        <*                                <* 
 *>        *>    glEnd   ();                                                              <*                                <* 
 *>        *> }                                                                           <+/                               <* 
 *>    }                                                                                                                    <* 
 *>    return 0;                                                                                                            <* 
 *> }                                                                                                                       <*/


char
TICK_servo_line    (tPANEL *a_panel, int a_line, char a_type, float a_base, float a_scale, int a_off, float a_z)
{
   /*---(original)----------------------*/
   if (strchr ("FPTXZY", a_type) != NULL) {
      TICK_servo__solid  (a_panel, a_line, a_type, a_base, a_scale, a_off, a_z);
      /*> TICK_servo__accel  (a_panel, a_line, a_base, a_z);                          <*/
      /*> if (strchr ("FX",     a_type) != NULL) {                                    <* 
       *>    TICK_servo__labels (a_panel, a_line, a_base, a_z);                       <* 
       *> }                                                                           <*/
   }
   /*---(revised)-----------------------*/
   else {
      TICK_servo__dotted (a_panel, a_line, a_type, a_base, a_scale, a_off, a_z);
   }
   /*---(complete)----------------------*/
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_sections      (tPANEL *a_panel)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_major     =  '-';
   char        x_minor     =  '-';
   int         x_line      =   -1;
   char        x_label     [LEN_LABEL];
   float       x_sec       = -1.0;
   char        t           [LEN_LABEL];
   float       x           =  0.0;
   int         i           =    0;
   int         x_ref       =    0;
   float       x_base      =  0.0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_complex ("panel"     , "%d %6.1fb %6.1fl %6.1fe", a_panel->seq, a_panel->beg, s_len, a_panel->beg + s_len);
   rc = yKINE_sect_cursor (YKINE_HEAD, &x_major, &x_minor, &x_line, x_label, &x_sec);
   DEBUG_GRAF   yLOG_complex ("section"   , "%3d %c %c %3d %-15.15s %6.1f", rc, x_major, x_minor, x_line, x_label, x_sec);
   while (rc >= 0) {
      /*---(filter)----------------------*/
      DEBUG_GRAF   yLOG_complex ("checking"  , "%6.1f < %6.1f <= %6.1f", a_panel->beg, x_sec, a_panel->beg + s_len);
      if (x_sec >  a_panel->beg + s_len) {
         DEBUG_GRAF   yLOG_note    ("overran panel end, stopping");
         break;
      }
      /*---(filter)----------------------*/
      if (x_sec <  a_panel->beg) {
         DEBUG_GRAF   yLOG_note    ("before panel start");
         rc = yKINE_sect_cursor (YKINE_NEXT, &x_major, &x_minor, &x_line, x_label, &x_sec);
         continue;
      }
      /*---(draw)------------------------*/
      sprintf (t, "%c%c", x_major, x_minor);
      x  = (x_sec - a_panel->beg) * 100.0;
      for (i = 0; i < my.p_nline; ++i) {
         x_ref    = TICK_line_find (i);
         x_base   = (s_tall - (s_yinc * x_ref)) - s_top;
         glColor4f    (0.50f, 0.00f, 0.50f, 1.0f);
         glLineWidth  (5.0f);
         glPushMatrix(); {
            glTranslatef  (x + 5, x_base + 55.0, 10.0);
            yFONT_print  (my.fixed,  15, YF_BOTLEF, t);
            glTranslatef  (5.0  , -10.0        ,  0.0);
            yFONT_print  (my.fixed,  10, YF_BOTLEF, x_label);
         } glPopMatrix();
         if (strcmp (t, "aa") == 0 || strcmp (t, "zz") == 0) {
            glBegin         (GL_LINES); {
               glVertex3f  (x - 2.0, x_base +  85.0,  10.0);
               glVertex3f  (x - 2.0, x_base -  85.0,  10.0);
               glVertex3f  (x + 2.0, x_base +  85.0,  10.0);
               glVertex3f  (x + 2.0, x_base -  85.0,  10.0);
            } glEnd   ();
         } else {
            glBegin         (GL_LINES); {
               glVertex3f  (x, x_base +  85.0,  10.0);
               glVertex3f  (x, x_base -  85.0,  10.0);
            } glEnd   ();
         }
      }
      /*---(next)------------------------*/
      rc = yKINE_sect_cursor (YKINE_NEXT, &x_major, &x_minor, &x_line, x_label, &x_sec);
      DEBUG_GRAF   yLOG_complex ("section"   , "%3d %c %c %3d %-15.15s %6.1f", rc, x_major, x_minor, x_line, x_label, x_sec);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_servos_pos    (tPANEL *a_panel, int a_line, float a_scale)
{
   /*---(locals)-----------+-----+-----+-*/
   char      rc            =    0;               /* generic return code       */
   float     x_base        =  0.0;
   float     x_unit        =    0;
   int       x_leg         =    0;
   float     x_bsec        =    0;
   float     x_bdeg        =    0;
   float     x_esec        =    0;
   float     x_edeg        =    0;
   int       i             =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   x_leg     = s_line_info [a_line].leg;
   x_base    = (s_tall - (s_yinc * a_line)) - s_top;
   x_unit    = s_xinc / my.p_inc;
   /*---(femur)--------------------------*/
   glPushMatrix(); {
      TICK_servo_prep ('X');
      TICK_servo_line (a_panel, a_line, 'X', x_base, a_scale,  3, 230);
      TICK_servo_prep ('X');
      TICK_servo_line (a_panel, a_line, 'X', x_base, a_scale, -3, 230);
      TICK_servo_prep ('x');
      TICK_servo_line (a_panel, a_line, 'x', x_base, a_scale,  0, 330);
   } glPopMatrix();
   /*---(patella)------------------------*/
   glPushMatrix(); {
      TICK_servo_prep ('Z');
      TICK_servo_line (a_panel, a_line, 'Z', x_base, a_scale,  1, 240);
      TICK_servo_prep ('Z');
      TICK_servo_line (a_panel, a_line, 'Z', x_base, a_scale, -1, 240);
      TICK_servo_prep ('z');
      TICK_servo_line (a_panel, a_line, 'z', x_base, a_scale,  0, 340);
   } glPopMatrix();
   /*---(tibia)--------------------------*/
   glPushMatrix(); {
      TICK_servo_prep ('Y');
      TICK_servo_line (a_panel, a_line, 'Y', x_base, a_scale,  0, 250);
      TICK_servo_prep ('y');
      TICK_servo_line (a_panel, a_line, 'y', x_base, a_scale,  0, 350);
      /*> printf ("%d  %3d  :: %2d   %8.1ff %8.1fp %8.1ft   %8.1fx %8.1fz %8.1fy    %8.1fx %8.1fy \n", a_panel->sect, i, a_line, a_panel->detail [a_line][i].o_femu, a_panel->detail [a_line][i].o_pate, a_panel->detail [a_line][i].o_tibi, a_panel->detail [a_line][i].o_xpos, a_panel->detail [a_line][i].o_zpos, a_panel->detail [a_line][i].o_ypos, i * 10, x_base + a_panel->detail [a_line][i].o_tibi);   <*/
   } glPopMatrix();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_servos_deg    (tPANEL *a_panel, int a_line, float a_scale)
{
   /*---(locals)-----------+-----+-----+-*/
   char      rc            =    0;               /* generic return code       */
   float     x_base        =  0.0;
   float     x_unit        =    0;
   int       x_leg         =    0;
   float     x_bsec        =    0;
   float     x_bdeg        =    0;
   float     x_esec        =    0;
   float     x_edeg        =    0;
   int       i             =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   x_leg     = s_line_info [a_line].leg;
   x_base    = (s_tall - (s_yinc * a_line)) - s_top;
   x_unit    = s_xinc / my.p_inc;
   /*---(femur)--------------------------*/
   glPushMatrix(); {
      TICK_servo_prep ('F');
      TICK_servo_line (a_panel, a_line, 'F', x_base, a_scale,  3, 230);
      TICK_servo_prep ('F');
      TICK_servo_line (a_panel, a_line, 'F', x_base, a_scale, -3, 230);
      TICK_servo_prep ('f');
      TICK_servo_line (a_panel, a_line, 'f', x_base, a_scale,  0, 330);
   } glPopMatrix();
   /*---(patella)------------------------*/
   glPushMatrix(); {
      TICK_servo_prep ('P');
      TICK_servo_line (a_panel, a_line, 'P', x_base, a_scale,  1, 240);
      TICK_servo_prep ('P');
      TICK_servo_line (a_panel, a_line, 'P', x_base, a_scale, -1, 240);
      TICK_servo_prep ('p');
      TICK_servo_line (a_panel, a_line, 'p', x_base, a_scale,  0, 340);
   } glPopMatrix();
   /*---(tibia)--------------------------*/
   glPushMatrix(); {
      TICK_servo_prep ('T');
      TICK_servo_line (a_panel, a_line, 'T', x_base, a_scale,  0, 250);
      TICK_servo_prep ('t');
      TICK_servo_line (a_panel, a_line, 't', x_base, a_scale,  0, 350);
      /*> printf ("%d  %3d  :: %2d   %8.1ff %8.1fp %8.1ft   %8.1fx %8.1fz %8.1fy    %8.1fx %8.1fy \n", a_panel->sect, i, a_line, a_panel->detail [a_line][i].o_femu, a_panel->detail [a_line][i].o_pate, a_panel->detail [a_line][i].o_tibi, a_panel->detail [a_line][i].o_xpos, a_panel->detail [a_line][i].o_zpos, a_panel->detail [a_line][i].o_ypos, i * 10, x_base + a_panel->detail [a_line][i].o_tibi);   <*/
   } glPopMatrix();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          heat map                            ----===*/
/*====================------------------------------------====================*/
static void      o___HEATMAP_________________o (void) {;}

/*> char                                                                                                                   <* 
 *> TICK_servoheat     (char a_type, float a_base, float a_sec1, float a_sec2, float a_deg1, float a_deg2, float a_unit)   <* 
 *> {                                                                                                                      <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                            <* 
 *>    float     x_pos1        = 0.0;                                                                                      <* 
 *>    float     x_pos2        = 0.0;                                                                                      <* 
 *>    float     y_pos1        = 0.0;                                                                                      <* 
 *>    float     y_pos2        = 0.0;                                                                                      <* 
 *>    float     y_inc         = 0.0;                                                                                      <* 
 *>    float     x_inc         = 0.0;                                                                                      <* 
 *>    float     z_pos         = 0.0;                                                                                      <* 
 *>    /+---(prepare points)-----------------+/                                                                            <* 
 *>    x_pos1   = (a_sec1 * a_unit) - s_start;                                                                             <* 
 *>    x_pos2   = (a_sec2 * a_unit) - s_start;                                                                             <* 
 *>    y_pos1   = a_base + a_deg1;                                                                                         <* 
 *>    y_pos2   = a_base + a_deg2;                                                                                         <* 
 *>    /+---(prepare adjustments)------------+/                                                                            <* 
 *>    switch (a_type) {                                                                                                   <* 
 *>    case 'f' : y_inc = 5; z_pos = 30.0;  break;   /+ femur         +/                                                   <* 
 *>    case 'p' : y_inc = 2; z_pos = 33.0;  break;   /+ patella       +/                                                   <* 
 *>    case 't' : y_inc = 0; z_pos = 36.0;  break;   /+ tibia         +/                                                   <* 
 *>    case 'e' : x_inc = 5; z_pos = 39.0;  break;   /+ end point     +/                                                   <* 
 *>    case 'c' : x_inc = 0; z_pos = 39.0;  break;   /+ current       +/                                                   <* 
 *>    }                                                                                                                   <* 
 *>    /+---(draw)--------+/                                                                                               <* 
 *>    if      (x_pos2 <  0.0     )  ;  /+ line to early +/                                                                <* 
 *>    else if (x_pos1 >  s_cutend)  ;  /+ line to late  +/                                                                <* 
 *>    else if (x_pos2 <= s_cutmid)                                                                                        <* 
 *>       TICK_line (x_pos1           , y_pos1         , x_pos2           , y_pos2         , x_inc, y_inc, z_pos);         <* 
 *>    else if (x_pos1 >= s_cutmid)                                                                                        <* 
 *>       TICK_line (x_pos1 - s_cutmid, y_pos1 + 1500.0, x_pos2 - s_cutmid, y_pos2 + 1500.0, x_inc, y_inc, z_pos);         <* 
 *>    else {                                                                                                              <* 
 *>       TICK_line (x_pos1           , y_pos1         , x_pos2           , y_pos2         , x_inc, y_inc, z_pos);         <* 
 *>       TICK_line (x_pos1 - s_cutmid, y_pos1 + 1500.0, x_pos2 - s_cutmid, y_pos2 + 1500.0, x_inc, y_inc, z_pos);         <* 
 *>    }                                                                                                                   <* 
 *>    /+---(complete)-----------------------+/                                                                            <* 
 *>    return 0;                                                                                                           <* 
 *> }                                                                                                                      <*/

/*> static char  /+--> represent tibia placement accuracy ----[ ------ [ ------ ]-+/            <* 
 *> TICK__heat_spot    (char a_type, char a_rc, float  a_diff, float  a_x, float  *a_y)         <* 
 *> {                                                                                           <* 
 *>    float       x_diff      =   1.00;                                                        <* 
 *>    float       x_base      =   1.00;                                                        <* 
 *>    float       x_width     =   8.00;                                                        <* 
 *>    float       x_xoff      =   1.00;                                                        <* 
 *>    float       x_height    =   8.00;                                                        <* 
 *>    float       x_yoff      =  12.00;                                                        <* 
 *>    float       x_alpha     =   0.50;                                                        <* 
 *>    float       x_z         = 180.00;                                                        <* 
 *>    float       x_red, x_grn, x_blu;                                                         <* 
 *>    switch (a_type) {                                                                        <* 
 *>    case 't' :  /+ touch      (xzy) +/                                                       <* 
 *>       x_base    *= 1.0;                                                                     <* 
 *>       x_height  *= 2.0;                                                                     <* 
 *>       break;                                                                                <* 
 *>    case 'x' : case 'z' :                                                                    <* 
 *>       x_diff     = -(fabs (a_diff));                                                        <* 
 *>       break;                                                                                <* 
 *>    case 'h' :  /+ horizontal (xz)  +/                                                       <* 
 *>       x_diff     = -(fabs (a_diff));                                                        <* 
 *>       x_base    *= 1.0;                                                                     <* 
 *>       /+> x_base    *= 2.0;                                                           <+/   <* 
 *>       x_height  *= 3.0;                                                                     <* 
 *>       x_yoff    *= 3.0;                                                                     <* 
 *>       break;                                                                                <* 
 *>    case 'y' :                                                                               <* 
 *>       x_diff     = -(fabs (a_diff));                                                        <* 
 *>       x_height  *= 2.0;                                                                     <* 
 *>       break;                                                                                <* 
 *>    case 'f' :  /+ full       (xzy) +/                                                       <* 
 *>       x_diff     = -(fabs (a_diff));                                                        <* 
 *>       x_base    *= 2.0;                                                                     <* 
 *>       x_height  *= 3.0;                                                                     <* 
 *>       break;                                                                                <* 
 *>    default  : return -1;                                                                    <* 
 *>               break;                                                                        <* 
 *>    }                                                                                        <* 
 *>    if      (a_rc   <   0 )  yCOLOR_val2error  (0.8);                                        <* 
 *>    else                     yCOLOR_val2color  (x_base * a_diff, 0.8);                       <* 
 *>    glPushMatrix(); {                                                                        <* 
 *>       glBegin         (GL_POLYGON); {                                                       <* 
 *>          glVertex3f  (a_x + x_xoff          , *a_y           , x_z);                        <* 
 *>          glVertex3f  (a_x + x_xoff + x_width, *a_y           , x_z);                        <* 
 *>          glVertex3f  (a_x + x_xoff + x_width, *a_y - x_height, x_z);                        <* 
 *>          glVertex3f  (a_x + x_xoff          , *a_y - x_height, x_z);                        <* 
 *>       } glEnd   ();                                                                         <* 
 *>    } glPopMatrix();                                                                         <* 
 *>    *a_y -= (x_height + x_yoff);                                                             <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> static char  /+--> represent tibia placement accuracy ----[ ------ [ ------ ]-+/                                                                                                    <* 
 *> TICK__heat_column    (int a_leg, float  a_sec, float  a_x, float  a_y)                                                                                                              <* 
 *> {                                                                                                                                                                                   <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                                                                                         <* 
 *>    float     x_xdif        = 0.0;                                                                                                                                                   <* 
 *>    float     x_zdif        = 0.0;                                                                                                                                                   <* 
 *>    float     x_ydif        = 0.0;                                                                                                                                                   <* 
 *>    float     x_ypos        = 0.0;                                                                                                                                                   <* 
 *>    float     x_xz          =  0.0;                                                                                                                                                  <* 
 *>    float     x_full        =  0.0;                                                                                                                                                  <* 
 *>    char      rc            = 0;                                                                                                                                                     <* 
 *>    /+---(tibia)--------------------------+/                                                                                                                                         <* 
 *>    rc     = yKINE_move_exact (a_sec, a_leg, &x_xdif, &x_zdif, &x_ydif, &x_ypos);                                                                                                    <* 
 *>    x_xz   = sqrt ((x_xdif * x_xdif) + (x_zdif * x_zdif));                                                                                                                           <* 
 *>    x_full = sqrt ((x_xdif * x_xdif) + (x_zdif * x_zdif) + (x_ydif * x_ydif));                                                                                                       <* 
 *>    /+> if (a_leg == 1 && a_sec < 20.0)  printf ("TICK__heat_column, a_leg=%d, a_sec=%8.3lf, rc=%3d, s_lowest=%8.1lf, x_ypos=%8.1lf\n", a_leg, a_sec, rc, s_lowest, x_ypos);   <+/   <* 
 *>    s_debug_leg = a_leg;                                                                                                                                                             <* 
 *>    s_debug_sec = a_sec;                                                                                                                                                             <* 
 *>    TICK__heat_spot    ('t', rc, s_lowest - x_ypos   , a_x, &a_y);                                                                                                                   <* 
 *>    TICK__heat_spot    ('x', rc, x_xdif              , a_x, &a_y);                                                                                                                   <* 
 *>    TICK__heat_spot    ('z', rc, x_zdif              , a_x, &a_y);                                                                                                                   <* 
 *>    TICK__heat_spot    ('h', rc, x_xz                , a_x, &a_y);                                                                                                                   <* 
 *>    TICK__heat_spot    ('y', rc, x_ydif              , a_x, &a_y);                                                                                                                   <* 
 *>    TICK__heat_spot    ('f', rc, x_full              , a_x, &a_y);                                                                                                                   <* 
 *>    /+---(complete)-----------------------+/                                                                                                                                         <* 
 *>    return 0;                                                                                                                                                                        <* 
 *> }                                                                                                                                                                                   <*/


/*> char                                                                              <* 
 *> TICK_height        (int a_sec, float  a_pos)                                      <* 
 *> {                                                                                 <* 
 *>    float     x_barrange    = 0.0;                                                 <* 
 *>    float     x_bartop      = 0.0;                                                 <* 
 *>    float     x_barbot      = 0.0;                                                 <* 
 *>    x_barrange = s_lowest + 100.0;                                                 <* 
 *>    if (x_barrange >= 0.0) {                                                       <* 
 *>       glColor4f    (1.00f, 1.00f, 1.00f, 0.3f);                                   <* 
 *>       x_bartop = -155.0 + x_barrange;                                             <* 
 *>       x_barbot = -165.0;                                                          <* 
 *>    } else {                                                                       <* 
 *>       glColor4f    (0.00f, 1.00f, 0.00f, 0.3f);                                   <* 
 *>       x_bartop = -110.0;                                                          <* 
 *>       x_barbot = -120.0 + x_barrange;                                             <* 
 *>    }                                                                              <* 
 *>    glPushMatrix(); {                                                              <* 
 *>       glBegin         (GL_POLYGON); {                                             <* 
 *>          glVertex3f  (a_sec + 2.0      , a_pos + x_bartop, 120.0);                <* 
 *>          glVertex3f  (a_sec + 2.0 + 6.0, a_pos + x_bartop, 120.0);                <* 
 *>          glVertex3f  (a_sec + 2.0 + 6.0, a_pos + x_barbot, 120.0);                <* 
 *>          glVertex3f  (a_sec + 2.0      , a_pos + x_barbot, 120.0);                <* 
 *>       } glEnd   ();                                                               <* 
 *>    } glPopMatrix();                                                               <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                     yKINE tick drawing                       ----===*/
/*====================------------------------------------====================*/
static void      o___YKINE___________________o (void) {;}



/*====================------------------------------------====================*/
/*===----                         panel handlers                       ----===*/
/*====================------------------------------------====================*/
static void      o___LOAD____________________o (void) {;}

/*> char                                                                                                                                            <* 
 *> TICK_exact_alt     (tPANEL *a_panel, int i, int j, float a_pate, float a_tibi)                                                                  <* 
 *> {                                                                                                                                               <* 
 *>    char        rc          =    0;                                                                                                              <* 
 *>    float       x, y, z;                                                                                                                         <* 
 *>    float       x_dist      =  0.0;                                                                                                              <* 
 *>    float       xd, zd, yd;                                                                                                                      <* 
 *>    char        x_adj       =  '-';                                                                                                              <* 
 *>    rc = yKINE_forward (j - 1, a_panel->detail [j][i].a_femu, a_panel->detail [j][i].a_pate + a_pate, a_panel->detail [j][i].a_tibi + a_tibi);   <* 
 *>    yKINE_endpoint (j - 1, YKINE_TIBI, YKINE_FK, NULL, NULL, &x, &z, &y);                                                                        <* 
 *>    xd  = x - a_panel->detail [j][i].t_xpos;                                                                                                     <* 
 *>    zd  = z - a_panel->detail [j][i].t_zpos;                                                                                                     <* 
 *>    yd  = y - a_panel->detail [j][i].t_ypos;                                                                                                     <* 
 *>    x_dist = sqrt ((xd * xd) + (zd * zd) + (yd * yd));                                                                                           <* 
 *>    if (x_dist > a_panel->detail [j][i].a_error)  return 0;                                                                                      <* 
 *>    if (a_panel->detail [j][i].a_found == 'y' && x_dist > a_panel->detail [j][i].a_error)  return 0;                                             <* 
 *>    a_panel->detail [j][i].a_found = 'y';                                                                                                        <* 
 *>    a_panel->detail [j][i].x_pate += a_pate;                                                                                                     <* 
 *>    a_panel->detail [j][i].x_tibi += a_tibi;                                                                                                     <* 
 *>    a_panel->detail [j][i].a_xpos  = x;                                                                                                          <* 
 *>    a_panel->detail [j][i].a_zpos  = z;                                                                                                          <* 
 *>    a_panel->detail [j][i].a_ypos  = y;                                                                                                          <* 
 *>    a_panel->detail [j][i].a_error = x_dist;                                                                                                     <* 
 *>    if      (a_pate < 0.0)   x_adj  = 1;                                                                                                         <* 
 *>    else if (a_pate > 0.0)   x_adj  = 7;                                                                                                         <* 
 *>    else                     x_adj  = 4;                                                                                                         <* 
 *>    if      (a_tibi < 0.0)   x_adj += 0;                                                                                                         <* 
 *>    else if (a_tibi > 0.0)   x_adj += 2;                                                                                                         <* 
 *>    else                     x_adj += 1;                                                                                                         <* 
 *>    return x_adj;                                                                                                                                <* 
 *> }                                                                                                                                               <*/

/*> char                                                                                        <* 
 *> TICK_exact_driver      (tPANEL *a_panel, int i, int j, char a_size)                         <* 
 *> {                                                                                           <* 
 *>    char        rc        =    0;                                                            <* 
 *>    float       p         =  0.0;                                                            <* 
 *>    float       t         =  0.0;                                                            <* 
 *>    float       s         =  0.0;                                                            <* 
 *>    char        x_adj     =    0;                                                            <* 
 *>    switch (a_size) {                                                                        <* 
 *>    case 'U' : case 'u' : case 'û' : s = 0.062; break;      /+ ultra   +/                    <* 
 *>    case 'T' : case 't' : case 'ú' : s = 0.125; break;      /+ tiny    +/                    <* 
 *>    case 'F' : case 'f' : case 'ü' : s = 0.250; break;      /+ fine    +/                    <* 
 *>    case 'N' : case 'n' : case 'ô' : s = 0.500; break;      /+ normal  +/                    <* 
 *>    case 'G' : case 'g' :            s = 1.000; break;      /+ gross   +/                    <* 
 *>    default  :                       s = 1.500; break;      /+ super   +/                    <* 
 *>    }                                                                                        <* 
 *>    /+> a_panel->detail [j][i].a_found = '-';                                          <+/   <* 
 *>    for (p = -s; p <= s; p += s) {                                                           <* 
 *>       for (t = -s; t <= s; t += s) {                                                        <* 
 *>          /+> if (p == 0.0 && p == t)  continue;  /+ skip middle  +/                   <+/   <* 
 *>          rc = TICK_exact_alt (a_panel, i, j, p, t);                                         <* 
 *>          if (rc != 0)  x_adj = rc;                                                          <* 
 *>       }                                                                                     <* 
 *>    }                                                                                        <* 
 *>    if (x_adj != 0) {                                                                        <* 
 *>       a_panel->detail [j][i].a_pate   = a_panel->detail [j][i].x_pate;                      <* 
 *>       a_panel->detail [j][i].a_tibi   = a_panel->detail [j][i].x_tibi;                      <* 
 *>    }                                                                                        <* 
 *>    if (x_adj == 0)  x_adj = 5;                                                              <* 
 *>    switch (a_size) {                                                                        <* 
 *>    case 'û' : a_panel->detail [j][i].a_ultra3  = x_adj; break;                              <* 
 *>    case 'u' : a_panel->detail [j][i].a_ultra   = x_adj; break;                              <* 
 *>    case 'U' : a_panel->detail [j][i].a_ULTRA   = x_adj; break;                              <* 
 *>    case 'ú' : a_panel->detail [j][i].a_tiny3   = x_adj; break;                              <* 
 *>    case 't' : a_panel->detail [j][i].a_tiny    = x_adj; break;                              <* 
 *>    case 'T' : a_panel->detail [j][i].a_TINY    = x_adj; break;                              <* 
 *>    case 'ü' : a_panel->detail [j][i].a_fine3   = x_adj; break;                              <* 
 *>    case 'f' : a_panel->detail [j][i].a_fine    = x_adj; break;                              <* 
 *>    case 'F' : a_panel->detail [j][i].a_FINE    = x_adj; break;                              <* 
 *>    case 'ô' : a_panel->detail [j][i].a_normal3 = x_adj; break;                              <* 
 *>    case 'n' : a_panel->detail [j][i].a_normal  = x_adj; break;                              <* 
 *>    case 'N' : a_panel->detail [j][i].a_NORMAL  = x_adj; break;                              <* 
 *>    case 'g' : a_panel->detail [j][i].a_gross   = x_adj; break;                              <* 
 *>    case 'G' : a_panel->detail [j][i].a_GROSS   = x_adj; break;                              <* 
 *>    default  : a_panel->detail [j][i].a_super   = x_adj; break;                              <* 
 *>    }                                                                                        <* 
 *>    return x_adj;                                                                            <* 
 *> }                                                                                           <*/

/*> char                                                                                        <* 
 *> TICK_exact_driver_new  (tPANEL *a_panel, int i, int j, char a_size)                         <* 
 *> {                                                                                           <* 
 *>    char        rc        =    0;                                                            <* 
 *>    int         k         =    0;                                                            <* 
 *>    float       p         =  0.0;                                                            <* 
 *>    float       t         =  0.0;                                                            <* 
 *>    float       s         =  1.0;                                                            <* 
 *>    char        x_adj     =    0;                                                            <* 
 *>    for (k = 0;  k < a_size; ++k)    s *= 0.90;                                              <* 
 *>    for (p = -s; p <= s; p += s) {                                                           <* 
 *>       for (t = -s; t <= s; t += s) {                                                        <* 
 *>          /+> if (p == 0.0 && p == t)  continue;  /+ skip middle  +/                   <+/   <* 
 *>          rc = TICK_exact_alt (a_panel, i, j, p, t);                                         <* 
 *>          if (rc != 0)  x_adj = rc;                                                          <* 
 *>       }                                                                                     <* 
 *>    }                                                                                        <* 
 *>    if (x_adj != 0) {                                                                        <* 
 *>       a_panel->detail [j][i].a_pate   = a_panel->detail [j][i].x_pate;                      <* 
 *>       a_panel->detail [j][i].a_tibi   = a_panel->detail [j][i].x_tibi;                      <* 
 *>    }                                                                                        <* 
 *>    if (x_adj == 0)  x_adj = 5;                                                              <* 
 *>    a_panel->detail [j][i].a_check [a_size]  = '0' + x_adj;                                  <* 
 *>    return x_adj;                                                                            <* 
 *> }                                                                                           <*/

/*> char                                                                                        <* 
 *> TICK_exact_grid        (tPANEL *a_panel, int i, int j)                                      <* 
 *> {                                                                                           <* 
 *>    char        rc        =    0;                                                            <* 
 *>    float       p         =  0.0;                                                            <* 
 *>    float       t         =  0.0;                                                            <* 
 *>    float       s         =  0.0;                                                            <* 
 *>    char        x_adj     =    0;                                                            <* 
 *>    /+> a_panel->detail [j][i].a_found = '-';                                          <+/   <* 
 *>    for (p = -1.0; p <= 1.0; p += 0.25) {                                                    <* 
 *>       for (t = -1.0; t <= 1.0; t += 0.25) {                                                 <* 
 *>          rc = TICK_exact_alt (a_panel, i, j, p, t);                                         <* 
 *>          if (rc != 0)  x_adj = rc;                                                          <* 
 *>       }                                                                                     <* 
 *>    }                                                                                        <* 
 *>    /+> switch (a_size) {                                                              <*    <* 
 *>     *> case 't' : a_panel->detail [j][i].a_tiny   = x_adj; break;                     <*    <* 
 *>     *> case 'f' : a_panel->detail [j][i].a_fine   = x_adj; break;                     <*    <* 
 *>     *> case 'n' : a_panel->detail [j][i].a_normal = x_adj; break;                     <*    <* 
 *>     *> default  : a_panel->detail [j][i].a_gross  = x_adj; break;                     <*    <* 
 *>     *> }                                                                              <+/   <* 
 *>    return x_adj;                                                                            <* 
 *> }                                                                                           <*/

/*> char         /+--> draw texture for progress ticker ------[ ------ [ ------ ]-+/   <* 
 *> TICK_load_single        (tPANEL *a_panel, int a_pos)                               <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                        <* 
 *>    char        rce         =  -10;                                                 <* 
 *>    int         i           =    0;                                                 <* 
 *>    float       x_sec       =  0.0;                                                 <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(defense)------------------------+/                                        <* 
 *>    DEBUG_GRAF   yLOG_point   ("a_panel"   , a_panel);                              <* 
 *>    --rce;  if (a_panel == NULL) {                                                  <* 
 *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    DEBUG_GRAF   yLOG_value   ("a_pos"     , a_pos);                                <* 
 *>    --rce;  if (a_pos < 0 || a_pos >= MAX_WIDE) {                                        <* 
 *>       DEBUG_GRAF   yLOG_exit    (__FUNCTION__, rce);                               <* 
 *>       return rce;                                                                  <* 
 *>    }                                                                               <* 
 *>    /+---(prepare)------------------------+/                                        <* 
 *>    x_sec       = a_panel->beg + ((i / 10.0) * my.p_scale);                         <* 
 *>    rc = yKINE_exact_all (x_sec);                                                   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char         /*--> draw texture for progress ticker ------[ ------ [ ------ ]-*/
TICK_load_exact         (tPANEL *a_panel)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i;                             /* loop iterator                  */
   int         j;                             /* loop iterator                  */
   float       x_pos       =  0.0;
   float       f, p, t, fr, pr, tr;
   float       x, y, z, xr, yr, zr;
   float       x_yaw       =    0;
   float       x_pitch     =    0;
   float       x_roll      =    0;
   char        x_exact     =  '-';
   char        x_leg       [LEN_TERSE];
   char        x_label     [LEN_LABEL];
   char        x_cell      =  '-';
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_point   ("panel"     , a_panel);
   DEBUG_GRAF   yLOG_double  ("beg"       , a_panel->beg);
   DEBUG_GRAF   yLOG_double  ("my.p_len"  , my.p_len);
   /*---(load original points)-----------*/
   tick_panel_wipe (a_panel, '-');
   for (i = 0; i < MAX_WIDE; ++i) {
      x_pos       = a_panel->beg + ((i / 10.0) * my.p_scale);
      if (x_pos > my.p_len)  break;
      if (x_pos < 0.0     )  break;
      rc = yKINE_exact_all (x_pos);
      if (rc < 0)  continue;
      /*> printf ("\n");                                                              <*/
      /*> if (i % 3 == 0)  printf ("____secs ## CALC __femu __pate __tibi   __xpos __zpos __ypos    _IK   __femu __pate __tibi   __xpos __zpos __ypos\n\n");   <*/
      for (j = YKINE_CENTER; j <= YKINE_LR; ++j) {
         rc = yKINE_exact_leg   (j, 0.10, &x_exact, x_label, &x_cell, NULL, &f, &p, &t, &x, &z, &y, NULL, &fr, &pr, &tr, &xr, &zr, &yr);
         ++j;
         /*> rc = 0;                                                                  <*/
         /*---(special)---------------*/
         a_panel->detail [j][i].used   = 'y';
         a_panel->detail [j][i].exact  = x_exact;
         if (x_label [0] != '-')  a_panel->detail [j][i].label  = strdup (x_label);
         a_panel->detail [j][i].cell   = x_cell;
         /*---(original endpoint)-----*/
         a_panel->detail [j][i].o_xpos = x;
         a_panel->detail [j][i].o_zpos = z;
         a_panel->detail [j][i].o_ypos = y;
         /*---(original angles)-------*/
         a_panel->detail [j][i].o_femu = f;
         a_panel->detail [j][i].o_pate = p;
         a_panel->detail [j][i].o_tibi = t;
         /*---(revised angles)--------*/
         a_panel->detail [j][i].r_femu = fr;
         a_panel->detail [j][i].r_pate = pr;
         a_panel->detail [j][i].r_tibi = tr;
         /*---(revised endpoint)------*/
         a_panel->detail [j][i].r_xpos = xr;
         a_panel->detail [j][i].r_zpos = zr;
         a_panel->detail [j][i].r_ypos = yr;
         /*---(results)---------------*/
         a_panel->detail [j][i].r_rc = rc;
         /*---(done)------------------*/
         --j;
         if      (j < 0)  strlcpy (x_leg, "ce", LEN_TERSE);
         else             strlcpy (x_leg, yKINE_legtwo (j), LEN_TERSE);
         /*> printf ("%8.2f %2d %2s   %6.1f %6.1f %6.1f   %6.1f %6.1f %6.1f    %3d   %6.1f %6.1f %6.1f   %6.1f %6.1f %6.1f    %c %-15.15s %c\n", x_pos, j, x_leg, f, p, t, x, z, y, rc, fr, pr, tr, xr, zr, yr, x_exact, x_label, x_cell);   <*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       data accessors                         ----===*/
/*====================------------------------------------====================*/
static void      o___ACCESSORS_______________o (void) {;}

/*> char                                                                                 <* 
 *> TICK_exact_deg          (int a_leg, float *a_femu, float *a_pate, float *a_tibi)     <* 
 *> {                                                                                    <* 
 *>    if (a_leg == YKINE_BODY) {                                                        <* 
 *>       if (a_femu != NULL)  *a_femu = p_draw->detail [a_leg + 1][my.p_tick].o_femu;   <* 
 *>       if (a_pate != NULL)  *a_pate = p_draw->detail [a_leg + 1][my.p_tick].o_pate;   <* 
 *>       if (a_tibi != NULL)  *a_tibi = p_draw->detail [a_leg + 1][my.p_tick].o_tibi;   <* 
 *>    } else {                                                                          <* 
 *>       if (a_femu != NULL)  *a_femu = p_draw->detail [a_leg + 1][my.p_tick].r_femu;   <* 
 *>       if (a_pate != NULL)  *a_pate = p_draw->detail [a_leg + 1][my.p_tick].r_pate;   <* 
 *>       if (a_tibi != NULL)  *a_tibi = p_draw->detail [a_leg + 1][my.p_tick].r_tibi;   <* 
 *>    }                                                                                 <* 
 *>    return 0;                                                                         <* 
 *> }                                                                                    <*/

/*> char                                                                                 <* 
 *> TICK_exact_end          (int a_leg, float *x     , float *z     , float *y     )     <* 
 *> {                                                                                    <* 
 *>    if (a_leg == YKINE_BODY) {                                                        <* 
 *>       if (x      != NULL)  *x      = p_draw->detail [a_leg    ][my.p_tick].o_xpos;   <* 
 *>       if (z      != NULL)  *z      = p_draw->detail [a_leg    ][my.p_tick].o_zpos;   <* 
 *>       if (y      != NULL)  *y      = p_draw->detail [a_leg    ][my.p_tick].o_ypos;   <* 
 *>    } else {                                                                          <* 
 *>       if (x      != NULL)  *x      = p_draw->detail [a_leg + 1][my.p_tick].r_xpos;   <* 
 *>       if (z      != NULL)  *z      = p_draw->detail [a_leg + 1][my.p_tick].r_zpos;   <* 
 *>       if (y      != NULL)  *y      = p_draw->detail [a_leg + 1][my.p_tick].r_ypos;   <* 
 *>    }                                                                                 <* 
 *>    return 0;                                                                         <* 
 *> }                                                                                    <*/

/*> char                                                                               <* 
 *> TICK_exact_target       (int a_leg, float *x     , float *z     , float *y     )   <* 
 *> {                                                                                  <* 
 *>    if (x      != NULL)  *x      = p_draw->detail [a_leg + 1][my.p_tick].o_xpos;    <* 
 *>    if (z      != NULL)  *z      = p_draw->detail [a_leg + 1][my.p_tick].o_zpos;    <* 
 *>    if (y      != NULL)  *y      = p_draw->detail [a_leg + 1][my.p_tick].o_ypos;    <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char                                                                               <* 
 *> TICK_exact_opengl       (int a_leg, float *x     , float *z     , float *y     )   <* 
 *> {                                                                                  <* 
 *>    if (x != NULL)  *x = p_draw->detail [a_leg + 1][my.p_tick].g_xpos;              <* 
 *>    if (z != NULL)  *z = p_draw->detail [a_leg + 1][my.p_tick].g_zpos;              <* 
 *>    if (y != NULL)  *y = p_draw->detail [a_leg + 1][my.p_tick].g_ypos;              <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char                                                                                     <* 
 *> TICK_opengl        (int a_leg, float x, float z, float y)                                <* 
 *> {                                                                                        <* 
 *>    p_draw->detail [a_leg + 1][my.p_tick].g_xpos  = x;                                    <* 
 *>    p_draw->detail [a_leg + 1][my.p_tick].g_zpos  = z;                                    <* 
 *>    p_draw->detail [a_leg + 1][my.p_tick].g_ypos  = y;                                    <* 
 *>    x -= p_draw->detail [a_leg + 1][my.p_tick].t_xpos;                                    <* 
 *>    z -= p_draw->detail [a_leg + 1][my.p_tick].t_zpos;                                    <* 
 *>    y -= p_draw->detail [a_leg + 1][my.p_tick].t_ypos;                                    <* 
 *>    p_draw->detail [a_leg + 1][my.p_tick].g_error = sqrt ((x * x) + (z * z) + (y * y));   <* 
 *>    if (a_leg == YKINE_RR && my.p_tick == 100)   TICK_panel_dump ();                      <* 
 *>    return 0;                                                                             <* 
 *> }                                                                                        <*/

char
TICK_panel_dump         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i;                             /* loop iterator                  */
   int         j;                             /* loop iterator                  */
   int         k;                             /* loop iterator                  */
   float       d;
   char        t;
   for (i = 0; i < MAX_WIDE; ++i) {
      /*> printf ("pan pos svo ::  --cX-- --cZ-- --cY--  --cy-- --cp-- --cr--  --oX-- --oZ-- --oY--  --of-- --op-- --ot--  rci  --tX-- --tZ-- --tY--  --rX-- --rZ-- --rY--  --rf-- --rp-- --rt--  --dX-- ----  --dZ-- ----  --rerr--  --gX-- --gZ-- --gY--  --dX-- ----  --dZ-- ----  --gerr--  a  --aX-- --aZ-- --aY--  --af-- --ap-- --at--  --dX-- ----  --dZ-- ----  --aerr--  s G g N n ô F f ü T t ú U u û\n");   <*/
      printf ("pan pos svo ::  --cX-- --cZ-- --cY--  --cy-- --cp-- --cr--  --oX-- --oZ-- --oY--  --of-- --op-- --ot--  rci  --tX-- --tZ-- --tY--  --rX-- --rZ-- --rY--  --rf-- --rp-- --rt--  --dX-- ----  --dZ-- ----  --rerr--  --gX-- --gZ-- --gY--  --dX-- ----  --dZ-- ----  --gerr--\n");
      for (j = 2; j < YKINE_MAX_LEGS; ++j) {
         if (p_curr->detail [j][i].r_rc < 0)  continue;
         printf ("%3d %3d %3d ::", p_curr->sect, i, j);
         /*---(center)-------------------*/
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [0][i].o_xpos, p_curr->detail [0][i].o_zpos, p_curr->detail [0][i].o_ypos);
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [1][i].o_femu, p_curr->detail [1][i].o_pate, p_curr->detail [1][i].o_tibi);
         /*---(original)-----------------*/
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [j][i].o_xpos, p_curr->detail [j][i].o_zpos, p_curr->detail [j][i].o_ypos);
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [j][i].o_femu, p_curr->detail [j][i].o_pate, p_curr->detail [j][i].o_tibi);
         /*---(adapted)------------------*/
         printf ("  %3d", p_curr->detail [j][i].r_rc);
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [j][i].t_xpos, p_curr->detail [j][i].t_zpos, p_curr->detail [j][i].t_ypos);
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [j][i].r_xpos, p_curr->detail [j][i].r_zpos, p_curr->detail [j][i].r_ypos);
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [j][i].r_femu, p_curr->detail [j][i].r_pate, p_curr->detail [j][i].r_tibi);
         /*---(revised vs original)------*/
         d   = p_curr->detail [j][i].o_xpos - p_curr->detail [j][i].r_xpos;
         printf ("  %6.1f", d);
         if      (d < 0.5 && d > -0.5)  printf (" -   ");
         else if (d < 1.0 && d > -1.0)  printf (" ====");
         else if (d < 2.0 && d > -2.0)  printf (" hmmm");
         else                           printf (" BOOM");
         d   = p_curr->detail [j][i].o_zpos - p_curr->detail [j][i].r_zpos;
         printf ("  %6.1f", d);
         if      (d < 0.5 && d > -0.5)  printf (" -   ");
         else if (d < 1.0 && d > -1.0)  printf (" ====");
         else if (d < 2.0 && d > -2.0)  printf (" hmmm");
         else                           printf (" BOOM");
         printf ("  %8.3f", p_curr->detail [j][i].r_error);
         /*---(opengl)-------------------*/
         printf ("  %6.1f %6.1f %6.1f", p_curr->detail [j][i].g_xpos, p_curr->detail [j][i].g_zpos, p_curr->detail [j][i].g_ypos);
         /*---(opengl vs opengl)---------*/
         d   = p_curr->detail [j][i].o_xpos - p_curr->detail [j][i].g_xpos;
         printf ("  %6.1f", d);
         if      (d < 0.5 && d > -0.5)  printf (" -   ");
         else if (d < 1.0 && d > -1.0)  printf (" ====");
         else if (d < 2.0 && d > -2.0)  printf (" hmmm");
         else                           printf (" BOOM");
         d   = p_curr->detail [j][i].o_zpos - p_curr->detail [j][i].g_zpos;
         printf ("  %6.1f", d);
         if      (d < 0.5 && d > -0.5)  printf (" -   ");
         else if (d < 1.0 && d > -1.0)  printf (" ====");
         else if (d < 2.0 && d > -2.0)  printf (" hmmm");
         else                           printf (" BOOM");
         printf ("  %8.3f", p_curr->detail [j][i].g_error);
         /*---(alternative)--------------*/
         printf ("\n");
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         panel handlers                       ----===*/
/*====================------------------------------------====================*/
static void      o___PANELS__________________o (void) {;}

char         /*--> draw texture for progress ticker ------[ ------ [ ------ ]-*/
TICK_draw_one      (tPANEL *a_panel)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i;                             /* loop iterator                  */
   int         x, y;
   int         x_ref       =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_point   ("a_panel"   , a_panel);
   DEBUG_GRAF   yLOG_value   ("seq"       , a_panel->seq);
   DEBUG_GRAF   yLOG_value   ("sect"      , a_panel->sect);
   /*---(free)------------------------*/
   if (a_panel->tex != 0) {
      rc = yGLTEX_free (&(a_panel->tex), &(a_panel->fbo), &(a_panel->depth));
      if (rc < 0) {
         DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
   }
   /*---(create)----------------------*/
   rc = yGLTEX_new  (&(a_panel->tex), &(a_panel->fbo), &(a_panel->depth), s_wide, s_tall);
   DEBUG_GRAF   yLOG_value   ("new"       , rc);
   if (rc < 0) {
      DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_GRAF   yLOG_value   ("tex"       , a_panel->tex);
   DEBUG_GRAF   yLOG_value   ("fbo"       , a_panel->fbo);
   DEBUG_GRAF   yLOG_value   ("s_wide"    , s_wide);
   DEBUG_GRAF   yLOG_value   ("s_tall"    , s_tall);
   /*---(draw)------------------------*/
   yGLTEX_draw       (a_panel->fbo, YGLTEX_BOTLEF, s_wide, s_tall, 1.0);
   TICK_back_copy    (a_panel);
   /*> TICK_load_exact   (a_panel);                                                   <*/
   TICK_sections     (a_panel);
   DEBUG_GRAF   yLOG_value   ("p_nline"   , my.p_nline);
   for (i = 0; i < my.p_nline; ++i) {
      DEBUG_GRAF   yLOG_value   ("line"      , i);
      x_ref = TICK_line_find (i);
      DEBUG_GRAF   yLOG_value   ("ref"       , x_ref);
      DEBUG_GRAF   yLOG_char    ("content"   , s_line_info [x_ref].content);
      switch (s_line_info [x_ref].content) {
      case 'z' : TICK_servos_pos  (a_panel, x_ref, 1.00);   break;
      case 'o' : TICK_servos_deg  (a_panel, x_ref, 2.00);   break;
      case 'l' : TICK_servos_deg  (a_panel, x_ref, 1.00);   break;
      }
   }
   if (a_panel->act == 's') {
      yGLTEX_tex2png    ("ticker.png", s_wide, s_tall);
      a_panel->act = '-';
   }
   yGLTEX_done       (a_panel->tex);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TICK_draw_all           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   float       x_pos       =  0.0;
   float       x_cur       =  0.0;
   int         x_section   =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_double  ("s_len"     , s_len);
   DEBUG_GRAF   yLOG_double  ("my.p_len"  , my.p_len);
   rc = TICK_curr  ();
   DEBUG_GRAF   yLOG_double  ("my.p_cur"  , my.p_cur);
   DEBUG_GRAF   yLOG_double  ("my.p_pos"  , my.p_pos);
   /*---(review sections)----------------*/
   s_nsec = 0;
   for (x_pos = 0.0; x_pos <= my.p_len; x_pos += s_len) {
      DEBUG_GRAF   yLOG_double  ("x_pos"     , x_pos);
      if (x_pos <  my.p_cur) {
         ++x_section;
         x_cur = x_pos;
      }
      ++s_nsec;
   }
   DEBUG_GRAF   yLOG_double  ("x_cur"     , x_cur);
   DEBUG_GRAF   yLOG_double  ("x_section" , x_section);
   DEBUG_GRAF   yLOG_double  ("s_nsec"    , s_nsec);
   /*---(assign globals)-----------------*/
   p_back->beg   = 0.0;
   p_pref->beg   = x_cur - s_len;
   p_pref->sect  = x_section - 1;
   p_curr->beg   = x_cur;
   p_curr->sect  = x_section;
   p_suff->beg   = x_cur + s_len;
   p_suff->sect  = x_section + 1;
   /*---(re-create)----------------------*/
   TICK_draw_one (p_pref);
   TICK_draw_one (p_curr);
   TICK_draw_one (p_suff);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TICK_rotate_earlier     (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   tPANEL     *x_panel     = NULL;
   /*---(rotate)-------------------------*/
   DEBUG_GRAF   yLOG_senter  (__FUNCTION__);
   x_panel       = p_suff;
   p_suff        = p_curr;
   p_curr        = p_pref;
   p_actv        = p_pref;
   p_pref        = x_panel;
   DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);
   /*---(draw earliest)------------------*/
   p_pref->beg   = p_curr->beg - s_len;
   p_pref->sect  = p_curr->sect - 1;
   rc = TICK_draw_one (p_pref);
   /*---(complete)-----------------------*/
   return 0;
}

char
TICK_rotate_later       (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   tPANEL     *x_panel     = NULL;
   /*---(rotate)-------------------------*/
   DEBUG_GRAF   yLOG_senter  (__FUNCTION__);
   x_panel       = p_pref;
   p_pref        = p_curr;
   p_curr        = p_suff;
   p_actv        = p_suff;
   p_suff        = x_panel;
   DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);
   /*---(draw latest)--------------------*/
   p_suff->beg   = p_curr->beg + s_len;
   p_suff->sect  = p_curr->sect + 1;
   rc = TICK_draw_one (p_suff);
   /*---(complete)-----------------------*/
   return 0;
}

char
TICK_show_panel         (tPANEL *a_panel, float  a_beg, float  a_end, float  a_left, float  a_wide)
{
   DEBUG_GRAF   yLOG_senter  (__FUNCTION__);
   DEBUG_GRAF   yLOG_spoint  (a_panel);
   DEBUG_GRAF   yLOG_sdouble (a_beg);
   DEBUG_GRAF   yLOG_sdouble (a_end);
   DEBUG_GRAF   yLOG_sdouble (a_left);
   DEBUG_GRAF   yLOG_sdouble (a_wide);
   glBindTexture   (GL_TEXTURE_2D, a_panel->tex);
   glBegin(GL_POLYGON); {
      /*---(top beg)--------*/
      glTexCoord2f (a_beg, s_textop);
      glVertex3f   (a_left, 100.00, 0.00);
      /*---(top end)--------*/
      glTexCoord2f (a_end, s_textop);
      glVertex3f   (a_left + a_wide, 100.00, 0.00);
      /*---(bottom end)-----*/
      glTexCoord2f (a_end, s_texbot);
      glVertex3f   (a_left + a_wide,   0.00, 0.00);
      /*---(bottom beg)-----*/
      glTexCoord2f (a_beg, s_texbot);
      glVertex3f   (a_left,   0.00, 0.00);
      /*---(done)-----------*/
   } glEnd();
   glBindTexture   (GL_TEXTURE_2D, 0);
   DEBUG_GRAF   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         main drivers                         ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVERS_________________o (void) {;}

char
TICK_anchor             (void)
{
   switch (s_anchor) {
   case '0' :
      s_texbeg  = s_curp - (s_texpct * 0.00);
      s_texend  = s_texbeg  + s_texpct;
      s_cur     = 0.00 * my.p_wide;
      break;
   case 's' :
      s_texbeg  = s_curp - (s_texpct * 0.05);
      s_texend  = s_texbeg  + s_texpct;
      s_cur     = 0.05 * my.p_wide;
      break;
   case 'h' :
      s_texbeg  = s_curp - (s_texpct * 0.28);
      s_texend  = s_texbeg  + s_texpct;
      s_cur     = 0.28 * my.p_wide;
      break;
   case 'c' :
      s_texbeg  = s_curp - (s_texpct * 0.50);
      s_texend  = s_texbeg  + s_texpct;
      s_cur     = 0.50 * my.p_wide;
      break;
   case 'l' :
      s_texbeg  = s_curp - (s_texpct * 0.72);
      s_texend  = s_texbeg  + s_texpct;
      s_cur     = 0.72 * my.p_wide;
      break;
   case 'e' :
      s_texbeg  = s_curp - (s_texpct * 0.95);
      s_texend  = s_texbeg  + s_texpct;
      s_cur     = 0.95 * my.p_wide;
      break;
   case '$' :
      s_texbeg  = s_curp - (s_texpct * 1.00);
      s_texend  = s_texbeg  + s_texpct;
      s_cur     = 1.00 * my.p_wide;
      break;
   }
   return 0;
}

char         /*--> calculate texture positioning ---------[ ------ [ ------ ]-*/
TICK_curr          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(get the current data)-----------*/
   rc  = yVIKEYS_view_bounds   (YVIKEYS_PROGRESS, NULL, NULL, &my.p_wide, NULL, NULL, &my.p_tall);
   rc  = yVIKEYS_prog_cur (&s_anchor, &my.p_cur, &my.p_scale, &my.p_inc, &my.p_line);
   my.p_leg = my.p_line - 1;
   DEBUG_GRAF   yLOG_value   ("p_line"    , my.p_line);
   DEBUG_GRAF   yLOG_value   ("p_leg"     , my.p_leg);
   /*---(secs per panel)-----------------*/
   DEBUG_GRAF   yLOG_double  ("p_cur"     , my.p_cur);
   DEBUG_GRAF   yLOG_double  ("p_scale"   , my.p_scale);
   s_len       = my.p_scale * UNIT2PANEL;
   DEBUG_GRAF   yLOG_double  ("s_len"     , s_len);
   /*---(check rotation)-----------------*/
   if      (my.p_cur <  p_curr->beg - (s_len * 0.25))  TICK_rotate_earlier ();
   else if (my.p_cur >  p_suff->beg + (s_len * 0.25))  TICK_rotate_later   ();
   /*---(tex measures)-------------------*/
   s_textop    = ((float) (my.p_nline - my.p_line    ) * s_yinc) / s_tall;
   DEBUG_GRAF   yLOG_double  ("s_textop"  , s_textop);
   s_texbot    = ((float) (my.p_nline - my.p_line - 1) * s_yinc) / s_tall;
   DEBUG_GRAF   yLOG_double  ("s_texbot"  , s_texbot);
   s_texavail  = my.p_wide * 2.0;
   DEBUG_GRAF   yLOG_double  ("s_texavail", s_texavail);
   s_texpct    = s_texavail / s_wide;
   DEBUG_GRAF   yLOG_double  ("s_texpct"  , s_texpct);
   /*---(current horizontal pos)---------*/
   s_curp      = (my.p_cur - p_curr->beg) / s_len;
   DEBUG_GRAF   yLOG_double  ("s_curp"    , s_curp);
   TICK_anchor ();
   DEBUG_GRAF   yLOG_char    ("s_anchor"  , s_anchor);
   DEBUG_GRAF   yLOG_double  ("s_texbeg"  , s_texbeg);
   DEBUG_GRAF   yLOG_double  ("s_texend"  , s_texend);
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> show texture on screen ----------------[ ------ [ ------ ]-*/
TICK_show          (void)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   float       x_beg       =  0.0;
   float       x_end       =  0.0;
   float       x_left      =  0.0;
   float       x_wide      =  0.0;
   /*---(upper bar)----------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   rc = TICK_curr  ();
   if (yVIKEYS_prog_redraw ()) {
      rc = TICK_back_draw ();
      rc = TICK_draw_all ();
   }
   /*---(first two panels displayed)-----*/
   if (s_texbeg  < 0.00) {
      x_beg  = 1.00 + s_texbeg;
      x_end  = 1.00;
      x_left = 0.00;
      x_wide = ((-s_texbeg) / s_texpct) * my.p_wide;
      TICK_show_panel (p_pref, x_beg, x_end, x_left, x_wide);
      x_beg  = 0.00;
      x_end  = s_texend;
      x_left = x_wide;
      x_wide = (x_end / s_texpct) * my.p_wide;
      TICK_show_panel (p_curr, x_beg, x_end, x_left, x_wide);
   }
   /*---(second two panels displayed)----*/
   else if (s_texend > 1.00) {
      x_beg  = s_texbeg;
      x_end  = 1.00;
      x_left = 0.00;
      x_wide = ((1.00 -s_texbeg) / s_texpct) * my.p_wide;
      TICK_show_panel (p_curr, x_beg, x_end, x_left, x_wide);
      x_beg  = 0.00;
      x_end  = s_texend - 1.00;
      x_left = x_wide;
      x_wide = (x_end / s_texpct) * my.p_wide;
      TICK_show_panel (p_suff, x_beg, x_end, x_left, x_wide);
   }
   /*---(all on main panel)--------------*/
   else {
      x_beg  = s_texbeg;
      x_end  = s_texend;
      x_left = 0.00;
      x_wide = ((x_end - x_beg) / s_texpct) * my.p_wide;
      TICK_show_panel (p_curr, x_beg, x_end, x_left, x_wide);
   }
   /*---(current)------------------------*/
   glColor4f    (0.00f, 0.00f, 1.00f, 1.0f);
   glLineWidth  ( 5.0f);
   glPushMatrix(); {
      glBegin         (GL_LINES); {
         glVertex3f  (s_cur,  85.0,   239.0);
         glVertex3f  (s_cur,   0.0,   239.0);
      } glEnd   ();
   } glPopMatrix();
   /*---(drawing position)---------------*/
   if      (my.p_cur <  p_curr->beg)  p_draw = p_pref;
   else if (my.p_cur >  p_suff->beg)  p_draw = p_suff;
   else                               p_draw = p_curr;
   /*> my.p_tick = ((my.p_cur - p_draw->beg) / s_len) * MAX_WIDE;                        <*/
   my.p_tick = my.p_cur * 10.0;
   DEBUG_GRAF   yLOG_point   ("p_draw"    , p_draw);
   DEBUG_GRAF   yLOG_value   ("->sect"    , p_draw->sect);
   DEBUG_GRAF   yLOG_value   ("my.p_tick" , my.p_tick);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
TICK_snap               (void)
{
   p_curr->act = 's';
   TICK_draw_one (p_curr);
   return 0;
}

char
TICK_debug              (void)
{
   /*---(show debug)---------------------*/
   if (s_debug == 'y') {
      printf ("TICK_show ()  debugging\n");
      printf ("\n");
      printf ("   ___script_____________________\n");
      printf ("   my.p_len         = %10.3f\n", my.p_len);
      printf ("   s_anchor         = %c\n"    , s_anchor);
      printf ("   my.p_inc         = %10.3f\n", my.p_inc);
      printf ("   my.p_cur         = %10.3f\n", my.p_cur);
      printf ("   s_curp           = %10.3f\n", s_curp);
      printf ("   s_cur            = %10.3f\n", s_cur);
      printf ("\n");
      printf ("   ___vertical___________________\n");
      printf ("   s_tall           = %6d\n"   , s_tall);
      printf ("   my.p_tall        = %6d\n"   , my.p_tall);
      printf ("   max legs         = %6d\n"   , 6);
      printf ("   height per leg   = %10.3f\n", s_tall / 6.0);
      printf ("   my.p_line        = %10.3f\n", my.p_line);
      printf ("   my.p_leg         = %10.3f\n", my.p_leg);
      printf ("   s_textop         = %10.3f\n", s_textop);
      printf ("   s_texbot         = %10.3f\n", s_texbot);
      printf ("\n");
      printf ("   ___horizontal_________________\n");
      printf ("   my.p_wide        = %6d\n"   , my.p_wide);
      printf ("   s_wide           = %6d\n"   , s_wide);
      printf ("   my.p_inc         = %10.3f\n", my.p_inc);
      printf ("   s_texavail       = %10.3f\n", s_texavail);
      printf ("   s_texpct         = %10.3f\n", s_texpct);
      printf ("\n");
      printf ("   s_tsec           = %10.3f\n", s_tsec);
      /*> printf ("   s_tnsec          = %10.3f\n", s_tnsec);                         <*/
      /*> printf ("   s_tsecp          = %10.3f\n", s_tsecp);                         <*/
      printf ("   s_plenp          = %10.3f\n", s_plenp);
      printf ("\n");
      /*> printf ("   s_maxlabel       = %10.3f\n", (s_wide / 10.0) * my.p_multi);   <*/
      printf ("   s_texbeg1        = %10.3f\n", s_texbeg1);
      printf ("   s_texend1        = %10.3f\n", s_texend1);
      printf ("   s_texpct1        = %10.3f\n", s_texpct1);
      printf ("   s_texbeg2        = %10.3f\n", s_texbeg2);
      printf ("   s_texend2        = %10.3f\n", s_texend2);
      printf ("   s_texpct2        = %10.3f\n", s_texpct2);
      s_debug = '-';
   }
   return 0;
}




/*============================----end-of-source---============================*/
