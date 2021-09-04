/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: BMS.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __BMS_H
#define __BMS_H

#include "stm32f4xx.h"

/* 数据定义 */
typedef  struct _BmsData
{
	u16 vol;	//总电压
	s16 cur;	//充电电流
	u16 temp;	//最高温度
	u16 res_cap;	//剩余电量
	u16 capacity;	//总容量
	u8 level;	//剩余电量除以总容量100%
	u16 protect;//默认位0
	
}BmsDataDef;

/* 函数接口 */
void RequestBmsData(void);
void Uart4_ReceiveIQR(uint8_t Buffer);
void Usart1_SendBms(void);		//上传BMS

extern uint8_t  Uart4_RxBuffer[255];

#endif 
