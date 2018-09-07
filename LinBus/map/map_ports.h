#ifndef _MAP_PORTS_H
#define _MAP_PORTS_H

#include <ree_macros.h>
#include <standardports.h>

#ifndef DRIVER_LOCAL
#include <standardports_p.h>
#endif

/* ADDRESS DEFINITIONS */

#define N_PORT_BASE                  ( 0xffc10000 )
#define N_PORT_MODE_BASE             ( 0xffc10300 )
#define N_PORT_MODE_CONTROL_BASE     ( 0xffc10400 )
#define N_PORT_FUNCTION_CONTROL_BASE ( 0xffc10500 )

/* #define A_PORT_BASE                  ( 0xfffff000 ) */
/* #define A_PORT_CONTROL_BASE          ( 0xfffff020 ) */
/* #define A_PORT_MODE_CONTROL_BASE     ( 0xfffff040 ) */
                                                                   
/* NON-EXISTING PORT AREAS */

#undef PORT_3
#undef PORT_4
#undef PORT_5
#undef PORT_6
#undef PORT_7
#undef PORT_13
#undef PORT_14
#undef PORT_15
#undef PORT_16
#undef PORT_17
#undef PORT_20
#undef PORT_24
#undef PORT_25
#undef PORT_29

#undef PORT_ALPHA

/* PORT STRUCTURE */

typedef struct port_numeric {
  
  volatile unsigned short  p0;
  volatile unsigned short  RESERVED0;
  volatile unsigned short  p1;
  volatile unsigned short  RESERVED1;
  volatile unsigned short  p2;
  volatile unsigned short  RESERVED2;
  volatile unsigned short  p3;
  volatile unsigned short  RESERVED3;
  volatile unsigned short  p4;
  volatile unsigned short  RESERVED4;
  volatile unsigned short  p5;
  volatile unsigned short  RESERVED5;
  volatile unsigned short  p6;
  volatile unsigned short  RESERVED6;
  volatile unsigned short  p7;
  volatile unsigned short  RESERVED7;
  volatile unsigned short  p8;
  volatile unsigned short  RESERVED8;
  volatile unsigned short  p9;
  volatile unsigned short  RESERVED9;
  volatile unsigned short  p10;
  volatile unsigned short  RESERVED10;
  volatile unsigned short  p11;
  volatile unsigned short  RESERVED11;
  volatile unsigned short  p12;
  volatile unsigned short  RESERVED12;
  
} port_numeric;


typedef struct port_mode_numeric {
  
  volatile unsigned short pm0;       
  volatile unsigned short RESERVED0;
  volatile unsigned short pm1;
  volatile unsigned short RESERVED1;
  volatile unsigned short pm2;
  volatile unsigned short RESERVED2;
  volatile unsigned short pm3;
  volatile unsigned short RESERVED3;
  volatile unsigned short pm4;
  volatile unsigned short RESERVED4;
  volatile unsigned short pm5;
  volatile unsigned short RESERVED5;
  volatile unsigned short pm6;
  volatile unsigned short RESERVED6;
  volatile unsigned short pm7;
  volatile unsigned short RESERVED7;
  volatile unsigned short pm8;
  volatile unsigned short RESERVED8;
  volatile unsigned short pm9;
  volatile unsigned short RESERVED9;
  volatile unsigned short pm10;
  volatile unsigned short RESERVED10;
  volatile unsigned short pm11;
  volatile unsigned short RESERVED11;
  volatile unsigned short pm12;
  volatile unsigned short RESERVED12;
  
} port_mode_numeric;


typedef struct port_mode_control_numeric {
  
  volatile unsigned short pmc0;      
  volatile unsigned short RESERVED0;
  volatile unsigned short pmc1;
  volatile unsigned short RESERVED1;
  volatile unsigned short pmc2;
  volatile unsigned short RESERVED2;
  volatile unsigned short pmc3;
  volatile unsigned short RESERVED3;
  volatile unsigned short pmc4;
  volatile unsigned short RESERVED4;
  volatile unsigned short pmc5;
  volatile unsigned short RESERVED5;
  volatile unsigned short pmc6;
  volatile unsigned short RESERVED6;
  volatile unsigned short pmc7;
  volatile unsigned short RESERVED7;
  volatile unsigned short pmc8;
  volatile unsigned short RESERVED8;
  volatile unsigned short pmc9;
  volatile unsigned short RESERVED9;
  volatile unsigned short pmc10;
  volatile unsigned short RESERVED10;
  volatile unsigned short pmc11;
  volatile unsigned short RESERVED11;
  volatile unsigned short pmc12;
  volatile unsigned short RESERVED12;
  
} port_mode_control_numeric;

