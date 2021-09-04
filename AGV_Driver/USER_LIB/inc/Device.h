/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Device.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/

#ifndef _DEVICE_H
#define _DEVICE_H

#include "stm32f4xx.h"

/* 接口 */
#define  EN_IR                     //开启红外自动回充                  

#define  EN_INT			    __enable_irq()		            //开启全局中断
#define  DIS_INT		    __disable_irq()		            //关闭全局中断

#define  EN_EXIT       EXTI->IMR |= EXTI_Line2 | EXTI_Line3 | EXTI_Line4 //开放外部中断
#define  DIS_EXIT      EXTI->IMR &= ~(EXTI_Line2 | EXTI_Line3 | EXTI_Line4)//屏蔽外部中断

//#define  WATER_S                  (GPIOE->ODR & GPIO_Pin_10)
#define  SP9108_ON                (GPIOB->BSRRL = GPIO_Pin_7)
#define  AMP_EN                   (GPIOB->BSRRH = GPIO_Pin_8)//拉低

#define  POWER_S                   (GPIOE->IDR & GPIO_Pin_0) 

#define  POWER_ON			GPIOF->BSRRL = GPIO_Pin_1  //电源开
#define  _POWER_ON		    GPIOF->BSRRH  = GPIO_Pin_1 //电源关


#define  FAN1_ON			GPIOF->BSRRL = GPIO_Pin_0  //风扇开
#define  _FAN1_ON		    GPIOF->BSRRH  = GPIO_Pin_0 //风扇关

#define  FAN2_ON			GPIOC->BSRRL = GPIO_Pin_13  //风扇开
#define  _FAN2_ON		    GPIOC->BSRRH  = GPIO_Pin_13 //风扇关

#define  Motor_ON			GPIOB->BSRRL = GPIO_Pin_9  //电机驱动器开
#define  _Motor_ON		    GPIOB->BSRRH  = GPIO_Pin_9 //电机驱动器关

#define  CHARGE_ON			GPIOE->BSRRL = GPIO_Pin_1  //充电器开
#define  _CHARGE_ON		    GPIOE->BSRRH  = GPIO_Pin_1 //充电器关

#define  DIANJI_EN			GPIOE->BSRRL = GPIO_Pin_3  //电机使能
#define  _DIANJI_EN		    GPIOE->BSRRH  = GPIO_Pin_3 //

#define  DINGSHENG_EN			GPIOE->BSRRL = GPIO_Pin_4  //顶升电机使能
#define  _DINGSHENG_EN		    GPIOE->BSRRH  = GPIO_Pin_4 //顶升电机失能

#define  DINGSHENG1_S                   (GPIOB->IDR & GPIO_Pin_3)  //顶升传感器1
#define  DINGSHENG2_S                   (GPIOB->IDR & GPIO_Pin_4)//顶升传感器2

#define  ROTATE_ZERO1                   (GPIOG->IDR & GPIO_Pin_13)  //旋转零点传感器1
#define  ROTATE_ZERO2                   (GPIOG->IDR & GPIO_Pin_14)  //旋转零点传感器2

#define  JITING_S                       (GPIOG->IDR & GPIO_Pin_6)
#define  PENGZHUANG_S                   (GPIOD->IDR & GPIO_Pin_12)
#define  PENGZHUANG_F                   (GPIOD->IDR & GPIO_Pin_13)

/* 函数声明 */
void BspInit(void);
void CLK_Init(void);
void GPIO_Config(void);
void EXIT_Config(void);
void PeriphInit(void);








#endif
