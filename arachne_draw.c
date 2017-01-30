/*============================================================================*
 *=======                        START OF SOURCE                       =======*
 *============================================================================*/

#include "arachne.h"


char   flag_view  = 0;

void draw_spider();



/*===[[ TYPEDEFS ]]===========================================================*/


/*===[[ RUN TIME DEBUGGING ]]=================================================*/
int       debug_xlib_con  = 1;
int       debug_xlib_foc  = 1;

char      flag_annotate  = 'n';


struct cBODY {
   float   x, y, z;
} pos, off;

float     my_x     =  0.0;
float     my_y     =  0.0;
float     my_z     = -800.0;
float     my_vx    =  0.0;
float     my_vy    =  150.0;
float     my_vz    =  0.0;
float     my_roll  =  0;
float     my_yaw   =  0;
float     my_pitch =  0;
float     my_vroll  = 0;
float     my_vyaw   = 0;
float     my_vpitch = 0;

float     counter = 0;
float     my_calf = -20.0;
int       my_curr = 5;

float     my_len  = MAX_POS - 1;
double    my_ppos = 0;
float     my_run  = 0;
float     my_inc  = 1;
float     my_deg  = 0;


float     arg_thor = 0.0f;
float     arg_femu = 0.0f;
float     arg_pate = 0.0f;
float     arg_tibi = 0.0f;
float     arg_y    = 0.0f;



double   modelview[9][16];
double   my_location[16];





/*---(GLX Globals)---------------------------------*/
XWindowAttributes      gwa;



/*===[[ RUN TIME DEBUGGING ]]=================================================*/
int       is_test      = 1;
int       is_test_beg  = 1;
int       is_test_inp  = 1;
int       is_test_pro  = 1;
int       is_test_foc  = 1;
int       is_unittest  = 0;





/*===[[ yUNIT ]]==============================================================*/
char       eva_feedback[200] = "";
char      *eva_accessor     (char   *a_question);




/*====================------------------------------------====================*/
/*===----                       command line                           ----===*/
/*====================------------------------------------====================*/
static void      o___COMMAND_________________o (void) {;}

char
DRAW_command       (void)
{
   /*---(setup view)---------------------*/
   glViewport      ( my.c_left, my.c_bott, my.c_wide, my.c_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         ( 0.0f, my.c_wide, 0.0f, my.c_tall,  -500.0,   500.0);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   glPushMatrix    (); {
      glColor4f    (0.00f, 0.00f, 0.15f, 1.0f);
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0f     , my.c_tall,  0.0f);
         glVertex3f  (my.w_wide, my.c_tall,  0.0f);
         glVertex3f  (my.w_wide, 0.0f      ,  0.0f);
         glVertex3f  (0.0f     , 0.0f      ,  0.0f);
      } glEnd   ();
   } glPopMatrix   ();
   /*---(text)---------------------------*/
   glPushMatrix    (); {
      glTranslatef (    2.0f,    3.0f,    0.0f);
      glColor4f    (1.00f, 1.00f, 1.00f, 1.00f);
      yFONT_print  (my.font,  10, YF_BOTLEF, my.message);
   } glPopMatrix   ();
   /*---(complete)-----------------------*/
   return;
}

char
DRAW_title         (void)
{
   /*---(setup view)---------------------*/
   glViewport      ( my.t_left, my.t_bott, my.t_wide, my.t_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         ( 0.0f, my.t_wide, 0.0f, my.t_tall,  -500.0,   500.0);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   glColor4f    (0.60f, 0.30f, 0.00f, 1.0f);
   glPushMatrix    (); {
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0f     , my.t_tall - 100,  0.0f);
         glVertex3f  (my.t_wide, my.t_tall - 100,  0.0f);
         glVertex3f  (my.t_wide, 0.0f      ,  0.0f);
         glVertex3f  (0.0f     , 0.0f      ,  0.0f);
      } glEnd   ();
   } glPopMatrix   ();
   if (yURG_debugmode () == 'y')  glColor4f    (1.00f, 0.00f, 0.00f, 1.0f);
   else                           glColor4f    (0.60f, 0.30f, 0.00f, 1.0f);
   glPushMatrix    (); {
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0f     , my.t_tall      ,  0.0f);
         glVertex3f  (my.t_wide, my.t_tall      ,  0.0f);
         glVertex3f  (my.t_wide, my.t_tall - 100,  0.0f);
         glVertex3f  (0.0f     , my.t_tall - 100,  0.0f);
      } glEnd   ();
   } glPopMatrix   ();
   /*---(display)------------------------*/
   glColor4f    (0.00f, 0.00f, 0.00f, 1.0f);
   glPushMatrix    (); {
      glTranslatef (my.t_wide + 3,   5.0f,    0.0f);
      glRotatef    ( 90.0, 0.0f, 0.0f, 1.0f);
      yFONT_print  (my.font,  16, YF_BOTLEF, my.t_text);
   } glPopMatrix   ();
   if (yURG_debugmode () == 'y')  glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);
   else                           glColor4f    (0.00f, 0.00f, 0.00f, 1.0f);
   glPushMatrix    (); {
      glTranslatef (my.t_wide + 3,   5.0f,    0.0f);
      glRotatef    ( 90.0, 0.0f, 0.0f, 1.0f);
      glTranslatef (my.t_tall - 10.0,   0.0f,    0.0f);
      yFONT_print  (my.font,  16, YF_BOTRIG, VER_NUM);
   } glPopMatrix   ();
   return 0;
}

char
DRAW_alternate     (void)
{
   /*---(setup view)---------------------*/
   glViewport      ( my.a_left, my.a_bott, my.a_wide, my.a_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         ( 0.0f, my.a_wide, 0.0f, my.a_tall,  -500.0,   500.0);
   glMatrixMode    (GL_MODELVIEW);
   /*---(background)---------------------*/
   glColor4f    (0.00f, 0.00f, 0.60f, 1.0f);
   glPushMatrix    (); {
      glBegin         (GL_POLYGON); {
         glVertex3f  (0.0f     , my.a_tall,  0.0f);
         glVertex3f  (my.a_wide, my.a_tall,  0.0f);
         glVertex3f  (my.a_wide, 0.0f     ,  0.0f);
         glVertex3f  (0.0f     , 0.0f     ,  0.0f);
      } glEnd   ();
   } glPopMatrix   ();
   /*---(progress legend)----------------*/
   glColor4f    (1.00f, 1.00f, 1.00f, 1.0f);
   glPushMatrix    (); {
      glTranslatef (    5.0f, my.a_tall,    0.0f);
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "progress legend");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "femu");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "pate");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "tibi");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "heat 1");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "heat 2");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "heat 3");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "heat 4");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "heat 5");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "heat 6");
      glTranslatef (    0.0f,    -30.0f,    0.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, "heat 7");
   } glPopMatrix   ();
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         leg drawing                          ----===*/
/*====================------------------------------------====================*/
static void      o___LEG_GK__________________o (void) {;}

static double  s_loc      [16];
static double  s_xpos_p   = 0.0;
static double  s_zpos_p   = 0.0;
static double  s_ypos_p   = 0.0;
static double  s_xpos     = 0.0;
static double  s_zpos     = 0.0;
static double  s_ypos     = 0.0;
static double  s_xz       = 0.0;
static double  s_len      = 0.0;

