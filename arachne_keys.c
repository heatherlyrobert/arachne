/*============================----beg-of-source---============================*/

#include "arachne.h"



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
   DEBUG_USER   yLOG_char    ("mode"      , yVIKEYS_mode_curr ());
   --rce;  if (yVIKEYS_mode_not (MODE_GOD     )) {
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
         yVIKEYS_mode_enter  (MODE_PROGRESS);
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
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   DEBUG_USER   yLOG_char    ("a_major"   , a_major);
   DEBUG_USER   yLOG_char    ("a_minor"   , a_minor);
   /*---(defenses)-----------------------*/
   DEBUG_USER   yLOG_char    ("mode"      , yVIKEYS_mode_curr ());
   --rce;  if (yVIKEYS_mode_not (MODE_PROGRESS)) {
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
      /*---(choosing leg)----------------*/
      switch (a_minor) {
      case '_': my.p_leg  = 0;    break;
      case 'J': my.p_leg -= 5;    break;
      case 'j': my.p_leg -= 1;    break;
      case 'k': my.p_leg += 1;    break;
      case 'K': my.p_leg += 5;    break;
      case 'G': my.p_leg  = 5;    break;
      }
      if (my.p_leg < 0)  my.p_leg = 0;
      if (my.p_leg > 5)  my.p_leg = 5;
      /*---(zoom and retreat)------------*/
      switch (a_minor) {
      case '+':
         yVIKEYS_scale_less    (&my.p_inc);
         TICK_draw ();
         break;
      case '-':
         yVIKEYS_scale_more    (&my.p_inc);
         TICK_draw ();
         break;
      }
      /*---(play and stop)---------------*/
      switch (a_minor) {
      case '>':
         yVIKEYS_speed_more   (&my.p_waitns);
         TICK_draw ();
         break;
      case '<': 
         yVIKEYS_speed_less   (&my.p_waitns);
         TICK_draw ();
         break;
      case '.':
         yVIKEYS_speed_play   (&my.p_waitns);  /* toggles */
         TICK_draw ();
         break;
      }
      /*---(horizontal movement)---------*/
      if (strchr ("0HhlL$", a_minor) != 0) {
         yVIKEYS_keys_horz    (a_minor, &my_pos, my.p_inc, 0.0, my.p_len);
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
         yVIKEYS_mode_exit  ();
         TICK_draw ();
         break;
      }
   }
   /*---(complete)------------------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*============================----end-of-source---============================*/
