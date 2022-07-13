/*============================----beg-of-source---============================*/


#include "arachne.h"

static   int       dlist_ground       (void);
static   int       dlist_snowshoe     (void);
static   int       dlist_ruler        (void);
static   int       dlist_body         (void);
static   int       dlist_beak         (void);
static   int       dlist_coxa         (void);
static   int       dlist_femur        (void);
static   int       dlist_patella      (void);
static   int       dlist_tibia        (void);
static   int       dlist_foot         (void);
static   int       dlist_verify       (void);



char       /* ---- : prepare display lists for use ---------------------------*/
dlist_begin        (void)
{
   dlist_ground   ();
   dlist_snowshoe ();
   dlist_ruler    ();
   dlist_body     ();
   dlist_beak     ();
   dlist_coxa     ();
   dlist_femur    ();
   dlist_patella  ();
   dlist_tibia    ();
   dlist_foot     ();
   dlist_verify   ();
   return 0;
}

char       /* ---- : prepare display lists for use ---------------------------*/
dlist_end          (void)
{
   glDeleteLists (dl_ground  , 1);
   glDeleteLists (dl_snowshoe, 1);
   glDeleteLists (dl_ruler   , 1);
   glDeleteLists (dl_body    , 1);
   glDeleteLists (dl_beak    , 1);
   glDeleteLists (dl_coxa    , 1);
   glDeleteLists (dl_femur   , 1);
   glDeleteLists (dl_patella , 1);
   glDeleteLists (dl_tibia   , 1);
   glDeleteLists (dl_foot    , 1);
   glDeleteLists (dl_verify  , 1);
   return 0;
}


static float    s_side    = 25.4;   /*  one inch in millimeters               */
static float    s_scale   = 25.4;
static float    s_xoff    =  0.0;
static float    s_zoff    =  0.0;
static float    s_xinc    =  0.0;
static float    s_zinc    =  0.0;

static float    y         =  0.0;



/*====================------------------------------------====================*/
/*===----                      hexagonal grid                          ----===*/
/*====================------------------------------------====================*/
static void      o___HEXGRID_________________o (void) {;}

