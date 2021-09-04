#include "ws2812b.h"
#include "delay.h"
#include "BMS.h"
#include "Timer.h"
#include "Device.h"
#include "FreeRTOS.h"
#include "task.h"

#define BREATHE_CYCLE       60//��������
static u8 k=0,m=0;
static u32 colo_lr[RGB_LEFT_NUMBER]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//���ҵ���������

static u32 colo_cir[RGB_CIRCLE_NUMBER]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//Բ����������	

const u32 breathe[BREATHE_CYCLE]={0x7F,0x8C,0x9A,0xA6,0xB3,0xBF,0xCA,0xD4,0xDE,0xE6,
								  0xED,0xF3,0xF8,0xFC,0xFE,0xFE,0xFE,0xFC,0xF8,0xF3,
								  0xED,0xE6,0xDE,0xD4,0xCA,0xBF,0xB3,0xA6,0x9A,0x8C,
								  0x7F,0x72,0x64,0x58,0x4B,0x3F,0x34,0x2A,0x20,0x18,
								  0x11,0x0B,0x06,0x02,0x00,0x00,0x00,0x02,0x06,0x0B,
								  0x11,0x18,0x20,0x2A,0x34,0x3F,0x4B,0x58,0x64,0x72};//���յƺ�������
void RGB_LED_Init(void)
{
 	RGB_LEFT1_POWER;//�������1��Դ
	RGB_LEFT2_POWER;//�������2��Դ
	RGB_RIGHT1_POWER;//������Ƶ�Դ
	RGB_RIGHT2_POWER;//������Ƶ�Դ
    RGB_CIRCLE_LEFT_POWER;//������Ƶ�Դ
    RGB_CIRCLE_RIGHT_POWER;	//������Ƶ�Դ
	RGB_ATMP_LEFT_POWER;//������Ƶ�Դ
	RGB_ATMP_RIGHT_POWER;//������Ƶ�Դ
	FS1_POWER;//��������1
	FS2_POWER;//��������2
}
/********************************************************/
//
/********************************************************/
void RGB_LED_Write0(RGB_Typed light_type)
{
	if(light_type==RGB_LEFT_TYPE)
	{
		RGB_LEFT1_HIGH;RGB_LEFT2_HIGH;
	}
	else if(light_type==RGB_RIGHT_TYPE)
	{
		RGB_RIGHT1_HIGH;RGB_RIGHT2_HIGH;
	}
	else if(light_type==RGB_CIRCLE_TYPE)
	{
		RGB_CIRCLE_LEFT_HIGH;RGB_CIRCLE_RIGHT_HIGH;
	}
	else if(light_type==RGB_ATMP_TYPE)
	{
		RGB_ATMP_LEFT_HIGH;RGB_ATMP_RIGHT_HIGH;
	}
	//�ӳٷ�Χ��200ns~400ns  ʵ����ʱ��Լ200ns (ʾ�����۲�)	
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	if(light_type==RGB_LEFT_TYPE)
	{
		RGB_LEFT1_LOW;RGB_LEFT2_LOW;
	}
	else if(light_type==RGB_RIGHT_TYPE)
	{
		RGB_RIGHT1_LOW;RGB_RIGHT2_LOW;
	}
	else if(light_type==RGB_CIRCLE_TYPE)
	{
		RGB_CIRCLE_LEFT_LOW;RGB_CIRCLE_RIGHT_LOW;
	}
	else if(light_type==RGB_ATMP_TYPE)
	{
		RGB_ATMP_LEFT_LOW;RGB_ATMP_RIGHT_LOW;
	}	
  //�ӳٷ�Χ��650ns~950ns   ʵ����ʱ��Լ500ns (ʾ�����۲�)
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
}
/********************************************************/
//
/********************************************************/
void RGB_LED_Write1(RGB_Typed light_type)
{
	if(light_type==RGB_LEFT_TYPE)
	{
		RGB_LEFT1_HIGH;RGB_LEFT2_HIGH;
	}
	else if(light_type==RGB_RIGHT_TYPE)
	{
		RGB_RIGHT1_HIGH;RGB_RIGHT2_HIGH;
	}
	else if(light_type==RGB_CIRCLE_TYPE)
	{
		RGB_CIRCLE_LEFT_HIGH;RGB_CIRCLE_RIGHT_HIGH;
	}
	else if(light_type==RGB_ATMP_TYPE)
	{
		RGB_ATMP_LEFT_HIGH;RGB_ATMP_RIGHT_HIGH;
	}
	//�ӳٷ�Χ��550ns~850ns    ʵ����ʱ��Լ500ns(ʾ�����۲�)
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();
		if(light_type==RGB_LEFT_TYPE)
	{
		RGB_LEFT1_LOW;RGB_LEFT2_LOW;
	}
	else if(light_type==RGB_RIGHT_TYPE)
	{
		RGB_RIGHT1_LOW;RGB_RIGHT2_LOW;
	}
	else if(light_type==RGB_CIRCLE_TYPE)
	{
		RGB_CIRCLE_LEFT_LOW;RGB_CIRCLE_RIGHT_LOW;
	}
	else if(light_type==RGB_ATMP_TYPE)
	{
		RGB_ATMP_LEFT_LOW;RGB_ATMP_RIGHT_LOW;
	}
    //�ӳٷ�Χ��450ns~750ns   ʵ����ʱ��Լ350ns(ʾ�����۲�)
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}
void RGB_LED_Reset(RGB_Typed light_type)
{
	if(light_type==RGB_LEFT_TYPE)
	{
		RGB_LEFT1_LOW;RGB_LEFT2_LOW;
	}
	else if(light_type==RGB_RIGHT_TYPE)
	{
		RGB_RIGHT1_LOW;RGB_RIGHT2_LOW;
	}
	else if(light_type==RGB_CIRCLE_TYPE)
	{
		RGB_CIRCLE_LEFT_LOW;RGB_CIRCLE_RIGHT_LOW;
	}
	else if(light_type==RGB_ATMP_TYPE)
	{
		RGB_ATMP_LEFT_LOW;RGB_ATMP_RIGHT_LOW;
	}
	delay_us(80);
}

