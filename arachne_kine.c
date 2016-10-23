/*============================----beg-of-source---============================*/

#include "arachne.h"




/*---[[ CONSTANTS ]]----------------------------------------------------------*/
char   *legscon[MAX_LEGS]  = { "L_RF", "L_RM", "L_RR", "L_LR", "L_LM", "L_LF"};



const    double   DEG2RAD   = M_PI / 180.0;
const    double   RAD2DEG   = 180.0 / M_PI;


tSEG      fk [MAX_LEGS] [MAX_SEGS];    /* opengl kinematics check   */
tSEG      gk [MAX_LEGS] [MAX_SEGS];    /* forward kinematics        */
tSEG      ik [MAX_LEGS] [MAX_SEGS];    /* inverse kinematics        */


char      umake_setup   = 'n';
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
   kine_height       (segs_len [TIBI]);
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
   if (a_nseg <= COXA)    a_curr->d   = legs_deg [a_nleg];
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
/*===----                           printing                           ----===*/
/*====================------------------------------------====================*/
static void      o___PRINT___________________o (void) {;}

char       /*----: print a segment kinematic data header ---------------------*/
kine_header        (void)
{
   printf("name------ ---mm---    --degs--== --hrad--== --vrad--== ---xx---== ---zz---== ---xz---== ---yy---== ---sl---==    --cdeg--== -chrad--== -cvrad--== ---cx---== ---cz---== --cxz---== ---cy---== ---fl---==\n");
   return 0;
}

