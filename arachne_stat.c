/*============================----beg-of-source---============================*/

/*
 *   --- from wikipedia ------------------------------------------------------
 *   statics is the branch of mechanics concerned with the analysis of loads
 *   (force, torque/moment) on physical systems in static equalibrium, that is
 *   in a state where the relative positions of subsystems do not vary over
 *   time, or where components and structures are at a constant velocity.
 *
 *    purpose
 *       - establish body configuration, size, and mass
 *       - calculate the mass center
 *       - calculate leg torque/moment based on gravity
 *
 *
 *
 *
 *
 */

#include  "arachne.h"

#define   GRAVITY       9.80665
#define   NM2OZIN     141.61193
#define   HT475HB       0.53668
#define   HT645MG       0.93919

char      debug_sizing   = 'n';
#define   DEBUG_SIZING   if (debug_sizing == 'y')

char      model_name [100]    = "ch3-r";
char      model_desc [100]    = "";
int       LEGS           = 6;




/*====================------------------------------------====================*/
/*===----                            setting                           ----===*/
/*====================------------------------------------====================*/
static void      o___SETTING_________________o (void) {;}

char
stat_init          (char  *a_model)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   FILE     *f         = NULL;         /* input file                          */
   int       nrecd     = 0;            /* number of current input record      */
   char      recd [1000];              /* input record                        */
   int       len       = 0;            /* length of input record              */
   char      type;                     /* record type                         */
   char      active    = '-';          /* record for use                      */
   char     *p         = NULL;
   int       i         = 0;            /* loop iterator -- segs               */
   int       j         = 0;            /* loop iterator -- legs               */
   /*---(header)-------------------------*/
   DEBUG_CONF  yLOG_enter   (__FUNCTION__);;
   /*---(open file)----------------------*/
   DEBUG_CONF  yLOG_info    ("file"      , CONF_SIZE);;
   f = fopen (CONF_SIZE, "r");
   DEBUG_CONF  yLOG_point   ("f"         , f);;
   --rce;  if (f == NULL) {
      DEBUG_CONF  yLOG_exitr   (__FUNCTION__, rce);;
      return rce;
   }
   /*---(check for model name)-----------*/
   if (a_model != NULL && strcmp (a_model, "") != 0) {
      strncpy (model_name, a_model, 25);
      active = 's';
      DEBUG_SIZING  fprintf (stderr, "   - requested model = <%s>\n", model_name);
   } else {
      strncpy (model_name, "ch3-r", 25);
   }
   DEBUG_CONF  yLOG_info    ("model"     , model_name);;
   /*---(read sections)------------------*/
   while (1) {
      /*---(read)------------------------*/
      fgets (recd, 500, f);
      if (feof (f))                         break;
      recd [strlen(recd) - 1] = '\0';
      ++nrecd;
      /*---(defense)---------------------*/
      if (recd[0] == '\0')                  continue;
      if (recd[0] == '#' )                  continue;
      /*---(find current)----------------*/
      p = strtok (recd, " ");
      if (strcmp (p, "current") == 0) {
         if (active != '-')                 continue;
         p = strtok (NULL, " ");
         if (p == NULL) break;
         strncpy (model_name, p, 25);
         active = 's';
         DEBUG_SIZING  fprintf (stderr, "   - current model   = <%s>\n", model_name);
         continue;
      }
      if (active != 's' && active != 'y')   continue;
      /*---(find model)------------------*/
      if (strcmp (p, "config") == 0) {
         if (active == 'y')                 break;
         p = strtok (NULL, " ");
         if (p == NULL)                     break;
         if (strcmp (model_name, p) != 0)   continue;
         DEBUG_SIZING  fprintf (stderr, "   - found model\n");
         active = 'y';
         p = strtok (NULL, " ");
         if (p == NULL)                     break;
         LEGS = atoi (p);
         DEBUG_SIZING  fprintf (stderr, "   - legs specified  = %d\n", LEGS);
         p = strtok (NULL, "\x00");
         strncpy (model_desc, "", 50);
         if (p != NULL) {
            strncpy (model_desc, p, 50);
            DEBUG_SIZING  fprintf (stderr, "   - description     = %s\n", model_desc);
         }
         continue;
      }
      if (active != 'y')                    continue;
      /*---(save values)-----------------*/
      type  = p[0];
      switch (type) {
      case  '0': case  '1': case  '2': case  '3': case  '4': case  '5': case  '6': case  '7':
         p = strtok (NULL, " ");
         if (p == NULL)                     break;
         strncpy (legs_name [type - '0'], p, 4);
         p = strtok (NULL, " ");
         if (p == NULL)                     break;
         legs_deg [type - '0'] = gk [type - '0'][YKINE_THOR].d = gk [type - '0'][YKINE_COXA].d = atof (p);
         p = strtok (NULL, " ");
         if (p == NULL)                     break;
         strncpy (legs_long [type - '0'], p, 22);
         DEBUG_SIZING  fprintf (stderr, "   - leg %c at %4.0f called %s, %s\n", type, gk [type - '0'][YKINE_THOR].d, legs_name [type - '0'], legs_long [type - '0']);
         break;
      case  'l': case  'n': case  'x': case  'a': case  'b':
         for (i = YKINE_THOR; i <= YKINE_TARS; ++i) {
            p = strtok (NULL, " ");
            if (p == NULL)                  break;
            switch (type) {
            case 'l' : segs_len  [i]  = atof(p);   break;
            case 'n' : segs_min  [i]  = atof(p);   break;
            case 'x' : segs_max  [i]  = atof(p);   break;
            case 'a' : segs_act  [i]  = atof(p);   break;
            case 'b' : segs_lnk  [i]  = atof(p);   break;
            }
            DEBUG_SIZING  fprintf (stderr, "   - leg %c segment %s is %4.0f\n", type, segs_name[i], atof (p));
         }
         break;
      case  'p':
         for (i = YKINE_THOR; i <= YKINE_TARS; ++i) {
            p = strtok (NULL, " ");
            if (p == NULL)                  break;
            if (i < YKINE_FEMU) continue;
            for (j = 0; j <= MAX_LEGS; ++j) gk [j][i].d = atof(p);
            /*> printf ("joint %d is %8.1f and %8.1f\n", i, atof(p), gk [j][i].d);    <*/
         }
         break;
      default  :
         break;
      }
   }
   /*---(check if model found)-----------*/
   if        (active == '-') {
      fprintf (stderr, "FATAL : current model not specified in arachne_size.conf file\n");
      return   -2;
   } else if (active == 's') {
      fprintf (stderr, "FATAL : specified current model <%s> not found in arachne_size.conf file\n", model_name);
      return   -3;
   }
   /*---(complete)-----------------------*/
   DEBUG_CONF  yLOG_exit    (__FUNCTION__);;
   return 0;
}

