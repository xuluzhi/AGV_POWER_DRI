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

/* �����ʱ�� */


#define  IRCAPTIMER                 0
#define  ROUNDLEDTIMER                   1
#define  ROUNDALMTIMER                   2
#define  TURNFRONTTIMER                  3
#define  TOLEFTTIMER                     4 //��ƶ�ʱ��
#define  TORIGHTTIMER                    5 //�ҵƶ�ʱ��
#define  TURNLEDTIMER                    6 //���еƶ�ʱ��
#define  DECLEDTMIER                     7
#define  SOCLEDTIMER                     8 //��ص�����ʱ��
#define  POWERKEYTIMER                   9 //��Դ������ʱ��
#define	 TIMERTOTAL			        10    //����������Ҫ�������


/*��ʱ���ṹ��*/
typedef struct{
	uint8_t Enable;
	uint8_t Flag;
	uint32_t Count;
}Timer_DataTypeDef;


/*�ⲿ��������*/

extern volatile Timer_DataTypeDef Timer[TIMERTOTAL];


/* �������� */
void TIM2_Init(void);
void TIM3_Init(void);
void Timer_IQR(void);
void Set_Timer(uint8_t Index, uint32_t Count);
void Reset_Timer(uint8_t Index);
void Timer_Init(void);
#endif
