/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Timer.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "Timer.h"

/* 全局变量 */
volatile Timer_DataTypeDef Timer[TIMERTOTAL];
/*************************************************

Function:	TIM2_init()

Description:  定时器2配置初始化   定时器2作为软件定时器 84M 1ms

*************************************************/
void TIM2_Init(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    TIM_DeInit(TIM2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 1000-1;
    TIM_TimeBaseStructure.TIM_Prescaler = 84-1;                                   
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);



}

/*************************************************

Function:	TIM3_init()

Description:  定时器3配置初始化   定时器2作为软件定时器 84M 1ms

*************************************************/
void TIM3_Init(void)
{

	    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    TIM_DeInit(TIM3);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 5000-1;   //5ms中断一次
    TIM_TimeBaseStructure.TIM_Prescaler = 84-1;                                   
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);



}
/*************************************************

Function:	Timer_IQR()

Description:  软件定时器处理函数

*************************************************/

void Timer_IQR(void)
{
	uint8_t i;
  for(i = 0; i < TIMERTOTAL; i++)
	{
		if(Timer[i].Enable)
		{
			if(Timer[i].Count > 1)
			{
				Timer[i].Count--;
			}
			else
			{
				Timer[i].Enable = 0;
				Timer[i].Flag = 1;
			}
		}
	}

}


/*************************************************

Function:	  Set_Timer()

Description:	启动软件定时器

*************************************************/
void Set_Timer(uint8_t Index, uint32_t Count)
{
	Timer[Index].Flag = 0;
	Timer[Index].Count = Count;
	Timer[Index].Enable = 1;
}

/*************************************************

Function:		Reset_Timer()

Description:	复位软件定时器

*************************************************/
void Reset_Timer(uint8_t Index)
{
	Timer[Index].Enable = 0;
	Timer[Index].Flag = 0;
}
/*************************************************

Function:		Timer_Init()

Description:	软件定时器初始化

*************************************************/
void Timer_Init(void)
{

	Reset_Timer(IRCAPTIMER);
	Set_Timer(ROUNDLEDTIMER,2);
	Set_Timer(ROUNDALMTIMER,2);
	Set_Timer(TURNFRONTTIMER,2);
	Set_Timer(TOLEFTTIMER,2);
	Set_Timer(TORIGHTTIMER,2);
	Set_Timer(TURNLEDTIMER,2);
	Set_Timer(DECLEDTMIER,2);
	Set_Timer(SOCLEDTIMER,2);	
}