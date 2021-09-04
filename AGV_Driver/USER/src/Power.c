/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Power.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "Power.h"
#include "Usart.h"
#include "Device.h"
#include "delay.h"
#include "Timer.h"
#include "UserTask.h"
#include "ws2812b.h"
#include "Data.h"
#include "ws2812b.h"
u8 KeyDown_status = 0;
u8 Scram_Flag = 0;
u8 PowerKey_status = 0;


/*************************************************

Function:	Scram_Key_ctrl()

Description: 急停按键控制

*************************************************/
void Scram_Key_ctrl(void)
{
	static u8 old_sta_scram=0;
	static u8 old_sta_col=0;
	static u8 old_sta_col2=0;
	static u8 emerg_sta = 0;
	static u8 crash_status = 0;
	u8 new_sta_scram = 0;
	u8 new_sta_col = 0;
	u8 new_sta_col2 = 0;
	if(!JITING_S) new_sta_scram=1;
	else          new_sta_scram=0;
	
	if(!PENGZHUANG_F||!PENGZHUANG_S)//||//检测碰撞端口
		
	//	!DINGSHENG1_S||DINGSHENG2_S||//检测顶升传感器 其中传感器2 初始化为低电平 传感器1 初始化为高
	   // !ROTATE_ZERO1||!ROTATE_ZERO2)//旋转传感器
	{
		delay_us(10000);//延迟10ms消抖
//		if(!DINGSHENG1_S||DINGSHENG2_S)//顶升传感器
//		{
//			DINGSHENG_EN;//关闭顶升
//			if(!DINGSHENG1_S) 
//			{
//				#if DEBUG_STATE 
//				USARTSendByte(0xBB,USART1);USARTSendByte(0xCC,USART1);
//				#endif
//				xEventGroupSetBits(EventGroup_Handler,SENSOR1_JACK_BIT0);//发送此顶升传感器1事件	
//			}
//			else if(DINGSHENG2_S) 
//			{
//				#if DEBUG_STATE 
//				USARTSendByte(0xBB,USART1);USARTSendByte(0xDD,USART1);
//				#endif
//				xEventGroupSetBits(EventGroup_Handler,SENSOR2_JACK_BIT1);//发送此顶升传感器2事件	
//			}
//		}else 
//		{
//			if(!emerg_sta)//目前状态不是急停，则恢复
//			_DINGSHENG_EN;//开启顶升
//		}
//		if(!ROTATE_ZERO1||!ROTATE_ZERO2)//旋转电机传感器
//		{
//			#if DEBUG_STATE 
//			USARTSendByte(0xBB,USART1);USARTSendByte(0xFF,USART1);
//			#endif
//			xEventGroupSetBits(EventGroup_Handler,SENSOR_ROTATE_BIT2);//发送旋转电机传感器过零信号
//		}
		if(!PENGZHUANG_F)//前碰撞
		{
		  new_sta_col=1;
		}
		if(!PENGZHUANG_S)//后碰撞
		{
		  new_sta_col2=1;
		}
	}
	else if (PENGZHUANG_F)
		new_sta_col=0;	
	else if (PENGZHUANG_S)
		new_sta_col2=0;	
	if(old_sta_scram==new_sta_scram&&old_sta_col==new_sta_col&&old_sta_col2==new_sta_col2)
	{
	   return;//状态不变则退出
	}		
	//if(old_sta_scram==new_sta_scram) return;//状态不变则退出
	old_sta_scram=new_sta_scram;
	old_sta_col=new_sta_col;
	old_sta_col2=new_sta_col2;
	if(!JITING_S||(!PENGZHUANG_F||!PENGZHUANG_S))//碰撞发生
	//if(!JITING_S)//碰撞发生
	{
		//Robot.RunSta.dir=FSTOP;
		DIANJI_EN;//急停
        DINGSHENG_EN;//顶升
	}
	//else if(JITING_S)//无碰撞发送
	else if(JITING_S&&(PENGZHUANG_F&&PENGZHUANG_S))//无碰撞发送
	{		
		//Robot.RunSta.dir=ESTOP;
		_DIANJI_EN;   //急停
		//if(DINGSHENG1_S&&!DINGSHENG2_S)//检测没有顶升极限则恢复
		_DINGSHENG_EN;//顶升
	}
	if(!JITING_S && !emerg_sta)//急停协议信息发送（触发）
	{
		USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
		USARTSendByte(0x01,USART1);USARTSendByte(0xB3,USART1);
		USARTSendByte(0x01,USART1);USARTSendByte(0xC5,USART1);
		Scram_Flag = 1;
		emerg_sta = 1;
	}
	else if(JITING_S && emerg_sta)//急停协议信息发送（未触发）
	{
		USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
		USARTSendByte(0x01,USART1);USARTSendByte(0xB3,USART1);
		USARTSendByte(0x00,USART1);USARTSendByte(0xC4,USART1);
		Scram_Flag = 0;
		emerg_sta = 0;
	}
	
	
	delay_ms(20);//退出任务20ms
	crash_status=(!PENGZHUANG_F)|((!PENGZHUANG_S)<<1);//获取碰撞状态
	switch(crash_status)
	{
		case 0x00://无碰撞
		{
			USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xB5,USART1);
			USARTSendByte(0x00,USART1);USARTSendByte(0xC5,USART1);
		}break;
		case 0x01://前碰撞
		{
			USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xB5,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xC6,USART1);
		}break;
		case 0x02://后碰撞
		{
			USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xB5,USART1);
			USARTSendByte(0x02,USART1);USARTSendByte(0xC7,USART1);
		}break;
		case 0x03://前后都碰撞
		{
			USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xB5,USART1);
			USARTSendByte(0x03,USART1);USARTSendByte(0xC8,USART1);
		}break;
		default:break;
	}
	
}
/*************************************************

Function:	PowerKey_ctrl()

Description:  开关机按键控制

*************************************************/
void PowerKey_ctrl(void)
{
	static u8 firstFlag = 0;
	if(firstFlag == 0)
	{
		firstFlag =1;
		USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
		USARTSendByte(0x01,USART1);USARTSendByte(0xB1,USART1);
		USARTSendByte(0x00,USART1);USARTSendByte(0xC2,USART1);
		PowerKey_status = 1;
		KeyDown_status = 4;//初始
		return;
	}
	if(!POWER_S&&!KeyDown_status)
	{
		delay_us(10);
		if(!POWER_S)
		{
			KeyDown_status = 1;
			Set_Timer(POWERKEYTIMER,100);//500ms
		}
	}
	else if(!POWER_S&&KeyDown_status==1)
	{
		if(Timer[POWERKEYTIMER].Flag)
		{
			KeyDown_status = 2;
			Set_Timer(POWERKEYTIMER,460);//2300ms
		}
	}
	else if(!POWER_S&&KeyDown_status==2)
	{
		if(Timer[POWERKEYTIMER].Flag)//2800ms 判定为长按 规定大约3秒，预留200ms给上层处理下达命令
		{
			Reset_Timer(POWERKEYTIMER);//复位电源计数定时器
			KeyDown_status=3;//长按标记
			#if DEBUG_STATE 
			_POWER_ON;//调试阶段开启
			#endif
			if(PowerKey_status!=2)
			{
			    PowerKey_status =2;	
				//发送关机命令
				USARTSendByte(0x66,USART1);
				USARTSendByte(0xAA,USART1);
				USARTSendByte(0x01,USART1);
				USARTSendByte(0xB1,USART1);
				USARTSendByte(0x01,USART1);
				USARTSendByte(0xC3,USART1);
			}
		}
	}
	else if(!POWER_S&&KeyDown_status==4)
	{
		return;//初始开机长按，误触发关机指令
	}
	else if(POWER_S&&KeyDown_status==4)
	{
		KeyDown_status=0;
	}
	else if(POWER_S&&KeyDown_status)//当按键弹起则判断短按
	{
		if(KeyDown_status==1)
		{
			Reset_Timer(POWERKEYTIMER);//复位电源计数定时器
			KeyDown_status = 0;
			return; //500ms以内 不响应
		}
		else if(KeyDown_status==2)//不关机状态下
		{
			Reset_Timer(POWERKEYTIMER);//复位电源计数定时器
			KeyDown_status = 0;
			POWER_ON;   //短按开机
			PowerKey_status = 1;
			USARTSendByte(0x66,USART1);
			USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);
			USARTSendByte(0xB1,USART1);
			USARTSendByte(0x00,USART1);
			USARTSendByte(0xC2,USART1);
		}
		if(KeyDown_status==3)//清除长按标记
		{
			KeyDown_status=0;
		}
	}
	return ;
	
	
	
}
#if 0
uint8_t  Uart6_RxBuffer[255]; 
uint16_t Datalen2 = 0;//接收数据长度
uint8_t Uart6_RecOK = 0;
uint8_t ReCmd2 =0;
extern uint8_t  ReCmd;
/*************************************************

Function:	Uart6_ReceiveIQR()

Description:  串口6接收处理

*************************************************/
void Uart6_ReceiveIQR(uint8_t Buffer)
{
	uint8_t i,sum=0;
 	static uint16_t Uart6_RecCycle = 0;
	static uint16_t Uart6_RecCount = 0;
	static uint8_t  Uart6_RecType = SOI;
	if(Uart6_RecOK==1)return;
	switch(Uart6_RecType)
	{
		case SOI:
			if(Buffer == 0x66)
			{
			   Uart6_RecCount = 0;
			   Uart6_RxBuffer[Uart6_RecCount++] = Buffer;
                Uart6_RecCycle++;
              
			}
			else if(Buffer == 0xAA)
			{
			
			  
				Uart6_RxBuffer[Uart6_RecCount++] = Buffer;
				if(++Uart6_RecCycle>=2)
				{
                    Uart6_RecCycle = 0;
                    Uart6_RecType = LENGTH;
              
				}
			
			}
                break;
	    case LENGTH:
		  Uart6_RxBuffer[Uart6_RecCount++] = Buffer;
		  Datalen2 = Buffer;
		  Uart6_RecType = CMD;
		  break;
		
		case CMD:
		  Uart6_RxBuffer[Uart6_RecCount++] = Buffer;
		  ReCmd2=Buffer;
		  if(Datalen2>0)
		  {
		    Uart6_RecType = DATA;
		  }
		  else if(Datalen2 == 0)
		  {   
		   Uart6_RecType = CHKSUM;
		  }
		  break;
	    case DATA:
		  Uart6_RxBuffer[Uart6_RecCount++] = Buffer;
		  if(++Uart6_RecCycle>=Datalen2)
		  {
		    Uart6_RecCycle = 0;
			Uart6_RecType =CHKSUM;
		  }
		  break;
		case CHKSUM:
		  Uart6_RxBuffer[Uart6_RecCount] = Buffer;
		  for(i=0;i<Uart6_RecCount;++i)
		  {
		    sum+=Uart6_RxBuffer[i];//低八位
		  }
		  if(sum==Buffer)
		  {
		    Uart6_RecOK = 1;
		  }
		  Uart6_RecType =SOI;
		default:
				break;
	}
}

