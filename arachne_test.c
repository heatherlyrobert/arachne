/*============================----beg-of-source---============================*/

#include "arachne.h"



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];



char*      /*----: unit testing accessor for clean validation interface ------*/
unit_accessor      (char *a_question, int a_leg, int a_seg)
{
   /*---(defense)-----------------------------------------*/
   if (a_leg < 0 || a_leg > MAX_LEGS) {
      sprintf(unit_answer, "leg value out of range (0 - %d), not %d", a_leg, MAX_LEGS - 1);
      return unit_answer;
   }
   if (a_seg < 0 || a_seg > MAX_SEGS) {
      sprintf(unit_answer, "segment value out of range (0 - %d), not %d", a_seg, MAX_SEGS - 1);
      return unit_answer;
   }
   /*---(answer)------------------------------------------*/
   strncpy(unit_answer, "unknown request", 100);
   if (strcmp(a_question, "IK_final") == 0) {
      sprintf(unit_answer, "IK %1d/final     : %8.2fx,%8.2fz,%8.2fy", 
            a_leg, ik[a_leg][YKINE_CALC].x, ik[a_leg][YKINE_CALC].z, ik[a_leg][YKINE_CALC].y);
   } else if (strcmp(a_question, "FK_final") == 0) {
      sprintf(unit_answer, "FK %1d/final     : %8.2fx,%8.2fz,%8.2fy", 
            a_leg, fk[a_leg][YKINE_CALC].x, fk[a_leg][YKINE_CALC].z, fk[a_leg][YKINE_CALC].y);
   } else if (strcmp(a_question, "IK_target") == 0) {
      sprintf(unit_answer, "IK %1d/target    : %8.2fx,%8.2fz,%8.2fy", 
            a_leg, ik[a_leg][YKINE_TARG].x, ik[a_leg][YKINE_TARG].z, ik[a_leg][YKINE_TARG].y);
   } else if (strcmp(a_question, "IK_match") == 0) {
      if (  ik[a_leg][YKINE_CALC].x == ik[a_leg][YKINE_TARG].x &&
            ik[a_leg][YKINE_CALC].z == ik[a_leg][YKINE_TARG].z &&
            ik[a_leg][YKINE_CALC].y == ik[a_leg][YKINE_TARG].y )
         sprintf(unit_answer, "IK %1d/success", a_leg);
      else
         sprintf(unit_answer, "IK %1d/FAILURE", a_leg);
   } else if (strcmp(a_question, "IK_angles_OLD") == 0) {
      sprintf(unit_answer, "IK %1d/angles    : %8.2fc,%8.2ff,%8.2fp,%8.2ft", 
            a_leg, ik[a_leg][YKINE_COXA].d, ik[a_leg][YKINE_FEMU].d,
            ik[a_leg][YKINE_PATE].d, ik[a_leg][YKINE_TIBI].d);
   } else if (strcmp(a_question, "IK_lower")  == 0) {
      sprintf(unit_answer, "IK %1d/lower     : %8.3fp,%8.3ft", 
            a_leg, ik[a_leg][YKINE_PATE].v, ik[a_leg][YKINE_TIBI].v);
   } else if (strcmp(a_question, "final")     == 0) {
      sprintf(unit_answer, "%1d/%1d %4.4s final  %8.1fl,%8.1fx,%8.1fz,%8.1fy", 
            a_leg, a_seg, segs_long [a_seg], fk[a_leg][a_seg].l,
            fk[a_leg][a_seg].x, fk[a_leg][a_seg].z, fk[a_leg][a_seg].y);
   }
   /*---(leg values)--------------------------------------*/
   else if (strcmp(a_question, "FK_angles"     ) == 0) {
      sprintf(unit_answer, "%-10.10s deg :%8.1fm,%8.1fd,%8.3fv,%8.3fh", 
            fk[a_leg][a_seg].n , fk[a_leg][a_seg].l,
            fk[a_leg][a_seg].d , fk[a_leg][a_seg].cv, fk[a_leg][a_seg].ch);
   }
   else if (strcmp(a_question, "FK_segment"    ) == 0) {
      sprintf(unit_answer, "%-10.10s seg :%8.1fm,%8.1fx,%8.1fz,%8.1fy", 
            fk[a_leg][a_seg].n , fk[a_leg][a_seg].l,
            fk[a_leg][a_seg].x , fk[a_leg][a_seg].z, fk[a_leg][a_seg].y);
   }
   else if (strcmp(a_question, "FK_endpoint"   ) == 0) {
      sprintf(unit_answer, "%-10.10s end :%8.1fm,%8.1fx,%8.1fz,%8.1fy", 
            fk[a_leg][a_seg].n , fk[a_leg][a_seg].l,
            fk[a_leg][a_seg].cx, fk[a_leg][a_seg].cz, fk[a_leg][a_seg].cy);
   }
   else if (strcmp(a_question, "IK_angles"     ) == 0) {
      sprintf(unit_answer, "%-10.10s deg :%8.1fm,%8.1fd,%8.3fv,%8.3fh", 
            ik[a_leg][a_seg].n , ik[a_leg][a_seg].l,
            ik[a_leg][a_seg].d , ik[a_leg][a_seg].cv, ik[a_leg][a_seg].ch);
   }
   else if (strcmp(a_question, "IK_segment"    ) == 0) {
      sprintf(unit_answer, "%-10.10s seg :%8.1fm,%8.1fx,%8.1fz,%8.1fy", 
            ik[a_leg][a_seg].n , ik[a_leg][a_seg].l,
            ik[a_leg][a_seg].x , ik[a_leg][a_seg].z, ik[a_leg][a_seg].y);
   }
   else if (strcmp(a_question, "IK_endpoint"   ) == 0) {
      sprintf(unit_answer, "%-10.10s end :%8.1fm,%8.1fx,%8.1fz,%8.1fy", 
            ik[a_leg][a_seg].n , ik[a_leg][a_seg].l,
            ik[a_leg][a_seg].cx, ik[a_leg][a_seg].cz, ik[a_leg][a_seg].cy);
   }
   /*---(complete)----------------------------------------*/
   return unit_answer;
}


/*============================----end-of-source---============================*/
