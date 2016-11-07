/*============================----beg-of-source---============================*/

#include "arachne.h"






/*====================------------------------------------====================*/
/*===----                      mode data structure                     ----===*/
/*====================------------------------------------====================*/
static void  o___STRUCTURE_______o () { return; }


typedef  struct  cMODE_INFO  tMODE_INFO;
#define     MAX_MODES       30
struct cMODE_INFO {
   char        abbr;                   /* single character abbreviation       */
   char        major;                  /* major mode (y/n)                    */
   char        show;                   /* show a message line (y/n)           */
   char        three       [ 5];       /* very short name                     */
   char        terse       [10];       /* short name                          */
   char        desc        [50];       /* description of mode                 */
   char        allow       [25];       /* allowed mode transitions            */
   int         count;                  /* number of times used                */
   char        mesg        [LEN_STR];  /* informative message for display     */
};

static tMODE_INFO  s_mode_info [MAX_MODES] = {
   /*-a-- -maj show --tla- ---terse----- ---description---------------------------------------- -----------------------,----- 123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789- */
   /*---(major modes)--------------------*/
   { 'G' , 'y', 'y', "GOD", "god"       , "god-mode allowing 3D omnicient viewing"             , "P"                   ,    0, "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                },
   { 'P' , 'y', 'y', "PRG", "progress"  , "progress timeline adding time dimension"            , ""                    ,    0, "0LlhH$"},
   { 'M' , 'y', 'y', "MAP", "map"       , "map-mode providing 2D review of object collections" , "GVSI:/\"b\'$oe\\"    ,    0, "horz(a)=0HhlL$  horz(g/z)=sh,le  vert(a)=_KkjJG  vert(g/z)=tk.jb  modes=vIFV:{ret}"      },
   { 'V' , 'y', 'y', "VIS", "visual"    , "visual selection of objects for collection action"  , "\""                  ,    0, "0HhlL$_KkjJG  gz=sh,letk.jb  dxy  !: ~uU /nN oO sS"                                      },
   { 'S' , 'y', 'y', "SRC", "source"    , "linewise review of textual content"                 , "Isrte"               ,    0, "hor=0HhlL$bBeEwW  g/z=sh,le  sel=vV\"  pul=yYdDxX  put=pP  chg=rRiIaA  fnd=fnN"          },
   { 'I' , 'y', 'y', "INP", "input"     , "linewise creation and editing of textual content"   , ""                    ,    0, ""                                                                                        },
   { ':' , 'y', '-', "CMD", "command"   , "command line capability for advanced actions"       , ""                    ,    0, ""                                                                                        },
   /*---(sub-modes)----------------------*/
   { 'e' , '-', 'y', "err", "errors"    , "display and action errors"                          , ""                    ,    0, ""                                                                                        },
   { 's' , '-', 'y', "sel", "select"    , "visual selection within text content"               , "t"                   ,    0, "0HhlL$"                                                                                  },
   { 'r' , '-', 'y', "rep", "replace"   , "linewise overtyping of content in source mode"      , ""                    ,    0, "type over character marked with special marker"                                          },
   { '"' , '-', 'y', "reg", "register"  , "selecting specific registers for data movement"     , ""                    ,    0, "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           },
   { 't' , '-', 'y', "trg", "text reg"  , "selecting specific registers for text movement"     , ""                    ,    0, "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           },
   { ',' , '-', 'y', "buf", "buffer"    , "moving and selecting between buffers and windows"   , ""                    ,    0, "select=0...9  modes={ret}(esc}"                                                          },
   { '@' , '-', 'y', "wdr", "wander"    , "formula creation by moving to target cells"         , ""                    ,    0, "modes={ret}{esc}"                                                                        },
   { '$' , '-', 'y', "frm", "format"    , "content formatting options"                         , ""                    ,    0, "ali=<|>[^] num=irg,as$%%p tec=#eExXbBoO tim=tdT dec=0-9 str= _-=.+"                      },
   { 'o' , '-', 'y', "obj", "object"    , "object formatting and sizing options"               , ""                    ,    0, ""                                                                                        },
   { '\'', '-', 'y', "mrk", "mark"      , "object and location marking"                        , ""                    ,    0, "set=a-zA-Z()  del=#*  hlp=?!@_  go='a-zA-Z()[<>]"                                        },
   { '\\', '-', 'y', "mnu", "menus"     , "menu system"                                        , ""                    ,    0, ""                                                                                        },
   /*---(done)---------------------------*/
   { '-' , '-', 'y', "bad", "bad mode"  , "default message when mode is not understood"        , ""                    ,    0, "mode not understood"                                                                     },
};

static      char        s_majors       [MAX_MODES] = "";



/*====================------------------------------------====================*/
/*===----                      mode stack handling                     ----===*/
/*====================------------------------------------====================*/
static void  o___MODE_STACK______o () { return; }
#define        MAX_STACK   100
static char    s_modes    [MAX_STACK];      /* gyges mode stack               */
static int     s_nmode;                     /* depth of gyges mode stack      */
static char    s_cmode;                     /* current gyges mode             */

char
MODE_init          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        t           [5]         = "";
   /*---(check modes)--------------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      if (s_mode_info [i].major != 'y')    continue;
      sprintf (t, "%c", s_mode_info [i].abbr);
      strlcat (s_majors, t, MAX_MODES);
   }
   /*---(validate mode)------------------*/
   for (i = 0; i < MAX_STACK; ++i) {
      s_modes [i] = '-';
   }
   s_nmode =  0;
   s_cmode = '-';
   /*---(complete)-----------------------*/
   return 0;
}

char
MODE_enter         (char a_mode)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           = 0;
   char        x_mode      = '-';
   int         x_index     = -1;
   /*---(check for dup)------------------*/
   /*> if (s_modes [s_nmode] == a_mode)  return 1;                            <*/
   /*---(validate mode)------------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      if (s_mode_info[i].abbr == '-'    )  break;
      if (s_mode_info[i].abbr != a_mode )  continue;
      ++s_mode_info [i].count;
      x_mode  = a_mode;
   }
   --rce;  if (x_mode  == '-')  return rce;
   /*---(check if allowed)---------------*/
   if (s_nmode > 0)  {
      for (i = 0; i < MAX_MODES; ++i) {
         if (s_mode_info[i].abbr == '-'    )  break;
         if (s_mode_info[i].abbr != s_cmode)  continue;
         x_index = i;
      }
      --rce;  if (x_index <   0 )  return rce;
      --rce;  if (strchr (s_mode_info[x_index].allow, a_mode) == NULL)  return rce;
   }
   /*---(add mode)-----------------------*/
   --rce;  if (s_nmode >= MAX_STACK)   return rce;
   s_modes [s_nmode] = a_mode;
   ++s_nmode;
   /*---(set global mode)----------------*/
   s_cmode = a_mode;
   /*---(complete)-----------------------*/
   return 0;
}

char
MODE_return        (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        x_mode      = '-';
   /*---(check stack)--------------------*/
   --rce;  if (s_nmode <= 0)  return rce;
   --s_nmode;
   s_modes [s_nmode] = x_mode;
   x_mode = s_modes [s_nmode - 1];
   /*---(set global mode)----------------*/
   s_cmode = x_mode;
   /*---(complete)-----------------------*/
   return 0;
}

char
MODE_curr          (void)
{
   return s_cmode;
}

char
MODE_prev          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        x_mode      = '-';
   /*---(check stack)--------------------*/
   --rce;  if (s_nmode <= 1)            return rce;
   /*---(grab previous)------------------*/
   x_mode = s_modes [s_nmode - 2];
   if (strchr (s_majors, x_mode) != NULL)   return x_mode;
   /*---(go back one more)---------------*/
   --rce;  if (s_nmode <= 2)            return rce;
   x_mode = s_modes [s_nmode - 3];
   /*---(complete)-----------------------*/
   return x_mode;
}

