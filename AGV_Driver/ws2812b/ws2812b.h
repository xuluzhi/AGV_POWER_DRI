/*
 * ws2812b.h
 *
 *  Created on: 10 lut 2016
 *      Author: Tomasz
 */

#ifndef WS2812B_H_
#define WS2812B_H_

#include "stm32f4xx.h"
#include "Data.h"

#define LED_BUFFER_SIZE 		(LED_COUNT_IN_SEGMENT*24*3)
//GRB颜色值
#define RGB_BLUE       (uint32_t)(0x0000FF)//GRB
#define RGB_RED        (uint32_t)(0x00FF00)
#define RGB_GREEN      (uint32_t)(0xFF0000)
#define RGB_YELLOW     (uint32_t)(0xF5F500)
#define RGB_OFF        (uint32_t)(0x000000)
#define RGB_ON         (uint32_t)(0xFFFFFF)
//不同灯珠的RGB数量
//#define RGB_LEFT_NUM          21
//#define RGB_RIGHT_NUM         21
//#define RGB_CIRCLE_NUM        21
//#define RGB_RGB_ATMP_NUM      48


#define RGB_LEFT_NUMBER     12//左转灯珠数量
#define RGB_RIGHT_NUMBER    12//右转灯珠数量
#define RGB_CIRCLE_NUMBER   22//圆灯灯珠数量
#define RGB_ATMP_NUMBER     47//气氛灯灯珠数量

//灯珠类型
typedef enum _RGB_Typed
{
    RGB_LEFT_TYPE=1,
    RGB_RIGHT_TYPE, 
	RGB_CIRCLE_TYPE,
	RGB_ATMP_TYPE,
}RGB_Typed;

#define  FS1_POWER			GPIOE->BSRRL = GPIO_Pin_5//
#define  _FS1_POWER			GPIOE->BSRRH = GPIO_Pin_5//
#define  FS2_POWER			GPIOE->BSRRL = GPIO_Pin_6// 
#define  _FS2_POWER			GPIOE->BSRRH = GPIO_Pin_6//

//电源端口
/*左RGB灯端口控制*/
#define  RGB_LEFT1_POWER			GPIOF->BSRRL = GPIO_Pin_11//
#define  _RGB_LEFT1_POWER			GPIOF->BSRRH = GPIO_Pin_11//
#define  RGB_LEFT2_POWER			GPIOF->BSRRL = GPIO_Pin_12// 
#define  _RGB_LEFT2_POWER			GPIOF->BSRRH = GPIO_Pin_12//



/*右RGB灯端口控制*/
#define  RGB_RIGHT1_POWER			GPIOC->BSRRL = GPIO_Pin_4//
#define  _RGB_RIGHT1_POWER			GPIOC->BSRRH = GPIO_Pin_4//
#define  RGB_RIGHT2_POWER			GPIOC->BSRRL = GPIO_Pin_5//
#define  _RGB_RIGHT2_POWER			GPIOC->BSRRH = GPIO_Pin_5//

//数据端口
/*左RGB灯端口控制*/
#define	 RGB_LEFT1_HIGH	        GPIOF->BSRRL = GPIO_Pin_15//
#define  RGB_LEFT1_LOW		    GPIOF->BSRRH = GPIO_Pin_15//
#define	 RGB_LEFT2_HIGH	        GPIOG->BSRRL = GPIO_Pin_0 //
#define  RGB_LEFT2_LOW		    GPIOG->BSRRH = GPIO_Pin_0 //
/*右RGB灯端口控制*/
#define	 RGB_RIGHT1_HIGH	    GPIOF->BSRRL = GPIO_Pin_13//
#define  RGB_RIGHT1_LOW		    GPIOF->BSRRH = GPIO_Pin_13//
#define	 RGB_RIGHT2_HIGH	    GPIOF->BSRRL = GPIO_Pin_14//
#define  RGB_RIGHT2_LOW		    GPIOF->BSRRH = GPIO_Pin_14//

