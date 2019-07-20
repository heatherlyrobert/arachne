/*============================----beg-of-source---============================*/
#include    "arachne.h"



char
api_yvikeys_init        (void)
{
   yVIKEYS_view_config   ("arachne, hexapod visualization and simulation", VER_NUM, YVIKEYS_OPENGL, 800, 500, 0);
   yVIKEYS_view_setup    (YVIKEYS_MAIN    , YVIKEYS_DEPTH, YVIKEYS_MIDCEN, my.m_xmin, my.m_xmax - my.m_xmin, my.m_ymin, my.m_ymax - my.m_ymin, my.m_zmin, my.m_zmax - my.m_zmin, YCOLOR_BAS, DRAW_primary);
   yVIKEYS_view_setup    (YVIKEYS_FLOAT   , YVIKEYS_DEPTH, YVIKEYS_MIDCEN, -200     , 400      , -35      , 20       , 0        , 0        , 0         , NULL);
   yVIKEYS_view_setup    (YVIKEYS_MENUS   , YVIKEYS_DEPTH, YVIKEYS_MIDCEN, 0        , 0        , 0        , 0        , 0        , 0        , 0         , NULL);
   yVIKEYS_view_simple   (YVIKEYS_PROGRESS, YCOLOR_BAS   , TICK_show   );
   yVIKEYS_cmds_direct   (":xaxis    disable");
   yVIKEYS_cmds_direct   (":yaxis    disable");
   yVIKEYS_cmds_direct   (":ribbon   disable");
   yVIKEYS_cmds_direct   (":nav      disable");
   yVIKEYS_cmds_direct   (":alt      disable");
   yVIKEYS_cmds_direct   (":formula  disable");
   yVIKEYS_cmds_direct   (":layout   work");
   yVIKEYS_cmds_direct   (":progress show");
   yVIKEYS_cmds_direct   (":buffer   show");
   /*---(colors)-------------------------*/
   yVIKEYS_cmds_direct   (":palette 100 rcomp pale earthy");
   yVIKEYS_view_colors   (YCOLOR_POS, YCOLOR_BAS, YCOLOR_NEG, YCOLOR_POS);
   yVIKEYS_cmds_add ('a', "p_cond"      , ""    , ""     , KINE_unitcond        , "write out a unit testing condition for yKINE"    );
   yVIKEYS_cmds_add ('a', "p_ik"        , ""    , ""     , KINE_unitcond_ik     , "write out a unit testing condition for yKINE"    );
   yVIKEYS_cmds_add ('t', "golem"       , ""    , "s"    , yGOLEM_toggle        , "turn yGOLEM on and off"                          );
   yVIKEYS_map_config    (YVIKEYS_OFFICE, api_yvikeys_mapper, NULL, NULL);



   return 0;
}


/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPING_________o () { return; }

char
api_yvikeys_mapper      (char a_req)
{  /*---(design notes)-------------------*/
   /*
    * YVIKEYS_INIT   -> clean everything, redo everything
    * YVIKEYS_UPDATE -> change as necessary
    *
    */
   char         x_bup      = YVIKEYS_MINOR;
   char         x_xup      = YVIKEYS_MINOR;
   char         x_yup      = YVIKEYS_MINOR;
   char         x_zup      = YVIKEYS_MINOR;
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_view_size     (YVIKEYS_MAIN, NULL, &g_xmap.avail, NULL, &g_ymap.avail, NULL);   <*/
   /*---(check changes)------------------*/
   if (a_req == YVIKEYS_INIT) {
      x_bup   = x_xup   = x_zup   = x_yup  = YVIKEYS_MAJOR;
      yVIKEYS_clear_mapping (&g_bmap);
      yVIKEYS_clear_mapping (&g_xmap);
      yVIKEYS_clear_mapping (&g_zmap);
      yVIKEYS_clear_mapping (&g_ymap);
   /*> } else {                                                                       <* 
    *>    if (g_xmap.gcur != my.m_xcur)  x_zup = x_yup = YVIKEYS_MAJOR;               <* 
    *>    if (g_zmap.gcur != my.m_zcur)  x_xup = x_yup = YVIKEYS_MAJOR;               <* 
    *>    if (g_ymap.gcur != my.m_ycur)  x_xup = x_zup = YVIKEYS_MAJOR;               <*/
   }
   /*---(save current)-------------------*/
   my.p_cur  = g_xmap.gcur;
   /*> my.m_zcur = g_zmap.gcur;                                                       <*/
   my.p_line = g_ymap.gcur;
   /*---(update maps)--------------------*/
   yVIKEYS_uniform_mapping (a_req, &g_xmap, 0, 40        , 1, 16);
   yVIKEYS_uniform_mapping (a_req, &g_zmap, 0, 0         , 1, 1);
   yVIKEYS_uniform_mapping (a_req, &g_ymap, 0, my.p_nline, 1, 1);
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
}
