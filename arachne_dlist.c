/*============================----beg-of-source---============================*/


#include "arachne.h"

static   int       dlist_ground       (void);
static   int       dlist_ruler        (void);
static   int       dlist_body         (void);
static   int       dlist_beak         (void);
static   int       dlist_coxa         (void);
static   int       dlist_femur        (void);
static   int       dlist_patella      (void);
static   int       dlist_tibia        (void);
static   int       dlist_foot         (void);


char       /* ---- : prepare display lists for use ---------------------------*/
dlist_begin        (void)
{
   dlist_ground  ();
   dlist_ruler   ();
   dlist_body    ();
   dlist_beak    ();
   dlist_coxa    ();
   dlist_femur   ();
   dlist_patella ();
   dlist_tibia   ();
   dlist_foot    ();
   return 0;
}

char       /* ---- : prepare display lists for use ---------------------------*/
dlist_end          (void)
{
   glDeleteLists (dl_ground  , 1);
   glDeleteLists (dl_ruler   , 1);
   glDeleteLists (dl_body    , 1);
   glDeleteLists (dl_beak    , 1);
   glDeleteLists (dl_coxa    , 1);
   glDeleteLists (dl_femur   , 1);
   glDeleteLists (dl_patella , 1);
   glDeleteLists (dl_tibia   , 1);
   glDeleteLists (dl_foot    , 1);
   return 0;
}


static float    s_side    = 25.4;   /*  one inch in millimeters               */
static float    s_xoff    =  0.0;
static float    s_zoff    =  0.0;
static float    s_xinc    =  0.0;
static float    s_zinc    =  0.0;

static float    y         =  0.0;


static char
dlist__hex         (int l, char c, float x, float z)
{
   float       y           =  0.0;
   /* exterior hexagon */
   if (l > 0 && l < 3)  return 0;
   switch (c) {
   case 'y' : glColor4f (0.8f, 0.4f, 0.2f, 1.0f);  break;
   case 'i' : glColor4f (0.2f, 0.2f, 0.2f, 0.3f);  break;
   case 'o' : glColor4f (0.2f, 0.2f, 0.2f, 0.1f);  break;
   case '-' : glColor4f (0.2f, 0.2f, 0.2f, 0.1f);  break;
   default  : glColor4f (0.8f, 0.8f, 0.8f, 0.1f);  break;
   }
   glBegin       (GL_LINE_STRIP); {
      glVertex3f (x - s_xoff - 0.5 * s_side, y, z         );  /* east       */
      glVertex3f (x          - 0.5 * s_side, y, z - s_zoff);  /* east-north */
      glVertex3f (x          + 0.5 * s_side, y, z - s_zoff);  /* west-north */
      glVertex3f (x + s_xoff + 0.5 * s_side, y, z         );  /* west       */
      glVertex3f (x          + 0.5 * s_side, y, z + s_zoff);  /* west-south */
      glVertex3f (x          - 0.5 * s_side, y, z + s_zoff);  /* east-south */
      glVertex3f (x - s_xoff - 0.5 * s_side, y, z         );  /* east       */
   } glEnd         ();
   /* interior triangles */
   if (l > 0 && l <= 10) {
      glColor4f (0.2f, 0.2f, 0.2f, 0.1f);
      glBegin       (GL_LINES); {
         glVertex3f (x - s_xoff - 0.5 * s_side, y, z         );  /* east       */
         glVertex3f (x + s_xoff + 0.5 * s_side, y, z         );  /* west       */
         glVertex3f (x          - 0.5 * s_side, y, z - s_zoff);  /* east-north */
         glVertex3f (x          + 0.5 * s_side, y, z + s_zoff);  /* west-south */
         glVertex3f (x          + 0.5 * s_side, y, z - s_zoff);  /* west-north */
         glVertex3f (x          - 0.5 * s_side, y, z + s_zoff);  /* east-south */
      } glEnd         ();
   }
   /*> y -= 1.0;                                                                      <*/
}

