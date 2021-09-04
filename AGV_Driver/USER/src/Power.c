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

Description: ��ͣ��������

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
	
	if(!PENGZHUANG_F||!PENGZHUANG_S)//||//�����ײ�˿�
		
	//	!DINGSHENG1_S||DINGSHENG2_S||//��ⶥ�������� ���д�����2 ��ʼ��Ϊ�͵�ƽ ������1 ��ʼ��Ϊ��
	   // !ROTATE_ZERO1||!ROTATE_ZERO2)//��ת������
	{
		delay_us(10000);//�ӳ�10ms����
//		if(!DINGSHENG1_S||DINGSHENG2_S)//����������
//		{
//			DINGSHENG_EN;//�رն���
//			if(!DINGSHENG1_S) 
//			{
//				#if DEBUG_STATE 
//				USARTSendByte(0xBB,USART1);USARTSendByte(0xCC,USART1);
//				#endif
//				xEventGroupSetBits(EventGroup_Handler,SENSOR1_JACK_BIT0);//���ʹ˶���������1�¼�	
//			}
//			else if(DINGSHENG2_S) 
//			{
//				#if DEBUG_STATE 
//				USARTSendByte(0xBB,USART1);USARTSendByte(0xDD,USART1);
//				#endif
//				xEventGroupSetBits(EventGroup_Handler,SENSOR2_JACK_BIT1);//���ʹ˶���������2�¼�	
//			}
//		}else 
//		{
//			if(!emerg_sta)//Ŀǰ״̬���Ǽ�ͣ����ָ�
//			_DINGSHENG_EN;//��������
//		}
//		if(!ROTATE_ZERO1||!ROTATE_ZERO2)//��ת���������
//		{
//			#if DEBUG_STATE 
//			USARTSendByte(0xBB,USART1);USARTSendByte(0xFF,USART1);
//			#endif
//			xEventGroupSetBits(EventGroup_Handler,SENSOR_ROTATE_BIT2);//������ת��������������ź�
//		}
		if(!PENGZHUANG_F)//ǰ��ײ
		{
		  new_sta_col=1;
		}
		if(!PENGZHUANG_S)//����ײ
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
	   return;//״̬�������˳�
	}		
	//if(old_sta_scram==new_sta_scram) return;//״̬�������˳�
	old_sta_scram=new_sta_scram;
	old_sta_col=new_sta_col;
	old_sta_col2=new_sta_col2;
	if(!JITING_S||(!PENGZHUANG_F||!PENGZHUANG_S))//��ײ����
	//if(!JITING_S)//��ײ����
	{
		//Robot.RunSta.dir=FSTOP;
		DIANJI_EN;//��ͣ
        DINGSHENG_EN;//����
	}
	//else if(JITING_S)//����ײ����
	else if(JITING_S&&(PENGZHUANG_F&&PENGZHUANG_S))//����ײ����
	{		
		//Robot.RunSta.dir=ESTOP;
		_DIANJI_EN;   //��ͣ
		//if(DINGSHENG1_S&&!DINGSHENG2_S)//���û�ж���������ָ�
		_DINGSHENG_EN;//����
	}
	if(!JITING_S && !emerg_sta)//��ͣЭ����Ϣ���ͣ�������
	{
		USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
		USARTSendByte(0x01,USART1);USARTSendByte(0xB3,USART1);
		USARTSendByte(0x01,USART1);USARTSendByte(0xC5,USART1);
		Scram_Flag = 1;
		emerg_sta = 1;
	}
	else if(JITING_S && emerg_sta)//��ͣЭ����Ϣ���ͣ�δ������
	{
		USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
		USARTSendByte(0x01,USART1);USARTSendByte(0xB3,USART1);
		USARTSendByte(0x00,USART1);USARTSendByte(0xC4,USART1);
		Scram_Flag = 0;
		emerg_sta = 0;
	}
	
	
	delay_ms(20);//�˳�����20ms
	crash_status=(!PENGZHUANG_F)|((!PENGZHUANG_S)<<1);//��ȡ��ײ״̬
	switch(crash_status)
	{
		case 0x00://����ײ
		{
			USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xB5,USART1);
			USARTSendByte(0x00,USART1);USARTSendByte(0xC5,USART1);
		}break;
		case 0x01://ǰ��ײ
		{
			USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xB5,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xC6,USART1);
		}break;
		case 0x02://����ײ
		{
			USARTSendByte(0x66,USART1);USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);USARTSendByte(0xB5,USART1);
			USARTSendByte(0x02,USART1);USARTSendByte(0xC7,USART1);
		}break;
		case 0x03://ǰ����ײ
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

Description:  ���ػ���������

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
		KeyDown_status = 4;//��ʼ
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
		if(Timer[POWERKEYTIMER].Flag)//2800ms �ж�Ϊ���� �涨��Լ3�룬Ԥ��200ms���ϲ㴦���´�����
		{
			Reset_Timer(POWERKEYTIMER);//��λ��Դ������ʱ��
			KeyDown_status=3;//�������
			#if DEBUG_STATE 
			_POWER_ON;//���Խ׶ο���
			#endif
			if(PowerKey_status!=2)
			{
			    PowerKey_status =2;	
				//���͹ػ�����
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
		return;//��ʼ�����������󴥷��ػ�ָ��
	}
	else if(POWER_S&&KeyDown_status==4)
	{
		KeyDown_status=0;
	}
	else if(POWER_S&&KeyDown_status)//�������������ж϶̰�
	{
		if(KeyDown_status==1)
		{
			Reset_Timer(POWERKEYTIMER);//��λ��Դ������ʱ��
			KeyDown_status = 0;
			return; //500ms���� ����Ӧ
		}
		else if(KeyDown_status==2)//���ػ�״̬��
		{
			Reset_Timer(POWERKEYTIMER);//��λ��Դ������ʱ��
			KeyDown_status = 0;
			POWER_ON;   //�̰�����
			PowerKey_status = 1;
			USARTSendByte(0x66,USART1);
			USARTSendByte(0xAA,USART1);
			USARTSendByte(0x01,USART1);
			USARTSendByte(0xB1,USART1);
			USARTSendByte(0x00,USART1);
			USARTSendByte(0xC2,USART1);
		}
		if(KeyDown_status==3)//����������
		{
			KeyDown_status=0;
		}
	}
	return ;
	
	
	
}
#if 0
uint8_t  Uart6_RxBuffer[255]; 
uint16_t Datalen2 = 0;//�������ݳ���
uint8_t Uart6_RecOK = 0;
uint8_t ReCmd2 =0;
extern uint8_t  ReCmd;
/*************************************************

Function:	Uart6_ReceiveIQR()

Description:  ����6���մ���

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
		    sum+=Uart6_RxBuffer[i];//�Ͱ�λ
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

void Usart1_SendPowerCmd(void)	//�ϱ������ػ�����
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
//	send_flag = 0;	//������ɱ�־λ��0
}


void Usart1_SendScramCmd(void)	//�ϱ���ͣ����
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