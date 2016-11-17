/*============================----beg-of-source---============================*/

#include "arachne.h"



/*---[[ CONSTANTS ]]----------------------------------------------------------*/
char    legs_name [YKINE_MAX_LEGS][LEN_LABEL];
char    legs_long [YKINE_MAX_LEGS][LEN_LABEL];
double  legs_deg  [YKINE_MAX_LEGS];

char    segs_name [YKINE_MAX_SEGS][LEN_LABEL];
char    segs_long [YKINE_MAX_SEGS][LEN_LABEL];
double  segs_len  [YKINE_MAX_SEGS];
double  segs_max  [YKINE_MAX_SEGS];
double  segs_min  [YKINE_MAX_SEGS];

double  segs_lnk  [YKINE_MAX_SEGS]      = {   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0  };
double  segs_act  [YKINE_MAX_SEGS]      = {   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0  };


const    double   DEG2RAD   = M_PI / 180.0;
const    double   RAD2DEG   = 180.0 / M_PI;


tSEG      fk [MAX_LEGS] [MAX_SEGS];    /* opengl kinematics check   */
tSEG      gk [MAX_LEGS] [MAX_SEGS];    /* forward kinematics        */
tSEG      ik [MAX_LEGS] [MAX_SEGS];    /* inverse kinematics        */


char      umake_init    = 'n';
char      umake_leg     = 'n';


static   FILE  *f_unit  = NULL;


static  int   s_error [YKINE_MAX_SEGS][YKINE_MAX_METH];
static  int   s_warn  [YKINE_MAX_SEGS][YKINE_MAX_METH];



/*====================------------------------------------====================*/
/*===----                        initialization                        ----===*/
/*====================------------------------------------====================*/
static void      o___INIT____________________o (void) {;}

