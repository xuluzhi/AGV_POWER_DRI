/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: voice.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "voice.h"
#include "Usart.h"

#define   VOI_SYN   0x01 //语音合成
#define   STOP_SYN  0x02 //停止合成
#define   SUSP_SYN  0x03 //暂停合成
#define   REC_SYN   0x04 //恢复合成
#define   STA_QUE   0x21 //MCU状态请求
#define   STANDBY   0x22 //MCU进入休眠
#define   WAKEUP    0xFF //MCU唤醒
#define   SET_Vol   0x5B //设置音量
#define   SET_Voi   0x5C //设置发音人

//FD 00 12 01 01 B7 A2 C9 FA C5 F6 D7 B2 A3 AC C7 EB B4 A6 C0 ED 
u8 voice_1[18]={0xB7,0xA2,0xC9,0xFA,0xC5,0xF6,0xD7,0xB2,0xA3,0xAC,0xC7,\
0xEB,0xB4,0xA6,0xC0,0xED };//碰撞
//FD 00 4E 01 01 BB FA C6 F7 D5 FD D4 DA D4 CB D0 D0 D6 D0 A3 AC
//C7 EB B1 A3 B3 D6 C2 B7 C3 E6 D5 FB BD E0 A3 BB CE AA C1 CB C4 FA
//B5 C4 B0 B2 C8 AB A3 AC B7 C7 BD F4 BC B1 C7 E9 BF F6 CF C2 A3 AC
//C7 EB CE F0 BF BF BD FC D0 D0 B3 B5 C7 F8 D3 F2
u8 voice_2[78]={0xBB,0xFA,0xC6,0xF7,0xD5,0xFD,0xD4,0xDA,0xD4,0xCB,0xD0,0xD0,0xD6,0xD0,0xA3,0xAC,\
0xC7,0xEB,0xB1,0xA3,0xB3,0xD6,0xC2,0xB7,0xC3,0xE6,0xD5,0xFB,0xBD,0xE0,0xA3,0xBB,0xCE,0xAA,0xC1,0xCB,0xC4,0xFA,\
0xB5,0xC4,0xB0,0xB2,0xC8,0xAB,0xA3,0xAC,0xB7,0xC7,0xBD,0xF4,0xBC,0xB1,0xC7,0xE9,0xBF,0xF6,0xCF,0xC2,0xA3,0xAC,\
0xC7,0xEB,0xCE,0xF0,0xBF,0xBF,0xBD,0xFC,0xD0,0xD0,0xB3,0xB5,0xC7,0xF8,0xD3,0xF2	
};//自动运行中
	
// FD 00 06 01 01 D2 EC B3 A3 
u8 voice_3[6]={0xD2,0xEC,0xB3,0xA3};
	//异常
//FD 00 10 01 01 B5 B9 B3 B5 A3 AC C7 EB D7 A2 D2 E2 A3 A1
u8 voice_4[16]={0xB5,0xB9,0xB3,0xB5,0xA3,0xAC,0xC7,0xEB,0xD7,0xA2,0xD2,0xE2,0xA3,0xA1};
	//倒车，请注意
//FD 00 10 01 01 D7 AA CD E4 A3 AC C7 EB D7 A2 D2 E2 0D 0A
u8 voice_5[16]={0xD7,0xAA,0xCD,0xE4,0xA3,0xAC,0xC7,0xEB,0xD7,0xA2,0xD2,0xE2,0x0D,0x0A};
	//转弯，请注意
//FD 00 0A 01 01 B6 D4 BD D3 B3 C9 B9 A6
u8 voice_6[10]={0xB6,0xD4,0xBD,0xD3,0xB3,0xC9,0xB9,0xA6};
	//对接成功
//FD 00 0A 01 01 B6 D4 BD D3 CA A7 B0 DC
u8 voice_7[10]={0xB6,0xD4,0xBD,0xD3,0xCA,0xA7,0xB0,0xDC};
	//对接失败
//FD 00 0E 01 01 BD F8 C8 EB BD F4 BC B1 CD A3 D6 B9
u8 voice_8[14]={0xBD,0xF8,0xC8,0xEB,0xBD,0xF4,0xBC,0xB1,0xCD,0xA3,0xD6,0xB9};
	//进入紧急停止
//FD 00 0E 01 01 D7 BC B1 B8 BD F8 C8 EB D4 CB D0 D0
u8 voice_9[14]={0xD7,0xBC,0xB1,0xB8,0xBD,0xF8,0xC8,0xEB,0xD4,0xCB,0xD0,0xD0};
	//准备进入运行
u8 SYN_idleFlag = 1;
u8 SYN_sendFlag = 0;


 
/*************************************************

Function:	Uart6_ReceiveIQR()

Description:  串口6接收处理

*************************************************/
void Uart6_ReceiveIQR(uint8_t Buffer)
{
//	SYN_sendFlag=0;
	if(Buffer!=0x4f) return;
	SYN_idleFlag = 1;
		
	
	
	
}
/*************************************************

Function:	SetVoi_SYN6658(u8 voi );

Description:  SYN6658设置声音  voi: 0-5  设置成功：1 设置失败 ：0

*************************************************/
u8 SetVoi_SYN6658(u8 voi)
{
	if(voi>5) return 0;
	else if(voi==0)
	{
		//FD 00 06 01 01 5B 6D 33 5D
		USARTSendByte(0xFD,USART6);
		USARTSendByte(0x00,USART6);
		USARTSendByte(0x06,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x5B,USART6);
		USARTSendByte(0x6D,USART6);
		USARTSendByte(0x33,USART6);
		USARTSendByte(0x5D,USART6);
	}
	else  
	{
		USARTSendByte(0xFD,USART6);
		USARTSendByte(0x00,USART6);
		USARTSendByte(0x07,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x5B,USART6);
		USARTSendByte(0x6D,USART6);
		USARTSendByte(0x35,USART6);
		USARTSendByte(0x30+voi,USART6);
		USARTSendByte(0x5D,USART6);
	}
	return 1;
	
}