char       /*----: print a segments kinematic data ---------------------------*/
kine_print         (tSEG *a_curr, tSEG *a_comp)
{
   char      d         = ' ';
   char      h         = ' ';
   char      v         = ' ';
   char      x         = ' ';
   char      z         = ' ';
   char      xz        = ' ';
   char      y         = ' ';
   char      sl        = ' ';
   char      cd        = ' ';
   char      ch        = ' ';
   char      cv        = ' ';
   char      cx        = ' ';
   char      cz        = ' ';
   char      cxz       = ' ';
   char      cy        = ' ';
   char      fl        = ' ';
   if (a_comp != NULL) {
      d    = (a_curr->d  != a_comp->d ) ? '<' : '.';
      h    = (a_curr->h  != a_comp->h ) ? '<' : '.';
      v    = (a_curr->v  != a_comp->v ) ? '<' : '.';
      x    = (round(a_curr->x  ) != round(a_comp->x  )) ? ((trunc(a_curr->x  ) != trunc(a_comp->x  )) ? '<' : 't') : '.';
      z    = (round(a_curr->z  ) != round(a_comp->z  )) ? ((trunc(a_curr->z  ) != trunc(a_comp->z  )) ? '<' : 't') : '.';
      xz   = (round(a_curr->xz ) != round(a_comp->xz )) ? ((trunc(a_curr->xz ) != trunc(a_comp->xz )) ? '<' : 't') : '.';
      y    = (round(a_curr->y  ) != round(a_comp->y  )) ? ((trunc(a_curr->y  ) != trunc(a_comp->y  )) ? '<' : 't') : '.';
      sl   = (round(a_curr->sl ) != round(a_comp->sl )) ? ((trunc(a_curr->sl ) != trunc(a_comp->sl )) ? '<' : 't') : '.';
      /*> cd   = (a_curr->cd != a_comp->cd) ? '<' : '.';                              <* 
       *> ch   = (a_curr->ch != a_comp->ch) ? '<' : '.';                              <* 
       *> cv   = (a_curr->cv != a_comp->cv) ? '<' : '.';                              <*/
      cx   = (round(a_curr->cx ) != round(a_comp->cx )) ? ((trunc(a_curr->cx ) != trunc(a_comp->cx )) ? '<' : 't') : '.';
      cz   = (round(a_curr->cz ) != round(a_comp->cz )) ? ((trunc(a_curr->cz ) != trunc(a_comp->cz )) ? '<' : 't') : '.';
      cxz  = (round(a_curr->cxz) != round(a_comp->cxz)) ? ((trunc(a_curr->cxz) != trunc(a_comp->cxz)) ? '<' : 't') : '.';
      cy   = (round(a_curr->cy ) != round(a_comp->cy )) ? ((trunc(a_curr->cy ) != trunc(a_comp->cy )) ? '<' : 't') : '.';
      fl   = (round(a_curr->fl ) != round(a_comp->fl )) ? ((trunc(a_curr->fl ) != trunc(a_comp->fl )) ? '<' : 't') : '.';
   }
   printf("%10.10s %8.0f    %8.1f %c %8.3f %c %8.3f %c %8.1f %c %8.1f %c %8.1f %c %8.1f %c %8.1f %c    %8.1f %c %8.3f %c %8.3f %c %8.1f %c %8.1f %c %8.1f %c %8.1f %c %8.1f %c\n",
         a_curr->n  ,   a_curr->l ,
         a_curr->d  ,   d, a_curr->h  ,   h, a_curr->v  ,   v,
         a_curr->x  ,   x, a_curr->z  ,   z, a_curr->xz ,  xz, a_curr->y  ,  y, a_curr->sl , sl,
         a_curr->cd ,  cd, a_curr->ch ,  ch, a_curr->cv ,  cv,
         a_curr->cx ,  cx, a_curr->cz ,  cz, a_curr->cxz, cxz, a_curr->cy , cy, a_curr->fl , fl );
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         calculation                          ----===*/
/*====================------------------------------------====================*/
static void      o___CALC____________________o (void) {;}

char       /*----: given joint angles, calculate the final foot position -----*/
kine_forward       (int a_num, float a_thorax, float a_femur, float a_patella, float a_tibia)
{
   /*---(design notes)-------------------*/
   /*
    * only a 3DOF leg will work
    *
    * direct external access...
    *    - constants   MAX_SEGS, DEG2RAD, THOR...TARS
    *    - fk
    *    - center
    *
    */
   /*---(locals)-------------------------*/
   int      i = 0;              /* iterator            */
   float    l , fl, sl;         /* full and xz lens    */
   float    x , y , z , xz;     /* basic x, y, z       */
   float    cx, cy, cz;         /* cum x, y, z         */
   float    ox, oy, oz;
   float    d;                  /* rads and degs       */
   float    v, h;               /* vert/horz rads      */
   tSEG    *a_leg = ((tSEG *) fk) + (a_num * MAX_SEGS);
   /*---(clear forward calcs)------------*/
   for (i = 0; i < MAX_SEGS; ++i)  kine_clear ("fk", &fk[a_num][i], a_num, i);
   /*---(defense)------------------------*/
   if (a_femur    < segs_min [FEMU])        return -1;
   if (a_femur    > segs_max [FEMU])        return -2;
   if (a_patella  < segs_min [PATE])        return -3;
   if (a_patella  > segs_max [PATE])        return -4;
   if (a_tibia    < segs_min [TIBI])        return -5;
   if (a_tibia    > segs_max [TIBI])        return -6;
   /*---(core)---------------------------*/
   cx  =  ox  =  a_leg[CORE].cx  = center.bx;
   cz  =  oz  =  a_leg[CORE].cz  = center.bz;
   cy  =  oy  =  a_leg[CORE].cy  = center.by;
   /*---(thorax)-------------------------*/
   l   =  a_leg[THOR].l   =  segs_len [THOR];
   d   =  a_leg[THOR].d   =  a_thorax;
   h   =  a_leg[THOR].h   =  d * DEG2RAD;
   d   =  a_leg[THOR].cd  =  d;
   h   =  a_leg[THOR].ch  =  h;
   x   =  a_leg[THOR].x   =  l * cos(h);
   z   =  a_leg[THOR].z   = -l * sin(h);
   y   =  a_leg[THOR].y   =  0.0f;
   xz  =  a_leg[THOR].xz  = sqrt((x * x) + (z * z));
   sl  =  a_leg[THOR].sl  = sqrt((x * x) + (z * z) + (y * y));
   a_leg[THOR].cx         = cx   += x;
   a_leg[THOR].cz         = cz   += z;
   a_leg[THOR].cy         = cy   += y;
   xz  =  a_leg[THOR].cxz = sqrt((cx * cx) + (cz * cz));
   fl  =  a_leg[THOR].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   /*---(coxa)---------------------------*/
   l   =  a_leg[COXA].l   = segs_len [COXA];
   d   =  a_leg[COXA].d   = a_thorax;
   h   =  a_leg[COXA].h   =  d * DEG2RAD;
   d   =  a_leg[COXA].cd  =  d;
   h   =  a_leg[COXA].ch  =  h;
   x   =  a_leg[COXA].x   =  l * cos(h);
   z   =  a_leg[COXA].z   = -l * sin(h);
   y   =  a_leg[COXA].y   =  0.0f;
   xz  =  a_leg[COXA].xz  = sqrt((x * x) + (z * z));
   sl  =  a_leg[COXA].sl  = sqrt((x * x) + (z * z) + (y * y));
   a_leg[COXA].cx         = cx   += x;
   a_leg[COXA].cz         = cz   += z;
   a_leg[COXA].cy         = cy   += y;
   xz  =  a_leg[COXA].cxz = sqrt((cx * cx) + (cz * cz));
   fl  =  a_leg[COXA].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   /*---(femur)--------------------------*/
   l   =  a_leg[FEMU].l   = segs_len [FEMU];
   d   =  a_leg[FEMU].d   = a_femur;
   h   =  a_leg[FEMU].h   =  d * DEG2RAD;
   d   =  a_leg[FEMU].cd  =  d + a_thorax;
   h   =  a_leg[FEMU].ch  =  d * DEG2RAD;
   x   =  a_leg[FEMU].x   =  l * cos(h);
   z   =  a_leg[FEMU].z   = -l * sin(h);
   y   =  a_leg[FEMU].y   =  0.0f;
   xz  =  a_leg[FEMU].xz  = sqrt((x * x) + (z * z));
   sl  =  a_leg[FEMU].sl  = sqrt((x * x) + (z * z) + (y * y));
   a_leg[FEMU].cx         = cx   += x;
   a_leg[FEMU].cz         = cz   += z;
   a_leg[FEMU].cy         = cy   += y;
   xz  =  a_leg[FEMU].cxz = sqrt((cx * cx) + (cz * cz));
   fl  =  a_leg[FEMU].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   /*---(patella)------------------------*/
   l   =  a_leg[PATE].l   = segs_len [PATE];
   d   =  a_leg[PATE].d   = a_patella;
   v   =  a_leg[PATE].v   =  d * DEG2RAD;
   d   =  a_leg[PATE].cd  =  d;
   v   =  a_leg[PATE].cv  =  d * DEG2RAD;
   y   =  a_leg[PATE].y   =  l * sin(v);
   xz  =  a_leg[PATE].xz  =  sqrt((l * l) - (y * y));
   x   =  a_leg[PATE].x   =  xz * cos(h);
   z   =  a_leg[PATE].z   = -xz * sin(h);
   sl  =  a_leg[PATE].sl = sqrt((x * x) + (z * z) + (y * y));
   a_leg[PATE].cx         = cx   += x;
   a_leg[PATE].cz         = cz   += z;
   a_leg[PATE].cy         = cy   += y;
   xz  =  a_leg[PATE].cxz = sqrt((cx * cx) + (cz * cz));
   fl  =  a_leg[PATE].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   a_leg[PATE].ch  =  h;
   /*---(tibia)--------------------------*/
   l   =  a_leg[TIBI].l   = segs_len [TIBI];
   d   =  a_leg[TIBI].d   = a_tibia;
   v   =  a_leg[TIBI].v   =  d * DEG2RAD;
   d   =  a_leg[TIBI].cd  =  d + a_patella;
   v   =  a_leg[TIBI].cv  =  d * DEG2RAD;
   y   =  a_leg[TIBI].y   =  l * sin(v);
   xz  =  a_leg[TIBI].xz  =  sqrt((l * l) - (y * y));
   if (d < -90.0) xz *= -1;
   x   =  a_leg[TIBI].x   =  xz * cos(h);
   z   =  a_leg[TIBI].z   = -xz * sin(h);
   sl  =  a_leg[TIBI].sl = sqrt((x * x) + (z * z) + (y * y));
   a_leg[TIBI].cx         = cx   += x;
   a_leg[TIBI].cz         = cz   += z;
   a_leg[TIBI].cy         = cy   += y;
   xz  =  a_leg[TIBI].cxz = sqrt((cx * cx) + (cz * cz));
   fl  =  a_leg[TIBI].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   a_leg[TIBI].ch  =  h;
   /*---(tarsus)-------------------------*/
   l   =  a_leg[TARS].l   = segs_len [TARS];
   a_leg[TARS].cv  =  v;
   a_leg[TARS].ch  =  h;
   a_leg[TARS].cx  = a_leg[TIBI].cx;
   a_leg[TARS].cz  = a_leg[TIBI].cz;
   a_leg[TARS].cxz = a_leg[TIBI].cxz;
   a_leg[TARS].cy  = a_leg[TIBI].cy;
   a_leg[TARS].fl  = a_leg[TIBI].fl;
   /*---(lower)--------------------------*/
   x   =  a_leg[VERT].x   = a_leg[PATE].x + a_leg[TIBI].x;
   z   =  a_leg[VERT].z   = a_leg[PATE].z + a_leg[TIBI].z;
   y   =  a_leg[VERT].y   = a_leg[PATE].y + a_leg[TIBI].y;
   xz  =  a_leg[VERT].xz  = sqrt((x * x) + (z * z));
   l   =  a_leg[VERT].sl  = sqrt((x * x) + (y * y) + (z * z));
   v   =  a_leg[VERT].v   = atan2(y, xz);
   /*---(complete)-----------------------*/
   return 0;
}


char       /*----: given fixed body position, match the leg to the target ----*/
kine_inverse       (int a_num, float a_thorax, float a_x, float a_z, float a_y, char a_debug)
{
   /*---(design notes)-------------------*/
   /*
    * only a 3DOF leg will work
    *
    * direct external access...
    *    - constants   MAX_SEGS, DEG2RAD, RAD2DEG, CORE...TARS
    *    - structures  fk, center
    *
    */
   /*---(calculations)-*-----------------*/
   float     x,  y,  z;                /* coords  : specific segment          */
   float     ox, oy, oz;               /* coords  : body origin               */
   float     cx, cy, cz;               /* coords  : cumulative                */
   float     tx, ty, tz;               /* coords  : target                    */
   float     dx, dy, dz;               /* coords  : delta (for lower leg)     */
   float     l, sl, fl, xz;            /* lengths : std, seg, full, and xz    */
   float     d, d_save;                /* degrees : calc and saved            */
   float     v, h;                     /* radians : vert and horz             */
   double    plen, tlen;               /* lengths : patella and tibia         */
   /*---(workers)------*-----------------*/
   tSEG     *a_leg = NULL;
   int       rc        = 0;            /* generic return code                 */
   char      targ[200];                /* position testing string -- target   */
   char      tars[200];                /* position testing string -- tarsus   */
   int       i         = 0;            /* loop iterator -- segment            */
   /*---(switches)-----*-----------------*/
   char      femu_switch = 'n';
   char      tibi_switch = 'n';
   /*---(begin)--------------------------*/
   DEBUG_L  printf("\n---(%d)------------------------------------------\n", a_num);
   DEBUG_L  printf("ARGS : deg = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f\n", a_thorax, a_x, a_z, a_y);
   /*---(clear)--------------------------*/
   for (i = 0; i < MAX_SEGS; ++i) kine_clear ("ik", &ik[a_num][i], a_num, i);
   /*---(point to leg)-------------------*/
   a_leg = ((tSEG *) ik) + (a_num * MAX_SEGS);
   /*---(core)---------------------------*/
   cx   =  ox  =  a_leg[CORE].cx  = center.bx;
   cz   =  oz  =  a_leg[CORE].cz  = center.bz;
   cy   =  oy  =  a_leg[CORE].cy  = center.by;
   DEBUG_L  printf("CORE : len = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f\n", 0.0, cx, cz, cy);
   /*---(target)-------------------------*/
   /*> x    =  tx  =  a_leg[TARG].cx  = cx + a_x;                                     <*/
   x    =  tx  =  a_leg[TARG].cx  = a_x - cx * 0.00;
   /*> z    =  tz  =  a_leg[TARG].cz  = cz + a_z;                                     <*/
   z    =  tz  =  a_leg[TARG].cz  = a_z - cz * 0.00;
   y    =  ty  =  a_leg[TARG].cy  = cy + a_y;
   xz   = a_leg[TARG].cxz  = sqrt  ((x * x) + (z * z));
   h    = a_leg[TARG].ch   = atan2 (z, x);
   v    = a_leg[TARG].cv   = atan2 (y, xz);
   fl   = a_leg[TARG].fl   = sqrt  ((x * x) + (z * z) + (y * y));
   DEBUG_L  printf("TARG : len = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f, h=%6.3f, v=%6.3f, dh=%6.1f, dv=%6.1f\n", fl, x, z, y, h, v, h * RAD2DEG, v * RAD2DEG);
   DEBUG_L  printf("----\n");
   /*---(thorax)-------------------------*/
   l    =  a_leg[THOR].l   =  segs_len [THOR];
   d    =  a_leg[THOR].d   =  a_thorax;
   h    =  a_leg[THOR].h   =  d * DEG2RAD;
   d    =  a_leg[THOR].cd  =  d;
   h    =  a_leg[THOR].ch  =  h;
   x    =  a_leg[THOR].x   =  l * cos(h);
   z    =  a_leg[THOR].z   = -l * sin(h);
   y    =  a_leg[THOR].y   =  0.0f;
   xz   =  a_leg[THOR].xz  = sqrt((x * x) + (z * z));
   sl   =  a_leg[THOR].sl  = sqrt((x * x) + (z * z) + (y * y));
   a_leg[THOR].cx         = cx   += x;
   a_leg[THOR].cz         = cz   += z;
   a_leg[THOR].cy         = cy   += y;
   xz   =  a_leg[THOR].cxz = sqrt((cx * cx) + (cz * cz));
   fl   =  a_leg[THOR].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   a_leg[CALC].x  += x;
   a_leg[CALC].z  += z;
   a_leg[CALC].y  += y;
   DEBUG_L  printf("THOR : len = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f, h=%6.3f, v=%6.3f, dh=%6.2f, dv=%6.2f\n", fl, cx, cz, cy, h, v, h * RAD2DEG, v * RAD2DEG);
   /*---(coxa)---------------------------*/
   l    =  a_leg[COXA].l   = segs_len [COXA];
   d    =  a_leg[COXA].d   = a_thorax;
   h    =  a_leg[COXA].h   =  d * DEG2RAD;
   d    =  a_leg[COXA].cd  =  d;
   h    =  a_leg[COXA].ch  =  h;
   x    =  a_leg[COXA].x   =  l * cos(h);
   z    =  a_leg[COXA].z   = -l * sin(h);
   y    =  a_leg[COXA].y   =  0.0f;
   xz   =  a_leg[COXA].xz  = sqrt((x * x) + (z * z));
   sl   =  a_leg[COXA].sl  = sqrt((x * x) + (z * z) + (y * y));
   a_leg[COXA].cx         = cx   += x;
   a_leg[COXA].cz         = cz   += z;
   a_leg[COXA].cy         = cy   += y;
   xz   =  a_leg[COXA].cxz = sqrt((cx * cx) + (cz * cz));
   fl   =  a_leg[COXA].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   DEBUG_L  printf("COXA : len = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f, h=%6.3f, v=%6.3f, dh=%6.2f, dv=%6.2f\n", fl, cx, cz, cy, h, v, h * RAD2DEG, v * RAD2DEG);
   DEBUG_L  printf("----\n");
   /*----(remaining)-----------------------------------*/
   dx   = a_leg[TARG].cx - a_leg[COXA].cx;
   dz   = a_leg[TARG].cz - a_leg[COXA].cz;
   dy   = a_leg[TARG].cy - a_leg[COXA].cy;
   h    = atan2(dz, dx);
   d    = (atan2(-dz, dx) * RAD2DEG) - a_leg[COXA].d;
   DEBUG_L  printf("LEFT : deg = %8.2f, dx=%8.2f, dz=%8.2f, dy=%8.2f, h=%6.3f, v= -----, dh=%6.2f, dv= -----\n", d, dx, dz, dy, h, h * RAD2DEG);
   /*---(clean up and adjust)--------------------------*/
   snprintf(targ, 200, "%8.1f", d);
   d = atof(targ);
   d_save = d;
   if (d >  180.0) { d =  d - 360.0; femu_switch = 'n'; }
   if (d >   90.0) { d =  d - 180.0; femu_switch = 'y'; }
   if (d < -180.0) { d =  360.0 + d; femu_switch = 'n'; }
   if (d <  -90.0) { d =  180.0 + d; femu_switch = 'y'; }
   DEBUG_L  if (d != d_save) printf("degree was %8.1f, adjusted to %8.1f\n", d_save, d);
   /*---(femur)----------------------------------------*/
   l    =  a_leg[FEMU].l   = segs_len [FEMU];
   a_leg[FEMU].d           =  d;
   a_leg[FEMU].h           =  d * DEG2RAD;
   d    =  a_leg[FEMU].cd  =  d + a_leg[COXA].d;
   h    =  a_leg[FEMU].ch  =  a_leg[FEMU].cd * DEG2RAD;
   x    =  a_leg[FEMU].x   =  l * cos(h);
   z    =  a_leg[FEMU].z   = -l * sin(h);
   y    =  a_leg[FEMU].y   =  0.0f;
   xz   =  a_leg[FEMU].xz  = sqrt((x * x) + (z * z));
   sl   =  a_leg[FEMU].sl  = sqrt((x * x) + (z * z) + (y * y));
   a_leg[FEMU].cx          = cx   += x;
   a_leg[FEMU].cz          = cz   += z;
   a_leg[FEMU].cy          = cy   += y;
   xz   =  a_leg[FEMU].cxz = sqrt((cx * cx) + (cz * cz));
   fl   =  a_leg[FEMU].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   DEBUG_L  printf("FEMU : len = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f, h=%6.3f, v=%6.3f, dh=%6.2f, dv=%6.2f, switch = %c\n", fl, cx, cz, cy, h, v, h * RAD2DEG, v * RAD2DEG, femu_switch);
   DEBUG_L  printf("----\n");
   /*---(patella/tibia)--------------------------------*/
   dx   = a_leg[VERT].x    = a_leg[TARG].cx - a_leg[FEMU].cx;
   dz   = a_leg[VERT].z    = a_leg[TARG].cz - a_leg[FEMU].cz;
   dy   = a_leg[VERT].y    = a_leg[TARG].cy - a_leg[FEMU].cy;
   xz   = a_leg[VERT].xz   = sqrt((dx * dx) + (dz * dz));
   sl   = a_leg[VERT].sl   = sqrt((dx * dx) + (dz * dz) + (dy * dy));
   v    = a_leg[VERT].v    = atan2(dy, xz);
   d    = a_leg[VERT].d    = v * RAD2DEG;
   h    = atan2(dz, dx);
   d    = (atan2(-dz, dx) * RAD2DEG) - a_leg[COXA].d;
   while (round(d) <    0.0) d += 360.0;
   while (round(d) >= 360.0) d -= 360.0;
   DEBUG_L  printf("LEFT : len = %8.2f, dx=%8.2f, dz=%8.2f, dy=%8.2f, h=%6.3f, v=%6.3f, dh=%6.2f, dv=%6.2f\n", sl, dx, dz, dy, h, v, h * RAD2DEG, v * RAD2DEG);
   /*---(clean up and adjust)--------------------------*/
   snprintf(targ, 200, "%8.1f", d);
   d = atof(targ);
   if      (d >  180.0) tibi_switch = 'n';
   else if (d >   90.0) tibi_switch = 'y';
   else if (d < -180.0) tibi_switch = 'n';
   else if (d <  -90.0) tibi_switch = 'y';
   DEBUG_L  printf("tibi_switch=%c\n", tibi_switch);
   /*---(vertical)-------------------------------------*/
   double a1, a2, a3;
   plen = a_leg[PATE].l   = segs_len [PATE];
   tlen = a_leg[TIBI].l   = segs_len [TIBI];
   l    = sl;
   a1   = acos(((l * l)   + (tlen * tlen) - (plen * plen)) / (2.0 * l    * tlen));
   a2   = acos(((l * l)   + (plen * plen) - (tlen * tlen)) / (2.0 * l    * plen));
   a3   = acos(((tlen * tlen) + (plen * plen) - (l * l)  ) / (2.0 * plen * tlen));
   a3  -= M_PI;
   DEBUG_L  printf("plen=%8.3f, tlen=%8.3f, hypo=%8.3f\n", plen, tlen, l);
   DEBUG_L  printf("a2  =%8.3f, a2d =%8.1f, v   =%8.3f, so  =%8.1f\n", a2, a2 * RAD2DEG, v * RAD2DEG, a2 + (90.0 - v));
   DEBUG_L  printf("  a1  = %8.3f, a1d = %8.3f\n",  a1, a1 * RAD2DEG);
   DEBUG_L  printf("  a2  = %8.3f, a2d = %8.3f\n",  a2, a2 * RAD2DEG);
   DEBUG_L  printf("  a3  = %8.3f, a3d = %8.3f\n",  a3, a3 * RAD2DEG);
   float d1 = a1 * RAD2DEG;
   float d2 = a2 * RAD2DEG;
   float dv = v  * RAD2DEG;
   DEBUG_L  printf("d2  =%8.3f, dv  =%8.3f, d2 - dv = %8.3f, maybe=%8.3f\n", d2, dv, d2 - dv, fabs(a2 * RAD2DEG) - fabs(v * RAD2DEG));
   /*---(tibia angle)----------------------------------*/
   l    =  a_leg[TIBI].l;
   v    =  a_leg[TIBI].v   =  a3;
   if (isnan(a3)) v    =  a_leg[TIBI].v   =  0.0;
   d    =  a_leg[TIBI].d   =  v * RAD2DEG;
   /*---(patella angle)--------------------------------*/
   if (isnan(a2)) d2    =  0.0;
   if (femu_switch == 'n') {
      DEBUG_L  printf("femu_switch = n\n");
      if (a_leg[VERT].y > 0.0) {
         DEBUG_L  printf("above center and tibi_switch = n\n");
         d    =  a_leg[PATE].d   =  dv + d2;
      } else if (tibi_switch== 'y') {
         DEBUG_L  printf("tibi_switch = y\n");
         d    =  a_leg[PATE].d   =  dv - a_leg[VERT].d;
      } else {
         DEBUG_L  printf("tibi_switch = n\n");
         d    =  a_leg[PATE].d   =  fabs(d2) - fabs(dv);
      }
   } else {
      DEBUG_L  printf("femu_switch = y\n");
      if (tibi_switch == 'n') {
         DEBUG_L  printf("tibi_switch = n\n");
         d    =  a_leg[PATE].d   =  dv - a_leg[VERT].d;
      } else {
         DEBUG_L  printf("tibi_switch = y\n");
         d    =  a_leg[PATE].d   =  (d2 - dv) - 180.0;
      }
   }
   DEBUG_L  printf("----\n");
   /*---(patella)-----------------------------------------*/
   l   =  a_leg[PATE].l;
   d   =  a_leg[PATE].d;
   h   =  a_leg[PATE].h   =  a_leg[FEMU].ch;
   v   =  a_leg[PATE].v   =  d * DEG2RAD;
   d   =  a_leg[PATE].cd  =  d;
   v   =  a_leg[PATE].cv  =  d * DEG2RAD;
   y   =  a_leg[PATE].y   =  l * sin(v);
   xz  =  a_leg[PATE].xz  =  sqrt((l * l) - (y * y));
   x   =  a_leg[PATE].x   =  xz * cos(h);
   z   =  a_leg[PATE].z   = -xz * sin(h);
   sl  =  a_leg[PATE].sl = sqrt((x * x) + (z * z) + (y * y));
   a_leg[PATE].cx         = cx   += x;
   a_leg[PATE].cz         = cz   += z;
   a_leg[PATE].cy         = cy   += y;
   xz  =  a_leg[PATE].cxz = sqrt((cx * cx) + (cz * cz));
   fl  =  a_leg[PATE].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   a_leg[PATE].h   =  a_leg[FEMU].h;
   a_leg[PATE].ch  =  a_leg[FEMU].ch;
   DEBUG_L  printf("PATE : len = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f, h= -----, v=%6.3f, dh= -----, dv=%6.2f\n", fl, cx, cz, cy, v, v * RAD2DEG);
   /*---(tibia)-------------------------------------------*/
   l   =  a_leg[TIBI].l;
   d   =  a_leg[TIBI].d;
   h   =  a_leg[TIBI].h   =  a_leg[FEMU].ch;
   v   =  a_leg[TIBI].v   =  d * DEG2RAD;
   d   =  a_leg[TIBI].cd  =  d + a_leg[PATE].d;
   v   =  a_leg[TIBI].cv  =  d * DEG2RAD;
   y   =  a_leg[TIBI].y   =  l * sin(v);
   xz  =  a_leg[TIBI].xz  =  sqrt((l * l) - (y * y));
   if (d < -90.0) xz *= -1;
   x   =  a_leg[TIBI].x   =  xz * cos(h);
   z   =  a_leg[TIBI].z   = -xz * sin(h);
   sl  =  a_leg[TIBI].sl = sqrt((x * x) + (z * z) + (y * y));
   a_leg[TIBI].cx         = cx   += x;
   a_leg[TIBI].cz         = cz   += z;
   a_leg[TIBI].cy         = cy   += y;
   xz  =  a_leg[TIBI].cxz = sqrt((cx * cx) + (cz * cz));
   fl  =  a_leg[TIBI].fl  = sqrt((cx * cx) + (cz * cz) + ((cy - oy) * (cy - oy)));
   a_leg[TIBI].h   =  a_leg[FEMU].h;
   a_leg[TIBI].ch  =  a_leg[FEMU].ch;
   DEBUG_L  printf("TIBI : len = %8.2f, cx=%8.2f, cz=%8.2f, cy=%8.2f, h= -----, v=%6.3f, dh= -----, dv=%6.2f\n", fl, cx, cz, cy, a_leg[TIBI].v, a_leg[TIBI].v * RAD2DEG);
   /*---(tarsus)-------------------------*/
   a_leg[TARS].ch         = a_leg[TIBI].ch;
   a_leg[TARS].cv         = a_leg[TIBI].cv;
   a_leg[TARS].cx         = a_leg[TIBI].cx;
   a_leg[TARS].cz         = a_leg[TIBI].cz;
   a_leg[TARS].cxz        = a_leg[TIBI].cxz;
   a_leg[TARS].cy         = a_leg[TIBI].cy;
   a_leg[TARS].fl         = a_leg[TIBI].fl;
   /*---(check joint angles)-------------*/
   rc = kine_max(a_leg[FEMU].d, a_leg[PATE].d, a_leg[TIBI].d);
   DEBUG_L  printf("%1d : rc=%2d, f=%8.1f, p=%8.1f, t=%8.1f\n", a_num, rc, a_leg[FEMU].d, a_leg[PATE].d, a_leg[TIBI].d);
   if (rc != 0) {
      a_leg[TARG].rc  = 0;
      return rc;
   }
   /*---(check actual vs target)---------*/
   snprintf(targ, 200, "%8.1f", a_leg[TARG].cx);
   snprintf(tars, 200, "%8.1f", a_leg[TARS].cx);
   DEBUG_L  printf("x : targ=<<%s>> vs tars=<<%s>>\n", targ, tars);
   if ((rc = yVAR_round("u_round1", targ, tars)) >= 0) {
      snprintf(targ, 200, "%8.1f", a_leg[TARG].cz);
      snprintf(tars, 200, "%8.1f", a_leg[TARS].cz);
      DEBUG_L  printf("z : targ=<<%s>> vs tars=<<%s>>\n", targ, tars);
      if ((rc = yVAR_round("u_round1", targ, tars)) >= 0) {
         snprintf(targ, 200, "%8.1f", a_leg[TARG].cy);
         snprintf(tars, 200, "%8.1f", a_leg[TARS].cy);
         DEBUG_L  printf("y : targ=<<%s>> vs tars=<<%s>>\n", targ, tars);
         if ((rc = yVAR_round("u_round1", targ, tars)) >= 0) {
            DEBUG_L  printf("NAILED IT\n");
            a_leg[TARG].rc  = 1;
            return 0;
         } else {
            DEBUG_L  printf("%d y is off : rc=%3d, targ=<<%s>> vs tars=<<%s>>\n", a_num, rc, targ, tars);
            a_leg[TARG].rc  = 0;
            return -11;
         }
      } else {
         DEBUG_L  printf("%d z is off : rc=%3d, targ=<<%s>> vs tars=<<%s>>\n", a_num, rc, targ, tars);
          a_leg[TARG].rc  = 0;
         return -12;
      }
   } else {
      DEBUG_L  printf("%d x is off : rc=%3d, targ=<<%s>> vs tars=<<%s>>\n", a_num, rc, targ, tars);
      a_leg[TARG].rc  = 0;
      return -13;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
kine_max           (float a_femur, float a_patella, float a_tibia)
{
   char      actu[200] = "";
   float     value     = 0.0;
   char      rc        = 0;
   /*> printf("femur = %10.4f, patella = %10.4f, tibia = %10.4f\n", a_femur, a_patella, a_tibia);   <*/
   /*---(femur absolute)-----------------*/
   snprintf(actu, 200, "%8.1f", a_femur);
   value = atof(actu);
   if (value <  segs_min [FEMU] )                return -1;
   if (value >  segs_max [FEMU] )                return -2;
   /*---(patella absolute)---------------*/
   snprintf(actu, 200, "%8.1f", a_patella);
   value = atof(actu);
   if (value <  segs_min [PATE] )                return -3;
   if (value >  segs_max [PATE] )                return -4;
   /*---(tibia absolute)-----------------*/
   snprintf(actu, 200, "%8.1f", a_tibia);
   value = atof(actu);
   if (value <  segs_min [TIBI] )                return -5;
   if (value >  segs_max [TIBI] )                return -6;
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


int      /*->> read scripting commands from stdin and interpret               */
kinetics_scripter (void)
{
   /*---(local variables)------------------*/
   char  x_line[500] = "";
   /*---(set stdin to non-blocking)--------*/
   int flags = fcntl(0, F_GETFL, 0);
   fcntl(0, F_SETFL, flags | O_NONBLOCK);
   /*---(test stdin for input)-------------*/
   /*> printf("testing for script input...");                                         <*/
   char  x_ch = getc(stdin);
   if (x_ch == -1) {
      /*> printf(" no input presently available\n");                                  <*/
      return -1;
   }
   /*> printf(" found input, now read line\n");                                       <*/
   ungetc(x_ch, stdin);
   fcntl(0, F_SETFL, flags);
   /*---(continue with input)--------------*/
   fgets(x_line, 500, stdin);
   while (1) {
      if (feof(stdin)) {
         /*> printf("   eof character, assumed end of stdin\n");                      <*/
         return -2;
      }
      x_line[strlen(x_line) - 1] = '\0';
      /*> if (strlen(x_line) <= 60) printf("   read <<%s>>\n",       x_line);         <* 
       *> else                      printf("   read <<%.60s...>>\n", x_line);         <*/
      fgets(x_line, 500, stdin);
   }
   /*> printf("   done\n");                                                           <*/
   /*---(complete)-------------------------*/
   return 0;
}


/*====================------------------------------------====================*/
/*===----                     unit testing creation                    ----===*/
/*====================------------------------------------====================*/
static void      o___UNIT____________________o (void) {;}

char       /*----: create a unit test SCRP line ------------------------------*/
unit_SCRP          (char *a_desc, char *a_method)
{
   printf("\n\n\n");
   printf("#==(verb)===  ===========(short desc)============  =====(function)=====  ========================(arguments)=========================  ==========  ==========================(results)========================= \n");
   printf("%-12.12s  %-35.35s  %-20.20s \n", "SCRP", a_desc, a_method);
   return 0;
}

char       /*----: create a unit test SCRP line ------------------------------*/
unit_COND          (char *a_desc)
{
   printf("\n");
   printf("   %-9.9s  %-59.59s \n", "COND", a_desc);
   return 0;
}

char       /*----: create a unit test EXEC line ------------------------------*/
unit_EXEC          (char *a_desc, char *a_method)
{
   char      msg[200] = "";
   snprintf(msg, 200, "%d", 0);
   printf("     %-7.7s  %-35.35s  %-20.20s  %-60.60s  %-10.10s  %-60.60s \n", "exec", a_desc, a_method, " ", "i_equal", msg);
   return 0;
}

char       /*----: create the unit test for initialization -------------------*/
unit_init          (void)
{
   /*---(locals)-------------------------*/
   int i = 0;
   int j = 0;
   char      msg[200] = "";
   /*---(init)---------------------------*/
   unit_SCRP("verify kinematics initialation", "kine_init");
   unit_COND("basic initialization");
   unit_EXEC("call the initialization", "kine_init");
   /*---(FK)-----------------------------*/
   for (i = 0; i < LEGS; ++i) {
      snprintf(msg, 200, "check %s values for %s leg/segment", "FK", legs_name [i]);
      unit_COND(msg);
      for (j = 0; j < MAX_SEGS; ++j) {
         snprintf(msg, 200, "\"%s\"", "FK_angles");
         printf("     get      check %2.2s.%4.4s angles                 %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  s_equal     %s.%s.fk deg :%8.1fl,%8.1fd,%8.3fv,%8.3fh      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], 0.0f, 0.0f, 0.0f);
         snprintf(msg, 200, "\"%s\"", "FK_actual");
         printf("     get      check %2.2s.%4.4s actuals                %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  s_equal     %s.%s.fk act :%8.1fl,%8.1fx,%8.1fz,%8.1fy      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], 0.0f, 0.0f, 0.0f);
      }
   }
   /*---(IK)-----------------------------*/
   for (i = 0; i < LEGS; ++i) {
      snprintf(msg, 200, "check %s values for %s leg/segment", "IK", legs_name [i]);
      unit_COND(msg);
      for (j = 0; j < MAX_SEGS; ++j) {
         snprintf(msg, 200, "\"%s\"", "IK_angles");
         printf("     get      check %2.2s.%4.4s angles                 %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  s_equal     %s.%s.ik deg :%8.1fl,%8.1fd,%8.3fv,%8.3fh      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], 0.0f, 0.0f, 0.0f);
         snprintf(msg, 200, "\"%s\"", "IK_actual");
         printf("     get      check %2.2s.%4.4s actuals                %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  s_equal     %s.%s.ik act :%8.1fl,%8.1fx,%8.1fz,%8.1fy      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], 0.0f, 0.0f, 0.0f);
      }
   }
   printf("\n\n");
   return 0;
}

char       /*----: create the unit test for all current leg positions --------*/
unit_FK            (void)
{
   /*---(locals)-------------------------*/
   int i = 0;
   int j = 0;
   char      msg[200] = "";
   /*---(init)---------------------------*/
   unit_SCRP("verify FK/IK for ...", "kine_forward");
   /*---(set body position)--------------*/
   unit_COND("set the body center point");
   printf("     exec       establish a predictable center     %-20.20s  %10.1f, %10.1f, %10.1f %25.25s  i_equal     %-60.60s \n",
         "kine_center"  , 0.0f, 0.0f, segs_len [TIBI], " ", "0");
   printf("     exec       establish a neutral pivot          %-20.20s  %10.1f, %10.1f %37.37s  i_equal     %-60.60s \n",
         "kine_pivot"   , 0.0f, 0.0f, " ", "0");
   printf("     exec       establish a neutral attitude       %-20.20s  %10.1f, %10.1f, %10.1f %25.25s  i_equal     %-60.60s \n",
         "kine_attitude", 0.0f, 0.0f, 0.0f, " ", "0");
   /*---(FK)-----------------------------*/
   for (i = 0; i < LEGS; ++i) {
      snprintf(msg, 200, "check %s values for %s leg/segment", "FK", legs_name [i]);
      unit_COND(msg);
      printf("     exec       evaluate %2.2s forward kinematics     %-20.20s  %-4.4s, %10.1f, %10.1f, %10.1f, %10.1f, 0 %4.4s  i_equal     %-60.60s \n",
            legs_name [i], "kine_forward", legscon[i], gk[i][THOR].d, gk[i][FEMU].d, gk[i][PATE].d, gk[i][TIBI].d, " ", "0");
      for (j = 0; j <= TARS; ++j) {
         snprintf(msg, 200, "\"%s\"", "FK_angles");
         printf("     get        check %2.2s.%4.4s FK segment angles    %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  u_roundz    %s.%s.fk deg :%8.1fl,%8.1fd,%8.3fv,%8.3fh      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], gk[i][j].d , gk[i][j].v , gk[i][j].h );
         snprintf(msg, 200, "\"%s\"", "FK_actual");
         printf("     get        check %2.2s.%4.4s FK segment actual    %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  u_roundz    %s.%s.fk act :%8.1fl,%8.1fx,%8.1fz,%8.1fy      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], gk[i][j].x , gk[i][j].z , gk[i][j].y );
         snprintf(msg, 200, "\"%s\"", "FK_endpoint");
         printf("     get        check %2.2s.%4.4s FK cum endpoint      %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  u_roundz    %s.%s.fk end :%8.1fl,%8.1fx,%8.1fz,%8.1fy      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], gk[i][j].cx, gk[i][j].cz, gk[i][j].cy);
      }
      snprintf(msg, 200, "check %s values for %s leg/segment", "IK", legs_name [i]);
      unit_COND(msg);
      printf("     exec       evaluate %2.2s inverse kinematics     %-20.20s  %-4.4s, %10.3f, %10.3f, %10.3f, %10.3f, 0 %4.4s  i_equal     %-60.60s \n",
            legs_name [i], "kine_inverse", legscon[i], gk[i][THOR].d, gk[i][TARS].cx, gk[i][TARS].cz, gk[i][TARS].cy, " ", "0");
      for (j = 0; j <= TARS; ++j) {
         snprintf(msg, 200, "\"%s\"", "IK_angles");
         printf("     get        check %2.2s.%4.4s IK segment angles    %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  u_roundz    %s.%s.ik deg :%8.1fl,%8.1fd,%8.3fv,%8.3fh      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], gk[i][j].d , gk[i][j].v , gk[i][j].h );
         snprintf(msg, 200, "\"%s\"", "IK_actual");
         printf("     get        check %2.2s.%4.4s IK segment actual    %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  u_roundz    %s.%s.ik act :%8.1fl,%8.1fx,%8.1fz,%8.1fy      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], gk[i][j].x , gk[i][j].z , gk[i][j].y );
         snprintf(msg, 200, "\"%s\"", "IK_endpoint");
         printf("     get        check %2.2s.%4.4s IK cum endpoint      %-20.20s  %-15.15s, %-10.10s, %-10.10s %-20.20s  u_roundz    %s.%s.ik end :%8.1fl,%8.1fx,%8.1fz,%8.1fy      \n",
               legs_name [i], segs_name[j], "unit_accessor", msg, legscon[i], segs_caps[j], " ", legs_name [i], segs_name[j], segs_len [j], gk[i][j].cx, gk[i][j].cz, gk[i][j].cy);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

void
view_unit()
{
   int i = 0;
   int j = 0;
   printf("\n");
   printf("  exec  | evaluate the body and coxa positions  |"
         " kinetics_body             |                "
         "                                         | i_equal   |"
         " 0                                                       |\n");
   for (j = 1; j < LEGS; ++j) {
      printf("  exec  | set the target tarsus position        |"
            " kinetics_target           | L_%s,%12.4f,%12.4f,%12.4f"
            "             | i_equal   |"
            " 0                                                       |\n",
            legs_name [j], gk[j][TIBI].x, gk[j][TIBI].z, gk[j][TIBI].y);
      printf("  exec  | evaluate the femur position           |"
            " ik_femur                  | L_%s             "
            "                                       | i_equal   |"
            " 0                                                       |\n",
            legs_name [j]);
      printf("  exec  | evaluate the femur position           |"
            " kinetics_lower            | L_%s             "
            "                                       | i_equal   |"
            " 0                                                       |\n",
            legs_name [j]);
      for (i = 0; i < MAX_SEGS; ++i) {
         printf("  get   | check %s %s position                |"
               " kinetics_getter           |"
               " \"actual\",       L_%s,  %s                             |"
               " u_round   | %s.%s actual :%8.0fl,%8.0fx,%8.0fz,%8.0fy |\n",
               legs_name [j], segs_name[i], legs_name [j], segs_caps[i], legs_name [j], segs_name[i],
               gk[j][i].l, gk[j][i].x, gk[j][i].z, gk[j][i].y);
         printf("  get   | check %s %s angles                  |"
               " kinetics_getter           |"
               " \"angles\",       L_%s,  %s                             |"
               " u_round   | %s.%s angles :%8.0fl,%8.0fd,%8.2fv,%8.2fh |\n",
               legs_name [j], segs_name[i], legs_name [j], segs_caps[i], legs_name [j], segs_name[i],
               gk[j][i].l, gk[j][i].d, gk[j][i].v, gk[j][i].h);
      }
   }
   return;
}

/*============================----end-of-source---============================*/
