/*============================----beg-of-source---============================*/

#include "arachne.h"


/*---(horizontal)---------------------*/
static int         s_wide      =   4000;        /* texture panel width        */
static int         s_xinc      =     10;        /* horizontal increment       */
static char        s_anchor    =    'c';        /* horz scroll anchor point   */
/*---(vertical)-----------------------*/
static int         s_title     =     30;        /* title at top of line       */
static int         s_top       =    115;        /* positive part of each line */
static int         s_bot       =    -85;        /* negative part of each line */
static int         s_yinc      =    200;        /* vertical size of line      */
static int         s_tall      =      0;        /* texture panel height       */
static int         s_nline     =      0;        /* number of lines            */
/*---(handles)------------------------*/
#define    MAX_PANEL      4
static uint        s_fbo       [MAX_PANEL];     /* framebuffer       */
static uint        s_depth     [MAX_PANEL];     /* depth buffer      */
static uint        s_tex       [MAX_PANEL];     /* texture array     */
static float       s_beg       [MAX_PANEL];     /* begining script position   */             
static int         s_nsec      =      0;        /* total number of sections   */             
static int         s_sec       [MAX_PANEL];     /* each panels section assign */             
static float       s_len       =    0.0;
/*---(debugging)----------------------*/
static char        s_debug     =    '-';
static char        s_snap      =    '-';


#define        UNIT2PANEL       40.0