void RGB_LED_Write_Byte(u8 byte,RGB_Typed light_type)
{
	
	uint8_t i=0;
	while(i<8)
	{
	    if(byte&0x80)
		{
			RGB_LED_Write1(light_type);
	    }
		else
		{
			RGB_LED_Write0(light_type);
		}
		byte <<= 1;
		i++;
	}
		
}
void RGB_LED_Write_24Bits(RGB_Typed light_type,u8 green,u8 red,u8 blue)
{
	RGB_LED_Write_Byte(green,light_type);
	RGB_LED_Write_Byte(red,light_type);
	RGB_LED_Write_Byte(blue,light_type);
}
void RGB_LED_Light(RGB_Typed light_type,u8 num,u32 colour)
{
	uint8_t i=0;
	while(i<num)
	{
		RGB_LED_Write_24Bits(light_type,(colour>>16)&0xFF,(colour>>8)&0xFF,colour&0xFF);
		i++;
	}
	
}
void RGB_LED_BLN(RGB_Typed light_type,u8 num)
{
	static uint8_t atm=0;
	RGB_LED_Light(light_type,num,breathe[atm++]);
	if(atm>=BREATHE_CYCLE) 
    atm=0;
}
void RGB_init_status(RGB_Typed light_type,u8 hrl_dir,u32 *arr,u8 num,u32 colour)
{
	uint8_t i=0;
	for(i=0;i<num;i++) //��ʼ�����������  
	{
		//˳ʱ���ʼ��0~10�Ÿ�������	��ʱ�������ʼ��10~20�Ÿ�������	
		if(i<k)
		{
			if(hrl_dir)//˳ʱ������
				arr[i]=colour;
			else 
			    arr[num-1-i]=colour;
		}
		else 
		{
			if(hrl_dir)//˳ʱ������
				arr[i]=0;
			else 
			    arr[num-1-i]=0;
		}  		
	}
}
/*===========================================================================
 *�������� : RGB_LED_HRL
 *�������� : RGB����Ƴ���
 *����˵�� : robot �������˶��ṹ��������ɻ�ȡ�˶�״̬
 *����ֵ   : ��
============================================================================*/
void RGB_LED_HRL(RGB_Typed light_type,u8 hrl_dir,u32 *arr,u8 num,u32 colour)
{
	uint8_t i=0;
	while(i<num)
	{
		RGB_LED_Write_24Bits(light_type,(arr[i]>>16)&0xFF,(arr[i]>>8)&0xFF,arr[i]&0xFF);
		i++;
	}	
	if(hrl_dir)
	{
		arr[k]=colour;
		arr[m]=0;
	}
	else
	{
		arr[num-1-k]=colour;
		arr[num-1-m]=0;
	}
	k++;m++;		
	if(k>=num) k=0;
	if(m>=num) m=0;		
}

/*===========================================================================
 *�������� : LED_Battery_Liht
 *�������� : ��ص���ָʾ����ʾ����
 *����˵�� : robot �������˶��ṹ��������ɻ�ȡ��ص���
 *����ֵ   : ��
 *��ע     : ��
============================================================================*/
void LED_Battery_Liht(RobotDef* robot)
{ 
	u8 soc = 0;
	soc=robot->BmsSta.soc;
	soc=36;
	if(soc>80)
	{
		LED1;LED2;LED3;LED4;_LED_ALARM;
	}
	else if(soc>65)
	{
		LED1;LED2;LED3;_LED4;_LED_ALARM;
	}
	else if(soc>50)
	{
	LED1;LED2;_LED3;_LED4;_LED_ALARM;
	}
	 else if(soc>35)
	 {
		LED1;_LED2;_LED3;_LED4;_LED_ALARM;
	 }
	 else
	 { 
		_LED1;_LED2;_LED3;_LED4;
		if(Timer[SOCLEDTIMER].Flag)
		{
			Set_Timer(SOCLEDTIMER,300);//1.5����˸һ��
			if(LED_ALARMS)//��˸Ч��
			_LED_ALARM;
			else
			LED_ALARM;	
		}	
    }
}

