#ifndef _RLIN3_UCIAPRLN
#define _RLIN3_UCIAPRLN
                   
#include <ree_types.h>

/* RSCAN Globals */

#define RLIN3_0       ( 0 )         /* RLIN3 macro selection */
#define RLIN3_1       ( 1 )
#define RLIN3_2       ( 2 )
#define RLIN3_3       ( 3 )
#define RLIN3_4       ( 4 )
#define RLIN3_5       ( 5 )

#define RLIN3_NP      ( 0x0F )
#define RLIN3_TP      ( 0xF0 )     /* ports for non-wait sending */
#define RLIN3_TP_0    ( 16 )
#define RLIN3_TP_1    ( 17 )
#define RLIN3_TP_2    ( 18 )
#define RLIN3_TP_3    ( 19 )
#define RLIN3_TP_4    ( 20 )
#define RLIN3_TP_5    ( 21 )

#define RLIN3_MAXMACROS ( 6 )

#define RLIN3_INVALID ( 0xFF )    /* no macro assignment */

#define RLIN3_TRANSFER_OK true    /* Status report defines */
#define RLIN3_OK          true
#define RLIN3_ERROR       false

#define RLIN3_SET         ( 1 )
#define RLIN3_CLEAR       ( 0 )

#define RLIN3_IRQ_SEND    ( 0 )   /* interrupt sources */
#define RLIN3_IRQ_RECEIVE ( 1 )
#define RLIN3_IRQ_ERROR   ( 2 )
#define RLIN3_IRQ_COMMON  ( 4 )

#define RLIN3_INTERRUPTS  ( 3 )

/* application level constants */

#define RLIN3_A_TTYMODE_CR        ( 0 )
#define RLIN3_A_TTYMODE_CRLF      ( 1 )
#define RLIN3_A_HEXMODE           ( 1 )
#define RLIN3_A_DECMODE           ( 0 )
#define RLIN3_A_RECBUFLEN         ( 80 )

#define RLIN3_A_SKIPMESSAGE       ( 0 )
#define RLIN3_A_GETMESSAGE        ( 1 )
#define RLIN3_A_GETHEADER         ( 2 )

/* Setting defines */

#define RLIN3_MAX_MODULUS_MASTER  ( 0x100 )
#define RLIN3_MAX_MODULUS_OTHER   ( 0x10000 )
#define RLIN3_MIN_MODULUS         ( 1 )
#define RLIN3_MAX_PRESCALER       ( 7 )

#define RLIN3_SAMPLES_MASTER      ( 16 )
#define RLIN3_SAMPLES_SLAVE_FIX   ( 16 )
#define RLIN3_SAMPLES_SLAVE_AUTO  ( 8 )
#define RLIN3_SAMPLES_MIN         ( 4 )
#define RLIN3_SAMPLES_MAX         ( 16 )
#define RLIN3_SAMPLES_FORBIDDEN   ( 5 )

#define RLIN3_LINSTANDARD_V1      ( 0 )
#define RLIN3_LINSTANDARD_V2      ( 1 )

#define RLIN3_CLOCK_FA            ( 0 )
#define RLIN3_CLOCK_FB            ( 1 )
#define RLIN3_CLOCK_FC            ( 2 )
#define RLIN3_CLOCK_FD            ( 3 )

#define RLIN3_CLOCKDIVIDER_FA     ( 1 )
#define RLIN3_CLOCKDIVIDER_FB     ( 2 )
#define RLIN3_CLOCKDIVIDER_FC     ( 8 )
#define RLIN3_CLOCKDIVIDER_FD     ( 2 )

#define RLIN3_MODE_UART           ( 1 )
#define RLIN3_MODE_LINMASTER      ( 0 )
#define RLIN3_MODE_LINSLAVE_FIX   ( 3 )
#define RLIN3_MODE_LINSLAVE_AUTO  ( 2 )

#define RLIN3_MMODE_RESET         ( 0x00 )
#define RLIN3_MMODE_RESET_MASK    ( 0x01 )
#define RLIN3_MMODE_WAKEUP        ( 0x01 )
#define RLIN3_MMODE_OPERATION     ( 0x03 )

#define RLIN3_SINGLE_INT          ( 0 )
#define RLIN3_TRIPLE_INT          ( 1 )

#define RLIN3_SAMPLE_SINGLE       ( 1 )
#define RLIN3_SAMPLE_TRIPLE       ( 0 )

