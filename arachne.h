/*============================----beg-of-source---============================*/

/*===[[ HEADER ]]=============================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

#define     P_FOCUS     "RO (robotics)"
#define     P_NICHE     "hx (hexapoda)"
#define     P_PURPOSE   "wickedly accurate and useful hexapod visualization and simulation"

#define     P_EXECUTE   "arachne"
#define     P_FULLPATH  "/usr/local/bin/arachne"
#define     P_ONELINE   "arachne-anassa (spider queen) work-spider simulation"

#define     P_SUFFIX    "arac"
#define     P_CONTENT   "spider movement script"

#define     P_NAMESAKE  "arachne-anassa (spider queen)"
#define     P_HERITAGE  "prideful lydian weaver who challenged the gods and lost"
#define     P_IMAGERY   "womans upside down head/torso attached to spider abdomen/legs"
#define     P_REASON    "queen of spiders would be the patron of spider robots"

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "moderate    (less than 5,000 lines of code)"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2009-07"
#define     P_DEPENDS   "yKINE, yFONT"

#define     P_VERMAJOR  "1.--, working and advancing"
#define     P_VERMINOR  "1.1-, porting to latest yVIKEYS"
#define     P_VERNUM    "1.1m"
#define     P_VERTXT    "ticker built entirely from yKINE_tick data"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"


/*===[[ METIS ]]==============================================================*/
/*
 * metis  tn4#·  add yvikeys mapping functions for x, y, and z
 * metis  tn4#·  create a yvikeys god mode for arachne and lock it in (no map mode)
 * metis  dn2#·  update yvikeys to allow ",p" to enter progress mode
 * metis  dn4··  yvikeys remap when entering progress mode
 * metis  dn4··  yvikeys progress mode to enable time movement in arachne
 *
 *
 *
 *
 */




/*===[[ HEADER ]]=============================================================*/

/*   focus         : (RB) robotics
 *   niche         : (hx) hexapods
 *   heritage      : arachne (prideful lydian weaver turned into first spider)
 *   purpose       : wickedly useful hexapod spider visualization
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : ansi-c      (wicked, limitless, universal, and everlasting)
 *   dependencies  : yKINE, opengl, yXINIT, yFONT
 *   size goal     : moderate (less than 5,000 slocL)
 *
 *   author        : heatherly
 *   created       : 2009-07 (about when we bought eva and igor)
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */
/*===[[ SUMMARY ]]============================================================*/

/*   arachne will be a wickedly accurate and useful hexapod visualizer that
 *   will allow me to deeply test stances, movements, and interactions of a
 *   spider/hexapod robot before commiting to real-world tests.
 *
 */
/*===[[ PURPOSE ]]============================================================*/

/*   arachne is a pure visualization and simulation tool for hexapod spider
 *   robotics.  given the complexity of 18+ DOF spiders, a wide variety of
 *   tools is required to make sure they perform correctly.
 *
 *   arachne will...
 *   -- accept joint/servo movement instructions from stdin
 *   -- use timing information in those instructions to understand sequencing
 *   -- produce a visualization of the result of those instructions
 *   -- allow the operator to alter and manipulate the visualization
 *   -- allow the operator to move backwards and forwards through the sequence
 *
 *   arachne will supplement kinematic debugging by...
 *   -- displaying detailed feedback from opengl to confirm positions
 *   -- optionally, produce an output file of detailed position information
 *
 *   arachne will not provide...
 *   -- means to alter servo or body positions
 *   -- means to edit scripts, instructions, or sequences
 *   -- means to connect or interact with the spiders or hexapods directly
 *   -- any kinematics calculations, those are all within yKINE
 *
 *   as always, there are many, stable, accepted, existing programs and
 *   utilities built by better programmers that are likely superior in speed,
 *   size, capability, and reliability; BUT, i would not have learned nearly as
 *   much using them, so i follow the adage...
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the underlying idea is to use this build process to learn new programming
 *   capabilities and put additional pressure on me to improve my standards,
 *   maintenance, and coding environment.  just using programs and ideas does
 *   not lead to understanding -- so get messy ;)
 *
 *   any one who wishes to become a good writer [programmer] should endeavour,
 *   before he allows himself to be tempted by the more showy qualities, to be
 *   direct, simple, brief, vigourous, and lucid -- henry watson fowler (1908)
 *
 *   simplicity is prerequisite for reliability - edsger dijkstra
 *
 */
