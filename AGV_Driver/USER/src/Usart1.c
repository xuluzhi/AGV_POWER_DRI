/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Usart1.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "Usart1.h"
#include "Usart.h"
#include "ks136.h"
#include "Power.h"
#include "BMS.h"
#include "Device.h"
#include "Data.h"
#include "Rfid.h"
#include "motor.h"
#include "ws2812b.h"

#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "UserTask.h"
#endif

uint8_t  Uart1_RxBuffer[255];             
uint8_t  Uart1_TxBuffer[255];             
uint8_t  ReCmd = 0; //发送命令标志   CMD
uint16_t Datalen = 0; //接收数据长度
volatile uint8_t Uart1_RecOK = 0;
extern volatile uint8_t Uart3_RecNoOK;
static uint16_t Uart1_RecCount = 0;

extern RobotDef Robot;
extern QueueHandle_t Queue_usart1_Rec;
extern SemaphoreHandle_t BiSemapUsart1;
BaseType_t Usart1_Rec_return;

extern motorData_t motorData;
/*===========================================================================
 *函数名称  : Uart1_ReceiveIQR
 *功能描述  : 串口1中断数据处理函数
 *参数说明  : Buffer 接收到的单个字节数据
 *返回值    : 无
 *备注      : 被UART1中断服务函数USART1_IRQHandler()调用
============================================================================*/
void Uart1_ReceiveIQR(uint8_t Buffer)	
{
 	static uint16_t Uart1_RecCycle = 0;
	static uint8_t  Uart1_RecType = SOI;
	static u8 Cmd;
	BaseType_t xHigherPriorityTaskWoken;
	switch(Uart1_RecType)
	{
		case SOI:
			if(Buffer == 0x66)
			{
			   Uart1_RecOK = 0;
			   Uart1_RecCount = 0;
			   Uart1_RxBuffer[Uart1_RecCount++] = Buffer;
			   if(++Uart1_RecCycle>=2)
			   {
					Uart1_RecCycle = 0;
					Uart1_RecType =LENGTH;
			   }
			}	
            else if(Buffer == 0xAA)
			{
			  Uart1_RxBuffer[Uart1_RecCount++] = Buffer;
				if(++Uart1_RecCycle>=2)
			   {
				Uart1_RecCycle = 0;
				Uart1_RecType =LENGTH;
			   }
			}				
                break;
	    case LENGTH:
		  Uart1_RxBuffer[Uart1_RecCount++] = Buffer;
		  Datalen = Buffer;
		  Uart1_RecType = CMD;
		  break;
		
		case CMD:
		  Uart1_RxBuffer[Uart1_RecCount++] = Buffer;
		  Cmd=Buffer;
		  if(Datalen>0)
		  {
		    Uart1_RecType = DATA;
		  }
		  else if(Datalen == 0)
		  {   
		   Uart1_RecType = CHKSUM;
		  }
		  break;
		 
	    case DATA:
		  Uart1_RxBuffer[Uart1_RecCount++] = Buffer;
		  if(++Uart1_RecCycle>=Datalen)
		  {
		    Uart1_RecCycle = 0;
			Uart1_RecType =CHKSUM;
		  }
		  break;
		case CHKSUM:
		  Uart1_RxBuffer[Uart1_RecCount++] = Buffer;
		    if(Cmd==BMS)
			{
				Uart1_RecOK = 1;//电池数据
			}
			else if(Cmd==ROBTSTA)
			{
			    Uart1_RecOK = 2;//运行状态
			}
			else if(Cmd==PowerSta)
			{
			    Uart1_RecOK = 3;//开机状态
			}
			else if(Cmd==LEDFWCTRL)
			{
				Uart1_RecOK = 4;//氛围灯控制
			}
			else if(Cmd==READRFID)
			{
				Uart1_RecOK = 5;//读货架号信息
			}
			else if(Cmd==WriteRFID)
			{
				Uart1_RecOK = 6;//写货架号信息
			}
			else if(Cmd ==JITINGSta)
			{
				Uart1_RecOK = 7;	
			}
			else if(Cmd == PENGZHUANG)
			{
				Uart1_RecOK = 8;
			}
			else if(Cmd == MOTOR_MOVE)
			{
				Uart1_RecOK = 9;
			}
			else if(Cmd == MOTOR_LOCATION)
			{
				Uart1_RecOK = 10;
			}
			else if(Cmd == AUTO_CHARGE)
			{
				Uart1_RecOK = 11;
			}
		  Uart1_RecType =SOI;
		break;
		default:
		break;
	}

	//释放二值信号量
	if((Uart1_RecOK!=0)&&(BiSemapUsart1!=NULL))//接收到数据，并且二值信号量有效
	{
//		Uart1_RecOK = 0;
		xSemaphoreGiveFromISR(BiSemapUsart1,&xHigherPriorityTaskWoken);	//释放二值信号量
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
	}
}

