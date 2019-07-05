/*============================----beg-of-source---============================*/
#include    "arachne.h"



char
api_yvikeys_init        (void)
{
   yVIKEYS_view_config   ("arachne, hexapod visualization and simulation", VER_NUM, YVIKEYS_OPENGL, 800, 500, 0);
   yVIKEYS_view_setup    (YVIKEYS_MAIN     , YVIKEYS_DEPTH, YVIKEYS_MIDCEN, my.m_xmin, my.m_xmax, my.m_ymin, my.m_ymax, my.m_zmin, my.m_zmax, YCOLOR_BAS, DRAW_primary);
   yVIKEYS_view_simple   (YVIKEYS_PROGRESS , YCOLOR_BAS   , TICK_show   );
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
api_yvikeys__unmap      (tMAPPED *a_map)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   /*---(lefts)--------------------------*/
   a_map->umin = a_map->gmin = a_map->gamin = a_map->glmin = a_map->gprev = 0;
   /*---(map)----------------------------*/
   for (i= 0; i < LEN_HUGE; ++i)  a_map->map [i] =  YVIKEYS_EMPTY;
   /*---(rights)-------------------------*/
   a_map->umax = a_map->gmax = a_map->gamax = a_map->glmax = a_map->gnext = 0;
   /*---(screen)-------------------------*/
   a_map->ubeg   = a_map->ucur   = a_map->uend   =  0;
   a_map->ulen   = a_map->utend  = a_map->uavail =  1;
   /*---(grids)--------------------------*/
   a_map->gbeg   = a_map->gcur   = a_map->gend   = 0;
   /*---(complete)-----------------------*/
   return  0;
}

char
api_yvikeys__map           (char a_req, tMAPPED *a_map)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_slot      =    0;
   int         i           =    0;
   int         c           =    0;
   int         x_min       =    0;
   int         x_max       =    0;
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter    (__FUNCTION__);
   /*---(adjust max)---------------------*/
   DEBUG_MAP    yLOG_char     ("which"     , a_map->which);
   --rce;  switch (a_map->which) {
   case 'x' :
      x_min   = my.m_xmin;
      x_max   = my.m_xmax;
      break;
   case 'y' :
      x_min   = my.m_ymin;
      x_max   = my.m_ymax;
      break;
   case 'z' :
      x_min   = my.m_zmin;
      x_max   = my.m_zmax;
      break;
   default  :
      DEBUG_MAP    yLOG_exitr    (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_MAP    yLOG_value    ("x_min"     , x_min);
   DEBUG_MAP    yLOG_value    ("x_max"     , x_max);
   /*---(clear map)----------------------*/
   for (i = 0; i < LEN_HUGE; ++i) a_map->map [i] = 0;
   /*---(load map)-----------------------*/
   for (i = x_min; i <= x_max; ++i) {
      a_map->map [x_slot++] = i;
   }
   /*---(unit min/max)-------------------*/
   a_map->umin  = 0;
   DEBUG_MAP    yLOG_value    ("umin"      , a_map->umin);
   a_map->umax  = x_slot - 1;
   DEBUG_MAP    yLOG_value    ("umax"      , a_map->umax);
   /*---(grid mins)----------------------*/
   a_map->gmin   = a_map->map [a_map->umin];
   a_map->gamin  = a_map->map [a_map->umin];
   a_map->glmin  = a_map->map [a_map->umin];
   a_map->gprev  = a_map->map [a_map->umin];
   DEBUG_MAP    yLOG_value    ("gmin"      , a_map->gmin);
   /*---(grid maxs)----------------------*/
   a_map->gnext  = a_map->map [a_map->umax];
   a_map->glmax  = a_map->map [a_map->umax];
   a_map->gamax  = a_map->map [a_map->umax];
   a_map->gmax   = a_map->map [a_map->umax];
   DEBUG_MAP    yLOG_value    ("gmax"      , a_map->gmax);
   /*---(indexes)------------------------*/
   DEBUG_MAP   yLOG_snote   ("screen");
   if (a_req == YVIKEYS_INIT) {
      a_map->ubeg   = x_slot / 2;
      a_map->ucur   = a_map->ubeg;
      a_map->uend   = a_map->ubeg;
      a_map->ulen   =  1;
      a_map->uavail =  1;
      a_map->utend  =  1;
   }
   /*---(grids)--------------------------*/
   DEBUG_MAP   yLOG_snote   ("grid");
   if (a_req == YVIKEYS_INIT) {
      a_map->gbeg   = a_map->map [a_map->ubeg];
      a_map->gcur   = a_map->map [a_map->ucur];
      a_map->gend   = a_map->map [a_map->uend];
   }
   DEBUG_MAP    yLOG_value    ("gbeg"      , a_map->gbeg);
   DEBUG_MAP    yLOG_value    ("gcur"      , a_map->gcur);
   DEBUG_MAP    yLOG_value    ("gend"      , a_map->gend);
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit     (__FUNCTION__);
   return  0;
}

char
api_yvikeys_mapper      (char a_req)
{
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_view_size     (YVIKEYS_MAIN, NULL, &g_xmap.avail, NULL, &g_ymap.avail, NULL);   <*/
   my.m_xcur = g_xmap.gcur;
   my.m_zcur = g_xmap.gcur;
   my.m_ycur = g_xmap.gcur;
   api_yvikeys__unmap (&g_bmap);
   api_yvikeys__map   (a_req, &g_xmap);
   api_yvikeys__map   (a_req, &g_ymap);
   api_yvikeys__map   (a_req, &g_zmap);
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
}