/*===========================================================================
 *�������� : RGB_Run_Status_Light
 *�������� : �˶�״̬RGBָʾ����
 *����˵�� : robot �������˶��ṹ��������ɻ�ȡ�˶�״̬
 *����ֵ   : ��
 *��ע     : ��
============================================================================*/
void RGB_Run_Status_Light(RobotDef* robot)
{
	static u8 s_num=0;
	static u8 flag_up=0;
	static u8 s_flag_old=0;
	u8 s_flag_new=0;
	s_flag_new=robot->RunSta.dir;
	if(s_flag_new!=s_flag_old) 
		flag_up=1;
	else flag_up=0;
	s_flag_old=s_flag_new;
	//robot->RunSta.dir=DIDLE;
	switch (robot->RunSta.dir)
	{
		case INITS: //��ʼ��״̬
		{
			if(flag_up==1)
			{
				DIS_INT;	
//				taskENTER_CRITICAL(); //�����ٽ���
//                //������Ը�λ�źţ�������Э�飬ˢ�µ���������ִ�еĳ����൱���ӳ٣�����50us�Ժ��൱��һ����λ�źţ�				
				RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);  //�����ҵ� ��Լ��ʱ360us
				RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);    //������� ��Լ��ʱ360us
//				EN_INT;
//				delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ (�����һ����佫��һ�ιر����жϣ��˴��൱���к�)
//				DIS_INT;
				RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//������� ��Լ��ʱ660us
//				EN_INT;
//				delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ (�����һ����佫��һ�ιر����жϣ��˴��൱���к�)
//				DIS_INT;
				RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);    //�������յ� ���յƽϳ�����Լ��ʱ1.4ms 
				EN_INT;	
//				taskEXIT_CRITICAL(); //�˳��ٽ���
			}	
			
		}break;
		case PRERUN://Ԥ������(���������ʼ����)
		{
			
		}break;
	    case BACK:  //����
		{
			
			if(Timer[ROUNDALMTIMER].Flag)
			{
				Set_Timer(ROUNDALMTIMER,11);//60ms
				if(flag_up==1)
				{
					DIS_INT;
//					taskENTER_CRITICAL(); //�����ٽ���
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);//�ҵƳ���	
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);  //��Ƴ���
//					EN_INT;
//					delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//					DIS_INT;
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);  //���յƳ���
                    EN_INT;	
//					taskEXIT_CRITICAL(); //�˳��ٽ���
                    k=6,m=0;//��ʼ����������					
					RGB_init_status(RGB_CIRCLE_TYPE,0,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);//Բ�����ݳ�ʼ��,˳ʱ�뷽��,Բ������						
				}
				//DIS_INT;
				RGB_LED_HRL(RGB_CIRCLE_TYPE,0,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);//	Բ��˳ʱ������
                //EN_INT;				
			}
		}break;
		case FORWARD://ǰ��
		{
			if(Timer[TURNFRONTTIMER].Flag)
			{
				Set_Timer(TURNFRONTTIMER,12);//60ms
				if(flag_up==1)
				{	
					DIS_INT;
//					taskENTER_CRITICAL(); //�����ٽ���
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);//�ҵƳ���
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);  //��Ƴ���
//					EN_INT;
//					delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//					DIS_INT;					
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);  //���յƳ���
					EN_INT;
//					taskEXIT_CRITICAL(); //�˳��ٽ���
					k=6,m=0;//��ʼ����������	
					RGB_init_status(RGB_CIRCLE_TYPE,1,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);	//Բ�����ݳ�ʼ������ʱ�뷽��,Բ������					
				}
				//DIS_INT;
				RGB_LED_HRL(RGB_CIRCLE_TYPE,1,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);//	Բ����ʱ������
                //EN_INT;				
			}
		}break;
		case TOLEFT: //��ת
		{
			if(Timer[TOLEFTTIMER].Flag)
			{
				Set_Timer(TOLEFTTIMER,12);//60ms
				if(flag_up==1)
				{		
					DIS_INT;	
//taskENTER_CRITICAL(); //�����ٽ���					
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);//�ҵƳ��� 360us
					RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE); //Բ�Ƴ��� 660us
				
//				EN_INT;
//				delay_us(10);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//				DIS_INT;		
				
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);//���յƳ���
					EN_INT;	