static char
dlist__level       (int n, int l, char t, char b, float x, float z)
{
   char        c           =  '-';
   int         i           =    0;
   char        x_subs      [LEN_LABEL] = ".------.";
   /*---(defense)------------------------*/
   if (l > n)  return 0;
   /*---(new coords)---------------------*/
   switch (t) {
   case  0 :                             strlcpy (x_subs, "-yyyyyy-", LEN_LABEL);  break;      /* in place          */
   case  1 :  z -= 2.0 * s_zinc;         strlcpy (x_subs, "-yb-----", LEN_LABEL);  break;      /* north             */
   case  2 :  z -= s_zinc; x += s_xinc;  strlcpy (x_subs, "--yb----", LEN_LABEL);  break;      /* north-east        */
   case  3 :  z += s_zinc; x += s_xinc;  strlcpy (x_subs, "---yb---", LEN_LABEL);  break;      /* south-east        */
   case  4 :  z += 2.0 * s_zinc;         strlcpy (x_subs, "----yb--", LEN_LABEL);  break;      /* south             */
   case  5 :  z += s_zinc; x -= s_xinc;  strlcpy (x_subs, "-----yb-", LEN_LABEL);  break;      /* south-west        */
   case  6 :  z -= s_zinc; x -= s_xinc;  strlcpy (x_subs, "-b----y-", LEN_LABEL);  break;      /* north-west        */
   }
   /*---(colors)-------------------------*/
   if      (l ==  0)   c = 'y';
   else if (l <=  6)   c = 'i';
   else if (l <= 10)   c = 'o';
   /*---(drawa)--------------------------*/
   dlist__hex (l, c, x, z);
   /*---(branch)-------------------------*/
   for (i = 1; i <= 6; ++i) {
      if (x_subs [i] == '-')              continue;
      if (x_subs [i] == 'b') {
         if (b == 'b')                    continue;
         dlist__level (n, l + 1, i, 'b', x, z);
      } else {
         dlist__level (n, l + 1, i,  b , x, z);
      }
   }
   /*---(done)---------------------------*/
   return 0;
}

static char
dlist__compass    (void)
{
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (    0.00f,   20.00f, -750.00f);
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      yFONT_print  (my.fixed, 45 , YF_BASCEN, "n");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (    0.00f,   20.00f,  750.00f);
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      yFONT_print  (my.fixed, 45 , YF_BASCEN, "s");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef ( -750.00f * sin (60 * DEG2RAD), 0.0f, -750.00f * cos (60 * DEG2RAD));
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      yFONT_print  (my.fixed, 45 , YF_BASCEN, "nw");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (  750.00f * sin (60 * DEG2RAD), 0.0f, -750.00f * cos (60 * DEG2RAD));
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      yFONT_print  (my.fixed, 45 , YF_BASCEN, "ne");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef ( -750.00f * sin (60 * DEG2RAD), 0.0f,  750.00f * cos (60 * DEG2RAD));
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      yFONT_print  (my.fixed, 45 , YF_BASCEN, "sw");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (  750.00f * sin (60 * DEG2RAD), 0.0f,  750.00f * cos (60 * DEG2RAD));
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      yFONT_print  (my.fixed, 45 , YF_BASCEN, "se");
   } glPopMatrix();
   return 0;
}