#define RLIN3_SBF13BITS           ( 0 )
#define RLIN3_SBF14BITS           ( 1 )
#define RLIN3_SBF15BITS           ( 2 )
#define RLIN3_SBF16BITS           ( 3 )
#define RLIN3_SBF17BITS           ( 4 )
#define RLIN3_SBF18BITS           ( 5 )
#define RLIN3_SBF19BITS           ( 6 )
#define RLIN3_SBF20BITS           ( 7 )
#define RLIN3_SBF21BITS           ( 8 )
#define RLIN3_SBF22BITS           ( 9 )
#define RLIN3_SBF23BITS           ( 10 )
#define RLIN3_SBF24BITS           ( 11 )
#define RLIN3_SBF25BITS           ( 12 )
#define RLIN3_SBF26BITS           ( 13 )
#define RLIN3_SBF27BITS           ( 14 )
#define RLIN3_SBF28BITS           ( 15 )

#define RLIN3_BDT1BITS            ( 0 )
#define RLIN3_BDT2BITS            ( 1 )
#define RLIN3_BDT3BITS            ( 2 )
#define RLIN3_BDT4BITS            ( 3 )

#define RLIN3_SBFSLAVE_SHORT      ( 0 )
#define RLIN3_SBFSLAVE_LONG       ( 1 )

#define RLIN3_CHARLEN_7BITS       ( 1 )
#define RLIN3_CHARLEN_8BITS       ( 0 )

#define RLIN3_DIR_MSBFIRST        ( 1 )
#define RLIN3_DIR_LSBFIRST        ( 0 )

#define RLIN3_ONESTOPBIT          ( 0 )
#define RLIN3_TWOSTOPBITS         ( 1 )

#define RLIN3_PARITY_NONE         ( 0 )
#define RLIN3_PARITY_ZERO         ( 2 )
#define RLIN3_PARITY_ODD          ( 3 )
#define RLIN3_PARITY_EVEN         ( 1 )

#define RLIN3_TXDNORMAL           ( 0 )
#define RLIN3_TXDINVERT           ( 1 )
#define RLIN3_RXDNORMAL           ( 0 )
#define RLIN3_RXDINVERT           ( 1 )

#define RLIN3_SPACE_HDRRESP_0BITS ( 0 )
#define RLIN3_SPACE_HDRRESP_1BITS ( 1 )
#define RLIN3_SPACE_HDRRESP_2BITS ( 2 )
#define RLIN3_SPACE_HDRRESP_3BITS ( 3 )
#define RLIN3_SPACE_HDRRESP_4BITS ( 4 )
#define RLIN3_SPACE_HDRRESP_5BITS ( 5 )
#define RLIN3_SPACE_HDRRESP_6BITS ( 6 )
#define RLIN3_SPACE_HDRRESP_7BITS ( 7 )

#define RLIN3_SPACE_RESBYTE_0BITS ( 0 )
#define RLIN3_SPACE_RESBYTE_1BITS ( 1 )
#define RLIN3_SPACE_RESBYTE_2BITS ( 2 )
#define RLIN3_SPACE_RESBYTE_3BITS ( 3 )

#define RLIN3_WUPTL_1BITS         ( 0 )
#define RLIN3_WUPTL_2BITS         ( 1 )
#define RLIN3_WUPTL_3BITS         ( 2 )
#define RLIN3_WUPTL_4BITS         ( 3 )
#define RLIN3_WUPTL_5BITS         ( 4 )
#define RLIN3_WUPTL_6BITS         ( 5 )
#define RLIN3_WUPTL_7BITS         ( 6 )
#define RLIN3_WUPTL_8BITS         ( 7 )
#define RLIN3_WUPTL_9BITS         ( 8 )
#define RLIN3_WUPTL_10BITS        ( 9 )
#define RLIN3_WUPTL_11BITS        ( 10 )
#define RLIN3_WUPTL_12BITS        ( 11 )
#define RLIN3_WUPTL_13BITS        ( 12 )
#define RLIN3_WUPTL_14BITS        ( 13 )
#define RLIN3_WUPTL_15BITS        ( 14 )
#define RLIN3_WUPTL_16BITS        ( 15 )

#define RLIN3_LINDATALEN_MAX      ( 8 )
#define RLIN3_UARTDATALEN_MAX     ( 9 )

#define RLIN3_RESPONSE_RECEIVE    ( 0 )
#define RLIN3_RESPONSE_TRANSMIT   ( 1 )

#define RLIN3_CHECKSUM_CLASSIC    ( 0 )
#define RLIN3_CHECKSUM_ENHANCED   ( 1 )

