/*============================----beg-of-source---============================*/

#include "arachne.h"



/*---[[ CONSTANTS ]]----------------------------------------------------------*/
char    legs_name [YKINE_MAX_LEGS][LEN_LABEL];
char    legs_long [YKINE_MAX_LEGS][LEN_LABEL];
float   legs_deg  [YKINE_MAX_LEGS];

char    segs_name [YKINE_MAX_SEGS][LEN_LABEL];
char    segs_long [YKINE_MAX_SEGS][LEN_LABEL];
float   segs_len  [YKINE_MAX_SEGS];
float   segs_max  [YKINE_MAX_SEGS];
float   segs_min  [YKINE_MAX_SEGS];

float   segs_lnk  [YKINE_MAX_SEGS]      = {   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0  };
float   segs_act  [YKINE_MAX_SEGS]      = {   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0 ,   0.0  };


const    float    DEG2RAD   = M_PI / 180.0;
const    float    RAD2DEG   = 180.0 / M_PI;


tSEG      fk [MAX_LEGS] [MAX_SEGS];    /* opengl kinematics check   */
tSEG      gk [MAX_LEGS] [MAX_SEGS];    /* forward kinematics        */
tSEG      ik [MAX_LEGS] [MAX_SEGS];    /* inverse kinematics        */


char      umake_init    = 'n';
char      umake_leg     = 'n';


static   FILE  *f_dump  = NULL;
static   FILE  *f_cond  = NULL;
static   FILE  *f_move  = NULL;

#define     BOOM      0
#define     SHIT      1
#define     WARN      2
#define     HMMM      3

static  int   s_error [YKINE_MAX_SEGS][YKINE_MAX_METH][5];