static int
dlist_ground            (void)
{
   dl_ground = glGenLists(1);
   glNewList(dl_ground, GL_COMPILE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   /*---(begin)-----------------------------*/
   s_side      =  25.4;
   s_xoff      = s_side * cos (60 * DEG2RAD);
   s_zoff      = s_side * sin (60 * DEG2RAD);
   s_xinc      = s_side + s_xoff;
   s_zinc      = s_zoff;
   dlist__level (14, 0, 0, '-', 0, 0);
   dlist__compass ();
   /*---(end)-------------------------------*/
   glEndList();
   return 0;
}

static int
dlist_ruler             (void)
{
   float       i           =  0.0;
   float       j           =  0.0;
   char        n           [LEN_LABEL] = "a";
   dl_ruler = glGenLists(1);
   glNewList(dl_ruler, GL_COMPILE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   /*---(begin)-----------------------------*/
   glColor4f (0.0, 0.0, 0.0, 1.0);
   for (i = 0; i < 360; i += 60) {
      glPushMatrix    (); {
         glRotatef (i, 0.0f, 1.0f, 0.0f);
         glColor4f (0.3, 0.3, 0.3, 0.3);
         glBegin(GL_LINES); {
            glVertex3f ( 0.0 * 25.4,  1.00f,  0.00f);
            glVertex3f ( 3.0 * 25.4,  1.00f,  0.00f);
         } glEnd();
         glColor4f (0.0, 0.0, 0.0, 1.0);
         glBegin(GL_LINES); {
            glVertex3f ( 3.0 * 25.4,  1.00f,  0.00f);
            glVertex3f (16.0 * 25.4,  1.00f,  0.00f);
         } glEnd();
         n [0] = 'a';
         for (j = 3.0 * 25.4; j < 16.0 * 25.4; j += 25.4 * 0.50) {
            glBegin(GL_LINES); {
               glVertex3f (  j, 1.00f, -1.00f);
               glVertex3f (  j, 1.00f,  1.00f);
               glVertex3f (  j, 0.00f,  0.00f);
               glVertex3f (  j, 2.00f,  0.00f);
            } glEnd();
            glPushMatrix    (); {
               glTranslatef( j, 4.00f, -0.00f);
               yFONT_print  (my.fixed,  6 , YF_BASCEN, n);
            } glPopMatrix();
            ++n [0];
         }
         /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      } glPopMatrix();
   }
   n [0] = 'a';
   glBegin(GL_LINES); {
      glVertex3f (0.0, -4.5 * 25.4, 0.0);
      glVertex3f (0.0,  8.0 * 25.4, 0.0);
   } glEnd();
   for (j = -4.5 * 25.4; j <  8.0 * 25.4; j += 25.4 * 0.50) {
      glBegin(GL_LINES); {
         glVertex3f ( 1.0f,  j,  0.00f);
         glVertex3f (-1.0f,  j,  0.00f);
         glVertex3f ( 0.0f,  j,  1.00f);
         glVertex3f ( 0.0f,  j, -1.00f);
      } glEnd();
      glPushMatrix    (); {
         glTranslatef( 2.00 , j,  0.00f);
         yFONT_print  (my.fixed,  6 , YF_MIDLEF, n);
      } glPopMatrix();
      ++n [0];
   }
   /*---(end)-------------------------------*/
   glEndList();
   return 0;
}

static char  /* ---- : draw a standard joint -----------------------------------*/
dlist__joint       (float  a_min, float  a_max)
{
   /*---(locals)-------*-----------------*/
   int       d;
   float     r;
   float     nx, ny;
   float     px, py;
   /*---(setup)--------------------------*/
   glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
   /*> glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);                                    <*/
   glLineWidth   (0.5);
   glColor3f     (1.0f, 1.0f, 0.0f);
   /*---(draw segment)-------------------*/
   glColor3f     (1.0f, 1.0f, 0.0f);
   px = py = 0.0;
   for (d = 0; d < 365; d += 30) {
      /*---(calc)------------------------*/
      r = d * ((2 * M_PI) / 360);
      nx   = 0.50 * 12 * cos(r);
      ny   = 0.50 * 12 * sin(r);
      /*---(color)-----------------------*/
      if (d % 60 == 0) glColor3f     (1.0f, 1.0f, 0.0f);
      else             glColor3f     (1.0f, 0.0f, 0.0f);
      /*---(draw)------------------------*/
      if (d != 0) {
         glBegin       (GL_POLYGON); {
            glVertex3f ( px, py,  0.50 * 30);
            glVertex3f ( px, py, -0.50 * 30);
            glVertex3f ( nx, ny, -0.50 * 30);
            glVertex3f ( nx, ny,  0.50 * 30);
         } glEnd         ();
      }
      /*---(prepare)---------------------*/
      px = nx;
      py = ny;
      /*---(done)------------------------*/
   }
   /*---(draw allowed angle fan)---------*/
   /*> glBegin       (GL_TRIANGLE_FAN);                                               <* 
    *> glColor3f     (0.0f, 0.5f, 0.0f);                                              <* 
    *> glVertex3f ( 0.00, 0.00,  0.00);                                               <* 
    *> for (d = a_min; d < a_max; d +=  5) {                                          <* 
    *>    r = d * ((2 * M_PI) / 360);                                                 <* 
    *>    x   = 1.50 * 25 * cos(r);                                                   <* 
    *>    y   = 1.50 * 25 * sin(r);                                                   <* 
    *>    glVertex3f ( x, y,  0.00);                                                  <* 
    *> }                                                                              <* 
    *> glEnd         ();                                                              <*/
   /*---(complete)-----------------------*/
   return 0;
}

static int   /* ---- : create a saved shape for a tibia ------------------------*/
dlist_tibia        (void)
{
   /*---(locals)-------*-----------------*/
   float     z         = 0.0f;
   /*---(begin)--------------------------*/
   dl_tibia = glGenLists(1);
   glNewList(dl_tibia, GL_COMPILE);
   /*---(draw both sides)----------------*/
   for(z = -0.20 * 25; z <= 0.20 * 25; z += 0.39 * 25) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      /*---(interior)--------------------*/
      glColor4f(0.0f, 0.4f, 0.0f, 0.5f);
      glBegin(GL_TRIANGLE_FAN); {
         glVertex3f (-0.50f * 25,  0.50f * 25,  z);
         glVertex3f (-2.00f * 25,  0.50f * 25,  z);
         glVertex3f (-1.00f * 25,  0.00f * 25,  z);
         glVertex3f (-0.75f * 25, -0.50f * 25,  z);
         glVertex3f ( 1.50f * 25, -0.50f * 25,  z);
         glVertex3f ( 2.50f * 25, -0.00f * 25,  z);
         glVertex3f ( 3.50f * 25,  0.15f * 25,  z);
         glVertex3f (-0.25f * 25,  1.00f * 25,  z);
      } glEnd      ();
      glBegin(GL_TRIANGLE_FAN); {
         glVertex3f (-0.25f * 25,  1.00f * 25,  z);
         glVertex3f ( 3.50f * 25,  0.15f * 25,  z);
         glVertex3f ( 4.50f * 25, -0.00f * 25,  z);
         glVertex3f ( 5.15f * 25, -0.20f * 25,  z);
         glVertex3f ( 5.20f * 25, -0.00f * 25,  z);
         glVertex3f ( 5.15f * 25,  0.20f * 25,  z);
         glVertex3f ( 3.50f * 25,  0.75f * 25,  z);
         glVertex3f ( 1.50f * 25,  1.50f * 25,  z);
         glVertex3f (-0.00f * 25,  1.75f * 25,  z);
         glVertex3f (-1.00f * 25,  1.50f * 25,  z);
      } glEnd      ();
      /*---(outline to clarify)----------*/
      glColor4f(0.0f, 0.1f, 0.0f, 1.0f);
      glLineWidth(2.0);
      glBegin    (GL_LINE_STRIP); {
         glVertex3f (-2.00f * 25,  0.50f * 25,  z);
         glVertex3f (-1.00f * 25,  0.00f * 25,  z);
         glVertex3f (-0.75f * 25, -0.50f * 25,  z);
         glVertex3f ( 1.50f * 25, -0.50f * 25,  z);
         glVertex3f ( 2.50f * 25, -0.00f * 25,  z);
         glVertex3f ( 3.50f * 25,  0.15f * 25,  z);
         glVertex3f ( 4.50f * 25, -0.00f * 25,  z);
         glVertex3f ( 5.15f * 25, -0.20f * 25,  z);
         glVertex3f ( 5.20f * 25, -0.00f * 25,  z);
         glVertex3f ( 5.15f * 25,  0.20f * 25,  z);
         glVertex3f ( 3.50f * 25,  0.75f * 25,  z);
         glVertex3f ( 1.50f * 25,  1.50f * 25,  z);
         glVertex3f (-0.00f * 25,  1.75f * 25,  z);
         glVertex3f (-1.00f * 25,  1.50f * 25,  z);
         glVertex3f (-0.25f * 25,  1.00f * 25,  z);
         glVertex3f (-0.50f * 25,  0.50f * 25,  z);
         glVertex3f (-2.00f * 25,  0.50f * 25,  z);
      } glEnd      ();
      /*---(interior hole)---------------*/
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      glBegin(GL_POLYGON); {
         glVertex3f( 0.75f * 25,  1.10f * 25,  z * 2);
         glVertex3f( 1.20f * 25,  0.25f * 25,  z * 2);
         glVertex3f( 2.00f * 25,  0.43f * 25,  z * 2);
         glVertex3f( 3.25f * 25,  0.50f * 25,  z * 2);
      } glEnd();
   }
   /*---(lengthwise alignment line)------*/
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glColor3f(0.3f, 0.8f, 0.0f);
   glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0xA0A0);
   glLineWidth (2.0);
   glBegin(GL_LINES); {
      glVertex3f( -5.00f     * 25,  0.00f,  0.00f);
      glVertex3f( segs_len [YKINE_TIBI] + (0.5 * 25.0) ,  0.00f,  0.00f);
   } glEnd();
   glBegin(GL_LINES); {
      glVertex3f(  5.20f * 25.0, -0.50f * 25.0,  0.00f);
      glVertex3f(  5.20f * 25.0,  0.50f * 25.0,  0.00f);
   } glEnd();
   glBegin(GL_LINES); {
      glVertex3f(  5.20f * 25.0, 0.00f, -0.50f * 25.0);
      glVertex3f(  5.20f * 25.0, 0.00f,  0.50f * 25.0);
   } glEnd();
   glLineWidth (0.5);
   glDisable(GL_LINE_STIPPLE);
   /*---(prepare for next segment)-------*/
   glTranslatef( segs_len [YKINE_TIBI], 0.00f,  0.00f);
   glEndList();
   /*---(complete)-----------------------*/
   return 0;
}