/*************************************************

Function:	Uart1_DataRespond()

Description:  串口1回应

*************************************************/
void Uart1_DataRespond(void)
{
	
	Datalen+=4;
	if(IsCheckSum_OK(Uart1_RxBuffer,Datalen))
	{
	  switch (ReCmd)
		{
			case 0X0A:		
			{
				
			}
			break;
			case 0XF0:		
			{
				   //主控板初始化
				USARTSendByte(0x55,USART1);
				USARTSendByte(0xAA,USART1);
				USARTSendByte(0x00,USART1);
				USARTSendByte(0xF0,USART1);
				USARTSendByte(0xEF,USART1);
				
				/* if(SendCount++>=4) */
						ReCmd = 0;
			}
			break;
			case 0XF1:		
			{
				//主控板初始化
				USARTSendByte(0x55,USART1);
				USARTSendByte(0xAA,USART1);
				USARTSendByte(0x00,USART1);
				USARTSendByte(0xF1,USART1);
				USARTSendByte(0xF0,USART1);
				 ReCmd = 0;

			}
			break;
			
			case 0XFD:		
			{
				//主控板初始化
				USARTSendByte(0x55,USART1);
				USARTSendByte(0xAA,USART1);
				USARTSendByte(0x00,USART1);
				USARTSendByte(0xFD,USART1);
				USARTSendByte(0xFC,USART1);
				ReCmd = 0;

			}
			break;

			case 0XB1:
			{
	//		  Usart1_SendPowerCmd();//上传长安关机命令
			}
			break;
				
			case 0XB3:
			{
	//			Usart1_SendScramCmd();	//上传急停命令
			}
			break;
				
			case 0X0D:
				
			{
				
			}
			break;
				
			case 0X01:
			{
				
			}
			break;
				
			case 0X02:
			{
				
			}
			break;
				
			case 0XA2:
			{
				
			}
			break;
				
			case 0X03:	//上传超声波数据
			{
	//			Usrat1_SendKs136Data();
			}
			break;
							
			case 0X05:
			{
				
			}
			break;
										
			case 0X06:
			{
				
			}
			break;
													
			case 0XA6:
			{
				
			}
			break;
			
			case 0XB6:
			{
				
			}
			break;
								
			case 0XC6:
			{
				
			}
			break;
								
			case 0X0E:
			{
//				Usart1_SendBms();	//电池
			}
			break;
								
			case 0XFA:
			{
	//			Send_Ir();	//上传红外数据
			}
			break;
			case 0XBA:
			{
				#if 0
				//水位信息
				USARTSendByte(0x55,USART1);
				USARTSendByte(0xAA,USART1);
				USARTSendByte(0x01,USART1);
				USARTSendByte(0xBA,USART1);	   
				if(WATER_S)	//水位过低
				{
					USARTSendByte(0x01,USART1);
					USARTSendByte(0xBB,USART1);
				}
				else
				{
					USARTSendByte(0x00,USART1);
					USARTSendByte(0xBA,USART1);
				}
				#endif

			}
			break;
			default:
				break;
		}
	}


}

/*************************************************

Function:	IsCheckSum_OK(uint8_t *Rec,uint8_t Count)

Description:  计算校验是否正确

*************************************************/
uint8_t IsCheckSum_OK(uint8_t *Rec,uint8_t Count)
{
   uint8_t i=0;
   uint8_t sum=0;
	for(i=0;i<Count;++i)
	{
	  sum +=Rec[i];

	}
    if(sum==Rec[Count])//低8位
	{
	  return 1;
	}
	return 0;

}

