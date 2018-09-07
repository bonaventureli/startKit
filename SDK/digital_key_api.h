#ifndef DIGITAL_KEY_API_H
#define DIGITAL_KEY_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/




/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/
#define SPARAM_MAX_LEN   48

#define BEGYEAR  1970     //  UTC started at 00:00:00 January 1, 2000
#define DAY      86400UL  // 24 hours * 60 minutes * 60 seconds
#define YearLength(yr)  (IsLeapYear(yr) ? 366 : 365)
#define IsLeapYear(yr)  (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef struct{
    int command;
    int result;
    int index;
    int permission;
    int sparam_size;
    unsigned char sparam[SPARAM_MAX_LEN];
} DK_Cmd_Meg;


typedef struct
{
  unsigned char seconds;  // 0-59
  unsigned char minutes;  // 0-59
  unsigned char hour;     // 0-23
  unsigned char day;      // 0-30
  unsigned char month;    // 0-11
  unsigned short year;    // 2000+
} UTCTimeStruct;


/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
typedef int (*read_CallBack)(unsigned char *out, unsigned int rlen, unsigned int offset);
typedef int (*write_CallBack)(unsigned char *in, unsigned int wlen, unsigned int offset);
typedef int (*Rand_CallBack)(void *p_rng, unsigned char *rand, unsigned int randlen);
typedef int (*Print_Str)(const char *fmt, int len);

extern int ingeek_get_sec_status(void);
extern int ingeek_se_final(void);
extern int ingeek_se_init(void);
extern int ingeek_pull_info(unsigned char *output,unsigned int* olen);
extern int ingeek_push_info(unsigned char *input, int ilen);
extern int ingeek_push_auth(unsigned char *input, unsigned int ilen, unsigned char* output, unsigned int* olen);
extern int ingeek_push_session(unsigned char *input,unsigned int ilen,unsigned char* output, unsigned int* olen);
extern int ingeek_command_output_action(DK_Cmd_Meg* Can_Packet, unsigned char* output, unsigned int* olen);
extern int ingeek_command_input_action(unsigned char *input,unsigned int ilen, DK_Cmd_Meg* Can_Packet);
extern int ingeek_message_output_action(DK_Cmd_Meg* Can_Packet, unsigned char* output, unsigned int* outlen);
//extern int ingeek_set_RTC(unsigned char *input,unsigned int size, UTCTimeStruct* utc_time);
//extern int ingeek_push_whitelist(unsigned char *input, int ilen);
extern unsigned char* ingeek_get_version(void);
extern unsigned int   ingeek_get_pair_df(void);
extern unsigned int   ingeek_calculate_vinhash(unsigned char* output, unsigned int* outlen);
extern unsigned int   ingeek_get_permission(void);
extern void ingeek_set_seid(unsigned char *seid);
extern void ingeek_set_callback(read_CallBack,write_CallBack,Rand_CallBack randcb,Print_Str g_printcb);

/************************************************************************************
*************************************************************************************
* Public SDK Code
*************************************************************************************
************************************************************************************/
typedef enum {
    INGEEK_OK=0,
    INGEEK_FAILED_PBENCODE,	/*1*/
    INGEEK_FAILED_PBDECODE,	/*2*/
    INGEEK_FAILED_DECRYPT,	/*3*/
    INGEEK_FAILED_ENCRYPT,	/*4*/
    INGEEK_FAILED_WRITECB,	/*5*/
    INGEEK_FAILED_READCB,	/*6*/
    INGEEK_FAILED_RANDCB,	/*7*/
    INGEEK_FAILED_WHITELIST,	/*8*/
    INGEEK_FAILED_MDINFO,	/*9*/
    INGEEK_FAILED_MD,		/*10*/
    INGEEK_FAILED_HMAC,		/*11*/
    INGEEK_FAILED_CMAC,		/*12*/
    INGEEK_FAILED_SEID,		/*13*/
    INGEEK_FAILED_TRND,
    INGEEK_FAILED_RTC,
    INGEEK_FAILED_STATUS,
    INGEEK_FAILED_NULL,
    INGEEK_FAILED_PARAM,
    
    INGEEK_FAILED
}SDKCode;


#ifdef __cplusplus
}
#endif


#endif/* DIGITAL_KEY_API_H */
