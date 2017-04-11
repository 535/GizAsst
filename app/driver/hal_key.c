/**
************************************************************
* @file         hal_key.c
* @brief        按键驱动
* 
* 按键模块采用定时器+GPIO状态读取机制，GPIO的配置要根据ESP8266的相关手册来配置
* 
* 本驱动支持 0 ~ 12 个GPIO按键扩展，支持跨平台移植。
* @author       Gizwits
* @date         2016-09-05
* @version      V03010201
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include "driver/hal_key.h"
#include "mem.h"

uint32 keyCountTime = 0; 
static uint8_t keyTotolNum = 0;

/**
* @brief Read the GPIO state
* @param [in] keys 按键功能全局结构体指针
* @return uint16_t型的GPIO状态值
*/
static ICACHE_FLASH_ATTR uint16_t keyValueRead(keys_typedef_t * keys)
{
    uint8_t i = 0; 
    uint16_t read_key = 0;

    //GPIO Cyclic scan
    for(i = 0; i < keys->keyTotolNum; i++)
    {
        if(!GPIO_INPUT_GET(keys->singleKey[i]->gpio_id))
        {
            G_SET_BIT(read_key, keys->singleKey[i]->gpio_number);
        }
    }
    
    return read_key;
}

/**
* @brief Read the KEY value
* @param [in] keys 按键功能全局结构体指针
* @return uint16_t型的按键状态值
*/
static uint16_t ICACHE_FLASH_ATTR keyStateRead(keys_typedef_t * keys)
{
    static uint8_t Key_Check = 0;
    static uint8_t Key_State = 0;
    static uint16_t Key_LongCheck = 0;
    uint16_t Key_press = 0; 
    uint16_t Key_return = 0;
    static uint16_t Key_Prev = 0;
    
    //累加按键时间
    keyCountTime++;
        
    //按键消抖30MS
    if(keyCountTime >= (DEBOUNCE_TIME / keys->key_timer_ms)) 
    {
        keyCountTime = 0; 
        Key_Check = 1;
    }
    
    if(Key_Check == 1)
    {
        Key_Check = 0;
        
        //获取当前按键触发值
        Key_press = keyValueRead(keys); 
        
        switch (Key_State)
        {
            //"首次捕捉按键"状态
            case 0:
                if(Key_press != 0)
                {
                    Key_Prev = Key_press;
                    Key_State = 1;
                }
    
                break;
                
                //"捕捉到有效按键"状态
            case 1:
                if(Key_press == Key_Prev)
                {
                    Key_State = 2;
                    Key_return= Key_Prev | KEY_DOWN;
                }
                else
                {
                    //按键抬起,是抖动,不响应按键
                    Key_State = 0;
                }
                break;
                
                //"捕捉长按键"状态
            case 2:
    
                if(Key_press != Key_Prev)
                {
                    Key_State = 0;
                    Key_LongCheck = 0;
                    Key_return = Key_Prev | KEY_UP;
                    return Key_return;
                }
    
                if(Key_press == Key_Prev)
                {
                    Key_LongCheck++;
                    if(Key_LongCheck >= (PRESS_LONG_TIME / DEBOUNCE_TIME))    //长按3S (消抖30MS * 100)
                    {
                        Key_LongCheck = 0;
                        Key_State = 3;
                        Key_return= Key_press |  KEY_LONG;
                        return Key_return;
                    }
                }
                break;
                
                //"还原初始"状态    
            case 3:
                if(Key_press != Key_Prev)
                {
                    Key_State = 0;
                }
                break;
        }
    }

    return  NO_KEY;
}

/**
* @brief 按键回调函数

* 在该函数内完成按键状态监测后调用对应的回调函数
* @param [in] keys 按键功能全局结构体指针
* @return none
*/
void ICACHE_FLASH_ATTR gokitKeyHandle(keys_typedef_t * keys)
{
    uint8_t i = 0;
    uint16_t key_value = 0;

    key_value = keyStateRead(keys); 
    
    if(!key_value) return;
    
    //Check short press button
    if(key_value & KEY_UP)
    {
        //Valid key is detected
        for(i = 0; i < keys->keyTotolNum; i++)
        {
            if(G_IS_BIT_SET(key_value, keys->singleKey[i]->gpio_number)) 
            {
                //key callback function of short press
                if(keys->singleKey[i]->short_press) 
                {
                    keys->singleKey[i]->short_press(); 
                    
                    os_printf("[zs] callback key: [%d][%d] \r\n", keys->singleKey[i]->gpio_id, keys->singleKey[i]->gpio_number); 
                }
            }
        }
    }

    //Check short long button
    if(key_value & KEY_LONG)
    {
        //Valid key is detected
        for(i = 0; i < keys->keyTotolNum; i++)
        {
            if(G_IS_BIT_SET(key_value, keys->singleKey[i]->gpio_number))
            {
                //key callback function of long press
                if(keys->singleKey[i]->long_press) 
                {
                    keys->singleKey[i]->long_press(); 
                    
                    os_printf("[zs] callback long key: [%d][%d] \r\n", keys->singleKey[i]->gpio_id, keys->singleKey[i]->gpio_number); 
                }
            }
        }
    }
}