void Usart1_SendPowerCmd(void)	//上报长按关机命令
{
	if(!Uart6_RecOK)return;
    Uart6_RecOK=0;
	USARTSendByte(0x55,USART1);
	USARTSendByte(0xAA,USART1);
	USARTSendByte(0x01,USART1);
	USARTSendByte(0xB1,USART1);
	if(ReCmd2 == 0x01)
	{
		USARTSendByte(0x01,USART1);
		USARTSendByte(0xB2,USART1);
		ReCmd2 = 0;
	}else
	{
		USARTSendByte(0x00,USART1);
		USARTSendByte(0xB1,USART1);
	}
	
	ReCmd = 0;
//	memset(usart6_resbuf,0,5);
//	send_flag = 0;	//接收完成标志位清0
}


void Usart1_SendScramCmd(void)	//上报急停命令
{
	if(!Uart6_RecOK)return;
	Uart6_RecOK=0;
	USARTSendByte(0x55,USART1);
	USARTSendByte(0xAA,USART1);
	USARTSendByte(0x01,USART1);
	USARTSendByte(0xB3,USART1);
	if(ReCmd2 == 0x02)
	{
		USARTSendByte(0x01,USART1);
		USARTSendByte(0xB4,USART1);
		ReCmd2 = 0;
	}
	else
	{
		USARTSendByte(0x00,USART1);
		USARTSendByte(0xB3,USART1);
		
	}
	ReCmd = 0;
}
#endif