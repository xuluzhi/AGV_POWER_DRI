/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Adc.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __USER_ADC_H
#define __USER_ADC_H
#include "stm32f4xx.h"



/* 函数接口定义 */
void ADC_Config(void);
void DMA_IT_Ctrl(void);
void DMA_Configuration(void);
void ADC_IT_Ctrl(void);
void Fallpre_Ctrl(void);
void ADCInit_Timer(void);
#endif