char
KINE_load          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   int         j           = 0;
   /*---(gather leg data)----------------*/
   for (i = 0; i < YKINE_MAX_LEGS; ++i) {
      strlcpy (legs_name [i], yKINE_legcaps (i), LEN_LABEL);
      strlcpy (legs_long [i], yKINE_legfull (i), LEN_LABEL);
      legs_deg  [i] = yKINE_legdeg (i);
   }
   /*---(gather segment data)------------*/
   for (i = 0; i < YKINE_MAX_SEGS; ++i) {
      strlcpy (segs_name [i], yKINE_segfour (i), LEN_LABEL);
      strlcpy (segs_long [i], yKINE_segfull (i), LEN_LABEL);
      segs_len  [i] = yKINE_seglen (i);
      segs_min  [i] = yKINE_segmin (i);
      segs_max  [i] = yKINE_segmax (i);
      for (j = 0; j < YKINE_MAX_METH; ++j) {
         s_error   [i][j] = 0;
         s_warn    [i][j] = 0;
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
KINE_begin         (void)
{
   yKINE_init      (0);
   yKINE_center    (0.0, 0.0, 0.0);
   KINE_load       ();
   f_unit = fopen ("arachne.dump", "w");
   return 0;
}

char
KINE_end           (void)
{
   fprintf (f_unit, "\n");
   fprintf (f_unit, "TOTAL FK ERRORS        ");
   fprintf (f_unit, "coxa errors = %-4d, warnings = %-4d                        ", s_error [YKINE_COXA][YKINE_FK], s_warn  [YKINE_COXA][YKINE_FK]);
   fprintf (f_unit, "femu errors = %-4d, warnings = %-4d                        ", s_error [YKINE_FEMU][YKINE_FK], s_warn  [YKINE_FEMU][YKINE_FK]);
   fprintf (f_unit, "pate errors = %-4d, warnings = %-4d                        ", s_error [YKINE_PATE][YKINE_FK], s_warn  [YKINE_PATE][YKINE_FK]);
   fprintf (f_unit, "tibi errors = %-4d, warnings = %-4d                        ", s_error [YKINE_TIBI][YKINE_FK], s_warn  [YKINE_TIBI][YKINE_FK]);
   fprintf (f_unit, "targ errors = %-4d, warnings = %-4d                        ", s_error [YKINE_TARG][YKINE_FK], s_warn  [YKINE_TARG][YKINE_FK]);
   fprintf (f_unit, "lowr errors = %-4d, warnings = %-4d                        ", s_error [YKINE_LOWR][YKINE_FK], s_warn  [YKINE_LOWR][YKINE_FK]);
   fprintf (f_unit, "\n");
   fprintf (f_unit, "TOTAL IK ERRORS        ");
   fprintf (f_unit, "coxa errors = %-4d, warnings = %-4d                        ", s_error [YKINE_COXA][YKINE_IK], s_warn  [YKINE_COXA][YKINE_IK]);
   fprintf (f_unit, "femu errors = %-4d, warnings = %-4d                        ", s_error [YKINE_FEMU][YKINE_IK], s_warn  [YKINE_FEMU][YKINE_IK]);
   fprintf (f_unit, "pate errors = %-4d, warnings = %-4d                        ", s_error [YKINE_PATE][YKINE_IK], s_warn  [YKINE_PATE][YKINE_IK]);
   fprintf (f_unit, "tibi errors = %-4d, warnings = %-4d                        ", s_error [YKINE_TIBI][YKINE_IK], s_warn  [YKINE_TIBI][YKINE_IK]);
   fprintf (f_unit, "targ errors = %-4d, warnings = %-4d                        ", s_error [YKINE_TARG][YKINE_IK], s_warn  [YKINE_TARG][YKINE_IK]);
   fprintf (f_unit, "lowr errors = %-4d, warnings = %-4d                        ", s_error [YKINE_LOWR][YKINE_IK], s_warn  [YKINE_LOWR][YKINE_IK]);
   fprintf (f_unit, "\n");
   fclose  (f_unit);
   return 0;
}

char
KINE_compare       (int a_seg)
{
   /*> double      x_len       = 0.0;                                                       <* 
    *> double      x_xpos      = 0.0;                                                       <* 
    *> double      x_zpos      = 0.0;                                                       <* 
    *> double      x_ypos      = 0.0;                                                       <* 
    *> double      x_coxa      = 0.0;                                                       <* 
    *> double      x_femu      = 0.0;                                                       <* 
    *> double      x_pate      = 0.0;                                                       <* 
    *> double      x_tibi      = 0.0;                                                       <* 
    *> printf ("--------  --coxa--  --femu--  --pate--  --tibi--\n");                       <* 
    *> yKINE_angles     (my.p_leg, &x_coxa, &x_femu, &x_pate, &x_tibi);                     <* 
    *> printf ("angles    %8.2f  %8.2f  %8.2f  %8.2f\n", x_coxa, x_femu, x_pate, x_tibi);   <* 
    *> printf ("%-4s----  --flen--  --xpos--  --zpos--  --ypos--\n", segs_name [a_seg]);    <* 
    *> yKINE_final_gk   (my.p_leg, a_seg, &x_len, &x_xpos, &x_zpos, &x_ypos);               <* 
    *> printf ("opengl    %8.2f  %8.2f  %8.2f  %8.2f\n", x_len, x_xpos, x_zpos, x_ypos);    <* 
    *> yKINE_final_fk   (my.p_leg, a_seg, &x_len, &x_xpos, &x_zpos, &x_ypos);               <* 
    *> printf ("fk        %8.2f  %8.2f  %8.2f  %8.2f\n", x_len, x_xpos, x_zpos, x_ypos);    <* 
    *> yKINE_final_fvg  (my.p_leg, a_seg, &x_len, &x_xpos, &x_zpos, &x_ypos);               <* 
    *> printf ("diff      %8.2f  %8.2f  %8.2f  %8.2f\n", x_len, x_xpos, x_zpos, x_ypos);    <* 
    *> printf ("\n");                                                                       <*/
   return 0;
}

char
KINE_line          (int a_line, char a_meth)
{
   char        x_type      [LEN_LABEL];
   double      x_deg       = 0.0;
   double      x_len       = 0.0;
   double      x_xpos      = 0.0;
   double      x_zpos      = 0.0;
   double      x_ypos      = 0.0;
   switch (a_meth) {
   case  YKINE_GK :
      fprintf (f_unit, "%-8d  %8.4f  %d  ", a_line,  my.p_cursec, (int) my.p_leg);
      strlcpy (x_type, "gk", LEN_LABEL);
      break;
   case  YKINE_FK :
      fprintf (f_unit, "                       ");
      strlcpy (x_type, "fk", LEN_LABEL);
      break;
   case  YKINE_IK :
      fprintf (f_unit, "                       ");
      strlcpy (x_type, "ik", LEN_LABEL);
      break;
   }
   /*---(coxa)---------------------------*/
   yKINE_final      ((int) my.p_leg, YKINE_COXA, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(femur)--------------------------*/
   yKINE_final      ((int) my.p_leg, YKINE_FEMU, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_unit, "%-7s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(patella)------------------------*/
   yKINE_final      ((int) my.p_leg, YKINE_PATE, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_unit, "%-7s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(tibia)--------------------------*/
   yKINE_final      ((int) my.p_leg, YKINE_TIBI, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_unit, "%-7s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(lower)--------------------------*/
   yKINE_final      ((int) my.p_leg, YKINE_TARG, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_unit, "%-7s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(lower)--------------------------*/
   yKINE_final      ((int) my.p_leg, YKINE_LOWR, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_unit, "%-7s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(wrap line)----------------------*/
   fprintf (f_unit, "\n");
   /*---(complete)-----------------------*/
   return 0;
}

char
KINE_diffmsg       (char *a_msg, int a_seg, int a_meth, double a_deg, double a_len, double a_xpos, double a_zpos, double a_ypos)
{
   /*---(locals)-------------------------*/
   char        x_msg       [LEN_LABEL];
   char        x_seg       [LEN_LABEL];
   /*---(prepare)------------------------*/
   strlcpy (x_msg, "----", LEN_LABEL);
   switch (a_seg) {
   case YKINE_COXA :  strlcpy (x_seg , "co", LEN_LABEL);   break;
   case YKINE_FEMU :  strlcpy (x_seg , "fe", LEN_LABEL);   break;
   case YKINE_PATE :  strlcpy (x_seg , "pa", LEN_LABEL);   break;
   case YKINE_TIBI :  strlcpy (x_seg , "ti", LEN_LABEL);   break;
   case YKINE_TARG :  strlcpy (x_seg , "tg", LEN_LABEL);   break;
   case YKINE_LOWR :  strlcpy (x_seg , "lw", LEN_LABEL);   break;
   default         :  strlcpy (x_seg , "--", LEN_LABEL);   break;
   }
   /*---(warnings)-----------------------*/
   if (fabs (a_deg ) >= 0.10)  strlcpy (x_msg, "warn", LEN_LABEL);
   if (fabs (a_xpos) >= 0.10)  strlcpy (x_msg, "warn", LEN_LABEL);
   if (fabs (a_zpos) >= 0.10)  strlcpy (x_msg, "warn", LEN_LABEL);
   if (fabs (a_ypos) >= 0.10)  strlcpy (x_msg, "warn", LEN_LABEL);
   /*---(bombs)--------------------------*/
   if (fabs (a_deg ) >= 1.00)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   if (fabs (a_xpos) >= 1.00)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   if (fabs (a_zpos) >= 1.00)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   if (fabs (a_ypos) >= 1.00)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   /*---(return value)-------------------*/
   if (a_msg != NULL)   sprintf (a_msg, "%s.%s", x_msg, x_seg);
   /*---(statistics)---------------------*/
   if (strcmp ("BOOM", x_msg) == 0) {
      s_error [a_seg][a_meth] += 1;
      return -1;
   }
   if (strcmp ("warn", x_msg) == 0) {
      s_warn  [a_seg][a_meth] += 1;
      return  0;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
KINE_diff          (int a_line, char a_meth)
{
   char        x_msg       [LEN_LABEL];
   double      x_deg       = 0.0;
   double      x_len       = 0.0;
   double      x_xpos      = 0.0;
   double      x_zpos      = 0.0;
   double      x_ypos      = 0.0;
   int         rc          =   0;
   fprintf (f_unit, "                       ");
   /*---(coxa)---------------------------*/
   yKINE_diff         ((int) my.p_leg, YKINE_COXA, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_COXA, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(femur)--------------------------*/
   yKINE_diff         ((int) my.p_leg, YKINE_FEMU, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_FEMU, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(patella)------------------------*/
   yKINE_diff         ((int) my.p_leg, YKINE_PATE, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_PATE, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(tibia)--------------------------*/
   yKINE_diff         ((int) my.p_leg, YKINE_TIBI, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_TIBI, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(target)-------------------------*/
   yKINE_diff         ((int) my.p_leg, YKINE_TARG, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_TARG, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(lower)--------------------------*/
   yKINE_diff         ((int) my.p_leg, YKINE_LOWR, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_LOWR, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_unit, "%-7s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(wrap line)----------------------*/
   fprintf (f_unit, "\n");
   /*---(complete)-----------------------*/
   return rc;
}

char
KINE_write         (void)
{
   static int       x_lines     =    0;
   static double    x_secs      = -1.0;
   int         rc          =   0;
   /*---(defense)------------------------*/
   if (my.p_cursec == x_secs)  return 0;
   x_secs = my.p_cursec;
   /*---(breaks)-------------------------*/
   if (x_lines % 10 == 0) {
      fprintf (f_unit, "header---------------  --coxa---------------------------------------------------  --femur--------------------------------------------------  --patella------------------------------------------------  --tibia--------------------------------------------------  --target-------------------------------------------------  --lower--------------------------------------------------\n");
      fprintf (f_unit, "--line--  --secs--  #  note---  --deg---  --len---  --xpos--  --zpos--  --ypos--  note---  --deg---  --len---  --xpos--  --zpos--  --ypos--  note---  --deg---  --len---  --xpos--  --zpos--  --ypos--  note---  --deg---  --len---  --xpos--  --zpos--  --ypos--  note---  --deg---  --len---  --xpos--  --zpos--  --ypos--  note---  --deg---  --len---  --xpos--  --zpos--  --ypos--\n");
      fprintf (f_unit, "\n");
   }
   /*---(next)---------------------------*/

   KINE_line  (x_lines, YKINE_GK);
   KINE_line  (x_lines, YKINE_FK);
   rc += KINE_diff  (x_lines, YKINE_FK);
   KINE_line  (x_lines, YKINE_IK);
   rc += KINE_diff  (x_lines, YKINE_IK);
   fprintf (f_unit, "\n");
   /*---(next)---------------------------*/
   ++x_lines;
   /*---(complete)-----------------------*/
   return 0;
}



/*> char       /+----: set all segments to defaults ------------------------------+/   <* 
 *> kine_init          (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    int      x_leg = 0;              /+ iterator         +/                         <* 
 *>    int      x_seg = 0;              /+ iterator         +/                         <* 
 *>    /+---(set body)-----------------------+/                                        <* 
 *>    kine_center       (0.0f, 0.0f);                                                 <* 
 *>    kine_height       (segs_len [YKINE_TIBI]);                                      <* 
 *>    kine_pivot        (0.0f, 0.0f);                                                 <* 
 *>    kine_attitude     (0.0f, 0.0f, 0.0f);                                           <* 
 *>    /+---(clean legs)---------------------+/                                        <* 
 *>    for (x_leg = 0; x_leg < MAX_LEGS; ++x_leg) {                                    <* 
 *>       for (x_seg = 0; x_seg < MAX_SEGS; ++x_seg) {                                 <* 
 *>          kine_clear ("gk", &gk[x_leg][x_seg], x_leg, x_seg);                       <* 
 *>          kine_clear ("fk", &fk[x_leg][x_seg], x_leg, x_seg);                       <* 
 *>          kine_clear ("ik", &ik[x_leg][x_seg], x_leg, x_seg);                       <* 
 *>       }                                                                            <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char       /+----: set segment kimematics to defaults ------------------------+/         <* 
 *> kine_clear         (char *a_name, tSEG *a_curr, int a_nleg, int a_nseg)                  <* 
 *> {                                                                                        <* 
 *>    /+---(defenses)-----------------------+/                                              <* 
 *>    if (strlen(a_name) != 2)                   return -1;                                 <* 
 *>    if (a_curr  == NULL)                       return -2;                                 <* 
 *>    if (a_nleg  <  0  || a_nleg >= MAX_LEGS)   return -3;                                 <* 
 *>    if (a_nleg  <  0  || a_nleg >= MAX_SEGS)   return -4;                                 <* 
 *>    /+---(set name)-----------------------+/                                              <* 
 *>    snprintf(a_curr->n, 11, "%s.%s.%s", legs_name [a_nleg], segs_name[a_nseg], a_name);   <* 
 *>    /+---(set lengths)--------------------+/                                              <* 
 *>    a_curr->l   = segs_len [a_nseg];                                                      <* 
 *>    a_curr->sl  = segs_len [a_nseg];                                                      <* 
 *>    a_curr->fl  =   0.0;                                                                  <* 
 *>    /+---(angles)-------------------------+/                                              <* 
 *>    if (a_nseg <= YKINE_COXA)    a_curr->d   = legs_deg [a_nleg];                         <* 
 *>    a_curr->h   =   0.0;                                                                  <* 
 *>    a_curr->v   =   0.0;                                                                  <* 
 *>    a_curr->cd  =   0.0;                                                                  <* 
 *>    a_curr->ch  =   0.0;                                                                  <* 
 *>    a_curr->cv  =   0.0;                                                                  <* 
 *>    /+---(coordinates)--------------------+/                                              <* 
 *>    a_curr->x   =   0.0;                                                                  <* 
 *>    a_curr->z   =   0.0;                                                                  <* 
 *>    a_curr->xz  =   0.0;                                                                  <* 
 *>    a_curr->y   =   0.0;                                                                  <* 
 *>    a_curr->cx  =   0.0;                                                                  <* 
 *>    a_curr->cz  =   0.0;                                                                  <* 
 *>    a_curr->cy  =   0.0;                                                                  <* 
 *>    /+---(control)------------------------+/                                              <* 
 *>    a_curr->p   =   'n';                                                                  <* 
 *>    a_curr->m   =   'i';                                                                  <* 
 *>    a_curr->c   =   'n';                                                                  <* 
 *>    /+---(complete)-----------------------+/                                              <* 
 *>    return 0;                                                                             <* 
 *> }                                                                                        <*/





/*====================------------------------------------====================*/
/*===----                        body adjustments                      ----===*/
/*====================------------------------------------====================*/
static void      o___BODY____________________o (void) {;}

char       /*----: change the center of gravity ------------------------------*/
kine_center        (double a_x, double a_z)
{
   center.bx     = a_x;
   center.bz     = a_z;
   return 0;
}

char       /*----: change the center of gravity ------------------------------*/
kine_height        (double a_y)
{
   center.by     = a_y;
   return 0;
}

char       /*----: change the pivot (center of movement) --------------------*/
kine_pivot         (double a_x, double a_z)
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
kine_attitude      (double a_roll, double a_pitch, double a_yaw)
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
