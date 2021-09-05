/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: main.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/


#include "Device.h"
#include "UserTask.h"
#include "Data.h"
#include "TemHum.h"
#include "stdio.h"
#include "delay.h"
#include "ws2812b.h"
//void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
//{ 
//	u32 temp;	  
//	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
//	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
//	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
//	temp&=0xf;								//ȡ����λ
//	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//ʹ���ж�λ(Ҫ����Ļ�,����ICER��ӦλΪ1����)
//	NVIC->IP[NVIC_Channel]|=temp<<4;				//������Ӧ���ȼ����������ȼ�   	    	  				   
//}
//���ע��
int main(void)
{
	BspInit();
	vUser_Init();
	RGB_LED_Init();
	vUserTaskSchedule();
	while(1)
	{	 
		;
	}
}

