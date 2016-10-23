/*============================----beg-of-source---============================*/

/*===[[ HEADER ]]=============================================================*

 *   focus         : (RB) robotics
 *   niche         : (hx) hexapods
 *   application   : arachne     (lydian weaver turned into the first spider)
 *   purpose       : wickedly useful hexapod stance and movement simulator
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : gnu/ansi-c  (right, just, best, standard, and everlasting)
 *   dependencies  : opengl, yXINIT, yFONT
 *   size goal     : moderate (less than 5,000 slocL)
 *
 *   author        : the_heatherlys
 *   created       : 2009-07 (about when we bought eva and igor)
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */
/*===[[ SUMMARY ]]============================================================*

 *   arachne will be a wickedly accurate hexapod simulator that will allow us to
 *   design stances, movements, and interactions of a CH3-R hexapod as well as
 *   generate scripts to test these theories in the physical world.
 *
 */
/*===[[ PURPOSE ]]============================================================*

 *   arachne is a set of functions to provide highly reliable, accurate, and
 *   predictable kinematic calculations for use in the control of a lynxmotion
 *   CHR-3 robotic hexapod with 18 DOF.  because the true location and
 *   orientation of the hexapod is a combination of these 18 DOF, machine
 *   control is very complex and most people resort to sticking with tradition,
 *   excel models, and/or trial and error to develop gaits and motions.
 *
 *   we must learn to master the kinematics involved from the ground up.  there
 *   are a great number of amazing experts out there pointing the way, but we
 *   can't fall into the trap of just becoming consumers and users of their
 *   great work -- build on their foundation and add your own creativity.
 *
 *   the goal of arachne is to take a painful, tedious, onerous, haphazard, and
 *   error prone process and turn it a easy, flexible, predictable, rapid, and
 *   automated set of functions.
 *
 *   arachne's core will focus on...
 *      - maintaining kinematic master data on body, legs, and other attachments
 *      - forward leg kinematics predicting x,y,z endpoint based on joint angles
 *      - inverse leg kinematics predicting joint angles based on x,y,z target
 *      - body movements to shift the relative leg positions
 *      - body orientation shifting almost all other calculations
 *      - adjustment relative to the ground to understand actual positioning
 *
 *   arachne will then build on top the ability to...
 *      - accept fixed positions and stances from input files
 *      - accept gait and movement designs from input files
 *      - accept scripts that combine stances and movements from input files
 *      - play and step through the scripts to test out the results visually
 *      - generate specific robot scripts to drive the hexapod
 *
 *   arachne will then provide the ability...
 *      - visualize a wireframe model of the hexapod in a simple 3d environment
 *      - assume designed stances with appropriate context and information
 *      - display internal kinetic states and positions on demand
 *
 *   arachne will also provide...
 *      - debugging capability to display key information on the process
 *      - string-formatted kinetic data in easily displayable, consumable format
 *      - deep unit testing interfaces to prove computational correctness
 *
 *   arachne will NOT provide...
 *      - ability to edit stances, gaits, movements, or scripts (that's vim)
 *      - ability to drive the hexapod directly (that's a specific application)
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
/*===[[ DECISION -- TO CODE, MODEL, OR MANUAL ]]==============================*

 *  PROBLEM (pr)
 *     in order to be able to do anything unique with jointed robots such as
 *     hexapods, robotic arms, or even stranger; we will have to develop our
 *     own capability for modeling and control.  if we only wanted to use
 *     existing gaits and positions for show, this is not a problem, but we
 *     view jointed robots as useful to our future, to we must press on.
 *
 *     unfortunately kinematics is quite a complex and detailed field of study
 *     that will require much time and effort to develop sufficient skill to
 *     drive the creation of our own hexapod movements, gaits, and other control
 *
 *     if the future is about delegation of known tasks to automation, both
 *     virtual and physical, then we have to be able to handle custom robotics.
 *     since the world can not be fully controlled and the applications of the
 *     automation with be variable, we have to be able to adapt.
 *
 *     so, we agree that we need to have direct control and knowledge in the
 *     long-term, the current decision is whether to dive in now or wait
 *
 *
 *  OBJECTIVES (o)
 *     - treat the application of our hexapods as vital to our future
 *     - make it do basic and simple things early so we build a foundation
 *     - learn the application of kinematics early in our studies
 *     - have a platform that can be expanded and grown
 *     - learn new and wicked ways to build programming capabilities
 *     - learn to computer-control robots remotely (not a phucking joystick)
 *     - push us hard to be better and more creative developers
 *
 *
 *  ALTERNATIVES and CONSEQUENCES (ac)
 *
 *     1) use the built-in hexapod capabilities...
 *        - focus on what is current, rather than possible
 *        - no work required as it's already done and working
 *        - can only stand, walk under direction, turn around, and sit
 *        - would be nothing more than a curiousity (at bloody best)
 *
 *     2) use the excel sheets that others have developed...
 *        - already built, but need to port to CH3-R hexapod
 *        - can even help develop new gaits
 *        - still only helps look better but not do anything useful
 *        - can not get beyond a cooler factor of curiousity
 *
 *     3) buy an upgraded hexapod with programmed brains...
 *        - already built and running like getting a new puppy
 *        - has the ability to react to movement in spider-like ways
 *        - doesn't do anything useful beyond very cool recognition
 *        - costs a ton of money and is not expandable (dead end)
 *
 *     4) build a programming simulator...
 *        - can do anything i can figure out or dream (mostly)
 *        - will allow me to develop features like hexapod drillpress
 *        - gonna take a ton of time (but i need to do it sooner or later)
 *        - must learn many new tools (but, will learn many new tools)
 *
 *     5) give up for now...
 *        - no additional effort
 *        - no additional cost
 *        - get back into basic learning
 *        - focus on other priorities
 *
 *
 *  JUDGEMENT
 *
 *     i have already benefited from working with the hexapod simulator by
 *     learning a good deal about opengl, xwindows, and simulation.  it could
 *     be argued that i would never learn these things without a great and
 *     grand project to push me, so excellent to date.
*
*     going forward, there is great promise to automating physical activities
*     from making to survailance to assistance.  the sooner i start the
*     further i will get.
*
*/
/*===[[ TERMINOLOGY ]]========================================================*

 *  hexapod       : a six-legged robot
 *  core          : the very center point of the hexapod
 *  thorax        : the round center of the CHR-3 hexapod
 *  coxa          : the small fixed hips attached to body (later my articulate)
 *  femur         : the next joint that moves horizontally
 *  patella       : the next joint that moves vertically
 *  tibia         : the last joint that moves vertically
 *  tarsus        : currently non-existant and might become a foot, claw, ...
 *
 */