typedef struct port_function_control_numeric {
  
  volatile unsigned short pfc0;
  volatile unsigned short RESERVED0;
  volatile unsigned short pfc1;
  volatile unsigned short RESERVED1;
  volatile unsigned short pfc2;
  volatile unsigned short RESERVED2;
  volatile unsigned short pfc3;
  volatile unsigned short RESERVED3;
  volatile unsigned short pfc4;
  volatile unsigned short RESERVED4;
  volatile unsigned short pfc5;
  volatile unsigned short RESERVED5;
  volatile unsigned short pfc6;
  volatile unsigned short RESERVED6;
  volatile unsigned short pfc7;
  volatile unsigned short RESERVED7;
  volatile unsigned short pfc8;
  volatile unsigned short RESERVED8;
  volatile unsigned short pfc9;
  volatile unsigned short RESERVED9;
  volatile unsigned short pfc10;
  volatile unsigned short RESERVED10;
  volatile unsigned short pfc11;
  volatile unsigned short RESERVED11;
  volatile unsigned short pfc12;
  volatile unsigned short RESERVED12;
  
} port_function_control_numeric;

/* typedef struct port_alpha { */
  
/*   volatile unsigned short pal; */
/*   volatile unsigned char  pah; */
/*   volatile unsigned char  RESERVED0; */
/*   volatile unsigned short pdl; */
/*   volatile unsigned short pdh; */
/*   volatile unsigned char  pcs; */
/*   volatile unsigned char  RESERVED1; */
/*   volatile unsigned char  pct; */
/*   volatile unsigned char  RESERVED2; */
/*   volatile unsigned char  pcm; */
/*   volatile unsigned char  RESERVED3; */
/*   volatile unsigned char  pcd; */
  
/* } port_alpha; */


/* typedef struct port_mode_alpha { */
  
/*   volatile unsigned short pmal; */
/*   volatile unsigned char  pmah; */
/*   volatile unsigned char  RESERVED0; */
/*   volatile unsigned short pmdl; */
/*   volatile unsigned short pmdh; */
/*   volatile unsigned char  pmcs; */
/*   volatile unsigned char  RESERVED1; */
/*   volatile unsigned char  pmct; */
/*   volatile unsigned char  RESERVED2; */
/*   volatile unsigned char  pmcm; */
/*   volatile unsigned char  RESERVED3; */
/*   volatile unsigned char  pmcd; */
  
/* } port_mode_alpha; */


/* typedef struct port_mode_control_alpha { */
  
/*   volatile unsigned short pmcal; */
/*   volatile unsigned char  pmcah; */
/*   volatile unsigned char  RESERVED0; */
/*   volatile unsigned short pmcdl; */
/*   volatile unsigned short pmcdh; */
/*   volatile unsigned char  pmccs; */
/*   volatile unsigned char  RESERVED1; */
/*   volatile unsigned char  pmcct; */
/*   volatile unsigned char  RESERVED2; */
/*   volatile unsigned char  pmccm; */
/*   volatile unsigned char  RESERVED3; */
/*   volatile unsigned char  pmccd;  */
  
/* } port_mode_control_alpha; */


/* PORT DECLARATION */

static struct port_numeric              
             *port_numeric_p = 
                                 ( struct port_numeric * )
                                 ( N_PORT_BASE );

static struct port_mode_numeric         
             *port_mode_numeric_p = 
                                 ( struct port_mode_numeric * )
                                 ( N_PORT_MODE_BASE );

static struct port_mode_control_numeric 
             *port_mode_control_numeric_p = 
                                 ( struct port_mode_control_numeric * )
                                 ( N_PORT_MODE_CONTROL_BASE );

static struct port_function_control_numeric 
             *port_function_control_numeric_p = 
                                 ( struct port_function_control_numeric * )
                                 ( N_PORT_FUNCTION_CONTROL_BASE );

/* static struct port_alpha                */
/*              *port_alpha_p =  */
/*                                  ( struct port_alpha * )              */
/*                                  ( A_PORT_BASE ); */

/* static struct port_mode_alpha           */
/*              *port_mode_alpha_p =  */
/*                                  ( struct port_mode_alpha * )         */
/*                                  ( A_PORT_CONTROL_BASE ); */

/* static struct port_mode_control_alpha   */
/*              *port_mode_control_alpha_p =  */
/*                                  ( struct port_mode_control_alpha * ) */
/*                                  ( A_PORT_MODE_CONTROL_BASE ); */

#endif

