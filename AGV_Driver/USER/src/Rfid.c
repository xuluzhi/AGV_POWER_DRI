/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Rfid.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "Rfid.h"
#include "Usart.h"


uint8_t  Uart3_RxBuffer[255];
volatile uint8_t Uart3_RecOK = 0;
volatile uint8_t Uart3_RecNoOK = 0;
u8 rfid_rectype = 0;
u8 cmd_rectype = 0;
u8 Rfid_DistOK = 0;
#if 1
u8 len1= 0;
u8 len2 =0;
#endif
/*************************************************

Function:	Uart3_ReceiveIQR()

Description:  RFID数据中断处理函数

*************************************************/
void Uart3_ReceiveIQR(uint8_t Buffer)
{
//	static u8 i=0;
	static u16 length = 0;
 	static uint16_t Uart3_RecCycle = 0;
	static uint16_t Uart3_RecCount = 0;
	static uint8_t  Uart3_RecType = RFID_HEADER;
	 
	switch(Uart3_RecType)
	{
		case RFID_HEADER:
			if(Buffer == 0xBB)
			{
			   Uart3_RecCount = 0;
			   Uart3_RxBuffer[Uart3_RecCount++] = Buffer;
			   Uart3_RecCycle = 0;
               Uart3_RecType = RFID_TYPE;
			   
			}
			
            break;
	    case RFID_TYPE:
			if(Buffer == 0x01)
			{
		      Uart3_RxBuffer[Uart3_RecCount++] = Buffer;
//		      rfid_rectype=Buffer;
		      Uart3_RecType = RFID_COMMAND;
		      
			}
			else
			{
				Uart3_RecType = RFID_HEADER;
			}
		 break;
		
		case RFID_COMMAND:
		  Uart3_RxBuffer[Uart3_RecCount++] = Buffer;
//		 cmd_rectype = Buffer;
		 if(Buffer==0xFF)
		 {
			 Uart3_RecType = RFID_HEADER;
		 }
		 else
		 {
		  Uart3_RecType = RFID_PL;	
		 }
		  
		  break;
	    case RFID_PL:
		  Uart3_RxBuffer[Uart3_RecCount++] = Buffer;
		  if(Uart3_RecCycle==0)
		  {
			  Uart3_RecCycle++;
		    length=((u16)Buffer)<<8;
		  }
		  else if(Uart3_RecCycle==1)
		  {
			length+=((u16)Buffer)&0x00ff;
		    Uart3_RecCycle = 0;
			Uart3_RecType =RFID_DATA;
		  }
		
		  break;
		  
		case RFID_DATA:
			if(length == 0)
			{
			  Uart3_RecType =RFID_CHKSUM;
			}
		  Uart3_RxBuffer[Uart3_RecCount++] = Buffer;
		 if(++Uart3_RecCycle>=length)
//			if(++Uart3_RecCycle>=0x19)
		 {
			Uart3_RecCycle = 0;
			Uart3_RecType =RFID_CHKSUM;
		 }
	
		 break;
		 
		case RFID_CHKSUM:
		  Uart3_RxBuffer[Uart3_RecCount++] = Buffer;
		  Uart3_RecType = RFID_END;
		break;
		
		case RFID_END:
		  if(Buffer == 0x7e)
		  {
		   Uart3_RxBuffer[Uart3_RecCount] = Buffer;
		   Uart3_RecType = RFID_HEADER;
		   Uart3_RecOK = 1;
		  }
		  else 
		  {
			  Uart3_RecType = RFID_HEADER;
		  }
		break;
		
		default:
				break;
	}
	

}