/*============================================================================*/


/*===[[ REASONING ]]==========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/




/*===[[ HEADER GUARD ]]===================================*/
#ifndef ARACHNE
#define ARACHNE  yes


/*===[[ VERSION ]]========================================*/


/*===[[ HEADERS ]]========================================*/
/*---(ansi-c standard)-------------------*/
#include    <stdio.h>        /* CLIBC   standard input/output                 */
#include    <stdlib.h>       /* CLIBC   standard general purpose              */
#include    <string.h>       /* CLIBC   standard string handling              */
#include    <unistd.h>       /* CLIBC   standard unix interface               */
#include    <error.h>        /* CLIBC   standard error handling               */
#include    <fcntl.h>        /* CLIBC   standard file control                 */
#include    <termios.h>      /* CLIBC   standard terminal control             */
#include    <math.h>         /* CLIBC   standard math functions               */
#include    <signal.h>       /* CLIBC   standard signal handling              */
#include    <time.h>         /* CLIBC   standard time and date handling       */
#include    <ctype.h>        /* CLIBC   standard character classes            */

/*---(posix standard)--------------------*/
#include    <unistd.h>       /* POSIX   standard operating system API         */
#include    <sys/time.h>     /* POSIX   standard time access                  */

/*---(X11 standard)----------------------*/
#include    <X11/X.h>        /* X11     standard overall file                 */
#include    <X11/Xlib.h>     /* X11     standard C API                        */


/*---(opengl standard)-------------------*/
#include    <GL/gl.h>        /* OPENGL  standard primary header               */
#include    <GL/glx.h>       /* OPENGL  standard X11 integration              */

/*---(heatherly made)--------------------*/
#include    <yURG.h>         /* CUSTOM  heatherly urgent processing           */
#include    <yX11.h>         /* CUSTOM  heatherly xlib/glx setup/teardown     */
#include    <yVAR.h>         /* CUSTOM  heatherly variable testing            */
#include    <yGOD.h>         /* CUSTOM  heatherly opengl godview              */
#include    <yKINE.h>        /* CUSTOM  heatherly kinematics                  */
#include    <yGOLEM.h>       /* CUSTOM  heatherly robotics controller         */
#include    <yVIKEYS.h>      /* CUSTOM  heatherly vi_keys standard            */
#include    <yFONT.h>        /* CUSTOM  heatherly texture-mapped fonts        */
#include    <yCOLOR.h>       /* CUSTOM  heatherly opengl color handling       */
#include    <yGLTEX.h>       /* CUSTOM  heatherly texture handling            */
#include    <ySTR.h>         /* CUSTOM  heatherly string handling             */
#include    <yPARSE.h>       /* CUSTOM  heatherly record parsing/queuing      */
#include    <yLOG.h>         /* CUSTOM  heatherly program logging             */




/*---(mapping)------------------------*/
#define     HEX_CENTER     'y'
#define     HEX_INNER      'i'
#define     HEX_OUTER      'o'
#define     HEX_BORDER     '-'
#define     HEX_HATCHED    "io"
/*---(footprint)----------------------*/
#define     HEX_ABOVE      'A'
#define     HEX_OVER       'O'
#define     HEX_TOUCH      '*'
#define     HEX_UNDER      'U'
#define     HEX_BELOW      'B'
#define     HEX_FILLED     "AO*UB"



/*===[[ RATIONAL LIMITS ]]====================================================*/
/*   LEN_ is a length or size of something
 *   MIN_ is a minimum count
 *   DEF_ is a default count
 *   MAX_ is a maximum count
 *
 */
