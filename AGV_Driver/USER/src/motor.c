/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: motor.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "motor.h"
#include "usart.h"
#include "delay.h"

u8 addr = 0x01;//1-31

u8 ToMotorbuffer[11]={0};

motorData_t motorData = {0};

uint8_t  Uart4_RxBuffer[255]; 
volatile uint8_t Uart4_RecOK = 0;

/*************************************************

Function:	伺服驱动器初始化

Description: SendCmdToMotor()

*************************************************/
void InitMotor(void)
{
	//设置操作模式
	SendCmdToMotor(MotCmd_net, 0x0000, 0x00000000);
	delay_ms(20);
	//设置控制模式
	SendCmdToMotor(MotCmd_pos, 0x0002, 0x00000000);
	delay_ms(20);
	//设置运动模式
	SendCmdToMotor(MotCmd_run, 0x0001, 0x00000000);	
	delay_ms(20);
	//使能电机
	SendCmdToMotor(MotCmd_ctr, 0x0001, 0x00000000);
	delay_ms(20);
	//读取电机位置实际值
	SendCmdToMotor(MotCmd_locpos, 0x0002, 0x00000000);
	delay_ms(20);
}

/*************************************************

Function:	Uart4_ReceiveIQR()

Description:  RFID数据中断处理函数

*************************************************/
void Uart4_ReceiveIQR(uint8_t Buffer)
{
	static u8 i=0;
	static u8 length = 0;
 	static u8 Uart4_RecCycle = 0;
	static uint16_t Uart4_RecCount = 0;
	static uint8_t  Uart4_RecType = MOTOR_ADDR;
	 
	switch(Uart4_RecType)
	{
		case MOTOR_ADDR:
			if(Buffer == 0x01)
			{
			   Uart4_RecCount = 0;
			   Uart4_RxBuffer[Uart4_RecCount++] = Buffer;
			   Uart4_RecCycle = 0;
               Uart4_RecType = MOTOR_FUN;			   
			}			
            break;
	    case MOTOR_FUN:
			if(Buffer == 0x03)
			{
		      Uart4_RxBuffer[Uart4_RecCount++] = Buffer;
//		      rfid_rectype=Buffer;
		      Uart4_RecType = MOTOR_LEN;
		      
			}
			else if(Buffer == 0x06)
			{
				Uart4_RecType = MOTOR_ADDR;
			}
		 break;
	    case MOTOR_LEN:
			Uart4_RxBuffer[Uart4_RecCount++] = Buffer;
			length = Buffer;
			Uart4_RecType = MOTOR_DATA;		
		  break;
		  
		case MOTOR_DATA:
		  Uart4_RxBuffer[Uart4_RecCount++] = Buffer;
		 if(++Uart4_RecCycle >= length)
		 {
			Uart4_RecCycle = 0;
			Uart4_RecType =MOTOR_CRC;
		 }	
		 break;		 
		case MOTOR_CRC:
		  Uart4_RxBuffer[Uart4_RecCount++] = Buffer;
		 if(++Uart4_RecCycle >= 2)
		 {
			//加入校验和比较（后补）
			Uart4_RecCycle = 0;
			Uart4_RecType =MOTOR_ADDR;
			Uart4_RecOK = 1;
		 }	
		break;
		
//		case RFID_END:
//		  if(Buffer == 0x7e)
//		  {
//		   Uart4_RxBuffer[Uart4_RecCount] = Buffer;
//		   Uart4_RecType = RFID_HEADER;
//		   Uart4_RecOK = 1;
//		  }
//		  else 
//		  {
//			  Uart4_RecType = RFID_HEADER;
//		  }
//		  break;
		
		default:
			break;
	}	
}

