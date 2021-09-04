/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Device.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 
************************
	mender:xu lu zhi
	Date: 2021.8.23
	Version: 1.0.1 
	revise content:
	1.更改PB3 PB4 端口为上拉模式
	1.更改PG13 PG14 端口为上拉输入模式
*************************
***********************************************************/
#include "Device.h"
#include "Usart.h"
#include "Timer.h"
#include "Adc.h"
#include "delay.h"
#include "ks136.h"
#include "ws2812b.h"
/*************************************************

Function:	Clk_init()

Description:  时钟配置初始化

*************************************************/
void CLK_Init(void)
{
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN\
                  | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN\
	              | RCC_AHB1ENR_GPIOGEN;
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
   
}

/*************************************************

Function:	GPIO_Config()

Description:  ODR输出数据寄存器
              MODE模式寄存器，  0：输入，1：输出，2：复用，3：模拟

              OTYPE控制寄存器， OUTPUT：0：推挽，1：漏极开路
                             
              OSPEED控制寄存器，OUTPUT：0：Low，1：Medium，2：Low，3：High
                                
              PUPD控制寄存器，  0：无上下拉，1：上拉，2：下拉，3：预留

*************************************************/
void GPIO_Config(void)
{	
 //PORT A GROUP
    /*BIT       15     14     13     12     11     10     9     8     7     6     5     4     3     2     1     0   */
    /*ODR       0      0      0      0      0      0      0     1     0     0     0     0     0     0     0     0   */
    /*MODE      1      0      0      1      1      1      1     1     1     1     1     1     1     1     1     1   */
    /*OTYPE     0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*OSPEED    0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*PUPD      0      0      0      0      0      0      0     1     0     0     0     0     0     0     0     0   */
    /*NAME      -     SWC    SWD     -      -     RX1    TX1    SCL   -     Ly9   Ly6   Ly8  Lyred1 -     -   Ly7   */
    
    GPIOA->ODR    |= 0x0100;
    GPIOA->MODER  |= 0x41555555;
    GPIOA->OTYPER |= 0x0000;
    GPIOA->PUPDR  |= 0x00010000;
    
    
//PORT B GROUP
    /*BIT       15     14     13     12     11     10     9     8     7     6     5     4     3     2     1     0   */
    /*ODR       0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*MODE      1      1      1      1      1      1      1     1     1     1     1     0     0     1     0     1   */
    /*OTYPE     0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*OSPEED    0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*PUPD      0      0      0      0      0      0      0     0     0     0     0     1     1     0     0     0   */
    /*NAME	   Ledld  Ledml Ledrd   Ledmr  Ledld  Ledml  Por2  AmpNo AmpEn  -     -    Mot4  Mot3  Ly10 BOOT1  Ly5  */
    
    GPIOB->ODR    = 0x0000;
    GPIOB->MODER  = 0x55555411;
    GPIOB->OTYPER = 0x0;
	GPIOB->OSPEEDR= 0x00A00000;
    GPIOB->PUPDR  = 0x00000140;//上拉PB3 PB4
    
    
//PORT C GROUP
    /*BIT       15     14     13     12     11     10     9     8     7     6     5     4     3     2     1     0   */
    /*ODR       0      0      0      0      0      0      1     0     0     0     0     0     0     0     0     0   */
    /*MODE      1      1      1      1      1      1      1     1     1     1     1     1     1     1     1     1   */
    /*OTYPE     0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*OSPEED    0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*PUPD      0      0      0      0      0      0      1     0     0     0     0     0     0     0     0     0   */
    /*NAME		OSCO  OSCI    fan    TX5     RX4    TX4   SDA    -    RX6   TX6   -     -     -     -     -     -   */
    
    
      GPIOC->ODR    = 0x0200;
      GPIOC->MODER  = 0x55555555;
      GPIOC->OTYPER = 0x0000;
      GPIOC->PUPDR  = 0x00040000;
  
    
    
//PORT D GROUP
    /*BIT       15     14     13     12     11     10     9     8     7     6     5     4     3     2     1     0   */
    /*ODR       0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*MODE      1      1      0      0      1      1      1     1     1     1     1     1     1     1     1     1   */
    /*OTYPE     0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*OSPEED    0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*PUPD      0      0      1      1      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*NAME	    -      -     FZ     FZ    Ledrl  Ledmr   RX3   TX3    -    RX2   TX2    -     -    RX5    -     -   */
    
    //GPIOD->ODR    = 0x0000;
    GPIOD->MODER  = 0x50555555;
    GPIOD->OTYPER = 0x0;
    GPIOD->PUPDR  = 0x05000000;
    
    
//PORT E GROUP
    /*BIT       15     14     13     12     11     10     9     8     7     6     5     4     3     2     1     0   */
    /*ODR       0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*MODE      1      1      1      1      1      1      1     1     1     1     1     1     1     1     1     0   */
    /*OTYPE     0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*OSPEED    0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*PUPD      0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     2   */
    /*NAME	 Lcred    LEDrt   Lc1   LEDrt   Lc2  LEDlt    Lc3  LEDlt  Lc4   Lfw   Lfw  Motmp Mot2  Mot1  Chg   Key  */
    
    GPIOE->ODR    = 0x0000;
    GPIOE->MODER  = 0x55555554;
    GPIOE->OTYPER = 0x0;
	GPIOE->OSPEEDR= 0xA0000000;
    GPIOE->PUPDR  = 0x00000002;
    
    
//PORT F GROUP
    /*BIT       15     14     13     12     11     10     9     8     7     6     5     4     3     2     1     0   */
    /*ODR       0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*MODE      1      1      1      1      1      1      1     1     1     1     1     1     1     1     1     1   */
    /*OTYPE     0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*OSPEED    2      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*PUPD      0      0      0      0      0      0      0     0     0     0     0     0     0     0     1     1   */
    /*NAME	    Ly2   Ly12    Ly3   Ly11   Ly4     -      -     -     -     -     -     -     -    LEDr PowerON fan */
   #if 0
    GPIOF->ODR    = 0x0;
    GPIOF->MODER  = 0x55555555;
    GPIOF->OTYPER = 0x0;
    GPIOF->PUPDR  = 0x00000005;
	#endif
	GPIOF->ODR    = 0x0;
    GPIOF->MODER  = 0x55555555;
    GPIOF->OTYPER = 0x0;
	GPIOF->OSPEEDR= 0xB8000000;
    GPIOF->PUPDR  = 0x00000005;
	
//PORT G GROUP
    /*BIT       15     14     13     12     11     10     9     8     7     6     5     4     3     2     1     0   */
    /*ODR       0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*MODE      1      0      0      1      1      1      1     1     1     0     1     1     1     1     1     1   */
    /*OTYPE     0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*OSPEED    0      0      0      0      0      0      0     0     0     0     0     0     0     0     0     0   */
    /*PUPD      0      1      1      0      0      0      0     0     0     1     0     0     0     0     0     0   */
    /*NAME	    -      -      -      -      -      -      -     -     -     stp   -     -     -     -    Ly1 Lyred2 */
    
    GPIOG->ODR    = 0x0000;
    GPIOG->MODER  = 0x41554555;
    GPIOG->OTYPER = 0x0;
	GPIOG->OSPEEDR= 0x00000002;
    GPIOG->PUPDR  = 0x14001000;	
	
//PORT H 0-1	reserve
	

}

