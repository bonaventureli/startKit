#ifndef _REE_TYPES_H
#define _REE_TYPES_H
#define _NEC_TYPES_H

#ifndef EE_API
#define EE_API
#endif
#include "r_typedefs.h"
#define TRUE    1
#define FALSE   0



//typedef enum Bit{ 
//	false = FALSE, 
//	true = TRUE 
//} bit;

#ifndef _TYPEDEF_H_
#define EE_NULL (void *)0
typedef bit ee_Bool_t;
#ifndef uint8_t
//typedef signed char        int8_t;
//typedef signed short       int16_t;  
//typedef signed int         int32_t; 
#ifdef __ghs__
//typedef signed long long   int64_t;  
//typedef unsigned long long uint64_t;  
#endif
//typedef unsigned char      uint8_t;  
//typedef unsigned short     uint16_t; 
//typedef unsigned int       uint32_t; 
#endif /* uint8_t */
#endif /* _TYPEDEF_H_ */

typedef bit bit_t;

typedef unsigned char  u08;
typedef unsigned char  u08_t;
typedef unsigned short u16;
typedef unsigned short u16_t;
typedef unsigned long  u32;
typedef unsigned long  u32_t;

typedef signed char  s08;
typedef signed char  s08_t;
typedef signed short s16;
typedef signed short s16_t;
typedef signed long  s32;
typedef signed long  s32_t;

typedef float       flt;
typedef float       flt_t;
typedef double      dbl;
typedef double      dbl_t;
typedef long double lgd;
typedef long double lgd_t;

typedef unsigned char  * pu08;
typedef unsigned char  * pu08_t;
typedef unsigned short * pu16;
typedef unsigned short * pu16_t;
typedef unsigned long  * pu32;
typedef unsigned long  * pu32_t;

typedef signed char  * ps08;
typedef signed char  * ps08_t;
typedef signed short * ps16;
typedef signed short * ps16_t;
typedef signed long  * ps32;
typedef signed long  * ps32_t;

typedef float       * pflt;
typedef float       * pflt_t;
typedef double      * pdbl;
typedef double      * pdbl_t;
typedef long double * plgd;
typedef long double * plgd_t;

#endif
