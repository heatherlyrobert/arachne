/*============================----beg-of-source---============================*/

#include "arachne.h"





/*====================------------------------------------====================*/
/*===----                       progress ticker                        ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRESS________________o (void) {;}

/*---(single leg vars)----------------*/
static float       s_textop    =   0.0;    /* single leg top percent in texture   */
static float       s_texbot    =   0.0;    /* single leg bot percent in texture   */
static float       s_texavail  =   0.0;         /* texture length available       */
static float       s_texpct    =   0.0;         /* texture width percent avail    */
static float       s_texctr    =   0.0;         /* texture width avail center     */
static float       s_tsec      =   0.0;         /* texture length of a second     */
/*> static float       s_tnsec     =   0.0;         /+ texture number of secs shown   +/   <*/
/*> static float       s_tsecp     =   0.0;         /+ width pct of a texture sec     +/   <*/
static float       s_plenp     =   0.0;         /* total length in texture pct    */
static float       s_texbeg    =   0.0;
static float       s_texend    =   0.0;
static float       s_curp      =   0.0;         /* cur pos in texture pct         */
static float       s_cur       =   0.0;

static float       s_start     =   0.0;
static float       s_cutmid    =   0.0;
static float       s_cutend    =   0.0;

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


char         /*--> set values for progress ticker --------[ ------ [ ------ ]-*/
TICK_init          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(sizes)--------------------------*/
   DEBUG_GRAF   yLOG_note    ("setting sizes (widths and heights)");
   my.p_texw      = 4000;   /* max allowed single dimension */
   my.p_texh      = 2400;   /* 200 for one row  (200 * 12)  */
   my.p_top       =  115;   /* tibia can go to 130 */
   my.p_bot       =  -85;   /* femur can go to -85 */
   my.p_pos       =  'c';   /* put current bar in middle if possible          */
   /*---(handles)------------------------*/
   DEBUG_GRAF   yLOG_note    ("initializing handles (tex, fbo, depth)");
   my.p_tex       =    0;
   my.p_fbo       =    0;
   my.p_depth     =    0;
   /*---(working)------------------------*/
   DEBUG_GRAF   yLOG_note    ("initializing working variables");
   /*---(debugging)----------------------*/
   my.p_debug     =  'y';
   /*---(generate)-----------------------*/
   rc = yGLTEX_init ();
   DEBUG_GRAF   yLOG_value   ("init"      , rc);
   rc = yGLTEX_new  (&my.p_tex, &my.p_fbo, &my.p_depth, my.p_texw, my.p_texh);
   DEBUG_GRAF   yLOG_value   ("new"       , rc);
   /*---(update colors)------------------*/
   /*> s_ncolor = yCOLOR_init     (YCOLOR_BROMAU2);                                   <* 
    *> yCOLOR_scale    (YCOLOR_PARABOLIC,  0.0,  30.0);                               <*/
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