/*************************************************

Function:	EXIT_Config()

Description:  外部中断配置

*************************************************/
void EXIT_Config(void)
{	

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
	
#ifdef EN_IR
	
	//PE 2 3 4   红外配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

	// 清除2 3 4外部线
    EXTI_ClearITPendingBit(EXTI_Line4);
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_ClearITPendingBit(EXTI_Line3);
	
	
	//配置 2 3 4 外部中断线
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);    //407使用的配置函数
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);    //407使用的配置函数
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);    //407使用的配置函数
	
	
	//EXTI2   
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//输入捕获
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//双边沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;		//使能
	EXTI_Init(&EXTI_InitStructure); 

	//EXTI3  
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;  //输入捕获  双边沿触发
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//输入捕获
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//双边沿触发
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;		//使能
	EXTI_Init(&EXTI_InitStructure);
	
	//EXTI4  
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//输入捕获  双边沿触发
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//输入捕获
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//双边沿触发
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;		//使能
	EXTI_Init(&EXTI_InitStructure);
	
//	DIS_EXIT;//默认关闭中断
#endif	
}

/*************************************************

Function:	PeriphInit()

Description:  外设初始化

*************************************************/
void PeriphInit(void)
{

   Usart_Config();	
//   TIM2_Init();
   TIM3_Init();
//	 ADCInit_Timer();
//   DMA_Configuration();
//   ADC_Config(); 
}
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB->AIRCR=temp;  //设置分组	    	  				   
}
/*************************************************

Function:	BspInit()

Description:  板级初始化

*************************************************/
void BspInit(void)
{

    DIS_INT;
    SystemInit();
    CLK_Init();
    GPIO_Config();
	MY_NVIC_PriorityGroupConfig(4);	 
	
//    EXIT_Config();
    EN_INT;
    delay_init(168);
    PeriphInit();
//	Ks136_Init();
}