/*---(string length)------------------*/
#define     LEN_RECD    2000
#define     LEN_STR     200
#define     LEN_LABEL   20
/*---(time)---------------------------*/
#define     MAX_SCALE   50
#define     MAX_SPEED   50
/*---(legs and servos)----------------*/
#define     MAX_LEGS    8
#define     MAX_SEGS    15
#define     MAX_SERVO   32
/*---(older stuff)--------------------*/
#define     MAX_POS     5000


/*===[[ TYPEDEFS ]]===========================================================*/
/*---(basics)--------------------------*/
typedef     unsigned    char        uchar;
typedef     const       char        cchar;
typedef     unsigned    short       ushort;
typedef     const       int         cint;
typedef     unsigned    long        ulong;
typedef     unsigned    long long   ullong;
/*---(library simplifications)---------*/
typedef     struct      FILE        tFILE;
typedef     struct      tm          tTIME;
typedef     struct      timespec    tTSPEC;
/*---(data structures)-----------------*/
typedef     struct      cACCESSOR   tACCESSOR;
typedef     struct      cMOVE       tMOVE;
typedef     struct      cSERVO      tSERVO;
/*---(older)---------------------------*/
typedef     struct      cSEG        tSEG;
typedef     struct      cGAIT       tGAIT;








/*===[ MODES ]================================================================*/

/*---(screen configurations)-------------*/
#define     SCRN_NORM      'n'    /* 3d, single progress, command line        */
#define     SCRN_PROG      'p'    /* all six progress lines                   */


#define     RPTG_NONE      '-'    /* no report to generate                    */
#define     RPTG_MOVES     'm'    /* report on final move lists               */


