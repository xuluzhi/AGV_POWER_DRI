/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Usart.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef _USART_H
#define _USART_H
#include "stm32f4xx.h"
/* 串口1 通用 */
#define  SOI 			        1
#define  LENGTH			        2
#define  CMD			        3
#define  DATA                   4
#define  CHKSUM			        5
#define  END                    6

/* 串口2  RFID*/
#define  RFID_HEADER 			         1
#define  RFID_TYPE                       2
#define  RFID_COMMAND                    3
#define  RFID_PL                         4
#define  RFID_DATA                       5
#define  RFID_CHKSUM                     6
#define  RFID_END                        7

/*串口4 MOTOR*/
enum
{
	MOTOR_ADDR = 1,
	MOTOR_FUN,
	MOTOR_LEN,
	MOTOR_DATA,
	MOTOR_CRC	
};

/*外部变量声明*/










/*函数声明*/

void USARTSendByte(unsigned char SendData,USART_TypeDef* USARTx);
void Usart_Config(void);
void Usart1_init(uint32_t BaudRate);
void Usart2_init(uint32_t BaudRate);
void Usart4_init(uint32_t BaudRate);
void Usart3_init(uint32_t BaudRate);
void Usart6_init(uint32_t BaudRate);
void Usart5_init(uint32_t BaudRate);











#endif
