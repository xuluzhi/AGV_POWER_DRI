/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Timer.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __USER_TIMER_H
#define __USER_TIMER_H
#include "stm32f4xx.h"

/* 软件定时器 */


#define  IRCAPTIMER                 0
#define  ROUNDLEDTIMER                   1
#define  ROUNDALMTIMER                   2
#define  TURNFRONTTIMER                  3
#define  TOLEFTTIMER                     4 //左灯定时器
#define  TORIGHTTIMER                    5 //右灯定时器
#define  TURNLEDTIMER                    6 //空闲灯定时器
#define  DECLEDTMIER                     7
#define  SOCLEDTIMER                     8 //电池电量定时器
#define  POWERKEYTIMER                   9 //电源按键定时器
#define	 TIMERTOTAL			        10    //总数，随需要可以添加


/*定时器结构体*/
typedef struct{
	uint8_t Enable;
	uint8_t Flag;
	uint32_t Count;
}Timer_DataTypeDef;


/*外部变量声明*/

extern volatile Timer_DataTypeDef Timer[TIMERTOTAL];


/* 函数声明 */
void TIM2_Init(void);
void TIM3_Init(void);
void Timer_IQR(void);
void Set_Timer(uint8_t Index, uint32_t Count);
void Reset_Timer(uint8_t Index);
void Timer_Init(void);
#endif