char
MODE_not           (char a_mode)
{
   if (a_mode != s_modes [s_nmode - 1]) return -1;
   return 0;
}

char       /*----: list the current mode stack -------------------------------*/
MODE_list          (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   char        t           [10];
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(walk the list)------------------*/
   sprintf (a_list, "modes (%d)", s_nmode);
   for (i = 0; i < 8; ++i) {
      sprintf (t, " %c", s_modes [i]);
      strlcat (a_list, t, LEN_STR);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
MODE_message       (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        x_major     = ' ';
   char        x_minor     = ' ';
   for (i = 0; i < MAX_MODES; ++i) {
      if (s_mode_info[i].abbr == '-'   )  break;
      if (s_mode_info[i].abbr == s_cmode)  break;
   }
   if (s_mode_info [i].major == 'y')  {
      x_major = s_cmode;
   } else {
      x_major = MODE_prev ();
      x_minor = s_cmode;
   }
   if (s_mode_info[i].show == 'y') {
      sprintf (my.message, "[%c%c] %-3.3s : %s\n", x_major, x_minor, s_mode_info[i].three, s_mode_info[i].mesg);
   } else {
      sprintf (my.message, "%s\n", my.c_command);
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     mode key handling                        ----===*/
/*====================------------------------------------====================*/
static void  o___MODE_KEYS_______o () { return; }

char         /*--> process keystrokes in normal mode -----[--------[--------]-*/
MODE_god           (char a_major, char a_minor)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   char        x_minors    [LEN_STR]  = "ypdx";
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_char    ("a_major"   , a_major);
   DEBUG_USER   yLOG_char    ("a_minor"   , a_minor);
   /*---(defenses)-----------------------*/
   DEBUG_USER   yLOG_char    ("mode"      , MODE_curr ());
   --rce;  if (MODE_not (MODE_GOD     )) {
      DEBUG_USER   yLOG_note    ("not the correct mode");
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(single key)---------------------*/
   --rce;
   if (a_major == ' ') {
      switch (a_minor) {
      /* crab  /horz  */  case 'h': case 'l': case 'H': case 'L': 
      /* boom  /vert  */  case 'j': case 'k': case 'J': case 'K':
      /* dolly /zoom  */  case 'i': case 'I': case 'o': case 'O':
      /* x-axis/pitch */  case 'p': case 'P': case 'a': case 'A':
      /* y-axis/yaw   */  case 'y': case 'Y': case 't': case 'T':
      /* z-axis/roll  */  case 'r': case 'R': case 'w': case 'W':
      case '0': case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9':
         yGOD_key (a_minor);
         break;
      case ',':
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return a_minor;
         break;
      }
   }
   /*---(buffer/area)--------------------*/
   if (a_major == ',') {
      switch (a_minor) {
      case 'p':
         MODE_enter  (MODE_PROGRESS);
         TICK_draw ();
         break;
      }
   }
   /*---(complete)------------------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> process keystrokes in progress mode ---[--------[--------]-*/
MODE_progress      (char a_major, char a_minor)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   char        x_minors    [LEN_STR]  = "ypdx";
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_char    ("a_major"   , a_major);
   DEBUG_USER   yLOG_char    ("a_minor"   , a_minor);
   /*---(defenses)-----------------------*/
   DEBUG_USER   yLOG_char    ("mode"      , MODE_curr ());
   --rce;  if (MODE_not (MODE_PROGRESS)) {
      DEBUG_USER   yLOG_note    ("not the correct mode");
      DEBUG_USER   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(single key)---------------------*/
   --rce;
   if (a_major == ' ') {
      /*---(modes)-----------------------*/
      switch (a_minor) {
      case ',':
         DEBUG_USER   yLOG_exit    (__FUNCTION__);
         return a_minor;
         break;
      }
      /*---(zoom and retreat)------------*/
      switch (a_minor) {
      case '+': SCALE_smaller ();   /* in  */                break;
      case '-': SCALE_larger  ();   /* out */                break;
      }
      /*---(play and stop)---------------*/
      switch (a_minor) {
      case '>':  SPEED_faster ();                            break;
      case '<':  SPEED_slower ();                            break;
      case '.':
         if (my.p_moving == 'y')  SPEED_stop   ();
         else                     SPEED_play   ();
         break;
      }
      /*---(horizontal movement)---------*/
      switch (a_minor) {
      case '0': my_pos  =  0.0;               break;
      case 'L': my_pos +=  my.p_inc * 5;      break;
      case 'l': my_pos +=  my.p_inc;          break;
      case 'h': my_pos -=  my.p_inc;          break;
      case 'H': my_pos -=  my.p_inc * 5;      break;
      case '$': my_pos  =  my.p_len;          break;
      }
      /*---(other)-----------------------*/
      switch (a_minor) {
      case '?': 
                my.p_debug = 'y';
                /*> TICK_show ();                                                     <*/
                break;
         /*> printf ("my.p_width         = %10.3f\n", my.w_width * 1.0);                         <* 
          *> printf ("full size          = %10.3f\n", my.w_width * 2.0);                         <* 
          *> printf ("x_half             = %10.3f\n", my.w_width * 1.0);                         <* 
          *> printf ("my_pos             = %10.3f\n", my_pos);                                   <* 
          *> printf ("x_inc              = %10.3f\n", 10.0);                                     <* 
          *> printf ("my.p_inc           = %10.3f\n", my.p_inc);                                 <* 
          *> printf ("x_sec              = %10.3f\n", 10.0 / my.p_inc);                          <* 
          *> printf ("# secs on screen   = %10.3f\n", (my.w_width * 2.0) / (10.0 / my.p_inc));   <* 
          *> printf (" * my_pos          = %10.3f\n", my_pos * (10.0 / my.p_inc));               <*/
   /*> /+---(calculate offset)---------------+/                                                 <* 
    *> x_right     = my.w_width;                                                                <* 
    *> x_cur       = my_pos * (x_inc / my.p_inc) / 2.0f;   /+ texture displayed at 0.5x +/      <* 
    *> x_half      = x_right;                                                                   <* 
    *> /+---(set beginning and end)----------+/                                                 <* 
    *> x_beg       = x_cur - x_half;                                                            <* 
    *> /+> x_beg       = my.w_width;                                                      <+/   <* 
    *> if (x_beg < 0.0)  x_beg = 0.0;                                                           <* 
    *> x_end       = x_beg + (x_half * 2.0f);                                                   <* 
    *> /+---(scale number to texture)--------+/                                                 <* 
    *> x_beg      /= my.p_texw;                                                                 <* 
    *> x_end      /= my.p_texw;                                                                 <*/
      }
   }
   /*---(buffer/area)--------------------*/
   if (a_major == ',') {
      switch (a_minor) {
      case 'a':
         MODE_return ();
         TICK_draw ();
         break;
      }
   }
   /*---(complete)------------------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*============================----end-of-source---============================*/