/*************************************************

Function:	SetVol_SYN6658(u8 vol );

Description:  SYN6658设置音量  vol: 0-10  设置成功：1 设置失败 ：0

*************************************************/
u8 SetVol_SYN6658(u8 vol)
{
	if(vol>10) return 0;
	else if(vol==10)
	{
		
		USARTSendByte(0xFD,USART6);
		USARTSendByte(0x00,USART6);
		USARTSendByte(0x07,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x5B,USART6);
		USARTSendByte(0x76,USART6);
		USARTSendByte(0x31,USART6);
		USARTSendByte(0x30,USART6);
		USARTSendByte(0x5D,USART6);
	}
	else  
	{
		USARTSendByte(0xFD,USART6);
		USARTSendByte(0x00,USART6);
		USARTSendByte(0x06,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x01,USART6);
		USARTSendByte(0x5B,USART6);
		USARTSendByte(0x76,USART6);
		USARTSendByte(0x30+vol,USART6);
		USARTSendByte(0x5D,USART6);
	}
	return 1;
	
}
/*************************************************

Function:	SendCommand_SYN6658();

Description:  SYN6658命令发送函数

*************************************************/
void SendCommand_SYN6658(u8 SendCmd,u16 num ,u8* str)
{
	switch(SendCmd)
	{
		
		case SET_Vol: 
			
		      break;
		case SET_Voi: break;
		
		      break;
		case VOI_SYN:
		     SelectSYN(num,str);
	          break;
		
		case STOP_SYN:
			 USARTSendByte(0xFD,USART6);
		     USARTSendByte(0x00,USART6);
		     USARTSendByte(0x01,USART6);
		     USARTSendByte(0x02,USART6);
		      break;
		
		case SUSP_SYN:
			 USARTSendByte(0xFD,USART6);
		     USARTSendByte(0x00,USART6);
		     USARTSendByte(0x01,USART6);
		     USARTSendByte(0x03,USART6);
		     break;
		
		case REC_SYN:
			 USARTSendByte(0xFD,USART6);
		     USARTSendByte(0x00,USART6);
		     USARTSendByte(0x01,USART6);
		     USARTSendByte(0x04,USART6);
		     break;
		
		case STA_QUE:
			 USARTSendByte(0xFD,USART6);
		     USARTSendByte(0x00,USART6);
		     USARTSendByte(0x01,USART6);
		     USARTSendByte(0x21,USART6);
		     break;
		case STANDBY:
			 USARTSendByte(0xFD,USART6);
		     USARTSendByte(0x00,USART6);
		     USARTSendByte(0x01,USART6);
		     USARTSendByte(0x22,USART6);
			break;
		
		case WAKEUP:
			 USARTSendByte(0xFD,USART6);
		     USARTSendByte(0x00,USART6);
		     USARTSendByte(0x01,USART6);
		     USARTSendByte(0xFF,USART6);
			break;
		default: break;
				
	}
		
}

/*************************************************

Function:	SelectSYN();

Description:  SYN6658语音合成命令函数

*************************************************/
void SelectSYN(u16 num ,u8* str)
{
	u8 i=0;
	USARTSendByte(0xFD,USART6);
    USARTSendByte((u8)(num>>8),USART6);
	USARTSendByte((u8)(num&0x00ff),USART6);
	USARTSendByte(0x01,USART6);
	USARTSendByte(0x01,USART6); //GBK码
	for(i=0;i<num-2;i++)
	{
		USARTSendByte(*(str+i),USART6);
	}
}

/*************************************************

Function:	SYN_ctrl();

Description:  SYN6658语音合成命令函数

*************************************************/
void SYN_ctrl(RobotDef *robot)
{
	switch (robot->RunSta.dir)
	{
		case INITS:
	    case PRERUN:
		{  
			if(SYN_idleFlag==1)
			{
			  SendCommand_SYN6658(VOI_SYN,14,voice_9);
			  SYN_idleFlag=0;
			}
		}
		    break;
		case FORWARD: break;
		case BACK:
			if(SYN_idleFlag==1)
			{
			  SendCommand_SYN6658(VOI_SYN,16,voice_4);
			  SYN_idleFlag=0;
			}; break;
		case TOLEFT:
	    case TORIGHT:
            if(SYN_idleFlag==1)
			{
			  SendCommand_SYN6658(VOI_SYN,16,voice_5);
			  SYN_idleFlag=0;
			}; break;
		case ESTOP:  
			 if(SYN_idleFlag==1)
			{
			  SendCommand_SYN6658(VOI_SYN,14,voice_8);
			  SYN_idleFlag=0;
			}; break;
	    case FSTOP:
              if(SYN_idleFlag==1)
			{
			  SendCommand_SYN6658(VOI_SYN,6,voice_3);
			  SYN_idleFlag=0;
			}; break;			
	    case DIDLE: break;
		default: break;
	}
	
	
	
}