/*************************************************

Function:	dataCtrl_MOTOR()

Description:  电机回传的数据处理函数

*************************************************/
void dataCtrl_MOTOR(motorData_t* MotorData)
{
	u8 cksum;


	if(!Uart4_RecOK) return;
	Uart4_RecOK = 0;
	MotorData->pos_data = Uart4_RxBuffer[3]<<24 || Uart4_RxBuffer[4]<<16 || Uart4_RxBuffer[5]<<8 || Uart4_RxBuffer[6];	
	
	USARTSendByte(0x66,USART1);
	USARTSendByte(0xAA,USART1);
	USARTSendByte(0x04,USART1);
	USARTSendByte(0x73,USART1);
	USARTSendByte(Uart4_RxBuffer[3],USART1);
	USARTSendByte(Uart4_RxBuffer[4],USART1);
	USARTSendByte(Uart4_RxBuffer[5],USART1);
	USARTSendByte(Uart4_RxBuffer[6],USART1);
    cksum=0x87 + Uart4_RxBuffer[3] + Uart4_RxBuffer[4] + Uart4_RxBuffer[5] + Uart4_RxBuffer[6];
	USARTSendByte(cksum,USART1);
}

/*************************************************

Function:	命令给伺服驱动器

Description: SendCmdToMotor()

*************************************************/
void SendCmdToMotor(u16 Cmd,u16 cmD,int32_t disSet)
{
	u8 len;
	u8 cmdType;
	
	switch (Cmd)
	{
		case MotCmd_net:
		case MotCmd_pos:
		case MotCmd_run:
		case MotCmd_ctr:
		case MotCmd_str:
		{
			len=6;
			cmdType=MotCmdCommand;
			ToMotorbuffer[0]=addr;
		    ToMotorbuffer[1]=cmdType; 
            ToMotorbuffer[2]=(u8)(Cmd>>8);	
            ToMotorbuffer[3]=(u8)(Cmd&0x00ff);
            ToMotorbuffer[4]=(u8)(cmD>>8);	
            ToMotorbuffer[5]=(u8)(cmD&0x00ff);			
			SendToMotor(ToMotorbuffer,len);
		}break;
		
		case MotCmd_dis:
		{
			len=11;
			cmdType=MotCmdWrite;
			ToMotorbuffer[0]=addr;
		    ToMotorbuffer[1]=cmdType; 
            ToMotorbuffer[2]=(u8)(Cmd>>8);	
            ToMotorbuffer[3]=(u8)(Cmd&0x00ff);
            ToMotorbuffer[4]=0x00;
            ToMotorbuffer[5]=0x02;	
            ToMotorbuffer[6]=0x04;
            ToMotorbuffer[7]=(u8)(disSet>>24);
            ToMotorbuffer[8]=(u8)((0x00ff0000&disSet)>>16);	
            ToMotorbuffer[9]=(u8)((0x0000ff00&disSet)>>8);
            ToMotorbuffer[10]=(u8)(0x000000ff&disSet);			
            			
			SendToMotor(ToMotorbuffer,len);
			
		}break;		
		case MotCmd_setpos:		
		case MotCmd_locpos:
		{
			len=6;
			cmdType=MotCmdRead;
			ToMotorbuffer[0]=addr;
		    ToMotorbuffer[1]=cmdType; 
            ToMotorbuffer[2]=(u8)(Cmd>>8);	
            ToMotorbuffer[3]=(u8)(Cmd&0x00ff);
            ToMotorbuffer[4]=(u8)(cmD>>8);	
            ToMotorbuffer[5]=(u8)(cmD&0x00ff);			
			SendToMotor(ToMotorbuffer,len);
			
		}break;
			
		default: break;
	}
	
}
/*************************************************

Function:	getCRC16()

Description: 得到校验码

*************************************************/
u16 getCRC16(u8 *ptr,u8 len)
{
	u8 i;
	u16 crc = 0xFFFF;
	if(len==0)
	{
	  len = 1;
	}
	while(len--)
 {
	  crc ^= *ptr;
	  for(i=0; i<8; i++)
   {
	  if(crc&1)
	 {
	  crc >>= 1;
	  crc ^= 0xA001;
	 }
	  else
	  crc >>= 1;
	
   }
   
	   ptr++;
 }
return(crc);
}


/*************************************************

Function:	发送子函数

Description: SendToMotor()

*************************************************/
void SendToMotor(u8 *str ,u8 len)
{
	u8 i=0;
	u16 crc = 0;
	for(i=0;i<len;i++)
{
	USARTSendByte(*(str+i),UART4);

}
	crc=getCRC16(str,len);
    USARTSendByte((u8)(crc&0x00FF),UART4);
	USARTSendByte((u8)(crc>>8)&0xFF,UART4);
    
}