static int
dlist_patella      ()
{
   dl_patella = glGenLists(1);
   glNewList(dl_patella, GL_COMPILE); {
      /*> glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  <*/
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      /*---(interior)--------------------*/
      float z = 0.0f;
      for(z = -0.50 * 25; z <= 0.50 * 25; z += 0.99 * 25) {
         glColor4f    (0.0f, 0.2f, 0.0f, 0.5f);
         glBegin(GL_POLYGON); {
            glVertex3f( 0.00f * 25,   0.00f * 25, z);
            glVertex3f( 0.50f * 25,   0.75f * 25, z);
            glVertex3f( 2.50f * 25,   0.75f * 25, z);
            glVertex3f( 2.75f * 25,   0.50f * 25, z);
            glVertex3f( 2.75f * 25,  -0.50f * 25, z);
            glVertex3f( 2.50f * 25,  -0.75f * 25, z);
            glVertex3f( 0.50f * 25,  -0.75f * 25, z);
         } glEnd();
         glColor4f    (0.0f, 0.5f, 0.0f, 1.0f);
         glLineWidth  (2.0);
         glBegin    (GL_LINE_STRIP); {
            glVertex3f( 0.00f * 25,   0.00f * 25, z);
            glVertex3f( 0.50f * 25,   0.75f * 25, z);
            glVertex3f( 2.50f * 25,   0.75f * 25, z);
            glVertex3f( 2.75f * 25,   0.50f * 25, z);
            glVertex3f( 2.75f * 25,  -0.50f * 25, z);
            glVertex3f( 2.50f * 25,  -0.75f * 25, z);
            glVertex3f( 0.50f * 25,  -0.75f * 25, z);
            glVertex3f( 0.00f * 25,   0.00f * 25, z);
         } glEnd();
      }
      /*---(prepare for next segment)------------------------*/
      glTranslatef( segs_len [YKINE_PATE], 0.00f,  0.00f);
      /*---(draw)--------------------------------------------*/
      dlist__joint  (segs_min [YKINE_TIBI], segs_max [YKINE_TIBI]);
   } glEndList();
   return 0;
}