/*============================================================================*/



/*===[[ HEADER GUARD ]]===================================*/
#ifndef ARACHNE
#define ARACHNE  yes


/*===[[ VERSION ]]========================================*/
/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "0.4a"
#define VER_TXT   "start total rework by adding to github"



/*===[[ HEADERS ]]========================================*/
/*---(ansi-c standard)-------------------*/
#include  <stdio.h>               /* clibc  standard input/output             */
#include  <stdlib.h>              /* clibc  standard general purpose          */
#include  <string.h>              /* clibc  standard string handling          */
#include  <unistd.h>              /* clibc  standard unix interface           */
#include  <error.h>               /* clibc  standard error handling           */
#include  <fcntl.h>               /* clibc  standard file control             */
#include  <termios.h>             /* clibc  standard terminal control         */
#include  <math.h>                /* clibc  standard math functions           */
#include  <signal.h>              /* clibc  standard signal handling          */
#include  <time.h>                /* clibc  standard time and date handling   */
#include  <ctype.h>               /* clibc  standard character classes        */

/*---(posix standard)--------------------*/
#include  <unistd.h>              /* POSIX  standard operating system API     */
#include  <sys/time.h>            /* POSIX  standard time access              */

/*---(X11 standard)----------------------*/
#include  <X11/X.h>               /* X11    standard overall file             */
#include  <X11/Xlib.h>            /* X11    standard C API                    */


/*---(opengl standard)-------------------*/
#include  <GL/gl.h>               /* opengl standard primary header           */
#include  <GL/glx.h>              /* opengl standard X11 integration          */

