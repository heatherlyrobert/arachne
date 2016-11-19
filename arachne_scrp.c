/*============================----beg-of-source---============================*/

#include  "arachne.h"


tGAIT     gait;


tSERVO     g_servos  [MAX_SERVO] = {
   /* label--------   cnt   curr  degs  --segno--  --coda--- scrp  prev  next */
   { "RR.femu"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RR.pate"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RR.tibi"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RM.femu"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RM.pate"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RM.tibi"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RF.femu"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RF.pate"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "RF.tibi"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LF.femu"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LF.pate"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LF.tibi"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LM.femu"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LM.pate"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LM.tibi"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LR.femu"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LR.pate"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "LR.tibi"      ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   { "end-of-list"  ,   0,  NULL,  0.0, '-', NULL, '-', NULL, '-', NULL, NULL },
   /* label--------   cnt   curr  degs  --segno--  --coda--- scrp  prev  next */
};
int         g_nservo;



/*====================------------------------------------====================*/
/*===----                         file access                          ----===*/
/*====================------------------------------------====================*/
static void      o___ACCESS__________________o (void) {;}

static FILE    *s_file  = NULL;
static int      s_lines = 0;
static char     s_recd  [LEN_RECD];


char         /*--> prepare for use ---------s-------------[ leaf   [ ------ ]-*/
SCRP_init          (void)
{
   int         i           = 0;
   g_nservo = 0;
   for (i = 0; i < MAX_SERVO; ++i) {
      if (g_servos [i].label [0] == 'e')   break;
      ++g_nservo;
   }
   return 0;
}

char         /*--> open file for reading -----------------[ leaf   [ ------ ]-*/
SCRP_open          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_info    ("filename"  , my.f_name);
   /*---(defense)------------------------*/
   --rce;  if (strcmp (my.f_name, "") == 0) {
      DEBUG_INPT  yLOG_note    ("no file to open");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(open stdin)---------------------*/
   if (strcmp ("stdin", my.f_name) == 0) {
      DEBUG_INPT  yLOG_note    ("data being provided on stdin");
      s_file = stdin;
   }
   /*---(open file)----------------------*/
   else {
      DEBUG_INPT  yLOG_note    ("data being provided in a file");
      s_file = fopen (my.f_name, "r");
   }
   /*---(check success)------------------*/
   DEBUG_INPT  yLOG_point   ("s_file"    , s_file);
   --rce;  if (s_file == NULL) {
      DEBUG_INPT  yLOG_note    ("file could not be openned");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_note    ("file successfully opened");
   /*---(init values)--------------*/
   SCRP_init ();
   /*---(complete)-----------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> close file after reading --------------[ flower [ ------ ]-*/
SCRP_close         (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(stdin)--------------------------*/
   if (strcmp (my.f_name, "") == 0) {
      DEBUG_INPT  yLOG_note    ("stdin should not be closed");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      s_file == NULL;
      return 0;
   }
   /*---(close file)---------------------*/
   DEBUG_INPT  yLOG_point   ("s_file"    , s_file);
   rc = fclose  (s_file);
   DEBUG_INPT  yLOG_value   ("rc"        , rc);
   /*---(check success)------------------*/
   --rce;  if (rc != 0) {
      DEBUG_INPT  yLOG_note    ("file could not be closed");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_note    ("file successfully closed");
   s_file == NULL;
   /*---(complete)-----------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        record parsing                        ----===*/
/*====================------------------------------------====================*/
static void      o___PARSING_________________o (void) {;}

#define     FIELD_SVO      1
#define     FIELD_ARG      2
#define     FIELD_DEG      3
#define     FIELD_SEC      4
#define     FIELD_XPOS     5
#define     FIELD_YPOS     6
#define     FIELD_ZPOS     7

#define     FIELD_COUNT    3
#define     FIELD_TIMES    4

int         s_len       = 0;
char       *s_q         = "";               /* strtok delimeters         */
char       *s_context   = NULL;               /* strtok context variable   */

char         /*--> locate a servo entry ------------------[ ------ [ ------ ]-*/
SCRP_servo         (char *a_label)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;                /* return code for errors    */
   int         i           = 0;
   int         x_index     = -1;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(cycle)--------------------------*/
   /*> printf ("SCRP_servo         looking for %s\n", a_label);                       <*/
   for (i = 0; i < g_nservo; ++i) {
      if (a_label [0] != g_servos [i].label [0])       continue;
      if (strcmp (a_label, g_servos [i].label) != 0)   continue;
      DEBUG_INPT   yLOG_snote   ("servo label found");
      g_servos [i].scrp = 'y';
     /*> printf ("SCRP_servo                        found\n");                        <*/
      x_index = i;
      break;
   }
   DEBUG_INPT   yLOG_svalue  ("index"     , x_index);
   --rce;  if (x_index < 0) {
      DEBUG_INPT   yLOG_snote   ("servo label not found");
      DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return x_index;
}

char         /*--> locate a servo entry ------------------[ ------ [ ------ ]-*/
SCRP_servos        (char *a_label)
{  /*---(design notes)-------------------*/
   /*
    *  L=left  , R=right
    *  F=front , M=middle, R=rear
    *  a=all   , +=large , -=small
    *
    *  troc = trocanter
    *  femu = femur
    *  pate = patella
    *  tibi = tibia
    *  meta = metatarsus
    *  tars = tarsus
    *  foot = foot
    *  claw = claw
    *  magn = magnet
    *  hook = hook
    *
    *  servo labels start with two-char leg, '.', and four-char segment
    *     LF.femu    = left-front leg's femur
    *     aF.femu    = femur on both front legs
    *     La.femu    = femurs on the left side
    *     aa.femu    = all femurs
    *     -F.femu    = femurs on the front small legs
    *     +F.femu    = femurs on the front large legs
    *     L-.femu    = femurs on the left small legs
    *     L+.femu    = femurs on the left large legs
    *     ++.femu    = femurs on all large legs
    *     --.femu    = femurs on all small legs
    *
    *   need to add front to back, side to side, and other mirroring
    *
    *
    */
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;                /* return code for errors    */
   int         i           = 0;
   int         j           = 0;
   int         x_index     = -1;
   char        x_side      [LEN_LABEL] = "";
   int         x_nside     = 0;
   char        x_rank      [LEN_LABEL] = "";
   int         x_nrank     = 0;
   char        x_label     [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(interpret side-to-side)---------*/
   switch (a_label [0]) {
   case 'L' : strlcpy (x_side  , "L"         , LEN_LABEL);   break;
   case 'R' : strlcpy (x_side  , "R"         , LEN_LABEL);   break;
   case 'l' : strlcpy (x_side  , "l"         , LEN_LABEL);   break;
   case 'r' : strlcpy (x_side  , "r"         , LEN_LABEL);   break;
   case '<' : strlcpy (x_side  , "Ll"        , LEN_LABEL);   break;
   case '>' : strlcpy (x_side  , "Rr"        , LEN_LABEL);   break;
   case '+' : strlcpy (x_side  , "LR"        , LEN_LABEL);   break;
   case '-' : strlcpy (x_side  , "lr"        , LEN_LABEL);   break;
   case 'a' : strlcpy (x_side  , "LRlr"      , LEN_LABEL);   break;
   default  : strlcpy (x_side  , ""          , LEN_LABEL);   break;
   }
   x_nside = strlen (x_side);
   /*> printf ("SCRP_servos  x_side (%d) %s\n", x_nside, x_side);                     <*/
   --rce;  if (x_nside == 0) {
      DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(interpret front-to-back)--------*/
   switch (a_label [1]) {
   case 'R' : strlcpy (x_rank  , "R"         , LEN_LABEL);   break;
   case 'M' : strlcpy (x_rank  , "M"         , LEN_LABEL);   break;
   case 'F' : strlcpy (x_rank  , "F"         , LEN_LABEL);   break;
   case 'r' : strlcpy (x_rank  , "r"         , LEN_LABEL);   break;
   case 'f' : strlcpy (x_rank  , "f"         , LEN_LABEL);   break;
   case '^' : strlcpy (x_rank  , "Ff"        , LEN_LABEL);   break;
   case '_' : strlcpy (x_rank  , "Rr"        , LEN_LABEL);   break;
   case '+' : strlcpy (x_rank  , "RMF"       , LEN_LABEL);   break;
   case '-' : strlcpy (x_rank  , "rf"        , LEN_LABEL);   break;
   case 'a' : strlcpy (x_rank  , "RMFrf"     , LEN_LABEL);   break;
   default  : strlcpy (x_rank  , ""          , LEN_LABEL);   break;
   }
   x_nrank = strlen (x_rank);
   /*> printf ("SCRP_servos  x_rank (%d) %s\n", x_nrank, x_rank);                     <*/
   if (x_nrank == 0)  return -1;
   /*---(cycle)--------------------------*/
   for (i = 0; i < x_nside; ++i) {
      for (j = 0; j < x_nrank; ++j) {
         sprintf (x_label, "%c%c.%s", x_side [i], x_rank [j], a_label + 3);
        /*> printf ("SCRP_servos     x_label %s\n", x_label);                         <*/
         x_index = SCRP_servo (x_label);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> parse a move entry --------------------[ ------ [ ------ ]-*/
SCRP_move          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;                /* return code for errors    */
   char        rc          = 0;
   char       *p           = NULL;
   int         i           = 0;
   int         j           = 0;
   int         x_len       = 0;
   int         x_servo     = -1;
   float       x_degs      = -1;
   float       x_secs      = -1;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(read fields)--------------------*/
   for (i = FIELD_SVO  ; i <= FIELD_ZPOS ; ++i) {
      /*---(parse field)-----------------*/
      DEBUG_INPT   yLOG_note    ("read next field");
      p = strtok_r (NULL  , s_q, &s_context);
      --rce;  if (p == NULL) {
         DEBUG_INPT   yLOG_note    ("strtok_r came up empty");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         break;
      }
      strltrim (p, ySTR_BOTH, LEN_RECD);
      x_len = strlen (p);
      DEBUG_INPT  yLOG_info    ("field"     , p);
      /*---(handle)----------------------*/
      switch (i) {
      case  FIELD_SVO   :  /*---(servo)----*/
         x_servo = SCRP_servos (p);
         --rce;  if (x_servo < 0) {
            DEBUG_INPT  yLOG_warn    ("servo"     , "not found");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         break;
      case  FIELD_ARG   :  /*---(args)-----*/
         break;
      case  FIELD_DEG   :  /*---(degrees)--*/
         x_degs = atof (p);
         DEBUG_INPT  yLOG_double  ("degrees"   , x_degs);
         break;
      case  FIELD_SEC   :  /*---(seconds)--*/
         x_secs = atof (p);
         DEBUG_INPT  yLOG_double  ("seconds"   , x_secs);
         for (j = 0; j < g_nservo; ++j) {
            if (g_servos [j].scrp != 'y') continue;
            MOVE_create (MOVE_SERVO, g_servos + j, "", 0, x_degs, x_secs);
         }
         break;
      }
      DEBUG_INPT   yLOG_note    ("done with loop");
   } 
   DEBUG_INPT   yLOG_note    ("done parsing fields");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> parse a segno marker ------------------[ ------ [ ------ ]-*/
SCRP_segno         (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;                /* return code for errors    */
   char        rc          = 0;
   char       *p           = NULL;
   int         i           = 0;
   int         j           = 0;
   int         x_len       = 0;
   int         x_servo     = -1;
   int         x_count     = -1;
   int         x_times     = -1;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(read fields)--------------------*/
   for (i = FIELD_SVO  ; i <= FIELD_SVO  ; ++i) {
      /*---(parse field)-----------------*/
      DEBUG_INPT   yLOG_note    ("read next field");
      p = strtok_r (NULL  , s_q, &s_context);
      --rce;  if (p == NULL) {
         DEBUG_INPT   yLOG_note    ("strtok_r came up empty");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         break;
      }
      strltrim (p, ySTR_BOTH, LEN_RECD);
      x_len = strlen (p);
      DEBUG_INPT  yLOG_info    ("field"     , p);
      /*---(handle)----------------------*/
      switch (i) {
      case  FIELD_SVO   :  /*---(servo to repeat)----*/
         x_servo = SCRP_servos (p);
         --rce;  if (x_servo < 0) {
            DEBUG_INPT  yLOG_warn    ("servo"     , "not found");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         for (j = 0; j < g_nservo; ++j) {
            if (g_servos [j].scrp != 'y') continue;
            g_servos [j].segno_flag = 'y';
            g_servos [j].segno      = NULL;
         }
         break;
      }
      DEBUG_INPT   yLOG_note    ("done with loop");
   } 
   DEBUG_INPT   yLOG_note    ("done parsing fields");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> parse a low level repeat --------------[ ------ [ ------ ]-*/
SCRP_repeat        (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;                /* return code for errors    */
   char        rc          = 0;
   char       *p           = NULL;
   int         i           = 0;
   int         j           = 0;
   int         x_len       = 0;
   int         x_servo     = -1;
   int         x_count     = -1;
   int         x_times     = -1;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(read fields)--------------------*/
   for (i = FIELD_SVO  ; i <= FIELD_TIMES; ++i) {
      /*---(parse field)-----------------*/
      DEBUG_INPT   yLOG_note    ("read next field");
      p = strtok_r (NULL  , s_q, &s_context);
      --rce;  if (p == NULL) {
         DEBUG_INPT   yLOG_note    ("strtok_r came up empty");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         break;
      }
      strltrim (p, ySTR_BOTH, LEN_RECD);
      x_len = strlen (p);
      DEBUG_INPT  yLOG_info    ("field"     , p);
      /*---(handle)----------------------*/
      switch (i) {
      case  FIELD_SVO   :  /*---(servo to repeat)----*/
         x_servo = SCRP_servos (p);
         --rce;  if (x_servo < 0) {
            DEBUG_INPT  yLOG_warn    ("servo"     , "not found");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         break;
      case  FIELD_ARG   :  /*---(args)-----*/
         break;
      case  FIELD_COUNT :  /*---(moves to repeat)----*/
         x_count = atoi (p);
         DEBUG_INPT  yLOG_value   ("x_count"   , x_count);
         --rce;  if (x_count < 1 || x_count > 100) {
            DEBUG_INPT  yLOG_warn    ("moves"     , "out of range (1 - 100)");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         break;
      case  FIELD_TIMES :  /*---(times to repeat)----*/
         x_times = atoi (p);
         DEBUG_INPT  yLOG_value   ("x_times"   , x_times);
         --rce;  if (x_times < 1 || x_times > 100) {
            DEBUG_INPT  yLOG_warn    ("times"     , "out of range (1 - 100)");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         for (j = 0; j < g_nservo; ++j) {
            if (g_servos [j].scrp != 'y') continue;
            MOVE_repeat     (g_servos + j, x_count, x_times);
         }
         break;
      }
      DEBUG_INPT   yLOG_note    ("done with loop");
   } 
   DEBUG_INPT   yLOG_note    ("done parsing fields");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> parse a high level repeat -------------[ ------ [ ------ ]-*/
SCRP_dalsegno      (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;                /* return code for errors    */
   char        rc          = 0;
   char       *p           = NULL;
   int         i           = 0;
   int         j           = 0;
   int         x_len       = 0;
   int         x_servo     = -1;
   int         x_times     = -1;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(read fields)--------------------*/
   for (i = FIELD_SVO  ; i <= FIELD_TIMES; ++i) {
      /*---(parse field)-----------------*/
      DEBUG_INPT   yLOG_note    ("read next field");
      p = strtok_r (NULL  , s_q, &s_context);
      --rce;  if (p == NULL) {
         DEBUG_INPT   yLOG_note    ("strtok_r came up empty");
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         break;
      }
      strltrim (p, ySTR_BOTH, LEN_RECD);
      x_len = strlen (p);
      DEBUG_INPT  yLOG_info    ("field"     , p);
      /*---(handle)----------------------*/
      switch (i) {
      case  FIELD_SVO   :  /*---(servo to repeat)----*/
         x_servo = SCRP_servos (p);
         --rce;  if (x_servo < 0) {
            DEBUG_INPT  yLOG_warn    ("servo"     , "not found");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         break;
      case  FIELD_ARG   :  /*---(args)-----*/
         break;
      case  FIELD_COUNT :  /*---(moves to repeat)----*/
         break;
      case  FIELD_TIMES :  /*---(times to repeat)----*/
         x_times = atoi (p);
         DEBUG_INPT  yLOG_value   ("x_times"   , x_times);
         --rce;  if (x_times < 1 || x_times > 100) {
            DEBUG_INPT  yLOG_warn    ("times"     , "out of range (1 - 100)");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         for (j = 0; j < g_nservo; ++j) {
            if (g_servos [j].scrp != 'y') continue;
            MOVE_dalsegno   (g_servos + j, x_times);
         }
         break;
      }
      DEBUG_INPT   yLOG_note    ("done with loop");
   } 
   DEBUG_INPT   yLOG_note    ("done parsing fields");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}







/*====================------------------------------------====================*/
/*===----                        script driver                         ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

char         /* file reading driver ----------------------[--------[--------]-*/
SCRP_prep          (void)
{
   int         i           = 0;
   for (i = 0; i < g_nservo; ++i) {
      g_servos [i].scrp  = '-';
   }
   return 0;
}

char         /* file reading driver ----------------------[--------[--------]-*/
SCRP_main          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         rc          = 0;
   char       *p;
   char        x_type      [20]        = "";;
   char        x_ver       = '-';
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   rc = SCRP_open   ();
   if (rc < 0) {
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(read lines)---------------------*/
   DEBUG_INPT  yLOG_note    ("read lines");
   while (1) {
      /*---(prepare)---------------------*/
      SCRP_prep      ();
      /*---(read and clean)--------------*/
      ++s_lines;
      DEBUG_INPT  yLOG_value   ("line"      , s_lines);
      fgets (s_recd, LEN_RECD, s_file);
      if (feof (s_file))  {
         DEBUG_INPT  yLOG_note    ("end of file reached");
         break;
      }
      s_len = strlen (s_recd);
      if (s_len <= 0)  {
         DEBUG_INPT  yLOG_note    ("record empty");
         continue;
      }
      s_recd [--s_len] = '\0';
      DEBUG_INPT  yLOG_value   ("length"    , s_len);
      DEBUG_INPT  yLOG_info    ("fixed"     , s_recd);
      if (s_recd [0] == '#') {
         DEBUG_INPT  yLOG_note    ("comment line, skipping");
         continue;
      }

      /*---(get recd type)---------------*/
      p = strtok_r (s_recd, s_q, &s_context);
      if (p == NULL) {
         DEBUG_INPT  yLOG_note    ("can not parse type field");
         continue;
      }
      strltrim  (p, ySTR_BOTH, LEN_RECD);
      strncpy   (x_type, p,  10);
      DEBUG_INPT  yLOG_info    ("type"      , x_type);
      /*---(get version)-----------------*/
      p = strtok_r (NULL     , s_q, &s_context);
      if (p == NULL) {
         DEBUG_INPT  yLOG_note    ("can not parse version field");
         continue;
      }
      strltrim  (p, ySTR_BOTH, LEN_RECD);
      if (strlen (p) != 3) {
         DEBUG_INPT  yLOG_note    ("invalid length version field (3)");
         continue;
      }
      if (p[0] != '-' || p[2] != '-') {
         DEBUG_INPT  yLOG_note    ("invalid format version field (-x-)");
         continue;
      }
      x_ver = p[1];
      DEBUG_INPT  yLOG_char    ("version"   , x_ver);
      /*---(handle types)----------------*/
      switch (x_type [0]) {
      case 'R' : /* repeat             */
         SCRP_repeat    ();
         break;
      case 'S' : /* segno              */
         SCRP_segno     ();
         break;
      case 'D' : /* dal segno          */
         SCRP_dalsegno  ();
         break;
      case 's' : /* servo, start       */
         SCRP_move      ();
         break;
      default  :
         DEBUG_INPT  yLOG_note    ("verb not recognized and skipped");
         break;
      }

   }
   /*---(close file)---------------------*/
   SCRP_close ();
   /*---(complete)-------------------------*/
   DEBUG_INPT yLOG_exit    (__FUNCTION__);
   return 0;
}







/*============================----end-of-source---============================*/
