/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Power.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __POWER_H
#define __POWER_H
#include "stm32f4xx.h"

/* �������� */
void Uart6_ReceiveIQR(uint8_t Buffer);
void Usart1_SendPowerCmd(void);	//�ϱ������ػ�����
void Usart1_SendScramCmd(void);  //�ϱ���ͣ����
void PowerKey_ctrl(void);
void Scram_Key_ctrl(void);
#endif 