static int
dlist_femur     ()
{
   dl_femur = glGenLists(1);
   glNewList(dl_femur, GL_COMPILE); {
      /*> glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  <*/
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      float z = 0.0f;
      /*---(begin)-----------------------------*/
      for(z = -0.45 * 25; z <= 0.45 * 25; z += 0.89 * 25) {
         glColor4f    (0.0f, 0.0f, 0.5f, 0.5f);
         glBegin(GL_POLYGON); {
            glVertex3f( 0.00f * 25,  0.00f * 25, z);
            glVertex3f( 0.50f * 25,  0.75f * 25, z);
            glVertex3f( 1.25f * 25,  0.75f * 25, z);
            glVertex3f( 1.50f * 25,  0.50f * 25, z);
            glVertex3f( 1.50f * 25, -0.50f * 25, z);
            glVertex3f( 1.25f * 25, -0.75f * 25, z);
            glVertex3f( 0.50f * 25, -0.75f * 25, z);
         } glEnd();
         glColor4f    (0.0f, 0.0f, 0.5f, 1.0f);
         glLineWidth  (2.0);
         glBegin    (GL_LINE_STRIP); {
            glVertex3f( 0.00f * 25,  0.00f * 25, z);
            glVertex3f( 0.50f * 25,  0.75f * 25, z);
            glVertex3f( 1.25f * 25,  0.75f * 25, z);
            glVertex3f( 1.50f * 25,  0.50f * 25, z);
            glVertex3f( 1.50f * 25, -0.50f * 25, z);
            glVertex3f( 1.25f * 25, -0.75f * 25, z);
            glVertex3f( 0.50f * 25, -0.75f * 25, z);
            glVertex3f( 0.00f * 25,  0.00f * 25, z);
         } glEnd();
      }
      /*---(prepare for next segment)------------------------*/
      glTranslatef( segs_len [YKINE_FEMU], 0.00f,  0.00f);
      /*---(draw)--------------------------------------------*/
      dlist__joint  (segs_min [YKINE_TIBI], segs_max [YKINE_TIBI]);
      /*---(end)-------------------------------*/
   } glEndList();
   return 0;
}

