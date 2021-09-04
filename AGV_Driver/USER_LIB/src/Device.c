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
	1.����PB3 PB4 �˿�Ϊ����ģʽ
	1.����PG13 PG14 �˿�Ϊ��������ģʽ
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

Description:  ʱ�����ó�ʼ��

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

Description:  ODR������ݼĴ���
              MODEģʽ�Ĵ�����  0�����룬1�������2�����ã�3��ģ��

              OTYPE���ƼĴ����� OUTPUT��0�����죬1��©����·
                             
              OSPEED���ƼĴ�����OUTPUT��0��Low��1��Medium��2��Low��3��High
                                
              PUPD���ƼĴ�����  0������������1��������2��������3��Ԥ��

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
    GPIOB->PUPDR  = 0x00000140;//����PB3 PB4
    
    
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

Description:  �ⲿ�ж�����

*************************************************/
void EXIT_Config(void)
{	

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
	
#ifdef EN_IR
	
	//PE 2 3 4   ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

	// ���2 3 4�ⲿ��
    EXTI_ClearITPendingBit(EXTI_Line4);
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_ClearITPendingBit(EXTI_Line3);
	
	
	//���� 2 3 4 �ⲿ�ж���
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);    //407ʹ�õ����ú���
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);    //407ʹ�õ����ú���
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);    //407ʹ�õ����ú���
	
	
	//EXTI2   
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//���벶��
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//˫���ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;		//ʹ��
	EXTI_Init(&EXTI_InitStructure); 

	//EXTI3  
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;  //���벶��  ˫���ش���
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//���벶��
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//˫���ش���
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;		//ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	//EXTI4  
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//���벶��  ˫���ش���
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//���벶��
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;	//˫���ش���
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;		//ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
//	DIS_EXIT;//Ĭ�Ϲر��ж�
#endif	
}

/*************************************************

Function:	PeriphInit()

Description:  �����ʼ��

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
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}
/*************************************************

Function:	BspInit()

Description:  �弶��ʼ��

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
