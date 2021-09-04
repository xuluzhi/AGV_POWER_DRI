#include "ws2812b.h"
#include "delay.h"
#include "BMS.h"
#include "Timer.h"
#include "Device.h"
#include "FreeRTOS.h"
#include "task.h"

#define BREATHE_CYCLE       60//呼吸周期
static u8 k=0,m=0;
static u32 colo_lr[RGB_LEFT_NUMBER]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//左右灯跑马数据

static u32 colo_cir[RGB_CIRCLE_NUMBER]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//圆灯跑马数据	

const u32 breathe[BREATHE_CYCLE]={0x7F,0x8C,0x9A,0xA6,0xB3,0xBF,0xCA,0xD4,0xDE,0xE6,
								  0xED,0xF3,0xF8,0xFC,0xFE,0xFE,0xFE,0xFC,0xF8,0xF3,
								  0xED,0xE6,0xDE,0xD4,0xCA,0xBF,0xB3,0xA6,0x9A,0x8C,
								  0x7F,0x72,0x64,0x58,0x4B,0x3F,0x34,0x2A,0x20,0x18,
								  0x11,0x0B,0x06,0x02,0x00,0x00,0x00,0x02,0x06,0x0B,
								  0x11,0x18,0x20,0x2A,0x34,0x3F,0x4B,0x58,0x64,0x72};//气氛灯呼吸数据