struct cACCESSOR {
   /*---(files)----------------*/
   int         logger;         /* log file so that we don't close it          */
   /*---(mode)------------*/
   char        scrn;                        /* screen display mode            */
   char        message     [LEN_STR];       /* message line                   */
   char        sta_error;
   char        cch;
   char        sch;
   char        report;                      /* report to generate             */
   float       y;
   float       std_height;                  /* spider height at attention     */
   char        verify;                      /* verify script processing       */
   /*---(fonts)-----------*/
   char        face_pretty [LEN_LABEL];
   char        face_fixed  [LEN_LABEL];
   char        fixed;
   char        pretty;
   /*---(window sizes)----*/
   char        w_title     [LEN_STR];       /* window title                   */
   int         w_wide;                      /* window width                   */
   int         w_tall;                      /* window heigth                  */
   /*---(spider sizes)----*/
   int         s_wide;                      /* width  of spider window        */
   int         s_left;                      /* left   of spider window        */
   int         s_tall;                      /* height of spider window        */
   int         s_bott;                      /* bottom of spider window        */
   /*---(progress sizes)--*/
   int         p_wide;                      /* width  of progress ticker      */
   int         p_left;                      /* left   of progress ticker      */
   int         p_tall;                      /* height of progress ticker      */
   int         p_bott;                      /* bottom of progress ticker      */
   /*---(command sizes)---*/
   int         c_wide;                      /* width  of command line         */
   int         c_left;                      /* left   of command line         */
   int         c_tall;                      /* height of command line         */
   int         c_bott;                      /* bottom of command line         */
   /*---(command title)---*/
   int         t_wide;                      /* width  of title line           */
   int         t_left;                      /* left   of title line           */
   int         t_tall;                      /* height of title line           */
   int         t_bott;                      /* bottom of title line           */
   /*---(alt sizes)-------*/
   int         a_wide;                      /* width  of alternate window     */
   int         a_left;                      /* left   of alternate window     */
   int         a_tall;                      /* height of alternate window     */
   int         a_bott;                      /* bottom of alternate window     */
   /*---(main)------------*/
   int         m_xmin;                      /* map movement min x_pos         */
   int         m_xcur;                      /* map movement current x_pos     */
   int         m_xmax;                      /* map movement max x_pos         */
   int         m_zmin;                      /* map movement min z_pos         */
   int         m_zcur;                      /* map movement current z_pos     */
   int         m_zmax;                      /* map movement max z_pos         */
   int         m_ymin;                      /* map movement min y_pos         */
   int         m_ycur;                      /* map movement current y_pos     */
   int         m_ymax;                      /* map movement max y_pos         */
   int         m_yaw;                       /* rotation on y                  */
   int         m_pitch;                     /* rotation on x                  */
   int         m_roll;                      /* rotation on z                  */
   /*---(file hanndling)--*/
   char        f_base      [LEN_STR];       /* specific file base name        */
   char        f_suffix    [LEN_STR];       /* file suffix for spreadsheet    */
   char        f_name      [LEN_STR];       /* full file name                 */
   /*---(progress pane)---*/
   char        p_pos;                       /* position of current bar (shcle)*/
   float       p_cur;                       /* current timeline seconds       */
   int         p_tick;                      /* current ticker position        */
   float       p_scale;                     /* scale of progress bar          */
   float       p_len;                       /* length of script               */
   float       p_endsec;                    /* end second for timeline play   */
   char        p_quit;                      /* quit after initial playing     */
   int         p_nline;                     /* number of progress lines       */
   int         p_line;                      /* progress line being viewed     */
   int         p_leg;                       /* leg currently viewed           */
   int         p_texw;                      /* texture full width             */
   int         p_texh;                      /* texture full height            */
   int         p_top;                       /* topmost y of progress bar      */
   int         p_bot;                       /* bottommost y of progress bar   */
   char        p_dump;                      /* dump none, curr, or all        */
   /*---(progress handles)*/
   uint        p_tex;                       /* texture for image              */
   /*> uint        p_fbo;                       /+ framebuffer                    +/   <*/
   /*> uint        p_depth;                     /+ depth buffer                   +/   <*/
   /*---(progress working)*/
   float       p_inc;                       /* increment for h,l              */
   float       p_multi;                     /* label multiple                 */
   char        p_debug; 
   /*---(command line)----*/
   char        c_command   [LEN_STR];       /* current text in command mode   */
   char        t_text      [LEN_STR];       /* title text                     */
   /*---(focus flags)-----*/
   char        f_ground;                    /* display ground                 */
   char        f_ruler;                     /* display ruler for movements    */
   char        f_body;                      /* display body                   */
   char        f_beak;                      /* display beak                   */
   char        f_turtle;                    /* display turtle                 */
   char        f_leg       [LEN_LABEL];     /* display legs 0/RR to 5/LR      */
   char        f_footprint;                 /* display footprints             */
   char        f_angle;                     /* display leg annotations        */
   char        f_joint;                     /* display joint annotations      */
   char        f_ygod;                      /* display ygod ball and jack     */
   char        f_verify;                    /* display possible leg positions */
   /*---(done)------------*/
};
extern      tACCESSOR my;

#define     WIN_MAIN    'a'
#define     WIN_PROG    'p'







#define     FILE_BLANK   "((none))"
#define     FILE_STDIN   "stdin"
#define     FILE_SUFFIX  "arac"

#define   FILE_GAIT     "/home/monkey/arachne.spider_visualization/arachne_gait.conf"

#define   CONF_GAIT     "/home/monkey/arachne.spider_visualization/arachne_gait.conf"
#define   CONF_SIZE     "/home/monkey/arachne.spider_visualization/arachne_size.conf"


extern    char      model_name [100];
extern    char      model_desc [100];


/*===[[ CLI ARGS ]]=======================================*/
extern    char      debug_sizing;
extern    char      umake_init;
extern    char      umake_leg;
extern    float     arg_thor;
extern    float     arg_femu;
extern    float     arg_pate;
extern    float     arg_tibi;
extern    float     arg_y;


/*===[[ CONSTANTS and ENUMS ]]============================*/
extern    int       LEGS;
extern    const     float     DEG2RAD;
extern    const     float     RAD2DEG;



