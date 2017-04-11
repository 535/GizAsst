
/*********************************************************
*
* @file      hal_led.c
* @author    Gizwtis
* @version   V3.0
* @date      2016-03-09
*
* @brief     机智云 只为智能硬件而生
*            Gizwits Smart Cloud  for Smart Products
*            链接|增值|开放|中立|安全|自有|自由|生态
*            www.gizwits.com
*
*********************************************************/

#include "driver/hal_led_switch.h"
#include "osapi.h"

void ICACHE_FLASH_ATTR rgbGpioInit(void)
{
    /* Migrate your driver code */
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12); 
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14); 
    
    //继电器开漏 open drain;
    GPIO_REG_WRITE(GPIO_PIN_ADDR(GPIO_ID_PIN(GPIO_SWITCH)), GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(GPIO_SWITCH))) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)); 
    
    gpio_output_set(0, 0, GPIO_ID_PIN(GPIO_LED) , 0); 
 	gpio_output_set(0, 0, GPIO_ID_PIN(GPIO_SWITCH) , 0); 
 	
    os_printf("GpioInit \r\n");
}

