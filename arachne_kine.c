/*============================----beg-of-source---============================*/

#include "arachne.h"




/*---[[ CONSTANTS ]]----------------------------------------------------------*/
char   *legscon[MAX_LEGS]  = { "L_RF", "L_RM", "L_RR", "L_LR", "L_LM", "L_LF"};



const    double   DEG2RAD   = M_PI / 180.0;
const    double   RAD2DEG   = 180.0 / M_PI;


tSEG      fk [MAX_LEGS] [MAX_SEGS];    /* opengl kinematics check   */
tSEG      gk [MAX_LEGS] [MAX_SEGS];    /* forward kinematics        */
tSEG      ik [MAX_LEGS] [MAX_SEGS];    /* inverse kinematics        */


char      umake_init    = 'n';
char      umake_leg     = 'n';




/*====================------------------------------------====================*/
/*===----                        initialization                        ----===*/
/*====================------------------------------------====================*/
static void      o___INIT____________________o (void) {;}

char       /*----: set all segments to defaults ------------------------------*/
kine_init          (void)
{
   /*---(locals)-------------------------*/
   int      x_leg = 0;              /* iterator         */
   int      x_seg = 0;              /* iterator         */
   /*---(set body)-----------------------*/
   kine_center       (0.0f, 0.0f);
   kine_height       (segs_len [YKINE_TIBI]);
   kine_pivot        (0.0f, 0.0f);
   kine_attitude     (0.0f, 0.0f, 0.0f);
   /*---(clean legs)---------------------*/
   for (x_leg = 0; x_leg < MAX_LEGS; ++x_leg) {
      for (x_seg = 0; x_seg < MAX_SEGS; ++x_seg) {
         kine_clear ("gk", &gk[x_leg][x_seg], x_leg, x_seg);
         kine_clear ("fk", &fk[x_leg][x_seg], x_leg, x_seg);
         kine_clear ("ik", &ik[x_leg][x_seg], x_leg, x_seg);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: set segment kimematics to defaults ------------------------*/
kine_clear         (char *a_name, tSEG *a_curr, int a_nleg, int a_nseg)
{
   /*---(defenses)-----------------------*/
   if (strlen(a_name) != 2)                   return -1;
   if (a_curr  == NULL)                       return -2;
   if (a_nleg  <  0  || a_nleg >= MAX_LEGS)   return -3;
   if (a_nleg  <  0  || a_nleg >= MAX_SEGS)   return -4;
   /*---(set name)-----------------------*/
   snprintf(a_curr->n, 11, "%s.%s.%s", legs_name [a_nleg], segs_name[a_nseg], a_name);
   /*---(set lengths)--------------------*/
   a_curr->l   = segs_len [a_nseg];
   a_curr->sl  = segs_len [a_nseg];
   a_curr->fl  =   0.0;
   /*---(angles)-------------------------*/
   if (a_nseg <= YKINE_COXA)    a_curr->d   = legs_deg [a_nleg];
   a_curr->h   =   0.0;
   a_curr->v   =   0.0;
   a_curr->cd  =   0.0;
   a_curr->ch  =   0.0;
   a_curr->cv  =   0.0;
   /*---(coordinates)--------------------*/
   a_curr->x   =   0.0;
   a_curr->z   =   0.0;
   a_curr->xz  =   0.0;
   a_curr->y   =   0.0;
   a_curr->cx  =   0.0;
   a_curr->cz  =   0.0;
   a_curr->cy  =   0.0;
   /*---(control)------------------------*/
   a_curr->p   =   'n';
   a_curr->m   =   'i';
   a_curr->c   =   'n';
   /*---(complete)-----------------------*/
   return 0;
}





/*====================------------------------------------====================*/
/*===----                        body adjustments                      ----===*/
/*====================------------------------------------====================*/
static void      o___BODY____________________o (void) {;}

char       /*----: change the center of gravity ------------------------------*/
kine_center        (float a_x, float a_z)
{
   center.bx     = a_x;
   center.bz     = a_z;
   return 0;
}

char       /*----: change the center of gravity ------------------------------*/
kine_height        (float a_y)
{
   center.by     = a_y;
   return 0;
}

char       /*----: change the pivot (center of movement) --------------------*/
kine_pivot         (float a_x, float a_z)
{
   /*---(save position)-----------------------------------*/
   if (a_x >  100.0)  a_x =  100.0;
   if (a_x < -100.0)  a_x = -100.0;
   center.mx     = a_x;
   if (a_z >  100.0)  a_z =  100.0;
   if (a_z < -100.0)  a_z = -100.0;
   center.mz     = a_z;
   /*---(complete)----------------------------------------*/
   return 0;
}

char       /*----: change the body angles ------------------------------------*/
kine_attitude      (float a_roll, float a_pitch, float a_yaw)
{
   /*---(fix pitch angle)---------------------------------*/
   /*--> properly only between -90 and 90 degrees         */
   while (a_pitch <= -180.0 || a_pitch >  180.0) {
      if (a_pitch <= -180.0)   a_pitch += 360.0;
      if (a_pitch >   180.0)   a_pitch -= 360.0;
   }
   if (a_pitch <= -90.0) {
      a_yaw    += 180.0;
      a_pitch   = -(180.0 + a_pitch);
      a_roll   += 180.0;
   }
   if (a_pitch >   90.0) {
      a_yaw    += 180.0;
      a_pitch   = 180.0 - a_pitch;
      a_roll   += 180.0;
   }
   /*---(fix roll angle)----------------------------------*/
   /*--> properly only between -180 and 180 degrees       */
   while (a_roll  <= -180.0 || a_roll  >  180.0) {
      if (a_roll  <= -180.0)   a_roll  += 360.0;
      if (a_roll  >   180.0)   a_roll  -= 360.0;
   }
   /*---(fix yaw angle)-----------------------------------*/
   /*--> properly only between -180 and 180 degrees       */
   while (a_yaw   <= -180.0 || a_yaw   >  180.0) {
      if (a_yaw   <= -180.0)   a_yaw   += 360.0;
      if (a_yaw   >   180.0)   a_yaw   -= 360.0;
   }
   /*---(save values)-------------------------------------*/
   center.dpitch = a_pitch;
   center.droll  = a_roll;
   center.dyaw   = a_yaw;
   center.pitch  = a_pitch * DEG2RAD;
   center.roll   = a_roll  * DEG2RAD;
   center.yaw    = a_yaw   * DEG2RAD;
   center.cy     = cos(center.yaw);
   center.sy     = sin(center.yaw);
   center.ny     = cos(-center.yaw);
   center.cp     = cos(center.pitch);
   center.sp     = sin(center.pitch);
   center.np     = cos(-center.pitch);
   center.cr     = cos(center.roll);
   center.sr     = sin(center.roll);
   center.nr     = cos(-center.roll);
   /*---(complete)----------------------------------------*/
   return 0;
}



/*============================----end-of-source---============================*/