/*---(heatherly made)--------------------*/
#include  <yX11.h>                /* heatherly xlib/glx setup/teardown        */
#include  <yVAR.h>                /* heatherly variable testing               */
#include  <yGOD.h>                /* heatherly opengl godview                 */
#include  <yFONT.h>               /* heatherly texture-mapped fonts           */



/*===[[ CUSTOM TYPES ]]===================================*/
#define   PRIV   static
typedef   unsigned long ulong;
typedef   unsigned int  uint;
typedef   unsigned char uchar;

typedef   struct FILE   tFILE;



/*===[[ DEBUGGING ]]======================================*/
#define   SILENT    0
#define   CHATTY    1
#define   DEBUG_L   if (a_debug)



#define   CONF_GAIT     "/home/monkey/arachne.hexapod_robotics/arachne_gait.conf"
#define   CONF_SIZE     "/home/monkey/arachne.hexapod_robotics/arachne_size.conf"


extern    char      model_name [100];
extern    char      model_desc [100];


/*===[[ CLI ARGS ]]=======================================*/
extern    char      debug_sizing;
extern    char      moving;
extern    char      umake_setup;
extern    char      umake_init;
extern    char      umake_leg;
extern    float     arg_thor;
extern    float     arg_femu;
extern    float     arg_pate;
extern    float     arg_tibi;
extern    float     arg_y;


/*===[[ CONSTANTS and ENUMS ]]============================*/
#define   MAX_LEGS   8
extern    int       LEGS;
enum      leg_nums { L_RF=0  , L_RM=1  , L_RR=2  , L_LR=3  , L_LM=4  , L_LF=5   };
#define   MAX_SEGS   15
enum      seg_nums { CORE=0, THOR=1, COXA=2, FEMU=3, PATE=4, TIBI=5, TARS=6, FOOT=7, TOES=8, TARG=9, ORIG=10, VERT=11, CALC=12 };
extern    const     double    DEG2RAD;
extern    const     double    RAD2DEG;



/*===[[ DATA STRUCTURES ]]================================*/

/*---(hexapod center)--------------------*/
struct cCENTER {
   float   bx, by, bz;                 /* body center location                */
   float   mx, my, mz;                 /* center of movement                  */
   float   yaw, pitch, roll;           /* angles of orientation (radians)     */
   float   dyaw, dpitch, droll;        /* angles of orientation (degrees)     */
   float   cy, sy, ny;                 /* sin, cos, and -cos of yaw           */
   float   cp, sp, np;                 /* sin, cos, and -cos of pitch         */
   float   cr, sr, nr;                 /* sin, cos, and -cos of roll          */
   float   mcx, mcz, mcy;              /* mass center coordinates             */
} center;

/*---(hexapod leg)-----------------------*/
typedef struct cSEG  tSEG;
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
extern char    legs_name [MAX_LEGS][ 5];
extern char    legs_long [MAX_LEGS][25];
extern float   legs_deg  [MAX_LEGS];

extern char    segs_name [MAX_SEGS][ 5];
extern char    segs_caps [MAX_SEGS][ 5];
extern char    segs_long [MAX_SEGS][25];

extern float   segs_len  [MAX_SEGS];
extern float   segs_act  [MAX_SEGS];
extern float   segs_lnk  [MAX_SEGS];
extern float   segs_max  [MAX_SEGS];
extern float   segs_min  [MAX_SEGS];




/*===[[ DISPLAY LISTS ]]==================================*/
GLuint    dl_spider;
GLuint    dl_ground;
GLuint    dl_body;
GLuint    dl_coxa;
GLuint    dl_femur;
GLuint    dl_patella;
GLuint    dl_tibia;


#define   MAX_POS     5000

typedef struct cGAIT tGAIT;
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
extern    float     my_pos;
extern    float     my_ppos;
extern    float     my_run;
extern    char      my_mode;
extern    float     my_inc;
extern    float     my_deg;

extern    char   face_bg [30];
extern    char   face_sm [30];
extern    int    txf_bg;
extern    int    txf_sm;


extern    char        flag_annotate;
extern    char        flag_view;







