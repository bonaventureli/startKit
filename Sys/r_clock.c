/*******************************************************************************
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
* (c) 2014 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/


/******************************************************************************
* File Name     : r_clock.c
* Version       : 1.0
* Device(s)     : R7F701035xAFP RH850/F1L
* Tool-Chain    : CubeSuite+(V2.01.00)
* Description   : This file is a sample of the clock setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 20.03.2014 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include    "r_typedefs.h"
#include    "iodefine.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
        void    R_CLOCK_Init( void );


/******************************************************************************
Private global variables and functions
******************************************************************************/
static  void    osc_clock( void );
static  void    set_clock_domain( void );


/******************************************************************************
* Function Name : void R_CLOCK_Init( void )
* Description   : This function initializes the clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_CLOCK_Init( void )
{
    osc_clock();                                /* Clock oscillation         */
    set_clock_domain();                         /* Domain clock setting      */
}


/******************************************************************************
* Function Name : static void osc_clock( void )
* Description   : This function oscillates the clock (main , sub and PLL clock).
* Argument      : none
* Return Value  : none
******************************************************************************/
static void osc_clock( void )
{
	/* Prepare 16MHz MainOsz */
  MOSCC=0x05;                            // Set MainOSC gain (8MHz < MOSC frequency =< 16MHz)
  MOSCST=0xFFFF;                         // Set MainOSC stabilization time to max (8,19 ms)
  
  //protected_write(PROTCMD0,MOSCE,0x01);  // Trigger Enable (protected write)	
  PROTCMD0=0xa5;
  MOSCE=0x01;
  MOSCE=~0x01;
  MOSCE=0x01;
  while ((MOSCS&0x04) != 0x4);           // Wait for aktive MainOSC
  
  /* Prepare PLL*/
  PLLC=0x00000a27;                       //16 MHz MainOSC -> 80MHz PLL 
  //protected_write(PROTCMD1,PLLE,0x01);   //enable PLL
  PROTCMD1=0xa5;
  PLLE=0x01;
  PLLE=~0x01;
  PLLE=0x01;
  while((PLLS&0x04) != 0x04);            //Wait for aktive PLL
  
  /* CPU Clock devider = /1 */
  //protected_write(PROTCMD1,CKSC_CPUCLKD_CTL,0x01);
  PROTCMD1=0xa5;
  CKSC_CPUCLKD_CTL=0x01;
  CKSC_CPUCLKD_CTL=~0x01;
  CKSC_CPUCLKD_CTL=0x01;
  while(CKSC_CPUCLKD_ACT!=0x01);

  /* PLL -> CPU Clock */ 
  //protected_write(PROTCMD1,CKSC_CPUCLKS_CTL,0x03);
  PROTCMD1=0xa5;
  CKSC_CPUCLKS_CTL=0x03;
  CKSC_CPUCLKS_CTL=~0x03;
  CKSC_CPUCLKS_CTL=0x03;
  while(CKSC_CPUCLKS_ACT!=0x03);
  
}


/******************************************************************************
* Function Name : static void set_clock_domain( void )
* Description   : This function sets the domain clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
static void set_clock_domain( void )
{
    uint32_t    reg32_value;

	#if 0
    /* Source Clock Setting for C_ISO_CPUCLK
    CKSC_CPUCLKS_CTL - C_ISO_CPUCLK Source Clock Selection Register
    b31:b 2                        - Reserved set to 0
    b 1:b 0            CPUCLKSCSID - Source Clock Setting for C_ISO_CPUCLK - CPPLLCLK */
    do
    {
        reg32_value                = 0x00000003UL;
        PROTCMD1                   = 0x000000A5UL;   /* Protection release the CKSC_CPUCLKS_CTL register */
        CKSC_CPUCLKS_CTL           = reg32_value;
        CKSC_CPUCLKS_CTL           = ~reg32_value;
        CKSC_CPUCLKS_CTL           = reg32_value;
    } while ( PROTS1 != 0x00000000UL );
    while ( CKSC_CPUCLKS_ACT != reg32_value )
    {
        /* Wait for CKSC_CPUCLKS_CTL to set. */
    }

    /* Clock Divider Setting for C_ISO_CPUCLK
    CKSC_CPUCLKD_CTL - C_ISO_CPUCLK Divided Clock Selection Register
    b31:b 2                        - Reserved set to 0
    b 1:b 0            CPUCLKDCSID - Source Clock Setting for C_ISO_CPUCLK - CKSC_CPUCLKS_CTL selection / 1 */
    do
    {
        reg32_value                = 0x00000001UL;
        PROTCMD1                   = 0x000000A5UL;   /* Protection release the CKSC_CPUCLKD_CTL register */
        CKSC_CPUCLKD_CTL           = reg32_value;
        CKSC_CPUCLKD_CTL           = ~reg32_value;
        CKSC_CPUCLKD_CTL           = reg32_value;
    } while ( PROTS1 != 0x00000000UL );
    while ( CKSC_CPUCLKD_ACT != reg32_value )
    {
        /* Waiting for CKSC_CPUCLKD_CTL to set. */
    }
	#endif
	
    /* Source Clock Setting for C_ISO_PERI1
    CKSC_IPERI1S_CTL - C_ISO_PERI1 Source Clock Selection Register
    b31:b 2                        - Reserved set to 0
    b 1:b 0            IPERI1SCSID - Source Clock Setting for C_ISO_PERI1 - PPLLCLK */
    do
    {
        reg32_value                = 0x00000002UL;
        PROTCMD1                   = 0x000000A5UL;   /* Protection release the CKSC_IPERI1S_CTL register */
        CKSC_IPERI1S_CTL           = reg32_value;
        CKSC_IPERI1S_CTL           = ~reg32_value;
        CKSC_IPERI1S_CTL           = reg32_value;
    } while ( PROTS1 != 0x00000000UL );
    while ( CKSC_IPERI1S_ACT != reg32_value )
    {
        /* Waiting for CKSC_IPERI1S_CTL to set. */
    }

    /* Source Clock Setting for C_ISO_PERI2
    CKSC_IPERI2S_CTL - C_ISO_PERI2 Source Clock Selection Register
    b31:b 2                        - Reserved set to 0
    b 1:b 0            IPERI2SCSID - Source Clock Setting for C_ISO_PERI2 - PPLLCLK2 */
    do
    {
        reg32_value                = 0x00000002UL;
        PROTCMD1                   = 0x000000A5UL;   /* Protection release the CKSC_IPERI2S_CTL register */
        CKSC_IPERI2S_CTL           = reg32_value;
        CKSC_IPERI2S_CTL           = ~reg32_value;
        CKSC_IPERI2S_CTL           = reg32_value;
    } while ( PROTS1 != 0x00000000UL );
    while ( CKSC_IPERI2S_ACT != reg32_value )
    {
        /* Waiting for CKSC_IPERI2S_CTL to set. */
    }
}