/*===[[ DATA STRUCTURES ]]================================*/

/*---(hexapod center)--------------------*/
/*> struct cCENTER {                                                                   <* 
 *>    float   bx, by, bz;                 /+ body center location                +/   <* 
 *>    float   mx, my, mz;                 /+ center of movement                  +/   <* 
 *>    float   yaw, pitch, roll;           /+ angles of orientation (radians)     +/   <* 
 *>    float   dyaw, dpitch, droll;        /+ angles of orientation (degrees)     +/   <* 
 *>    float   cy, sy, ny;                 /+ sin, cos, and -cos of yaw           +/   <* 
 *>    float   cp, sp, np;                 /+ sin, cos, and -cos of pitch         +/   <* 
 *>    float   cr, sr, nr;                 /+ sin, cos, and -cos of roll          +/   <* 
 *>    float   mcx, mcz, mcy;              /+ mass center coordinates             +/   <* 
 *> } center;                                                                          <*/

/*---(hexapod leg)-----------------------*/
struct cSEG {
   /*---(descriptions)-------------------*/
   char      n[12];               /* short name                               */
   float     l;                   /* joint length                             */
   float     min;                 /* min articulation                         */
   float     max;                 /* max articulation                         */
   float     mass;                /* mass of link                             */
   float     servo;               /* mass of servo                            */
   GLuint    dlist;               /* display list                             */
   /*---(lengths)------------------------*/
   float     fl;                  /* full calculated length from center       */
   float     sl;                  /* full calculated length of segment        */
   /*---(angles)-------------------------*/
   float     d;                   /* joint angle                              */
   float     h, v;                /* leg segment orientation in radians       */
   float     cd;                  /* cum joint angle                          */
   float     ch, cv;              /* cum segment orientation in radians       */
   /*---(coordinates)--------------------*/
   float     x , y , z , xz;      /* segment location                         */
   float     cx, cy, cz, cxz;     /* cumulative location                      */
   /*---(flags)--------------------------*/
   char      c;                   /* changed/updated flag                     */
   char      m;                   /* mode (i=IK, f=FK)                        */
   char      p;                   /* planted y/n?                             */
   char      rc;                  /* kinematics status : 0 = no, 1 = yes      */
};
extern    tSEG      gk [MAX_LEGS] [MAX_SEGS];    /* opengl kinematics check   */
extern    tSEG      fk [MAX_LEGS] [MAX_SEGS];    /* forward kinematics        */
extern    tSEG      ik [MAX_LEGS] [MAX_SEGS];    /* inverse kinematics        */



/*===[[ REFERENCE TEXT ]]=================================*/
extern char    legs_name [YKINE_MAX_LEGS][LEN_LABEL];
extern char    legs_long [YKINE_MAX_LEGS][LEN_LABEL];
extern float   legs_deg  [YKINE_MAX_LEGS];

extern char    segs_name [YKINE_MAX_SEGS][LEN_LABEL];
extern char    segs_long [YKINE_MAX_SEGS][LEN_LABEL];

extern float   segs_len  [YKINE_MAX_SEGS];
extern float   segs_act  [YKINE_MAX_SEGS];
extern float   segs_lnk  [YKINE_MAX_SEGS];
extern float   segs_max  [YKINE_MAX_SEGS];
extern float   segs_min  [YKINE_MAX_SEGS];


typedef  struct cLOCAUDIT  tLOCAUDIT;
struct cLOCAUDIT {
   tMOVE      *curr;
   float       sec;
   float       xpos;
   float       zpos;
   float       ypos;
};



/*===[[ DISPLAY LISTS ]]==================================*/
GLuint    dl_spider;
GLuint    dl_ground;
GLuint    dl_snowshoe;
GLuint    dl_ruler;
GLuint    dl_body;
GLuint    dl_beak;
GLuint    dl_coxa;
GLuint    dl_femur;
GLuint    dl_patella;
GLuint    dl_tibia;
GLuint    dl_foot;
GLuint    dl_verify;



