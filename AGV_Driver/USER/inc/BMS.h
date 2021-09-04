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

/* ���ݶ��� */
typedef  struct _BmsData
{
	u16 vol;	//�ܵ�ѹ
	s16 cur;	//������
	u16 temp;	//����¶�
	u16 res_cap;	//ʣ�����
	u16 capacity;	//������
	u8 level;	//ʣ���������������100%
	u16 protect;//Ĭ��λ0
	
}BmsDataDef;

/* �����ӿ� */
void RequestBmsData(void);
void Uart4_ReceiveIQR(uint8_t Buffer);
void Usart1_SendBms(void);		//�ϴ�BMS

extern uint8_t  Uart4_RxBuffer[255];

#endif 