/*> char         /+--> set-up drawing window -----------------[ ------ [ ------ ]-+/   <* 
 *> TICK_start         (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                        <* 
 *>    double    x_bot         = 0.0;                                                  <* 
 *>    double    x_top         = my.p_texh;                                            <* 
 *>    double    x_beg         = 0.0;                                                  <* 
 *>    double    x_end         = my.p_texw;                                            <* 
 *>    /+---(setup)--------------------------+/                                        <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    glViewport            (0.0,  0.0, x_end, x_top);                                <* 
 *>    glMatrixMode          (GL_PROJECTION);                                          <* 
 *>    glLoadIdentity        ();                                                       <* 
 *>    glOrtho               (0.0, x_end, 0.0, x_top, -500.0,  500.0);                 <* 
 *>    glMatrixMode          (GL_MODELVIEW);                                           <* 
 *>    glBindTexture         (GL_TEXTURE_2D, 0);                                       <* 
 *>    glBindFramebufferEXT  (GL_FRAMEBUFFER_EXT,  my.p_fbo);                          <* 
 *>    glClear               (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);              <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char         /+--> complete texture ----------------------[ ------ [ ------ ]-+/   <* 
 *> TICK_end           (void)                                                          <* 
 *> {                                                                                  <* 
 *>    DEBUG_GRAF   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    /+---(create mipmaps)-----------------+/                                        <* 
 *>    glBindFramebufferEXT  (GL_FRAMEBUFFER_EXT, 0);                                  <* 
 *>    glBindTexture         (GL_TEXTURE_2D, my.p_tex);                                <* 
 *>    glGenerateMipmapEXT   (GL_TEXTURE_2D);                                          <* 
 *>    glBindTexture         (GL_TEXTURE_2D, 0);                                       <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_GRAF   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

char         /*--> draw texture background ---------------[ ------ [ ------ ]-*/
TICK_back_vert     (void)
{
   /*---(locals)-----------+-----------+-*/
   int       x;                             /* loop iterator                  */
   int       j;                             /* loop iterator                  */
   int       x_xinc        = 10.0;
   int       x_yinc        =  0.0;
   float     x_bot         =  0.0;
   float     x_top         =  0.0;
   float     x_beg         =  0.0;
   float     x_end         = my.p_texw;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(vertical bars)------------------*/
   x_yinc = my.p_texh / 12.0;
   yVIKEYS_view_color (YCOLOR_NEG_MUT, 1.00);
   glLineWidth (2.0);
   for (x = x_beg; x <= x_end; x += x_xinc) {
      if (x % (x_xinc * 100) == 0) {
         glBegin         (GL_POLYGON); {
            glVertex3f  (x - x_xinc / 3.0,   my.p_texh,     0.0);
            glVertex3f  (x + x_xinc / 3.0,   my.p_texh,     0.0);
            glVertex3f  (x + x_xinc / 3.0,   0.0      ,     0.0);
            glVertex3f  (x - x_xinc / 3.0,   0.0      ,     0.0);
         } glEnd   ();
      } else if (x % (x_xinc *  10) == 0) {
         glBegin         (GL_POLYGON); {
            glVertex3f  (x - 1     ,   my.p_texh,     0.0);
            glVertex3f  (x + 1     ,   my.p_texh,     0.0);
            glVertex3f  (x + 1     ,   0.0      ,     0.0);
            glVertex3f  (x - 1     ,   0.0      ,     0.0);
         } glEnd   ();
      } else if (x % (x_xinc *   5) == 0) {
         /*---(top and bottom limits)----------*/
         for (j = 0; j < 12; ++j) {
            x_top = (j + 1) * x_yinc;
            x_bot = (j    ) * x_yinc;
            glBegin         (GL_LINES); {
               glVertex3f  (x, x_top     ,     0.0);
               glVertex3f  (x, x_top - 50,     0.0);
               glVertex3f  (x, x_bot     ,     0.0);
               glVertex3f  (x, x_bot + 20,     0.0);
            } glEnd   ();
         }
      } else {
         /*---(top and bottom limits)----------*/
         for (j = 0; j < 12; ++j) {
            x_top = (j + 1) * x_yinc;
            x_bot = (j    ) * x_yinc;
            glBegin         (GL_LINES); {
               glVertex3f  (x, x_top     ,     0.0);
               glVertex3f  (x, x_top - 40,     0.0);
               glVertex3f  (x, x_bot     ,     0.0);
               glVertex3f  (x, x_bot + 10,     0.0);
            } glEnd   ();
         }
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
   int       i;                             /* loop iterator                  */
   int       x_yinc        =  0.0;
   float     x_ypos        =  0.0;
   float     x_top         = my.p_texh;
   float     x_beg         =  0.0;
   float     x_end         = my.p_texw;
   char      x_msg         [100];
   x_yinc = x_top / 12.0;
   /*> x_bar  = my.p_top - my.p_bot;                                                  <*/
   /*---(top and bottom limits)----------*/
   for (i = 0; i < 12; ++i) {
      x_ypos = (i + 1) * x_yinc;
      /*> if (yVIKEYS_mode_curr () == MODE_PROGRESS && (5 - i == my.p_leg || 11 - i == my.p_leg))   <* 
       *>    glColor4f    (0.25f, 0.00f, 0.00f, 1.0f);                                              <* 
       *> else                                                                                      <*/
      yVIKEYS_view_color (YCOLOR_NEG_MUT, 1.00);
      glBegin         (GL_POLYGON); {
         glVertex3f  (x_beg, x_ypos             ,   10.0);
         glVertex3f  (x_end, x_ypos             ,   10.0);
         glVertex3f  (x_end, x_ypos - 30        ,   10.0);
         glVertex3f  (x_beg, x_ypos - 30        ,   10.0);
      } glEnd   ();
      glBegin         (GL_POLYGON); {
         glVertex3f  (x_beg, x_ypos - 110       ,   10.0);
         glVertex3f  (x_end, x_ypos - 110       ,   10.0);
         glVertex3f  (x_end, x_ypos - 120       ,   10.0);
         glVertex3f  (x_beg, x_ypos - 120       ,   10.0);
      } glEnd   ();
   }
}

char         /*--> draw texture background ---------------[ ------ [ ------ ]-*/
TICK_back          (void)
{
   /*---(locals)-----------+-----------+-*/
   int       i;                             /* loop iterator                  */
   int       j;                             /* loop iterator                  */
   int       x_xinc        = 10.0;
   int       x_yinc        =  0.0;
   float     x_bot         =  0.0;
   float     x_top         = my.p_texh;
   float     x_beg         =  0.0;
   float     x_end         = my.p_texw;
   float     x_bar         =  0.0;
   float     x_ypos        =  0.0;
   char      x_msg         [100];
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   x_yinc = x_top / 12.0;
   x_bar  = my.p_top - my.p_bot;
   /*---(vertical bars)------------------*/
   for (i = x_beg; i < x_end; i += x_xinc) {
      if      (i % (x_xinc * 100) == 0)  glColor4f   (0.40f, 0.20f, 0.00f, 1.0f);
      else if (i % (x_xinc * 10 ) == 0)  glColor4f   (0.20f, 0.10f, 0.00f, 1.0f);
      else if (i % (x_xinc * 2  ) == 0)  glColor4f   (0.15f, 0.07f, 0.00f, 1.0f);
      else                               glColor4f   (0.10f, 0.05f, 0.00f, 1.0f);
      glBegin         (GL_POLYGON); {
         glVertex3f  (i         ,   x_top,     0.0);
         glVertex3f  (i + x_xinc,   x_top,     0.0);
         glVertex3f  (i + x_xinc,   x_bot,     0.0);
         glVertex3f  (i         ,   x_bot,     0.0);
      } glEnd   ();
   }
   /*---(scale dividers)-----------------*/
   glColor4f   (0.10f, 0.10f, 0.10f, 1.0f);
   for (i = x_yinc; i <= x_top; i += x_yinc) {
      for (j = i - x_bar; j <= i; j += 50) {
         glBegin         (GL_POLYGON); {
            glVertex3f  (x_beg, j + 2.0,     5.0);
            glVertex3f  (x_end, j + 2.0,     5.0);
            glVertex3f  (x_end, j - 2.0,     5.0);
            glVertex3f  (x_beg, j - 2.0,     5.0);
         } glEnd   ();
      }
   }
   /*---(top and bottom limits)----------*/
   for (i = 0; i < 12; ++i) {
      x_ypos = (i + 1) * x_yinc;
      /*> if (yVIKEYS_mode_curr () == MODE_PROGRESS && (5 - i == my.p_leg || 11 - i == my.p_leg))   <* 
       *>    glColor4f    (0.25f, 0.00f, 0.00f, 1.0f);                                              <* 
       *> else                                                                                      <*/
      glColor4f    (0.00f, 0.00f, 0.00f, 1.0f);
      glBegin         (GL_POLYGON); {
         glVertex3f  (x_beg, x_ypos             ,   10.0);
         glVertex3f  (x_end, x_ypos             ,   10.0);
         glVertex3f  (x_end, x_ypos - 25        ,   10.0);
         glVertex3f  (x_beg, x_ypos - 25        ,   10.0);
      } glEnd   ();
      glBegin         (GL_POLYGON); {
         glVertex3f  (x_beg, x_ypos - x_bar     ,   10.0);
         glVertex3f  (x_end, x_ypos - x_bar     ,   10.0);
         glVertex3f  (x_end, x_ypos - x_bar + 25,   10.0);
         glVertex3f  (x_beg, x_ypos - x_bar + 25,   10.0);
      } glEnd   ();
   }
   /*---(end limits)---------------------*/
   glColor4f    (0.25f, 0.00f, 0.00f, 1.0f);
   glLineWidth  (10.0f);
   glPushMatrix(); {
      glBegin(GL_LINE_STRIP); {
         glVertex3f  ( 5.0f      , x_top,   15.0);
         glVertex3f  ( 5.0f      , x_bot,   15.0);
      } glEnd   ();
      glBegin(GL_LINE_STRIP); {
         glVertex3f  (x_end - 5.0, x_top,   15.0);
         glVertex3f  (x_end - 5.0, x_bot,   15.0);
      } glEnd   ();
   } glPopMatrix();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TICK_line          (float a_x1, float a_y1, float a_x2, float a_y2, float a_xinc, float a_yinc, float a_z)
{
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*> glBegin(GL_LINE_STRIP); {                                                      <* 
    *>    glVertex3f  (a_x1 - 0.0, a_y1 + 0.0, a_z);                                  <* 
    *>    glVertex3f  (a_x2 - 0.0, a_y2 + 0.0, a_z);                                  <* 
    *> } glEnd   ();                                                                  <*/
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
TICK_servoline     (char a_type, float a_base, float a_sec1, float a_sec2, float a_deg1, float a_deg2, float a_unit)
{
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   float     x_pos1        = 0.0;
   float     x_pos2        = 0.0;
   float     y_pos1        = 0.0;
   float     y_pos2        = 0.0;
   float     y_inc         = 0.0;
   float     x_inc         = 0.0;
   float     z_pos         = 0.0;
   float     y_off         = my.p_texh / 2.0;
   /*---(prepare points)-----------------*/
   x_pos1   = (a_sec1 * a_unit) - s_start;
   x_pos2   = (a_sec2 * a_unit) - s_start;
   y_pos1   = a_base + a_deg1;
   y_pos2   = a_base + a_deg2;
   /*---(prepare adjustments)------------*/
   switch (a_type) {
   case 'f' : y_inc = 4; z_pos = 230.0;  break;   /* femur         */
   case 'p' : y_inc = 2; z_pos = 233.0;  break;   /* patella       */
   case 't' : y_inc = 1; z_pos = 236.0;  break;   /* tibia         */
   case 'e' : x_inc = 5; z_pos = 239.0;  break;   /* end point     */
   case 'c' : x_inc = 0; z_pos = 239.0;  break;   /* current       */
   }
   /*---(draw)--------*/
   if      (x_pos2 <  0.0     )  ;  /* line to early */
   else if (x_pos1 >  s_cutend)  ;  /* line to late  */
   else if (x_pos2 <= s_cutmid)
      TICK_line (x_pos1           , y_pos1         , x_pos2           , y_pos2         , x_inc, y_inc, z_pos);
   else if (x_pos1 >= s_cutmid)
      TICK_line (x_pos1 - s_cutmid, y_pos1 + y_off , x_pos2 - s_cutmid, y_pos2 + y_off , x_inc, y_inc, z_pos);
   else {
      TICK_line (x_pos1           , y_pos1         , x_pos2           , y_pos2         , x_inc, y_inc, z_pos);
      TICK_line (x_pos1 - s_cutmid, y_pos1 + y_off , x_pos2 - s_cutmid, y_pos2 + y_off , x_inc, y_inc, z_pos);
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_servos        (int a_leg)
{
   /*---(locals)-----------+-----------+-*/
   int       x_xinc        = 100.0;
   int       x_yinc        =  0.0;
   float     x_bot         =  0.0;
   float     x_top         = my.p_texh;
   float     x_beg         =  0.0;
   float     x_end         = my.p_texw;
   float     x_pos         =  0.0;
   char      x_msg         [100];
   float     x_base        = 0.0;
   int       i;                             /* loop iterator                  */
   int       j;                             /* loop iterator                  */
   int       rc      = 0;                   /* simple return code             */
   float     x_unit        = 0;
   double    x_sec1        = 0;
   double    x_deg1        = 0;
   double    x_sec2        = 0;
   double    x_deg2        = 0;
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   x_yinc    = x_top / 12.0;
   x_base    = ((my.p_texh / 2.0) - (x_yinc * a_leg)) - my.p_top;
   x_unit    = x_xinc / my.p_inc;
   /*---(femur)--------------------------*/
   glColor4f    (0.50f, 0.00f, 0.00f, 1.0f);
   glLineWidth  (15.0f);
   glPushMatrix(); {
      rc = yKINE_move_first (a_leg * 3, &x_sec1, &x_deg1);
      while (rc >= 0) {
         /*---(read next)---*/
         rc = yKINE_move_next  (&x_sec2, &x_deg2);
         if (rc <  0) break;
         /*---(fix points)--*/
         TICK_servoline     ('f', x_base, x_sec1, x_sec2, x_deg1, x_deg2, x_unit);
         /*---(save)--------*/
         x_sec1 = x_sec2;
         x_deg1 = x_deg2;
         /*---(done)--------*/
      }
   } glPopMatrix();
   /*---(patella)------------------------*/
   glColor4f    (0.20f, 0.20f, 0.70f, 1.0f);
   glLineWidth  (10.0f);
   glPushMatrix(); {
      rc = yKINE_move_first ((a_leg * 3) + 1, &x_sec1, &x_deg1);
      while (rc >= 0) {
         /*---(read next)---*/
         rc = yKINE_move_next  (&x_sec2, &x_deg2);
         if (rc <  0) break;
         /*---(fix points)--*/
         TICK_servoline     ('p', x_base, x_sec1, x_sec2, x_deg1, x_deg2, x_unit);
         /*---(save)--------*/
         x_sec1 = x_sec2;
         x_deg1 = x_deg2;
         /*---(done)--------*/
      }
   } glPopMatrix();
   /*---(tibia)--------------------------*/
   glColor4f    (0.60f, 0.60f, 0.00f, 1.0f);
   glLineWidth  ( 4.0f);
   glPushMatrix(); {
      rc = yKINE_move_first ((a_leg * 3) + 2, &x_sec1, &x_deg1);
      while (rc >= 0) {
         /*---(read next)---*/
         rc = yKINE_move_next  (&x_sec2, &x_deg2);
         if (rc <  0) break;
         /*---(fix points)--*/
         TICK_servoline     ('t', x_base, x_sec1, x_sec2, x_deg1, x_deg2, x_unit);
         /*---(save)--------*/
         x_sec1 = x_sec2;
         x_deg1 = x_deg2;
         /*---(done)--------*/
      }
   } glPopMatrix();
   /*---(draw end)-----------------------*/
   glColor4f    (0.50f, 0.00f, 0.50f, 1.0f);
   glLineWidth  (10.0f);
   glPushMatrix(); {
      TICK_servoline     ('e', x_base, my.p_len, my.p_len, my.p_top - 25.0, my.p_bot + 25.0, x_unit);
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
      else if (c >  0)  sprintf (s_sectext, "%c%c"    , c + 'a', d + 'a');
      else              sprintf (s_sectext, "%c"      , d + 'a');
   }
   /*> printf (">   %s\n", s_sectext);                                                <*/
   /*---(complete)-----------------------*/
   return s_sectext;
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

char         /*--> draw texture labels -------------------[ ------ [ ------ ]-*/
TICK_labels_newer  (void)
{
   /*---(locals)-----------+-----------+-*/
   int       i;                             /* loop iterator                  */
   int       j;                             /* loop iterator                  */
   float     x_xinc        = 10.0;
   float     x_yinc        =  0.0;
   float     x_bot         =  0.0;
   float     x_top         = my.p_texh;
   float     x_beg         =  0.0;
   float     x_end         = my.p_texw;
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
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_scale_base (&my.p_multi, &my.p_base);                                  <*/
   x_yinc      = x_top / 12.0;
   /*> x_labelper  = (my.p_texw / 10.0) * my.p_multi;                                 <*/
   /*> printf ("%8d  %8.3lf\n", my.p_texw, my.p_multi);                               <*/
   /*> x_secbeg1   = x_labelper * s_section;                                          <*/
   /*> x_secbeg2   = x_secbeg1 + x_labelper;                                          <*/
   strlcpy (x_label1, TICK_sectext (s_section    ), 10);
   strlcpy (x_label2, TICK_sectext (s_section + 1), 10);
   /*> printf ("%8d  %8d  %8d\n", x_labelper, x_secbeg1, x_secbeg2);                  <*/
   /*---(leg labels)---------------------*/
   for (i = x_beg; i < x_end; i += x_xinc * 100) {
      for (j = 0; j < 12; ++j) {
         /*---(leg)----------------------*/
         x_pos = my.p_texh - (j * x_yinc);
         sprintf (x_msg, "%d/%s", j % 6, legs_name [j % 6]);
         yVIKEYS_view_color (YCOLOR_BAS_ACC, 1.00);
         glPushMatrix(); {
            glTranslatef ( i + 30.0 , x_pos    -  115.0    ,    60.0  );
            glRotatef  ( 90.0  , 0.0f, 0.0f, 1.0f);
            yFONT_print  (my.fixed,  25, YF_MIDCEN, x_msg);
         } glPopMatrix();
         /*---(hint)---------------------*/
         /*> yVIKEYS_view_color (YCOLOR_BAS_MUT, 1.00);                               <* 
          *> if (j >= 6)  strlcpy (x_msg, x_label1, LEN_STR);                         <* 
          *> else         strlcpy (x_msg, x_label2, LEN_STR);                         <* 
          *> glColor4f    (0.25f, 0.25f, 0.25f, 1.0f);                                <* 
          *> glPushMatrix(); {                                                        <* 
          *>    glTranslatef ( i +  80.0 , x_pos    -   40.0    ,    60.0  );         <* 
          *>    yFONT_print  (my.fixed,  20, YF_MIDCEN, x_msg);                       <* 
          *> } glPopMatrix();                                                         <*/
         /*---(scale)--------------------*/
         yVIKEYS_scale_desc (x_msg);
         yVIKEYS_view_color (YCOLOR_BAS_ACC, 1.00);
         glPushMatrix(); {
            glTranslatef ( i +   50.0 , x_pos   -  28.0 ,    60.0  );
            yFONT_print  (my.fixed,  16, YF_BOTLEF, x_msg);
         } glPopMatrix();
         /*---(speed)--------------------*/
         yVIKEYS_speed_desc (x_msg);
         glPushMatrix(); {
            glTranslatef ( i +  950.0, x_pos    -  28.0 ,    60.0  );
            yFONT_print  (my.fixed,  16, YF_BOTRIG, x_msg);
         } glPopMatrix();
         /*---(done)---------------------*/
      }
   }
   return 0;
}

/*> char         /+--> draw texture labels -------------------[ ------ [ ------ ]-+/                                                       <* 
 *> TICK_labels        (void)                                                                                                              <* 
 *> {                                                                                                                                      <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                                            <* 
 *>    int       i;                             /+ loop iterator                  +/                                                       <* 
 *>    int       j;                             /+ loop iterator                  +/                                                       <* 
 *>    float     x_xinc        = 10.0;                                                                                                     <* 
 *>    float     x_yinc        =  0.0;                                                                                                     <* 
 *>    float     x_bot         =  0.0;                                                                                                     <* 
 *>    float     x_top         = my.p_texh;                                                                                                <* 
 *>    float     x_beg         =  0.0;                                                                                                     <* 
 *>    float     x_end         = my.p_texw;                                                                                                <* 
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
 *>    x_bar       = my.p_top - my.p_bot;                                                                                                  <* 
 *>    x_labelper  = (my.p_texw / 10.0) * my.p_multi;                                                                                      <* 
 *>    /+> printf ("%8d  %8.3lf\n", my.p_texw, my.p_multi);                               <+/                                              <* 
 *>    x_secbeg1   = x_labelper * s_section;                                                                                               <* 
 *>    x_secbeg2   = x_secbeg1 + x_labelper;                                                                                               <* 
 *>    strlcpy (x_label1, TICK_sectext (s_section    ), 10);                                                                               <* 
 *>    strlcpy (x_label2, TICK_sectext (s_section + 1), 10);                                                                               <* 
 *>    /+> printf ("%8d  %8d  %8d\n", x_labelper, x_secbeg1, x_secbeg2);                  <+/                                              <* 
 *>    /+---(leg labels)---------------------+/                                                                                            <* 
 *>    for (i = x_beg; i < x_end; i += x_xinc * 100) {                                                                                     <* 
 *>       for (j = 0; j < 12; ++j) {                                                                                                       <* 
 *>          x_pos = my.p_texh - (j * x_yinc);                                                                                             <* 
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
*>       for (i = 0; i < my.p_texw; i += x_xinc) {                                                                                        <* 
   *>          /+---(labels)-------------------+/                                                                                            <* 
      *>          if (i % (int) (x_xinc * 10) == 0) {                                                                                           <* 
         *>             for (j = 0; j < 12; ++j) {                                                                                                 <* 
            *>                glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);                                                                               <* 
               *>                if (j < 6)  snprintf     (x_msg, 50, "%d%c"  , (int) ((i / x_xinc) * my.p_multi) + x_secbeg1, my.p_base);               <* 
               *>                else        snprintf     (x_msg, 50, "%d%c"  , (int) ((i / x_xinc) * my.p_multi) + x_secbeg2, my.p_base);               <* 
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
                              *>                x_sec = (((float) (i) / x_xinc) * my.p_multi) + x_secbeg1;                                                              <* 
                              *>                if (j == 0) {                                                                                                           <* 
                                 *>                   rc = yKINE_phys_flat   (YKINE_FK, x_sec, &s_lowest, &x_lowcnt);                                                      <* 
                                    *>                   /+> printf ("i=%5d, j=%2d, x_sec=%8.3lf, s_lowest=%8.1lf, x_lowcnt=%2d\n", i, j, x_sec, s_lowest, x_lowcnt);   <+/   <* 
                                    *>                }                                                                                                                       <* 
                                    *>                /+> TICK_height   (i, x_pos);                                          <+/                                              <* 
                                    *>                if (x_sec >= 0.0 && x_sec <= my.p_len)  TICK__heat_column (j    , x_sec, i, x_pos - 35.0);                              <* 
                                    *>             } else {                                                                                                                   <* 
                                       *>                x_pos = (12 + (6 - j)) * x_yinc;                                                                                        <* 
                                          *>                x_sec = (((float) (i) / x_xinc) * my.p_multi) + x_secbeg2;                                                              <* 
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
   s_textop    = 0.5 - ((my.p_leg + 0) * y_inc);
   s_texbot    = 0.5 - ((my.p_leg + 1) * y_inc);
   /*---(calc basics)--------------------*/
   s_texavail  = my.p_wide * 2.0;
   s_texpct    = s_texavail / my.p_texw;
   s_texctr    = s_texpct / 2.0;
   s_start     = s_section * my.p_texw;
   s_cutmid    = my.p_texw;
   s_cutend    = my.p_texw * 2;
   /*---(seconds)------------------------*/
   s_tsec      = x_inc / my.p_inc;
   /*> s_tnsec     = s_texavail / s_tsec;                                             <*/
   /*> s_tsecp     = s_texpct / s_tnsec;                                              <*/
   s_plenp     = ((my.p_len * s_tsec) - s_start) / my.p_texw;
   if (s_plenp > 2.0)  s_plenp = 2.0;
   if (s_plenp < 0.0)  s_plenp = 0.0;
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> draw texture for progress ticker ------[ ------ [ ------ ]-*/
TICK_draw          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i;                             /* loop iterator                  */
   int         x, y;
   /*---(new)----------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*> printf ("start drawing ticker\n");                                             <*/
   TICK_globals ();
   DEBUG_GRAF   yLOG_value   ("p_tex"     , my.p_tex);
   DEBUG_GRAF   yLOG_value   ("p_fbo"     , my.p_fbo);
   DEBUG_GRAF   yLOG_value   ("p_texw"    , my.p_texw);
   DEBUG_GRAF   yLOG_value   ("p_texh"    , my.p_texh);
   yGLTEX_draw_start (my.p_fbo, YGLTEX_BOTLEF, my.p_texw, my.p_texh, 1.0);
   /*> for (x = 0; x < my.p_texw; x += 100) {                                         <* 
    *>    for (y = 0; y < my.p_texh; y += 100) {                                      <* 
    *>       glColor4f    (0.00f, 0.00f, 0.00f, 1.0f);                                <* 
    *>       glLineWidth  (10.0f);                                                    <* 
    *>       glPushMatrix(); {                                                        <* 
    *>          glBegin(GL_LINE_STRIP); {                                             <* 
    *>             glVertex3f  (x     , y     ,   10.0);                              <* 
    *>             glVertex3f  (x + 20, y     ,   10.0);                              <* 
    *>          } glEnd   ();                                                         <* 
    *>          glBegin(GL_LINE_STRIP); {                                             <* 
    *>             glVertex3f  (x     , y     ,   10.0);                              <* 
    *>             glVertex3f  (x     , y + 20,   10.0);                              <* 
    *>          } glEnd   ();                                                         <* 
    *>       } glPopMatrix();                                                         <* 
    *>    }                                                                           <* 
    *> }                                                                              <*/
   /*> TICK_back         ();                                                          <*/
   TICK_back_vert    ();
   TICK_back_horz    ();
   TICK_labels_newer ();
   for (i = 0; i <= 5; ++i) TICK_servos  (i);
   /*> yGLTEX_tex2png    ("ticker.png", my.p_texw, my.p_texh);                        <*/
   yGLTEX_draw_end   (my.p_tex);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> calculate texture positioning ---------[ ------ [ ------ ]-*/
TICK_current       (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   int         x_save      = s_section;
   /*---(current pos)--------------------*/
   s_curp      = ((my.p_cur * s_tsec) - s_start) / my.p_texw;
   while (s_curp  > 1.60) {
      ++s_section;
      TICK_globals ();
      s_curp      = ((my.p_cur * s_tsec) - s_start) / my.p_texw;
      ++rc;
   }
   while (s_curp  < 0.40) {
      --s_section;
      TICK_globals ();
      s_curp      = ((my.p_cur * s_tsec) - s_start) / my.p_texw;
      ++rc;
   }
   if (s_section != x_save)  TICK_draw ();
   /*---(script fits screen)-------------*/
   if (s_plenp <= s_texpct) {
      s_texbeg = 0.0f;
      s_texend = s_texpct;
   }
   /*---(script is bigger than screen)---*/
   else {
      switch (my.p_pos) {
      case '0' :
         s_texbeg  = s_curp - (s_texpct * 0.00);
         s_texend  = s_texbeg  + s_texpct;
         break;
      case 's' :
         s_texbeg  = s_curp - (s_texpct * 0.05);
         s_texend  = s_texbeg  + s_texpct;
         break;
      case 'h' :
         s_texbeg  = s_curp - (s_texpct * 0.28);
         s_texend  = s_texbeg  + s_texpct;
         break;
      case 'c' :
         s_texbeg  = s_curp - (s_texpct * 0.50);
         s_texend  = s_texbeg  + s_texpct;
         break;
      case 'l' :
         s_texbeg  = s_curp - (s_texpct * 0.72);
         s_texend  = s_texbeg  + s_texpct;
         break;
      case 'e' :
         s_texbeg  = s_curp - (s_texpct * 0.95);
         s_texend  = s_texbeg  + s_texpct;
         break;
      case '$' :
         s_texbeg  = s_curp - (s_texpct * 1.00);
         s_texend  = s_texbeg  + s_texpct;
         break;
      }
      /*> if (s_texbeg < 0.0) {                                                       <* 
       *>    s_texbeg  = 0.0f;                                                        <* 
       *>    s_texend  = s_texpct;                                                    <* 
       *> } else if (s_texend > s_plenp) {                                            <* 
       *>    s_texend  = s_plenp;                                                     <* 
       *>    s_texbeg  = s_texend - s_texpct;                                         <* 
       *> }                                                                           <*/
   }
   /*---(prepare for multitex)-----------*/
   s_texbeg1 = s_texend1 = s_texpct1 = s_texbeg2 = s_texend2 = s_texpct2 = 0.0;
   /*---(all first tex area)-------------*/
   if        (s_texend <= 1.0) {
      s_texbeg1  = s_texbeg;
      s_texend1  = s_texend;
      s_texpct1  = 1.0;
   }
   /*---(all second tex area)------------*/
   else if (s_texbeg >= 1.0) {
      s_texbeg2  = s_texbeg - 1.0;
      s_texend2  = s_texend - 1.0;
      s_texpct2  = 1.0;
   }
   /*---(mixed text area)----------------*/
   else {
      s_texbeg1  = s_texbeg;
      s_texend1  = 1.0;
      s_texpct1  = (s_texend1 - s_texbeg1) / s_texpct;
      s_texbeg2  = 0.0;
      s_texend2  = s_texend - 1.0;
      s_texpct2  = (s_texend2 - s_texbeg2) / s_texpct;
      /*---(rounding issues)-------------*/
      if (s_texpct1 <= 0.01) {
         s_texbeg1  = 0.0;
         s_texend1  = 0.0;
         s_texpct1  = 0.0;
         s_texbeg2  = s_texbeg - 1.0;
         s_texend2  = s_texend - 1.0;
         s_texpct2  = 1.0;
      }
      /*---(rounding issues)-------------*/
      if (s_texpct2 <= 0.01) {
         s_texbeg1  = s_texbeg;
         s_texend1  = s_texend;
         s_texpct1  = 1.0;
         s_texbeg2  = 0.0;
         s_texend2  = 0.0;
         s_texpct2  = 0.0;
      }
   }
   /*---(current pos)--------------------*/
   s_cur       = ((s_curp - s_texbeg) / s_texpct) * my.p_wide;
   /*---(complete)-----------------------*/
   return rc;
}

char         /*--> show texture on screen ----------------[ ------ [ ------ ]-*/
TICK_showtex       (float a_height, float a_top, float a_bot)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   /*---(figure current)-----------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_value   ("a_height"  , a_height);
   DEBUG_GRAF   yLOG_value   ("a_top"     , a_top);
   DEBUG_GRAF   yLOG_value   ("a_bot"     , a_bot);
   rc = TICK_current    ();
   /*---(draw texture)-------------------*/
   glBindTexture   (GL_TEXTURE_2D, my.p_tex);
   /*---(first part)---------------------*/
   if (s_texpct1 >= 0.01) {
      glBegin(GL_POLYGON); {
         /*---(top beg)--------*/
         glTexCoord2f (s_texbeg1             , a_top );
         glVertex3f   (0.0                   , a_height ,     0.00f);
         /*---(top end)--------*/
         glTexCoord2f (s_texend1             , a_top );
         glVertex3f   (my.p_wide * s_texpct1, a_height ,     0.00f);
         /*---(bottom end)-----*/
         glTexCoord2f (s_texend1             , a_bot );
         glVertex3f   (my.p_wide * s_texpct1, 0.0      ,     0.00f);
         /*---(bottom beg)-----*/
         glTexCoord2f (s_texbeg1             , a_bot );
         glVertex3f   (0.0                   , 0.0      ,     0.00f);
         /*---(done)-----------*/
      } glEnd();
   }
   if (s_texpct2 >= 0.01) {
      glBegin(GL_POLYGON); {
         /*---(top beg)--------*/
         glTexCoord2f (s_texbeg2             , a_top + 0.5 );
         glVertex3f   (my.p_wide * s_texpct1, a_height ,     0.00f);
         /*---(top end)--------*/
         glTexCoord2f (s_texend2             , a_top + 0.5 );
         glVertex3f   (my.p_wide            , a_height ,     0.00f);
         /*---(bottom end)-----*/
         glTexCoord2f (s_texend2             , a_bot + 0.5 );
         glVertex3f   (my.p_wide            , 0.0      ,     0.00f);
         /*---(bottom beg)-----*/
         glTexCoord2f (s_texbeg2             , a_bot + 0.5 );
         glVertex3f   (my.p_wide * s_texpct1, 0.0      ,     0.00f);
         /*---(done)-----------*/
      } glEnd();
   }
   glBindTexture   (GL_TEXTURE_2D, 0);
   /*---(draw current)-------------------*/
   glColor4f    (0.00f, 0.00f, 1.00f, 1.0f);
   glLineWidth  ( 5.0f);
   glPushMatrix(); {
      glBegin(GL_LINE_STRIP); {
         glVertex3f  (s_cur, a_height - 15.0,   70.0);
         glVertex3f  (s_cur, 0.0            ,   70.0);
      } glEnd   ();
   } glPopMatrix();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return rc;
}

/*> char         /+--> show texture on screen ----------------[ ------ [ ------ ]-+/   <* 
 *> TICK_full          (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    char        rc          = 0;                                                    <* 
 *>    /+---(setup view)---------------------+/                                        <* 
 *>    glViewport      (    0, 0.0        , my.p_wide, my.w_tall);                     <* 
 *>    glMatrixMode    (GL_PROJECTION);                                                <* 
 *>    glLoadIdentity  ();                                                             <* 
 *>    glOrtho         ( 0.0f, my.p_wide, 0.0     , my.w_tall,  -500.0,   500.0);      <* 
 *>    glMatrixMode    (GL_MODELVIEW);                                                 <* 
 *>    /+---(firgure current)----------------+/                                        <* 
 *>    rc = TICK_showtex    (my.w_tall,  0.5,  0.0);                                   <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return rc;                                                                      <* 
 *> }                                                                                  <*/

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
   /*---(upper bar)----------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*> for (y = -300; y <= 300; y += 100) {                                           <* 
    *>    glColor4f    (1.00f, 1.00f, 1.00f, 1.00f);                                  <* 
    *>    for (x = -300; x <= 300; x += 100) {                                        <* 
    *>       glPushMatrix (); {                                                       <* 
    *>          glTranslatef(       x,       y,    300);                              <* 
    *>          snprintf (t, 30, "%dx, %dy", x, y);                                   <* 
    *>          yFONT_print (my.fixed,  10, YF_MIDCEN, t);                            <* 
    *>       } glPopMatrix ();                                                        <* 
    *>    }                                                                           <* 
    *> }                                                                              <*/
   /*---(draw texture)-------------------*/
   /*> glBindTexture   (GL_TEXTURE_2D, my.p_tex);                                     <* 
    *> glBegin(GL_POLYGON); {                                                         <* 
    *>    /+---(top beg)--------+/                                                    <* 
    *>    glTexCoord2f (0.0                   , 1.0   );                              <* 
    *>    glVertex3f   (0.0                   , 100.0    ,     0.00f);                <* 
    *>    /+---(top end)--------+/                                                    <* 
    *>    glTexCoord2f (1.0                   , 1.0   );                              <* 
    *>    glVertex3f   (800.0                 , 100.0    ,     0.00f);                <* 
    *>    /+---(bottom end)-----+/                                                    <* 
    *>    glTexCoord2f (1.0                   , 11/12.0 );                            <* 
    *>    glVertex3f   (800.0                 , -100.0   ,     0.00f);                <* 
    *>    /+---(bottom beg)-----+/                                                    <* 
    *>    glTexCoord2f (0.0                   , 11/12.0 );                            <* 
    *>    glVertex3f   (0.0                   , -100.0   ,     0.00f);                <* 
    *>    /+---(done)-----------+/                                                    <* 
    *> } glEnd();                                                                     <* 
    *> glBindTexture   (GL_TEXTURE_2D, 0);                                            <*/
   /*> s_texbeg1 = 0.0;                                                               <* 
    *> s_texend1 = 1.0;                                                               <*/
   /*> int a_top =   1.0;                                                             <* 
    *> int a_bot =   0.0;                                                             <* 
    *> int a_height = 200.0;                                                          <* 
    *> my.p_wide    = 800.0;                                                          <* 
    *> s_texpct1    = 100.0;                                                          <* 
    *> glBegin(GL_POLYGON); {                                                         <* 
    *>    /+---(top beg)--------+/                                                    <* 
    *>    glTexCoord2f (s_texbeg1             , a_top );                              <* 
    *>    glVertex3f   (0.0                   , a_height ,     0.00f);                <* 
    *>    /+---(top end)--------+/                                                    <* 
    *>    glTexCoord2f (s_texend1             , a_top );                              <* 
    *>    glVertex3f   (my.p_wide * s_texpct1, a_height ,     0.00f);                 <* 
    *>    /+---(bottom end)-----+/                                                    <* 
    *>    glTexCoord2f (s_texend1             , a_bot );                              <* 
    *>    glVertex3f   (my.p_wide * s_texpct1, 0.0      ,     0.00f);                 <* 
    *>    /+---(bottom beg)-----+/                                                    <* 
    *>    glTexCoord2f (s_texbeg1             , a_bot );                              <* 
    *>    glVertex3f   (0.0                   , 0.0      ,     0.00f);                <* 
    *>    /+---(done)-----------+/                                                    <* 
    *> } glEnd();                                                                     <*/
   /*---(firgure current)----------------*/
   rc = yVIKEYS_prog_cur (&my.p_pos, &my.p_cur, &my.p_scale, &my.p_inc);
   if (yVIKEYS_prog_redraw ())  rc = TICK_draw ();
   rc = TICK_showtex     (my.p_tall,  s_textop,  s_texbot);
   /*---(show debug)---------------------*/
   if (my.p_debug == 'y') {
      printf ("TICK_show ()  debugging\n");
      printf ("\n");
      printf ("   ___script_____________________\n");
      printf ("   my.p_len         = %10.3f\n", my.p_len);
      printf ("   my.p_pos         = %c\n"    , my.p_pos);
      printf ("   my.p_inc         = %10.3f\n", my.p_inc);
      printf ("   my.p_cur         = %10.3f\n", my.p_cur);
      printf ("   s_curp           = %10.3f\n", s_curp);
      printf ("   s_cur            = %10.3f\n", s_cur);
      printf ("\n");
      printf ("   ___vertical___________________\n");
      printf ("   my.p_texh        = %6d\n"   , my.p_texh);
      printf ("   my.p_tall      = %6d\n"   , my.p_tall);
      printf ("   max legs         = %6d\n"   , 6);
      printf ("   height per leg   = %10.3f\n", my.p_texh / 6.0);
      printf ("   my.p_leg         = %10.3f\n", my.p_leg);
      printf ("   s_textop         = %10.3f\n", s_textop);
      printf ("   s_texbot         = %10.3f\n", s_texbot);
      printf ("\n");
      printf ("   ___horizontal_________________\n");
      printf ("   my.p_wide       = %6d\n"   , my.p_wide);
      printf ("   my.p_texw        = %6d\n"   , my.p_texw);
      printf ("   my.p_inc         = %10.3f\n", my.p_inc);
      printf ("   s_texavail       = %10.3f\n", s_texavail);
      printf ("   s_texpct         = %10.3f\n", s_texpct);
      printf ("   s_texctr         = %10.3f\n", s_texctr);
      printf ("\n");
      printf ("   s_tsec           = %10.3f\n", s_tsec);
      /*> printf ("   s_tnsec          = %10.3f\n", s_tnsec);                         <*/
      /*> printf ("   s_tsecp          = %10.3f\n", s_tsecp);                         <*/
      printf ("   s_plenp          = %10.3f\n", s_plenp);
      printf ("\n");
      /*> printf ("   s_maxlabel       = %10.3f\n", (my.p_texw / 10.0) * my.p_multi);   <*/
      printf ("   s_texbeg1        = %10.3f\n", s_texbeg1);
      printf ("   s_texend1        = %10.3f\n", s_texend1);
      printf ("   s_texpct1        = %10.3f\n", s_texpct1);
      printf ("   s_texbeg2        = %10.3f\n", s_texbeg2);
      printf ("   s_texend2        = %10.3f\n", s_texend2);
      printf ("   s_texpct2        = %10.3f\n", s_texpct2);
      my.p_debug = '-';
   }
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return rc;
}




/*============================----end-of-source---============================*/
