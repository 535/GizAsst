/**
************************************************************
* @file         gizwits_product.h
* @brief        对应gizwits_product.c的头文件(包含产品软硬件版本定义)
* @author       Gizwits
* @date         2016-09-05
* @version      V03000202
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#ifndef _GIZWITS_PRODUCT_H_
#define _GIZWITS_PRODUCT_H_

#include "osapi.h"
#include <stdint.h>

/**
* @name 日志打印宏定义
* @{
*/
#define GIZWITS_LOG os_printf                       ///<运行日志打印
//#define PROTOCOL_DEBUG                              ///<协议数据打印

#ifndef ICACHE_FLASH_ATTR
#define ICACHE_FLASH_ATTR
#endif

/**@} */

#define typedef_t typedef

/**
* MCU硬件版本号
*/
#define HARDWARE_VERSION                        "03000001"
/**
* MCU软件版本号
*/
#define SOFTWARE_VERSION                        "03000003"

/**
* gagent小版本号，用于OTA升级
* OTA硬件版本号: 00ESP826
* OTA软件版本号: 040204xx       // “xx”为SDK_VERSION 
*/
#define SDK_VERSION                             "20"


#define DEV_IS_GATEWAY   0                    ///< 设备是否为中控类,0表示否,1表示是
#define NINABLETIME  0                        ///< 绑定时间

#ifndef SOFTWARE_VERSION
    #error "no define SOFTWARE_VERSION"
#endif

#ifndef HARDWARE_VERSION
    #error "no define HARDWARE_VERSION"
#endif

#define SIG_UPGRADE_DATA 0x01

/**@name Gizwits 用户API接口
* @{
*/
//int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len);
/**@} */

#endif
