/*============================----beg-of-source---============================*/

/*===[[ HEADER ]]=============================================================*/

/*   focus         : (RB) robotics
 *   niche         : (hx) hexapods
 *   application   : arachne     (lydian weaver turned into the first spider)
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
 *   will allow me to test stances, movements, and interactions of a hexapod
 *   before commiting to real-world tests.
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



/*===[[ HEADER GUARD ]]===================================*/
#ifndef ARACHNE
#define ARACHNE  yes


/*===[[ VERSION ]]========================================*/
/* rapidly evolving version number to aid with visual change confirmation     */
#define VER_NUM   "0.4w"
#define VER_TXT   "progress ticker with better labels and scale shown"



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
#include  <ySTR.h>                /* heatherly string handling                */
#include  <yLOG.h>                /* heatherly logging                        */




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
/*---(data structures)-----------------*/
typedef     struct      cDEBUG      tDEBUG;
typedef     struct      cACCESSOR   tACCESSOR;
typedef     struct      cMOVE       tMOVE;
typedef     struct      cSERVO      tSERVO;
/*---(older)---------------------------*/
typedef     struct      cSEG        tSEG;
typedef     struct      cGAIT       tGAIT;



/*===[[ DEBUGGING SETUP ]]====================================================*/
/* this is my latest standard format, vars, and urgents                       */
/* v3.0b : added signal handling                                (2014-feb-01) */
struct cDEBUG
{
   /*---(handle)-------------------------*/
   int         logger;                 /* log file so that we don't close it  */
   /*---(overall)------------------------*/  /* abcdefghi_kl__opq_stu__x__    */
   /* f = full urgents turns on all standard urgents                          */
   /* k = kitchen sink and turns everything, i mean everything on             */
   /* q = quiet turns all urgents off including the log itself                */
   char        tops;                   /* t) broad structure and context      */
   char        summ;                   /* s) statistics and analytical output */
   /*---(startup/shutdown)---------------*/
   char        args;                   /* a) command line args and urgents    */
   char        conf;                   /* c) configuration handling           */
   char        prog;                   /* p) program setup and teardown       */
   /*---(file processing)----------------*/
   char        inpt;                   /* i) text and data file input         */
   char        outp;                   /* o) text and data file output        */
   /*---(event handling)-----------------*/
   char        loop;                   /* l) main program event loop          */
   char        user;                   /* u) user input and handling          */
   char        apis;                   /* i) interprocess communication       */
   char        sign;                   /* x) os signal handling               */
   char        scrp;                   /* b) scripts and batch operations     */
   char        hist;                   /* h) history, undo, redo              */
   /*---(program)------------------------*/
   char        graf;                   /* g) grahpics, drawing, and display   */
   char        data;                   /* d) complex data structure handling  */
   char        envi;                   /* e) environment processing           */
   /*---(specific)-----------------------*/
   /*---(done)---------------------------*/
};
extern tDEBUG      debug;

#define     DEBUG_TOPS          if (debug.tops      == 'y')
#define     DEBUG_SUMM          if (debug.summ      == 'y')
#define     DEBUG_ARGS          if (debug.args      == 'y')
#define     DEBUG_CONF          if (debug.conf      == 'y')
#define     DEBUG_PROG          if (debug.prog      == 'y')
#define     DEBUG_INPT          if (debug.inpt      == 'y')
#define     DEBUG_OUTP          if (debug.outp      == 'y')
#define     DEBUG_LOOP          if (debug.loop      == 'y')
#define     DEBUG_USER          if (debug.user      == 'y')
#define     DEBUG_APIS          if (debug.apis      == 'y')
#define     DEBUG_SIGN          if (debug.sign      == 'y')
#define     DEBUG_SCRP          if (debug.scrp      == 'y')
#define     DEBUG_HIST          if (debug.hist      == 'y')
#define     DEBUG_GRAF          if (debug.graf      == 'y')
#define     DEBUG_DATA          if (debug.data      == 'y')
#define     DEBUG_ENVI          if (debug.envi      == 'y')