#define RLIN3_MFRAME_COMBINED     ( 0 )
#define RLIN3_MFRAME_SEPARATE     ( 1 )

#define RLIN3_PIDMASK             ( 0x3F )
#define RLIN3_PID_P0              ( 6 )
#define RLIN3_PID_P1              ( 7 )

/* RLIN3 access structures */

typedef struct rlin3_lwbr
{
  volatile u08                    lwbr0     : 1;
  volatile u08                    lprs      : 3;
  volatile u08                    nspb      : 4;
  
} rlin3_lwbr;

typedef struct rlin3_lmd
{
  volatile u08                    lmd       : 2;
  volatile u08                    lcks      : 2;
  volatile u08                    lios      : 1;
  volatile u08                    lrdnfs    : 1;
  volatile u08 RESERVED0                    : 2;
    
} rlin3_lmd;

typedef struct rlin3_lsc
{
  volatile u08                    ibhs      : 3;
  volatile u08 RESERVED0                    : 1;
  volatile u08                    ibs       : 2;
  volatile u08 RESERVED1                    : 2;
    
} rlin3_lsc;

typedef struct rlin3_lwup
{
  volatile u08 RESERVED0                    : 4;
  volatile u08                    wutl      : 4;
  
} rlin3_lwup;

typedef struct rlin3_lie
{
  volatile u08                    ftcie     : 1;
  volatile u08                    frcie     : 1;
  volatile u08                    errie     : 1;
  volatile u08                    shie      : 1;
  volatile u08 RESERVED0                    : 4;
  
} rlin3_lie;

typedef struct rlin3_lcuc
{
  volatile u08                    om        : 2;
  volatile u08 RESERVED0                    : 6;
  
} rlin3_lcuc;

typedef struct rlin3_ltrc
{
  volatile u08                    fts       : 1;
  volatile u08                    rts       : 1;
  volatile u08                    lnrr      : 1;
  volatile u08 RESERVED0                    : 5;
  
} rlin3_ltrc;

typedef struct rlin3_lst_bit
{
  volatile u08                    ftc       : 1;
  volatile u08                    frc       : 1;
  volatile u08 RESERVED0                    : 1;
  volatile u08                    err       : 1;
  volatile u08                    uts       : 1;
  volatile u08                    urs       : 1;
  volatile u08                    d1rc      : 1;
  volatile u08                    htrc      : 1;
  
} rlin3_lst_bit;

typedef union rlin3_lst
{
  volatile u08                    all;
  struct rlin3_lst_bit            b;
  
} rlin3_lst;

typedef struct rlin3_luoer
{
  volatile u08                    utoe      : 1;
  volatile u08                    uroe      : 1;
  volatile u08 RESERVED0                    : 6;
  
} rlin3_luoer;

typedef struct rlin3_luor1
{
  volatile u08                    uebe      : 1;
  volatile u08                    uebdl     : 1;
  volatile u08                    uebdce    : 1;
  volatile u08                    utigts    : 1;
  volatile u08                    uecd      : 1;
  volatile u08 RESERVED0                    : 3;
      
} rlin3_luor1;

typedef struct rlin3_lbfc_master
{
  volatile u08                    blt       : 4;
  volatile u08                    bdt       : 2;
  volatile u08 RESERVED0                    : 2;
  
} rlin3_lbfc_master;

typedef struct rlin3_lbfc_slave
{
  volatile u08                    lblt      : 1;
  volatile u08 RESERVED0                    : 7;
  
} rlin3_lbfc_slave;

typedef struct rlin3_lbfc_uart
{
  volatile u08                    ubls      : 1;
  volatile u08                    ubos      : 1;
  volatile u08                    usbls     : 1;
  volatile u08                    ups       : 2;
  volatile u08                    urps      : 1;
  volatile u08                    utps      : 1;
  volatile u08 RESERVED0                    : 1;  
    
} rlin3_lbfc_uart;

typedef struct rlin3_lede_lin
{
  volatile u08                    bere      : 1;
  volatile u08                    pbere     : 1;
  volatile u08                    ftere     : 1;
  volatile u08                    fere      : 1;
  volatile u08                    sfere     : 1;
  volatile u08 RESERVED0                    : 1;
  volatile u08                    ipere     : 1;
  volatile u08                    ltes      : 1;
  
} rlin3_lede_lin;

typedef struct rlin3_lede_uart
{
  volatile u08                    bere      : 1;
  volatile u08 RESERVED0                    : 1;
  volatile u08                    oere      : 1;
  volatile u08                    fere      : 1;
  volatile u08 RESERVED1                    : 4;
  
} rlin3_lede_uart;

