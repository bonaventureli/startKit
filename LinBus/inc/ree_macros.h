#ifndef _REE_MACROS_H
#define _REE_MACROS_H

#define DEBUG_INTERNAL          0

/* Ports */
#define PORTS_STANDARD        500

/* AD converters */
#define AD_STANDARD          1000
#define AD_CANAIR            1001
#define QLNPBADSCTL0         1002
#define QMNPBADA23CV1        1003    
#define QMNPUADA23EV1        1004
#define QL85EADLV3           1005
#define QMNPUADA82C	         1006
#define QMNPBADA4SST28V1     1007
#define QMNPBADA4SST28V1_1   1008
#define QL85EPAD             1009
#define QMNPUADA82XV1        1010

/* Baud Rate Generators BRG */
#define BRG_STANDARD         2000
#define QLNPUBRG2V1          2000

/* CSI interfaces */
#define CSI_STANDARD         3000
#define QLNPUCSI0V2          3000
#define QLNPUCSI3V1          3001

/* CSI B macros */
#define CSIB_STANDARD        3100
#define QLNPUCSIBV1          3100

/* LCD macros */
#define LCD_STANDARD         4000

/* Timer C macros */
#define TMC_STANDARD         5000
#define QLNPUTMCV2           5000

/* Timer D macros */
#define TMD_STANDARD         6000

/* Timer E macros */
#define TME_STANDARD         7000

/* Timer G macros */
#define TMG_STANDARD         8000
#define QLNPUTMGV1           8000

/* Timer M macros */
#define TMM_STANDARD         9000

/* Timer P macros */
#define TMP_STANDARD        10000
#define QLNPUTMPV1          10000
#define QLNPUTMPV2          10001

/* Timer Y macros */
#define TMY_STANDARD        10100
#define QLNPUTMYV1          10100

/* Timer Z macros */
#define TMZ_STANDARD        10200
#define QLNPUTMZV1          10200

/* Timer 0 macros */
#define TM0_STANDARD        10300
#define QLNPUTM0V2          10300

/* Watchdog Timer macros */
#define WDT_STANDARD        11000
#define QLNPUWDT2V1         11000
#define QLNPUEEWDTV1        11001

/* Watch Timer macros */
#define WTM_STANDARD        11500
#define QLNPUWT0V2          11500
#define QLNPUWT3V1          11501

/* DCAN macros */
#define DCAN_STANDARD       12000
#define QLNPUDCAN1V1        12000

/* FCAN macros */
#define FCAN_STANDARD       13000
#define QLNPBFC1A21V1       13002
#define QLNPUFC4AxV1        13003
#define QLNPUFC4AxV3        13004
#define FCAN_A1             ( QLNPBFC1A21V1 )
#define FCAN_A3             13001
#define FCAN_A4             ( QLNPUFC4AxV3 )

/* aFCAN macros */
#define AFCAN_STANDARD      13100
#define QLNPUAFC0A12V1      13101
#define QLNPUAFC0A10V1      13102
#define QLAPBAFC0C1xV1      13110

/* FVAN macros */
#define FVAN_STANDARD       13500

/* MCAN macros */
#define MCAN_R301           13600

/* MTTCAN macros */
#define MTTCAN_R301         13800

/* RSCAN macros */
#define RSCAN_3CH           14000
#define RSCAN_UCIAPRCN      14010
#define RSCAN_LITE_1CH      14020
#define RSCANFD_UCIAPRCN    14030

/* RSCAN-FD macros */
#define RSCFD_UCIAPRCN      14100

/* UART macros */
#define UART_STANDARD       15000
#define QLNPUART6V1         15001

/* UART A macros */
#define UARTA_STANDARD      15100
#define QLNPUUARTAV1        15100

/* UART C macros */
#define UARTC_STANDARD      15200
#define QLNPUUARTCV1        15200

/* UART D macros */
#define UARTD_STANDARD      15300
#define QLNPSUARTDV1        15300

/* UART E macros */
#define UARTE_STANDARD      15400
#define QLAPBUARTEV1        15400

/* UART F macros */
#define UARTF_STANDARD      15500
#define QLAPBUARTFV1        15500

/* RLIN3 macros */
#define RLIN3_STANDARD      15600
#define RLIN3_UCIAPRLN      15600

/* RLIN2 macros */
#define RLIN2_STANDARD      15700
#define RLIN2_UCIAPRLN      15700

/* DMA macros */
#define DMAC_STANDARD       16000
#define QLNPUDMA            16000
#define QL85EHDMA           16001

/* INTC macros */
#define INTC_STANDARD       17000
#define QL85E70X            17000

/* MEMC macros */
#define MEMC_STANDARD       18000
#define QL85E59X            18000

/* RNG macros */
#define RNG_STANDARD        19000
#define QLNPURNG0V1         19000
            
/* SWC macros */

#define SWC_STANDARD        20000
#define SWC_PROTOTYPE       20000

#endif