/*************************************************

Function:	dataCtrl_RFID()

Description:  RFID数据处理函数

*************************************************/
void dataCtrl_RFID(RfidDef* RfidData)
{
	u8 cksum;


	if(!Uart3_RecOK)
	{
		if(Uart3_RecNoOK)
		{					
			Uart3_RecNoOK = 0;	
			USARTSendByte(0x66,USART1);
			USARTSendByte(0xAA,USART1);
			USARTSendByte(0x06,USART1);
			USARTSendByte(0x91,USART1);
			USARTSendByte(0x00,USART1);
			USARTSendByte(0x00,USART1);
			USARTSendByte(0x00,USART1);
			USARTSendByte(0x00,USART1);
			USARTSendByte(0x00,USART1);
			USARTSendByte(0x00,USART1);
			cksum=0xA7;
			USARTSendByte(cksum,USART1);
			return;

		}
		else
			return;			
			
	}		
	Uart3_RecOK = 0;
    RfidData->epc.shelf_ID=((u16)Uart3_RxBuffer[8]<<8)+(u16)Uart3_RxBuffer[9];
	RfidData->epc.factor_ID=((u16)Uart3_RxBuffer[10]<<8)+(u16)Uart3_RxBuffer[11];
	RfidData->epc.other=((u16)Uart3_RxBuffer[12]<<8)+(u16)Uart3_RxBuffer[13];
//	if(RfidData->epc.other!=0||RfidData->epc.shelf_ID==0||RfidData->epc.factor_ID==0)return; //核对校验码
		
	
	USARTSendByte(0x66,USART1);
	USARTSendByte(0xAA,USART1);
	USARTSendByte(0x06,USART1);
	USARTSendByte(0x91,USART1);
	USARTSendByte((RfidData->epc.shelf_ID)>>8,USART1);
	USARTSendByte((u8)((RfidData->epc.shelf_ID)&0x00ff),USART1);
	USARTSendByte(RfidData->epc.factor_ID>>8,USART1);
	USARTSendByte(RfidData->epc.factor_ID&0x00ff,USART1);
	USARTSendByte(RfidData->epc.other>>8,USART1);
	USARTSendByte(RfidData->epc.other&0x00ff,USART1);
    cksum=0xA7+(u8)(RfidData->epc.shelf_ID>>8)+(u8)(RfidData->epc.shelf_ID&0x00ff)\
	+(u8)(RfidData->epc.factor_ID>>8)+(u8)(RfidData->epc.factor_ID&0x00ff)\
	+(u8)(RfidData->epc.other>>8)+(u8)(RfidData->epc.other&0x00ff);
	USARTSendByte(cksum,USART1);
}

/*************************************************

Function:	RFID_init()

Description:  RFID初始化

BB 00 12 00 01 01 14 7E

*************************************************/
u8 RFID_init(void)
{
	u8 i=0;
	USARTSendByte(0xBB,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x12,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x01,USART3);
	USARTSendByte(0x01,USART3);
	USARTSendByte(0x14,USART3);
	USARTSendByte(0x7E,USART3);
	while(Uart3_RxBuffer[2]!=0x0C&&Uart3_RxBuffer[2]!=0x12)
	{
		if(i++>250)
		{
			return 0;
		}
	}
    return 1;
	
}

/*************************************************

Function:	RFID_readEPC()

Description:  RFID读标签

BB 00 39 00 09 00 00 00 00 01 00 02 00 03 48 7E 

*************************************************/
void RFID_readEPC(void)
{
	USARTSendByte(0xBB,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x39,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x09,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x01,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x02,USART3); //02地址开始
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x03,USART3); //3个字
	USARTSendByte(0x48,USART3); //cksum
	USARTSendByte(0x7E,USART3);
	
	
}

/*************************************************

Function:	RFID_writeEPC()

Description:  RFID写标签

BB 00 49 00 0F 00 00 00 00 01 00 02 00 03 00 01 00 04 00 00 63 7E 

*************************************************/
void RFID_writeEPC(RfidDef* RfidData)
{
	u8 cksum;
	cksum=0xBC+(u8)(RfidData->epc.shelf_ID>>8)+(u8)(RfidData->epc.shelf_ID&0x00ff)\
	+(u8)(RfidData->epc.factor_ID>>8)+(u8)(RfidData->epc.factor_ID&0x00ff)\
	+(u8)(RfidData->epc.other>>8)+(u8)(RfidData->epc.other&0x00ff);
	USARTSendByte(0xBB,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x49,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x0F,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x01,USART3);
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x02,USART3); //02地址开始
	USARTSendByte(0x00,USART3);
	USARTSendByte(0x03,USART3); //3个字
	USARTSendByte(RfidData->epc.shelf_ID>>8,USART3);
	USARTSendByte(RfidData->epc.shelf_ID&0x00ff,USART3);
	USARTSendByte(RfidData->epc.factor_ID>>8,USART3);
	USARTSendByte(RfidData->epc.factor_ID&0x00ff,USART3);
	USARTSendByte(RfidData->epc.other>>8,USART3);
	USARTSendByte(RfidData->epc.other&0x00ff,USART3);
	USARTSendByte(cksum,USART3); //cksum
	USARTSendByte(0x7E,USART3);
	
}
