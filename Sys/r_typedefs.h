/*****************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*****************************************************************************/
/*******************************************************************************
* File Name : r_typedefs.h
* Description : This file is for type definition.
******************************************************************************/

#ifndef R_TYPEDEFS_H
#define R_TYPEDEFS_H

typedef signed   char      int8_t;
typedef signed   short     int16_t;
typedef signed   long      int32_t;
#ifndef __STDC__
typedef signed   long long int64_t;
#endif

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
#ifndef __STDC__
typedef unsigned long long uint64_t;
#endif

#ifdef __STDC__
typedef unsigned int _Bool;
#endif
 
#define bool  _Bool
//#define false 0
//#define true  1
#define FALSE 0
#define TRUE 1

typedef enum Bit{ 
	false = FALSE, 
	true = TRUE 
} bit;

#define protected_write(preg,reg,value)   preg=0xa5;\
                                          reg=value;\
                                          reg=~value;\
    				          			  reg=value;

#endif /* R_TYPEDEFS_H */