/*===[[ PROTOTYPES ]]=========================================================*/
/*---(arachne_main)----------------------*/
int        main              (int argc, char *argv[]);
char       prog_args         (int argc, char *argv[]);
char       prog_begin        (void);
char       prog_event        (void);
char       prog_end          (void);

/*---(arachne_dlist)---------------------*/
char       dlist_begin       (void);
char       dlist_end         (void);

void       glx_draw          (void);
int        glx_init          (void);
void       glx_resize        (uint, uint);


/*---(arachne_draw)----------------------*/
char      draw_begin         (void);      /* prepare drawing environment      */
char      draw_reset         (void);      /* set starting point for drawing   */
char      draw_end           (void);      /* teardown drawing environment     */


char      draw_setup         (void);
char      draw_prep          (void);
void      draw_main          (void);

char      draw_leg           (int  a_num, tSEG  a_curr[], char a_loc);
char      draw_body          (tSEG *a_curr);
void      view_leg           (void);

void       draw_ground       (void);
void       draw_axis         (void);
void       draw_radians      (void);
void       draw_arrow        (void);
void       draw_masscenter   (void);
void       draw_contact      (void);
void       draw_3d_orient    (void);
void       draw_3d_locate    (void);

void       position_draw     (int);
char       view_setup        (char *a_title, char *a_desc);
void       view_3d           (void);
void       view_top          (void);
void       view_side         (void);

char       view_gait         (void);
char       view_touch        (void);
char       view_hildebrand   (void);
char       view_step         (void);
char       view_waves        (void);
char       view_torque       (void);

void       view_fk           (void);
void       view_ik           (void);
void       view_unit         (void);
void       view_fkline       (int a_leg, int a_seg, char* a_name);
void       view_ikline       (int a_leg, int a_seg, char* a_name);
void       view_iikline      (int a_leg, int a_seg, char* a_name);

/*---(new)------------------------------------*/
void       leg_init          (tSEG a_leg);
/*> void       leg_ik            (int a_num, struct cFK *a_leg, float a_x, float a_y, float a_z);   <*/


/*---(kinematics)---------------------*/
char      kine_init          (void);
char      kine_clear         (char *a_name, tSEG *a_curr, int a_nleg, int a_nseg);
char      kine_header        (void);
char      kine_print         (tSEG *a_curr, tSEG *a_comp);
char      kine_forward       (int, float, float, float, float);
char      kine_inverse       (int, float, float, float, float, char);
char      kine_max           (float, float, float);


int       arachne_init       (void);

char      kine_center        (float a_x, float a_z);
char      kine_height        (float a_y);
char      kine_pivot         (float a_x, float a_z);
char      kine_attitude      (float, float, float);


void       kinetics_fk_print (int, int, char*);
void       kinetics_ik_print (int, int, char*);

int        ik_femur          (int);
int        kinetics_lower    (int);
int        kinetics_ik_lower (int, float, float);

int        kinetics_orient   (int, int);

int        kinetics_body     (void);


/*---(scripting)----------------------*/
char      pose_spread        (void);
char      pose_attention     (void);
char      pose_high          (void);
char      pose_low           (void);
char      pose_crab          (void);

char      scrp_init          (void);
char      scrp_vertical      (int  a_pos);
char      scrp_hulahoop      (int  a_pos);
char      scrp_sideside      (int  a_pos);
char      scrp_tripod        (int  a_pos);
char      scrp_wave          (int  a_pos);
char      scrp_quad          (int  a_pos);
char      scrp_position      (int  a_pos);


char      gait_clear         (void);
char      gait_square        (void);
char      gait_curved        (void);
char      gait_read          (void);


/*---(unit testing)---------------------------------------*/
char     *unit_accessor      (char*, int, int);
int       kinetics_scripter  (void);
char      unit_SCRP          (char*, char*);
char      unit_init          (void);
char      unit_FK            (void);
char      unit_IK            (void);

char      font_load          (void);
char      font_delete        (void);



/*---(statics)--------------------------------------------*/
char      stat_init          (char  *a_model);
char      stat_masscenter    (void);
char      stat_torque        (int   a_pos , int   a_leg);
float     stat_settle        (void);



#endif

/*============================----end-of-source---============================*/