typedef struct rlin3_lest_lin
{
  volatile u08                    ber       : 1;
  volatile u08                    pber      : 1;
  volatile u08                    ter       : 1;
  volatile u08                    fer       : 1;
  volatile u08                    sfer      : 1;
  volatile u08                    cser      : 1;
  volatile u08                    iper      : 1;
  volatile u08                    rper      : 1;
  
} rlin3_lest_lin;

typedef struct rlin3_lest_uart
{
  volatile u08                    ber       : 1;
  volatile u08 RESERVED0                    : 1;
  volatile u08                    oer       : 1;
  volatile u08                    fer       : 1;
  volatile u08                    exbt      : 1;
  volatile u08                    idmt      : 1;
  volatile u08                    uper      : 1;
  volatile u08 RESERVED1                    : 1;
  
} rlin3_lest_uart;

typedef struct rlin3_ldfc_lin
{
  volatile u08                    rfdl      : 4;
  volatile u08                    rcds      : 1;
  volatile u08                    lcs       : 1;
  volatile u08                    fsm       : 1;
  volatile u08                    lss       : 1;
  
} rlin3_ldfc_lin;

typedef struct rlin3_ldfc_uart
{
  volatile u08                    mdl       : 4;
  volatile u08 RESERVED0                    : 1;
  volatile u08                    utsw      : 1;
  volatile u08 RESERVED1                    : 2;
  
} rlin3_ldfc_uart;

typedef struct rlin3_lidb_lin
{
  volatile u08                    id        : 6;
  volatile u08                    idp0      : 1;
  volatile u08                    idp1      : 1;
  
} rlin3_lidb_lin;

typedef struct rlin3_lidb_uart
{
  volatile u08                    id        : 8;
  
} rlin3_lidb_uart;

typedef struct rlin3_ldb_buf8
{
  volatile u08 RESERVED0;
  volatile u08                    buf[ RLIN3_LINDATALEN_MAX ];
  
} rlin3_ldb_buf8;

typedef struct rlin3_ldb_buf9
{
  volatile u08                    buf[ RLIN3_UARTDATALEN_MAX ];

} rlin3_ldb_buf9;

typedef union rlin3_lbfc
{
  struct rlin3_lbfc_master        master;
  struct rlin3_lbfc_slave         slave;
  struct rlin3_lbfc_uart          uart;
  
} rlin3_lbfc;

typedef union rlin3_lede
{
  struct rlin3_lede_lin           lin;
  struct rlin3_lede_uart          uart;
  
} rlin3_lede;

typedef union rlin3_lest
{
  struct rlin3_lest_lin           lin;
  struct rlin3_lest_uart          uart;
  volatile u08                    all;
  
} rlin3_lest;

typedef union rlin3_ldfc
{
  struct rlin3_ldfc_lin           lin;
  struct rlin3_ldfc_uart          uart;
  
} rlin3_ldfc;

typedef union rlin3_lidb
{
  struct rlin3_lidb_lin           lin;
  struct rlin3_lidb_uart          uart;

} rlin3_lidb;

typedef union rlin3_ldb
{
  struct rlin3_ldb_buf8           buf8;
  struct rlin3_ldb_buf9           buf9;
  
} rlin3_ldb;

typedef struct rlin3
{
  volatile u08                    RESERVED0[ 1 ];

  struct rlin3_lwbr               lwbr;
  volatile u08                    brp0;
  volatile u08                    brp1;
  volatile u08                    lstc;

  volatile u08                    RESERVED1[ 3 ];

  struct rlin3_lmd                lmd;
  union  rlin3_lbfc               lbfc;
  struct rlin3_lsc                lsc;
  struct rlin3_lwup               lwup;
  struct rlin3_lie                lie;
  union  rlin3_lede               lede;
  struct rlin3_lcuc               lcuc;

  volatile u08                    RESERVED2[ 1 ];

  struct rlin3_ltrc               ltrc;
  struct rlin3_lcuc               lmst;
  union  rlin3_lst                lst;
  union  rlin3_lest               lest;
  union  rlin3_ldfc               ldfc;
  union  rlin3_lidb               lidb;
  volatile u08                    lcbr;
  union  rlin3_ldb                ldb;
  struct rlin3_luoer              luoer;
  struct rlin3_luor1              luor1;
  
  volatile u08                    RESERVED3[ 2 ];
  
  volatile u16                    lutdr;
  volatile u16                    lurdr;
  volatile u16                    luwtdr;
  
} rlin3;

#endif
