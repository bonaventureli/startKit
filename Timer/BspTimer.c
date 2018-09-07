/******************************************************************************
* File Name     : r_taud_interval.c
* Version       : 1.0
* Device(s)     : R7F701035xAFP RH850/F1L
* Tool-Chain    : CubeSuite+(V2.01.00)
* Description   : This file is a sample of the interval timer (TAUD).
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 20.03.2014 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include 		"BspTimer.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
        void    R_TAUD_Init( void );
        void    R_TAUD_Start( void );


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name : void R_TAUD_Init( void )
* Description   : This function initializes the TAUD ch.0. (interval timer function)
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_TAUD_Init( void )
{
    /* Set interrupt flags */
    MKTAUD0I0              = 1U;
    RFTAUD0I0              = 0U;
    TBTAUD0I0              = 0U;


    /* Specifies the CK3 , CK2 , CK1 and CK0 clock
    TAUDnTPS   - TAUDn Prescaler Clock Select Register
    b15:b12      TAUDnPRS3 - Specifies CK3_PRE clock - PCLK / 1
    b11:b 8      TAUDnPRS2 - Specifies CK2_PRE clock - PCLK / 1
    b 7:b 4      TAUDnPRS1 - Specifies CK1_PRE clock - PCLK / 1
    b 3:b 0      TAUDnPRS0 - Specifies CK0_PRE clock - PCLK / 4096 */
    TAUD0.TPS              = 0x000CU;


    /* Data register for the capture/compare value
    TAUDnCDRm  - TAUDn Channel Data Register
    b15:b 0      TAUDnCDR  - 1ms */
    TAUD0.CDR0             = 10U - 1U;//20U - 1U;         /* INTTAUDnIm cycle = count clock cycle * (TAUDnCDRm + 1) */


    /* Controls channel m operation
    TAUDnCMORm - TAUDn Channel Mode OS Register
    b15:b14      TAUDnCKS  - Selects an operation clock.                  - Selected Operation CK0
    b13:b12      TAUDnCCS0 - Selects a count clock for TAUDnCNTm counter. - Uses an operation clock as a count clock.
    b11          TAUDnMAS  - Specifies whether the channel is a master
                             channe or slave channel during synchronous
                             channel operation.                           - Independent operation, Set to 0.
    b10:b 8      TAUDnSTS  - Selects an external start trigger.           - Triggers the counter by software.
    b 7:b 6      TAUDnCOS  - Specifies the timing for updating capture
                             register TAUDnCDRm and overflow flag
                             TAUDnCSRm.TAUDnOVF of channel m.             - Unused. Set to 00'b.
    b 5                    - Reserved set to 0
    b 4:b 1      TAUDnMD   - Specifies an operating mode.                 - Interval timer mode. Set to 0000'b
    b 0          TAUDnMD0  - Specifies whether INTTAUDnIm is generated
                             at the beginning of count operation (when
                             a start trigger is entered) or not.          - INTTAUDnIm is not generated to toggle
                                                                            TAUDTTOUTm at the beginning of an
                                                                            operation. */
    TAUD0.CMOR0            = 0x0000U;


    /* Specifies the type of valid edge detection used for the TAUDTTINm input.
    TAUDnCMURm - TAUDn Channel Mode User Register
    b 7:b 2                - Reserved set to 0
    b 1:b 0      TAUDnTIS  - Specifies a valid edge of TAUDTTINm input
                             signal.                                      - Unused. Set to 00'b. */
    TAUD0.CMUR0            = 0x00U;


    /* The simultaneous rewrite registers (TAUDnTOM, TAUDnTOC, TAUDnTOL, TAUDnTDE , TAUDnTDM , TAUDnTDL
       TAUDnTRE, TAUDnTRO, TAUDnTRC, TAUDnTME) cannot be used with the Interval Timer Function.
       Therefore, these registers must be set to 0. */
    TAUD0.TOM              &= 0xFFFEU;
    TAUD0.TOC              &= 0xFFFEU;
    TAUD0.TOL              &= 0xFFFEU;
    TAUD0.TDE              &= 0xFFFEU;
    TAUD0.TDM              &= 0xFFFEU;
    TAUD0.TDL              &= 0xFFFEU;

    TAUD0.TRE              &= 0xFFFEU;
    TAUD0.TRO              &= 0xFFFEU;
    TAUD0.TRC              &= 0xFFFEU;
    TAUD0.TME              &= 0xFFFEU;


    /* Disable the channel output mode.
    TAUDnTOE   - TAUDn Channel Output Enable Register
    b15:b 0      TAUDnTOEm - Enables/disables the independent channel
                             output function.                             - Disables the independent timer output
                                                                            function (controlled by software). */
    TAUD0.TOE              &= 0xFFFEU;


    /* The simultaneous rewrite registers (TAUDnRDE, TAUDnRDS, TAUDnRDM, and TAUDnRDC)
       cannot be used with the Interval Timer Function. Therefore, these registers must be set to 0. */
    TAUD0.RDE              &= 0xFFFEU;
    TAUD0.RDS              &= 0xFFFEU;
    TAUD0.RDM              &= 0xFFFEU;
    TAUD0.RDC              &= 0xFFFEU;


    SELB_INTC1             &= 0xFFEFU;           /* INTTAUD0I0(ch.0) / INTCSIH2IC(ch.124) */

    MKTAUD0I0              = 0U;                 /* Enable INTTAUD0I0 interrupt           */
}

