/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Adc.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/

#include "Adc.h"
#include "Usart.h"
#include "stdio.h"
#include "delay.h"
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "UserTask.h"
#endif

//#define         preFall_DEBUG   //��������

#define N  6 //������ȡƽ��ֵ
#define  AD_V_MCU_Gain      ((float)Vref / 4096)
#define  ADCONVTODIS(x)     (((float)12000/(x))-0.42)
#define ADC3_DR_Address    ((u32)0x40012200+0x4c)
#define OFFSET(x)         (((float)x)/4+2.5)

uint16_t ADC_Value[8*N];//ADC�ɼ�����
uint16_t ADC_Result[8];
float Value_AD[8] = {0,0,0,0,0,0,0,0};
uint16_t Vref = 3300;//mv
uint16_t Distance[8] = {0,0,0,0,0,0,0,0};//cm

extern SemaphoreHandle_t BiSemapDma;
void ADCInit_Timer(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);          

    //ʧ�ܶ�ʱ��
    TIM_Cmd(TIM2, DISABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //��ʼ����ʱ��
    
    //��ʱ������,T=(84 * 625) / 84M = 625us = 20ms/32,50Hz�ź�,ÿ���ڲ���32����,����Ƶ��1600
    TIM_TimeBaseStructure.TIM_Prescaler = 84-1; //��Ƶϵ��,TIM2ʱ���ڷ�Ƶϵ����Ϊ1ʱ,ʱ����APB1ʱ�ӵ�����84M
    TIM_TimeBaseStructure.TIM_Period = 625 - 1; //����ֵ1M/32=31250,CurrentFreq��λΪ0.1hz,���������10��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷ�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ; //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ����ʱ��2
    
    //ʹ�ܶ�ʱ���ж�    
    TIM_ARRPreloadConfig(TIM2, ENABLE); //����TIM2��ʱ����
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);  //ѡ��TIM2��UPDATA�¼�����Ϊ����Դ
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //����TIM2�ж�����
    
    //ʹ��TIM2
    TIM_Cmd(TIM2, ENABLE); 
  
}
/*************************************************

Function:	ADC_Config()

Description:  ADC���ó�ʼ��

*************************************************/
void ADC_Config(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    ADC_InitTypeDef ADC_InitStruct;
	
	ADC_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//84/4=21M
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12 λģʽ
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//����ת��
//	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //�����ش���
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;  //��ʱ���¼�2����ADC
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	ADC_InitStruct.ADC_NbrOfConversion = 8;//8 ��ת���ڹ���������
	ADC_Init(ADC3, &ADC_InitStruct);//ADC ��ʼ��
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_480Cycles );//PF3 

	ADC_RegularChannelConfig(ADC3, ADC_Channel_14, 2, ADC_SampleTime_480Cycles );//PF4
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_15, 3, ADC_SampleTime_480Cycles );//PF5
    ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 4, ADC_SampleTime_480Cycles );//PF6
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 5, ADC_SampleTime_480Cycles );//PF7
	ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 6, ADC_SampleTime_480Cycles );//PF8
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 7, ADC_SampleTime_480Cycles );//PF9
	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 8, ADC_SampleTime_480Cycles );//PF10	
	
    ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); //����ʹ��DMA
	ADC_DMACmd(ADC3, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
//	ADC_ClearFlag(ADC3, ADC_FLAG_EOC);
//	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);
//	ADC_SoftwareStartConv(ADC3);

  
}

/*************************************************

Function:	DMA_Configuration(void)

Description:  DMA���ó�ʼ��

*************************************************/
void DMA_Configuration(void) 
{
    /* ADC3  DMA2 Channel Config */  
    
    DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //ʹ��DMAʱ��
	
	DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
   
	DMA_InitStructure.DMA_Channel=DMA_Channel_2;//ͨ��2ΪADC3
    DMA_InitStructure.DMA_PeripheralBaseAddr =  ADC3_DR_Address;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Value;      //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = 8*N;  //��ֵΪ����һ��DMA����Ĵ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������λ���16λ����DMA����ߴ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ��һ�ֽ������Զ���ʼ���ִ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//FIFO ��ֵ
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralInc=0;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);  //����DMA_InitStruct�в���DMAͨ��
	DMA_ClearFlag(DMA2_Stream0,DMA_IT_TC);
	DMA_ITConfig( DMA2_Stream0, DMA_IT_TC, ENABLE);
//	DMA_ITConfig( DMA2_Stream0, DMA_IT_HT, ENABLE); 
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

/*************************************************

Function:	DMA_IT_Ctrl(void)

Description:  DMA�жϴ���

*************************************************/
void DMA_IT_Ctrl(void)
{
   BaseType_t xHigherPriorityTaskWoken;
	uint8_t i,j=0;

    for(i=0;i<8*N;++i)
	{
		if(j==8)
		{
		  j=0;
		}
	 ADC_Result[j++]+=ADC_Value[i];
	}
	for(j=0;j<8;j++)
	{
      ADC_Result[j]/=N;
	}
		if(BiSemapDma!=NULL)//���յ����ݣ����Ҷ�ֵ�ź�����Ч
	{
//		Uart1_RecOK = 0;
		xSemaphoreGiveFromISR(BiSemapDma,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
	}

 
	
}

/*************************************************

Function:	 ADC_IT_Ctrl();

Description:  ADC�жϴ���

*************************************************/
 void ADC_IT_Ctrl(void)
 {

 }	 
 /*************************************************

Function:	Fallpre_Ctrl(void)

Description:  ��������ɼ�

*************************************************/
 void Fallpre_Ctrl(void)
 {
	
	 u8 j;
	 for(j=0;j<8;j++)
	{
	    Value_AD[j]=(float)ADC_Result[j] * AD_V_MCU_Gain;
		Distance[j]=(uint16_t)ADCONVTODIS(Value_AD[j]);
		Distance[j]=Distance[j]+(uint16_t)OFFSET(Distance[j]);
	}

	for(j=0;j<8;j++)
	{
		if(Distance[j]>45||Distance[j]<3)
		{Distance[j]=0xffff;}
		
		#ifdef preFall_DEBUG
		printf("��%d·������:%d\n",j,(uint8_t)((Distance[j])&0x00FF));
		#endif
	}	
 }
 