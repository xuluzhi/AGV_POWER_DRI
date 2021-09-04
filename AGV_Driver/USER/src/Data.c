/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Data.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "Data.h"
#include "Timer.h"
#include "TemHum.h"
#include "Rfid.h"
#include "Device.h"
#include "voice.h"
#include "delay.h"
#include "Usart.h"
#include "Power.h"
#include "Usart1.h"
#include "motor.h"
/* 全局变量 */
//------------------------------------------faultFlag-故障状态---------------------------------------
//      BIT7        BIT6         BIT5         BIT4         BIT3	       BIT2        BIT1      BIT0
//                Ks136故障   
volatile uint8_t faultFlag = 0;

RobotDef Robot={0};
RfidDef Rfid = {0};
/*************************************************

Function:	vUser_Init()

Description:  系统初始化

*************************************************/
void vUser_Init(void)
{
	Timer_Init();	
//	SHT3X_Init(0x44);
	RFID_init();
	
    SP9108_ON;                
    AMP_EN ;
//	delay_ms(5000);	
    SetVol_SYN6658(1);
	FAN1_ON;
	FAN2_ON;
    Motor_ON;
	_CHARGE_ON;

	POWER_ON;
	PowerKey_ctrl();
    delay_ms(5000);	
	InitMotor();
	#if 0
	while(1)
	{
//		USARTSendByte(len1,USART1);
		robotvStatus(&Robot);
//        Scram_Key_ctrl();
//		vLedCtrl(&Robot);
//		 SYN_ctrl(&Robot);
//		  RFID_readEPC();
		delay_ms(500);	
		dataCtrl_RFID(&Rfid);
	}
    #endif
	//USARTSendByte(0x0A,USART1);
	//USARTSendByte(0x0D,USART1);
}