//Timer Array Unit D (TAUD)
void R_TAUD_InitB( void )
{
    /* Set interrupt flags */
    MKTAUD0I0              = 1U;
    RFTAUD0I0              = 0U;
    TBTAUD0I0              = 0U;


    /* Specifies the CK3 , CK2 , CK1 and CK0 clock
    TAUDnTPS   - TAUDn Prescaler Clock Select Register
    b15:b12      TAUDnPRS3 - Specifies CK3_PRE clock - PCLK / 1
    b11:b 8      TAUDnPRS2 - Specifies CK2_PRE clock - PCLK / 1
    b 7:b 4      TAUDnPRS1 - Specifies CK1_PRE clock - PCLK / 1
    b 3:b 0      TAUDnPRS0 - Specifies CK0_PRE clock - PCLK / 4096 */
    TAUD0.TPS              = 0x000CU;


    /* Data register for the capture/compare value
    TAUDnCDRm  - TAUDn Channel Data Register
    b15:b 0      TAUDnCDR  - 1ms */
    TAUD0.CDR0             = 10U - 1U;//20U - 1U;         /* INTTAUDnIm cycle = count clock cycle * (TAUDnCDRm + 1) */


    /* Controls channel m operation
    TAUDnCMORm - TAUDn Channel Mode OS Register
    b15:b14      TAUDnCKS  - Selects an operation clock.                  - Selected Operation CK0
    b13:b12      TAUDnCCS0 - Selects a count clock for TAUDnCNTm counter. - Uses an operation clock as a count clock.
    b11          TAUDnMAS  - Specifies whether the channel is a master
                             channe or slave channel during synchronous
                             channel operation.                           - Independent operation, Set to 0.
    b10:b 8      TAUDnSTS  - Selects an external start trigger.           - Triggers the counter by software.
    b 7:b 6      TAUDnCOS  - Specifies the timing for updating capture
                             register TAUDnCDRm and overflow flag
                             TAUDnCSRm.TAUDnOVF of channel m.             - Unused. Set to 00'b.
    b 5                    - Reserved set to 0
    b 4:b 1      TAUDnMD   - Specifies an operating mode.                 - Interval timer mode. Set to 0000'b
    b 0          TAUDnMD0  - Specifies whether INTTAUDnIm is generated
                             at the beginning of count operation (when
                             a start trigger is entered) or not.          - INTTAUDnIm is not generated to toggle
                                                                            TAUDTTOUTm at the beginning of an
                                                                            operation. */
    TAUD0.CMOR0            = 0x0000U;


    /* Specifies the type of valid edge detection used for the TAUDTTINm input.
    TAUDnCMURm - TAUDn Channel Mode User Register
    b 7:b 2                - Reserved set to 0
    b 1:b 0      TAUDnTIS  - Specifies a valid edge of TAUDTTINm input
                             signal.                                      - Unused. Set to 00'b. */
    TAUD0.CMUR0            = 0x00U;


    /* The simultaneous rewrite registers (TAUDnTOM, TAUDnTOC, TAUDnTOL, TAUDnTDE , TAUDnTDM , TAUDnTDL
       TAUDnTRE, TAUDnTRO, TAUDnTRC, TAUDnTME) cannot be used with the Interval Timer Function.
       Therefore, these registers must be set to 0. */
    TAUD0.TOM              &= 0xFFFEU;
    TAUD0.TOC              &= 0xFFFEU;
    TAUD0.TOL              &= 0xFFFEU;
    TAUD0.TDE              &= 0xFFFEU;
    TAUD0.TDM              &= 0xFFFEU;
    TAUD0.TDL              &= 0xFFFEU;

    TAUD0.TRE              &= 0xFFFEU;
    TAUD0.TRO              &= 0xFFFEU;
    TAUD0.TRC              &= 0xFFFEU;
    TAUD0.TME              &= 0xFFFEU;


    /* Disable the channel output mode.
    TAUDnTOE   - TAUDn Channel Output Enable Register
    b15:b 0      TAUDnTOEm - Enables/disables the independent channel
                             output function.                             - Disables the independent timer output
                                                                            function (controlled by software). */
    TAUD0.TOE              &= 0xFFFEU;


    /* The simultaneous rewrite registers (TAUDnRDE, TAUDnRDS, TAUDnRDM, and TAUDnRDC)
       cannot be used with the Interval Timer Function. Therefore, these registers must be set to 0. */
    TAUD0.RDE              &= 0xFFFEU;
    TAUD0.RDS              &= 0xFFFEU;
    TAUD0.RDM              &= 0xFFFEU;
    TAUD0.RDC              &= 0xFFFEU;


    SELB_INTC1             &= 0xFFEFU;           /* INTTAUD0I0(ch.0) / INTCSIH2IC(ch.124) */

    MKTAUD0I0              = 0U;                 /* Enable INTTAUD0I0 interrupt           */
}

/******************************************************************************
* Function Name : void R_TAUD_Start( void )
* Description   : This function start the TAUD ch.0.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_TAUD_Start( void )
{
    /* Enables the counter for each channel
    TAUDnTS    - TAUDn Channel Start Trigger Register
    b15:b 0      TAUDnTSm  - Enables the counter operation of channel m.  - Enables the ch.0 counter   */
    TAUD0.TS               = 0x0001U;
}



void IntTimerCallback( uint32_t regEIIC_value )
{
	if(0x00001000UL == regEIIC_value)
	{
		//SysTimeInc();
		Uart3DataSaveCache();
	}
}