char       /*----: determine the current opengl position ---------------------*/
draw_locate_NEW    (int a_leg, int a_seg, float a_deg)
{
   /*---(locals)-------------------------*/
   char      x_msg [100];
   double    x, y, z;
   /*---(current)------------------------*/
   glGetDoublev(GL_MODELVIEW_MATRIX,  s_loc);
   s_xpos     = s_loc[12];
   s_zpos     = s_loc[14];
   s_ypos     = s_loc[13];
   x          = s_xpos - s_xpos_p;
   z          = s_zpos - s_zpos_p;
   y          = s_ypos - s_ypos_p;
   s_len      = sqrt ((x * x) + (z * z) + (y * y));
   /*---(draw)---------------------------*/
   glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
   glPushMatrix (); {
      /*---(label)-------*/
      glTranslatef(    0.0  ,    45.0 ,     0.0  );
      yFONT_print (my.font,  3, YF_TOPLEF, segs_long [a_seg]);
      /*---(x_pos)-------*/
      glTranslatef(    0.0  ,    -5.0 ,     0.0  );
      snprintf (x_msg, 20, "x = %.1lf", s_xpos);
      yFONT_print (my.font,  3, YF_TOPLEF, x_msg);
      /*---(z_pos)-------*/
      glTranslatef(    0.0  ,    -5.0 ,     0.0  );
      snprintf (x_msg, 20, "z = %.1lf", s_zpos);
      yFONT_print (my.font,  3, YF_TOPLEF, x_msg);
      /*---(y_pos)-------*/
      glTranslatef(    0.0  ,    -5.0 ,     0.0  );
      snprintf (x_msg, 20, "y = %.1lf", s_ypos);
      yFONT_print (my.font,  3, YF_TOPLEF, x_msg);
      /*---(len)---------*/
      glTranslatef(    0.0  ,    -5.0 ,     0.0  );
      snprintf (x_msg, 20, "l = %.1lf", s_len);
      yFONT_print (my.font,  3, YF_TOPLEF, x_msg);
      /*---(done)--------*/
   } glPopMatrix ();
   /*---(save)---------------------------*/
   s_xpos_p   = s_xpos;
   s_zpos_p   = s_zpos;
   s_ypos_p   = s_ypos;
   /*---(save to yKINE)------------------*/
   yKINE_opengl   (a_leg, a_seg, a_deg, s_xpos, s_zpos, s_ypos, s_len);
   if (a_seg == YKINE_COXA)
      yKINE_opengl   (a_leg, YKINE_TROC, a_deg, s_xpos, s_zpos, s_ypos, 0.0);
   if (a_seg == YKINE_TIBI) {
      yKINE_opengl   (a_leg, YKINE_META,   0.0, s_xpos, s_zpos, s_ypos, 0.0);
      yKINE_opengl   (a_leg, YKINE_TARS,   0.0, s_xpos, s_zpos, s_ypos, 0.0);
      yKINE_opengl   (a_leg, YKINE_FOOT,   0.0, s_xpos, s_zpos, s_ypos, 0.0);
      yKINE_inverse  (a_leg, s_xpos, s_zpos, s_ypos);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
TEST_seg           (void)
{
   return 0;
}

char       /*----: determine the current opengl position ---------------------*/
draw_locate        (tSEG *a_curr, tSEG *a_prev, tSEG *a_orig)
{
   /*---(locals)-------------------------*/
   float     d;
   float     l;
   float     x,  y,  z,  xz;
   float     px, pz, py;
   float     ox, oz, oy;
   /*---(origin)-------------------------*/
   ox         = a_orig->cx;
   oz         = a_orig->cz;
   oy         = a_orig->cy;
   /*---(current)------------------------*/
   glGetDoublev(GL_MODELVIEW_MATRIX,  my_location);
   x          = a_curr->cx  = my_location[12];
   z          = a_curr->cz  = my_location[14];
   y          = a_curr->cy  = my_location[13];
   xz         = a_curr->cxz = sqrt((x * x) + (z * z));
   y         -= a_orig->cy;
   l          = a_curr->fl  = sqrt((x * x) + (z * z) + (y * y));
   /*---(previous)-----------------------*/
   if (a_prev != NULL) {
      px         = a_prev->cx;
      pz         = a_prev->cz;
      py         = a_prev->cy - a_orig->cy;
   } else {
      px         = x;
      pz         = z;
      py         = y;
   }
   /*---(segment distance)---------------*/
   a_curr->x  = x        -= px;
   a_curr->z  = z        -= pz;
   a_curr->y  = y        -= py;
   a_curr->xz = sqrt((x * x) + (z * z));
   a_curr->sl = sqrt((x * x) + (z * z) + (y * y));
   /*---(complete)-----------------------*/
   return 0;
}


char       /*----: draw the leg to opengl ------------------------------------*/
draw_leg_NEW       (int a_leg, float a_body, float a_coxa, float a_femu, float a_pate, float a_tibi)
{
   /*---(locals)-------------------------*/
   float     d;
   float     x,  y,  z,  xz,  l;
   char      x_msg [100];
   /*---(prepare)------------------------*/
   s_xpos = s_xpos_p = 0.0;
   s_zpos = s_zpos_p = 0.0;
   s_ypos = s_ypos_p = 0.0;
   s_len  = 0.0;
   /*---(begin)--------------------------*/
   glPushMatrix (); {
      /*---(thorax)----------------------*/
      glTranslatef (a_body,  0.0,  0.0f);
      draw_locate_NEW (a_leg, YKINE_THOR, a_coxa);
      if (a_leg == my.p_leg) glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      glPushMatrix (); {
         /*---(label)-------*/
         snprintf (x_msg, 10, "#%d/%s", a_leg, legs_name [a_leg]);
         glTranslatef(    0.0  ,   -15.0 ,     0.0  );
         yFONT_print (my.font, 12, YF_TOPLEF, x_msg);
         /*---(desc)--------*/
         snprintf (x_msg, 25, "%s", legs_long [a_leg]);
         glTranslatef(    0.0  ,   -18.0 ,     0.0  );
         yFONT_print (my.font,  5, YF_TOPLEF, x_msg);
         /*---(prep)--------*/
         glTranslatef(   20.0  ,     0.0 ,     0.0  );
         /*---(coxa)--------*/
         glTranslatef(    0.0  ,    -8.0 ,     0.0  );
         yFONT_print (my.font,  5, YF_TOPRIG, "coxa");
         snprintf (x_msg, 25, ": %+.1f", a_coxa);
         yFONT_print (my.font,  5, YF_TOPLEF, x_msg);
         /*---(femu)--------*/
         glTranslatef(    0.0  ,    -8.0 ,     0.0  );
         yFONT_print (my.font,  5, YF_TOPRIG, "femur");
         snprintf (x_msg, 25, ": %+.1f", a_femu);
         yFONT_print (my.font,  5, YF_TOPLEF, x_msg);
         /*---(pate)--------*/
         glTranslatef(    0.0  ,    -8.0 ,     0.0  );
         yFONT_print (my.font,  5, YF_TOPRIG, "patella");
         snprintf (x_msg, 25, ": %+.1f", a_pate);
         yFONT_print (my.font,  5, YF_TOPLEF, x_msg);
         /*---(tibi)--------*/
         glTranslatef(    0.0  ,    -8.0 ,     0.0  );
         yFONT_print (my.font,  5, YF_TOPRIG, "tibia");
         snprintf (x_msg, 25, ": %+.1f", a_tibi);
         yFONT_print (my.font,  5, YF_TOPLEF, x_msg);
         /*---(done)--------*/
      } glPopMatrix ();
      /*---(coxa)------------------------*/
      glCallList (dl_coxa);
      draw_locate_NEW (a_leg, YKINE_COXA, a_coxa);
      /*---(femur)--------------------------*/
      glRotatef  ( a_femu, 0.0f, 1.0f, 0.0f);
      glCallList (dl_femur);
      draw_locate_NEW (a_leg, YKINE_FEMU, a_femu);
      /*---(patella)------------------------*/
      glRotatef  (-a_pate, 0.0f, 0.0f, 1.0f);
      glCallList (dl_patella);
      draw_locate_NEW (a_leg, YKINE_PATE, a_pate);
      /*---(tibia)--------------------------*/
      glRotatef  (-a_tibi, 0.0f, 0.0f, 1.0f);
      glCallList (dl_tibia);
      draw_locate_NEW (a_leg, YKINE_TIBI, a_tibi);
   } glPopMatrix ();
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: draw the leg to opengl ------------------------------------*/
draw_leg           (int a_num, tSEG a_curr[], char a_loc)
{
   /*---(locals)-------------------------*/
   float     d;
   float     x,  y,  z;
   float     px, py, pz;
   char      msg [100];
   /*---(begin)--------------------------*/
   glPushMatrix (); {
      /*---(core)------------------------*/
      if (a_loc == 'y') draw_locate (&a_curr[YKINE_CORE], NULL, &a_curr[YKINE_CORE]);
      /*---(thorax)----------------------*/
      d = a_curr[YKINE_THOR].d;
      a_curr[YKINE_THOR].h   = d * DEG2RAD;
      glTranslatef(a_curr[YKINE_THOR].l,  0.0,  0.0f);
      if (a_loc == 'y') draw_locate (&a_curr[YKINE_THOR], &a_curr[YKINE_CORE], &a_curr[YKINE_CORE]);
      if (a_num == my_curr) glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      glPushMatrix (); {
         snprintf (msg, 10, "#%d/%s", a_num, legs_name [a_num]);
         glTranslatef(    0.0  ,   -15.0 ,     0.0  );
         yFONT_print (my.font, 12, YF_TOPLEF, msg);
         snprintf (msg, 25, "%s", legs_long[a_num]);
         glTranslatef(    0.0  ,   -18.0 ,     0.0  );
         yFONT_print (my.font,  5, YF_TOPLEF, msg);
      } glPopMatrix ();
      /*---(coxa)------------------------*/
      d = a_curr[YKINE_COXA].d;
      a_curr[YKINE_COXA].h   = d * DEG2RAD;
      glCallList(dl_coxa);
      if (a_loc == 'y') draw_locate (&a_curr[YKINE_COXA], &a_curr[YKINE_THOR], &a_curr[YKINE_CORE]);
      /*---(femur)--------------------------*/
      d = a_curr[YKINE_FEMU].d;
      a_curr[YKINE_FEMU].h   = d * DEG2RAD;
      glRotatef(d, 0.0f, 1.0f, 0.0f);
      glCallList(dl_femur);
      if (a_loc == 'y') draw_locate (&a_curr[YKINE_FEMU], &a_curr[YKINE_COXA], &a_curr[YKINE_CORE]);
      /*---(patella)------------------------*/
      d = a_curr[YKINE_PATE].d;
      a_curr[YKINE_PATE].v   = d * DEG2RAD;
      glRotatef(d, 0.0f, 0.0f, 1.0f);
      glCallList(dl_patella);
      if (a_loc == 'y') draw_locate (&a_curr[YKINE_PATE], &a_curr[YKINE_FEMU], &a_curr[YKINE_CORE]);
      /*---(tibia)--------------------------*/
      d = a_curr[YKINE_TIBI].d;
      a_curr[YKINE_TIBI].v   = d * DEG2RAD;
      glRotatef(d, 0.0f, 0.0f, 1.0f);
      /*---(text)---------------------------*/
      if (a_num == my_curr) glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      /*> glPushMatrix (); {                                                          <* 
       *>    snprintf (msg, 10, "%d", a_num);                                         <* 
       *>    glTranslatef(    0.0  ,    43.0 ,    10.0  );                            <* 
       *>    yFONT_print (my.font, 18, YF_TOPLEF, msg);                                <* 
       *>    glTranslatef(    0.0  ,     0.0 ,   -20.0  );                            <* 
       *>    yFONT_print (my.font, 18, YF_TOPLEF, msg);                                <* 
       *> } glPopMatrix ();                                                           <*/
      glCallList(dl_tibia);
      if (a_loc == 'y') draw_locate (&a_curr[YKINE_TIBI], &a_curr[YKINE_PATE], &a_curr[YKINE_CORE]);
      /*---(tarsus)-------------------------*/
      if (a_loc == 'y') draw_locate (&a_curr[YKINE_TARS], &a_curr[YKINE_TIBI], &a_curr[YKINE_CORE]);
   } glPopMatrix ();
   /*---(information box)----------------*/
   /*> glPushMatrix (); {                                                             <* 
    *>    snprintf (msg, 10, "%d", a_num);                                            <* 
    *>    glTranslatef(    0.0  ,    43.0 ,    10.0  );                               <* 
    *>    yFONT_print (my.font, 18, YF_TOPLEF, msg);                                   <* 
    *>    glTranslatef(    0.0  ,     0.0 ,   -20.0  );                               <* 
    *>    yFONT_print (my.font, 18, YF_TOPLEF, msg);                                   <* 
    *> } glPopMatrix ();                                                              <*/
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          environment                         ----===*/
/*====================------------------------------------====================*/
static void      o___ENVIRON_________________o (void) {;}

char       /* ---- : establish rational drawing settings ---------------------*/
DRAW_begin         (void)
{
   /*---(color)--------------------------*/
   glClearColor    (0.1f, 0.1f, 0.1f, 1.0f);
   glClearDepth    (1.0f);
   /*---(textures)-----------------------*/
   glEnable        (GL_TEXTURE_2D);    /* NEW */
   /*---(blending)-----------------------*/
   glShadeModel    (GL_SMOOTH);
   glEnable        (GL_DEPTH_TEST);
   glEnable        (GL_ALPHA_TEST);
   glAlphaFunc     (GL_GEQUAL, 0.0125);
   glEnable        (GL_BLEND);
   glBlendFunc     (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDepthFunc     (GL_LEQUAL);
   /*---(anti-aliasing)------------------*/
   glHint          (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /*---(special polygon antialiasing)----------*/
   glEnable        (GL_POLYGON_SMOOTH);
   glPolygonMode   (GL_FRONT_AND_BACK, GL_FILL);
   glHint          (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   /*---(simple defaulting)--------------*/
   glLineWidth     (0.50f);
   /*---(process immediately)------------*/
   glFlush       ();
   /*---(complete)-----------------------*/
   return 0;
}

char       /* ---- : reset drawing environment for a drawing run -------------*/
DRAW_reset         (void)
{
   glLineWidth     (0.50f);
   glPointSize     (5.00f);
   return 0;
}

char       /* ---- : teardown drawing environment ----------------------------*/
DRAW_end           (void)
{
   return 0;
}

char
draw_setup ()
{
   int       i;
   float     d;
   for(i = 0; i < LEGS; ++i) {
      /*---(fix angle)--------------------*/
      d = (float) (i - 1) * 60.0f;
      if (d > 180.0) d  -= 360.0;
      /*---(set height)-------------------*/
      /*---(set degrees)------------------*/
      if (umake_leg == 'y') {
         /*> gk[i][YKINE_THOR].d    +=  arg_thor;                                           <* 
          *> gk[i][YKINE_COXA].d    +=  arg_thor;                                           <*/
         /*> gk[i][YKINE_THOR].d     =  d      + arg_thor;                                  <* 
          *> gk[i][YKINE_COXA].d     =  d      + arg_thor;                                  <*/
         gk[i][YKINE_FEMU].d     =  arg_femu;
         gk[i][YKINE_PATE].d     =  arg_pate;
         gk[i][YKINE_TIBI].d     =  arg_tibi;
         gk[i][YKINE_TARS].d     =    0.0f;
      } else {
         /*> gk[i][YKINE_THOR].d     =  d      + arg_thor;                                  <* 
          *> gk[i][YKINE_COXA].d     =  d      + arg_thor;                                  <*/
         /*> gk[i][YKINE_THOR].d    +=  arg_thor;                                           <* 
          *> gk[i][YKINE_COXA].d    +=  arg_thor;                                           <*/
         /*> gk[i][YKINE_FEMU].d     =    0.0f;                                             <* 
          *> gk[i][YKINE_PATE].d     =    0.0f;                                             <* 
          *> gk[i][YKINE_TIBI].d     =  -90.0f;                                             <*/
         /*> gk[i][YKINE_FEMU].d     =    0.0f;                                             <* 
          *> gk[i][YKINE_PATE].d     =   20.0f;                                             <* 
          *> gk[i][YKINE_TIBI].d     =  -90.0f;                                             <*/
         /*> gk[i][YKINE_FEMU].d     =   30.0f;                                                <* 
          *> gk[i][YKINE_PATE].d     =   45.0f;                                                <* 
          *> gk[i][YKINE_TIBI].d     =  -90.0f;                                                <*/
         /*> gk[i][YKINE_FEMU].d     =    0.0f;                                                <* 
          *> gk[i][YKINE_PATE].d     =    0.0f;                                                <* 
          *> gk[i][YKINE_TIBI].d     =    0.0f;                                                <*/
         gk[i][YKINE_TARS].d     =    0.0f;
      }
   }
   if (arg_y != 0.0) {
      printf("setting center to %8.1f\n", arg_y);
      center.by = arg_y;
   }
   /*> my_y   = lens[YKINE_TIBI];                                                           <*/
   my_y   = center.by;
   /*-------- label   min     max   start    step   minor   major -----*/
   yGOD_axis( 'v',      0,    360,     25,      1,      5,     45);
   yGOD_axis( 'o',      0,    360,     20,      1,      5,     45);
   yGOD_axis( 's',      0,    360,      0,      1,      5,     45);
   yGOD_axis( 'c',   -600,    600,      0,      1,      5,     50);
   yGOD_axis( 'b',    -75,     75,    -10,      1,      5,     50);
   yGOD_axis( 'd',   -150,    300,     75,      1,      5,     50);
   return 0;
}

char       /* ---- : driver for main drawing cycle ---------------------------*/
draw_main          (void)
{
   /*---(locals)-------*-----------------*/
   char        rc          = 0;
   /*---(prepare)------------------------*/
   draw_spider ();
   glClear         (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*---(normal)-------------------------*/
   if (my.scrn == SCRN_NORM) {
      /*> if      (flag_view == 1) view_hildebrand ();                                   <* 
       *> else if (flag_view == 2) view_waves  ();                                       <* 
       *> else if (flag_view == 3) view_torque ();                                       <* 
       *> else if (flag_view == 4) view_step   ();                                       <* 
       *> else                     view_gait   ();                                       <*/
      /*> view_top    ();                                                                <*/
      view_3d     ();
      rc = TICK_show   ();
      DRAW_command     ();
      DRAW_title       ();
      /*> DRAW_alternate   ();                                                        <*/
      TICK_legend      ();
      /*> view_leg    ();                                                                <*/
   }
   /*---(progress)-----------------------*/
   else if (SCRN_PROG) {
      rc = TICK_full   ();
   }
   /*---(send for processing)------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   /*---(complete)-----------------------*/
   return rc;
}

char       /* ---- : display key body information ----------------------------*/
draw__center       (void)
{
   /*---(locals)-------*-----------------*/
   char      msg [200] = "";
   /*---(prepare)------------------------*/
   glLineWidth ( 0.50f);
   glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
   /*---(upper bar)----------------------*/
   glColor4f(1.00f, 0.00f, 0.00f, 0.10f);
   glPushMatrix (); {
      glTranslatef( -70.00f,  75.00f,  -2.00f);
      glBegin (GL_POLYGON);
      glVertex3f (    0.00f,    0.00f,    0.00f);
      glVertex3f (  140.00f,    0.00f,    0.00f);
      glVertex3f (  140.00f,  -50.00f,    0.00f);
      glVertex3f (    0.00f,  -50.00f,    0.00f);
      glEnd   ();
   } glPopMatrix ();
   /*---(upper bar)----------------------*/
   glColor4f(1.00f, 0.00f, 0.00f, 1.00f);
   glPushMatrix (); {
      glTranslatef( -70.00f,  75.00f,   0.00f);
      glLineWidth (2.00f);
      glBegin (GL_LINES);
      glVertex3f (    0.00f,    0.00f,    0.00f);
      glVertex3f (  140.00f,    0.00f,    0.00f);
      glEnd   ();
      glLineWidth (0.50f);
   } glPopMatrix ();
   /*---(position)-----------------------*/
   glPushMatrix (); {
      glTranslatef( -65.00f,  70.00f,   0.00f);
      snprintf (msg, 30, "cen x  = %7.2f", center.bx);
      yFONT_print (my.font,   5, YF_TOPLEF, msg);
      glTranslatef(  0.00f, -9.00f,   0.00f);
      snprintf (msg, 30, "cen z  = %7.2f", center.bz);
      yFONT_print (my.font,   5, YF_TOPLEF, msg);
      glTranslatef(  0.00f, -9.00f,   0.00f);
      snprintf (msg, 30, "cen y  = %7.2f", center.by);
      yFONT_print (my.font,   5, YF_TOPLEF, msg);
   } glPopMatrix ();
   /*---(complete)-----------------------*/
   return 0;
}

void
draw_spider        (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   int         x_leg       = 0;
   int         x_servo     = 0;
   double      x_coxa      = 0.0;
   double      x_femu      = 0.0;
   double      x_pate      = 0.0;
   double      x_tibi      = 0.0;
   double      x_xpos1     = 0.0;
   double      x_zpos1     = 0.0;
   double      x_ypos1     = 0.0;
   double      x_xpos2     = 0.0;
   double      x_zpos2     = 0.0;
   double      x_ypos2     = 0.0;
   char        x_debug     = '-';
   char        rc          = 0;
   /*> draw_axis();                                                                   <*/
   DRAW_reset  ();
   if (dl_spider != 0) glDeleteLists(dl_spider, 1);
   dl_spider = glGenLists(1);
   glNewList       (dl_spider, GL_COMPILE_AND_EXECUTE);
   /*---(begin)--------------------------*/
   glPushMatrix (); {
      /*> draw_masscenter ();                                                         <*/
      glTranslatef    (center.bx,      0.0  , center.bz);
      /*> draw_arrow      ();                                                         <*/
      /*> if (flag_annotate == 'y')  draw__center ();                                 <*/
      glCallList      (dl_body);
      yKINE_move_curall ( my.p_cursec);
      for (x_leg = 0; x_leg < 6; ++x_leg) {
         glPushMatrix (); {
            /*---(prepare)---------------*/
            glColor3f(1.0f, 1.0f, 1.0f);
            glRotatef( legs_deg [x_leg], 0.0f, 1.0f, 0.0f);
            /*---(default values)--------*/
            x_coxa  = legs_deg [x_leg];
            /*> x_femu  =  0.0;                                                       <*/
            /*> x_pate  =  0.0;                                                       <*/
            /*> x_tibi  = 90.0;                                                       <*/
            /*---(check servos)----------*/
            x_servo = x_leg * 3;
            rc = yKINE_servo_deg  (x_leg, YKINE_FEMU, &x_femu);
            rc = yKINE_servo_deg  (x_leg, YKINE_PATE, &x_pate);
            rc = yKINE_servo_deg  (x_leg, YKINE_TIBI, &x_tibi);
            /*> if (g_servos [  x_servo].curr != NULL)  my.s_femu = x_femu = g_servos [x_servo].deg;   <* 
             *> if (g_servos [++x_servo].curr != NULL)  my.s_pate = x_pate = g_servos [x_servo].deg;   <* 
             *> if (g_servos [++x_servo].curr != NULL)  my.s_tibi = x_tibi = g_servos [x_servo].deg;   <*/
            /*---(draw)------------------*/
            draw_leg_NEW   (x_leg, segs_len [YKINE_THOR], x_coxa, x_femu, x_pate, x_tibi);
            /*---(calc in yKINE)---------*/
            yKINE_forward  (x_leg, x_femu, x_pate, x_tibi);
            KINE_write     (x_leg);
            /*---(done)------------------*/
         } glPopMatrix ();
         glPushMatrix (); {
            /*---(figure move line)------*/
            x_debug = '-';
            /*> if (strcmp (g_servos [x_servo].label, "RR.tibi") == 0) x_debug = 'y';   <*/
            /*> if (x_debug == 'y') {                                                 <*/
            /*> if (x_debug == 'y')  printf ("name = %s\n", g_servos [x_servo].label);          <* 
             *> if (x_debug == 'y')  printf ("curr = %p\n", g_servos [x_servo].curr);           <* 
             *> if (x_debug == 'y')  printf ("prev = %p\n", g_servos [x_servo].curr->s_prev);   <*/
            rc = yKINE_servo_line (x_leg, YKINE_TIBI, &x_xpos1, &x_zpos1, &x_ypos1, &x_xpos2, &x_zpos2, &x_ypos2);
            /*> if (g_servos [x_servo].curr->s_prev != NULL) {                                                        <* 
             *>    x_xpos1 = g_servos [x_servo].curr->s_prev->x_pos;                                                  <* 
             *>    x_zpos1 = g_servos [x_servo].curr->s_prev->z_pos;                                                  <* 
             *>    x_ypos1 = g_servos [x_servo].curr->s_prev->y_pos;                                                  <* 
             *>    if (x_debug == 'y')  printf ("   pos1  %8.1lfx, %8.1lfz, %8.1lfy\n", x_xpos1, x_zpos1, x_ypos1);   <* 
             *>    x_xpos2 = g_servos [x_servo].curr->x_pos;                                                          <* 
             *>    x_zpos2 = g_servos [x_servo].curr->z_pos;                                                          <* 
             *>    x_ypos2 = g_servos [x_servo].curr->y_pos;                                                          <* 
             *>    if (x_debug == 'y')  printf ("   pos2  %8.1lfx, %8.1lfz, %8.1lfy\n", x_xpos2, x_zpos2, x_ypos2);   <*/
            if (rc >= 0) {
               glLineWidth  ( 2.00f);
               glColor4f    (1.0f, 0.0f, 0.0f, 1.0f);
               glBegin      (GL_LINES); {
                  glVertex3f  (x_xpos1, x_ypos1, x_zpos1);
                  glVertex3f  (x_xpos2, x_ypos2, x_zpos2);
               } glEnd   ();
               glColor4f    (1.0f, 0.0f, 1.0f, 1.0f);
               glBegin      (GL_POINTS); {
                  glVertex3f  (x_xpos1, x_ypos1, x_zpos1);
                  glVertex3f  (x_xpos2, x_ypos2, x_zpos2);
               } glEnd   ();
            }
            /*> }                                                                     <*/
         } glPopMatrix ();
      }
      /*> draw_contact    ();                                                         <*/
   } glPopMatrix ();
   glEndList();
   /*---(complete)-----------------------*/
   return;
}

char
view_progress      (void)
{
   int         i           = 0;
   char        x_msg       [200];
   /*---(show current position)----------*/
   glPushMatrix(); {
      glLineWidth  ( 2.00f);
      glColor4f    (0.2f, 0.2f, 0.2f, 1.0f);
      glBegin      (GL_POLYGON); {
         glVertex3f  (  -2,  -22.0, -110.0);
         glVertex3f  (   2,  -22.0, -110.0);
         glVertex3f  (   2,  -43.0, -110.0);
         glVertex3f  (  -2,  -43.0, -110.0);
      } glEnd   ();
   } glPopMatrix();
   /*---(progress bar)-------------------*/
   glPushMatrix(); {
      glLineWidth  (30.00f);
      for (i = 0; i < 10; ++i) {
         if (i % 2 == 0)  glColor4f    (1.0f, 0.5f, 0.0f, 1.0f);
         else             glColor4f    (0.5f, 0.2f, 0.0f, 1.0f);
         glBegin         (GL_POLYGON); {
            glVertex3f  ( -30 + (i + 0) * 6,  -24.3, -100.0);
            glVertex3f  ( -30 + (i + 1) * 6,  -24.3, -100.0);
            glVertex3f  ( -30 + (i + 1) * 6,  -25.7, -100.0);
            glVertex3f  ( -30 + (i + 0) * 6,  -25.7, -100.0);
         } glEnd   ();
         /*> glBegin         (GL_POLYGON); {                                          <* 
          *>    glVertex3f  ( -30 + (i + 0) * 6,  -29.8, -100.0);                     <* 
          *>    glVertex3f  ( -30 + (i + 1) * 6,  -29.8, -100.0);                     <* 
          *>    glVertex3f  ( -30 + (i + 1) * 6,  -30.2, -100.0);                     <* 
          *>    glVertex3f  ( -30 + (i + 0) * 6,  -30.2, -100.0);                     <* 
          *> } glEnd   ();                                                            <*/
         glBegin         (GL_POLYGON); {
            glVertex3f  ( -30 + (i + 0) * 6,  -34.3, -100.0);
            glVertex3f  ( -30 + (i + 1) * 6,  -34.3, -100.0);
            glVertex3f  ( -30 + (i + 1) * 6,  -35.7, -100.0);
            glVertex3f  ( -30 + (i + 0) * 6,  -35.7, -100.0);
         } glEnd   ();
      }
   } glPopMatrix();
   /*---(progress bar labels)------------*/
   glPushMatrix(); {
      glTranslatef ( -30.0f, -28.0, -100.0);
      glColor4f    (1.0f, 1.0f, 1.0f, 1.0f);
      for (i = 0; i < 10; ++i) {
         snprintf     (x_msg, 100, "%d", i);
         yFONT_print  (my.font,   1, YF_MIDCEN, x_msg);
         glTranslatef (   0.0f,  -4.0,    0.0);
         yFONT_print  (my.font,   1, YF_MIDCEN, x_msg);
         glTranslatef (   6.0f,   4.0,    0.0);
      }
   } glPopMatrix();
   /*---(movement axis)------------------*/
   glPushMatrix(); {
      glColor4f    (1.0f, 0.5f, 0.0f, 1.0f);
      glLineWidth  ( 3.00f);
      for (i = -31; i <= 31; i += 62) {
         glBegin      (GL_LINES); {
            glVertex3f  (   i,  -20.0, -100.0);
            glVertex3f  (   i,  -24.8, -100.0);
            glVertex3f  (   i,  -25.2, -100.0);
            glVertex3f  (   i,  -29.8, -100.0);
            glVertex3f  (   i,  -30.2, -100.0);
            glVertex3f  (   i,  -34.8, -100.0);
            glVertex3f  (   i,  -35.2, -100.0);
            glVertex3f  (   i,  -40.0, -100.0);
         } glEnd   ();
      }
   } glPopMatrix();
   /*---(movement axis labels)-----------*/
   glPushMatrix(); {
      glColor4f    (1.0f, 0.5f, 0.0f, 1.0f);
      glTranslatef ( -30.0f, -20.0, -100.0);
      yFONT_print  (my.font,   1, YF_MIDLEF, "+100");
      glTranslatef (  60.0f,   0.0,    0.0);
      yFONT_print  (my.font,   1, YF_MIDRIG, "+100");
      glTranslatef ( -60.0f, -20.0,    0.0);
      yFONT_print  (my.font,   1, YF_MIDLEF, "-100");
      glTranslatef (  60.0f,   0.0,    0.0);
      yFONT_print  (my.font,   1, YF_MIDRIG, "-100");
   } glPopMatrix();
   /*---(show scale notation)------------*/
   /*> glPushMatrix(); {                                                                                                               <* 
    *>    glColor4f    (1.0f, 0.5f, 0.0f, 1.0f);                                                                                       <* 
    *>    glTranslatef ( -20.0f, -40.0, -100.0);                                                                                       <* 
    *>    snprintf     (x_msg, 100, "%s.%-6s.%s", g_scale [my.p_scale].code, g_scale [my.p_scale].label, g_scale [my.p_scale].desc);   <* 
    *>    yFONT_print  (my.font,   1, YF_MIDLEF, x_msg);                                                                                <* 
    *> } glPopMatrix();                                                                                                                <*/

   /*---(show leg angle curves)----------*/
   glPushMatrix(); {
      glLineWidth  ( 2.00f);
      glColor4f    (1.0f, 0.0f, 0.0f, 1.0f);
      glBegin(GL_LINE_STRIP); {
         glVertex3f  ( -30,  -24.0, -100.0);
         glVertex3f  ( -10,  -20.0, -100.0);
         glVertex3f  (  -5,  -20.0, -100.0);
         glVertex3f  (  10,  -35.0, -100.0);
         glVertex3f  (  25,  -40.0, -100.0);
         glVertex3f  (  30,  -35.0, -100.0);
      } glEnd   ();
      glPointSize  ( 6.00f);
      glBegin      (GL_POINTS); {
         glVertex3f  ( -10,  -20.0, -100.0);
         glVertex3f  (  -5,  -20.0, -100.0);
         glVertex3f  (  10,  -35.0, -100.0);
         glVertex3f  (  25,  -40.0, -100.0);
      } glEnd   ();
   } glPopMatrix();
   /*---(show leg angle curves)----------*/
   glPushMatrix(); {
      glLineWidth  ( 2.00f);
      glColor4f    (0.5f, 1.0f, 0.0f, 1.0f);
      glBegin(GL_LINE_STRIP); {
         glVertex3f  ( -30,  -30.0, -100.0);
         glVertex3f  (  30,  -30.0, -100.0);
      } glEnd   ();
   } glPopMatrix();
   /*---(show leg angle curves)----------*/
   glPushMatrix(); {
      glLineWidth  ( 2.00f);
      glColor4f    (0.0f, 0.5f, 1.0f, 1.0f);
      glBegin(GL_LINE_STRIP); {
         glVertex3f  ( -30,  -25.0, -100.0);
         glVertex3f  (  30,  -25.0, -100.0);
      } glEnd   ();
   } glPopMatrix();

   return 0;

   glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
   glBegin (GL_LINES);
   glVertex3f( (56.0 / MAX_POS) * gait.pos - 29,  -40.0, -100.0);
   glVertex3f( (56.0 / MAX_POS) * gait.pos - 27,  -40.0, -100.0);
   glEnd   ();
   glLineWidth(0.5f);
   glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
   glPushMatrix(); {
      glTranslatef( -28.0f, -39.0, -100.0);
      snprintf (x_msg, 50, "%03d", 0);
      yFONT_print (my.font,   2, YF_BOTLEF, x_msg);
   } glPopMatrix();
   glPushMatrix(); {
      glTranslatef ( -45.0f,  40.0 , -100.0);
      glRotatef    (  90.0 ,   0.0f,    0.0f,   1.0f);
      snprintf (x_msg, 100, "%s, %s", model_desc, model_name);
      yFONT_print  (my.font,   2, YF_BOTRIG, x_msg);
   } glPopMatrix();
   /*> glPushMatrix(); {                                                              <* 
    *>    glTranslatef ( -53.0f,  38.5 , -100.0);                                     <* 
    *>    yFONT_print  (my.font,   2, YF_BOTLEF, model_desc);                          <* 
    *> } glPopMatrix();                                                               <*/
   glPushMatrix(); {
      glTranslatef(  28.0f, -39.0, -100.0);
      snprintf (x_msg, 50, "%03d", MAX_POS);
      yFONT_print (my.font,   2, YF_BOTRIG, x_msg);
   } glPopMatrix();
   glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
   glPushMatrix(); {
      glTranslatef(   0.0f, -39.0, -100.0);
      snprintf (x_msg, 50, "%03d", (int) my.p_cursec);
      yFONT_print (my.font,   2, YF_BOTCEN, x_msg);
   } glPopMatrix();
   return 0;
}

void
view_3d()
{
   /*---(setup view)---------------------*/
   glViewport      (my.s_left, my.s_bott, my.s_wide, my.s_tall);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   gluPerspective  (45.0f, (GLfloat) my.s_wide / (GLfloat) my.s_tall, 0.01f, 4000.0f);
   glMatrixMode    (GL_MODELVIEW);

   DRAW_reset      ();
   glPushMatrix    (); {
      glTranslatef ( -(my.s_wide / 18.0),  -32.0f, -100.0f);
      yGOD_orient ();
      glTranslatef (  (my.s_wide /  9.0),    0.0f,    0.0f);
      yGOD_locate ();
   } glPopMatrix   ();
   /*---(setup view)---------------------*/
   glPushMatrix    (); {
      /*> yGOD_orient_xzy (  40, -100,  -33);                                         <* 
       *> yGOD_locate_xzy ( -40, -100,  -33);                                         <*/
      /*> yGOD_orient ();                                                             <* 
       *> yGOD_locate ();                                                             <*/
      /*> view_progress   ();                                                         <*/
      glLineWidth ( 0.50f);
      yGOD_view       ();
      /*> glTranslatef    (    0.0  , -center.by,     0.0  );                         <*/
      /*> yGOD_locate ();                                                             <*/
      /*> glLineWidth ( 0.50f);                                                       <*/
      /*> glCallList      (dl_ground);                                                <*/
      /*> glTranslatef    (center.bx,      0.0  , center.bz);                         <*/
      /*> glTranslatef    (      0.0,  center.by,       0.0);                         <*/
      /*---(body)---------------------------*/
      glCallList      (dl_spider);
   } glPopMatrix   ();
   /*---(complete)-----------------------*/
   return;
}

char view_top_curve     (int a_leg, int a_joint, float a_radius, float a_max, float a_z) {
   /*---(locals)-------*-----------------*/
   float     j         = 0;
   float     r, x, z;
   int       d;
   float     xmin      = 0.0;
   float     xmax      = 0.0;
   float     xcur      = 0.0;
   char      msg [200];
   float     xpos      = 0.0;
   /*---(set min/max)--------------------*/
   switch (a_joint) {
   case YKINE_FEMU : xmin = - (M_PI /  2);
                     xmax = + (M_PI /  2);
                     xcur = - gk[a_leg][a_joint].d * DEG2RAD;
                     xpos = 0.0;
                     break;
   case YKINE_PATE : xmin = - (M_PI /  2);
                     xmax = + (M_PI /  4);
                     xcur = - gk[a_leg][a_joint].d * DEG2RAD;
                     xpos = 1.0;
                     break;
   case YKINE_TIBI : xmin = + (M_PI / 36);
                     xmax = + (M_PI * 0.75);
                     xcur = - gk[a_leg][a_joint].d * DEG2RAD;
                     xpos = 2.0;
                     break;
   }
   glLineWidth(5.00f);
   /*---(draw range curve)---------------*/
   glBegin(GL_LINE_STRIP);
   for (j = xmin; j <= xmax; j += M_PI / 36) {
      x   = a_radius * cos(j);
      z   = a_radius * sin(j);
      glVertex3f( x,  a_z, z);
   }
   glEnd();
   /*---(draw current angle)-------------*/
   r   = xcur;
   glBegin(GL_LINES);
   x   = a_radius * cos(r);
   z   = a_radius * sin(r);
   glVertex3f( x, a_z, z);
   x   = a_max    * cos(r);
   z   = a_max    * sin(r);
   glVertex3f( x, a_z, z);
   glEnd();
   /*---(label)--------------------------*/
   glPushMatrix    (); {
      glTranslatef (  350, a_z, - 400.0 + (xpos * 50.0));
      glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
      snprintf     (msg,   100, "%6.3f", xcur);
      yFONT_print  (my.font,   32, YF_TOPLEF, msg);
      glTranslatef (    0.0, -700.0, 0.0);
      snprintf     (msg,   100, "%6.1f", xcur * RAD2DEG);
      yFONT_print  (my.font,   32, YF_TOPLEF, msg);
   } glPopMatrix    ();
   /*---(complete)-----------------------*/
   return 0;
}

void view_top() {
   /*---(locals)-------*-----------------*/
   int       i         = my_curr;
   float     j         = 0;
   float     r, x, z;
   int       d;
   /*===[[ SETUP VIEW ]]==================================*/
   glViewport      (700, 468, 300, 300);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         (-500.0, 500.0 , -500.0, 500.0,-500.0,  500.0);
   glMatrixMode    (GL_MODELVIEW);
   DRAW_reset      ();
   /*---(setup specifics)----------------*/
   glPushMatrix    ();
   glRotatef( 90.0, 1.0f, 0.0f, 0.0f);
   draw_arrow();
   glCallList(dl_spider);
   /*---(draw curves)--------------------*/
   glColor3f(1.0f, 0.0f, 0.0f);
   view_top_curve (my_curr, YKINE_FEMU, 370.0, 450.0,  10.0);
   glColor3f(0.0f, 0.5f, 1.0f);
   view_top_curve (my_curr, YKINE_PATE, 390.0, 450.0,   0.0);
   glColor3f(1.0f, 0.5f, 0.0f);
   view_top_curve (my_curr, YKINE_TIBI, 410.0, 450.0, -10.0);
   /*===[[ PATELLA ]]=====================================*/
   /*> glBegin(GL_LINE_STRIP);                                                                         <* 
    *> for (j = ik[i][YKINE_COXA].v - (M_PI_2); j <= ik[i][YKINE_COXA].v + (M_PI / 4); j += M_PI / 36) {           <* *>    x   = 390.0f * cos(j);                                                                       <* *>    z   = 390.0f * sin(j);                                                                       <* *>    glVertex3f( x, 0.0, z);                                                                      <* *> }                                                                                               <* *> glEnd();                                                                                        <* *> glBegin(GL_LINES);                                                                              <* *> r   = -ik[i][YKINE_PATE].v;                                                                           <* *> x   = 390.0f * cos(r);                                                                          <* *> z   = 390.0f * sin(r);
    <* *> glVertex3f( x,  0, z);                                                                          <* *> x   = 450.0f * cos(r);                                                                          <* *> z   = 450.0f * sin(r);                                                                          <* *> glVertex3f( x,  0, z);                                                                          <* *> glEnd();                                                                                        <* *> /+===[[ TIBIA ]]=======================================+/                                       <* *> glColor3f(0.0f, 0.0f, 1.0f);                                                                    <* *> glBegin(GL_LINE_STRIP);                                                                         <* *> for (j = -ik[i][YKINE_PATE].v + (M_PI / 36); j <= -ik[i][YKINE_PATE].v + (0.75 * M_PI); j += M_PI / 36) {   <* *>    x   = 410.0f * cos(j);
    <* *>    z   = 410.0f * sin(j);                                                                       <* *>    glVertex3f( x,  -9.0, z);                                                                    <* *> }                                                                                               <* *> glEnd();                                                                                        <* *> glBegin(GL_LINES);                                                                              <* *> r   = -ik[i][YKINE_TIBI].v;                                                                           <* *> x   = 410.0f * cos(r);                                                                          <* *> z   = 410.0f * sin(r);                                                                          <* *> glVertex3f( x,  -9.0, z);                                                                       <* *> x   = 450.0f * cos(r);
    <* *> z   = 450.0f * sin(r);                                                                          <* *> glVertex3f( x,  -9.0, z);                                                                       <* *> glVertex3f(15.0, -10.0,  5.0);                                                                  <* *> glVertex3f(17.0, -10.0,  5.0);                                                                  <* *> glEnd();                                                                                        <*/
   /*---(tarsus to origin line)-----------------*/
   glLineWidth( 1.50f);
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0xAAAA);
   glBegin(GL_LINES);
   glColor3f(1.0f, 1.0f, 0.0f);
   glVertex3f( 0.00f, 0.00f, 0.00f);
   glVertex3f( fk[i][YKINE_TARS].cx, fk[i][YKINE_TARS].cy, fk[i][YKINE_TARS].cz);
   glEnd();
   /*===[[ DRAW AXIS ]]==================================*/
   glEnable(GL_LINE_STIPPLE);
   glLineWidth(0.50f);
   /*---(positive X :: blue   course dashed)-------------*/
   glLineStipple(2, 0x0F0F);
   glColor3f(1.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f( 250.00f, -30.00f,   0.00f);
   glVertex3f( 500.00f, -30.00f,   0.00f);
   glEnd();
   /*---(negative X :: blue   fine dotted)---------------*/
   glLineStipple(2, 0xAAAA);
   glColor3f(1.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(-250.00f, -30.00f,   0.00f);
   glVertex3f(-500.00f, -30.00f,   0.00f);
   glEnd();
   /*---(positive Z :: cyan   course dashed)-------------*/
   glLineStipple(2, 0x0F0F);
   glColor3f(0.0f, 1.0f, 1.0f);
   glBegin(GL_LINES);
   glColor3f(0.0f, 1.0f, 1.0f);
   glVertex3f(   0.00f, -30.00f, 250.00f);
   glVertex3f(   0.00f, -30.00f, 500.00f);
   glEnd();
   /*---(negative Z :: cyan   fine dotted)---------------*/
   glLineStipple(2, 0xAAAA);
   glColor3f(0.0f, 1.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(   0.00f, -30.00f,-250.00f);
   glVertex3f(   0.00f, -30.00f,-500.00f);
   glEnd();
   glDisable(GL_LINE_STIPPLE);
   /*===[[ DRAW RADIANS ]]================================*/
   glColor3f(0.0f, 0.0f, 1.0f);
   for (d = 0; d < 365; d += 5) {
      glLineWidth(0.50f);
      if (d % 15 == 0) glLineWidth(2.50f);
      glBegin(GL_LINES);
      r   = d * DEG2RAD;
      x   = 370.0f * cos(r);
      z   = 370.0f * sin(r);
      glVertex3f( x,-20.00f, z);
      if (d % 15 == 0) {
         x   = 430.0f * cos(r);
         z   = 430.0f * sin(r);
      } else {
         x   = 410.0f * cos(r);
         z   = 410.0f * sin(r);
      }
      glVertex3f( x,-20.00f, z);
      glEnd();
      if ((d + 45) % 90 == 0) {
         glLineWidth(2.50f);
         for (i = 0; i < (d + 45)/90; ++i) {
            glBegin(GL_LINES);
            r = (d + (i * 2)) * ((2 * M_PI) / 360);
            x   = 14.0f * cos(r);
            z   = 14.0f * sin(r);
            glVertex3f(x,  0.0f, z);
            x   = 15.0f * cos(r);
            z   = 15.0f * sin(r);
            glVertex3f(x,  0.0f, z);
            glEnd();
         } glLineWidth(0.50f);
      }
   }
   /*> x   = 12.0f * cos(ik[i][6].h);                                                 <* 
    *> z   = 12.0f * sin(ik[i][6].h);                                                 <* *> glVertex3f( x,  -9.0, z);                                                      <* *> x   = 13.0f * cos(ik[i][6].h);                                                 <* *> z   = 13.0f * sin(ik[i][6].h);                                                 <* *> glVertex3f( x,  -9.0, z);                                                      <* *> glVertex3f(15.0, -10.0, -3.0);                                                 <* *> glVertex3f(17.0, -10.0, -3.0);                                                 <* *> glEnd();                                                                       <*/
   /*---(done)----------------------------------*/
   glPopMatrix();
   return;
}

void view_side() {
   int      i = my_curr;
   float    j = 0;
   float    r, x, z;
   int      d;
   /*===[[ SETUP VIEW ]]==================================*/
   glViewport      (  0,   0, 700, 160);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         (-500.0, 500.0 ,  -50.0, 150.0,-500.0,  500.0);
   glMatrixMode    (GL_MODELVIEW);
   glPolygonMode   (GL_FRONT_AND_BACK, GL_FILL);
   DRAW_reset      ();
   glPushMatrix    ();
   glTranslatef( 0.0,  center.by, 0);
   /*> glRotatef(- my_yaw  , 0.0f, 1.0f, 0.0f);                                       <* 
    *> glRotatef(- my_pitch, 1.0f, 0.0f, 0.0f);                                       <* *> glRotatef(- my_roll , 0.0f, 0.0f, 1.0f);                                       <*/
   /*> glCallList      (dl_spider);                                                   <*/
   glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
   /*> glTranslatef( 0.0,  50.0,  100.0);                                             <*/
   /*> yFONT_print (my.font,  20, YF_TOPLEF, "testing");                               <*/
   glPopMatrix();
   return;
}



/*====================------------------------------------====================*/
/*===----                       gait visualization                     ----===*/
/*====================------------------------------------====================*/
static void      o___GAIT_VIS________________o (void) {;}

int       beg, end;

char       /* ---- : visualize number of legs touching -----------------------*/
view_setup         (char *a_title, char *a_desc)
{
   /*---(setup view)---------------------*/
   glViewport      (  0,   0, 700, 160);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         (-500.0, 500.0 ,  -25.0, 175.0,-500.0,  500.0);
   glMatrixMode    (GL_MODELVIEW);
   DRAW_reset      ();
   /*---(defaults)-----------------------*/
   glPolygonMode   (GL_FRONT_AND_BACK, GL_FILL);
   /*---(show title)---------------------*/
   glPushMatrix    (); {
      glTranslatef (-495.00, 75.00, 0.00f);
      glRotatef    ( 90.0, 0.0f, 0.0f, 1.0f);
      glColor4f    (0.5f, 0.0f, 1.0f, 1.0f);
      yFONT_print  (my.font,  18, YF_TOPCEN, a_title);
   } glPopMatrix   ();
   /*---(explanation)--------------------*/
   glPushMatrix    (); {
      glTranslatef (-415.00, 155.00, 0.00f);
      glColor4f    (1.0f, 1.0f, 1.0f, 1.0f);
      yFONT_print  (my.font,  12, YF_BOTLEF, a_desc);
   } glPopMatrix   ();
   /*---(range)--------------------------*/
   beg  =   1;
   end  = 450;
   if (gait.pos >= MAX_POS) gait.pos = MAX_POS - 1;
   if (gait.pos <= 225) {
      beg  =   0;
      end  = 449;
   } else   if (gait.pos >= MAX_POS - 225) {
      beg  = MAX_POS - 450;
      end  = MAX_POS -   1;
   } else {
      beg = gait.pos - 225;
      end = gait.pos + 225;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char       /* ---- : visualize number of legs touching -----------------------*/
view_touch         (void)
{
   /*---(locals)-------*-----------------*/
   int       j         = 0;            /* loop iterator -- position           */
   /*---(touches)------------------------*/
   glPushMatrix    (); {
      glPointSize  (25.00f);
      glLineWidth  ( 0.50f);
      glTranslatef ( -425.00f, 135.00f - (5 * 30.0),  -20.00f);
      for (j = beg; j < end; ++j) {
         switch (gait.touch [j]) {
         case 0  :
         case 1  : glColor4f (1.0, 0.0, 0.0, 0.5); break;
         case 2  : glColor4f (0.7, 0.3, 0.0, 0.5); break;
         case 3  : glColor4f (0.5, 0.5, 0.0, 0.5); break;
         case 4  : glColor4f (0.0, 0.5, 0.5, 0.5); break;
         case 5  :
         case 6  : glColor4f (0.0, 0.0, 1.0, 0.5); break;
         default : glColor4f (0.5, 0.0, 0.5, 0.5); break;
         }
         glBegin (GL_LINES);
         glVertex3f ((j - beg) * 2.00f, 165.00f, -10.00f);
         glVertex3f ((j - beg) * 2.00f,   0.00f, -10.00f);
         glEnd ();
      }
   } glPopMatrix  ();
   return 0;
}

char       /* ---- : visualize number of legs touching -----------------------*/
view_vert          (void)
{
   /*---(locals)-------*-----------------*/
   int       j         = 0;            /* loop iterator -- position           */
   /*---(vertical markers)---------------*/
   glPushMatrix    (); {
      glTranslatef ( -425.00f, 135.00f - (5 * 30.0),    0.00f);
      glColor4f    (1.0f, 1.0f, 1.0f, 1.0f);
      glEnable     (GL_LINE_STIPPLE);
      glLineStipple(1, 0xAAAA);
      glBegin      (GL_LINES);
      for (j = beg; j < end; ++j) {
         if (j % gait.dmax != 0) continue;
         glVertex3f ((j - beg) * 2.00f, 165.0f, 10.00f);
         glVertex3f ((j - beg) * 2.00f,   0.0f, 10.00f);
      }
      glEnd ();
      glDisable    (GL_LINE_STIPPLE);
   } glPopMatrix();
   return 0;
}

char       /* ---- : build a gait visualization ------------------------------*/
view_gait          (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   int       j         = 0;            /* loop iterator -- position           */
   char      msg [100] = "";
   /*---(setup view)---------------------*/
   view_setup  ("gait analysis", "gait = method, pattern, rhythm, and speed of limb movement during ground-based locomotion");
   glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
   for (i = 0; i < LEGS; ++i) {
      glPushMatrix    (); {
         /*---(left title)---------------*/
         glTranslatef ( -455.00f, 135.00f - (i * 25.0),    0.00f);
         snprintf     (msg,  50, "#%d", i);
         yFONT_print  (my.font,  12, YF_TOPLEF, msg);
         /*---(baseline)-----------------*/
         glTranslatef (   30.00f, -10.00f,    0.00f);
         glEnable     (GL_LINE_STIPPLE);
         glLineStipple(1, 0xF0F0);
         glBegin      (GL_LINES);
         glVertex3f   ( -10.00f, 0.00f,   0.00f);
         glVertex3f   ( 903.00f, 0.00f,   0.00f);
         glEnd        ();
         glDisable    (GL_LINE_STIPPLE);
         /*---(gait flow)----------------*/
         glBegin      (GL_LINE_STRIP);
         for (j = beg; j < end; ++j) {
            glVertex3f ((j - beg) * 2.00f, gait.y [j][i], 0.00f);
         }
         glEnd ();
         /*---(current)------------------*/
         glPointSize  (5.00f);
         glBegin      (GL_POINTS);
         glVertex3f   ((gait.pos - beg) * 2.00f, gait.y [gait.pos][i], 0.00f);
         glEnd        ();
         /*---(right title)--------------*/
         glTranslatef (  903.00f,  10.00f,    0.00f);
         yFONT_print  (my.font,  12, YF_TOPLEF, msg);
      } glPopMatrix();
   }
   view_touch ();
   view_vert  ();
   /*---(complete)-----------------------*/
   return 0;
}

char       /* ---- : hildebrand gait visualization ---------------------------*/
view_hildebrand    (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   int       j         = 0;            /* loop iterator -- position           */
   char      msg [100] = "";
   char      on        = 0;
   int       save      = 0;
   /*---(setup view)---------------------*/
   view_setup  ("hildebrand", "gait = method, pattern, rhythm, and speed of limb movement during ground-based locomotion");
   /*---(show name)----------------------*/
   glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
   for (i = 0; i < LEGS; ++i) {
      glPushMatrix    (); {
         glTranslatef ( -455.00f, 135.00f - (i * 25.0),    0.00f);
         snprintf     (msg,  50, "#%d", i);
         yFONT_print  (my.font,  12, YF_TOPLEF, msg);
         glTranslatef (   30.00f, -10.00f,    0.00f);
         on   = 0;
         save = beg;
         glLineWidth ( 8.00f);
         for (j = beg; j < end; ++j) {
            if (gait.tleg [j][i] != 1 ) {
               glVertex3f ((save - beg) * 2.00f, 0.00f, 0.00f);
               glEnd ();
               on = 0;
               continue;
            }
            if (on == 0) glBegin (GL_LINE_STRIP);
            glVertex3f ((j - beg) * 2.00f, 0.00f, 0.00f);
            ++on;
            save = j;
         }
         if (on == 1) glVertex3f ((save - beg) * 2.00f, 0.00f, 0.00f);
         if (on > 0) glEnd ();
         glPointSize (15.00f);
         glBegin (GL_POINTS);
         glVertex3f ((gait.pos - beg) * 2.00f, gait.y [gait.pos][i], 0.00f);
         glEnd ();
      } glPopMatrix();
      /*> exit (-1);                                                                  <*/
   }
   glLineWidth (0.50f);
   view_touch ();
   view_vert  ();
   return 0;
}

char       /* ---- : build a step path visualization -------------------------*/
view_step          (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   int       j         = 0;            /* loop iterator -- position           */
   char      msg [100] = "";
   int       save      = 0;
   int       first     = 0;
   int       count     = 0;
   int       place     = 0;
   /*---(setup view)---------------------*/
   view_setup  ("step cycle", "step cycle = the sequence of functions by one limb within a gait cycle");
   /*---(show name)----------------------*/
   glPushMatrix    (); {
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
      glTranslatef ( 0.00, 35.00, 0.00f);
      snprintf     (msg,  50, "%s", gait.name);
      yFONT_print  (my.font,  18, YF_BOTCEN, msg);
   } glPopMatrix();
   /*---(show points)--------------------*/
   glPushMatrix    (); {
      glTranslatef ( 0.00, 25.00, 0.00f);
      /*---(guidelines)------------------*/
      glLineWidth (2.00f);
      glColor4f (0.0f, 1.0f, 0.0f, 0.3f);
      glBegin (GL_LINES);
      glVertex3f (-400.00, 0.00, -5.00);
      glVertex3f ( 400.00, 0.00, -5.00);
      glEnd ();
      glColor4f (1.0f, 1.0f, 0.0f, 0.3f);
      glBegin (GL_LINES);
      glVertex3f (-400.00,  50.00, -5.00);
      glVertex3f ( 400.00,  50.00, -5.00);
      glEnd ();
      /*---(path)------------------------*/
      glColor4f (1.0f, 0.0f, 0.0f, 1.0f);
      glBegin (GL_LINE_STRIP);
      for (j = 0; j < gait.dmax; ++j) {
         switch (gait.dphz[j]) {
         case 1  : glColor4f (1.0f, 0.0f, 0.0f, 0.8f); break;
         case 2  : glColor4f (1.0f, 1.0f, 1.0f, 0.8f); break;
         case 3  : glColor4f (0.7f, 0.3f, 0.0f, 0.8f); break;
         case 4  : glColor4f (0.3f, 0.7f, 0.0f, 0.8f); break;
         case 5  : glColor4f (0.0f, 0.7f, 0.3f, 0.8f); break;
         case 6  : glColor4f (0.0f, 0.5f, 0.7f, 0.8f); break;
         case 7  : glColor4f (0.0f, 0.0f, 1.0f, 0.8f); break;
         case 8  : glColor4f (0.3f, 0.0f, 0.7f, 0.8f); break;
         case 9  : glColor4f (0.7f, 0.0f, 0.3f, 0.8f); break;
         default : glColor4f (0.5f, 0.5f, 0.5f, 0.8f); break;
         }
         glVertex3f (gait.dxz [j] * 10.00f, gait.dy [j] * 10.00f, 0.00f);
      }
      j = 0;
      glVertex3f (gait.dxz [j] * 10.00f, gait.dy [j] * 10.00f, 0.00f);
      glEnd ();
      /*---(points)----------------------*/
      glPointSize (10.0);
      glBegin (GL_POINTS);
      for (j = 0; j < gait.dmax; ++j) {
         switch (gait.dphz[j]) {
         case 1  : glColor4f (1.0f, 0.0f, 0.0f, 0.8f); break;
         case 2  : glColor4f (0.5f, 0.5f, 0.5f, 0.8f); break;
         case 3  : glColor4f (0.7f, 0.3f, 0.0f, 0.8f); break;
         case 4  : glColor4f (0.3f, 0.7f, 0.0f, 0.8f); break;
         case 5  : glColor4f (0.0f, 0.7f, 0.3f, 0.8f); break;
         case 6  : glColor4f (0.0f, 0.5f, 0.7f, 0.8f); break;
         case 7  : glColor4f (0.0f, 0.0f, 1.0f, 0.8f); break;
         case 8  : glColor4f (0.3f, 0.0f, 0.7f, 0.8f); break;
         case 9  : glColor4f (0.7f, 0.0f, 0.3f, 0.8f); break;
         default : glColor4f (0.5f, 0.5f, 0.5f, 0.8f); break;
         }
         glVertex3f (gait.dxz [j] * 10.00f, gait.dy [j] * 10.00f, 0.00f);
      }
      glEnd ();
      /*---(current point)---------------*/
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
      glPointSize (15.0);
      glBegin (GL_POINTS);
      glVertex3f (gait.dxz [gait.cur [2]] * 10.00f, gait.dy [gait.cur [2]] * 10.00f, 10.00f);
      glEnd ();
   } glPopMatrix();
   /*---(show labels)--------------------*/
   save  =  gait.dphz [0];
   first =  0;
   count =  1;
   for (j = 0; j <= gait.dmax; ++j) {
      if (save == gait.dphz[j] && j != gait.dmax) {
         ++count;
         continue;
      }
      place = first + ((count - 1) / 2.0);
      /*> printf ("phase = %1d, j = %3d, save = %3d, first = %3d, count = %3d, place = %3d\n", gait.dphz [first], j, save, first, count, place);   <*/
      glPushMatrix    (); {
         glTranslatef ( 0.00, 25.00, 0.00f);
         glTranslatef (gait.dxz [place] * 10.00f, gait.dy [place] * 10.00f, 0.00f);
         switch (gait.dphz[first]) {
         case 1  :
            glColor4f    (1.0f, 0.0f, 0.0f, 0.8f);
            glTranslatef (   0.00, -10.00, 0.00);
            yFONT_print  (my.font,  12, YF_TOPCEN, "fore");
            break;
         case 2  :
            glColor4f    (0.5f, 0.5f, 0.5f, 0.8f);
            glTranslatef (   0.00, -10.00, 0.00);
            yFONT_print  (my.font,  12, YF_TOPCEN, "mark");
            break;
         case 3  :
            glColor4f    (0.7f, 0.3f, 0.0f, 0.8f);
            glTranslatef (   0.00, -10.00, 0.00);
            yFONT_print  (my.font,  12, YF_TOPCEN, "back");
            break;
         case 4  :
            glColor4f    (0.3f, 0.7f, 0.0f, 0.8f);
            glTranslatef ( -20.00,   0.00, 0.00);
            yFONT_print  (my.font,  12, YF_MIDRIG, "kick");
            break;
         case 5  :
            glColor4f    (0.0f, 0.7f, 0.3f, 0.8f);
            glTranslatef ( -20.00,   0.00, 0.00);
            yFONT_print  (my.font,  12, YF_MIDRIG, "accel");
            break;
         case 6  :
            glColor4f    (0.0f, 0.5f, 0.7f, 0.8f);
            glTranslatef (   0.00,  10.00, 0.00);
            yFONT_print  (my.font,  12, YF_BOTCEN, "return");
            break;
         case 7  :
            glColor4f (0.1f, 0.1f, 1.0f, 0.8f);
            glTranslatef (  20.00,   0.00, 0.00);
            yFONT_print  (my.font,  12, YF_MIDLEF, "decel");
            break;
         case 8  :
            glColor4f (0.3f, 0.0f, 0.7f, 0.8f);
            glTranslatef (  20.00,   0.00, 0.00);
            yFONT_print  (my.font,  12, YF_MIDLEF, "load");
            break;
         default : glColor4f (0.5f, 0.5f, 0.5f, 0.8f); break;
         }
      } glPopMatrix();
      first =  j;
      count =  1;
      save  =  gait.dphz[j];
   }
   /*---(show name)----------------------*/
   glPushMatrix    (); {
      glTranslatef ( 0.00, 65.00, 0.00f);
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
      snprintf     (msg,  50, "duty ratio = %6.2f%%", gait.duty);
      yFONT_print  (my.font,  12, YF_BOTCEN, msg);
   } glPopMatrix();
   return 0;
}

char       /* ---- : build a joint angle visualization -----------------------*/
view_waves         (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   int       j         = 0;            /* loop iterator -- position           */
   char      msg [100] = "";
   float     velocity  = 0.00f;
   float     accel     = 0.00f;
   float     save      = 0.00f;
   /*---(setup view)---------------------*/
   view_setup  ("joint angles", "visually display joint angle, velocity, and acceleration");
   /*---()-------------------------------*/
   i = my_curr;
   glPushMatrix    (); {
      glLineWidth (2.00f);
      glTranslatef ( -425.00f,  75.00f,    0.00f);
      glColor4f (1.0f, 0.0f, 0.0f, 1.0f);
      glBegin (GL_LINE_STRIP);
      for (j = beg; j < end; ++j) {
         glVertex3f ((j - beg) * 2.00f, gait.femu [j][i] * 4.0, 0.00f);
      }
      glEnd ();
      glBegin (GL_LINES);
      for (j = beg + 1; j < end; j += 3) {
         velocity = (gait.femu [j][i] - gait.femu [j - 1][i]) * 30.00f;
         glVertex3f ((j - beg) * 2.00f, 0.00f   , -10.00f);
         glVertex3f ((j - beg) * 2.00f, velocity, -10.00f);
      }
      glEnd ();
      glPointSize (15.00f);
      glBegin (GL_POINTS);
      glVertex3f ((gait.pos - beg) * 2.00f, gait.femu [gait.pos][i] * 4.0, 0.00f);
      glEnd ();
   } glPopMatrix();
   glPushMatrix    (); {
      glLineWidth (2.00f);
      glTranslatef ( -425.00f, 110.00f,    0.00f);
      glColor4f (0.0f, 0.5f, 1.0f, 1.0f);
      glBegin (GL_LINE_STRIP);
      for (j = beg; j < end; ++j) {
         glVertex3f ((j - beg) * 2.00f, gait.pate [j][i] * 2.0, 0.00f);
      }
      glEnd ();
      glBegin (GL_LINES);
      for (j = beg + 1; j < end; j += 3) {
         velocity = (gait.pate [j][i] - gait.pate [j - 1][i]) * 30.00f;
         glVertex3f ((j - beg) * 2.00f, 0.00f   ,  -8.00f);
         glVertex3f ((j - beg) * 2.00f, velocity,  -8.00f);
      }
      glEnd ();
      /*> glEnable(GL_LINE_STIPPLE);                                                  <* 
       *> glLineStipple(2, 0xF0F0);                                                   <* 
       *> glBegin (GL_LINE_STRIP);                                                    <* 
       *> for (j = beg + 2; j < end; j += 3) {                                        <* 
       *>    save     = (gait.pate [j - 1][i] - gait.pate [j - 2][i]) * 30.00f;       <* 
       *>    velocity = (gait.pate [j - 0][i] - gait.pate [j - 1][i]) * 30.00f;       <* 
       *>    accel    = velocity - save;                                              <* 
       *>    glVertex3f ((j - beg) * 2.00f, save,  -6.00f);                           <* 
       *> }                                                                           <* 
       *> glEnd ();                                                                   <* 
       *> glDisable(GL_LINE_STIPPLE);                                                 <*/
      glPointSize (15.00f);
      glBegin (GL_POINTS);
      glVertex3f ((gait.pos - beg) * 2.00f, gait.pate [gait.pos][i] * 2.0, 0.00f);
      glEnd ();
   } glPopMatrix();
   glPushMatrix    (); {
      glLineWidth (2.00f);
      glTranslatef ( -425.00f, 215.00f,    0.00f);
      glColor4f (1.0f, 0.5f, 0.0f, 1.0f);
      glBegin (GL_LINE_STRIP);
      for (j = beg; j < end; ++j) {
         glVertex3f ((j - beg) * 2.00f, gait.tibi [j][i] * 2.0, 0.00f);
      }
      glEnd ();
      glBegin (GL_LINES);
      for (j = beg + 2; j < end; j += 3) {
         velocity = (gait.tibi [j][i] - gait.tibi [j - 1][i]) * 30.00f;
         glVertex3f ((j - beg) * 2.00f, -180.00f   ,  -6.00f);
         glVertex3f ((j - beg) * 2.00f, -180.00f + velocity,  -6.00f);
      }
      glEnd ();
      glPointSize (15.00f);
      glBegin (GL_POINTS);
      glVertex3f ((gait.pos - beg) * 2.00f, gait.tibi [gait.pos][i] * 2.0, 0.00f);
      glEnd ();
   } glPopMatrix();
   view_vert  ();
   return 0;
}

char       /* ---- : build a joint torque visualization ----------------------*/
view_torque        (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = my_curr;      /* loop iterator -- leg                */
   int       j         = 0;            /* loop iterator -- position           */
   char      msg [100] = "";
   float     torque    = 0.00f;
   float     length    = 0.00f;
   /*---(setup view)---------------------*/
   view_setup  ("joint torque", "visually display joint torques");
   view_vert   ();
   /*---(tibia)--------------------------*/
   glPushMatrix    (); {
      /*---(prepare)-----------*/
      glLineWidth (2.00f);
      glTranslatef ( -425.00f, 215.00f,    0.00f);
      glColor4f (1.0f, 0.5f, 0.0f, 1.0f);
      /*---(torque)------------*/
      glBegin (GL_LINE_STRIP);
      for (j = beg; j < end; ++j) {
         length  = gait.tibi [j][i];
         glVertex3f ((j - beg) * 2.00f, gait.tibi [j][i] * 2.0, 0.00f);
      }
      glEnd ();
      /*---(current)-----------*/
      glPointSize (15.00f);
      glBegin (GL_POINTS);
      glVertex3f ((gait.pos - beg) * 2.00f, gait.tibi [gait.pos][i] * 2.0, 0.00f);
      glEnd ();
      /*---(done)--------------*/
   } glPopMatrix();
   /*---(complete)-----------------------*/
   return 0;
}

void
view_leg()
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;
   float     x, y, z, xz;
   char      message [100];
   float     offset    = 0.00f;
   /*---(figure out left offset)---------*/
   offset = segs_len [YKINE_THOR];
   if (offset > 25.0) offset -= 25.0;
   /*---(setup view)---------------------*/
   glViewport      (700, 168, 300, 300);
   glMatrixMode    (GL_PROJECTION);
   glLoadIdentity  ();
   glOrtho         (offset, 300.0 + offset, -150.0, 150.0,-150.0,  150.0);
   glMatrixMode    (GL_MODELVIEW);
   DRAW_reset      ();
   /*---(draw leg segment)---------------*/
   glPushMatrix();
   glColor3f(1.0f, 1.0f, 1.0f);
   /*> glTranslatef (  0.0,  lens [YKINE_THOR], 0.0);                                       <*/
   /*> glTranslatef (  0.0,  lens [YKINE_THOR], 0.0);                                       <*/
   i = my_curr;
   glCallList      (dl_body);
   draw_leg   (i, gk[i], 'n');
   glPopMatrix();
   return;
   /*> /+---(create a grid)-------------------------+/                                <* 
    *> glPushMatrix();                                                                <* 
    *> glEnable(GL_LINE_STIPPLE);                                                     <* 
    *> glLineStipple(1, 0xAAAA);                                                      <* 
    *> glColor3f(0.2f, 0.2f, 0.2f);                                                   <* 
    *> glBegin(GL_LINES);                                                             <* 
    *> for (x = 0; x < 15; x += 2) {                                                  <* 
    *>    glVertex3f( x,  8.00f, 0);                                                  <* 
    *>    glVertex3f( x, -8.00f, 0);                                                  <* 
    *> }                                                                              <* 
    *> for (y = 0; y < 17; y += 2) {                                                  <* 
    *>    glVertex3f( 0.00f, 8 - y, 0);                                               <* 
    *>    glVertex3f(14.00f, 8 - y, 0);                                               <* 
    *> }                                                                              <* 
    *> /+---(second grid)---------------------------+/                                <* 
    *> glBegin(GL_LINES);                                                             <* 
    *> for (x = 16; x < 30; x += 2) {                                                 <* 
    *>    glVertex3f( x,  8.00f, 0);                                                  <* 
    *>    glVertex3f( x, -8.00f, 0);                                                  <* 
    *> }                                                                              <* 
    *> for (y = 0; y < 17; y += 2) {                                                  <* 
    *>    glVertex3f(16.00f, 8 - y, 0);                                               <* 
    *>    glVertex3f(30.00f, 8 - y, 0);                                               <* 
    *> }                                                                              <* 
    *> glEnd();                                                                       <* 
    *> /+---(draw origin lines)---------------------+/                                <* 
    *> glEnable(GL_LINE_STIPPLE);                                                     <* 
    *> glLineStipple(1, 0xAAAA);                                                      <* 
    *> glColor3f(0.0f, 1.0f, 0.0f);                                                   <* 
    *> x  = fk[my_curr][3].x + fk[my_curr][4].x + fk[my_curr][5].x;                   <* 
    *> z  = fk[my_curr][3].z + fk[my_curr][4].z + fk[my_curr][5].z;                   <* 
    *> y  = fk[my_curr][3].y + fk[my_curr][4].y + fk[my_curr][5].y;                   <* 
    *> xz = sqrt((x * x) + (z * z));                                                  <* 
    *> glBegin(GL_LINES);                                                             <* 
    *> glVertex3f( 5.37f, 0.65f, 0.00f);                                              <* 
    *> glVertex3f( 5.35f + x, 0.65 + y, 0.00f);                                       <* 
    *> glEnd();                                                                       <* 
    *> glDisable(GL_LINE_STIPPLE);                                                    <* 
    *> /+---(vertical bar charts)-------------------+/                                <* 
    *> i = my_curr;                                                                   <* 
    *> glLineWidth(3.50f);                                                            <* 
    *> glColor3f(0.0f, 0.0f, 1.0f);                                                   <* 
    *> glBegin(GL_LINES);                                                             <* 
    *> glVertex3f( 1.0f, 0.00f, 0.0f );                                               <* 
    *> glVertex3f( 1.0f, -fabs(fk[i][3].y), 0.0f );                                   <* 
    *> glVertex3f( 2.0f, 0.00f, 0.0f );                                               <* 
    *> glVertex3f( 2.0f, -fabs(fk[i][5].y - fk[i][4].y), 0.0f );                      <* 
    *> glVertex3f( 3.0f, 0.00f, 0.0f );                                               <* 
    *> glVertex3f( 3.0f, -fabs(fk[i][5].y - fk[i][4].y - fk[i][3].y), 0.0f );         <* 
    *> glEnd();                                                                       <*/
   /*> glPopMatrix();                                                                           <* 
    *> /+---(horizontal bar charts)-----------------+/                                          <* 
    *> glPushMatrix();                                                                          <* 
    *> glTranslatef(  5.37f,  0.00f,  0.00f);                                                   <* 
    *> glBegin(GL_LINES);                                                                       <* 
    *> glVertex3f( 0.00f, 7.00f, 0.00f);                                                        <* 
    *> x = fk[i][3].x;                                                                          <* 
    *> z = fk[i][3].z;                                                                          <* 
    *> glVertex3f(sqrt((x * x) + (z * z)), 7.0f, 0.0f );                                        <* 
    *> glEnd();                                                                                 <* 
    *> /+> glRasterPos2f(-5.0f, 6.7f);                                                    <*    <* 
    *>  *> if (font == NULL) font_load();                                                 <*    <* 
    *>  *> sprintf(message, "%4.2fp", sqrt((x * x) + (z * z)));                           <*    <* 
    *>  *> ftglRenderFont(font, message, FTGL_RENDER_ALL);                                <+/   <* 
    *> glBegin(GL_LINES);                                                                       <* 
    *> glVertex3f( 0.00f, 6.00f, 0.00f);                                                        <* 
    *> x = fk[i][5].x - fk[i][4].x;                                                             <* 
    *> z = fk[i][5].z - fk[i][4].z;                                                             <* 
    *> glVertex3f(sqrt((x * x) + (z * z)), 6.0f, 0.0f );                                        <* 
    *> glEnd();                                                                                 <* 
    *> /+> glRasterPos2f(-5.0f, 5.7f);                                                    <*    <* 
    *>  *> if (font == NULL) font_load();                                                 <*    <* 
    *>  *> sprintf(message, "%4.2ft", sqrt((x * x) + (z * z)));                           <*    <* 
    *>  *> ftglRenderFont(font, message, FTGL_RENDER_ALL);                                <+/   <* 
    *> glBegin(GL_LINES);                                                                       <* 
    *> glVertex3f( 0.00f, 5.00f, 0.00f);                                                        <* 
    *> x = fk[i][5].x - fk[i][4].x - fk[i][3].x;                                                <* 
    *> z = fk[i][5].z - fk[i][4].z - fk[i][3].z;                                                <* 
    *> glVertex3f(sqrt((x * x) + (z * z)), 5.0f, 0.0f );                                        <* 
    *> glEnd();                                                                                 <* 
    *> /+> glRasterPos2f(-5.0f, 4.7f);                                                    <*    <* 
    *>  *> if (font == NULL) font_load();                                                 <*    <* 
    *>  *> sprintf(message, "%4.2ff", sqrt((x * x) + (z * z)));                           <*    <* 
    *>  *> ftglRenderFont(font, message, FTGL_RENDER_ALL);                                <+/   <* 
    *> glLineWidth(0.50f);                                                                      <* 
    *> glDisable(GL_LINE_STIPPLE);                                                              <* 
    *> glRotatef(fk[i][2].d, 0.0f, 1.0f, 0.0f);                                                 <* 
    *> glPopMatrix();                                                                           <* 
    *> /+---(setup specifics)------------------------+/                                         <* 
    *> /+> glTranslatef( -2.50f,  0.0,  0.0f);                                            <*    <* 
    *>  *> draw_body();                                                                   <+/   <* 
    *> /+> glTranslatef(  4.25f,  0.0,  0.0f);                                            <+/   <* 
    *> /+---(draw ik triangle)-----------------------------+/                                   <* 
    *> float len, lenxz, angle1, angle2, angle3, angleY, deg1, deg2, deg3, degY;                <* 
    *> glPushMatrix();                                                                          <* 
    *> glTranslatef( 16.00f,  0.00f,  0.00f);                                                   <* 
    *> x = ik[i][6].x - ik[i][2].x - ik[i][1].x - ik[i][0].x;                                   <* 
    *> z = ik[i][6].z - ik[i][2].z - ik[i][1].z - ik[i][0].z;                                   <* 
    *> y = ik[i][6].y - ik[i][2].y - ik[i][1].y - ik[i][0].y;                                   <* 
    *> len    = sqrt((x * x) + (z * z) + (y * y));                                              <* 
    *> lenxz  = sqrt((x * x) + (z * z));                                                        <* 
    *> /+> if (fk[i][2].h != fk[i][4].h) lenxz *= -1;                                     <+/   <* 
    *> angle1 = acos(((len * len)   + (5.50 * 5.50) - (2.25 * 2.25)) / (2 * len  * 5.50));      <* 
    *> angle2 = acos(((len * len)   + (2.25 * 2.25) - (5.50 * 5.50)) / (2 * len  * 2.25));      <* 
    *> angle3 = acos(((5.50 * 5.50) + (2.25 * 2.25) - (len * len)  ) / (2 * 2.25 * 5.50));      <* 
    *> angleY = cos(y / len);                                                                   <* 
    *> deg1   = angle1 * (360 / (2 * M_PI));                                                    <* 
    *> deg2   = angle2 * (360 / (2 * M_PI));                                                    <* 
    *> deg3   = angle3 * (360 / (2 * M_PI));                                                    <* 
    *> degY   = angleY * (360 / (2 * M_PI));                                                    <*/
   /*---(draw patella/tibia legend)--------------*/
   glTranslatef( 5.37f,  0.65f,  0.00f);
   glLineWidth(2.50f);
   glBegin(GL_LINES);
   glColor3f(0.0f, 1.0f, 0.5f);
   glVertex3f( 2.00f, 6.80f, 0.00f);
   glVertex3f( 3.50f, 6.80f, 0.00f);
   glColor3f(0.0f, 0.0f, 1.0f);
   glVertex3f( 2.00f, 5.30f, 0.00f);
   glVertex3f( 3.50f, 5.30f, 0.00f);
   glColor3f(1.0f, 1.0f, 1.0f);
   glVertex3f( 2.00f, 3.80f, 0.00f);
   glVertex3f( 3.50f, 3.80f, 0.00f);
   glEnd();
   /*> glRasterPos2f( 4.0f, 6.5f);                                                    <* 
    *> ftglRenderFont(font, "patella", FTGL_RENDER_ALL);                              <* 
    *> glRasterPos2f( 4.0f, 5.0f);                                                    <* 
    *> ftglRenderFont(font, "tibia", FTGL_RENDER_ALL);                                <* 
    *> glRasterPos2f( 4.0f, 3.5f);                                                    <* 
    *> ftglRenderFont(font, "hyp", FTGL_RENDER_ALL);                                  <* 
    *> glRasterPos2f( 4.0f, 2.0f);                                                    <* 
    *> ftglRenderFont(font, "xz/y", FTGL_RENDER_ALL);                                 <*/
   /*---(draw patella/tibia triangle)------------*/
   glLineWidth(2.50f);
   glColor3f(0.0f, 1.0f, 0.5f);
   glBegin(GL_LINE_LOOP);
   glColor3f(0.0f, 1.0f, 0.5f);
   glVertex3f( 0.00f,  0.00f,  0.00f);
   glVertex3f(ik[i][3].xz, ik[i][3].y, 0.00f);
   glColor3f(0.0f, 0.0f, 1.0f);
   glVertex3f(ik[i][3].xz + ik[i][4].xz, ik[i][3].y + ik[i][4].y, 0.00f);
   glColor3f(1.0f, 1.0f, 1.0f);
   glVertex3f( 0.00f,  0.00f,  0.00f);
   glEnd();
   /*> glRotatef(180 - deg2, 0.0f, 0.0f, 1.0f);                                       <*/
   /*> d = fk[i][3].v * (360 / (2 * M_PI));                                           <*/
   /*> d = ((-M_PI / 2) + angle2 + cos(y/len) + ((y>=0)?90:0)) * (360 / (2 * M_PI));            <* 
    *> glRotatef(d, 0.0f, 0.0f, 1.0f);                                                          <* 
    *> glLineWidth(2.50f);                                                                      <* 
    *> glColor3f(0.0f, 1.0f, 0.5f);                                                             <* 
    *> glBegin(GL_LINES);                                                                       <* 
    *> glVertex3f( 0.00f, 0.00f, 0.00f);                                                        <* 
    *> glVertex3f( 2.25f, 0.00f, 0.00f);                                                        <* 
    *> glEnd();                                                                                 <* 
    *> glTranslatef( 2.25f,  0.00f,  0.00f);                                                    <* 
    *> /+> glRotatef(180 + deg3, 0.0f, 0.0f, 1.0f);                                       <+/   <* 
    *> glRotatef(180 + deg3, 0.0f, 0.0f, 1.0f);                                                 <* 
    *> glColor3f(0.0f, 0.0f, 1.0f);                                                             <* 
    *> glBegin(GL_LINES);                                                                       <* 
    *> glVertex3f( 0.00f, 0.00f, 0.00f);                                                        <* 
    *> glVertex3f( 5.50f, 0.00f, 0.00f);                                                        <* 
    *> glEnd();                                                                                 <* 
    *> glTranslatef( 5.50f,  0.00f,  0.00f);                                                    <* 
    *> glRotatef(180 + deg1, 0.0f, 0.0f, 1.0f);                                                 <* 
    *> glColor3f(1.0f, 1.0f, 1.0f);                                                             <* 
    *> glBegin(GL_LINES);                                                                       <* 
    *> glVertex3f( 0.00f, 0.00f, 0.00f);                                                        <* 
    *> glVertex3f( len  , 0.00f, 0.00f);                                                        <* 
    *> glEnd();                                                                                 <* 
    *> glTranslatef( len,  0.00f,  0.00f);                                                      <*/
   glPopMatrix();
   /*---(draw y/xz triangle)---------------------*/
   glPushMatrix();
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(2, 0xAAAA);
   glLineWidth(0.50f);
   glColor3f(1.0f, 0.0f, 1.0f);
   glTranslatef( 16.00f,  0.65f,  0.00f);
   glTranslatef(  5.37f,  0.00f,  0.00f);
   glBegin(GL_LINE_LOOP);
   glVertex3f( 0.00f, 0.00f, 0.00f);
   glVertex3f( 0.00f, y    , 0.00f);
   x = ik[i][6].x;
   z = ik[i][6].z;
   float xz6 = sqrt(( x *  x) + ( z *  z));
   x = ik[i][2].x + ik[i][1].x + ik[i][0].x;
   z = ik[i][2].z + ik[i][1].z + ik[i][0].z;
   float xz3 = sqrt(( x *  x) + ( z *  z));
   float lenxz;
   if (xz6 < xz3) lenxz *= -1;
   glVertex3f( lenxz, y    , 0.00f);
   /*> if (ik[i][5].v >= 0) glVertex3f( lenxz, y    , 0.00f);                         <* 
    *> else                 glVertex3f(-lenxz, y    , 0.00f);                         <*/
   glVertex3f( 0.00f, 0.00f, 0.00f);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f( 2.00f, 2.30f, 0.00f);
   glVertex3f( 3.50f, 2.30f, 0.00f);
   glEnd();
   glDisable(GL_LINE_STIPPLE);
   /*---(complete)-----------------------*/
   glPopMatrix();
   return;
}





/*> void                                                                                        <* 
 *> draw_3d_locate()                                                                            <* 
 *> {                                                                                           <* 
 *>    glPushMatrix();                                                                          <* 
 *>    glTranslatef( 32.0, -21.7,  -75.0);                                                      <* 
 *>    glColor3f(1.0f, 1.0f, 0.0f);                                                             <* 
 *>    glCallList(dl_godbox);                                                                   <* 
 *>    /+---(write the label)-----------------------+/                                          <* 
 *>    /+> if (font == NULL) font_load();                                                 <*    <* 
 *>     *> glRasterPos2f( 3.0f, -5.9f);                                                   <*    <* 
 *>     *> ftglRenderFont(font, "god-box", FTGL_RENDER_ALL);                              <*    <* 
 *>     *> glRasterPos2f( 2.5f, -6.9f);                                                   <*    <* 
 *>     *> ftglRenderFont(font, "(cdb-lae)", FTGL_RENDER_ALL);                            <+/   <* 
 *>    /+---(boom)-----------------------------------+/                                         <* 
 *>    glPushMatrix();                                                                          <* 
 *>    glColor3f(1.0f, 1.0f, 0.0f);                                                             <* 
 *>    glBegin(GL_LINES);                                                                       <* 
 *>    glVertex3f( -7.0f,   0.0f,   0.0f);                                                      <* 
 *>    glVertex3f(  7.0f,   0.0f,   0.0f);                                                      <* 
 *>    glVertex3f(  0.0f,   7.0f,   0.0f);                                                      <* 
 *>    glVertex3f(  0.0f,  -7.0f,   0.0f);                                                      <* 
 *>    glVertex3f(  0.0f,   0.0f,  10.0f);                                                      <* 
 *>    glVertex3f(  0.0f,   0.0f, -10.0f);                                                      <* 
 *>    glEnd();                                                                                 <* 
 *>    glPopMatrix();                                                                           <* 
 *>    if (my_x != 0.0) {                                                                       <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glColor3f(0.0f, 1.0f, 0.0f);                                                          <* 
 *>       glTranslatef(my_x / 40.0,   0.0,   1.0);                                              <* 
 *>       glCallList(dl_cube);                                                                  <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    if (my_vx != 0.0) {                                                                      <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glColor3f(0.0f, 0.0f, 1.0f);                                                          <* 
 *>       glTranslatef(-my_vx / 40.0,   0.0,  -1.0);                                            <* 
 *>       glCallList(dl_cube);                                                                  <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    if (my_z != -800.0) {                                                                    <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glColor3f(0.0f, 1.0f, 0.0f);                                                          <* 
 *>       glTranslatef( 0.0,  1.0,  (my_z + 800) / 60.0);                                       <* 
 *>       glCallList(dl_sphere);                                                                <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    if (my_vz != 0.0) {                                                                      <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glColor3f(0.0f, 0.0f, 1.0f);                                                          <* 
 *>       glTranslatef( 0.0, -1.0, -my_vz / 60.0);                                              <* 
 *>       glCallList(dl_sphere);                                                                <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    if (my_y != 124.0) {                                                                     <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glColor3f(0.0f, 1.0f, 0.0f);                                                          <* 
 *>       glTranslatef(  1.0, ((my_y - 124) / 50.0),    0.0);                                   <* 
 *>       glCallList(dl_pyramid);                                                               <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    if (my_vy != 150.0) {                                                                    <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glColor3f(0.0f, 0.0f, 1.0f);                                                          <* 
 *>       glTranslatef( -1.0, -(my_vy - 150) / 50.0,   0.0);                                    <* 
 *>       glCallList(dl_pyramid);                                                               <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    /+---(end)-----------------------------------+/                                          <* 
 *>    glPopMatrix();                                                                           <* 
 *>    return;                                                                                  <* 
 *> }                                                                                           <*/


/*> void                                                                                        <* 
 *> draw_3d_orient()                                                                            <* 
 *> {                                                                                           <* 
 *>    glPushMatrix();                                                                          <* 
 *>    /+---(get to the right place)----------------+/                                          <* 
 *>    glTranslatef(-32.0, -22.0,  -75.0);                                                      <* 
 *>    /+---(draw god's balls)----------------------+/                                          <* 
 *>    glColor3f(1.0f, 1.0f, 0.0f);                                                             <* 
 *>    glCallList(dl_godball);                                                                  <* 
 *>    /+---(write the label)-----------------------+/                                          <* 
 *>    /+> if (font == NULL) font_load();                                                 <*    <* 
 *>     *> glRasterPos2f(-9.0f, -5.7f);                                                   <*    <* 
 *>     *> ftglRenderFont(font, "god-ball", FTGL_RENDER_ALL);                             <*    <* 
 *>     *> glRasterPos2f(-9.2f, -6.7f);                                                   <*    <* 
 *>     *> ftglRenderFont(font, "(pry-vso)", FTGL_RENDER_ALL);                            <+/   <* 
 *>    /+---(yaw)-----------------------------------+/                                          <* 
 *>    if (my_yaw != 0.0) {                                                                     <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glRotatef(90.0, 0.0f, 1.0f, 0.0f);                                                    <* 
 *>       glColor3f(1.0f, 1.0f, 0.0f);                                                          <* 
 *>       if (((int) my_yaw) % 45 == 0) glColor3f(0.0f, 1.0f, 0.0f);                            <* 
 *>       glRotatef(-my_yaw, 0.0f, 1.0f, 0.0f);                                                 <* 
 *>       glTranslatef(  5.0,   0.0,    0.0);                                                   <* 
 *>       glCallList(dl_pyramid);                                                               <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    /+---(orbit)---------------------------------+/                                          <* 
 *>    if (my_vyaw != 0.0) {                                                                    <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glRotatef(90.0, 0.0f, 1.0f, 0.0f);                                                    <* 
 *>       glColor3f(0.0f, 0.0f, 1.0f);                                                          <* 
 *>       if (((int) my_vyaw) % 45 == 0) glColor3f(0.0f, 1.0f, 0.0f);                           <* 
 *>       glRotatef(-(my_vyaw    ), 0.0f, 1.0f, 0.0f);                                          <* 
 *>       glTranslatef(  7.0,   0.0,    0.0);                                                   <* 
 *>       glCallList(dl_pyramid);                                                               <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    /+---(pitch)--------------------------------+/                                           <* 
 *>    if (my_pitch != 0.0) {                                                                   <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glRotatef(90.0, 0.0f, 0.0f, 1.0f);                                                    <* 
 *>       glColor3f(1.0f, 1.0f, 0.0f);                                                          <* 
 *>       if (((int) my_pitch) % 45 == 0) glColor3f(0.0f, 1.0f, 0.0f);                          <* 
 *>       glRotatef(my_pitch, 0.0f, 1.0f, 0.0f);                                                <* 
 *>       glTranslatef(  5.0,   0.0,    0.0);                                                   <* 
 *>       glCallList(dl_cube);                                                                  <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    /+---(vantage)------------------------------+/                                           <* 
 *>    if (my_vpitch != 0.0) {                                                                  <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glRotatef(90.0, 0.0f, 0.0f, 1.0f);                                                    <* 
 *>       glColor3f(0.0f, 0.0f, 1.0f);                                                          <* 
 *>       if (((int) my_vpitch) % 45 == 0) glColor3f(0.0f, 1.0f, 0.0f);                         <* 
 *>       glRotatef(my_vpitch, 0.0f, 1.0f, 0.0f);                                               <* 
 *>       glTranslatef(  7.0,   0.0,    0.0);                                                   <* 
 *>       glCallList(dl_cube);                                                                  <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    /+---(roll)---------------------------------+/                                           <* 
 *>    if (my_roll != 0.0) {                                                                    <* 
 *>       glPushMatrix();                                                                       <* 
 *>       glRotatef(90.0, 1.0f, 0.0f, 0.0f);                                                    <* 
 *>       glColor3f(1.0f, 1.0f, 0.0f);                                                          <* 
 *>       if (((int) my_roll) % 45 == 0) glColor3f(0.0f, 1.0f, 0.0f);                           <* 
 *>       glRotatef(-my_roll, 0.0f, 1.0f, 0.0f);                                                <* 
 *>       glTranslatef(  5.0,   0.0,    0.0);                                                   <* 
 *>       glCallList(dl_sphere);                                                                <* 
 *>       glPopMatrix();                                                                        <* 
 *>    }                                                                                        <* 
 *>    /+---(spin)---------------------------------+/                                           <* 
*>    if (my_vroll != 0.0) {                                                                   <* 
   *>       glPushMatrix();                                                                       <* 
      *>       glRotatef(90.0, 1.0f, 0.0f, 0.0f);                                                    <* 
      *>       glColor3f(0.0f, 0.0f, 1.0f);                                                          <* 
      *>       if (((int) my_vroll) % 45 == 0) glColor3f(0.0f, 1.0f, 0.0f);                          <* 
      *>       glRotatef(-my_vroll, 0.0f, 1.0f, 0.0f);                                               <* 
      *>       glTranslatef(  7.0,   0.0,    0.0);                                                   <* 
      *>       glCallList(dl_sphere);                                                                <* 
      *>       glPopMatrix();                                                                        <* 
      *>    }                                                                                        <* 
      *>    /+---(complete)-------------------------------+/                                         <* 
      *>    glPopMatrix();                                                                           <* 
      *>    return;                                                                                  <* 
      *> }                                                                                           <*/


void
draw_axis()
{
   glEnable(GL_LINE_STIPPLE);
   /*---(positive X :: blue   course dashed)-------------*/
   glLineStipple(2, 0x0F0F);
   glColor3f(0.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(   0.00f,   0.00f,   0.00f);
   glVertex3f(  20.00f,   0.00f,   0.00f);
   glEnd();
   /*---(negative X :: blue   fine dotted)---------------*/
   glLineStipple(2, 0xAAAA);
   glColor3f(0.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(   0.00f,   0.00f,   0.00f);
   glVertex3f( -20.00f,   0.00f,   0.00f);
   glEnd();
   /*---(positive Z :: cyan   course dashed)-------------*/
   glLineStipple(2, 0x0F0F);
   glBegin(GL_LINES);
   glColor3f(0.0f, 1.0f, 1.0f);
   glVertex3f(   0.00f,   0.00f,   0.00f);
   glVertex3f(   0.00f,   0.00f,  20.00f);
   glEnd();
   /*---(negative Z :: cyan   fine dotted)---------------*/
   glLineStipple(2, 0xAAAA);
   glColor3f(0.0f, 1.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(   0.00f,   0.00f,   0.00f);
   glVertex3f(   0.00f,   0.00f, -20.00f);
   glEnd();
   glDisable(GL_LINE_STIPPLE);
   /*---(positive Y :: purple course dashed)-------------*/
   glLineStipple(2, 0x0F0F);
   glBegin(GL_LINES);
   glColor3f(1.0f, 0.0f, 1.0f);
   glVertex3f(   0.00f,   0.00f,   0.00f);
   glVertex3f(   0.00f,  20.00f,   0.00f);
   glEnd();
   /*---(negative Y :: purple fine dotted)---------------*/
   glLineStipple(2, 0xAAAA);
   glColor3f(1.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(   0.00f,   0.00f,   0.00f);
   glVertex3f(   0.00f, -20.00f,   0.00f);
   glEnd();
   glDisable(GL_LINE_STIPPLE);
   return;
}


void
draw_radians()
{
   /*---(xz radians)-------------------------------------*/
   float x, y, z, r;
   int   d, i;
   glColor3f(0.0f, 0.0f, 1.0f);
   for (d = 0; d < 365; d += 5) {
      glBegin(GL_LINES);
      r   = d * ((2 * M_PI) / 360);
      x   = 12.0f * cos(r);
      z   = 12.0f * sin(r);
      glVertex3f( x,  0.00f, z);
      if (d % 15 == 0) {
         x   = 13.5f * cos(r);
         z   = 13.5f * sin(r);
      } else {
         x   = 12.5f * cos(r);
         z   = 12.5f * sin(r);
      }
      glVertex3f( x,  0.00f, z);
      glEnd();
      if ((d + 45) % 90 == 0) {
         glLineWidth(2.50f);
         for (i = 0; i < (d + 45)/90; ++i) {
            glBegin(GL_LINES);
            r = (d + (i * 2)) * ((2 * M_PI) / 360);
            x   = 14.0f * cos(r);
            z   = 14.0f * sin(r);
            glVertex3f(x,  0.0f, z);
            x   = 15.0f * cos(r);
            z   = 15.0f * sin(r);
            glVertex3f(x,  0.0f, z);
            glEnd();
         }
         glLineWidth(0.50f);
      }
   }
   /*---(xy radians)-------------------------------------*/
   glColor3f(1.0f, 0.0f, 1.0f);
   for (d = 0; d < 365; d += 5) {
      glBegin(GL_LINES);
      r   = d * ((2 * M_PI) / 360);
      x   = 12.0f * cos(r);
      y   = 12.0f * sin(r);
      glVertex3f( x,  y, 0.00f);
      if (d % 15 == 0) {
         x   = 13.5f * cos(r);
         y   = 13.5f * sin(r);
      } else {
         x   = 12.5f * cos(r);
         y   = 12.5f * sin(r);
      }
      glVertex3f( x,  y,  0.00f);
      glEnd();
      if ((d + 45) % 90 == 0) {
         glLineWidth(2.50f);
         for (i = 0; i < (d + 45)/90; ++i) {
            glBegin(GL_LINES);
            r = (d + (i * 2)) * ((2 * M_PI) / 360);
            x   = 14.0f * cos(r);
            y   = 14.0f * sin(r);
            glVertex3f(x,  y, 0.0f);
            x   = 15.0f * cos(r);
            y   = 15.0f * sin(r);
            glVertex3f(x,  y, 0.0f);
            glEnd();
         }
         glLineWidth(0.50f);
      }
   }
   /*---(yz radians)-------------------------------------*/
   glColor3f(0.0f, 1.0f, 1.0f);
   for (d = 0; d < 365; d += 5) {
      glBegin(GL_LINES);
      r   = d * ((2 * M_PI) / 360);
      y   = 12.0f * cos(r);
      z   = 12.0f * sin(r);
      glVertex3f( 0.0f,  y,  z);
      if (d % 15 == 0) {
         y   = 13.5f * cos(r);
         z   = 13.5f * sin(r);
      } else {
         y   = 12.5f * cos(r);
         z   = 12.5f * sin(r);
      }
      glVertex3f( 0.0f,  y,  z);
      glEnd();
      if ((d + 45) % 90 == 0) {
         glLineWidth(2.50f);
         for (i = 0; i < (d + 45)/90; ++i) {
            glBegin(GL_LINES);
            r = (d + (i * 2)) * ((2 * M_PI) / 360);
            y   = 14.0f * cos(r);
            z   = 14.0f * sin(r);
            glVertex3f(0.0f,  y, z);
            y   = 15.0f * cos(r);
            z   = 15.0f * sin(r);
            glVertex3f(0.0f,  y, z);
            glEnd();
         }
         glLineWidth(0.50f);
      }
   }
   return;
}


void
position_draw(int a_leg)
{
   int l = a_leg;
   float x1 = modelview[0][12];
   float y1 = modelview[0][13];
   float z1 = modelview[0][14];
   float x2 = modelview[l][12];
   float y2 = modelview[l][13];
   float z2 = modelview[l][14];
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x0F0F);
   glColor3f(   0.00f,  0.00f,  1.00f);
   glBegin(GL_TRIANGLES);
   glVertex3f(  x1, y1, z1);
   glVertex3f(  x1, y2, z1);
   glVertex3f(  x2, y2, z2);
   glEnd();
   glDisable(GL_LINE_STIPPLE);
   return;
}


/*> void                                                                                        <* 
 *> leg_init           (tSEG *a_leg)                                                            <* 
 *> {                                                                                           <* 
 *>    /+---(initialize lengths)-------------------+/                                           <* 
 *>    /+> a_leg.body.l = 4.25;                                                           <*    <* 
 *>     *> a_leg.coxa.l = 1.12;                                                           <*    <* 
 *>     *> a_leg.femu.l = 1.00;                                                           <*    <* 
 *>     *> a_leg.pate.l = 2.25;                                                           <*    <* 
 *>     *> a_leg.tibi.l = 5.50;                                                           <*    <* 
 *>     *> a_leg.tars.l = 0.00;                                                           <*    <* 
 *>     *> a_leg.bott.l = 0.00;                                                           <*    <* 
 *>     *> a_leg.full.l = 0.00;                                                           <*    <* 
 *>     *> a_leg.targ.l = 0.00;                                                           <+/   <* 
 *>    /+> a_leg->[YKINE_THOR].l = 4.25;                                                        <*    <* 
 *>     *> a_leg->[YKINE_COXA].l = 1.12;                                                        <*    <* 
 *>     *> a_leg->[YKINE_FEMU].l = 1.00;                                                        <*    <* 
 *>     *> a_leg->[YKINE_PATE].l = 2.25;                                                        <*    <* 
 *>     *> a_leg->[YKINE_TIBI].l = 5.50;                                                        <*    <* 
 *>     *> a_leg->[YKINE_TARS].l = 0.00;                                                        <*    <* 
 *>     *> a_leg->[BOTT].l = 0.00;                                                        <*    <* 
 *>     *> a_leg->[FULL].l = 0.00;                                                        <*    <* 
 *>     *> a_leg->[YKINE_TARG].l = 0.00;                                                        <+/   <* 
 *>    return;                                                                                  <* 
 *> }                                                                                           <*/


/*> void                                                                                              <* 
 *> leg_ik(int a_num, struct cFK *a_leg, float a_x, float a_z, float a_y)                             <* 
 *> {                                                                                                 <* 
 *>    static   float   lx = 0.0;                                                                     <* 
 *>    static   float   lz = 0.0;                                                                     <* 
 *>    static   float   ly = 0.0;                                                                     <* 
 *>    float    x, y, z;            /+ basic x, y, z    +/                                            <* 
 *>    float    dx, dy, dz;         /+ delta x, y, z    +/                                            <* 
 *>    float    r, d;               /+ rads and degs    +/                                            <* 
 *>    float    l, xz;              /+ full and xz lens +/                                            <* 
 *>    float    v, h, dh;           /+ vert/horz rads   +/                                            <* 
 *>    /+---(defenses)-------------------------------------+/                                         <* 
 *>    if (a_x == lx && a_z == lz && a_y == ly) return;                                               <* 
 *>    lx = a_x;                                                                                      <* 
 *>    ly = a_y;                                                                                      <* 
 *>    lz = a_z;                                                                                      <* 
 *>    /+---(set target)-----------------------------------+/                                         <* 
 *>    /+> printf("leg_ik----------------------------\n");                                <+/         <* 
 *>    x   = a_leg[6].x   = a_x;                                                                      <* 
 *>    z   = a_leg[6].z   = a_z;                                                                      <* 
 *>    y   = a_leg[6].y   = a_y;                                                                      <* 
 *>    /+---(calc remaining target info)-------------------+/                                         <* 
 *>    l   = a_leg[6].l   = sqrt((x * x) + (y * y) + (z * z));                                        <* 
 *>    xz  = a_leg[6].xz  = sqrt((x * x) + (z * z));                                                  <* 
 *>    h   = a_leg[6].h  = atan2(z, x);                                                               <* 
 *>    v   = a_leg[6].v  = atan2(y, xz);                                                              <* 
 *>    /+> printf("   target    %8.0fl  %8.0fd  %8.0fx  %8.0fz  %8.0fy\n", l, 0.0, x, z, y);   <+/    <* 
 *>    /+---(body)-----------------------------------------+/                                         <* 
 *>    d   = (float) (a_num - 1) * 60.0f;                                                             <* 
 *>    r   = d * ((2 * M_PI) / 360);                                                                  <* 
 *>    /+> l   = a_leg[0].l  = 4.25;                                                      <+/         <* 
 *>    l   = a_leg[0].l  = 125.0;                                                                     <* 
 *>    h   = a_leg[0].h  = r;                                                                         <* 
 *>    x   = a_leg[0].x  = l * cos(r);                                                                <* 
 *>    z   = a_leg[0].z  = l * sin(r);                                                                <* 
 *>    y   = a_leg[0].v  = 141.0 - 17.0;                                                              <* 
 *>    xz  = a_leg[0].xz  = sqrt((l * l) - (y * y));                                                  <* 
 *>    /+> printf("   body      %8.0fl  %8.0fd  %8.0fx  %8.0fz  %8.0fy\n", l, d, x, z, y);   <+/      <* 
 *>    /+---(coxa)-----------------------------------------+/                                         <* 
 *>    /+> l   = a_leg[1].l  = 1.12;                                                      <+/         <* 
 *>    l   = a_leg[1].l  = 30.0;                                                                      <* 
 *>    h   = a_leg[1].h  = r;                                                                         <* 
 *>    v   = a_leg[1].v  = 0.00;                                                                      <* 
 *>    x   = a_leg[1].x  = l * cos(r);                                                                <* 
 *>    z   = a_leg[1].z  = l * sin(r);                                                                <* 
 *>    y   = a_leg[1].y  = 141.0;                                                                     <* 
 *>    xz  = a_leg[1].xz  = sqrt((l * l) - (y * y));                                                  <* 
 *>    /+> printf("   coxa      %8.0fl  %8.0fd  %8.0fx  %8.0fz  %8.0fy\n", l, d, x, z, y);   <+/      <* 
 *>    /+---(femur)----------------------------------------+/                                         <* 
 *>    dx  = a_leg[6].x - a_leg[0].x - a_leg[1].x;                                                    <* 
 *>    dz  = a_leg[6].z - a_leg[0].z - a_leg[1].z;                                                    <* 
 *>    dy  = a_leg[6].y - a_leg[0].y - a_leg[1].y;                                                    <* 
 *>    l  = sqrt((dx * dx) + (dy * dy) + (dz * dz));                                                  <* 
 *>    /+> printf("   remain    %8.0fl  %8.0fd  %8.0fx  %8.0fz  %8.0fy\n", l, d, dx, dz, dy);   <+/   <* 
 *>    xz = sqrt((dx * dx) + (dz * dz));                                                              <* 
 *>    h  = atan2(dz, dx);                                                                            <* 
 *>    dh = h - a_leg[1].h;                                                                           <* 
 *>    if      (dh >=  1.57) h = a_leg[1].h;                                                          <* 
 *>    else if (dh <= -1.57) h = a_leg[1].h;                                                          <* 
 *>    /+> l  = a_leg[2].l = 1.00;                                                        <+/         <* 
 *>    l  = a_leg[2].l = 25.0;                                                                        <* 
 *>    a_leg[2].v = 0.00;                                                                             <* 
 *>    a_leg[2].h = h;                                                                                <* 
 *>    dx -= x  = a_leg[2].x = cos(h);                                                                <* 
 *>    dz -= z  = a_leg[2].z = sin(h);                                                                <* 
 *>    dy -= y  = a_leg[2].y = 0.00;                                                                  <* 
 *>    xz  = a_leg[2].xz  = sqrt((l * l) - (y * y));                                                  <* 
 *>    /+---(patella/tibia)--------------------------------+/                                         <* 
 *>    float a1, a2, a3, aY;                                                                          <* 
 *>    float d1, d2, d3, dY;                                                                          <* 
 *>    l  = sqrt((dx * dx) + (dz * dz) + (dy * dy));                                                  <* 
*>    xz = sqrt((dx * dx) + (dz * dz));                                                              <* 
*>    a1 = acos(((l * l)   + (5.50 * 5.50) - (2.25 * 2.25)) / (2 * l  * 5.50));                      <* 
*>    a2 = acos(((l * l)   + (2.25 * 2.25) - (5.50 * 5.50)) / (2 * l  * 2.25));                      <* 
*>    a3 = acos(((5.50 * 5.50) + (2.25 * 2.25) - (l * l)  ) / (2 * 2.25 * 5.50));                    <* 
*>    aY = cos(dy / l);                                                                              <* 
*>    d1 = a1 * (360 / (2 * M_PI));                                                                  <* 
*>    d2 = a2 * (360 / (2 * M_PI));                                                                  <* 
*>    d3 = a3 * (360 / (2 * M_PI));                                                                  <* 
*>    dY = aY * (360 / (2 * M_PI));                                                                  <* 
*>    x = a_leg[6].x;                                                                                <* 
*>    z = a_leg[6].z;                                                                                <* 
*>    float xz6 = sqrt(( x *  x) + ( z *  z));                                                       <* 
*>    x = a_leg[2].x + a_leg[1].x + a_leg[0].x;                                                      <* 
*>    z = a_leg[2].z + a_leg[1].z + a_leg[0].z;                                                      <* 
*>    float xz3 = sqrt(( x *  x) + ( z *  z));                                                       <* 
*>    float dxz = 1;                                                                                 <* 
*>    if (xz6 < xz3) dxz *= -1;                                                                      <* 
*>    /+---(DEBUG)----------------------------------+/                                               <* 
*>    a_leg[5].l  = l;                                                                               <* 
*>    a_leg[5].h  = h;                                                                               <* 
*>    x   = a_leg[5].x   = dx;                                                                       <* 
*>    z   = a_leg[5].z   = dz;                                                                       <* 
*>    y   = a_leg[5].y   = dy;                                                                       <* 
*>    xz  = a_leg[5].xz  = sqrt((dx * dx) + (dz * dz)) * dxz;                                        <* 
*>    v   = a_leg[5].v   = atan2(y, xz);                                                             <* 
*>    if (v < -(M_PI / 2)) {                                                                         <* 
   *>       a_leg[5].h   = h   += M_PI;                                                                 <* 
      *>    }                                                                                              <* 
      *>    /+---(DEBUG)----------------------------------+/                                               <* 
      *>    a_leg[4].h   = a_leg[3].h = a_leg[2].h;                                                        <* 
      *>    /+---(patella)--------------------------------+/                                               <* 
      *>    /+> l   = a_leg[3].l   = 2.25;                                                     <+/         <* 
      *>    l   = a_leg[3].l   = 57.0;                                                                     <* 
      *>    h   = a_leg[3].h   = a_leg[2].h;                                                               <* 
      *>    v   = a_leg[3].v   = a_leg[5].v + a2;                                                          <* 
      *>    y   = a_leg[3].y   = l  * sin(v);                                                              <* 
      *>    xz  = a_leg[3].xz  = l  * cos(v);                                                              <* 
      *>    x   = a_leg[3].x   = xz * cos(h);                                                              <* 
      *>    z   = a_leg[3].z   = xz * sin(h);                                                              <* 
      *>    /+---(tibia)----------------------------------+/                                               <* 
      *>    /+> l   = a_leg[4].l   = 5.50;                                                     <+/         <* 
      *>    l   = a_leg[4].l   = 141.0;                                                                    <* 
      *>    h   = a_leg[4].h   = a_leg[3].h;                                                               <* 
      *>    v   = a_leg[4].v   = a_leg[3].v - (M_PI - a3);                                                 <* 
      *>    y   = a_leg[4].y   = l  * sin(v);                                                              <* 
      *>    xz  = a_leg[4].xz  = l  * cos(v);                                                              <* 
      *>    x   = a_leg[4].x   = xz * cos(h);                                                              <* 
      *>    z   = a_leg[4].z   = xz * sin(h);                                                              <* 
      *>    if (v < -(M_PI / 2)) {                                                                         <* 
         *>       a_leg[4].h   = h   += M_PI;                                                                 <* 
            *>    }                                                                                              <* 
            *>    /+> printf("----------------------------------\n");                                <+/         <* 
            *>    return;                                                                                        <* 
            *> }                                                                                                 <*/

void
draw_masscenter    (void)
{
   /*---(locals)-------------------------*/
   float     x, z;
   int       deg;
   float     rad;
   /*---(center)-------------------------*/
   glPushMatrix (); {
      /*> glTranslatef(center.mcx + center.bx, center.mcy - center.by, center.mcz + center.bz);   <*/
      glTranslatef(center.mcx, center.mcy, center.mcz);
      glLineWidth ( 5.0);
      glColor4f (1.0f, 0.0f, 0.0f, 1.0f);
      glBegin   (GL_LINE_STRIP);
      for (deg = 0; deg < 365; deg += 5) {
         rad = deg * DEG2RAD;
         x   = 15.0 * cos (rad);
         z   = 15.0 * sin (rad);
         glVertex3f ( x, 0.00f, z);
      }
      glEnd     ();
      glLineWidth ( 0.5);
   } glPopMatrix ();
   /*---(complete)-----------------------*/
   return;
}

void
draw_arrow()
{
   /*---(locals)-------------------------*/
   float     x, z;
   int       deg;
   float     rad;
   /*---(arrow)--------------------------*/
   glColor3f(0.0f, 1.0f, 0.0f);
   glBegin(GL_LINE_STRIP);
   glVertex3f( 0.0f * 25, 0.0f * 25,-1.0f * 25);
   glVertex3f( 1.0f * 25, 0.0f * 25, 0.0f * 25);
   glVertex3f( 0.5f * 25, 0.0f * 25, 0.0f * 25);
   glVertex3f( 0.5f * 25, 0.0f * 25, 1.0f * 25);
   glVertex3f(-0.5f * 25, 0.0f * 25, 1.0f * 25);
   glVertex3f(-0.5f * 25, 0.0f * 25, 0.0f * 25);
   glVertex3f(-1.0f * 25, 0.0f * 25, 0.0f * 25);
   glVertex3f( 0.0f * 25, 0.0f * 25,-1.0f * 25);
   glEnd();
   /*---(center)-------------------------*/
   glLineWidth ( 5.0);
   glColor4f (0.5f, 0.0f, 1.0f, 0.5f);
   glBegin   (GL_QUAD_STRIP);
   for (deg = 0; deg < 365; deg += 30) {
      rad = deg * DEG2RAD;
      x   = 5.0 * cos (rad);
      z   = 5.0 * sin (rad);
      glVertex3f ( x,    0.00f, z);
      glVertex3f ( x, -130.00f, z);
   }
   glEnd     ();
   glLineWidth ( 0.5);
   /*---(complete)-----------------------*/
   return;
}

void
draw_contact       (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;
   float     x, z;
   int       deg;
   float     rad;
   /*---(polygon)------------------------*/
   glColor4f       (0.5f, 0.0f, 1.0f, 0.1f);
   glPolygonMode   (GL_FRONT_AND_BACK, GL_FILL);
   glBegin         (GL_POLYGON);
   for (i = 0; i < LEGS; ++i) {
      if (gait.tleg[(int) my.p_cursec][i] != 1) continue;
      glVertex3f      ( gk [i][YKINE_TIBI].cx - center.bx, -center.by , gk [i][YKINE_TIBI].cz - center.bz);
   }
   glEnd           ();
   /*---(touches)------------------------*/
   glLineWidth ( 5.0);
   for (i = 0; i < LEGS; ++i) {
      switch (gait.tleg[(int) my.p_cursec][i]) {
      case  0  : 
         glColor4f       (0.2f, 0.2f, 0.2f, 1.0f);
         break;
      case  1  : 
         glColor4f       (0.5f, 0.0f, 1.0f, 1.0f);
         break;
      case -1  : 
         glColor4f       (1.0f, 0.0f, 0.5f, 1.0f);
         break;
      }
      if (gait.tleg[(int) my.p_cursec][i] != 0) {
         glBegin   (GL_LINE_STRIP);
         for (deg = 0; deg < 365; deg +=  5) {
            rad = deg * DEG2RAD;
            x   = 25.0 * cos (rad) + gk[i][YKINE_TIBI].cx - center.bx;
            z   = 25.0 * sin (rad) + gk[i][YKINE_TIBI].cz - center.bz;
            glVertex3f ( x, -center.by + 2.0, z);
         }
         glEnd     ();
      }
      glBegin   (GL_LINE_STRIP);
      for (deg = 0; deg < 365; deg +=  5) {
         rad = deg * DEG2RAD;
         x   =  5.0 * cos (rad) + gk[i][YKINE_TIBI].cx - center.bx;
         z   =  5.0 * sin (rad) + gk[i][YKINE_TIBI].cz - center.bz;
         glVertex3f ( x, -center.by + 2.0, z);
      }
      glEnd     ();
   }
   glLineWidth ( 0.5);
   /*---(complete)-----------------------*/
   return;
}




/*============================================================================*
 *=======                         END OF SOURCE                        =======*
 *============================================================================*/
