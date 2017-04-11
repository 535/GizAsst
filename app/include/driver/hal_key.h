/**
************************************************************
* @file         hal_uart.h
* @brief        hal_uart.c对应头文件
* @author       Gizwits
* @date         2016-09-05
* @version      V03010201
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#ifndef _HAL_KEY_H_
#define _HAL_KEY_H_

#include <stdio.h>
#include <c_types.h>
#include <gpio.h>
#include "os_type.h"
#include "osapi.h"

#define G_SET_BIT(a,b)                          (a |= (1 << b))
#define G_CLEAR_BIT(a,b)                        (a &= ~(1 << b))
#define G_IS_BIT_SET(a,b)                       (a & (1 << b))

#define KEY_TIMER_MS                            10                          ///< 定义按键模块所需定时器的定时周期
#define KEY_MAX_NUMBER                          12                          ///< 按键最大支持个数
#define DEBOUNCE_TIME                           30
#define PRESS_LONG_TIME                         3000

#define NO_KEY                                  0x0000
#define KEY_DOWN                                0x1000
#define KEY_UP                                  0x2000
#define KEY_LIAN                                0x4000
#define KEY_LONG                                0x8000

typedef void (*gokit_key_function)(void);

typedef struct
{
    uint8 gpio_number; 
    uint8 gpio_id;
    uint8 gpio_func;
    uint32 gpio_name;
    gokit_key_function short_press; 
    gokit_key_function long_press; 
}key_typedef_t; 

typedef struct
{
    uint8 keyTotolNum;
    os_timer_t key_timer;
    uint8 key_timer_ms; 
    key_typedef_t ** singleKey; 
}keys_typedef_t; 

/* Function declaration */

void gokitKeyHandle(keys_typedef_t * keys); 
key_typedef_t * keyInitOne(uint8 gpio_id, uint32 gpio_name, uint8 gpio_func, gokit_key_function long_press, gokit_key_function short_press); 
void keyParaInit(keys_typedef_t * keys);
void keySensorTest(void);

#endif /*_HAL_KEY_H*/