#define     MAX_LINES      20
typedef   struct cLINES  tLINES;
struct cLINES {
   char        name        [LEN_LABEL];
   char        active;
   char        content;
   int         leg;
};
tLINES      s_line_info  [MAX_LINES] = {
   { "orientation"      , 'y' , 'o' , -1       },
   { "center offset"    , 'y' , 'c' , -1       },
   { "right rear"       , 'y' , 'l' , YKINE_RR },
   { "right middle"     , 'y' , 'l' , YKINE_RM },
   { "right front"      , 'y' , 'l' , YKINE_RF },
   { "left front"       , 'y' , 'l' , YKINE_LF },
   { "left middle"      , 'y' , 'l' , YKINE_LM },
   { "left rear"        , 'y' , 'l' , YKINE_LR },
   { "right posterior"  , '-' , 'l' , YKINE_RP },
   { "right anterior"   , '-' , 'l' , YKINE_RA },
   { "left anterior"    , '-' , 'l' , YKINE_LA },
   { "left posterior"   , '-' , 'l' , YKINE_LP },
   { "---"              , 'x' , '-' , -1       },
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

static double      s_lowest    =   0.0;

static int       s_debug_leg;
static double    s_debug_sec;


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
   s_nline     =    0;
   /*---(cycle)--------------------------*/
   for (i = 0; i < MAX_LINES; ++i) {
      if (s_line_info [i].active == 'x')  break;
      /*> if (s_line_info [i].active == '-')  continue;                                   <*/
      ++s_nline;
   }
   DEBUG_GRAF   yLOG_sint    (s_nline);
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
   for (i = 0; i < MAX_LINES; ++i) {
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
   for (x_line = 0; x_line < s_nline; ++x_line) {
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


/*> char         /+--> draw texture background ---------------[ ------ [ ------ ]-+/                  <* 
 *> TICK_back_vert     (void)                                                                         <* 
 *> {                                                                                                 <* 
 *>    /+---(locals)-----------+-----------+-+/                                                       <* 
 *>    int       x;                             /+ loop iterator                  +/                  <* 
 *>    int       j;                             /+ loop iterator                  +/                  <* 
 *>    int       x_yinc        =  0.0;                                                                <* 
 *>    float     x_bot         =  0.0;                                                                <* 
 *>    float     x_top         =  0.0;                                                                <* 
 *>    float     x_beg         =  0.0;                                                                <* 
 *>    float     x_end         = s_wide;                                                              <* 
 *>    char      x_msg         [100];                                                                 <* 
 *>    float     x_large       =  0.0;                                                                <* 
 *>    int       x_small       =    0;                                                                <* 
 *>    float     x_section     =  0.0;                                                                <* 
 *>    float     x_pct         =  0.0;                                                                <* 
 *>    /+---(prepare)------------------------+/                                                       <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                                      <* 
 *>    /+---(vertical bars)------------------+/                                                       <* 
 *>    x_yinc = s_tall / 12.0;                                                                        <* 
 *>    yVIKEYS_view_color (YCOLOR_NEG_MUT, 1.00);                                                     <* 
 *>    glLineWidth (2.0);                                                                             <* 
 *>    for (x = x_beg; x <= x_end; x += s_xinc) {                                                     <* 
 *>       /+---(10.0x bars)------------------+/                                                       <* 
 *>       if (x % (s_xinc * 100) == 0) {                                                              <* 
 *>          glBegin         (GL_POLYGON); {                                                          <* 
 *>             glVertex3f  (x - s_xinc / 3.0,   s_tall,     0.0);                                    <* 
 *>             glVertex3f  (x + s_xinc / 3.0,   s_tall,     0.0);                                    <* 
 *>             glVertex3f  (x + s_xinc / 3.0,   0.0      ,     0.0);                                 <* 
 *>             glVertex3f  (x - s_xinc / 3.0,   0.0      ,     0.0);                                 <* 
 *>          } glEnd   ();                                                                            <* 
 *>          x_small = 0;                                                                             <* 
 *>          ++x_large;                                                                               <* 
 *>          for (j = 0; j < 12; ++j) {                                                               <* 
 *>             x_top = (j + 1) * x_yinc;                                                             <* 
 *>             x_bot = (j    ) * x_yinc;                                                             <* 
 *>             if (j <  6)  x_section = s_section;                                                   <* 
 *>             else         x_section = s_section + 1;                                               <* 
 *>             x_pct = ((((x_section * 4) + x_large) * my.p_scale * 10.0) / my.p_len) * 100;         <* 
 *>             if (x_pct > 100.0)  x_pct = 100.0;                                                    <* 
 *>             if (x_pct <   0.0)  x_pct =   0.0;                                                    <* 
 *>             /+> sprintf (x_msg, "%d", ((s_section * 4) + x_large) * my.p_scale * 10);   <+/       <* 
 *>             /+> sprintf (x_msg, "%.0f", ((x_section * 4) + x_large));                 <+/         <* 
 *>             /+> sprintf (x_msg, "%.0f", ((x_section * 4) + x_large) * my.p_scale * 10.0);   <+/   <* 
 *>             sprintf (x_msg, "%3.0f%%", x_pct);                                                    <* 
 *>             /+> yVIKEYS_view_color (YCOLOR_BAS_ACC, 1.00);                            <+/         <* 
 *>             glPushMatrix(); {                                                                     <* 
 *>                glTranslatef (x + 990.0, x_bot +  50.0    ,    60.0  );                            <* 
 *>                yFONT_print  (my.fixed,  18, YF_TOPRIG, x_msg);                                    <* 
 *>             } glPopMatrix();                                                                      <* 
 *>          }                                                                                        <* 
 *>       }                                                                                           <* 
 *>    }                                                                                              <* 
 *>    /+---(complete)-----------------------+/                                                       <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                                      <* 
 *>    return 0;                                                                                      <* 
 *> }                                                                                                 <*/

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
   for (x_line = 0; x_line < s_nline; ++x_line) {
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
   DEBUG_GRAF   yLOG_value   ("s_nline"   , s_nline);
   for (x_line = 0; x_line < s_nline; ++x_line) {
      DEBUG_GRAF   yLOG_value   ("x_line"    , x_line);
      x_top = (s_nline - x_line    ) * s_yinc;
      x_bot = (s_nline - x_line - 1) * s_yinc;
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
   if (s_tex [0] != 0) {
      rc = yGLTEX_free (&s_tex [0], &s_fbo [0], &s_depth [0]);
      if (rc < 0) {
         DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
   }
   /*---(create)----------------------*/
   rc = yGLTEX_new  (&s_tex [0], &s_fbo [0], &s_depth [0], s_wide, s_tall);
   DEBUG_GRAF   yLOG_value   ("new"       , rc);
   if (rc < 0) {
      DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(draw)---------------------------*/
   DEBUG_GRAF   yLOG_note    ("draw background");
   yGLTEX_draw_start (s_fbo [0], YGLTEX_BOTLEF, s_wide, s_tall, 1.0);
   TICK_back_vert    ();
   TICK_back_horz    ();
   TICK_back_label   ();
   yGLTEX_draw_end   (s_tex [0]);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_back_copy_label    (int a_panel)
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
   yVIKEYS_view_color (YCOLOR_BAS_MED, 1.00);
   /*---(vertical lines)-----------------*/
   DEBUG_GRAF   yLOG_value   ("s_nline"   , s_nline);
   for (x_line = 0; x_line < s_nline; ++x_line) {
      DEBUG_GRAF   yLOG_value   ("x_line"    , x_line);
      x_top = (s_nline - x_line    ) * s_yinc;
      x_bot = (s_nline - x_line - 1) * s_yinc;
      x_ref = TICK_line_find (x_line);
      DEBUG_GRAF   yLOG_info    ("name"      , s_line_info [x_ref].name);
      /*---(horizontal move)-------------*/
      for (x_pos = 0; x_pos <= s_wide; x_pos += s_xinc * 100) {
         DEBUG_GRAF   yLOG_value   ("x_pos"     , x_pos);
         /*---(index)--------------------*/
         sprintf (x_msg, "%d%c", a_panel, x_pos / (s_xinc * 100) + 'a');
         glPushMatrix(); {
            glTranslatef (x_pos + 250.0, x_top - 14.0,    60.0);
            yFONT_print  (my.fixed,  16, YF_MIDCEN, x_msg);
         } glPopMatrix();
         /*---(panel)--------------------*/
         sprintf (x_msg, "%d/%d", s_sec [a_panel], s_nsec);
         glPushMatrix(); {
            glTranslatef (x_pos + 750.0, x_top - 14.0,    60.0);
            yFONT_print  (my.fixed,  16, YF_MIDCEN, x_msg);
         } glPopMatrix();
         /*---(done)---------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture background ---------------[ ------ [ ------ ]-*/
TICK_back_copy          (int a_panel)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;          /* generic return code            */
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(copy)---------------------------*/
   glBindTexture   (GL_TEXTURE_2D, s_tex [0]);
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
   double    x_sec         = 0.0;
   double    x_lowest      = 0.0;
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
   for (x_line = 0; x_line < s_nline; ++x_line) {
      x_top = (x_line + 1) * s_yinc;
      x_bot = (x_line    ) * s_yinc;
      /*---(horizontal move)-------------*/
      for (x_pos = 0; x_pos <= s_wide; x_pos += s_xinc * 100) {
      }
   }
   /*---(leg labels)---------------------*/
   for (i = x_beg; i < x_end; i += s_xinc * 100) {
      for (j = 0; j < 12; ++j) {
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
   rc = yGLTEX_init ();
   DEBUG_GRAF   yLOG_value   ("yGLTEX"    , rc);
   if (rc < 0)  return rc;
   rc = yVIKEYS_cmds_add ('c', "p_snap"      , ""    , ""     , TICK_snap            , "save png image of full ticker/progress"                      );
   DEBUG_GRAF   yLOG_value   ("p_snap"    , rc);
   if (rc < 0)  return rc;
   /*---(sizes)--------------------------*/
   DEBUG_GRAF   yLOG_note    ("setting sizes (widths and heights)");
   TICK_line_count ();
   s_tall = s_nline * s_yinc;
   DEBUG_GRAF   yLOG_value   ("s_tall"    , s_tall);
   /*---(handles)------------------------*/
   DEBUG_GRAF   yLOG_note    ("initializing handles (tex, fbo, depth)");
   for (i = 0; i < MAX_PANEL; ++i) {
      s_tex [i] = s_fbo [i] = s_depth [i] = 0;
   }
   s_beg [0] = 0.0;
   s_sec [0] =   0;
   s_beg [1] = 0.0 - s_len;
   s_sec [1] =  -1;
   s_beg [2] = 0.0;
   s_sec [2] =   0;
   s_beg [3] = 0.0 + s_len;
   s_sec [3] =   1;
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
      rc = yGLTEX_free (&s_tex [i], &s_fbo [i], &s_depth [i]);
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
/*===----                             content                          ----===*/
/*====================------------------------------------====================*/
static void      o___CONTENT_________________o (void) {;}

char
TICK_line          (float a_x1, float a_y1, float a_x2, float a_y2, float a_xinc, float a_yinc, float a_z)
{
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(top line)-----------------------*/
   glBegin(GL_LINE_STRIP); {
      glVertex3f  (a_x1 - a_xinc, a_y1 + a_yinc, a_z);
      glVertex3f  (a_x2 - a_xinc, a_y2 + a_yinc, a_z);
   } glEnd   ();
   /*---(bottom line)--------------------*/
   glBegin(GL_LINE_STRIP); {
      glVertex3f  (a_x1 + a_xinc, a_y1 - a_yinc, a_z);
      glVertex3f  (a_x2 + a_xinc, a_y2 - a_yinc, a_z);
   } glEnd   ();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TICK_servoline     (int a_panel, char a_type, float a_base, float a_bsec, float a_esec, float a_bdeg, float a_edeg, float a_unit)
{
   /*---(locals)-----------+-----+-----+-*/
   float       x_beg       =  0.0;
   float       x_end       =  0.0;
   float       y_beg       =  0.0;
   float       y_end       =  0.0;
   float       y_inc       =  0.0;
   float       x_inc       =  0.0;
   float       z_pos       =  0.0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(prepare beg)--------------------*/
   x_beg   = (a_bsec * a_unit) - s_beg [a_panel];
   if (x_beg > s_wide) {
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare end)--------------------*/
   x_end   = (a_esec * a_unit) - s_beg [a_panel];
   if (x_end <  0.0     ) {
      DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare y-pos)------------------*/
   y_beg   = a_base + a_bdeg;
   y_end   = a_base + a_edeg;
   /*---(prepare adjustments)------------*/
   switch (a_type) {
   case 'f' :   /* femur         */
      y_inc = 4;
      z_pos = 230.0;
      glColor4f    (0.50f, 0.00f, 0.00f, 1.0f);
      glLineWidth  (15.0f);
      break;
   case 'p' :   /* patella       */
      y_inc = 2;
      z_pos = 233.0;
      glColor4f    (0.20f, 0.20f, 0.70f, 1.0f);
      glLineWidth  (10.0f);
      break;
   case 't' :   /* tibia         */
      y_inc = 1;
      z_pos = 236.0;
      glColor4f    (0.60f, 0.60f, 0.00f, 1.0f);
      glLineWidth  ( 4.0f);
      break;
   case 'e' :   /* end point     */
      x_inc = 5;
      z_pos = 239.0;
      glColor4f    (0.50f, 0.00f, 0.50f, 1.0f);
      glLineWidth  (10.0f);
      break;
   case 'c' :   /* current       */
      x_inc = 0;
      z_pos = 239.0;
      glColor4f    (0.00f, 0.00f, 1.00f, 1.0f);
      glLineWidth  ( 5.0f);
      break;
   }
   /*---(draw)--------*/
   TICK_line (x_beg, y_beg, x_end, y_end, x_inc, y_inc, z_pos);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_servos        (int a_panel, int a_line)
{
   /*---(locals)-----------+-----+-----+-*/
   char      rc            =    0;               /* generic return code       */
   float     x_base        =  0.0;
   float     x_unit        =    0;
   int       x_leg         =    0;
   double    x_bsec        =    0;
   double    x_bdeg        =    0;
   double    x_esec        =    0;
   double    x_edeg        =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   x_leg     = s_line_info [a_line].leg;
   x_base    = (s_tall - (s_yinc * a_line)) - s_top;
   x_unit    = s_xinc / my.p_inc;
   /*---(femur)--------------------------*/
   glPushMatrix(); {
      rc = yKINE_move_first (x_leg, YKINE_FEMU, &x_bsec, &x_bdeg);
      while (rc >= 0) {
         /*---(read next)---*/
         rc = yKINE_move_next  (&x_esec, &x_edeg);
         if (rc <  0) break;
         /*---(fix points)--*/
         TICK_servoline     (a_panel, 'f', x_base, x_bsec, x_esec, x_bdeg, x_edeg, x_unit);
         /*---(save)--------*/
         x_bsec = x_esec;
         x_bdeg = x_edeg;
         /*---(done)--------*/
      }
   } glPopMatrix();
   /*---(patella)------------------------*/
   glPushMatrix(); {
      rc = yKINE_move_first (x_leg, YKINE_PATE, &x_bsec, &x_bdeg);
      while (rc >= 0) {
         /*---(read next)---*/
         rc = yKINE_move_next  (&x_esec, &x_edeg);
         if (rc <  0) break;
         /*---(fix points)--*/
         TICK_servoline     (a_panel, 'p', x_base, x_bsec, x_esec, x_bdeg, x_edeg, x_unit);
         /*---(save)--------*/
         x_bsec = x_esec;
         x_bdeg = x_edeg;
         /*---(done)--------*/
      }
   } glPopMatrix();
   /*---(tibia)--------------------------*/
   glPushMatrix(); {
      rc = yKINE_move_first (x_leg, YKINE_TIBI, &x_bsec, &x_bdeg);
      while (rc >= 0) {
         /*---(read next)---*/
         rc = yKINE_move_next  (&x_esec, &x_edeg);
         if (rc <  0) break;
         /*---(fix points)--*/
         TICK_servoline     (a_panel, 't', x_base, x_bsec, x_esec, x_bdeg, x_edeg, x_unit);
         /*---(save)--------*/
         x_bsec = x_esec;
         x_bdeg = x_edeg;
         /*---(done)--------*/
      }
   } glPopMatrix();
   /*---(draw end)-----------------------*/
   glPushMatrix(); {
      TICK_servoline     (a_panel, 'e', x_base, my.p_len, my.p_len, s_top - 30.0, s_bot, x_unit);
   } glPopMatrix();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

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

static char  /*--> represent tibia placement accuracy ----[ ------ [ ------ ]-*/
TICK__heat_spot    (char a_type, char a_rc, double a_diff, double a_x, double *a_y)
{
   double      x_diff      =   1.00;
   double      x_base      =   1.00;
   double      x_width     =   8.00;
   double      x_xoff      =   1.00;
   double      x_height    =   8.00;
   double      x_yoff      =  12.00;
   double      x_alpha     =   0.50;
   double      x_z         = 180.00;
   double      x_red, x_grn, x_blu;
   switch (a_type) {
   case 't' :  /* touch      (xzy) */
      x_base    *= 1.0;
      x_height  *= 2.0;
      break;
   case 'x' : case 'z' :
      x_diff     = -(fabs (a_diff));
      break;
   case 'h' :  /* horizontal (xz)  */
      x_diff     = -(fabs (a_diff));
      x_base    *= 1.0;
      /*> x_base    *= 2.0;                                                           <*/
      x_height  *= 3.0;
      x_yoff    *= 3.0;
      break;
   case 'y' :
      x_diff     = -(fabs (a_diff));
      x_height  *= 2.0;
      break;
   case 'f' :  /* full       (xzy) */
      x_diff     = -(fabs (a_diff));
      x_base    *= 2.0;
      x_height  *= 3.0;
      break;
   default  : return -1;
              break;
   }
   if      (a_rc   <   0 )  yCOLOR_val2error  (0.8);
   else                     yCOLOR_val2color  (x_base * a_diff, 0.8);
   glPushMatrix(); {
      glBegin         (GL_POLYGON); {
         glVertex3f  (a_x + x_xoff          , *a_y           , x_z);
         glVertex3f  (a_x + x_xoff + x_width, *a_y           , x_z);
         glVertex3f  (a_x + x_xoff + x_width, *a_y - x_height, x_z);
         glVertex3f  (a_x + x_xoff          , *a_y - x_height, x_z);
      } glEnd   ();
   } glPopMatrix();
   *a_y -= (x_height + x_yoff);
   return 0;
}

static char  /*--> represent tibia placement accuracy ----[ ------ [ ------ ]-*/
TICK__heat_column    (int a_leg, double a_sec, double a_x, double a_y)
{
   /*---(locals)-----------+-----------+-*/
   double    x_xdif        = 0.0;
   double    x_zdif        = 0.0;
   double    x_ydif        = 0.0;
   double    x_ypos        = 0.0;
   double    x_xz          =  0.0;
   double    x_full        =  0.0;
   char      rc            = 0;
   /*---(tibia)--------------------------*/
   rc     = yKINE_move_exact (a_sec, a_leg, &x_xdif, &x_zdif, &x_ydif, &x_ypos);
   x_xz   = sqrt ((x_xdif * x_xdif) + (x_zdif * x_zdif));
   x_full = sqrt ((x_xdif * x_xdif) + (x_zdif * x_zdif) + (x_ydif * x_ydif));
   /*> if (a_leg == 1 && a_sec < 20.0)  printf ("TICK__heat_column, a_leg=%d, a_sec=%8.3lf, rc=%3d, s_lowest=%8.1lf, x_ypos=%8.1lf\n", a_leg, a_sec, rc, s_lowest, x_ypos);   <*/
   s_debug_leg = a_leg;
   s_debug_sec = a_sec;
   TICK__heat_spot    ('t', rc, s_lowest - x_ypos   , a_x, &a_y);
   TICK__heat_spot    ('x', rc, x_xdif              , a_x, &a_y);
   TICK__heat_spot    ('z', rc, x_zdif              , a_x, &a_y);
   TICK__heat_spot    ('h', rc, x_xz                , a_x, &a_y);
   TICK__heat_spot    ('y', rc, x_ydif              , a_x, &a_y);
   TICK__heat_spot    ('f', rc, x_full              , a_x, &a_y);
   /*---(complete)-----------------------*/
   return 0;
}

char
TICK_height        (int a_sec, double a_pos)
{
   double    x_barrange    = 0.0;
   double    x_bartop      = 0.0;
   double    x_barbot      = 0.0;
   x_barrange = s_lowest + 100.0;
   if (x_barrange >= 0.0) {
      glColor4f    (1.00f, 1.00f, 1.00f, 0.3f);
      x_bartop = -155.0 + x_barrange;
      x_barbot = -165.0;
   } else {
      glColor4f    (0.00f, 1.00f, 0.00f, 0.3f);
      x_bartop = -110.0;
      x_barbot = -120.0 + x_barrange;
   }
   glPushMatrix(); {
      glBegin         (GL_POLYGON); {
         glVertex3f  (a_sec + 2.0      , a_pos + x_bartop, 120.0);
         glVertex3f  (a_sec + 2.0 + 6.0, a_pos + x_bartop, 120.0);
         glVertex3f  (a_sec + 2.0 + 6.0, a_pos + x_barbot, 120.0);
         glVertex3f  (a_sec + 2.0      , a_pos + x_barbot, 120.0);
      } glEnd   ();
   } glPopMatrix();
   return 0;
}

/*> char         /+--> draw texture labels -------------------[ ------ [ ------ ]-+/                                                       <* 
 *> TICK_labels        (void)                                                                                                              <* 
 *> {                                                                                                                                      <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                                            <* 
 *>    int       i;                             /+ loop iterator                  +/                                                       <* 
 *>    int       j;                             /+ loop iterator                  +/                                                       <* 
 *>    float     x_yinc        =  0.0;                                                                                                     <* 
 *>    float     x_bot         =  0.0;                                                                                                     <* 
 *>    float     x_top         = s_tall;                                                                                                <* 
 *>    float     x_beg         =  0.0;                                                                                                     <* 
 *>    float     x_end         = s_wide;                                                                                                <* 
 *>    float     x_bar         =  0.0;                                                                                                     <* 
 *>    float     x_pos         =  0.0;                                                                                                     <* 
 *>    char      x_msg         [100];                                                                                                      <* 
 *>    char      x_part        [100];                                                                                                      <* 
 *>    int       x_labelper    =    0;                                                                                                     <* 
 *>    int       x_secbeg1     =    0;                                                                                                     <* 
 *>    int       x_secbeg2     =    0;                                                                                                     <* 
 *>    char      x_label1      [10];                                                                                                       <* 
 *>    char      x_label2      [10];                                                                                                       <* 
 *>    double    x_sec         = 0.0;                                                                                                      <* 
 *>    double    x_lowest      = 0.0;                                                                                                      <* 
 *>    int       x_lowcnt      = 0;                                                                                                        <* 
 *>    char      rc            = 0;                                                                                                        <* 
 *>    /+---(prepare)------------------------+/                                                                                            <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                                                                           <* 
 *>    yVIKEYS_scale_base (&my.p_multi, &my.p_base);                                                                                       <* 
 *>    x_yinc      = x_top / 12.0;                                                                                                         <* 
 *>    x_bar       = s_top - s_bot;                                                                                                  <* 
 *>    x_labelper  = (s_wide / 10.0) * my.p_multi;                                                                                      <* 
 *>    /+> printf ("%8d  %8.3lf\n", s_wide, my.p_multi);                               <+/                                              <* 
 *>    x_secbeg1   = x_labelper * s_section;                                                                                               <* 
 *>    x_secbeg2   = x_secbeg1 + x_labelper;                                                                                               <* 
 *>    strlcpy (x_label1, TICK_sectext (s_section    ), 10);                                                                               <* 
 *>    strlcpy (x_label2, TICK_sectext (s_section + 1), 10);                                                                               <* 
 *>    /+> printf ("%8d  %8d  %8d\n", x_labelper, x_secbeg1, x_secbeg2);                  <+/                                              <* 
 *>    /+---(leg labels)---------------------+/                                                                                            <* 
 *>    for (i = x_beg; i < x_end; i += s_xinc * 100) {                                                                                     <* 
 *>       for (j = 0; j < 12; ++j) {                                                                                                       <* 
 *>          x_pos = s_tall - (j * x_yinc);                                                                                             <* 
 *>          sprintf (x_msg, "%d/%s", j % 6, legs_name [j % 6]);                                                                           <* 
 *>          glColor4f    (0.50f, 0.50f, 0.50f, 1.0f);                                                                                     <* 
 *>          glPushMatrix(); {                                                                                                             <* 
 *>             glTranslatef ( i + 30.0 , x_pos    -  125.0    ,    60.0  );                                                               <* 
 *>             glRotatef  ( 90.0  , 0.0f, 0.0f, 1.0f);                                                                                    <* 
 *>             yFONT_print  (my.fixed,  40, YF_MIDCEN, x_msg);                                                                            <* 
 *>          } glPopMatrix();                                                                                                              <* 
 *>          if (j >= 6)  strlcpy (x_msg, x_label1, LEN_STR);                                                                              <* 
 *>          else         strlcpy (x_msg, x_label2, LEN_STR);                                                                              <* 
 *>          glColor4f    (0.25f, 0.25f, 0.25f, 1.0f);                                                                                     <* 
 *>          glPushMatrix(); {                                                                                                             <* 
 *>             glTranslatef ( i +  80.0 , x_pos    -   40.0    ,    60.0  );                                                              <* 
 *>             yFONT_print  (my.fixed,  40, YF_MIDCEN, x_msg);                                                                            <* 
 *>          } glPopMatrix();                                                                                                              <* 
 *>          yVIKEYS_scale_desc (x_msg);                                                                                                   <* 
 *>          glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                                                                     <* 
 *>          glPushMatrix(); {                                                                                                             <* 
 *>             glTranslatef ( i + 10.0 , x_pos    -  25.0 ,    60.0  );                                                                   <* 
 *>             yFONT_print  (my.fixed,  16, YF_BOTLEF, x_msg);                                                                            <* 
 *>          } glPopMatrix();                                                                                                              <* 
 *>          yVIKEYS_speed_desc (x_msg);                                                                                                   <* 
 *>          glPushMatrix(); {                                                                                                             <* 
 *>             glTranslatef ( i - 500.0, x_pos    -  25.0 ,    60.0  );                                                                   <* 
 *>             yFONT_print  (my.fixed,  16, YF_BOTLEF, x_msg);                                                                            <* 
 *>          } glPopMatrix();                                                                                                              <* 
 *>       }                                                                                                                                <* 
 *>    }                                                                                                                                   <* 
 *>    /+---(scale labels)-------------------+/                                                                                            <* 
 *>    glPushMatrix(); {                                                                                                                   <* 
 *>       glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                                                                        <* 
 *>       for (i = 0; i < s_wide; i += s_xinc) {                                                                                        <* 
*>          /+---(labels)-------------------+/                                                                                            <* 
*>          if (i % (int) (s_xinc * 10) == 0) {                                                                                           <* 
   *>             for (j = 0; j < 12; ++j) {                                                                                                 <* 
      *>                glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                                                               <* 
         *>                if (j < 6)  snprintf     (x_msg, 50, "%d%c"  , (int) ((i / s_xinc) * my.p_multi) + x_secbeg1, my.p_base);               <* 
         *>                else        snprintf     (x_msg, 50, "%d%c"  , (int) ((i / s_xinc) * my.p_multi) + x_secbeg2, my.p_base);               <* 
         *>                x_pos = (j + 1) * x_yinc;                                                                                               <* 
         *>                glPushMatrix(); {                                                                                                       <* 
            *>                   glTranslatef ( i , x_pos - x_bar + 25.0 ,    60.0  );                                                                <* 
               *>                   yFONT_print  (my.fixed,  14, YF_TOPLEF, x_msg);                                                                      <* 
               *>                } glPopMatrix();                                                                                                        <* 
               *>             }                                                                                                                          <* 
               *>          }                                                                                                                             <* 
               *>          /+---(accuracy heatmap)---------+/                                                                                            <* 
               *>          for (j = 0; j < 12; ++j) {                                                                                                    <* 
                  *>             if (j < 6) {                                                                                                               <* 
                     *>                x_pos = (6 - j) * x_yinc;                                                                                               <* 
                        *>                x_sec = (((float) (i) / s_xinc) * my.p_multi) + x_secbeg1;                                                              <* 
                        *>                if (j == 0) {                                                                                                           <* 
                           *>                   rc = yKINE_phys_flat   (YKINE_FK, x_sec, &s_lowest, &x_lowcnt);                                                      <* 
                              *>                   /+> printf ("i=%5d, j=%2d, x_sec=%8.3lf, s_lowest=%8.1lf, x_lowcnt=%2d\n", i, j, x_sec, s_lowest, x_lowcnt);   <+/   <* 
                              *>                }                                                                                                                       <* 
                              *>                /+> TICK_height   (i, x_pos);                                          <+/                                              <* 
                              *>                if (x_sec >= 0.0 && x_sec <= my.p_len)  TICK__heat_column (j    , x_sec, i, x_pos - 35.0);                              <* 
                              *>             } else {                                                                                                                   <* 
                                 *>                x_pos = (12 + (6 - j)) * x_yinc;                                                                                        <* 
                                    *>                x_sec = (((float) (i) / s_xinc) * my.p_multi) + x_secbeg2;                                                              <* 
                                    *>                if (j == 6) {                                                                                                           <* 
                                       *>                   yKINE_phys_flat   (YKINE_FK, x_sec, &s_lowest, &x_lowcnt);                                                           <* 
                                          *>                   /+> printf ("i=%5d, j=%2d, x_sec=%8.3lf, s_lowest=%8.1lf, x_lowcnt=%2d\n", i, j, x_sec, s_lowest, x_lowcnt);   <+/   <* 
                                          *>                }                                                                                                                       <* 
                                          *>                /+> TICK_height   (i, x_pos);                                          <+/                                              <* 
                                          *>                if (x_sec >= 0.0 && x_sec <= my.p_len)  TICK__heat_column (j - 6, x_sec, i, x_pos - 35.0);                              <* 
                                          *>             }                                                                                                                          <* 
                                          *>          }                                                                                                                             <* 
                                          *>       }                                                                                                                                <* 
                                          *>    } glPopMatrix();                                                                                                                    <* 
                                          *>    /+---(complete)-----------------------+/                                                                                            <* 
                                          *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                                                                           <* 
                                          *>    return 0;                                                                                                                           <* 
                                          *> }                                                                                                                                      <*/

char
TICK_globals       (void)
{
   /*---(locals)-----------+-----------+-*/
   float       x_inc       =    10.0;
   float       y_inc       =     0.0;
   /*---(set single leg vars)------------*/
   yVIKEYS_view_coords   (YVIKEYS_PROGRESS, NULL, &my.p_wide, NULL, &my.p_tall);
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   y_inc       = 1.0 / 12.0;
   s_textop    = 0.5 - ((my.p_line + 0) * y_inc);
   s_texbot    = 0.5 - ((my.p_line + 1) * y_inc);
   /*---(calc basics)--------------------*/
   s_texavail  = my.p_wide * 2.0;
   s_texpct    = s_texavail / s_wide;
   s_start     = s_section * s_wide;
   /*---(seconds)------------------------*/
   s_tsec      = x_inc / my.p_inc;
   /*> s_tnsec     = s_texavail / s_tsec;                                             <*/
   /*> s_tsecp     = s_texpct / s_tnsec;                                              <*/
   s_plenp     = ((my.p_len * s_tsec) - s_start) / s_wide;
   if (s_plenp > 2.0)  s_plenp = 2.0;
   if (s_plenp < 0.0)  s_plenp = 0.0;
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture for progress ticker ------[ ------ [ ------ ]-*/
TICK_draw_one      (int a_panel)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i;                             /* loop iterator                  */
   int         x, y;
   int         x_ref       =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_value   ("a_panel"   , a_panel);
   /*> TICK_globals ();                                                               <*/
   /*---(free)------------------------*/
   if (s_tex [a_panel] != 0) {
      rc = yGLTEX_free (&s_tex [a_panel], &s_fbo [a_panel], &s_depth [a_panel]);
      if (rc < 0) {
         DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
   }
   /*---(create)----------------------*/
   rc = yGLTEX_new  (&s_tex [a_panel], &s_fbo [a_panel], &s_depth [a_panel], s_wide, s_tall);
   DEBUG_GRAF   yLOG_value   ("new"       , rc);
   if (rc < 0) {
      DEBUG_GRAF   yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   DEBUG_GRAF   yLOG_value   ("tex"       , s_tex [a_panel]);
   DEBUG_GRAF   yLOG_value   ("fbo"       , s_fbo [a_panel]);
   DEBUG_GRAF   yLOG_value   ("s_wide"    , s_wide);
   DEBUG_GRAF   yLOG_value   ("s_tall"    , s_tall);
   /*---(draw)------------------------*/
   yGLTEX_draw_start (s_fbo [a_panel], YGLTEX_BOTLEF, s_wide, s_tall, 1.0);
   TICK_back_copy    (a_panel);
   for (i = 0; i < s_nline; ++i) {
      DEBUG_GRAF   yLOG_value   ("line"      , i);
      x_ref = TICK_line_find (i);
      DEBUG_GRAF   yLOG_value   ("ref"       , x_ref);
      DEBUG_GRAF   yLOG_char    ("content"   , s_line_info [x_ref].content);
      switch (s_line_info [x_ref].content) {
      case 'o' : break;
      case 'c' : break;
      case 'l' : TICK_servos  (a_panel, x_ref);   break;
      }
   }
   if (s_snap == 'y') {
      yGLTEX_tex2png    ("ticker.png", s_wide, s_tall);
      s_snap = '-';
   }
   yGLTEX_draw_end   (s_tex [a_panel]);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TICK_draw_all           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   double      x_pos       =  0.0;
   double      x_cur       =  0.0;
   int         x_section   =    0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_double  ("s_len"     , s_len);
   rc = TICK_curr  ();
   /*---(review sections)----------------*/
   for (x_pos = 0.0; x_pos <= my.p_len; x_pos += s_len) {
      DEBUG_GRAF   yLOG_double  ("p_pos"     , my.p_pos);
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
   s_beg [0] = 0.0;
   s_beg [1] = x_cur - s_len;
   s_sec [1] = x_section - 1;
   s_beg [2] = x_cur;
   s_sec [2] = x_section;
   s_beg [3] = x_cur + s_len;
   s_sec [3] = x_section + 1;
   /*---(re-create)----------------------*/
   TICK_draw_one (1);
   TICK_draw_one (2);
   TICK_draw_one (3);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TICK_snap               (void)
{
   s_snap = 'y';
   return 0;
}

char
TICK_rotate_earlier     (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   int         i           =    0;
   uint        x_fbo       =    0;
   uint        x_depth     =    0;
   uint        x_tex       =    0;
   float       x_beg       =  0.0;
   int         x_sec       =    0;
   /*---(save later into temp)-----------*/
   x_fbo       = s_fbo   [3];
   x_depth     = s_depth [3];
   x_tex       = s_tex   [3];
   x_beg       = s_beg   [3];
   x_sec       = s_sec   [3];
   /*---(move later into temp)-----------*/
   for (i = 3; i >= 2; --i) {
      s_fbo   [i] = s_fbo   [i - 1];
      s_depth [i] = s_depth [i - 1];
      s_tex   [i] = s_tex   [i - 1];
      s_beg   [i] = s_beg   [i - 1];
      s_sec   [i] = s_sec   [i - 1];
   }
   /*---(put temp into earliest)---------*/
   s_fbo   [1] = x_fbo;
   s_depth [1] = x_depth;
   s_tex   [1] = x_tex;
   s_beg   [1] = x_beg - s_len;
   s_sec   [1] = x_sec - 1;
   /*---(draw earliest)------------------*/
   rc = TICK_draw_one (1);
   /*---(complete)-----------------------*/
   return 0;
}

char
TICK_rotate_later       (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   int         i           =    0;
   uint        x_fbo       =    0;
   uint        x_depth     =    0;
   uint        x_tex       =    0;
   float       x_beg       =  0.0;
   int         x_sec       =    0;
   /*---(save later into temp)-----------*/
   x_fbo       = s_fbo   [1];
   x_depth     = s_depth [1];
   x_tex       = s_tex   [1];
   x_beg       = s_beg   [1];
   x_sec       = s_sec   [1];
   /*---(save later into temp)-----------*/
   for (i = 2; i <= 3; ++i) {
      s_fbo   [i - 1] = s_fbo   [i];
      s_depth [i - 1] = s_depth [i];
      s_tex   [i - 1] = s_tex   [i];
      s_beg   [i - 1] = s_beg   [i];
      s_sec   [i - 1] = s_sec   [i];
   }
   /*---(put temp into earliest)---------*/
   s_fbo   [3] = x_fbo;
   s_depth [3] = x_depth;
   s_tex   [3] = x_tex;
   s_beg   [3] = x_beg + s_len;
   s_sec   [3] = x_sec + 1;
   /*---(draw earliest)------------------*/
   rc = TICK_draw_one (3);
   /*---(complete)-----------------------*/
   return 0;
}

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
   rc  = yVIKEYS_view_coords   (YVIKEYS_PROGRESS, NULL, &my.p_wide, NULL, &my.p_tall);
   rc  = yVIKEYS_prog_cur (&s_anchor, &my.p_cur, &my.p_scale, &my.p_inc, &my.p_line);
   my.p_leg = my.p_line - 2;
   /*---(secs per panel)-----------------*/
   DEBUG_GRAF   yLOG_double  ("p_cur"     , my.p_cur);
   DEBUG_GRAF   yLOG_double  ("p_scale"   , my.p_scale);
   s_len       = my.p_scale * UNIT2PANEL;
   /*---(check rotation)-----------------*/
   if (my.p_cur <  s_beg [2])  TICK_rotate_earlier ();
   if (my.p_cur >= s_beg [3])  TICK_rotate_later   ();
   /*---(tex measures)-------------------*/
   s_textop    = ((12.0 - my.p_line    ) * s_yinc) / s_tall;
   DEBUG_GRAF   yLOG_double  ("s_textop"  , s_textop);
   s_texbot    = ((12.0 - my.p_line - 1) * s_yinc) / s_tall;
   DEBUG_GRAF   yLOG_double  ("s_texbot"  , s_texbot);
   s_texavail  = my.p_wide * 2.0;
   DEBUG_GRAF   yLOG_double  ("s_texavail", s_texavail);
   s_texpct    = s_texavail / s_wide;
   DEBUG_GRAF   yLOG_double  ("s_texpct"  , s_texpct);
   /*---(current horizontal pos)---------*/
   s_curp      = (my.p_cur - s_beg [2]) / s_len;
   DEBUG_GRAF   yLOG_double  ("s_curp"    , s_curp);
   TICK_anchor ();
   DEBUG_GRAF   yLOG_char    ("s_anchor"  , s_anchor);
   DEBUG_GRAF   yLOG_double  ("s_texbeg"  , s_texbeg);
   DEBUG_GRAF   yLOG_double  ("s_texend"  , s_texend);
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TICK_show_panel         (int a_panel, double a_beg, double a_end, double a_left, double a_wide)
{
   DEBUG_GRAF   yLOG_senter  (__FUNCTION__);
   DEBUG_GRAF   yLOG_sint    (a_panel);
   DEBUG_GRAF   yLOG_sdouble (a_beg);
   DEBUG_GRAF   yLOG_sdouble (a_end);
   DEBUG_GRAF   yLOG_sdouble (a_left);
   DEBUG_GRAF   yLOG_sdouble (a_wide);
   glBindTexture   (GL_TEXTURE_2D, s_tex [a_panel]);
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

char         /*--> show texture on screen ----------------[ ------ [ ------ ]-*/
TICK_show          (void)
{
   /*---(locals)-------------------------*/
   char        rc          = 0;
   /*---(setup view)---------------------*/
   /*> glViewport      (my.p_left, my.p_bott, my.p_wide, my.p_tall);                  <* 
    *> glMatrixMode    (GL_PROJECTION);                                               <* 
    *> glLoadIdentity  ();                                                            <* 
    *> glOrtho         ( 0.0f, my.p_wide, 0.0 , my.p_tall,  -500.0,   500.0);         <* 
    *> glMatrixMode    (GL_MODELVIEW);                                                <*/
   int         x           =    0;
   int         y           =    0;
   char        t           [LEN_LABEL] = "";
   double      x_beg       =  0.0;
   double      x_end       =  0.0;
   double      x_left      =  0.0;
   double      x_wide      =  0.0;
   /*---(upper bar)----------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   rc = TICK_curr  ();
   if (yVIKEYS_prog_redraw ())  rc = TICK_draw_all ();
   /*---(first two panels displayed)-----*/
   if (s_texbeg  < 0.00) {
      x_beg  = 1.00 + s_texbeg;
      x_end  = 1.00;
      x_left = 0.00;
      x_wide = ((-s_texbeg) / s_texpct) * my.p_wide;
      TICK_show_panel (1, x_beg, x_end, x_left, x_wide);
      x_beg  = 0.00;
      x_end  = s_texend;
      x_left = x_wide;
      x_wide = (x_end / s_texpct) * my.p_wide;
      TICK_show_panel (2, x_beg, x_end, x_left, x_wide);
   }
   /*---(second two panels displayed)----*/
   else if (s_texend > 100.00) {
      x_beg  = s_texbeg;
      x_end  = 1.00;
      x_left = 0.00;
      x_wide = ((1.00 -s_texbeg) / s_texpct) * my.p_wide;
      TICK_show_panel (2, x_beg, x_end, x_left, x_wide);
      x_beg  = 0.00;
      x_end  = 1.00 - s_texend;
      x_left = x_wide;
      x_wide = (x_end / s_texpct) * my.p_wide;
      TICK_show_panel (3, x_beg, x_end, x_left, x_wide);
   }
   /*---(all on main panel)--------------*/
   else {
      x_beg  = s_texbeg;
      x_end  = s_texend;
      x_left = 0.00;
      x_wide = ((x_end - x_beg) / s_texpct) * my.p_wide;
      TICK_show_panel (2, x_beg, x_end, x_left, x_wide);
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
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return rc;
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