char
dlist_hex          (int l, char t, float x, float z, float y)
{
   char        s           [LEN_LABEL];
   /* exterior hexagon */
   if (l > 0 && l < 5)  return 0;
   switch (t) {
      /*---(mapping)---------------------*/
   case HEX_CENTER : glColor4f (0.8f, 0.4f, 0.2f, 1.0f);  break;
   case HEX_INNER  : glColor4f (0.2f, 0.2f, 0.2f, 0.3f);  break;
   case HEX_OUTER  : glColor4f (0.2f, 0.2f, 0.2f, 0.1f);  break;
   case HEX_BORDER : glColor4f (0.2f, 0.2f, 0.2f, 0.1f);  break;
                     /*---(footprints)------------------*/
   case HEX_ABOVE  : glColor4f (0.5f, 0.5f, 0.5f, 1.0f);  break;
   case HEX_OVER   : glColor4f (0.7f, 0.7f, 0.0f, 1.0f);  break;
   case HEX_TOUCH  : glColor4f (0.0f, 0.7f, 0.0f, 1.0f);  break;
   case HEX_UNDER  : glColor4f (0.7f, 0.1f, 0.1f, 1.0f);  break;
   case HEX_BELOW  : glColor4f (0.0f, 0.0f, 0.0f, 1.0f);  break;
   default  : glColor4f (0.8f, 0.8f, 0.8f, 0.1f);  break;
   }
   if (strchr (HEX_FILLED, t) != NULL)  glBegin       (GL_POLYGON);
   else                                 glBegin       (GL_LINE_STRIP);
   {
      glVertex3f (x - s_xoff - 0.5 * s_side, y, z         );  /* east       */
      glVertex3f (x          - 0.5 * s_side, y, z - s_zoff);  /* east-north */
      glVertex3f (x          + 0.5 * s_side, y, z - s_zoff);  /* west-north */
      glVertex3f (x + s_xoff + 0.5 * s_side, y, z         );  /* west       */
      glVertex3f (x          + 0.5 * s_side, y, z + s_zoff);  /* west-south */
      glVertex3f (x          - 0.5 * s_side, y, z + s_zoff);  /* east-south */
      glVertex3f (x - s_xoff - 0.5 * s_side, y, z         );  /* east       */
   } glEnd         ();
   /* interior triangles */
   if (strchr (HEX_HATCHED, t) != NULL) {
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
   return 0;
}

static char
dlist__level       (int n, int l, char t, char b, float x, float z)
{
   char        c           = HEX_BORDER;
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
   else if (l <=  9)   c = HEX_INNER;
   else if (l <= 15)   c = HEX_OUTER;
   /*---(drawa)--------------------------*/
   dlist_hex (l, c, x, z, 0.0);
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



/*====================------------------------------------====================*/
/*===----                       footprint related                      ----===*/
/*====================------------------------------------====================*/
static void      o___FOOTPRINTS______________o (void) {;}

#define    MM2COL      25.400
#define    MM2ROW      29.328

char
dlist_distances         (float x, float z, float y)
{
   float       x_fd        [10];
   int         i           =    0;
   char        t           [LEN_LABEL];
   yKINE_neighbors (&x_fd [0], &x_fd [1], &x_fd [2], &x_fd [3], &x_fd [4], &x_fd [5], &x_fd [6]);
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   glPushMatrix    (); {
      glTranslatef (x, 1 + y, z);
      for (i = 0; i <= 6; ++i) {
         switch (i) {
         case 1 : glTranslatef (    0.0, 0.0,  MM2ROW);          break;
         case 2 : glTranslatef ( MM2COL, 0.0, -MM2ROW / 2.0);    break;
         case 3 : glTranslatef (    0.0, 0.0, -MM2ROW      );    break;
         case 4 : glTranslatef (-MM2COL, 0.0, -MM2ROW / 2.0);    break;
         case 5 : glTranslatef (-MM2COL, 0.0,  MM2ROW / 2.0);    break;
         case 6 : glTranslatef (    0.0, 0.0,  MM2ROW      );    break;
         }
         sprintf (t, "%4.2f", x_fd [i]);
         glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
         yFONT_print  (my.fixed,  3 , YF_MIDCEN, t);
         glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);
      }
   } glPopMatrix();
   return 0;
}

char
dlist_littlehex    (float x, float z, float y)
{
   /*> float       y           =  3.0;                                                <*/
   float       x_xoff = s_xoff / 4.0;
   float       x_zoff = s_zoff / 4.0;
   float       x_side = s_side / 4.0;
   glColor4f (0.4f, 0.4f, 0.4f, 1.0f);
   glBegin       (GL_POLYGON); {
      glVertex3f (x - x_xoff - 0.5 * x_side, y, z         );  /* east       */
      glVertex3f (x          - 0.5 * x_side, y, z - x_zoff);  /* east-north */
      glVertex3f (x          + 0.5 * x_side, y, z - x_zoff);  /* west-north */
      glVertex3f (x + x_xoff + 0.5 * x_side, y, z         );  /* west       */
      glVertex3f (x          + 0.5 * x_side, y, z + x_zoff);  /* west-south */
      glVertex3f (x          - 0.5 * x_side, y, z + x_zoff);  /* east-south */
      glVertex3f (x - x_xoff - 0.5 * x_side, y, z         );  /* east       */
   } glEnd         ();
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   glBegin       (GL_LINE_STRIP); {
      glVertex3f (x - x_xoff - 0.5 * x_side, y, z         );  /* east       */
      glVertex3f (x          - 0.5 * x_side, y, z - x_zoff);  /* east-north */
      glVertex3f (x          + 0.5 * x_side, y, z - x_zoff);  /* west-north */
      glVertex3f (x + x_xoff + 0.5 * x_side, y, z         );  /* west       */
      glVertex3f (x          + 0.5 * x_side, y, z + x_zoff);  /* west-south */
      glVertex3f (x          - 0.5 * x_side, y, z + x_zoff);  /* east-south */
      glVertex3f (x - x_xoff - 0.5 * x_side, y, z         );  /* east       */
   } glEnd         ();
   x_xoff /= 4.0;
   x_zoff /= 4.0;
   x_side /= 4.0;
   y      += 0.25;
   glBegin       (GL_POLYGON); {
      glVertex3f (x - x_xoff - 0.5 * x_side, y, z         );  /* east       */
      glVertex3f (x          - 0.5 * x_side, y, z - x_zoff);  /* east-north */
      glVertex3f (x          + 0.5 * x_side, y, z - x_zoff);  /* west-north */
      glVertex3f (x + x_xoff + 0.5 * x_side, y, z         );  /* west       */
      glVertex3f (x          + 0.5 * x_side, y, z + x_zoff);  /* west-south */
      glVertex3f (x          - 0.5 * x_side, y, z + x_zoff);  /* east-south */
      glVertex3f (x - x_xoff - 0.5 * x_side, y, z         );  /* east       */
   } glEnd         ();
   return 0;
}

char
dlist_footprint    (int l, char t, float x, float z, float y, char rc, int c, int r, float d, float o)
{
   char        s           [LEN_LABEL];
   dlist_hex (0, t, x, z, y);
   /*> glPointSize (15.0);                                                            <*/
   glPushMatrix    (); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      /*> glBegin(GL_POINT); {                                                        <* 
       *>    glVertex3f (x, 0.0, z);                                                  <* 
       *> } glEnd ();                                                                 <*/
      /*---(rc)------*/
      glTranslatef (    x, 1.00f + y, z - 13.0);
      sprintf (s, "%2drc", rc);
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed,  3 , YF_BASCEN, s);
      glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);
      /*---(hex)-----*/
      glTranslatef ( 0.00, 0.00,  5.0);
      sprintf (s, "%dc/%dr", c, r);
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed,  3 , YF_BASCEN, s);
      glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);
      /*---(dir)-----*/
      /*> glTranslatef ( 0.00, 0.00,  5.0);                                           <* 
       *> sprintf (s, "%5.1fd", d);                                                   <* 
       *> glRotatef (-90.0, 1.0f, 0.0f, 0.0f);                                        <* 
       *> yFONT_print  (my.fixed,  3 , YF_BASCEN, s);                                 <* 
       *> glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);                                        <*/
      /*---(dist)----*/
      /*> glTranslatef ( 0.00, 0.00,  5.0);                                           <* 
       *> sprintf (s, "%5.3fo", o);                                                   <* 
       *> glRotatef (-90.0, 1.0f, 0.0f, 0.0f);                                        <* 
       *> yFONT_print  (my.fixed,  3 , YF_BASCEN, s);                                 <* 
       *> glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);                                        <*/
      /*---(c/x)-----*/
      glTranslatef ( 0.00, 0.00, 13.0);
      sprintf (s, "%.1fx", x);
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed,  3 , YF_BASCEN, s);
      glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);
      /*---(r/z)-----*/
      glTranslatef ( 0.00, 0.00,  5.0);
      sprintf (s, "%.1fz", z);
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      yFONT_print  (my.fixed,  3 , YF_BASCEN, s);
      glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);
      /*---(r/z)-----*/
      /*> glTranslatef ( 0.00, 0.00,  5.0);                                           <* 
       *> sprintf (s, "%6.1fy", y);                                                   <* 
       *> glRotatef (-90.0, 1.0f, 0.0f, 0.0f);                                        <* 
       *> yFONT_print  (my.fixed,  3 , YF_BASCEN, s);                                 <* 
       *> glRotatef ( 90.0, 1.0f, 0.0f, 0.0f);                                        <*/
      /*---(done)----*/
   } glPopMatrix();
   return 0;
}

