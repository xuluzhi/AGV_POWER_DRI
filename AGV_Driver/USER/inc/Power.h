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

/* 函数声明 */
void Uart6_ReceiveIQR(uint8_t Buffer);
void Usart1_SendPowerCmd(void);	//上报长按关机命令
void Usart1_SendScramCmd(void);  //上报急停命令
void PowerKey_ctrl(void);
void Scram_Key_ctrl(void);
#endif 