static float     s_secs [YKINE_MAX_LEGS];


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
      s_secs    [i] = -1.0;
   }
   /*---(gather segment data)------------*/
   for (i = 0; i < YKINE_MAX_SEGS; ++i) {
      strlcpy (segs_name [i], yKINE_segfour (i), LEN_LABEL);
      strlcpy (segs_long [i], yKINE_segfull (i), LEN_LABEL);
      segs_len  [i] = yKINE_seglen (i);
      segs_min  [i] = yKINE_segmin (i);
      segs_max  [i] = yKINE_segmax (i);
      for (j = 0; j < YKINE_MAX_METH; ++j) {
         s_error   [i][j][BOOM] = 0;
         s_error   [i][j][SHIT] = 0;
         s_error   [i][j][WARN] = 0;
         s_error   [i][j][HMMM] = 0;
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
KINE_begin         (void)
{
   yKINE_init      (0);
   /*> yKINE_center    (0.0, 0.0, 0.0);                                               <*/
   KINE_load       ();
   if (my.p_dump   != '-') {
      f_dump = fopen ("arachne.dump", "w");
   }
   f_cond = fopen ("arachne.cond", "w");
   fprintf (f_cond, "#!/usr/local/bin/koios\n");
   fprintf (f_cond, "\n");
   fprintf (f_cond, "\n");
   fprintf (f_cond, "\n");
   fprintf (f_cond, "#23456789012  123  123456789-123456789-123456789-12345  123456789-123456789-123456  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  123456789-  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  -  123456789-123456789- \n");
   fprintf (f_cond, "#==(verb)===  ver  ===========(description)===========  =====(function)===========  ============================================(arguments)=============================================  ==(test)==  ==========================(results)=================================================================  t  ========(var)======= \n");
   fprintf (f_cond, "PREP          v21  include the prototype headers                                     \n");
   fprintf (f_cond, "   incl       v21  include public header                yKINE.h                    \n");
   fprintf (f_cond, "   incl       v21  include private header               yKINE_priv.h               \n");
   fprintf (f_cond, "\n");
   fprintf (f_cond, "\n");
   fprintf (f_cond, "\n");
   fprintf (f_cond, "#23456789012  123  123456789-123456789-123456789-12345  123456789-123456789-123456  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  123456789-  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  -  123456789-123456789- \n");
   fprintf (f_cond, "#==(verb)===  ver  ===========(description)===========  =====(function)===========  ============================================(arguments)=============================================  ==(test)==  ==========================(results)=================================================================  t  ========(var)======= \n");
   fprintf (f_cond, "SCRP          v21  debug arachne bombs                                                - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");
   fprintf (f_cond, "\n");
   fprintf (f_cond, "   COND       v21  initialize global data                                             - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");
   fprintf (f_cond, "     set      v21  start testing                        yKINE__unit_quiet                                                                                                                 i_equal     0                                                                                                    \n");
   fprintf (f_cond, "     exec     v21  run init                             yKINE_init                  '0'                                                                                                   i_equal     0                                                                                                    \n");
   fprintf (f_cond, "\n");
   return 0;
}

char
KINE_header        (void)
{
   fprintf (f_dump, "header------------------  --coxa------------------------------------------------------  --femur-----------------------------------------------------  --patella---------------------------------------------------  --tibia-----------------------------------------------------  --target----------------------------------------------------  --lower-----------------------------------------------------\n");
   fprintf (f_dump, "--line--  --secs--  leg-  note------  --deg---  --len---  --xpos--  --zpos--  --ypos--  note------  --deg---  --len---  --xpos--  --zpos--  --ypos--  note------  --deg---  --len---  --xpos--  --zpos--  --ypos--  note------  --deg---  --len---  --xpos--  --zpos--  --ypos--  note------  --deg---  --len---  --xpos--  --zpos--  --ypos--  note------  --deg---  --len---  --xpos--  --zpos--  --ypos--\n");
   fprintf (f_dump, "\n");
   return 0;
}

char
KINE_warns         (int a_meth)
{
   char        x_label     [ 5][LEN_LABEL] = {"BOOM", "shit", "warn", "hmmm", "----"};
   int         x_segs      [10]            = { YKINE_COXA, YKINE_FEMU, YKINE_PATE, YKINE_TIBI,  YKINE_TARG, YKINE_LOWR };
   int         i           = 0;
   int         j           = 0;
   fprintf (f_dump, "                          ");
   for (i = 0; i < 6; ++i) {
      fprintf (f_dump, "%s %s : ", (a_meth == YKINE_FK) ? "FK" : "IK", segs_name [x_segs[i]]);
      for (j = 0; j < 4; ++j) {
         fprintf (f_dump, "%s=%-5d ", x_label [j], s_error [x_segs [i]][a_meth][j]);
      }
      fprintf (f_dump, "        ");
   }
   fprintf (f_dump, "\n");
}

char
KINE_end           (void)
{
   fprintf (f_cond, "\n");
   fprintf (f_cond, "#---end-of-unit-test-script\n");
   fclose  (f_cond);
   if (my.p_dump   != '-') {
      fprintf (f_dump, "\n");
      KINE_header ();
      KINE_warns  (YKINE_FK);
      KINE_warns  (YKINE_IK);
      fprintf (f_dump, "\n");
      fclose  (f_dump);
   }
   return 0;
}

char
KINE_compare       (int a_seg)
{
   /*> float       x_len       = 0.0;                                                       <* 
    *> float       x_xpos      = 0.0;                                                       <* 
    *> float       x_zpos      = 0.0;                                                       <* 
    *> float       x_ypos      = 0.0;                                                       <* 
    *> float       x_coxa      = 0.0;                                                       <* 
    *> float       x_femu      = 0.0;                                                       <* 
    *> float       x_pate      = 0.0;                                                       <* 
    *> float       x_tibi      = 0.0;                                                       <* 
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
KINE_line          (int a_line, char a_meth, int a_leg)
{
   char        x_type      [LEN_LABEL];
   float       x_deg       = 0.0;
   float       x_len       = 0.0;
   float       x_xpos      = 0.0;
   float       x_zpos      = 0.0;
   float       x_ypos      = 0.0;
   switch (a_meth) {
   case  YKINE_GK :
      fprintf (f_dump, "%-8d  %8.4f  %d/%s  ", a_line,  my.p_cur, a_leg, legs_name [a_leg]);
      strlcpy (x_type, "gk", LEN_LABEL);
      break;
   case  YKINE_FK :
      fprintf (f_dump, "                          ");
      strlcpy (x_type, "fk", LEN_LABEL);
      break;
   case  YKINE_IK :
      fprintf (f_dump, "                          ");
      strlcpy (x_type, "ik", LEN_LABEL);
      break;
   }
   /*---(coxa)---------------------------*/
   yKINE_endpoint   (a_leg, YKINE_COXA, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(femur)--------------------------*/
   yKINE_endpoint   (a_leg, YKINE_FEMU, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_dump, "%-10s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(patella)------------------------*/
   yKINE_endpoint   (a_leg, YKINE_PATE, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_dump, "%-10s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(tibia)--------------------------*/
   yKINE_endpoint   (a_leg, YKINE_TIBI, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_dump, "%-10s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(lower)--------------------------*/
   yKINE_endpoint   (a_leg, YKINE_TARG, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_dump, "%-10s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(lower)--------------------------*/
   yKINE_endpoint   (a_leg, YKINE_LOWR, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   if (x_xpos + x_zpos + x_ypos == 0.0)  fprintf (f_dump, "%-10s     - - -     - - -     - - -     - - -     - - -  ", x_type);
   else                                  fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_type, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(wrap line)----------------------*/
   fprintf (f_dump, "\n");
   /*---(complete)-----------------------*/
   return 0;
}

char
KINE_diffmsg       (char *a_msg, int a_seg, int a_meth, float  a_deg, float  a_len, float  a_xpos, float  a_zpos, float  a_ypos)
{
   /*---(locals)-------------------------*/
   char        x_meth      [LEN_LABEL];
   char        x_msg       [LEN_LABEL];
   char        x_seg       [LEN_LABEL];
   float       x_forgive   = 0.0;
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
   switch (a_meth) {
   case YKINE_FK   :  strlcpy (x_meth, "FK" , LEN_LABEL);   break;
   case YKINE_IK   :  strlcpy (x_meth, "IK" , LEN_LABEL);   break;
   }
   /*---(warnings)-----------------------*/
   if (a_meth == YKINE_FK)   x_forgive = 0.01;
   else                      x_forgive = 0.10;
   if (fabs (a_deg ) >= x_forgive)  strlcpy (x_msg, "hmmm", LEN_LABEL);
   if (fabs (a_xpos) >= x_forgive)  strlcpy (x_msg, "hmmm", LEN_LABEL);
   if (fabs (a_zpos) >= x_forgive)  strlcpy (x_msg, "hmmm", LEN_LABEL);
   if (fabs (a_ypos) >= x_forgive)  strlcpy (x_msg, "hmmm", LEN_LABEL);
   /*---(warnings)-----------------------*/
   if (a_meth == YKINE_FK)   x_forgive = 0.05;
   else                      x_forgive = 0.50;
   if (fabs (a_deg ) >= x_forgive)  strlcpy (x_msg, "warn", LEN_LABEL);
   if (fabs (a_xpos) >= x_forgive)  strlcpy (x_msg, "warn", LEN_LABEL);
   if (fabs (a_zpos) >= x_forgive)  strlcpy (x_msg, "warn", LEN_LABEL);
   if (fabs (a_ypos) >= x_forgive)  strlcpy (x_msg, "warn", LEN_LABEL);
   /*---(bombs)--------------------------*/
   if (a_meth == YKINE_FK)   x_forgive = 0.20;
   else                      x_forgive = 2.00;
   if (fabs (a_deg ) >= x_forgive)  strlcpy (x_msg, "shit", LEN_LABEL);
   if (fabs (a_xpos) >= x_forgive)  strlcpy (x_msg, "shit", LEN_LABEL);
   if (fabs (a_zpos) >= x_forgive)  strlcpy (x_msg, "shit", LEN_LABEL);
   if (fabs (a_ypos) >= x_forgive)  strlcpy (x_msg, "shit", LEN_LABEL);
   /*---(bombs)--------------------------*/
   if (a_meth == YKINE_FK)   x_forgive = 1.00;
   else                      x_forgive = 9.00;
   if (fabs (a_deg ) >= x_forgive)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   if (fabs (a_xpos) >= x_forgive)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   if (fabs (a_zpos) >= x_forgive)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   if (fabs (a_ypos) >= x_forgive)  strlcpy (x_msg, "BOOM", LEN_LABEL);
   /*---(return value)-------------------*/
   if (a_msg != NULL) {
      if (strcmp (x_msg, "----") == 0) strlcpy (a_msg, x_msg, LEN_LABEL);
      else                             sprintf (a_msg, "%s.%s.%s", x_meth, x_msg, x_seg);
   }
   /*---(statistics)---------------------*/
   if (strcmp ("BOOM", x_msg) == 0) {
      s_error [a_seg][a_meth][0] += 1;
      return -1;
   }
   if (strcmp ("shit", x_msg) == 0) {
      s_error [a_seg][a_meth][1] += 1;
      return  0;
   }
   if (strcmp ("warn", x_msg) == 0) {
      s_error [a_seg][a_meth][2] += 1;
      return -1;
   }
   if (strcmp ("hmmm", x_msg) == 0) {
      s_error [a_seg][a_meth][3] += 1;
      return  0;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
KINE_diff          (int a_line, char a_meth, int a_leg)
{
   char        x_msg       [LEN_LABEL];
   float       x_deg       = 0.0;
   float       x_len       = 0.0;
   float       x_xpos      = 0.0;
   float       x_zpos      = 0.0;
   float       x_ypos      = 0.0;
   int         rc          =   0;
   fprintf (f_dump, "                          ");
   /*---(coxa)---------------------------*/
   yKINE_enddiff      (a_leg, YKINE_COXA, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_COXA, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(femur)--------------------------*/
   yKINE_enddiff      (a_leg, YKINE_FEMU, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_FEMU, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(patella)------------------------*/
   yKINE_enddiff      (a_leg, YKINE_PATE, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_PATE, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(tibia)--------------------------*/
   yKINE_enddiff      (a_leg, YKINE_TIBI, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_TIBI, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(target)-------------------------*/
   yKINE_enddiff      (a_leg, YKINE_TARG, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_TARG, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(lower)--------------------------*/
   yKINE_enddiff      (a_leg, YKINE_LOWR, a_meth, &x_deg, &x_len, &x_xpos, &x_zpos, &x_ypos);
   rc += KINE_diffmsg (x_msg         , YKINE_LOWR, a_meth,  x_deg,  x_len,  x_xpos,  x_zpos,  x_ypos);
   fprintf (f_dump, "%-10s  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f  ", x_msg, x_deg, x_len, x_xpos, x_zpos, x_ypos);
   /*---(wrap line)----------------------*/
   fprintf (f_dump, "\n");
   /*---(complete)-----------------------*/
   return rc;
}

char
KINE_write         (int a_leg)
{
   static int       x_lines                     =    0;
   int         rc          =   0;
   /*---(defense)------------------------*/
   if (my.p_dump   == '-')                          return 0;
   if (my.p_dump   == 'c' && a_leg != my.p_leg)     return 0;
   if (my.p_cur <= s_secs [a_leg])               return 0;
   /*---(update)-------------------------*/
   s_secs [a_leg] = my.p_cur;
   /*---(breaks)-------------------------*/
   if (x_lines % 10 == 0)   KINE_header  ();
   /*---(next)---------------------------*/
   KINE_line  (x_lines, YKINE_GK, a_leg);
   KINE_line  (x_lines, YKINE_FK, a_leg);
   rc += KINE_diff  (x_lines, YKINE_FK, a_leg);
   KINE_line  (x_lines, YKINE_IK, a_leg);
   rc += KINE_diff  (x_lines, YKINE_IK, a_leg);
   fprintf (f_dump, "\n");
   /*---(next)---------------------------*/
   ++x_lines;
   /*---(condition)----------------------*/
   /*> KINE_unitcond ();                                                              <*/
   /*---(complete)-----------------------*/
   return 0;
}

char
KINE_unitseg       (char *a_leg, int a_seg, int a_meth)
{  /*---(design notes)-------------------*/
   /*
    *   always request opengl actuals from yKINE as the "true" test
    *
    *
    */
   int         i           = 0.0;   /* iterator   */
   float       l           = 0.0;   /* length     */
   float       d           = 0.0;   /* degrees    */
   float       x           = 0.0;   /* x pos      */
   float       z           = 0.0;   /* z pos      */
   float       y           = 0.0;   /* y pos      */
   float       v           = 0.0;   /* vert rads  */
   float       h           = 0.0;   /* horz rads  */
   char        x_seg       [LEN_LABEL];
   char        x_segupper  [LEN_LABEL];
   char        x_full      [LEN_LABEL];
   char        x_meth      [LEN_LABEL];
   int         x_forgive   = 1;
   strlcpy  (x_seg     , segs_name [a_seg], LEN_LABEL);
   strlcpy  (x_segupper, segs_name [a_seg], LEN_LABEL);
   for (i = 0; i < 4; ++i)  x_segupper [i] = toupper (x_segupper [i]);
   strlcpy  (x_full, segs_long [a_seg], LEN_LABEL);
   if (a_meth == YKINE_FK) {
      strlcpy  (x_meth, "FK", LEN_LABEL);
      x_forgive = 1;
   } else { 
      strlcpy  (x_meth, "IK", LEN_LABEL);
      x_forgive = 3;
   }
   yKINE_angle     (my.p_leg, a_seg, YKINE_GK, &d, &l, &v, &h);       
   yKINE_endpoint  (my.p_leg, a_seg, YKINE_GK, &d, &l, &x, &z, &y);
   /*---(same for both)------------------*/
   if (a_seg <= YKINE_TROC || (a_seg >= YKINE_META && a_seg <= YKINE_FOOT)) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s                                                                                              i_equal     0                                                                                                    \n" , x_meth, a_leg, x_full, x_seg, a_leg);
   }
   /*---(forward special)----------------*/
   else if (a_seg <= YKINE_TIBI && a_meth == YKINE_FK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s       , %8.3lf       , YKINE_%s                                                            i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, d, x_meth);
   }
   else if (a_seg == YKINE_TARG && a_meth == YKINE_FK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__FK_%s              YKINE_%s       , YKINE_%s                                                                             i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x_meth);
   }
   else if (a_seg >  YKINE_TIBI && a_meth == YKINE_FK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s       , YKINE_%s                                                                             i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x_meth);
   }
   /*---(inverse special)----------------*/
   else if (a_seg <= YKINE_TIBI && a_meth == YKINE_IK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__IK_%s              YKINE_%s                                                                                              i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg);
   }
   else if (a_seg == YKINE_LOWR && a_meth == YKINE_IK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s       , YKINE_%s                                                                             i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x_meth);
   }
   else if (a_seg == YKINE_TARG && a_meth == YKINE_IK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__IK_%s              YKINE_%s       , %8.3lf       , %8.3lf       , %8.3lf                                           i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x, z, y);
   }
   fprintf (f_cond, "     get      v21  -- verify %s angles                  yKINE__getter               \"%s_seg_angle\" , YKINE_%s       , YKINE_%s                                                          u_round/%d   %s-%s.%s ang :%8.1lfd,%8.3lfv,%8.3lfh                                                        \n" , x_meth, x_meth, a_leg, x_segupper, x_forgive, x_meth, a_leg, x_seg, d, v, h);
   yKINE_segment   (my.p_leg, a_seg, YKINE_GK, &d, &l, &x, &z, &y);
   fprintf (f_cond, "     get      v21  -- verify %s segment                 yKINE__getter               \"%s_seg_size\"  , YKINE_%s       , YKINE_%s                                                          u_round/%d   %s-%s.%s siz :%8.1lfm,%8.1lfx,%8.1lfz,%8.1lfy                                              \n"   , x_meth, x_meth, a_leg, x_segupper, x_forgive, x_meth, a_leg, x_seg, l, x, z, y);
   yKINE_endpoint  (my.p_leg, a_seg, YKINE_GK, &d, &l, &x, &z, &y);
   fprintf (f_cond, "     get      v21  -- verify %s endpoint                yKINE__getter               \"%s_seg_end\"   , YKINE_%s       , YKINE_%s                                                          u_round/%d   %s-%s.%s end :%8.1lfm,%8.1lfx,%8.1lfz,%8.1lfy                                              \n"   , x_meth, x_meth, a_leg, x_segupper, x_forgive, x_meth, a_leg, x_seg, l, x, z, y);
   return 0;
}

char
KINE_unitcond      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_leg       [LEN_LABEL];
   /*---(prepare common vars)------------*/
   strlcpy  (x_leg, legs_name [(int) my.p_leg], LEN_LABEL);
   /*---(prepare for testing)------------*/
   fprintf (f_cond, "   GROUP      v21  automated test from arachne                                        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n", my.p_cur);
   fprintf (f_cond, "\n");
   fprintf (f_cond, "   COND       v21  forward kinematics (FK)                                            - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n", x_leg);
   /*---(forward kinematics)-------------*/
   fprintf (f_cond, "     exec     v21  wipe all calculated values           yKINE__wipe                 YKINE_%s       , YKINE_FK                                                                             i_equal     0                                                                      \n", x_leg);
   KINE_unitseg  (x_leg, YKINE_THOR, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_COXA, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_TROC, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_FEMU, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_PATE, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_TIBI, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_TARG, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_LOWR, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_META, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_TARS, YKINE_FK);
   KINE_unitseg  (x_leg, YKINE_FOOT, YKINE_FK);
   /*---(inverse kinematics)-------------*/
   fprintf (f_cond, "\n");
   fprintf (f_cond, "   COND       v21  inverse kinematics (IK)                                            - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n", x_leg);
   fprintf (f_cond, "     exec     v21  wipe all calculated values           yKINE__wipe                 YKINE_%s       , YKINE_FK                                                                             i_equal     0                                                                      \n", x_leg);
   KINE_unitseg  (x_leg, YKINE_TARG, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_THOR, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_COXA, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_TROC, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_FEMU, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_LOWR, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_PATE, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_TIBI, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_META, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_TARS, YKINE_IK);
   KINE_unitseg  (x_leg, YKINE_FOOT, YKINE_IK);
   /*---(add some space)-----------------*/
   fprintf (f_cond, "\n");
   fflush  (f_cond);
   /*---(complete)-----------------------*/
   return 0;
}


char
KINE_unitseg       (char *a_leg, int a_seg, int a_meth)
{  /*---(design notes)-------------------*/
   /*
    *   always request opengl actuals from yKINE as the "true" test
    *
    *
    */
   int         i           = 0.0;   /* iterator   */
   float       l           = 0.0;   /* length     */
   float       d           = 0.0;   /* degrees    */
   float       x           = 0.0;   /* x pos      */
   float       z           = 0.0;   /* z pos      */
   float       y           = 0.0;   /* y pos      */
   float       v           = 0.0;   /* vert rads  */
   float       h           = 0.0;   /* horz rads  */
   char        x_seg       [LEN_LABEL];
   char        x_segupper  [LEN_LABEL];
   char        x_full      [LEN_LABEL];
   char        x_meth      [LEN_LABEL];
   int         x_forgive   = 1;
   strlcpy  (x_seg     , segs_name [a_seg], LEN_LABEL);
   strlcpy  (x_segupper, segs_name [a_seg], LEN_LABEL);
   for (i = 0; i < 4; ++i)  x_segupper [i] = toupper (x_segupper [i]);
   strlcpy  (x_full, segs_long [a_seg], LEN_LABEL);
   if (a_meth == YKINE_FK) {
      strlcpy  (x_meth, "FK", LEN_LABEL);
      x_forgive = 1;
   } else { 
      strlcpy  (x_meth, "IK", LEN_LABEL);
      x_forgive = 3;
   }
   yKINE_angle     (my.p_leg, a_seg, YKINE_GK, &d, &l, &v, &h);       
   yKINE_endpoint  (my.p_leg, a_seg, YKINE_GK, &d, &l, &x, &z, &y);
   /*---(same for both)------------------*/
   if (a_seg <= YKINE_TROC || (a_seg >= YKINE_META && a_seg <= YKINE_FOOT)) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s                                                                                              i_equal     0                                                                                                    \n" , x_meth, a_leg, x_full, x_seg, a_leg);
   }
   /*---(forward special)----------------*/
   else if (a_seg <= YKINE_TIBI && a_meth == YKINE_FK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s       , %8.3lf       , YKINE_%s                                                            i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, d, x_meth);
   }
   else if (a_seg == YKINE_TARG && a_meth == YKINE_FK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__FK_%s              YKINE_%s       , YKINE_%s                                                                             i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x_meth);
   }
   else if (a_seg >  YKINE_TIBI && a_meth == YKINE_FK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s       , YKINE_%s                                                                             i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x_meth);
   }
   /*---(inverse special)----------------*/
   else if (a_seg <= YKINE_TIBI && a_meth == YKINE_IK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__IK_%s              YKINE_%s                                                                                              i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg);
   }
   else if (a_seg == YKINE_LOWR && a_meth == YKINE_IK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__%s                 YKINE_%s       , YKINE_%s                                                                             i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x_meth);
   }
   else if (a_seg == YKINE_TARG && a_meth == YKINE_IK) {
      fprintf (f_cond, "     exec     v21  calc %s on %s %-14s         yKINE__IK_%s              YKINE_%s       , %8.3lf       , %8.3lf       , %8.3lf                                           i_equal     0                                                                                                    \n"   , x_meth, a_leg, x_full, x_seg, a_leg, x, z, y);
   }
   fprintf (f_cond, "     get      v21  -- verify %s angles                  yKINE__getter               \"%s_seg_angle\" , YKINE_%s       , YKINE_%s                                                          u_round/%d   %s-%s.%s ang :%8.1lfd,%8.3lfv,%8.3lfh                                                        \n" , x_meth, x_meth, a_leg, x_segupper, x_forgive, x_meth, a_leg, x_seg, d, v, h);
   yKINE_segment   (my.p_leg, a_seg, YKINE_GK, &d, &l, &x, &z, &y);
   fprintf (f_cond, "     get      v21  -- verify %s segment                 yKINE__getter               \"%s_seg_size\"  , YKINE_%s       , YKINE_%s                                                          u_round/%d   %s-%s.%s siz :%8.1lfm,%8.1lfx,%8.1lfz,%8.1lfy                                              \n"   , x_meth, x_meth, a_leg, x_segupper, x_forgive, x_meth, a_leg, x_seg, l, x, z, y);
   yKINE_endpoint  (my.p_leg, a_seg, YKINE_GK, &d, &l, &x, &z, &y);
   fprintf (f_cond, "     get      v21  -- verify %s endpoint                yKINE__getter               \"%s_seg_end\"   , YKINE_%s       , YKINE_%s                                                          u_round/%d   %s-%s.%s end :%8.1lfm,%8.1lfx,%8.1lfz,%8.1lfy                                              \n"   , x_meth, x_meth, a_leg, x_segupper, x_forgive, x_meth, a_leg, x_seg, l, x, z, y);
   return 0;
}



char
KINE_unitcond_ik   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_leg       [LEN_LABEL];
   /*---(prepare common vars)------------*/
   strlcpy  (x_leg, legs_name [(int) my.p_leg], LEN_LABEL);
   /*---(prepare for testing)------------*/
   fprintf (f_cond, "   COND       v21  inverse kinematics (IK)                                            - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n", x_leg);
   fprintf (f_cond, "     exec     v21  wipe all calculated values           yKINE__wipe                 YKINE_%s       , YKINE_FK                                                                             i_equal     0                                                                      \n", x_leg);
   KINE_unitseg_ik  (x_leg, YKINE_TARG, YKINE_IK);
   KINE_unitseg_ik  (x_leg, YKINE_FEMU, YKINE_IK);
   KINE_unitseg_ik  (x_leg, YKINE_LOWR, YKINE_IK);
   KINE_unitseg_ik  (x_leg, YKINE_PATE, YKINE_IK);
   KINE_unitseg_ik  (x_leg, YKINE_TIBI, YKINE_IK);
   /*---(add some space)-----------------*/
   fprintf (f_cond, "\n");
   fflush  (f_cond);
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
kine_center        (float  a_x, float  a_z)
{
   center.bx     = a_x;
   center.bz     = a_z;
   return 0;
}

char       /*----: change the center of gravity ------------------------------*/
kine_height        (float  a_y)
{
   center.by     = a_y;
   return 0;
}

char       /*----: change the pivot (center of movement) --------------------*/
kine_pivot         (float  a_x, float  a_z)
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
kine_attitude      (float  a_roll, float  a_pitch, float  a_yaw)
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



/*====================------------------------------------====================*/
/*===----                  move unit testing helper                    ----===*/
/*====================------------------------------------====================*/
static void      o___MOVE_UNIT_______________o (void) {;}

/*> char                                                                                                                                                                                                                                                                                                                                                                      <* 
 *> MOVE_begin         (void)                                                                                                                                                                                                                                                                                                                                                 <* 
 *> {                                                                                                                                                                                                                                                                                                                                                                         <* 
 *>    f_move = fopen ("arachne.move", "w");                                                                                                                                                                                                                                                                                                                                  <* 
 *>                                                                                                                                                                                                                                                                                                                                                                           <* 
 *>    fprintf (f_cond, "   COND       v21  request a specific position                                        - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");                             <* 
 *>    fprintf (f_cond, "     exec     v21  clear all marks                      ykine__scrp_prep                                                                                                                  i_equal                                                                                                          \n");                              <* 
 *>    fprintf (f_cond, "     load     v21  add move request to stdin            stdin                       -15.15s � -a- �                                                                                         i_equal                                                                                                          \n");                            <* 
 *>      load     v21  add a line to stdin                  stdin                       fk_pure    � -a- � AR        �    0.500 �    45.00 �    30.00 �   -25.00 � -                    ��                                                                                                                                                                               <* 
 *>      exec     v21  read and parse                       ykine_parse                                                                                                                       i_equal     0                                                                                                                                                            <* 
 *>      exec     v21  run fk script handler                ykine_scrp_fk                                                                                                                     i_equal     0                                                                                                                                                            <* 
 *>      get      v21  check sides                          ykine__unit_servo           "sides"                                                                                               s_equal     SERVO sides    : 1 sides=L    , 1 ranks=R                                                                                                                    <* 
 *>      get      v21  check servos                         ykine__unit_servo           "servos"                                                                                              s_equal     SERVO servos   : ___ ___ ___ ___ ___ f__ ___ ___ ___ ___ ___ _                                                                                               <* 
 *>      get      v21  check move counts                    ykine__unit_move            "counts"       , -1      , -1        , -1                                                             s_equal     MOVE counts    : 000 000 000 000 000 111 000 000 000 000 000 0                                                                                               <* 
 *>                                                                                                                                                                                                                                                                                                                                                                           <* 
 *>    COND       v21  check the saved data                                               - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   ((01.002))  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -                                                         <* 
 *>      get      v21  check expected header                ykine__unit_move            "header"       , YKINE_LR, YKINE_FEMU,  0                                                             s_equal     MOVE header    :  1/ 1 s fk_pure       0.000    0.500    0.500                                                                                               <* 
 *>      get      v21  check expected detail                ykine__unit_move            "detail"       , YKINE_LR, YKINE_FEMU,  0                                                             u_round/3   MOVE detail    :      0.0b      0.0e    -92.5x    160.2z      0.0y                                                                                           <* 
 *>      get      v21  check next header is empty           ykine__unit_move            "header"       , YKINE_LR, YKINE_FEMU,  1                                                             s_equal     MOVE unit      : past end of moves                                                                                                                           <* 
 *>      get      v21  check expected header                ykine__unit_move            "header"       , YKINE_LR, YKINE_PATE,  0                                                             s_equal     MOVE header    :  1/ 1 s fk_pure       0.000    0.500    0.500                                                                                               <* 
 *>      get      v21  check expected detail                ykine__unit_move            "detail"       , YKINE_LR, YKINE_PATE,  0                                                             u_round/3   MOVE detail    :      0.0b      0.0e   -121.0x    209.6z      0.0y                                                                                           <* 
 *>      get      v21  check next header is empty           ykine__unit_move            "header"       , YKINE_LR, YKINE_PATE,  1                                                             s_equal     MOVE unit      : past end of moves                                                                                                                           <* 
 *>      get      v21  check expected header                ykine__unit_move            "header"       , YKINE_LR, YKINE_TIBI,  0                                                             s_equal     MOVE header    :  1/ 1 s fk_pure       0.000    0.500    0.500                                                                                               <* 
 *>      get      v21  check expected detail                ykine__unit_move            "detail"       , YKINE_LR, YKINE_TIBI,  0                                                             u_round/3   MOVE detail    :      0.0b      0.0e   -121.0x    209.6z   -130.0y                                                                                           <* 
 *>      get      v21  check next header is empty           ykine__unit_move            "header"       , YKINE_LR, YKINE_TIBI,  1                                                             s_equal     MOVE unit      : past end of moves                                                                                                                           <* 
 *>    fprintf (f_cond, "#!/usr/local/bin/koios\n");                                                                                                                                                                                                                                                                                                                          <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    fprintf (f_cond, "#23456789012  123  123456789-123456789-123456789-12345  123456789-123456789-123456  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  123456789-  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  -  123456789-123456789- \n");   <* 
 *>    fprintf (f_cond, "#==(verb)===  ver  ===========(description)===========  =====(function)===========  ============================================(arguments)=============================================  ==(test)==  ==========================(results)=================================================================  t  ========(var)======= \n");   <* 
 *>    fprintf (f_cond, "PREP          v21  include the prototype headers                                     \n");                                                                                                                                                                                                                                                        <* 
 *>    fprintf (f_cond, "   incl       v21  include public header                yKINE.h                    \n");                                                                                                                                                                                                                                                         <* 
 *>    fprintf (f_cond, "   incl       v21  include private header               yKINE_priv.h               \n");                                                                                                                                                                                                                                                         <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    fprintf (f_cond, "#23456789012  123  123456789-123456789-123456789-12345  123456789-123456789-123456  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  123456789-  123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  -  123456789-123456789- \n");   <* 
 *>    fprintf (f_cond, "#==(verb)===  ver  ===========(description)===========  =====(function)===========  ============================================(arguments)=============================================  ==(test)==  ==========================(results)=================================================================  t  ========(var)======= \n");   <* 
 *>    fprintf (f_cond, "SCRP          v21  debug arachne bombs                                                - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");                             <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    fprintf (f_cond, "   COND       v21  initialize global data                                             - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   - - - - -   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  \n");                             <* 
 *>    fprintf (f_cond, "     set      v21  start testing                        yKINE__unit_quiet                                                                                                                 i_equal     0                                                                                                    \n");                              <* 
 *>    fprintf (f_cond, "     exec     v21  run init                             yKINE_init                  '0'                                                                                                   i_equal     0                                                                                                    \n");                              <* 
 *>    fprintf (f_cond, "\n");                                                                                                                                                                                                                                                                                                                                                <* 
 *>    return 0;                                                                                                                                                                                                                                                                                                                                                              <* 
 *> }                                                                                                                                                                                                                                                                                                                                                                         <*/



/*============================----end-of-source---============================*/
