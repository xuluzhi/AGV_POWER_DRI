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

/* �ӿ� */
#define  EN_IR                     //���������Զ��س�                  

#define  EN_INT			    __enable_irq()		            //����ȫ���ж�
#define  DIS_INT		    __disable_irq()		            //�ر�ȫ���ж�

#define  EN_EXIT       EXTI->IMR |= EXTI_Line2 | EXTI_Line3 | EXTI_Line4 //�����ⲿ�ж�
#define  DIS_EXIT      EXTI->IMR &= ~(EXTI_Line2 | EXTI_Line3 | EXTI_Line4)//�����ⲿ�ж�

//#define  WATER_S                  (GPIOE->ODR & GPIO_Pin_10)
#define  SP9108_ON                (GPIOB->BSRRL = GPIO_Pin_7)
#define  AMP_EN                   (GPIOB->BSRRH = GPIO_Pin_8)//����

#define  POWER_S                   (GPIOE->IDR & GPIO_Pin_0) 

#define  POWER_ON			GPIOF->BSRRL = GPIO_Pin_1  //��Դ��
#define  _POWER_ON		    GPIOF->BSRRH  = GPIO_Pin_1 //��Դ��


#define  FAN1_ON			GPIOF->BSRRL = GPIO_Pin_0  //���ȿ�
#define  _FAN1_ON		    GPIOF->BSRRH  = GPIO_Pin_0 //���ȹ�

#define  FAN2_ON			GPIOC->BSRRL = GPIO_Pin_13  //���ȿ�
#define  _FAN2_ON		    GPIOC->BSRRH  = GPIO_Pin_13 //���ȹ�

#define  Motor_ON			GPIOB->BSRRL = GPIO_Pin_9  //�����������
#define  _Motor_ON		    GPIOB->BSRRH  = GPIO_Pin_9 //�����������

#define  CHARGE_ON			GPIOE->BSRRL = GPIO_Pin_1  //�������
#define  _CHARGE_ON		    GPIOE->BSRRH  = GPIO_Pin_1 //�������

#define  DIANJI_EN			GPIOE->BSRRL = GPIO_Pin_3  //���ʹ��
#define  _DIANJI_EN		    GPIOE->BSRRH  = GPIO_Pin_3 //

#define  DINGSHENG_EN			GPIOE->BSRRL = GPIO_Pin_4  //�������ʹ��
#define  _DINGSHENG_EN		    GPIOE->BSRRH  = GPIO_Pin_4 //�������ʧ��

#define  DINGSHENG1_S                   (GPIOB->IDR & GPIO_Pin_3)  //����������1
#define  DINGSHENG2_S                   (GPIOB->IDR & GPIO_Pin_4)//����������2

#define  ROTATE_ZERO1                   (GPIOG->IDR & GPIO_Pin_13)  //��ת��㴫����1
#define  ROTATE_ZERO2                   (GPIOG->IDR & GPIO_Pin_14)  //��ת��㴫����2

#define  JITING_S                       (GPIOG->IDR & GPIO_Pin_6)
#define  PENGZHUANG_S                   (GPIOD->IDR & GPIO_Pin_12)
#define  PENGZHUANG_F                   (GPIOD->IDR & GPIO_Pin_13)

/* �������� */
void BspInit(void);
void CLK_Init(void);
void GPIO_Config(void);
void EXIT_Config(void);
void PeriphInit(void);








#endif
