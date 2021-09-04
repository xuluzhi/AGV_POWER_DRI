/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: BMS.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "BMS.h"
#include "Usart.h"
#include "delay.h"
#include "User_Conf.h"

//#define BMSDebug 
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "UserTask.h"
#endif
extern uint8_t ReCmd;
uint8_t  Uart4_RxBuffer[255]; 

BmsDataDef BmsData;


/*************************************************

Function:	RequestBmsData()

Description:  请求BMS数据

*************************************************/
void RequestBmsData() //发送bms读指令
{
	
	
		/*白维通信协议*/
		USARTSendByte(0x7f,UART4);
	    USARTSendByte(0x10,UART4);
		USARTSendByte(0x02,UART4);
		USARTSendByte(0x06,UART4);
		USARTSendByte(0x11,UART4);
		USARTSendByte(0x58,UART4);
	
	/*嘉白达通信协议*/
//	UART4SendByte(0xDD);
//	UART4SendByte(0xA5);
//	UART4SendByte(0x03);
//	UART4SendByte(0x00);
//	UART4SendByte(0xFF);
//	UART4SendByte(0xFD);
//	UART4SendByte(0x77);

//	delay_ms(60);

}
/*************************************************

Function:	Uart4_ReceiveIQR()

Description:  串口4接收中断处理

*************************************************/
void Uart4_ReceiveIQR(uint8_t Buffer)
{
	static uint16_t Uart4_RecCount = 0;
	BaseType_t xHigherPriorityTaskWoken;

	if(Uart4_RecCount<27)
	{
	  Uart4_RxBuffer[Uart4_RecCount++]=Buffer;
	}
	else
	{
		 Uart4_RecCount = 0;
	   if(BiSemapBms!=NULL)//接收到数据，并且二值信号量有效
	 {
		
		xSemaphoreGiveFromISR(BiSemapBms,&xHigherPriorityTaskWoken);	//释放二值信号量
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
	 }
	}
}

/*************************************************

Function:	BmsDataUpdate()

Description:  BMS数据更新

*************************************************/
void BmsDataUpdate(void)
{
	uint8_t i=0;
	uint8_t sum=0;
	for(i=0;i<26;i++)
	{
	  sum+=Uart4_RxBuffer[i];

	}
	sum=(~sum+0x01);
	if(sum==Uart4_RxBuffer[26])
{  
	if( Uart4_RxBuffer[0]!=0x7f) return;

	BmsData.vol = (u16)Uart4_RxBuffer[15]<<8;
	BmsData.vol += (u16)Uart4_RxBuffer[16]&0x00ff;  //总电压 mv
	
	BmsData .temp  = ((float)Uart4_RxBuffer[17]+(float)Uart4_RxBuffer[18])/2;//取平均
	
	BmsData .res_cap = ((u16)Uart4_RxBuffer[22])<<8;
	BmsData .res_cap += Uart4_RxBuffer[21];	//剩余电量 AH
	
	BmsData.capacity  = ((u16)Uart4_RxBuffer[24])<<8;
	BmsData.capacity  += Uart4_RxBuffer[23];  //总容量 AH
    BmsData .level=(u16)(((float)BmsData .res_cap/(float)BmsData.capacity*100)+0.5);
	BmsData .cur =((u16)(Uart4_RxBuffer[10]<<8))|((u16)Uart4_RxBuffer[9]&0x00ff);

	
	#ifdef BMSDebug
	printf("电压为:%d\n",BmsData.vol);
	printf("电流为:%d\n",BmsData.cur);
	printf("温度为:%d\n",BmsData.temp);
	printf("rm为：%d\n",BmsData .res_cap);
	printf("cp为：%d\n",BmsData.capacity);
	printf("lv为：%d\n",BmsData.level);
    #endif

}
  

}

void Usart1_SendBms(void)		//上传BMS
{
	u8 cks;
	
	cks = 0x1A+(BmsData.vol>>8)+(BmsData.vol)+(BmsData .cur>>8)+(BmsData .cur)+BmsData .temp+BmsData .temp+(BmsData .res_cap>>8)+(BmsData .res_cap)
	+(BmsData.capacity>>8)+(BmsData.capacity)+(BmsData .level>>8)+(BmsData .level);
	
	USARTSendByte(0x55,USART1);
	USARTSendByte(0xAA,USART1);
	USARTSendByte(0x0D,USART1);
	USARTSendByte(0x0E,USART1);
	USARTSendByte(BmsData.vol>>8,USART1);
	USARTSendByte(BmsData.vol,USART1);
	USARTSendByte(BmsData .cur>>8,USART1);
	USARTSendByte(BmsData .cur,USART1);
	USARTSendByte(BmsData .temp,USART1);
	USARTSendByte(BmsData .temp,USART1);
	USARTSendByte(BmsData .res_cap>>8,USART1);
	USARTSendByte(BmsData .res_cap,USART1);
	USARTSendByte(BmsData.capacity>>8,USART1);
	USARTSendByte(BmsData.capacity,USART1);
	USARTSendByte(BmsData .level,USART1);
	USARTSendByte(0x00,USART1);
	USARTSendByte(0x00,USART1);
	USARTSendByte(cks,USART1);

	ReCmd =0;
	
}