static int
dlist_coxa      ()
{
   dl_coxa = glGenLists(1);
   glNewList(dl_coxa, GL_COMPILE); {
      /*> glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  <*/
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      float y = 0.0f;
      /*---(begin)-----------------------------*/
      for(y = -12.00; y <=  8.00; y += 20.00) {
         /*---(segment)----------------------------*/
         glColor4f    (0.0f, 0.5f, 0.5f, 0.5f);
         glBegin(GL_POLYGON); {     /*->> size is 1" wide by 13/8" out (1.62")      */
            glVertex3f( 0.00f * 25, y,  0.00f);
            glVertex3f( 0.00f * 25, y,  0.50f * 25);
            glVertex3f( 1.37f * 25, y,  0.50f * 25);
            glVertex3f( 1.62f * 25, y,  0.25f * 25);
            glVertex3f( 1.62f * 25, y, -0.25f * 25);
            glVertex3f( 1.37f * 25, y, -0.50f * 25);
            glVertex3f( 0.00f * 25, y, -0.50f * 25);
         } glEnd();
         glColor4f    (0.0f, 0.5f, 0.5f, 1.0f);
         glLineWidth  (2.0);
         glBegin    (GL_LINE_STRIP); {
            glVertex3f( 0.00f * 25, y,  0.00f);
            glVertex3f( 0.00f * 25, y,  0.50f * 25);
            glVertex3f( 1.37f * 25, y,  0.50f * 25);
            glVertex3f( 1.62f * 25, y,  0.25f * 25);
            glVertex3f( 1.62f * 25, y, -0.25f * 25);
            glVertex3f( 1.37f * 25, y, -0.50f * 25);
            glVertex3f( 0.00f * 25, y, -0.50f * 25);
            glVertex3f( 0.00f * 25, y,  0.00f);
         } glEnd();
      }
      /*---(prepare for next segment)--------------*/
      glTranslatef( segs_len [YKINE_COXA], 0.00f,  0.00f);
      /*---(draw joint pin)------------------------*/
      glRotatef     ( 90.00f, 1.00f, 0.00f,  0.00f);
      dlist__joint  (segs_min [YKINE_TIBI], segs_max [YKINE_TIBI]);
      glRotatef     (-90.00f, 1.00f, 0.00f,  0.00f);
      /*---(end)-------------------------------*/
   } glEndList();
   return 0;
}

static char
dlist_poly              (int d, float px, float pz, float nx, float nz)
{
   if (d % 10 == 0) glColor4f (0.0f, 1.0f, 0.0f, 0.5f);
   else             glColor4f (0.0f, 0.5f, 0.0f, 0.5f);
   glBegin       (GL_POLYGON); {
      glVertex3f ( px,   8.00f,  pz);
      glVertex3f ( px, -12.00f,  pz);
      glVertex3f ( nx, -12.00f,  nz);
      glVertex3f ( nx,   8.00f,  nz);
   } glEnd         ();
   glColor4f     (0.0f, 0.2f, 0.0f, 0.5f);
   glBegin       (GL_LINE_STRIP); {
      glVertex3f ( px,   8.00f,  pz);
      glVertex3f ( px, -12.00f,  pz);
      glVertex3f ( nx, -12.00f,  nz);
      glVertex3f ( nx,   8.00f,  nz);
      glVertex3f ( px,   8.00f,  pz);
   } glEnd         ();
}