//					taskEXIT_CRITICAL(); //�˳��ٽ���
                    k=3,m=0;//��ʼ����������		                   					
					RGB_init_status(RGB_LEFT_TYPE,1,colo_lr,RGB_LEFT_NUMBER,RGB_BLUE);				
				}
				//DIS_INT;
                RGB_LED_HRL(RGB_LEFT_TYPE ,1,colo_lr,RGB_LEFT_NUMBER,RGB_BLUE);//�������,��ʱ������		
               // EN_INT;					
			}
		} break;
		case TORIGHT://��ת
		{
			if(Timer[TORIGHTTIMER].Flag)
			{
				Set_Timer(TORIGHTTIMER,12);//60ms����Ƶ��
				if(flag_up==1)
				{		
					DIS_INT;
//					taskENTER_CRITICAL(); //�����ٽ���
					RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//Բ�Ƴ���
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);//��Ƴ���
//					EN_INT;
//					delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//					DIS_INT;			
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);//�ҵƳ�
                    EN_INT;	
//					taskEXIT_CRITICAL(); //�˳��ٽ���
					k=3,m=0;//��ʼ����������						
                    RGB_init_status(RGB_RIGHT_TYPE,1,colo_lr,RGB_LEFT_NUMBER,RGB_BLUE);					
				}
				//DIS_INT;
				RGB_LED_HRL(RGB_RIGHT_TYPE ,1,colo_lr,RGB_RIGHT_NUMBER,RGB_BLUE);//�������,��ʱ������
               // EN_INT;					
			}
		}break;
		case ESTOP:
		{
			if(flag_up==1)
			{
				DIS_INT;	
//				taskENTER_CRITICAL(); //�����ٽ���
                //������Ը�λ�źţ�������Э�飬ˢ�µ���������ִ�еĳ����൱���ӳ٣�����50us�Ժ��൱��һ����λ�źţ�				
				RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);  //�����ҵ� ��Լ��ʱ360us
				RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);    //�������	 ��Լ��ʱ360us
//				EN_INT;
//				delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//				DIS_INT;
				RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//������� ��Լ��ʱ660us
//				EN_INT;
//				delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//				DIS_INT;
				RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);    //�������յ� ���յƽϳ�����Լ��ʱ1.4ms 
//				 taskEXIT_CRITICAL(); //�˳��ٽ���
				EN_INT;	
			}		
		}break;
		case FSTOP://ֹͣ
		{
			if(flag_up==1)
			{
				DIS_INT;	
//				taskENTER_CRITICAL(); //�����ٽ���
                //������Ը�λ�źţ�������Э�飬ˢ�µ���������ִ�еĳ����൱���ӳ٣�����50us�Ժ��൱��һ����λ�źţ�				
				RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_RED);  //�����ҵ� ��Լ��ʱ360us
				RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_RED);    //�������	 ��Լ��ʱ360us
//				EN_INT;
//				delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//				DIS_INT;
				RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_RED);//������� ��Լ��ʱ660us
//				EN_INT;
//				delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//				DIS_INT;
				RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_RED);    //�������յ� ���յƽϳ�����Լ��ʱ1.4ms 
//				 taskEXIT_CRITICAL(); //�˳��ٽ���
				EN_INT;	
			}			
		}break;
		case DIDLE://����
		{
			if(Timer[TURNLEDTIMER].Flag)
			{
				s_num++;
				Set_Timer(TURNLEDTIMER, 17);//100ms�л�һ��
				if(flag_up==1)
				{		
					DIS_INT;
//					taskENTER_CRITICAL(); //�����ٽ���
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);//�����ҵ�
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);//��Ƴ���
//					EN_INT;
//					delay_us(400);//�ۼ�ʱ�䳬��ϵͳ���ģ��ɱ������ȼ�������ռ
//					DIS_INT;
					RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//Բ�Ƴ���
                    RGB_LED_BLN(RGB_ATMP_TYPE,RGB_ATMP_NUMBER);//���յƺ���Ч�� ��Լ��ʱ1.4ms	
//					 taskEXIT_CRITICAL(); //�˳��ٽ���
                    EN_INT;	
                    s_num=0;					
				}
				if(s_num>25)
				{
					s_num=26;
					DIS_INT;
					 //taskENTER_CRITICAL(); //�����ٽ���
				    RGB_LED_BLN(RGB_ATMP_TYPE,RGB_ATMP_NUMBER);//���յƺ���Ч�� ��Լ��ʱ830us 
					// taskEXIT_CRITICAL(); //�˳��ٽ���
                    EN_INT;
				}
			}  
		}break;
		default:break;
		
	}
	
}

void RGB_Ctrl(RobotDef* robot)
{
	RGB_Run_Status_Light(robot);
	LED_Battery_Liht(robot);
}