/**
* @brief 单按键初始化

* 在该函数内完成单个按键的初始化，这里需要结合ESP8266 GPIO寄存器说明文档来设置参数
* @param [in] gpio_id ESP8266 GPIO 编号
* @param [in] gpio_name ESP8266 GPIO 名称
* @param [in] gpio_func ESP8266 GPIO 功能
* @param [in] long_press 长按状态的回调函数地址
* @param [in] short_press 短按状态的回调函数地址
* @return 单按键结构体指针
*/
key_typedef_t * ICACHE_FLASH_ATTR keyInitOne(uint8 gpio_id, uint32 gpio_name, uint8 gpio_func, gokit_key_function long_press, gokit_key_function short_press)
{
    static int8_t key_total = -1;

    key_typedef_t * singleKey = (key_typedef_t *)os_zalloc(sizeof(key_typedef_t));

    singleKey->gpio_number = ++key_total;
    
    //Platform-defined GPIO
    singleKey->gpio_id = gpio_id;
    singleKey->gpio_name = gpio_name;
    singleKey->gpio_func = gpio_func;
    
    //Button trigger callback type
    singleKey->long_press = long_press;
    singleKey->short_press = short_press;
    
    keyTotolNum++;    

    return singleKey;
}

/**
* @brief 按键驱动初始化

* 在该函数内完成所有的按键GPIO初始化，并开启一个定时器开始按键状态监测
* @param [in] keys 按键功能全局结构体指针
* @return none
*/
void ICACHE_FLASH_ATTR keyParaInit(keys_typedef_t * keys)
{
    uint8 tem_i = 0; 
    
    if(NULL == keys)
    {
        return ;
    }
    
    //init key timer 
    keys->key_timer_ms = KEY_TIMER_MS; 
    os_timer_disarm(&keys->key_timer); 
    os_timer_setfn(&keys->key_timer, (os_timer_func_t *)gokitKeyHandle, keys); 
    
    keys->keyTotolNum = keyTotolNum;

    //Limit on the number keys (Allowable number: 0~12)
    if(KEY_MAX_NUMBER < keys->keyTotolNum) 
    {
        keys->keyTotolNum = KEY_MAX_NUMBER; 
    }
    
    //GPIO configured as a high level input mode
    for(tem_i = 0; tem_i < keys->keyTotolNum; tem_i++) 
    {
        PIN_FUNC_SELECT(keys->singleKey[tem_i]->gpio_name, keys->singleKey[tem_i]->gpio_func); 
        GPIO_OUTPUT_SET(GPIO_ID_PIN(keys->singleKey[tem_i]->gpio_id), 1); 
        PIN_PULLUP_EN(keys->singleKey[tem_i]->gpio_name); 
        GPIO_DIS_OUTPUT(GPIO_ID_PIN(keys->singleKey[tem_i]->gpio_id)); 
        
        os_printf("gpio_name %d \r\n", keys->singleKey[tem_i]->gpio_id); 
    }
    
    //key timer start
    os_timer_arm(&keys->key_timer, keys->key_timer_ms, 1); 
}

/**
* @brief 按键驱动测试

* 该函数模拟了外部对按键模块的初始化调用

* 注：用户需要定义相应的按键回调函数(如 key1LongPress ...)
* @param none
* @return none
*/
void ICACHE_FLASH_ATTR keyTest(void)
{
#ifdef KEY_TEST
    //按键GPIO参数宏定义
    #define GPIO_KEY_NUM                            2                           ///< 定义按键成员总数
    #define KEY_0_IO_MUX                            PERIPHS_IO_MUX_GPIO0_U      ///< ESP8266 GPIO 功能
    #define KEY_0_IO_NUM                            0                           ///< ESP8266 GPIO 编号
    #define KEY_0_IO_FUNC                           FUNC_GPIO0                  ///< ESP8266 GPIO 名称
    #define KEY_1_IO_MUX                            PERIPHS_IO_MUX_MTMS_U       ///< ESP8266 GPIO 功能
    #define KEY_1_IO_NUM                            14                          ///< ESP8266 GPIO 编号
    #define KEY_1_IO_FUNC                           FUNC_GPIO14                 ///< ESP8266 GPIO 名称
    LOCAL key_typedef_t * singleKey[GPIO_KEY_NUM];                              ///< 定义单个按键成员数组指针
    LOCAL keys_typedef_t keys;                                                  ///< 定义总的按键模块结构体指针    
    
    //每初始化一个按键调用一次keyInitOne ,singleKey次序加一
    singleKey[0] = keyInitOne(KEY_0_IO_NUM, KEY_0_IO_MUX, KEY_0_IO_FUNC,
                                key1LongPress, key1ShortPress);
                                
    singleKey[1] = keyInitOne(KEY_1_IO_NUM, KEY_1_IO_MUX, KEY_1_IO_FUNC,
                                key2LongPress, key2ShortPress);
                                
    keys.key_timer_ms = KEY_TIMER_MS; //设置按键定时器周期 建议10ms
    keys.singleKey = singleKey; //完成按键成员赋值
    keyParaInit(&keys); //按键驱动初始化
#endif
}