//左圆灯
//电源
#define  RGB_CIRCLE_LEFT_POWER		GPIOE->BSRRL = GPIO_Pin_10//
#define  _RGB_CIRCLE_LEFT_POWER	    GPIOE->BSRRH = GPIO_Pin_10//
//数据
#define	 RGB_CIRCLE_LEFT_HIGH	    GPIOE->BSRRL = GPIO_Pin_14//
#define  RGB_CIRCLE_LEFT_LOW		GPIOE->BSRRH = GPIO_Pin_14//
//右圆灯
#define  RGB_CIRCLE_RIGHT_POWER		GPIOE->BSRRL = GPIO_Pin_11//
#define  _RGB_CIRCLE_RIGHT_POWER	GPIOE->BSRRH = GPIO_Pin_11//
//数据
#define	 RGB_CIRCLE_RIGHT_HIGH	    GPIOE->BSRRL = GPIO_Pin_15//
#define  RGB_CIRCLE_RIGHT_LOW		GPIOE->BSRRH = GPIO_Pin_15//


//左气氛灯

//电源
#define  RGB_ATMP_LEFT_POWER	GPIOE->BSRRL = GPIO_Pin_12//
#define  _RGB_ATMP_LEFT_POWER	GPIOE->BSRRH = GPIO_Pin_12//
//数据
#define	 RGB_ATMP_LEFT_HIGH	    GPIOB->BSRRL = GPIO_Pin_10//
#define  RGB_ATMP_LEFT_LOW		GPIOB->BSRRH = GPIO_Pin_10//
//右气氛灯
#define  RGB_ATMP_RIGHT_POWER	GPIOE->BSRRL = GPIO_Pin_13//
#define  _RGB_ATMP_RIGHT_POWER	GPIOE->BSRRH = GPIO_Pin_13//
//数据
#define	 RGB_ATMP_RIGHT_HIGH	GPIOB->BSRRL = GPIO_Pin_11//
#define  RGB_ATMP_RIGHT_LOW		GPIOB->BSRRH = GPIO_Pin_11//



/* 4格电量显示灯  */
#define  LED4			GPIOA->BSRRL = GPIO_Pin_7
#define  _LED4			GPIOA->BSRRH  = GPIO_Pin_7
#define  LED4S			(GPIOA->ODR & GPIO_Pin_7)

#define  LED3			GPIOA->BSRRL = GPIO_Pin_6
#define  _LED3			GPIOA->BSRRH  = GPIO_Pin_6
#define  LED3S			(GPIOA->ODR & GPIO_Pin_6)

#define  LED2			GPIOA->BSRRL = GPIO_Pin_5
#define  _LED2			GPIOA->BSRRH  = GPIO_Pin_5
#define  LED2S			(GPIOA->ODR & GPIO_Pin_5)

#define  LED1			GPIOA->BSRRL = GPIO_Pin_4
#define  _LED1			GPIOA->BSRRH  = GPIO_Pin_4
#define  LED1S			(GPIOA->ODR & GPIO_Pin_4)

#define  LED_ALARM		GPIOA->BSRRL = GPIO_Pin_3
#define  _LED_ALARM		GPIOA->BSRRH  = GPIO_Pin_3
#define  LED_ALARMS		(GPIOA->ODR & GPIO_Pin_3)


void RGB_LED_Init(void);
void RGB_LED_Write0(RGB_Typed light_type);
void RGB_LED_Write1(RGB_Typed light_type);
void RGB_LED_Reset(RGB_Typed light_type);
void RGB_LED_Write_Byte(u8 byte,RGB_Typed light_type);
void RGB_LED_Write_24Bits(RGB_Typed light_type,u8 green,u8 red,u8 blue);
void RGB_LED_Light(RGB_Typed light_type,u8 num,u32 colour);
void RGB_LED_BLN(RGB_Typed light_type,u8 num);
void RGB_init_status(RGB_Typed light_type,u8 hrl_dir,u32 *arr,u8 num,u32 colour);
void RGB_LED_HRL(RGB_Typed light_type,u8 hrl_dir,u32 *arr,u8 num,u32 colour);
void RGB_Ctrl(RobotDef* robot);
void RGB_Run_Status_Light(RobotDef* robot);
void LED_Battery_Liht(RobotDef* robot);
#endif /* WS2812B_H_ */
