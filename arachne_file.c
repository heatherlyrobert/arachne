/*============================----beg-of-source---============================*/
#include  "arachne.h"



char
FILE_prepper            (char a_pass)
{
   /*---(header)-------------------------*/
   DEBUG_INPT    yLOG_enter   (__FUNCTION__);
   DEBUG_INPT    yLOG_value   ("a_pass"    , a_pass);
   yKINE_reinit ();
   my.p_len     =  0.0;
   my.p_line    =  0.0;
   my.p_leg     =  0.0;
   my.p_cur     =  0.0;
   my.p_endsec  = -1.0;
   yPARSE_delimiters  (YPARSE_FUNCTION);
   yKINE_scrp_prepper  (a_pass);
   DEBUG_INPT    yLOG_exit    (__FUNCTION__);
   return 1;
}

char
FILE_finisher           (char a_pass)
{
   DEBUG_INPT    yLOG_enter   (__FUNCTION__);
   DEBUG_INPT    yLOG_value   ("a_pass"    , a_pass);
   yKINE_scrp_finisher (a_pass, &my.p_len);
   DEBUG_INPT    yLOG_double  ("my.p_len"  , my.p_len);
   DEBUG_INPT    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_init               (void)
{
   char        rc          =    0;
   rc = yVIKEYS_whoami     (P_FULLPATH, P_VERNUM, P_VERTXT, P_NAMESAKE, P_SUFFIX, P_CONTENT, yKINE_handlers, FILE_prepper, FILE_finisher);
   rc = yPARSE_delimiters  (YPARSE_FUNCTION);
   rc = yVIKEYS_menu_grey   ("µf");   /* all file    */
   rc = yVIKEYS_menu_active ("µfw");  /* new         */
   rc = yVIKEYS_menu_active ("µfo");  /* read        */
   rc = yVIKEYS_menu_active ("µff");  /* refresh     */
   rc = yVIKEYS_menu_active ("µfq");  /* quit        */
   return 0;
}



