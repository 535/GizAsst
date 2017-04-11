#ifndef _HAL_LED_SWITCH_H
#define _HAL_LED_SWITCH_H

#include <stdio.h>
#include <c_types.h>
#include <gpio.h>
#include <eagle_soc.h>

/* Define your drive pin,when the io is low,the led will be lighted! */
#define GPIO_LED           12
#define GPIO_SWITCH        14
/* Set GPIO Direction */
#define LedOn()                GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_LED), 0)
#define LedOff()               GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_LED), 1)
#define SwitchOn()             GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_SWITCH), 0)
#define SwitchOff()            GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_SWITCH), 1)

/* Function declaration */
void rgbGpioInit(void); 

#endif /*_HAL_LED_SWITCH_H*/

