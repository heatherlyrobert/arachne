/*============================----beg-of-source---============================*/

#include  "arachne.h"


tGAIT     gait;


/*====================------------------------------------====================*/
/*===----                         file access                          ----===*/
/*====================------------------------------------====================*/
static void      o___ACCESS__________________o (void) {;}

#define         LEN_RECD        2000
static FILE    *s_file  = NULL;
static int      s_lines = 0;
static char     s_recd  [LEN_RECD];

char         /*--> open file for reading -----------------[ leaf   [ ------ ]-*/
SCRP_open          (char *a_name)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_point   ("filename"  , a_name);
   /*---(defense)------------------------*/
   --rce;  if (a_name == NULL) {
      DEBUG_INPT  yLOG_note    ("file name can not be null");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT  yLOG_info    ("filename"  , a_name);
   /*---(open stdin)---------------------*/
   if (strcmp ("stdin", a_name) == 0) {
      DEBUG_INPT  yLOG_note    ("data being provided on stdin");
      s_file = stdin;
   }
   /*---(open file)----------------------*/
   else {
      DEBUG_INPT  yLOG_note    ("data being provided in a file");
      s_file = fopen (a_name, "r");
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
   DEBUG_INPT  yLOG_note    ("file successfully opened");
   /*---(complete)-----------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /* file reading driver ----------------------[--------[--------]-*/
SCRP_main          (char *a_name)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         rc          = 0;
   char        x_temp      [LEN_RECD];           /* strtok version of input   */
   int         x_len       = 0;                  /* string length             */
   char       *p;
   int         x_celltry   = 0;
   int         x_cellbad   = 0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   rc = SCRP_open   (a_name);
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
      x_len = strlen (s_recd);
      if (x_len <= 0)  {
         DEBUG_INPT  yLOG_note    ("record empty");
         continue;
      }
      s_recd [--x_len] = '\0';
      DEBUG_INPT  yLOG_value   ("length"    , x_len);
      DEBUG_INPT  yLOG_info    ("fixed"     , s_recd);
      if (s_recd [0] == '#') {
         DEBUG_INPT  yLOG_note    ("comment line, skipping");
         continue;
      }
   }
   /*---(close file)---------------------*/
   SCRP_close ();
   /*---(complete)-------------------------*/
   DEBUG_INPT yLOG_exit    (__FUNCTION__);
   return 0;
}







/*============================----end-of-source---============================*/