static int   /* ---- : create a saved shape for the body -----------------------*/
dlist_body         (void)
{
   /*---(begin)-----------------------------*/
   float      ri, ro;
   float      nxi, nzi, nxo, nzo;
   float      pxi, pzi, pxo, pzo;
   int        d;
   float      r;
   ro = segs_len [YKINE_THOR];
   ri = ro - 20.0;
   dl_body = glGenLists(1);
   glNewList(dl_body, GL_COMPILE); {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      /*> glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  <*/
      /*---(draw)--------------------------------------------*/
      glLineWidth   (2.0);
      glColor4f    (0.5f, 0.0f, 0.0f, 0.5f);
      glPushMatrix  (); {
         glBegin(GL_POLYGON); {     /*->> size is 1" wide by 13/8" out (1.62")      */
            glColor3f(1.0f, 1.0f, 0.0f);
            for (d = 0; d <= 360; d +=  5) {
               /*---(calc)------------------------*/
               r    = d  * DEG2RAD;
               nxi  = ri * cos (r);
               nzi  = ri * sin (r);
               nxo  = ro * cos (r);
               nzo  = ro * sin (r);
               /*---(draw)------------------------*/
               if (d != 0) {
                  if (d <= 90 - 20 || d >  90 + 20) {
                     dlist_poly (d, pxi, pzi, nxi, nzi);
                     dlist_poly (d, pxo, pzo, nxo, nzo);
                  }
                  if (d == 90 - 20 || d == 90 + 20) {
                     dlist_poly (d, nxi, nzi, nxo, nzo);
                  }
               }
               /*---(prepare for next)------------*/
               pxi = nxi;
               pzi = nzi;
               pxo = nxo;
               pzo = nzo;
               /*---(done)------------------------*/
            }
         } glEnd();
      } glPopMatrix   ();
      glColor4f    (0.8f, 0.8f, 0.0f, 0.5f);
      glBegin(GL_LINE_STRIP); {
         glVertex3f ( 0.0f * 25, 0.0f * 25,-1.0f * 25);
         glVertex3f ( 1.0f * 25, 0.0f * 25, 0.0f * 25);
         glVertex3f ( 0.5f * 25, 0.0f * 25, 0.0f * 25);
         glVertex3f ( 0.5f * 25, 0.0f * 25, 1.0f * 25);
         glVertex3f (-0.5f * 25, 0.0f * 25, 1.0f * 25);
         glVertex3f (-0.5f * 25, 0.0f * 25, 0.0f * 25);
         glVertex3f (-1.0f * 25, 0.0f * 25, 0.0f * 25);
         glVertex3f ( 0.0f * 25, 0.0f * 25,-1.0f * 25);
      } glEnd();
      /*---(end)-------------------------------*/
   } glEndList();
   return 0;
}

static int   /* ---- : create a saved shape for the beak -----------------------*/
dlist_beak         (void)
{
   /*---(begin)-----------------------------*/
   float      x_radius    = 50.0;
   int      deg;
   float    rad;
   float     nx, nz;
   float     px, pz;
   dl_beak = glGenLists(1);
   glNewList(dl_beak, GL_COMPILE); {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      /*> glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  <*/
      /*---(draw)--------------------------------------------*/
      glColor4f    (0.5f, 0.0f, 0.0f, 0.5f);
      glPushMatrix  (); {
         glBegin(GL_POLYGON); {     /*->> size is 1" wide by 13/8" out (1.62")      */
            glColor3f(1.0f, 1.0f, 0.0f);
            for (deg = 0; deg < 365; deg += 45) {
               /*---(calc)------------------------*/
               rad = deg * DEG2RAD;
               nx  = x_radius * cos(rad);
               nz  = x_radius * sin(rad);
               /*---(color)-----------------------*/
               if (deg % 90 == 0) glColor4f (1.0f, 0.0f, 0.0f, 0.5f);
               else               glColor4f (5.0f, 0.5f, 0.0f, 0.5f);
               /*---(draw)------------------------*/
               if (deg != 0) {
                  glBegin       (GL_POLYGON); {
                     glVertex3f( px, -12.00f,  pz);
                     glVertex3f(  0, -60.00f,  0 );
                     glVertex3f( nx, -12.00f,  nz);
                     glVertex3f( px, -12.00f,  pz);
                  } glEnd         ();
                  glColor4f (0.0f, 0.2f, 0.0f, 0.5f);
                  glLineWidth  (2.0);
                  glBegin    (GL_LINE_STRIP); {
                     glVertex3f( px, -12.00f,  pz);
                     glVertex3f(  0, -60.00f,  0 );
                     glVertex3f( nx, -12.00f,  nz);
                     glVertex3f( px, -12.00f,  pz);
                  } glEnd         ();
               }
               /*---(prepare)---------------------*/
               px = nx;
               pz = nz;
               /*---(done)------------------------*/
               /*> glVertex3f( nx,   8.00f, nz);                                      <* 
                *> glVertex3f( nx, -12.00f, nz);                                      <*/
            }
         } glEnd();
      } glPopMatrix   ();
      /*---(end)-------------------------------*/
   } glEndList();
   return 0;
}