static char
dlist__compass    (void)
{
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (    0.00f,   0.00f, -750.00f);
      glRotatef (-90.0, 1.0f, 0.0f, 0.0f);
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
      yFONT_print  (my.fixed, 45 , YF_BASCEN, "n");
   } glPopMatrix();
   glPushMatrix    (); {
      glColor4f (1.0, 1.0, 0.0, 1.0);
      glTranslatef (    0.00f,   0.00f,  750.00f);
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
   s_side      =  (25.4 / 3.0) * 2.0;
   s_scale     =  25.4;
   s_xoff      = s_side * cos (60.0 * DEG2RAD);
   s_zoff      = s_side * sin (60.0 * DEG2RAD);
   s_xinc      = s_side + s_xoff;
   s_zinc      = s_zoff;
   /*> printf ("%8.2fs, %8.2fxo, %8.2fxi, %8.2fzo, %8.2fzi\n", s_side, s_xoff, s_xinc, s_zoff, s_zinc);   <*/
   dlist__level (20, 0, 0, '-', 0, 0);
   dlist__compass ();
   /*---(end)-------------------------------*/
   glEndList();
   return 0;
}

static int
dlist__diamond          (float x, float y, char *a_text, char a_dir)
{
   char        t           [LEN_LABEL];
   glBegin(GL_LINE_STRIP); {
      glVertex3f (x - 1.0,  y      ,  0.0);
      glVertex3f (x      ,  y + 1.0,  0.0);
      glVertex3f (x + 1.0,  y      ,  0.0);
      glVertex3f (x      ,  y - 1.0,  0.0);
      glVertex3f (x - 1.0,  y      ,  0.0);
   } glEnd();
   glBegin(GL_LINE_STRIP); {
      glVertex3f (x - 1.0,  y      ,  0.0);
      glVertex3f (x      ,  y      ,  1.0);
      glVertex3f (x + 1.0,  y      ,  0.0);
      glVertex3f (x      ,  y      , -1.0);
      glVertex3f (x - 1.0,  y      ,  0.0);
   } glEnd();
   glBegin(GL_LINE_STRIP); {
      glVertex3f (x      ,  y      ,  1.0);
      glVertex3f (x      ,  y + 1.0,  0.0);
      glVertex3f (x      ,  y      , -1.0);
      glVertex3f (x      ,  y - 1.0,  0.0);
      glVertex3f (x      ,  y      ,  1.0);
   } glEnd();
   if (a_text == NULL) {
      return 0;
   }
   if (a_dir == 'a') {
      glPushMatrix    (); {
         glTranslatef( x, 10.00f, 0.00f);
         yFONT_print  (my.fixed,  6 , YF_BASCEN, a_text);
      } glPopMatrix();
      if (a_text [0] < 'z') {
         glBegin(GL_LINES); {
            glVertex3f (x + 25.4 / 4.0,  y + 1.0        ,  0.0);
            glVertex3f (x + 25.4 / 4.0,  y - 1.0        ,  0.0);
            glVertex3f (x + 25.4 / 4.0,  y              ,  1.0);
            glVertex3f (x + 25.4 / 4.0,  y              , -1.0);
         } glEnd();
      }
      glPushMatrix    (); {
         glTranslatef( x, -4.00f, -0.00f);
         glRotatef (-90.0, 0.0f, 0.0f, 1.0f);
         sprintf (t, "%5.1f", x);
         yFONT_print  (my.fixed,  3 , YF_MIDLEF, t);
      } glPopMatrix();
   } else if (a_dir == 'r') {
      glPushMatrix    (); {
         glTranslatef( 2.00 , y,  0.00f);
         yFONT_print  (my.fixed,  6 , YF_MIDLEF, a_text);
      } glPopMatrix();
      if (a_text [0] < 'z') {
         glBegin(GL_LINES); {
            glVertex3f (x + 1.0, y + 25.4 / 4.0,  0.0);
            glVertex3f (x - 1.0, y + 25.4 / 4.0,  0.0);
            glVertex3f (x      , y + 25.4 / 4.0,  1.0);
            glVertex3f (x      , y + 25.4 / 4.0, -1.0);
         } glEnd();
      }
      glPushMatrix    (); {
         glTranslatef(-2.00, y, -0.00f);
         sprintf (t, "%5.1f", y);
         yFONT_print  (my.fixed,  3 , YF_MIDRIG, t);
      } glPopMatrix();
   }
   ++a_text [0];
   return 0;
}

