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
#include "FreeRTOS.h"					//FreeRTOSʹ��
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

Description:  ����BMS����

*************************************************/
void RequestBmsData() //����bms��ָ��
{
	
	
		/*��άͨ��Э��*/
		USARTSendByte(0x7f,UART4);
	    USARTSendByte(0x10,UART4);
		USARTSendByte(0x02,UART4);
		USARTSendByte(0x06,UART4);
		USARTSendByte(0x11,UART4);
		USARTSendByte(0x58,UART4);
	
	/*�ΰ״�ͨ��Э��*/
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

Description:  ����4�����жϴ���

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
	   if(BiSemapBms!=NULL)//���յ����ݣ����Ҷ�ֵ�ź�����Ч
	 {
		
		xSemaphoreGiveFromISR(BiSemapBms,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
	 }
	}
}

/*************************************************

Function:	BmsDataUpdate()

Description:  BMS���ݸ���

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
	BmsData.vol += (u16)Uart4_RxBuffer[16]&0x00ff;  //�ܵ�ѹ mv
	
	BmsData .temp  = ((float)Uart4_RxBuffer[17]+(float)Uart4_RxBuffer[18])/2;//ȡƽ��
	
	BmsData .res_cap = ((u16)Uart4_RxBuffer[22])<<8;
	BmsData .res_cap += Uart4_RxBuffer[21];	//ʣ����� AH
	
	BmsData.capacity  = ((u16)Uart4_RxBuffer[24])<<8;
	BmsData.capacity  += Uart4_RxBuffer[23];  //������ AH
    BmsData .level=(u16)(((float)BmsData .res_cap/(float)BmsData.capacity*100)+0.5);
	BmsData .cur =((u16)(Uart4_RxBuffer[10]<<8))|((u16)Uart4_RxBuffer[9]&0x00ff);

	
	#ifdef BMSDebug
	printf("��ѹΪ:%d\n",BmsData.vol);
	printf("����Ϊ:%d\n",BmsData.cur);
	printf("�¶�Ϊ:%d\n",BmsData.temp);
	printf("rmΪ��%d\n",BmsData .res_cap);
	printf("cpΪ��%d\n",BmsData.capacity);
	printf("lvΪ��%d\n",BmsData.level);
    #endif

}
  

}

void Usart1_SendBms(void)		//�ϴ�BMS
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