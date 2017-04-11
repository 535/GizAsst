#ifndef GAGENT_SOC_H
#define GAGENT_SOC_H

#include "os_type.h"

#ifndef MCU_PROTOCOLVER_LEN
#define MCU_PROTOCOLVER_LEN 8
#define MCU_P0VER_LEN 8
#define MCU_HARDVER_LEN 8
#define MCU_SOFTVER_LEN 8
#define MCU_MCUATTR_LEN 8
#define PK_LEN       32
#define PK_SECRET_LEN    32

#endif
#define SDK_USER_VER_LEN        2

struct devAttrs
{
    unsigned short mBindEnableTime;
    unsigned char mstrProtocolVer[MCU_PROTOCOLVER_LEN];
    unsigned char mstrP0Ver[MCU_P0VER_LEN];
    unsigned char mstrDevHV[MCU_HARDVER_LEN];
    unsigned char mstrDevSV[MCU_SOFTVER_LEN];
    unsigned char mstrProductKey[PK_LEN];
	unsigned char mstrPKSecret[PK_SECRET_LEN];
    unsigned char mDevAttr[MCU_MCUATTR_LEN];
    unsigned char mstrSdkVerLow[SDK_USER_VER_LEN];
};
typedef struct
{
   unsigned short year;
   unsigned char month;
   unsigned char day;
   unsigned char hour;
   unsigned char minute;
   unsigned char second;
   unsigned int ntp;
}_tm;

void gagentProcessRun(os_event_t *events);
int gagentUploadData(unsigned char *src, unsigned int len);
void gagentGetNTP(_tm *time);

/******************************************************
 *      FUNCTION        :   uGAgent_Config
 *      typed           :   1:AP MODE 2:Airlink
 *
 ********************************************************/
void gagentConfig(unsigned char configType);
void gagentReset(void);
void gagentInit(struct devAttrs attrs);
/**********************************************************
* @function GAgentEnableBind
* @brief    允许用户绑定设备
**********************************************************/
void GAgentEnableBind();

#endif /* #endif GAGENT_EXTERNAL_H */

