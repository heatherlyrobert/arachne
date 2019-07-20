/*============================----beg-of-source---============================*/


#include "arachne.h"

static   int       dlist_ground       (void);
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
   glDeleteLists (dl_body    , 1);
   glDeleteLists (dl_beak    , 1);
   glDeleteLists (dl_coxa    , 1);
   glDeleteLists (dl_femur   , 1);
   glDeleteLists (dl_patella , 1);
   glDeleteLists (dl_tibia   , 1);
   glDeleteLists (dl_foot    , 1);
   return 0;
}


static int
dlist_ground()
{
   dl_ground = glGenLists(1);
   glNewList(dl_ground, GL_COMPILE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   /*---(begin)-----------------------------*/
   int      x_pos, z_pos;
   float    x, z;
   int      deg;
   float    rad;
   /*---(draw)--------------------------------------------*/
   glColor4f (0.2f, 0.2f, 0.2f, 0.3f);
   for (z_pos = -1000; z_pos < 1000; z_pos += 50) {
      glBegin(GL_QUAD_STRIP);
      for (x_pos = -1000; x_pos < 1000; x_pos += 50) {
         glVertex3f( x_pos, 0.0f, z_pos);
         glVertex3f( x_pos, 0.0f, z_pos + 25);
      }
      glEnd();
      glBegin(GL_QUAD_STRIP);
      for (x_pos = -1000; x_pos < 1000; x_pos += 50) {
         glVertex3f( x_pos + 25, 0.0f, z_pos);
         glVertex3f( x_pos + 25, 0.0f, z_pos - 25);
      }
      glEnd();
   }
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (    0.00f,   20.00f, -950.00f);
      glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed, 150, YF_BASCEN, "n");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (    0.00f,   20.00f,  950.00f);
      glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed, 150, YF_BASCEN, "s");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (  950.00f,   20.00f,    0.00f);
      glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed, 150, YF_BASCEN, "e");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef ( -950.00f,   20.00f,    0.00f);
      glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed, 150, YF_BASCEN, "w");
   } glPopMatrix();
   /*---(center)----------------------------*/
   /*> glLineWidth ( 5.0);                                                            <* 
    *> glBegin   (GL_LINE_STRIP);                                                     <* 
    *> glColor4f (0.5f, 0.0f, 1.0f, 0.5f);                                            <* 
    *> for (deg = 0; deg < 365; deg += 15) {                                          <* 
    *>    rad = deg * DEG2RAD;                                                        <* 
    *>    x   = (segs_len [YKINE_THOR] / 5) * cos (rad);                                    <* 
    *>    z   = (segs_len [YKINE_THOR] / 5) * sin (rad);                                    <* 
    *>    glVertex3f ( x,  10.00f, z);                                                <* 
    *> }                                                                              <* 
    *> glEnd     ();                                                                  <* 
    *> glLineWidth ( 0.5);                                                            <*/
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

static int   /* ---- : create a saved shape for the body -----------------------*/
dlist_body         (void)
{
   /*---(begin)-----------------------------*/
   float      x_radius    = 0.0;
   int      deg;
   float    rad;
   float     nx, nz;
   float     px, pz;
   dl_body = glGenLists(1);
   glNewList(dl_body, GL_COMPILE); {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      /*> glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  <*/
      /*---(draw)--------------------------------------------*/
      x_radius = segs_len [YKINE_THOR];
      glColor4f    (0.5f, 0.0f, 0.0f, 0.5f);
      glPushMatrix  (); {
         glBegin(GL_POLYGON); {     /*->> size is 1" wide by 13/8" out (1.62")      */
            glColor3f(1.0f, 1.0f, 0.0f);
            for (deg = 0; deg < 365; deg +=  5) {
               /*---(calc)------------------------*/
               rad = deg * DEG2RAD;
               nx  = x_radius * cos(rad);
               nz  = x_radius * sin(rad);
               /*---(color)-----------------------*/
               if (deg % 10 == 0) glColor4f (0.0f, 1.0f, 0.0f, 0.5f);
               else               glColor4f (0.0f, 0.5f, 0.0f, 0.5f);
               /*---(draw)------------------------*/
               if (deg != 0) {
                  glBegin       (GL_POLYGON); {
                     glVertex3f( px,   8.00f,  pz);
                     glVertex3f( px, -12.00f,  pz);
                     glVertex3f( nx, -12.00f,  nz);
                     glVertex3f( nx,   8.00f,  nz);
                  } glEnd         ();
                  glColor4f (0.0f, 0.2f, 0.0f, 0.5f);
                  glLineWidth  (2.0);
                  glBegin    (GL_LINE_STRIP); {
                     glVertex3f( px,   8.00f,  pz);
                     glVertex3f( px, -12.00f,  pz);
                     glVertex3f( nx, -12.00f,  nz);
                     glVertex3f( nx,   8.00f,  nz);
                     glVertex3f( px,   8.00f,  pz);
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
      x_radius -= 20.0;
      glPushMatrix  (); {
         glBegin(GL_POLYGON); {     /*->> size is 1" wide by 13/8" out (1.62")      */
            glColor3f(1.0f, 1.0f, 0.0f);
            for (deg = 0; deg < 365; deg +=  5) {
               /*---(calc)------------------------*/
               rad = deg * DEG2RAD;
               nx  = x_radius * cos(rad);
               nz  = x_radius * sin(rad);
               /*---(color)-----------------------*/
               if (deg % 10 == 0) glColor4f (0.0f, 1.0f, 0.0f, 0.5f);
               else               glColor4f (0.0f, 0.5f, 0.0f, 0.5f);
               /*---(draw)------------------------*/
               if (deg != 0) {
                  glBegin       (GL_POLYGON); {
                     glVertex3f( px,   8.00f,  pz);
                     glVertex3f( px, -12.00f,  pz);
                     glVertex3f( nx, -12.00f,  nz);
                     glVertex3f( nx,   8.00f,  nz);
                  } glEnd         ();
                  glColor4f (0.0f, 0.2f, 0.0f, 0.5f);
                  glLineWidth  (2.0);
                  glBegin    (GL_LINE_STRIP); {
                     glVertex3f( px,   8.00f,  pz);
                     glVertex3f( px, -12.00f,  pz);
                     glVertex3f( nx, -12.00f,  nz);
                     glVertex3f( nx,   8.00f,  nz);
                     glVertex3f( px,   8.00f,  pz);
                  } glEnd         ();
               }
               /*---(prepare)---------------------*/
               px = nx;
               pz = nz;
               /*---(done)------------------------*/
            }
         } glEnd();
      } glPopMatrix   ();
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