struct cGAIT {
   /*---(status)-------------------------*/
   char      running;                     /* running or not                   */
   int       pos;                         /* position in gait index           */
   int       maxpos;                      /* largest position in gait         */
   int       off  [MAX_LEGS];             /* leg offset from current pos      */
   int       cur  [MAX_LEGS];             /* current position in gait         */
   char      dir  [MAX_LEGS];             /* direction of movement            */
   /*---(starting coord)-----------------*/
   float     sx   [MAX_LEGS];             /* starting x                       */
   float     sz   [MAX_LEGS];             /* starting z                       */
   float     sy   [MAX_LEGS];             /* starting y                       */
   /*---(current coord)------------------*/
   float     cx   [MAX_LEGS];             /* current  x                       */
   float     cz   [MAX_LEGS];             /* current  z                       */
   float     cy   [MAX_LEGS];             /* current  y                       */
   /*---(gait)---------------------------*/
   char      name [50];                   /* gait name                        */
   float     dxz  [MAX_POS ];             /* planned delta xz                 */
   float     dy   [MAX_POS ];             /* planned delta y                  */
   int       dphz [MAX_POS ];             /* phase number                     */
   int       dmax;                        /* number of steps in gait          */
   float     grounded;                    /* number of points on ground       */
   float     duty;                        /* duty ratio                       */
   /*---(recording)----------------------*/
   float     y    [MAX_POS ][MAX_LEGS];   /* successive heights               */
   float     cenx [MAX_POS ];             /* center x                         */
   float     cenz [MAX_POS ];             /* center z                         */
   float     ceny [MAX_POS ];             /* center y                         */
   float     coxa [MAX_POS ][MAX_LEGS];   /* successive coxa angles           */
   float     femu [MAX_POS ][MAX_LEGS];   /* successive femur angles          */
   float     pate [MAX_POS ][MAX_LEGS];   /* successive patella angles        */
   float     tibi [MAX_POS ][MAX_LEGS];   /* successive tibia angles          */
   int       touch[MAX_POS ];             /* how many legs touching           */
   int       tleg [MAX_POS ][MAX_LEGS];   /* specific legs touching           */
   float     lens [MAX_POS ][MAX_LEGS][MAX_SEGS];  /* leg segment lengths     */
   float     torq [MAX_POS ][MAX_LEGS][MAX_SEGS];  /* leg segment torque      */
   int       torqp[MAX_POS ][MAX_LEGS][MAX_SEGS];  /* leg segment torque 0-10 */
};
extern    tGAIT gait;




/*===[[ GRAPHIC ]]========================================*/
extern    float     my_x;
extern    float     my_y;
extern    float     my_z;
extern    float     my_vx;
extern    float     my_vy;
extern    float     my_vz;
extern    float     my_roll;
extern    float     my_yaw;
extern    float     my_pitch;
extern    float     my_vroll;
extern    float     my_vyaw;
extern    float     my_vpitch;

extern    float     counter;
extern    float     my_calf;
extern    int       my_curr;

extern    float     my_len;
extern    float     my_ppos;
extern    float     my_run;
extern    float     my_inc;
extern    float     my_deg;



extern    char        flag_annotate;
extern    char        flag_view;







/*===[[ PROTOTYPES ]]=========================================================*/
/*---(arachne_main)----------------------*/
int         main               (int argc, char *argv[]);

/*---(arachne_prog)----------------------*/
char        PROG_init          (void);
char        PROG_args          (int argc, char *argv[]);
char        PROG_begin         (void);
char        PROG_final         (void);
char        PROG_end           (void);

/*> char       *unit_accessor      (char *a_question, void *a_thing);                 <*/
char        PROG_testing       (void);
char        PROG_testloud      (void);




char        MOVE_create        (char a_type, tSERVO *a_servo, char *a_label, int a_line, float a_deg, float a_sec);
char        MOVE_repeat        (tSERVO *a_servo, int a_count, int a_times);
char        MOVE_dalsegno      (tSERVO *a_servo, int a_times);
char        MOVE_curleg        (float a_time, int a_leg);