struct cACCESSOR {
   /*---(files)----------------*/
   int         logger;         /* log file so that we don't close it          */
   /*---(window)----------*/
   char        w_title     [LEN_STR];       /* window title                   */
   int         w_height;                    /* window heigth                  */
   int         w_width;                     /* window width                   */
   /*---(file hanndling)--*/
   char        f_base      [LEN_STR];       /* specific file base name        */
   char        f_suffix    [LEN_STR];       /* file suffix for spreadsheet    */
   char        f_name      [LEN_STR];       /* full file name                 */
   /*---(spider pane)-----*/
   int         s_height;
   int         s_width;
   /*---(progress pane)---*/
   int         p_texw;                      /* texture full width             */
   int         p_texh;                      /* texture full height            */
   int         p_top;                       /* topmost y of progress bar      */
   int         p_bot;                       /* bottommost y of progress bar   */
   float       p_avail;                     /* units available on screen      */
   /*---(progress handles)*/
   uint        p_tex;                       /* texture for image              */
   uint        p_fbo;                       /* framebuffer                    */
   uint        p_depth;                     /* depth buffer                   */
   /*---(progress working)*/
   char        p_scale;                     /* scale of zoom                  */
   float       p_inc;                       /* increment for h,l              */
   char        p_base;                      /* base unit for display          */
   float       p_multi;                     /* label multiple                 */
   float       p_min;                       /* min progress bar position      */
   float       p_beg;                       /* beg of screen prog bar position*/
   float       p_cur;                       /* current progess bar position   */
   float       p_end;                       /* end of screen prog bar position*/
   float       p_max;                       /* max progress bar position      */
   /*---(done)------------*/
};
extern      tACCESSOR my;





/*===[[ DEBUGGING ]]======================================*/
#define   SILENT    0
#define   CHATTY    1
#define   DEBUG_L   if (a_debug)


#define     FILE_BLANK   "((none))"
#define     FILE_SUFFIX  "arac"

#define   FILE_GAIT     "/home/monkey/arachne.spider_visualization/arachne_gait.conf"

#define   CONF_GAIT     "/home/monkey/arachne.spider_visualization/arachne_gait.conf"
#define   CONF_SIZE     "/home/monkey/arachne.spider_visualization/arachne_size.conf"


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
extern    int       LEGS;
enum      leg_nums { L_RF=0  , L_RM=1  , L_RR=2  , L_LR=3  , L_LM=4  , L_LF=5   };
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





struct cSERVO {
   /*---(overall)------------------------*/
   char        label       [20];
   char        count;
   /*---(current)------------------------*/
   tMOVE      *curr;
   float       deg;
   /*---(list)---------------------------*/
   tMOVE      *head;
   tMOVE      *tail;
   /*---(done)---------------------------*/
};
extern      tSERVO      g_servos    [MAX_SERVO];



#define     MOVE_NULL   '-'
#define     MOVE_PAUSE  'p'
#define     MOVE_SERVO  's'

struct      cMOVE {
   int         seq;
   char        type;
   tSERVO     *servo;
   float       sec_dur;
   float       deg_beg;
   float       deg_end;
   float       sec_beg;
   float       sec_end;
   float       x_pos;
   float       y_pos;
   float       z_pos;
   tMOVE      *m_prev;
   tMOVE      *m_next;
   tMOVE      *s_prev;
   tMOVE      *s_next;
};
extern      tMOVE      *m_head;
extern      tMOVE      *m_tail;
extern      int         m_count;


/*===[[ DISPLAY LISTS ]]==================================*/
GLuint    dl_spider;
GLuint    dl_ground;
GLuint    dl_body;
GLuint    dl_coxa;
GLuint    dl_femur;
GLuint    dl_patella;
GLuint    dl_tibia;



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
int         main               (int argc, char *argv[]);

/*---(arachne_prog)----------------------*/
char        PROG_logger        (int argc, char *argv[]);
char        PROG_init          (void);
char        PROG_urgsmass      (char a_set, char a_extra);
char        PROG_urgs          (int argc, char *argv[]);
char        PROG_args          (int argc, char *argv[]);
char        PROG_begin         (void);
char        PROG_event         (void);
char        PROG_end           (void);

/*> char       *unit_accessor      (char *a_question, void *a_thing);                 <*/
char        PROG_testing       (void);
char        PROG_testloud      (void);


char        SCALE_init         (void);
char        SCALE_find         (char *a_code);
char        SCALE_smaller      (void);
char        SCALE_larger       (void);



char        MOVE_create        (char a_type, tSERVO *a_servo, float a_deg, float a_sec);
char        MOVE_curall        (float a_time);
char        MOVE_first         (int a_servo, float *a_sec, float *a_deg);
char        MOVE_next          (float *a_sec, float *a_deg);


/*---(arachne_dlist)---------------------*/
char       dlist_begin       (void);
char       dlist_end         (void);

void       glx_draw          (void);
int        glx_init          (void);
void       glx_resize        (uint, uint);


/*---(arachne_draw)----------------------*/

char      TICK_init          (void);
char      TICK_draw          (void);
char      TICK_show          (void);

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