void RGB_LED_Init(void)
{
 	RGB_LEFT1_POWER;//开启左灯1电源
	RGB_LEFT2_POWER;//开启左灯2电源
	RGB_RIGHT1_POWER;//开启左灯电源
	RGB_RIGHT2_POWER;//开启左灯电源
    RGB_CIRCLE_LEFT_POWER;//开启左灯电源
    RGB_CIRCLE_RIGHT_POWER;	//开启左灯电源
	RGB_ATMP_LEFT_POWER;//开启左灯电源
	RGB_ATMP_RIGHT_POWER;//开启左灯电源
	FS1_POWER;//开启风扇1
	FS2_POWER;//开启风扇2
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
	//延迟范围：200ns~400ns  实际延时大约200ns (示波器观察)	
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
  //延迟范围：650ns~950ns   实际延时大约500ns (示波器观察)
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
	//延迟范围：550ns~850ns    实际延时大约500ns(示波器观察)
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
    //延迟范围：450ns~750ns   实际延时大约350ns(示波器观察)
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
	for(i=0;i<num;i++) //初始化跑马灯数据  
	{
		//顺时针初始化0~10号个灯亮起	逆时针跑马初始化10~20号个灯亮起	
		if(i<k)
		{
			if(hrl_dir)//顺时针跑马
				arr[i]=colour;
			else 
			    arr[num-1-i]=colour;
		}
		else 
		{
			if(hrl_dir)//顺时针跑马
				arr[i]=0;
			else 
			    arr[num-1-i]=0;
		}  		
	}
}
/*===========================================================================
 *函数名称 : RGB_LED_HRL
 *功能描述 : RGB跑马灯程序
 *参数说明 : robot 机器人运动结构体变量，可获取运动状态
 *返回值   : 无
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
 *函数名称 : LED_Battery_Liht
 *功能描述 : 电池电量指示灯显示函数
 *参数说明 : robot 机器人运动结构体变量，可获取电池电量
 *返回值   : 无
 *备注     : 无
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
			Set_Timer(SOCLEDTIMER,300);//1.5秒闪烁一次
			if(LED_ALARMS)//闪烁效果
			_LED_ALARM;
			else
			LED_ALARM;	
		}	
    }
}

/*===========================================================================
 *函数名称 : RGB_Run_Status_Light
 *功能描述 : 运动状态RGB指示函数
 *参数说明 : robot 机器人运动结构体变量，可获取运动状态
 *返回值   : 无
 *备注     : 无
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
		case INITS: //初始化状态
		{
			if(flag_up==1)
			{
				DIS_INT;	
//				taskENTER_CRITICAL(); //进入临界区
//                //这里忽略复位信号（单总线协议，刷新灯珠后，则后面执行的程序相当于延迟，超过50us以后相当于一个复位信号）				
				RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);  //开启右灯 大约耗时360us
				RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);    //开启左灯 大约耗时360us
//				EN_INT;
//				delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占 (因此下一条语句将再一次关闭总中断，此处相当打招呼)
//				DIS_INT;
				RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//开启左灯 大约耗时660us
//				EN_INT;
//				delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占 (因此下一条语句将再一次关闭总中断，此处相当打招呼)
//				DIS_INT;
				RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);    //开启气氛灯 气氛灯较长，大约耗时1.4ms 
				EN_INT;	
//				taskEXIT_CRITICAL(); //退出临界区
			}	
			
		}break;
		case PRERUN://预备启动(顶升电机开始工作)
		{
			
		}break;
	    case BACK:  //后退
		{
			
			if(Timer[ROUNDALMTIMER].Flag)
			{
				Set_Timer(ROUNDALMTIMER,11);//60ms
				if(flag_up==1)
				{
					DIS_INT;
//					taskENTER_CRITICAL(); //进入临界区
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);//右灯常亮	
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);  //左灯常亮
//					EN_INT;
//					delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//					DIS_INT;
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);  //气氛灯常亮
                    EN_INT;	
//					taskEXIT_CRITICAL(); //退出临界区
                    k=6,m=0;//初始化跑马数据					
					RGB_init_status(RGB_CIRCLE_TYPE,0,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);//圆灯数据初始化,顺时针方向,圆灯数量						
				}
				//DIS_INT;
				RGB_LED_HRL(RGB_CIRCLE_TYPE,0,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);//	圆灯顺时针跑马
                //EN_INT;				
			}
		}break;
		case FORWARD://前进
		{
			if(Timer[TURNFRONTTIMER].Flag)
			{
				Set_Timer(TURNFRONTTIMER,12);//60ms
				if(flag_up==1)
				{	
					DIS_INT;
//					taskENTER_CRITICAL(); //进入临界区
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);//右灯常亮
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);  //左灯常亮
//					EN_INT;
//					delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//					DIS_INT;					
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);  //气氛灯常亮
					EN_INT;
//					taskEXIT_CRITICAL(); //退出临界区
					k=6,m=0;//初始化跑马数据	
					RGB_init_status(RGB_CIRCLE_TYPE,1,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);	//圆灯数据初始化，逆时针方向,圆灯数量					
				}
				//DIS_INT;
				RGB_LED_HRL(RGB_CIRCLE_TYPE,1,colo_cir,RGB_CIRCLE_NUMBER,RGB_BLUE);//	圆灯逆时针跑马
                //EN_INT;				
			}
		}break;
		case TOLEFT: //左转
		{
			if(Timer[TOLEFTTIMER].Flag)
			{
				Set_Timer(TOLEFTTIMER,12);//60ms
				if(flag_up==1)
				{		
					DIS_INT;	
//taskENTER_CRITICAL(); //进入临界区					
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);//右灯常亮 360us
					RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE); //圆灯常亮 660us
				
//				EN_INT;
//				delay_us(10);//累计时间超过系统节拍，可被高优先级任务抢占
//				DIS_INT;		
				
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);//气氛灯常亮
					EN_INT;	
//					taskEXIT_CRITICAL(); //退出临界区
                    k=3,m=0;//初始化跑马数据		                   					
					RGB_init_status(RGB_LEFT_TYPE,1,colo_lr,RGB_LEFT_NUMBER,RGB_BLUE);				
				}
				//DIS_INT;
                RGB_LED_HRL(RGB_LEFT_TYPE ,1,colo_lr,RGB_LEFT_NUMBER,RGB_BLUE);//左灯跑马,逆时针跑马		
               // EN_INT;					
			}
		} break;
		case TORIGHT://右转
		{
			if(Timer[TORIGHTTIMER].Flag)
			{
				Set_Timer(TORIGHTTIMER,12);//60ms跑马频率
				if(flag_up==1)
				{		
					DIS_INT;
//					taskENTER_CRITICAL(); //进入临界区
					RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//圆灯常量
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);//左灯常量
//					EN_INT;
//					delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//					DIS_INT;			
					RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);//右灯常
                    EN_INT;	
//					taskEXIT_CRITICAL(); //退出临界区
					k=3,m=0;//初始化跑马数据						
                    RGB_init_status(RGB_RIGHT_TYPE,1,colo_lr,RGB_LEFT_NUMBER,RGB_BLUE);					
				}
				//DIS_INT;
				RGB_LED_HRL(RGB_RIGHT_TYPE ,1,colo_lr,RGB_RIGHT_NUMBER,RGB_BLUE);//左灯跑马,逆时针跑马
               // EN_INT;					
			}
		}break;
		case ESTOP:
		{
			if(flag_up==1)
			{
				DIS_INT;	
//				taskENTER_CRITICAL(); //进入临界区
                //这里忽略复位信号（单总线协议，刷新灯珠后，则后面执行的程序相当于延迟，超过50us以后相当于一个复位信号）				
				RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);  //开启右灯 大约耗时360us
				RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);    //开启左灯	 大约耗时360us
//				EN_INT;
//				delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//				DIS_INT;
				RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//开启左灯 大约耗时660us
//				EN_INT;
//				delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//				DIS_INT;
				RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_BLUE);    //开启气氛灯 气氛灯较长，大约耗时1.4ms 
//				 taskEXIT_CRITICAL(); //退出临界区
				EN_INT;	
			}		
		}break;
		case FSTOP://停止
		{
			if(flag_up==1)
			{
				DIS_INT;	
//				taskENTER_CRITICAL(); //进入临界区
                //这里忽略复位信号（单总线协议，刷新灯珠后，则后面执行的程序相当于延迟，超过50us以后相当于一个复位信号）				
				RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_RED);  //开启右灯 大约耗时360us
				RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_RED);    //开启左灯	 大约耗时360us
//				EN_INT;
//				delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//				DIS_INT;
				RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_RED);//开启左灯 大约耗时660us
//				EN_INT;
//				delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//				DIS_INT;
				RGB_LED_Light(RGB_ATMP_TYPE,RGB_ATMP_NUMBER,RGB_RED);    //开启气氛灯 气氛灯较长，大约耗时1.4ms 
//				 taskEXIT_CRITICAL(); //退出临界区
				EN_INT;	
			}			
		}break;
		case DIDLE://空闲
		{
			if(Timer[TURNLEDTIMER].Flag)
			{
				s_num++;
				Set_Timer(TURNLEDTIMER, 17);//100ms切换一次
				if(flag_up==1)
				{		
					DIS_INT;
//					taskENTER_CRITICAL(); //进入临界区
					RGB_LED_Light(RGB_RIGHT_TYPE,RGB_RIGHT_NUMBER,RGB_BLUE);//开启右灯
					RGB_LED_Light(RGB_LEFT_TYPE,RGB_LEFT_NUMBER,RGB_BLUE);//左灯常量
//					EN_INT;
//					delay_us(400);//累计时间超过系统节拍，可被高优先级任务抢占
//					DIS_INT;
					RGB_LED_Light(RGB_CIRCLE_TYPE,RGB_CIRCLE_NUMBER,RGB_BLUE);//圆灯常量
                    RGB_LED_BLN(RGB_ATMP_TYPE,RGB_ATMP_NUMBER);//气氛灯呼吸效果 大约耗时1.4ms	
//					 taskEXIT_CRITICAL(); //退出临界区
                    EN_INT;	
                    s_num=0;					
				}
				if(s_num>25)
				{
					s_num=26;
					DIS_INT;
					 //taskENTER_CRITICAL(); //进入临界区
				    RGB_LED_BLN(RGB_ATMP_TYPE,RGB_ATMP_NUMBER);//气氛灯呼吸效果 大约耗时830us 
					// taskEXIT_CRITICAL(); //退出临界区
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

