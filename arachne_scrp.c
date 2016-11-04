/*============================----beg-of-source---============================*/

#include  "arachne.h"


tGAIT     gait;

tSERVO     g_servos  [MAX_SERVO] = {
   { "RF.femu"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RF.pate"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RF.tibi"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RM.femu"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RM.pate"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RM.tibi"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RR.femu"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RR.pate"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "RR.tibi"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LR.femu"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LR.pate"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LR.tibi"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LM.femu"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LM.pate"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LM.tibi"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LF.femu"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LF.pate"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "LF.tibi"      ,   0,  NULL,  0.0,  NULL, NULL },
   { "end-of-list"  ,   0,  NULL,  0.0,  NULL, NULL },
};



/*====================------------------------------------====================*/
/*===----                         file access                          ----===*/
/*====================------------------------------------====================*/
static void      o___ACCESS__________________o (void) {;}

static FILE    *s_file  = NULL;
static int      s_lines = 0;
static char     s_recd  [LEN_RECD];

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
#define     FIELD_DEG      2
#define     FIELD_SEC      3
#define     FIELD_XPOS     4
#define     FIELD_YPOS     5
#define     FIELD_ZPOS     6

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
   for (i = 0; i < MAX_SERVO; ++i) {
      if (a_label [0] != g_servos [i].label [0])       continue;
      if (strcmp (a_label, g_servos [i].label) != 0)   continue;
      DEBUG_INPT   yLOG_snote   ("servo label found");
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

char         /*--> parse a move entry --------------------[ ------ [ ------ ]-*/
SCRP_move          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;                /* return code for errors    */
   char        rc          = 0;
   char       *p           = NULL;
   int         i           = 0;
   int         x_len       = 0;
   int         x_servo     = -1;
   float       x_degs      = -1;
   float       x_secs      = -1;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(read fields)--------------------*/
   for (i = FIELD_SVO ; i <= FIELD_ZPOS ; ++i) {
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
      case  FIELD_SVO  :  /*---(servo)----*/
         x_servo = SCRP_servo (p);
         --rce;  if (x_servo < 0) {
            DEBUG_INPT  yLOG_warn    ("servo"     , "not found");
            DEBUG_INPT  yLOG_exit    (__FUNCTION__);
            return rce;
         }
         break;
      case  FIELD_DEG  :  /*---(degrees)--*/
         x_degs = atof (p);
         DEBUG_INPT  yLOG_double  ("degrees"   , x_degs);
         break;
      case  FIELD_SEC  :  /*---(seconds)--*/
         x_secs = atof (p);
         DEBUG_INPT  yLOG_double  ("seconds"   , x_secs);
         MOVE_create (MOVE_SERVO, g_servos + x_servo, x_degs, x_secs);
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
      case 's' : /* servo, start       */
         SCRP_move ();
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