static int
dlist_ruler_one         (float a_deg)
{
   float       i           =  0.0;
   float       j           =  0.0;
   char        n           [LEN_LABEL] = "a";
   glPushMatrix    (); {
      glRotatef (a_deg, 0.0f, 1.0f, 0.0f);
      /*> glColor4f (0.3, 0.3, 0.3, 0.3);                                             <*/
      glBegin(GL_LINES); {
         glVertex3f ( 0.0 * s_scale,  1.00f,  0.00f);
         glVertex3f ( 3.0 * s_scale,  1.00f,  0.00f);
      } glEnd();
      n [0] = 'a';
      for (j = 0.0 * s_scale; j <  3.0 * s_scale; j += s_scale * 0.50) {
         dlist__diamond (j, 1.0, n, '-');
      }
      /*> glColor4f (0.0, 0.8, 0.0, 1.0);                                             <*/
      glBegin(GL_LINES); {
         glVertex3f ( 3.0 * s_scale,  1.00f,  0.00f);
         glVertex3f (15.5 * s_scale,  1.00f,  0.00f);
      } glEnd();
      n [0] = 'a';
      for (j = 3.0 * s_scale; j < 16.0 * s_scale; j += s_scale * 0.50) {
         dlist__diamond (j, 1.0, n, 'a');
      }
      /*> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <*/
   } glPopMatrix();
   return 0;
}