static int   /* ---- : create a saved shape for the body -----------------------*/
dlist_foot         (void)
{
   /*---(begin)-----------------------------*/
   float      x_radius    = 0.0;
   int      deg;
   float    rad;
   float     nx, nz;
   float     px, pz;
   dl_foot = glGenLists(1);
   glNewList(dl_foot, GL_COMPILE); {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      /*---(draw)--------------------------------------------*/
      x_radius = segs_len [YKINE_FOOT];
      glColor4f    (0.5f, 0.0f, 0.0f, 0.5f);
      glPushMatrix  (); {
         glBegin(GL_POLYGON); {
            glColor3f(1.0f, 1.0f, 0.0f);
            for (deg = 0; deg < 365; deg += 15) {
               /*---(calc)------------------------*/
               rad = deg * DEG2RAD;
               nx  = x_radius * cos(rad);
               nz  = x_radius * sin(rad);
               /*---(color)-----------------------*/
               if      (deg == 15    ) glColor4f (1.0f, 1.0f, 0.0f, 0.5f);
               else if (deg == 360   ) glColor4f (1.0f, 1.0f, 0.0f, 0.5f);
               else if (deg % 10 == 0) glColor4f (0.0f, 1.0f, 0.0f, 0.5f);
               else                    glColor4f (0.0f, 0.5f, 0.0f, 0.5f);
               /*---(draw)------------------------*/
               if (deg != 0) {
                  glBegin       (GL_POLYGON); {
                     glVertex3f( px,  pz,   5.00f);
                     glVertex3f( px,  pz,  -5.00f);
                     glVertex3f( nx,  nz,  -5.00f);
                     glVertex3f( nx,  nz,   5.00f);
                  } glEnd         ();
                  glColor4f (0.0f, 0.2f, 0.0f, 0.5f);
                  glLineWidth  (2.0);
                  glBegin    (GL_LINE_STRIP); {
                     glVertex3f( px,  pz,   5.00f);
                     glVertex3f( px,  pz,  -5.00f);
                     glVertex3f( nx,  nz,  -5.00f);
                     glVertex3f( nx,  nz,   5.00f);
                     glVertex3f( px,  pz,   5.00f);
                  } glEnd         ();
               }
               /*---(prepare)---------------------*/
               px = nx;
               pz = nz;
               /*---(done)------------------------*/
               /*> glVertex3f( nx,   8.00f, nz);                                      <* 
                *> glVertex3f( nx, -12.00f, nz);                                      <*/
            }
         } glEnd();
      } glPopMatrix   ();
      /*---(lengthwise alignment line)------*/
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glColor3f(0.8f, 0.3f, 0.0f);
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0xA0A0);
      glLineWidth (5.0);
      glBegin(GL_LINES); {
         glVertex3f( -10.00f,  0.00f,  0.00f);
         glVertex3f( segs_len [YKINE_FOOT] + 10.0 ,  0.00f,  0.00f);
      } glEnd();
      glLineWidth (0.5);
      glDisable(GL_LINE_STIPPLE);
      /*---(prepare for next segment)-------*/
      glTranslatef( segs_len [YKINE_FOOT], 0.00f,  0.00f);
      /*---(end)-------------------------------*/
   } glEndList();
   return 0;
}






/*============================----end-of-source---============================*/