/*---(mode keys)------------*/
char        MODE_god           (char a_major, char a_minor);
char        MODE_progress      (char a_major, char a_minor);


/*---(arachne_dlist)---------------------*/
char       dlist_begin             (void);
char       dlist_end               (void);
char       dlist_distances         (float x, float z);
char       dlist_littlehex         (float x, float z);
char       dlist_hex               (int l, char c, float x, float z);
char       dlist_footprint         (int l, char t, float x, float z, float y, char rc, int c, int r, float d, float o);

void       glx_draw                (void);
int        glx_init                (void);


/*---(arachne_draw)----------------------*/

/*---1----- -----2----- -----3----- -----4-----  ---------comments------------*/
char        TICK_init               (void);
char        TICK_draw               (void);
char        TICK_draw_all           (void);
char        TICK_show               (void);
char        TICK_legend             (void);
char        TICK_snap               (void);
char        TICK_panel_dump         (void);


char      DRAW_init               (void);
char      DRAW_begin         (void);      /* prepare drawing environment      */
char      DRAW_reset         (void);      /* set starting point for drawing   */
char      DRAW_end           (void);      /* teardown drawing environment     */

char      DRAW_primary            (void);

char      draw_setup         (void);
char      draw_prep          (void);
char      draw_main          (void);
void      draw_spider        (void);

char      draw_body          (tSEG *a_curr);
void      view_leg           (void);

void       draw_ground       (void);
void       draw_axis         (void);
void       draw_radians      (void);
void       draw_arrow        (void);
void       draw_router       (void);
void       draw_masscenter   (void);
void       draw_contact      (void);
void       draw_3d_orient    (void);
void       draw_3d_locate    (void);

void       position_draw     (int);
char       view_setup        (char *a_title, char *a_desc);
void       view_3d           (void);
/*> void       view_top          (void);                                              <*/
/*> void       view_side         (void);                                              <*/

char       view_gait         (void);
char       view_touch        (void);
char       view_hildebrand   (void);
char       view_step         (void);
char       view_waves        (void);
char       view_torque       (void);


/*---(new)------------------------------------*/
void       leg_init          (tSEG a_leg);
/*> void       leg_ik            (int a_num, struct cFK *a_leg, float a_x, float a_y, float a_z);   <*/


/*---(kinematics)---------------------*/
/*> char      kine_init          (void);                                              <*/
/*> char      kine_clear         (char *a_name, tSEG *a_curr, int a_nleg, int a_nseg);   <*/



/*---1----- -----2----- -----3----- -----4-----  ---------comments------------*/
char        KINE_begin              (void);
char        KINE_end                (void);
char        KINE_write              (int   a_leg);
char        KINE_compare            (int   a_seg);
char        KINE_unitcond           (void);
char        KINE_unitcondall        (void);
char        KINE_unitcond_ik        (void);
/*---1----- -----2----- -----3----- -----4-----  ---------comments------------*/


char        api_yvikeys_init        (void);
char        api_yvikeys__unmap      (tMAPPED *a_map);
char        api_yvikeys__map        (char a_req, tMAPPED *a_map);
char        api_yvikeys_mapper      (char a_req);
char        api_yvikeys_focus       (char *a_name, char *a_option);


/*---(scripting)----------------------*/





/*---(unit testing)---------------------------------------*/
char     *unit_accessor      (char*, int, int);

char      font_load          (void);
char      font_delete        (void);



/*---(statics)--------------------------------------------*/
char      stat_init          (char  *a_model);
char      stat_masscenter    (void);
char      stat_torque        (int   a_pos , int   a_leg);
float     stat_settle        (void);



char      FILE_init               (void);
char      FILE_prepper            (char a_pass);
char      FILE_finisher           (char a_pass);
char      FILE_new                (void);


#endif

/*============================----end-of-source---============================*/