/*************************************************

Function:	IsCheckSum_OK(uint8_t *Rec,uint8_t Count)

Description:  设置状态及回复

*************************************************/
void robotvStatus(RobotDef* robot)
{
	if(!Uart1_RecOK)return;
	if(!IsCheckSum_OK(Uart1_RxBuffer,Datalen+4))return;
	switch(Uart1_RecOK)
	{
		case 1:
		{ /* 电池数据 */
			robot->BmsSta.chargeSta=Uart1_RxBuffer[7];
			robot->BmsSta.soc = Uart1_RxBuffer[6];
			robot->BmsSta.temp = Uart1_RxBuffer[5];
			robot->BmsSta.vol = Uart1_RxBuffer[4];
		}break;
		  case 2:
		{ /* 运动状态 */
			robot->RunSta.dir=Uart1_RxBuffer[4];//取出数据
			USARTSendByte(FLAGA,USART1);
			USARTSendByte(FLAGB,USART1);
			USARTSendByte(Uart1_RxBuffer[2],USART1);
			USARTSendByte(ROBTSTA,USART1);//运动命令
			USARTSendByte(Uart1_RxBuffer[4],USART1);
			USARTSendByte(Uart1_RxBuffer[5],USART1);
		}break;
		case 3:
		{
		//		USARTSendByte(FLAGA,USART1);
		//	    USARTSendByte(FLAGB,USART1);
		//	    USARTSendByte(Uart1_RxBuffer[2],USART1);
		//	    USARTSendByte(PowerSta,USART1);
		//	    USARTSendByte(Uart1_RxBuffer[4],USART1);
		//		USARTSendByte(Uart1_RxBuffer[5],USART1);
			_POWER_ON; //关机
		}break;
      case 4:
    {
	  if(Uart1_RxBuffer[4]==0x01) 
	  {
		 // LED_FW;//打开氛围灯
		RGB_ATMP_LEFT_POWER;//开启左灯电源
		RGB_ATMP_RIGHT_POWER;//开启左灯电源		  
	  }
	  else
	  { 
		_RGB_ATMP_LEFT_POWER;//开启左灯电源
		_RGB_ATMP_RIGHT_POWER;//开启左灯电源	
		 // _LED_FW;//关闭氛围灯
      }
	  USARTSendByte(FLAGA,USART1);
	  USARTSendByte(FLAGB,USART1);
	  USARTSendByte(Uart1_RxBuffer[2],USART1);
	  USARTSendByte(LEDFWCTRL,USART1);
	  USARTSendByte(Uart1_RxBuffer[4],USART1);
	  USARTSendByte(Uart1_RxBuffer[5],USART1);
    } break;
     case 5:
    {
	  RFID_readEPC();//读RFID epc
//	   USARTSendByte(0xcc,USART1);
	  Uart3_RecNoOK = 1;
    } break;
	 case 6:
	 {
		Rfid.epc.shelf_ID=((u16)Uart1_RxBuffer[4]<<8)+(u16)Uart1_RxBuffer[5];
		Rfid.epc.factor_ID=((u16)Uart1_RxBuffer[6]<<8)+(u16)Uart1_RxBuffer[7];
		Rfid.epc.other=((u16)Uart1_RxBuffer[8]<<8)+(u16)Uart1_RxBuffer[9];
		RFID_writeEPC(&Rfid);//写RFID epc
	 } break;
	 case 7:
	 {
		 
	 } break;
	 case 8:
	 {
		 
	 } break;
	 case 9://顶升电机移动
	 {
		 USARTSendByte(FLAGA,USART1);
		 USARTSendByte(FLAGB,USART1);
		 USARTSendByte(0x01,USART1);
		 USARTSendByte(MOTOR_MOVE,USART1);
		 USARTSendByte(0x01,USART1);
		 USARTSendByte(0x8A,USART1);
		 delay_ms(500);
		 switch(Uart1_RxBuffer[4])
		 {
			 case 0:
			 {
				 SendCmdToMotor(MotCmd_ctr, 0x0000, 0x00);
			 }break;
			 case 1:
			 {
				 SendCmdToMotor(MotCmd_ctr, 0x0001, 0x00);
				 delay_ms(20);
				 motorData.pos_data = (Uart1_RxBuffer[5] << 24) + (Uart1_RxBuffer[6] << 16) + (Uart1_RxBuffer[7] << 8) + Uart1_RxBuffer[8];
				 SendCmdToMotor(MotCmd_dis, 0x0002, motorData.pos_data);
				 delay_ms(20);
				 SendCmdToMotor(MotCmd_str, 0x0001, 0x00);
				 
			 }break;
			 case 2:
			 {
				 motorData.pos_data = (Uart1_RxBuffer[5] << 24) + (Uart1_RxBuffer[6] << 16) + (Uart1_RxBuffer[7] << 8) + Uart1_RxBuffer[8];
				 SendCmdToMotor(MotCmd_dis, 0x0002, motorData.pos_data);
				 delay_ms(20);
				 SendCmdToMotor(MotCmd_str, 0x0001, 0x00);
			 }break;
			 default:break;
		 }
	}
	 break;
	case 10:
	{
		SendCmdToMotor(MotCmd_locpos, 0x0002, 0x00);
	}
	break;
	case 11:
	{
		//CHARGE_ON;
		Uart1_RxBuffer[4] ? (CHARGE_ON) : (_CHARGE_ON);
		USARTSendByte(FLAGA,USART1);
		USARTSendByte(FLAGB,USART1);
		USARTSendByte(Uart1_RxBuffer[2],USART1);
		USARTSendByte(AUTO_CHARGE,USART1);
		USARTSendByte(Uart1_RxBuffer[4],USART1);
		USARTSendByte(Uart1_RxBuffer[5],USART1);
	}
	break;
	 default:break;
  }
  Uart1_RecOK = 0;
}