static int
dlist_snowshoe          (void)
{
   dl_snowshoe = glGenLists(1);
   glNewList(dl_snowshoe, GL_COMPILE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   /*---(leg radius)---------------------*/
   glColor4f (0.0, 0.3, 0.0, 1.0);
   dlist_ruler_one (0);
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
   /*---(leg radius)---------------------*/
   glColor4f (0.0, 0.0, 0.0, 1.0);
   for (i = 0; i < 360; i += 60) {
      dlist_ruler_one (i);
      /*> glPushMatrix    (); {                                                                    <* 
       *>    glRotatef (i, 0.0f, 1.0f, 0.0f);                                                      <* 
       *>    glColor4f (0.3, 0.3, 0.3, 0.3);                                                       <* 
       *>    glBegin(GL_LINES); {                                                                  <* 
       *>       glVertex3f ( 0.0 * s_scale,  1.00f,  0.00f);                                       <* 
       *>       glVertex3f ( 3.0 * s_scale,  1.00f,  0.00f);                                       <* 
       *>    } glEnd();                                                                            <* 
       *>    n [0] = 'a';                                                                          <* 
       *>    for (j = 0.0 * s_scale; j <  3.0 * s_scale; j += s_scale * 0.50) {                    <* 
       *>       dlist__diamond (j, 1.0, n, '-');                                                   <* 
       *>    }                                                                                     <* 
       *>    glColor4f (0.0, 0.0, 0.0, 1.0);                                                       <* 
       *>    glBegin(GL_LINES); {                                                                  <* 
       *>       glVertex3f ( 3.0 * s_scale,  1.00f,  0.00f);                                       <* 
       *>       glVertex3f (15.5 * s_scale,  1.00f,  0.00f);                                       <* 
       *>    } glEnd();                                                                            <* 
       *>    n [0] = 'a';                                                                          <* 
       *>    for (j = 3.0 * s_scale; j < 16.0 * s_scale; j += s_scale * 0.50) {                    <* 
       *>       dlist__diamond (j, 1.0, n, 'a');                                                   <* 
       *>    }                                                                                     <* 
       *>    /+> glRotatef (180.0, 0.0f, 0.0f, 1.0f);                                        <+/   <* 
       *> } glPopMatrix();                                                                         <*/
   }
   /*---(body height)--------------------*/
   glBegin(GL_LINES); {
      glVertex3f (0.0, -4.5 * s_scale, 0.0);
      glVertex3f (0.0,  8.0 * s_scale, 0.0);
   } glEnd();
   n [0] = 'a';
   for (j = -4.5 * s_scale; j <  8.0 * s_scale; j += s_scale * 0.50) {
      dlist__diamond (0.0, j, n, 'r');
   }
   /*---(leg depth)----------------------*/
   /*> for (i = 0; i < 360; i += 60) {                                                <* 
    *>    glPushMatrix    (); {                                                       <* 
    *>       glColor4f (0.3, 0.3, 0.3, 0.5);                                          <* 
    *>       glRotatef (i, 0.0f, 1.0f, 0.0f);                                         <* 
    *>       glTranslatef (228.1, 139.700f,  0.00f);                                  <* 
    *>       glBegin(GL_LINES); {                                                     <* 
    *>          glVertex3f (0.0, -12.5 * s_scale, 0.0);                               <* 
    *>          glVertex3f (0.0,   0.0 * s_scale, 0.0);                               <* 
    *>       } glEnd();                                                               <* 
    *>       n [0] = 'a';                                                             <* 
    *>       for (j = 0.0; j > -13.0 * s_scale; j -= s_scale * 0.50) {                <* 
    *>          dlist__diamond (0.0, j, n, 'r');                                      <* 
    *>       }                                                                        <* 
    *>       glColor4f (0.5, 0.1, 0.1, 0.5);                                          <* 
    *>       glBegin(GL_LINES); {                                                     <* 
    *>          glVertex3f (0.0,   0.0 * s_scale, 0.0);                               <* 
    *>          glVertex3f (0.0,  12.5 * s_scale, 0.0);                               <* 
    *>       } glEnd();                                                               <* 
    *>       n [0] = 'b';                                                             <* 
    *>       for (j = 0.5 * s_scale; j <  13.0 * s_scale; j += s_scale * 0.50) {      <* 
    *>          dlist__diamond (0.0, j, n, 'r');                                      <* 
    *>       }                                                                        <* 
    *>    } glPopMatrix();                                                            <* 
    *> }                                                                              <*/
   /*---(x-axis)-------------------------*/
   for (i = -20; i <= 20; ++i) {
      glColor4f (0.8, 0.0, 0.0, 1.0);
      glBegin(GL_POLYGON); {
         glVertex3f ((i + 0.30) * s_scale,  1.00, -3.00);
         glVertex3f ((i + 0.00) * s_scale,  1.00,  3.00);
         glVertex3f ((i + 0.70) * s_scale,  1.00,  3.00);
         glVertex3f ((i + 1.00) * s_scale,  1.00, -3.00);
      } glEnd();
      glColor4f (0.3, 0.0, 0.0, 1.0);
      glBegin(GL_POLYGON); {
         glVertex3f ((i + 1.00) * s_scale,  1.00, -3.00);
         glVertex3f ((i + 0.70) * s_scale,  1.00,  3.00);
         glVertex3f ((i + 1.00) * s_scale,  1.00,  3.00);
         glVertex3f ((i + 1.30) * s_scale,  1.00, -3.00);
      } glEnd();
   }
   /*---(z-axis)-------------------------*/
   for (i = -20; i <= 20; ++i) {
      glColor4f (0.8, 0.8, 0.0, 1.0);
      glBegin(GL_POLYGON); {
         glVertex3f (-3.00,  1.00, (i + 0.30) * s_scale);
         glVertex3f ( 3.00,  1.00, (i + 0.00) * s_scale);
         glVertex3f ( 3.00,  1.00, (i + 0.70) * s_scale);
         glVertex3f (-3.00,  1.00, (i + 1.00) * s_scale);
      } glEnd();
      glColor4f (0.3, 0.3, 0.0, 1.0);
      glBegin(GL_POLYGON); {
         glVertex3f (-3.00,  1.00, (i + 1.00) * s_scale);
         glVertex3f ( 3.00,  1.00, (i + 0.70) * s_scale);
         glVertex3f ( 3.00,  1.00, (i + 1.00) * s_scale);
         glVertex3f (-3.00,  1.00, (i + 1.30) * s_scale);
      } glEnd();
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
      glVertex3f( segs_len [YKINE_TIBI] + (1.0 * 25.0) ,  0.00f,  0.00f);
   } glEnd();
   /*> glBegin(GL_LINES); {                                                           <* 
    *>    glVertex3f(  5.20f * 25.0, -0.50f * 25.0,  0.00f);                          <* 
    *>    glVertex3f(  5.20f * 25.0,  0.50f * 25.0,  0.00f);                          <* 
    *> } glEnd();                                                                     <*/
   /*> glBegin(GL_LINES); {                                                           <* 
    *>    glVertex3f(  5.20f * 25.0, 0.00f, -0.50f * 25.0);                           <* 
    *>    glVertex3f(  5.20f * 25.0, 0.00f,  0.50f * 25.0);                           <* 
    *> } glEnd();                                                                     <*/
   /*---(alignment line ends)------------*/
   glDisable(GL_LINE_STIPPLE);
   glLineWidth (5.0);
   glPushMatrix  (); {
      glTranslatef( -5.00f     * 25,  0.00f,  0.00f);
      dlist__diamond (0.0, 0.0, NULL, '-');
   } glPopMatrix   ();
   glPushMatrix  (); {
      glTranslatef( segs_len [YKINE_TIBI] + (1.0 * 25.0) ,  0.00f,  0.00f);
      dlist__diamond (0.0, 0.0, NULL, '-');
   } glPopMatrix   ();
   glLineWidth (0.5);
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
      /*---(insides)---------------------*/
      float z = 0.0f;
      for(z = -0.50 * 25; z <= 0.50 * 25; z += 0.99 * 25) {
         /*> glColor4f    (0.0f, 0.2f, 0.0f, 0.5f);                                   <*/
         glBegin(GL_POLYGON); {
            glVertex3f( 0.00f * 25,   0.00f * 25, z);
            glVertex3f( 0.50f * 25,   0.75f * 25, z);
            glVertex3f( 2.50f * 25,   0.75f * 25, z);
            glVertex3f( 2.75f * 25,   0.50f * 25, z);
            glVertex3f( 2.75f * 25,  -0.50f * 25, z);
            glVertex3f( 2.50f * 25,  -0.75f * 25, z);
            glVertex3f( 0.50f * 25,  -0.75f * 25, z);
         } glEnd();
         /*> glColor4f    (0.0f, 0.5f, 0.0f, 1.0f);                                   <*/
      }
      /*---(outsides)--------------------*/
      for(z = -0.50 * 25; z <= 0.50 * 25; z += 0.99 * 25) {
         glColor4f    (0.0f, 0.0f, 0.0f, 1.0f);
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
      /*---(insides)---------------------------*/
      for(z = -0.45 * 25; z <= 0.45 * 25; z += 0.89 * 25) {
         /*> glColor4f    (0.0f, 0.0f, 0.5f, 0.5f);                                   <*/
         glBegin(GL_POLYGON); {
            glVertex3f( 0.00f * 25,  0.00f * 25, z);
            glVertex3f( 0.50f * 25,  0.75f * 25, z);
            glVertex3f( 1.25f * 25,  0.75f * 25, z);
            glVertex3f( 1.50f * 25,  0.50f * 25, z);
            glVertex3f( 1.50f * 25, -0.50f * 25, z);
            glVertex3f( 1.25f * 25, -0.75f * 25, z);
            glVertex3f( 0.50f * 25, -0.75f * 25, z);
         } glEnd();
      }
      /*---(outsides)--------------------------*/
      for(z = -0.45 * 25; z <= 0.45 * 25; z += 0.89 * 25) {
         glColor4f    (0.0f, 0.0f, 0.0f, 1.0f);
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
      /*---(insides)---------------------------*/
      for(y = -12.00; y <=  8.00; y += 20.00) {
         /*> glColor4f    (0.0f, 0.5f, 0.0f, 0.5f);                                   <*/
         glBegin(GL_POLYGON); {     /*->> size is 1" wide by 13/8" out (1.62")      */
            glVertex3f( 0.00f * 25, y,  0.00f);
            glVertex3f( 0.00f * 25, y,  0.50f * 25);
            glVertex3f( 1.37f * 25, y,  0.50f * 25);
            glVertex3f( 1.62f * 25, y,  0.25f * 25);
            glVertex3f( 1.62f * 25, y, -0.25f * 25);
            glVertex3f( 1.37f * 25, y, -0.50f * 25);
            glVertex3f( 0.00f * 25, y, -0.50f * 25);
         } glEnd();
      }
      /*---(lines)-----------------------------*/
      for(y = -12.00; y <=  8.00; y += 20.00) {
         glColor4f    (0.0f, 0.0f, 0.0f, 1.0f);
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
   if (d % 10 == 0)     glColor4f (0.0f, 0.8f, 0.0f, 0.5f);
   else                 glColor4f (0.0f, 0.5f, 0.0f, 0.5f);
   glBegin       (GL_POLYGON); {
      glVertex3f ( px,   8.00f,  pz);
      glVertex3f ( px, -12.00f,  pz);
      glVertex3f ( nx, -12.00f,  nz);
      glVertex3f ( nx,   8.00f,  nz);
   } glEnd         ();
   /*> if (my.p_leg == 0)  glColor4f     (1.0f, 1.0f, 1.0f, 0.5f);                    <* 
    *> else                glColor4f     (0.0f, 0.2f, 0.0f, 0.5f);                    <*/
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
               /*> if (my.p_leg ==  2)  glColor4f (1.0f, 1.0f, 1.0f, 1.0f);           <* 
                *> else                 glColor4f (0.0f, 0.8f, 0.0f, 1.0f);           <*/
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
      /*> glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                  <* 
       *> glColor3f(0.8f, 0.3f, 0.0f);                                                <* 
       *> glEnable(GL_LINE_STIPPLE);                                                  <* 
       *> glLineStipple(1, 0xA0A0);                                                   <* 
       *> glLineWidth (5.0);                                                          <* 
       *> glBegin(GL_LINES); {                                                        <* 
       *>    glVertex3f( -10.00f,  0.00f,  0.00f);                                    <* 
       *>    glVertex3f( segs_len [YKINE_FOOT] + 10.0 ,  0.00f,  0.00f);              <* 
       *> } glEnd();                                                                  <* 
       *> glLineWidth (0.5);                                                          <* 
       *> glDisable(GL_LINE_STIPPLE);                                                 <*/
      /*---(prepare for next segment)-------*/
      glTranslatef( segs_len [YKINE_FOOT], 0.00f,  0.00f);
      /*---(end)-------------------------------*/
   } glEndList();
   return 0;
}

static int   /* ---- : create a saved shape for the body -----------------------*/
dlist_verify       (void)
{
   char        rc;
   uchar       x_val;
   int         x_row;
   int         x_col;
   char        o_str       [LEN_LABEL];
   char        y_str       [LEN_LABEL];
   float       x, y;
   /*---(begin)-----------------------------*/
   /*> printf ("DLIST_VERIFY ---beg-----------------------------------------\n");     <*/
   dl_verify = glGenLists(1);
   glNewList(dl_verify, GL_COMPILE); {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      rc = ykine_stance_scale_head (&x_row, y_str);
      while (rc >= 0) {
         y = yKINE_scale  (y_str);
         rc = ykine_stance_radius_head (&x_col, o_str);
         while (rc >= 0) {
            x = yKINE_radius (o_str) + 228.1;
            x_val = yKINE_verify_rc (x_row, x_col);
            /*> printf ("%10.10s (%8.2f)   %10.10s (%8.2f)  %c (%d)", y_str, y, o_str, x, x_val, x_val);   <*/
            if (x_val == 172) {
               /*> printf ("  SHOW\n");                                               <*/
               glColor4f (1.0f, 1.0f, 1.0f, 0.5f);
               glBegin       (GL_POLYGON); {
                  glVertex3f (x - 1.5, y + 1.5, -3.00f);
                  glVertex3f (x + 1.5, y + 1.5, -3.00f);
                  glVertex3f (x + 1.5, y - 1.5, -3.00f);
                  glVertex3f (x - 1.5, y - 1.5, -3.00f);
               } glEnd         ();
               glColor4f (0.0f, 0.0f, 0.0f, 0.5f);
               glBegin       (GL_LINE_STRIP); {
                  glVertex3f (x - 1.5, y + 1.5, -3.00f);
                  glVertex3f (x + 1.5, y + 1.5, -3.00f);
                  glVertex3f (x + 1.5, y - 1.5, -3.00f);
                  glVertex3f (x - 1.5, y - 1.5, -3.00f);
                  glVertex3f (x - 1.5, y + 1.5, -3.00f);
               } glEnd         ();
            } else {
               /*> printf ("\n");                                                     <*/
            }
            rc = ykine_stance_radius_next (&x_col, o_str);
         }
         if (y_str [1] == '·') {
            glPushMatrix    (); {
               glColor4f (0.0, 0.0, 0.0, 1.0);
               glTranslatef (380.0, y, 5.00f);
               yFONT_print  (my.fixed,  5 , YF_BASCEN, y_str);
            } glPopMatrix();
         }
         rc = ykine_stance_scale_next   (&x_row, y_str);
      }
      /*---(end)-------------------------------*/
   } glEndList();
   /*> printf ("DLIST_VERIFY ---end-----------------------------------------\n");     <*/
   return 0;
}






/*============================----end-of-source---============================*/