char       /* ---- : change the length of a leg segment ----------------------*/
stat_length        (int   a_seg, float a_len, float a_def)
{
   if (a_seg <  YKINE_THOR )   return -1;
   if (a_seg >  YKINE_TARS )   return -2;
   if (a_len <=  0.0 )   segs_len [a_seg] = a_def;
   else                  segs_len [a_seg] = a_len;
   return 0;
}

char       /* ---- : change the length of all leg segments -------------------*/
stat_lengths       (
      float a_thor, float a_coxa, float a_femu,
      float a_pate, float a_tibi, float a_tars)
{
   char      rc        = 0;
   if (rc == 0)  rc = stat_length (YKINE_THOR, a_thor, 125.0);
   if (rc == 0)  rc = stat_length (YKINE_COXA, a_coxa,  30.0);
   if (rc == 0)  rc = stat_length (YKINE_FEMU, a_femu,  30.0);
   if (rc == 0)  rc = stat_length (YKINE_PATE, a_femu,  57.0);
   if (rc == 0)  rc = stat_length (YKINE_TIBI, a_tibi, 139.7);
   if (rc == 0)  rc = stat_length (YKINE_TARS, a_tars,   0.0);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                             mass                             ----===*/
/*====================------------------------------------====================*/
static void      o___MASS____________________o (void) {;}

char    /* ---- : calculate the mass center ----------------------------------*/
stat_masscenter    (void)
{
   /*---(locals)-------*-----------------*/
   float     sumx      = 0.0;          /* total mass * distance               */
   float     sumz      = 0.0;          /* total mass * distance               */
   float     sumy      = 0.0;          /* total mass * distance               */
   float     mass      = 0.0;          /* total mass in the system            */
   float     x         = 0.0;          /* x coordinate of mass center         */
   float     z         = 0.0;          /* z coordinate of mass center         */
   float     y         = 0.0;          /* y coordinate of mass center         */
   int       i         = 0;            /* loop iterator -- legs               */
   int       j         = 0;            /* loop iterator -- segments           */
   float     dist      = 0.0;          /* distance for calcs                  */
   /*---(leg by leg)---------------------*/
   /*> printf ("\n");                                                                 <*/
   for (i = 0; i < LEGS; ++i) {
      /*> printf ("%d -----------------------------------------------------------------------\n", i);   <*/
      for (j = YKINE_CORE; j <= YKINE_FOOT; ++j) {
         /*---(links)--------------------*/
         mass  +=  segs_lnk [j];
         /*> printf ("   %02d/%s %8.2f (lnks) : %8.2f at", j, segs_name[j], segs_len [j], segs_lnk  [j]);   <*/
         dist   =  gk[i][j - 1].cx + (gk[i][j].x * 0.5);
         sumx  +=  dist * segs_lnk [j];
         /*> printf ("  %8.2fx", dist);                                               <*/
         dist   =  gk[i][j - 1].cz + (gk[i][j].z * 0.5);
         sumz  +=  dist * segs_lnk [j];
         /*> printf (", %8.2fz", dist);                                               <*/
         dist   =  gk[i][j - 1].cy + (gk[i][j].y * 0.5);
         sumy  +=  dist * segs_lnk [j];
         /*> printf (", %8.2fy", dist);                                               <* 
          *> printf ("\n");                                                           <*/
         /*---(actuators)----------------*/
         mass  += segs_act [j];
         /*> printf ("                    (acts) : %8.2f at", segs_act  [j]);              <*/
         sumx  += gk[i][j].cx * segs_act [j];
         /*> printf ("  %8.2fx", gk[i][j].cx);                                        <*/
         sumz  += gk[i][j].cz * segs_act [j];
         /*> printf (", %8.2fz", gk[i][j].cz);                                        <*/
         sumy  += gk[i][j].cy * segs_act [j];
         /*> printf (", %8.2fy", gk[i][j].cy);                                        <* 
          *> printf ("\n");                                                           <*/
         /*---(done)---------------------*/
      }
      /*> printf ("\n");                                                              <*/
   }
   /*---(body)---------------------------*/
   /*> mass       += 720.0;                                                           <* 
    *> sumx       += 720.0 * center.bx;                                               <* 
    *> sumz       += 720.0 * center.bz;                                               <* 
    *> sumy       += 720.0 * center.by;                                               <*/
   /*> printf ("center   %8.2fx, %8.2fz, %8.2fy\n", center.bx, center.bz, center.by);   <*/
   /*---(final calculations)-------------*/
   /*> center.mcx  = sumx / mass;                                                     <* 
    *> center.mcz  = sumz / mass;                                                     <* 
    *> center.mcy  = sumy / mass;                                                     <*/
   /*> printf ("mass   %10.2lf\n", mass);                                             <* 
    *> printf ("sumx   %10.2lf,  %8.2lfx\n", sumx, sumx / mass);                      <* 
    *> printf ("sumz   %10.2lf,  %8.2lfz\n", sumz, sumz / mass);                      <* 
    *> printf ("sumy   %10.2lf,  %8.2lfy\n", sumy, sumy / mass);                      <* 
    *> printf ("\n");                                                                 <*/
   /*---(complete)-----------------------*/
   return 0;
}

/*> float      /+ ---- : calculate the torque in newton-meters -------------------+/                       <* 
 *> stat_torque        (                                                                                   <* 
 *>       float     a_len,                      /+ link length in millimeters     +/                       <* 
 *>       float     a_end,                      /+ end point mass in grams        +/                       <* 
 *>       float     a_link)                     /+ link mass in grams             +/                       <* 
 *> {                                                                                                      <* 
 *>    /+---(locals)-------*-----------------+/                                                            <* 
 *>    float     l         =   0.0;             /+ length                         +/                       <* 
 *>    float     m         =   0.0;             /+ mass                           +/                       <* 
 *>    float     w         =   0.0;             /+ weight                         +/                       <* 
 *>    float     ef        =   0.0;             /+ end-force                      +/                       <* 
 *>    float     lf        =   0.0;             /+ end-force                      +/                       <* 
 *>    /+---(end torque)---------------------+/                                                            <* 
 *>    l    = a_len          / 1000.0;          /+ millimeters into meters        +/                       <* 
 *>    m    = a_end          / 1000.0;          /+ grams into kilograms           +/                       <* 
 *>    w    = m * GRAVITY;                                                                                 <* 
 *>    ef   = l * w;                                                                                       <* 
 *>    /+---(link torque)--------------------+/                                                            <* 
 *>    l   *= 0.5;                                                                                         <* 
 *>    m    = a_link         / 1000.0;                                                                     <* 
 *>    w    = m * GRAVITY;                                                                                 <* 
 *>    lf   = l * w;                                                                                       <* 
 *>    /+---(output)-------------------------+/                                                            <* 
 *>    /+> if (a_leg == 0) {                                                                         <*    <* 
 *>     *>    printf ("      %8.6fmm  %8.6fg\n", ik[a_leg][YKINE_TIBI].xz, mass[YKINE_TIBI]);                    <*    <* 
 *>     *>    printf ("      %8.6fm   %8.6fkg  %8.6fms  %8.6fN   %8.6fNm\n", l, m, GRAVITY, w, f);   <*    <* 
 *>     *> }                                                                                         <+/   <* 
 *>    /+---(complete)-----------------------+/                                                            <* 
 *>    return  ef + lf;                                                                                    <* 
 *> }                                                                                                      <*/

char    /* ---- : calculate the torques at each joint ------------------------*/
stat_torque        (int  a_pos, int a_leg)
{
   /*---(design notes)-------------------*/
   /*
    *  g function is a hack ut i sort q t actual formulas.  ut
    *  tn, i wl be using approximations...
    *
    *  calculations are in g-mm and need to be converted to kg-m in the end
    *  which is 0.000001.
    *
    */
   /*---(locals)-------*-----------------*/
   float     l         = 0.00f;        /* length of link                      */
   float     m         = 0.00f;        /* mass of link or actuator            */
   float     b         = 0.00f;        /* body mass                           */
   float     F         = 0.00f;        /* resulting force                     */
   float     body      = 0.00f;
   float     femu      = 0.00f;
   float     pate      = 0.00f;
   float     tibi      = 0.00f;
   float     center    = 0.00f;
   /*---(defense)------------------------*/
   if (gait.tleg [a_pos][a_leg] == 0) {
      gait.torq[a_pos][a_leg][YKINE_FEMU] = 0.00f;
      gait.torq[a_pos][a_leg][YKINE_PATE] = 0.00f;
      gait.torq[a_pos][a_leg][YKINE_TIBI] = 0.00f;
      return 0;
   }
   /*---(sizes)--------------------------*/
   body    = gk [a_leg][YKINE_THOR].xz + gk [a_leg][YKINE_COXA].xz;
   femu    = gk [a_leg][YKINE_FEMU].xz;
   pate    = gk [a_leg][YKINE_PATE].xz;
   tibi    = gk [a_leg][YKINE_TIBI].xz;
   center  = (720.0 + (230.0 * (LEGS - gait.touch [a_pos]))) / gait.touch [a_pos];
   /*> fprintf (stderr, "center = %8.2f with %d touching\n", center, gait.touch [a_pos]);   <*/
   /*---(tibia)--------------------------*/
   F      = 0.00f;
   F     += (        (pate * 0.5)        ) * segs_lnk [YKINE_PATE];
   F     += (        pate                ) * segs_act [YKINE_PATE];
   F     += (        pate + (femu * 0.5) ) * segs_lnk [YKINE_FEMU];
   F     += (        pate + femu         ) * segs_act [YKINE_FEMU];
   F     += (        pate + femu + body  ) * center;
   F     *= 0.000001;
   gait.torq  [a_pos][a_leg][YKINE_TIBI]  = F;
   gait.torqp [a_pos][a_leg][YKINE_TIBI]  = (F / HT475HB) * 100.0;
   /*---(tibia)--------------------------*/
   /*> F      = 0.00f;                                                                <* 
    *> F     += ( tibi * 0.5                 ) * segs_lnk [YKINE_TIBI];                     <* 
    *> F     += ( tibi                       ) * segs_act [YKINE_TIBI];                     <* 
    *> F     += ( tibi + (pate * 0.5)        ) * segs_lnk [YKINE_PATE];                     <* 
    *> F     += ( tibi + pate                ) * segs_act [YKINE_PATE];                     <* 
    *> F     += ( tibi + pate + (femu * 0.5) ) * segs_lnk [YKINE_FEMU];                     <* 
    *> F     += ( tibi + pate + femu         ) * segs_act [YKINE_FEMU];                     <* 
    *> F     += ( tibi + pate + femu + body  ) * center;                              <* 
    *> F     *= 0.000001 * GRAVITY;                                                   <* 
    *> gait.torq  [a_pos][a_leg][YKINE_TIBI]  = F;                                          <* 
    *> gait.torqp [a_pos][a_leg][YKINE_TIBI]  = (F / HT475HB) * 100.0;                      <*/
   /*---(femur/hip)----------------------*/
   /*> l     = thor + coxa;                                                           <* 
    *> m     = body;                                                                  <* 
    *> F     = l * m * GRAVITY;                                                       <* 
    *> gait.torq  [a_pos][a_leg][YKINE_FEMU] = F;                                           <* 
    *> gait.torqp [a_pos][a_leg][YKINE_FEMU] = (F / HT475HB) * 100.0;                       <*/
   /*---(patella)------------------------*/
   /*> l    = (thor + coxa + femu) / 1000.0;                                          <* 
    *> m    =  b / 1000.0;                                                            <* 
    *> F    = l * m * GRAVITY;                                                        <* 
    *> l    = (femu) / 1000.0;                                                        <* 
    *> m    =  segs_act  [YKINE_COXA]                              / 1000.0;                <* 
    *> F   += l * m * GRAVITY;                                                        <* 
    *> l    = (femu * 0.5)                       / 1000.0;                            <* 
    *> m    =  segs_lnk  [YKINE_FEMU]                              / 1000.0;                <* 
    *> F   += l * m * GRAVITY;                                                        <* 
    *> gait.torq  [a_pos][a_leg][YKINE_PATE] = F;                                           <* 
    *> gait.torqp [a_pos][a_leg][YKINE_PATE] = (F / HT475HB) * 100.0;                       <*/
   /*---(complete)-----------------------*/
   return 0;
}

float
stat_settle        (void)
{
   /*---(locals)-------*-----------------*/
   int       i         = 0;            /* loop iterator -- legs               */
   float     ymin      = 1000.00f;     /* shortest gap to ground              */
   int       leg       = 1;            /* leg of shortest gap                 */
   /*---(find min)-----------------------*/
   for (i = 0; i < LEGS; ++i) {
      if (ik [i][YKINE_TIBI].cy < ymin) {
         ymin = ik [i][YKINE_TIBI].cy;
         leg = i;
      }
   }
   printf ("min y on leg %d at %8.3f\n", leg, ymin);
   /*---(complete)-----------------------*/
   return ymin;
}



/*============================----end-of-source---============================*/
