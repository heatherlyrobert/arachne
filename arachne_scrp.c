/*============================----beg-of-source---============================*/

#include  "arachne.h"


tGAIT     gait;


/*====================------------------------------------====================*/
/*===----                           utilities                          ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char*      /* ---- : clean whitespace from both sides of a string -----------*/
str_trim           (char *a_cstring)
{
   /*---(defenses)-------------------------*/
   if (a_cstring == NULL) return NULL;
   /*---(locals)---------------------------*/
   int i, j;
   int len = 0;
   /*---(leading whitespace)---------------*/
   len = strlen(a_cstring);
   for (i = 0; i <= len; ++i) {
      if (a_cstring[0] != ' ') break;
      for (j = 0; j <= len; ++j)
         a_cstring[j] = a_cstring[j + 1];
      --len;
   }
   /*---(trailing whitespace)--------------*/
   len = strlen(a_cstring);
   for (i = len - 1; i >= 0; --i) {
      if (a_cstring[i] != ' ') break;
      a_cstring[i] = '\0';
   }
   /*---(complete)-------------------------*/
   return a_cstring;
}

PRIV char  /* ---- : write a step record -------------------------------------*/
scrp__step         (int  a_index)
{
   printf ("\n\n");
   printf ("#leg  ---femu---  ---pate---  ---tibi---  ---pace---  ---human-notes----------------\n");
   printf ("STEP %6d\n", a_index);
   gait.touch [a_index] = 0;
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char  /* ---- : write a leg record --------------------------------------*/
scrp__leg          (int  a_index, float a_dur)
{
   /*---(identify leg)-------------------*/
   printf("%1d    ", a_index);
   /*---(joint angles)-------------------*/
   printf("%10.3f  %10.3f  %10.3f",
         gk[a_index][FEMU].d, gk[a_index][PATE].d, gk[a_index][TIBI].d);
   /*---(duration)-----------------------*/
   printf("  %10.3f", a_dur);
   /*---(notes)--------------------------*/
   printf ("      ");
   switch (a_index) {
   case 0: printf ("RR");   break;
   case 1: printf ("RM");   break;
   case 2: printf ("RF");   break;
   case 3: printf ("LF");   break;
   case 4: printf ("LM");   break;
   case 5: printf ("LR");   break;
   }
   /*---(finish)-------------------------*/
   printf ("   %8.3f, %8.3f", center.by, ik[a_index][TIBI].cy);
   printf ("\n");
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char  /* ---- : save the starting position ------------------------------*/
scrp__start        (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   /*---(write the first)----------------*/
   printf ("#!/usr/local/bin/golem\n#\n");
   printf ("# arachne generated golem script\n#\n");
   gait_clear ();
   scrp__step (0);
   /*---(save)---------------------------*/
   for (i = 0; i < LEGS; ++i) {
      gait.sx[i]  = gk[i][TARS].cx;
      gait.sz[i]  = gk[i][TARS].cz;
      gait.sy[i]  = gk[i][TARS].cy;
      /*> printf ("%d, sx=%6.1f, sz=%6.1f, sy=%6.1f\n", i, gait.sx[i], gait.sz[i], gait.sy[i]);   <*/
      gait.cx[i]  = gait.sx[i];
      gait.cz[i]  = gait.sz[i];
      gait.cy[i]  = gait.sy[i];
      gait.dir[i] = 'f';
      scrp__leg (i, 1.000);
   }
   /*---(update)-------------------------*/
   gait.running = 'y';
   /*---(complete)-----------------------*/
   return 0;
}

PRIV char  /* ---- : save the current position -------------------------------*/
scrp__saveik       (int a_index, int a_pos)
{
   gk[a_index][FEMU].d  = ik[a_index][FEMU].d;
   gk[a_index][PATE].d  = ik[a_index][PATE].d;
   gk[a_index][TIBI].d  = ik[a_index][TIBI].d;
   if (a_pos >= MAX_POS) return 0;
   gait.y    [a_pos][a_index] = ik[a_index][TIBI].cy;
   gait.coxa [a_pos][a_index] = ik[a_index][COXA].d;
   gait.femu [a_pos][a_index] = ik[a_index][FEMU].d;
   gait.pate [a_pos][a_index] = ik[a_index][PATE].d;
   gait.tibi [a_pos][a_index] = ik[a_index][TIBI].d;
   if (a_pos > gait.maxpos)  gait.maxpos = a_pos;
   /*---(min and touch)------------------*/
   if      (gait.y [a_pos][a_index] >  1.0)  gait.tleg [a_pos][a_index] =  0;
   else if (gait.y [a_pos][a_index] < -1.0)  gait.tleg [a_pos][a_index] = -1;
   else  {
      gait.tleg [a_pos][a_index] =  1;
      ++gait.touch[a_pos];
   }
   /*---(torque)-------------------------*/
   /*> stat_torque (a_pos, a_index);                                                  <*/
   /*---(complete)-----------------------*/
   return 0;
}

char
scrp_position      (int a_pos)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   kine_center (gait.cenx [a_pos], gait.cenz [a_pos]);
   for (i = 0; i < LEGS; ++i) {
      /*> rc = kine_forward (i, gait.coxa [a_pos][i], gait.femu [a_pos][i], gait.pate [a_pos][i], gait.tibi [a_pos][i]);   <*/
      gk[i][FEMU].d     =  gait.femu [a_pos][i];
      gk[i][PATE].d     =  gait.pate [a_pos][i];
      gk[i][TIBI].d     =  gait.tibi [a_pos][i];
      gait.cur [i] = (a_pos + gait.off[i]) % gait.dmax;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            poses                             ----===*/
/*====================------------------------------------====================*/
static void      o___POSES___________________o (void) {;}

char
pose_spread        (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   /*---(save)---------------------------*/
   for (i = 0; i < LEGS; ++i) {
      gk[i][FEMU].d     =    0.0f;
      gk[i][PATE].d     =    0.0f;
      gk[i][TIBI].d     =  -90.0f;
      /*> rc = kine_forward (i, gk[i][THOR].d, gk[i][FEMU].d , gk[i][PATE].d , gk[i][TIBI].d );   <*/
      /*> rc = kine_inverse (i, gk[i][THOR].d, fk[i][FEMU].cx, fk[i][PATE].cz, fk[i][TIBI].cy, SILENT);   <*/
   }
   kine_height (130.0);
   /*---(complete)-----------------------*/
   return 0;
}

char
pose_attention     (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   /*---(save)---------------------------*/
   for (i = 0; i < LEGS; ++i) {
      gk[i][FEMU].d     =    0.0f;
      gk[i][PATE].d     =   30.0f;
      gk[i][TIBI].d     = -100.0f;
      /*> rc = kine_forward (i, gk[i][THOR].d, gk[i][FEMU].d , gk[i][PATE].d , gk[i][TIBI].d );   <*/
      /*> rc = kine_inverse (i, gk[i][THOR].d, fk[i][FEMU].cx, fk[i][PATE].cz, fk[i][TIBI].cy, SILENT);   <*/
   }
   kine_height ( 94.0);
   /*---(complete)-----------------------*/
   return 0;
}

char
pose_high          (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   /*---(save)---------------------------*/
   for (i = 0; i < LEGS; ++i) {
      gk[i][FEMU].d     =    0.0f;
      gk[i][PATE].d     =  -40.0f;
      gk[i][TIBI].d     =  -50.0f;
      /*> rc = kine_forward (i, gk[i][THOR].d, gk[i][FEMU].d , gk[i][PATE].d , gk[i][TIBI].d );   <*/
      /*> rc = kine_inverse (i, gk[i][THOR].d, fk[i][FEMU].cx, fk[i][PATE].cz, fk[i][TIBI].cy, SILENT);   <*/
   }
   kine_height (167.0);
   /*> kine_height (100.0);                                                           <*/
   /*---(complete)-----------------------*/
   return 0;
}

char
pose_low           (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   /*---(save)---------------------------*/
   for (i = 0; i < LEGS; ++i) {
      gk[i][FEMU].d     =    0.0f;
      gk[i][PATE].d     =   30.0f;
      gk[i][TIBI].d     = -110.0f;
      /*> rc = kine_forward (i, gk[i][THOR].d, gk[i][FEMU].d , gk[i][PATE].d , gk[i][TIBI].d );   <*/
      /*> rc = kine_inverse (i, gk[i][THOR].d, fk[i][FEMU].cx, fk[i][PATE].cz, fk[i][TIBI].cy, SILENT);   <*/
   }
   /*> kine_height (167.0);                                                           <*/
   kine_height (100.0);
   /*---(complete)-----------------------*/
   return 0;
}

char
pose_crab          (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   /*---(save)---------------------------*/
   for (i = 0; i < LEGS; ++i) {
      switch (i) {
      case 0: case 3:  gk[i][FEMU].d     =   55.0f;  break;
      case 1: case 4:  gk[i][FEMU].d     =    0.0f;  break;
      case 2: case 5:  gk[i][FEMU].d     =  -55.0f;  break;
      }
      gk[i][PATE].d     =   10.0f;
      gk[i][TIBI].d     =  -80.0f;
      /*> rc = kine_forward (i, gk[i][THOR].d, gk[i][FEMU].d , gk[i][PATE].d , gk[i][TIBI].d );   <*/
      /*> rc = kine_inverse (i, gk[i][THOR].d, fk[i][FEMU].cx, fk[i][PATE].cz, fk[i][TIBI].cy, SILENT);   <*/
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          step sequenses                      ----===*/
/*====================------------------------------------====================*/
static void      o___SEQ_____________________o (void) {;}

char       /* ---- : prepare a cross wave sequence ---------------------------*/
seq__wave_cross    (void)
{
   /*---(design notes)-------------------*/
   /*
    *  taken from blue hexapod wave gait youtube videos
    *
    */
   /*---(setup)--------------------------*/
   gait.off [5] = (gait.dmax / 6.0) * 0;
   gait.off [2] = (gait.dmax / 6.0) * 1;
   gait.off [0] = (gait.dmax / 6.0) * 2;
   gait.off [4] = (gait.dmax / 6.0) * 3;
   gait.off [1] = (gait.dmax / 6.0) * 4;
   gait.off [3] = (gait.dmax / 6.0) * 5;
   /*---(complete)-----------------------*/
   return 0;
}

char       /* ---- : prepare a back-to-front wave sequence -------------------*/
seq__wave_forward  (void)
{
   /*---(design notes)-------------------*/
   /*
    *  taken from ragno wave gait youtube videos
    *
    *  essentially legs move left, then right; back first to front
    *
    */
   /*---(setup)--------------------------*/
   gait.off [2] = (gait.dmax / 6.0) * 0;
   gait.off [3] = (gait.dmax / 6.0) * 1;
   gait.off [1] = (gait.dmax / 6.0) * 2;
   gait.off [4] = (gait.dmax / 6.0) * 3;
   gait.off [0] = (gait.dmax / 6.0) * 4;
   gait.off [5] = (gait.dmax / 6.0) * 5;
   /*---(complete)-----------------------*/
   return 0;
}

char       /* ---- : prepare a tripod sequenced wave sequence ----------------*/
seq__wave_tripod   (void)
{
   /*---(design notes)-------------------*/
   /*
    *  taken from v-hex wave gait youtube videos
    *
    */
   /*---(setup)--------------------------*/
   gait.off [0] = (gait.dmax / 6.0) * 0;
   gait.off [4] = (gait.dmax / 6.0) * 1;
   gait.off [2] = (gait.dmax / 6.0) * 2;
   gait.off [5] = (gait.dmax / 6.0) * 3;
   gait.off [1] = (gait.dmax / 6.0) * 4;
   gait.off [3] = (gait.dmax / 6.0) * 5;
   /*---(complete)-----------------------*/
   return 0;
}

char       /* ---- : prepare a ferrell wave sequence -------------------------*/
seq__wave_ferrell  (void)
{
   /*---(design notes)-------------------*/
   /*
    *  from Robust and Adaptive Locomotion of an Autonomous Hexapod by Cynthia
    *  Ferrell in 1994 -- gaits designed at MIT for Hannibal
    *
    *  essentially back to front on left side, then back to front on the right
    *
    */
   /*---(setup)--------------------------*/
   gait.off [2] = (gait.dmax / 6.0) * 0;
   gait.off [1] = (gait.dmax / 6.0) * 1;
   gait.off [0] = (gait.dmax / 6.0) * 2;
   gait.off [3] = (gait.dmax / 6.0) * 3;
   gait.off [4] = (gait.dmax / 6.0) * 4;
   gait.off [5] = (gait.dmax / 6.0) * 5;
   /*---(complete)-----------------------*/
   return 0;
}

char       /* ---- : prepare a spinger classic wave sequence -----------------*/
seq__wave_springer (void)
{
   /*---(design notes)-------------------*/
   /*
    *  from Springer Handbook of Robotics by Bruno Siciliano and Oussama Khatib
    *  shown on page 380, figure 16.27 and 16.28
    *
    */
   /*---(setup)--------------------------*/
   gait.off [1] = (gait.dmax / 6.0) * 0;
   gait.off [5] = (gait.dmax / 6.0) * 1;
   gait.off [2] = (gait.dmax / 6.0) * 2;
   gait.off [4] = (gait.dmax / 6.0) * 3;
   gait.off [0] = (gait.dmax / 6.0) * 4;
   gait.off [3] = (gait.dmax / 6.0) * 5;
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           scripts                            ----===*/
/*====================------------------------------------====================*/
static void      o___SCRIPTS_________________o (void) {;}

char
scrp_init          (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- position           */
   int       j         = 0;            /* loop iterator -- position           */
   /*---(stance)-------------------------*/
   moving = 'y';
   /*> pose_spread    ();                                                             <*/
   /*> pose_attention ();                                                             <*/
   /*> pose_high      ();                                                             <*/
   /*> pose_low       ();                                                             <*/
   /*---(run gait)-----------------------*/
   for (i = 0; i < MAX_POS; ++i) {
      /*> scrp_hulahoop (i);                                                          <*/
      /*> scrp_wave     (i);                                                          <*/
      /*> scrp_tripod   (i);                                                          <*/
      /*> kine_height (center.by - stat_settle ());                                   <*/
      /*> scrp_tripod   (i);                                                          <*/
      /*> for (j = 0; j < LEGS; ++j)  stat_torque   (i, j);                           <*/
   }
   moving = 'n';
   /*---(complete)-----------------------*/
   return 0;
}

char
scrp_vertical      (int  a_pos)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   /*---(test for first run)-------------*/
   if (gait.running != 'y') scrp__start ();
   /*---(defense)------------------------*/
   if (moving == 'n')  return 0;
   /*---(register step)------------------*/
   scrp__step (a_pos);
   /*---(run through legs)---------------*/
   for (i = 0; i < LEGS; ++i) {
      /*---(next position)---------------*/
      if   (gait.dir[i] == 'f')  --gait.cy[i];
      else                       ++gait.cy[i];
      /*---(calculate)-------------------*/
      rc = kine_inverse(i, gk[i][THOR].d, gait.sx[i], gait.sz[i], gait.cy[i], SILENT);
      /*---(test for goodness)-----------*/
      if (rc >= 0) {
         scrp__saveik (i, 0);
         scrp__leg    (i, 0.200);
         continue;
      }
      /*---(else switch directions)------*/
      if   (gait.dir[i] == 'f') {  gait.dir[i]  = 'r';  gait.cy[i]   += 2;  }
      else                      {  gait.dir[i]  = 'f';  gait.cy[i]   -= 2;  }
   }
   /*---(complete)-----------------------*/
   return 0;
}


char
scrp_hulahoop      (int  a_pos)
{
   /*---(defense)------------------------*/
   /*> if (moving == 'n')  return 0;                                                  <*/
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   float     x, z;                     /* coordinates                         */
   float     rad       = 0.0;          /* radians                             */
   float     l         = 0.0;          /* radius of movement                  */
   /*---(test for first run)-------------*/
   if (gait.running != 'y') {
      scrp__start ();
      gait_read   ();
   }
   /*---(defense)------------------------*/
   if (moving == 'n' && my_pos == my_ppos)  return 0;
   gait.pos = a_pos;
   /*---(register step)------------------*/
   scrp__step (a_pos);
   /*---(slowly increase radius)---------*/
   l = (float) a_pos * 0.25;
   if (a_pos > 120.0)  l = 30.0;
   /*> l = (float) a_pos * 0.25;                                                      <* 
    *> if (a_pos > 220.0)  l = 55.0;                                                  <*/
   /*---(position body)------------------*/
   rad   = a_pos * DEG2RAD;
   x     = (l * cos(rad));
   z     = (l * sin(rad));
   kine_center ( x,  z );
   gait.cenx [a_pos] = center.bx;
   gait.cenz [a_pos] = center.bz;
   /*---(run through legs)---------------*/
   for (i = 0; i < LEGS; ++i) {
      gait.cur [i] = a_pos;
      /*---(calculate)-------------------*/
      /*> if (i == 0 && (a_pos %  45 ==  0 )) rc = kine_inverse (i, gk[i][THOR].d, gait.sx[i], gait.sz[i], gait.sy[i], CHATTY);   <* 
       *> else                                rc = kine_inverse (i, gk[i][THOR].d, gait.sx[i], gait.sz[i], gait.sy[i], SILENT);   <*/
      rc = kine_inverse (i, gk[i][THOR].d, gait.sx[i], gait.sz[i], gait.sy[i], SILENT);
      /*> rc = kine_inverse (i, gk[i][THOR].d, gait.sx[i], gait.sz[i], gait.sy[i], CHATTY);   <*/
      /*---(test for goodness)-----------*/
      if (rc >= 0) {
         scrp__saveik (i, a_pos);
         scrp__leg    (i, 0.200);
         continue;
      } else {
         printf ("BOOM!!!!!\n\n");
         exit (-1);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
scrp_sideside      (int  a_pos)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   /*---(test for first run)-------------*/
   if (gait.running != 'y') scrp__start ();
   /*---(defense)------------------------*/
   if (moving == 'n')  return 0;
   /*---(register step)------------------*/
   scrp__step (a_pos);
   /*---(run through legs)---------------*/
   for (i = 0; i < LEGS; ++i) {
      /*---(next position)---------------*/
      if   (gait.dir[i] == 'f')  --gait.cx[i];
      else                  ++gait.cx[i];
      /*---(calculate)-------------------*/
      rc = kine_inverse (i, gk[i][THOR].d,  gait.cx[i], gait.sz[i], gait.sy[i], SILENT);
      /*---(test for goodness)-----------*/
      if (rc >= 0) {
         scrp__saveik (i, a_pos);
         scrp__leg    (i, 0.200);
         continue;
      }
      /*---(else switch directions)------*/
      if   (gait.dir[i] == 'f') {  gait.dir[i]  = 'r';  gait.cx[i]   += 2;  }
      else                      {  gait.dir[i]  = 'f';  gait.cx[i]   -= 2;  }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
scrp_tripod        (int  a_pos)
{
   /*> printf ("entering scrp_square (%d)\n", a_pos);                                 <*/
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   int       pos       = 0;
   /*---(test for first run)-------------*/
   /*> printf ("   gait.running = %c\n", gait.running);                               <*/
   if (gait.running != 'y') {
      scrp__start ();
      gait_read   ();
      /*> gait_square ();                                                             <*/
   }
   /*---(defense)------------------------*/
   if (moving == 'n' && my_pos == my_ppos)  return 0;
   gait.pos = a_pos;
   /*---(starting)-----------------------*/
   if (a_pos < 10) {
      /*---(register step)------------------*/
      scrp__step (a_pos);
      /*---(run through legs)---------------*/
      for (i = 0; i < LEGS; ++i) {
         if (i % 2 != 0)  {
            rc = kine_inverse (i, gk[i][THOR].d,  gait.sx[i], gait.sz[i], gait.sy[i]                , SILENT);
            gait.off [i] = 0;
         } else {
            rc = kine_inverse (i, gk[i][THOR].d,  gait.sx[i], gait.sz[i], gait.sy[i] + (a_pos / 3.0), SILENT);
            gait.off [i] = gait.dmax / 2;
         }
         /*---(test for goodness)-----------*/
         if (rc >= 0) {
            scrp__saveik (i, a_pos);
            scrp__leg    (i, 0.200);
         } else {
            printf ("%d) rc = %2d\n", i, rc);
         }
      }
      return 0;
   }
   /*---(register step)------------------*/
   scrp__step (a_pos);
   /*---(run through legs)---------------*/
   for (i = 0; i < LEGS; ++i) {
      pos   = (a_pos + gait.off [i]) % gait.dmax;
      gait.cur [i] = pos;
      /*---(calculate)-------------------*/
      rc = kine_inverse (i, gk[i][THOR].d,  gait.sx[i], gait.sz[i] - gait.dxz[pos], gait.sy[i] + gait.dy[pos], SILENT);
      /*---(test for goodness)-----------*/
      if (rc >= 0) {
         scrp__saveik (i, a_pos);
         scrp__leg    (i, 0.200);
         continue;
      } else {
         printf ("BOOM!!!!!\n\n");
         /*> exit (-1);                                                               <*/
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
scrp_wave          (int  a_pos)
{
   /*> printf ("entering scrp_square (%d)\n", a_pos);                                 <*/
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   int       pos       = 0;
   /*---(test for first run)-------------*/
   /*> printf ("   gait.running = %c\n", gait.running);                               <*/
   if (gait.running != 'y') {
      scrp__start ();
      gait_read   ();
   }
   /*---(defense)------------------------*/
   if (moving == 'n' && my_pos == my_ppos)  return 0;
   gait.pos = a_pos;
   /*---(starting)-----------------------*/
   /*> seq__wave_cross   ();                                                          <*/
   seq__wave_tripod  ();
   /*> seq__wave_ferrell ();                                                          <*/
   /*---(register step)------------------*/
   scrp__step (a_pos);
   /*---(run through legs)---------------*/
   for (i = 0; i < LEGS; ++i) {
      pos   = (a_pos + gait.off [i]) % gait.dmax;
      gait.cur [i] = pos;
      /*---(calculate)-------------------*/
      rc = kine_inverse (i, gk[i][THOR].d,  gait.sx[i], gait.sz[i] - gait.dxz[pos], gait.sy[i] + gait.dy[pos], SILENT);
      /*---(test for goodness)-----------*/
      if (rc >= 0) {
         scrp__saveik (i, a_pos);
         scrp__leg    (i, 0.200);
         continue;
      } else {
         printf ("BOOM!!!!!\n\n");
         exit (-1);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
scrp_quad          (int  a_pos)
{
   /*> printf ("entering scrp_square (%d)\n", a_pos);                                 <*/
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   int       pos       = 0;
   /*---(test for first run)-------------*/
   /*> printf ("   gait.running = %c\n", gait.running);                               <*/
   if (gait.running != 'y') {
      scrp__start ();
      gait_square ();
      gait_read ();
   }
   /*---(defense)------------------------*/
   if (moving == 'n' && my_pos == my_ppos)  return 0;
   gait.pos = a_pos;
   /*---(starting)-----------------------*/
   gait.off [0] =  0;
   gait.off [3] =  0;
   gait.off [1] = 12;
   gait.off [4] = 36;
   gait.off [2] = 24;
   gait.off [5] = 24;
   /*---(register step)------------------*/
   scrp__step (a_pos);
   /*---(run through legs)---------------*/
   for (i = 0; i < LEGS; ++i) {
      pos   = (a_pos + gait.off [i]) % gait.dmax;
      gait.cur [i] = pos;
      /*---(calculate)-------------------*/
      rc = kine_inverse (i, gk[i][THOR].d,  gait.sx[i], gait.sz[i] - gait.dxz[pos], gait.sy[i] + gait.dy[pos], SILENT);
      /*---(test for goodness)-----------*/
      if (rc >= 0) {
         scrp__saveik (i, a_pos);
         scrp__leg    (i, 0.200);
         continue;
      } else {
         printf ("BOOM!!!!!\n\n");
         exit (-1);
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            gaits                             ----===*/
/*====================------------------------------------====================*/
static void      o___GAITS___________________o (void) {;}

char
gait_clear         (void)
{
   int       i         = 0;            /* loop iterator -- position           */
   int       j         = 0;            /* loop iterator -- leg                */
   for (i = 0; i < MAX_POS; ++i) {
      for (j = 0; j < MAX_LEGS; ++j) {
         gait.y    [i][j]  = 0.0;
         gait.coxa [i][j]  = 0.0;
         gait.femu [i][j]  = 0.0;
         gait.pate [i][j]  = 0.0;
         gait.tibi [i][j]  = 0.0;
         gait.tleg [i][j]  = 0.0;
      }
      gait.dxz  [i]  = 0.0;
      gait.dy   [i]  = 0.0;
      gait.dphz [i]  = 0;
      gait.touch[i]  = 0;
      gait.cenx [i]  = 0.0;
      gait.cenz [i]  = 0.0;
      gait.ceny [i]  = 0.0;
   }
   gait.maxpos = 0;
   return 0;
}

char
gait_square        (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   int       pos       = 0;            /* position in gait                    */
   int       beg       =  15;
   int       end       = -20;
   /*---(load power stroke)--------------*/
   for (i = 0; i < beg; i += 1) {
      gait.dxz [pos] = -i;
      gait.dy  [pos] =  0;
      ++pos;
   }
   /*---(load kick)----------------------*/
   for (i = 1; i < 6 ; i += 2) {
      gait.dxz [pos] = gait.dxz [pos - 1];
      gait.dy  [pos] =  i * 3;
      ++pos;
   }
   /*---(return)-------------------------*/
   for (i = beg; i > end; i -= 5) {
      gait.dxz [pos] =  -i;
      gait.dy  [pos] =  gait.dy [pos - 1];
      ++pos;
   }
   /*---(load touchdown)-----------------*/
   for (i = 5; i > 0; i -= 2) {
      gait.dxz [pos] = gait.dxz [pos - 1];
      gait.dy  [pos] = i * 3;
      ++pos;
   }
   /*---(load power stroke)--------------*/
   for (i = end; i < 0; i += 1) {
      gait.dxz [pos] = -i;
      gait.dy  [pos] =  0;
      ++pos;
   }
   gait.dmax = pos;
   /*---(complete)-----------------------*/
   return 0;
}

char
gait_curved        (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- leg                */
   char      rc        = 0;            /* generic return code                 */
   int       pos       = 0;            /* position in gait                    */
   int       beg       =  15;
   int       end       = -20;
   /*---(load power stroke)--------------*/
   for (i = 0; i < beg; i += 1) {
      gait.dxz [pos] = -i;
      gait.dy  [pos] =  0;
      ++pos;
   }
   /*---(load kick)----------------------*/
   for (i = 1; i < 6 ; i += 2) {
      gait.dxz [pos] = gait.dxz [pos - 1];
      gait.dy  [pos] =  i * 3;
      ++pos;
   }
   /*---(return)-------------------------*/
   for (i = beg; i > end; i -= 5) {
      gait.dxz [pos] =  -i;
      gait.dy  [pos] =  gait.dy [pos - 1];
      ++pos;
   }
   /*---(load touchdown)-----------------*/
   for (i = 5; i > 0; i -= 2) {
      gait.dxz [pos] = gait.dxz [pos - 1];
      gait.dy  [pos] = i * 3;
      ++pos;
   }
   /*---(load power stroke)--------------*/
   for (i = end; i < 0; i += 1) {
      gait.dxz [pos] = -i;
      gait.dy  [pos] =  0;
      ++pos;
   }
   gait.dmax = pos;
   /*---(complete)-----------------------*/
   return 0;
}

char
gait_read          (void)
{
   /*---(locals)-------*-----------------*/
   FILE     *f         = NULL;         /* input file                          */
   int       nrecd     = 0;            /* number of current input record      */
   char      recd [1000];              /* input record                        */
   int       len       = 0;            /* length of input record              */
   int       posxz     = 0;            /* number of current position in step  */
   int       posy      = 0;            /* number of current position in step  */
   char     *p         = NULL;         /* token pointer                       */
   char      active    = '-';
   char      phase     = 0;
   /*---(open file)----------------------*/
   f = fopen (CONF_GAIT, "r");
   if (f == NULL) {
      printf ("FATAL : can not open gait.conf file\n");
      return -1;
   }
   gait.grounded = 0;
   /*---(read sections)------------------*/
   while (1) {
      fgets (recd, 500, f);
      if (feof (f)) break;
      recd [strlen(recd) - 1] = '\0';
      ++nrecd;
      /*---(defense)---------------------*/
      if (recd[0] == '\0') continue;
      if (recd[0] == '#' ) continue;
      /*> printf ("%03d) %-50.50s\n", nrecd, recd);                                   <*/
      if (recd[0] != ' ' && active == 'y') {
         /*> printf ("ending\n");                                                     <*/
         break;
      }
      p = strtok (recd, " ");
      if (active != 'y') {
         /*> printf ("starting\n");                                                   <*/
         active = 'y';
         strncpy (gait.name, p, 25);
         continue;
      }
      /*---(save marks)------------------*/
      if      (strcmp (p, "fore"   ) == 0)  { phase = 1; continue; }
      else if (strcmp (p, "mark"   ) == 0)  { phase = 2; continue; }
      else if (strcmp (p, "back"   ) == 0)  { phase = 3; continue; }
      else if (strcmp (p, "kick"   ) == 0)  { phase = 4; continue; }
      else if (strcmp (p, "accel"  ) == 0)  { phase = 5; continue; }
      else if (strcmp (p, "return" ) == 0)  { phase = 6; continue; }
      else if (strcmp (p, "decel"  ) == 0)  { phase = 7; continue; }
      else if (strcmp (p, "load"   ) == 0)  { phase = 8; continue; }
      /*---(deal with xy)----------------*/
      if        (strcmp (p, "xz") == 0) {
         while (1) {
            p = strtok (NULL, " ");
            if (p == NULL) break;
            gait.dxz [posxz] = atof (p);
            gait.dphz[posxz] = phase;
            ++posxz;
            if  (phase >= 1 && phase <= 3)   ++gait.grounded;
         }
      } else if (strcmp (p, "y" ) == 0) {
         while (1) {
            p = strtok (NULL, " ");
            if (p == NULL) break;
            gait.dy  [posy ] = atof (p);
            ++posy;
         }
      }
   }
   /*> printf ("read %dxz and %dy\n", posxz, posy);                                   <*/
   gait.dmax  = posxz;
   gait.duty  = (float) gait.grounded / gait.dmax;
   /*---(complete)-----------------------*/
   return 0;
}



/*============================----end-of-source---============================*/
