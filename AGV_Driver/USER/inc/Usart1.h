/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Usart1.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __USART1_H
#define __USART1_H
#include "stm32f4xx.h"
#include "Data.h"

#define FLAGA 0x66
#define FLAGB 0xAA
/* ´®¿Ú1 CMD */
#define BMS       0x0E
#define ROBTSTA   0x70
#define RUNSTA    0x02
#define PowerSta  0xB1
#define LEDFWCTRL 0x80
#define READRFID  0x91
#define WriteRFID 0x93
#define JITINGSta  0xB3
#define PENGZHUANG 0xB5
#define MOTOR_MOVE 0x78
#define MOTOR_LOCATION 0x73
#define AUTO_CHARGE 0xB2

void Uart1_ReceiveIQR(uint8_t Buffer);
void Uart1_DataRespond(void);
uint8_t IsCheckSum_OK(uint8_t *Rec,uint8_t Count);
extern void robotvStatus(RobotDef* robot);

extern volatile uint8_t Uart1_RecOK;


#endif
