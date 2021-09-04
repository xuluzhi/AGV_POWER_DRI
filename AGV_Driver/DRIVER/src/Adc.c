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
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "UserTask.h"
#endif

//#define         preFall_DEBUG   //防跌调试

#define N  6 //次数，取平均值
#define  AD_V_MCU_Gain      ((float)Vref / 4096)
#define  ADCONVTODIS(x)     (((float)12000/(x))-0.42)
#define ADC3_DR_Address    ((u32)0x40012200+0x4c)
#define OFFSET(x)         (((float)x)/4+2.5)

uint16_t ADC_Value[8*N];//ADC采集数据
uint16_t ADC_Result[8];
float Value_AD[8] = {0,0,0,0,0,0,0,0};
uint16_t Vref = 3300;//mv
uint16_t Distance[8] = {0,0,0,0,0,0,0,0};//cm

extern SemaphoreHandle_t BiSemapDma;
void ADCInit_Timer(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);          

    //失能定时器
    TIM_Cmd(TIM2, DISABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //初始化定时器
    
    //定时器设置,T=(84 * 625) / 84M = 625us = 20ms/32,50Hz信号,每周期采样32个点,采样频率1600
    TIM_TimeBaseStructure.TIM_Prescaler = 84-1; //分频系数,TIM2时钟在分频系数不为1时,时钟是APB1时钟的两倍84M
    TIM_TimeBaseStructure.TIM_Period = 625 - 1; //周期值1M/32=31250,CurrentFreq单位为0.1hz,因此需扩大10倍
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ; //向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化定时器2
    
    //使能定时器中断    
    TIM_ARRPreloadConfig(TIM2, ENABLE); //允许TIM2定时重载
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);  //选择TIM2的UPDATA事件更新为触发源
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //配置TIM2中断类型
    
    //使能TIM2
    TIM_Cmd(TIM2, ENABLE); 
  
}
/*************************************************

Function:	ADC_Config()

Description:  ADC配置初始化

*************************************************/
void ADC_Config(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    ADC_InitTypeDef ADC_InitStruct;
	
	ADC_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//84/4=21M
	ADC_CommonInit(&ADC_CommonInitStructure);//初始化

    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	ADC_StructInit(&ADC_InitStruct);
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12 位模式
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;//扫描模式
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//连续转换
//	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //上升沿触发
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;  //定时器事件2触发ADC
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStruct.ADC_NbrOfConversion = 8;//8 个转换在规则序列中
	ADC_Init(ADC3, &ADC_InitStruct);//ADC 初始化
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_480Cycles );//PF3 

	ADC_RegularChannelConfig(ADC3, ADC_Channel_14, 2, ADC_SampleTime_480Cycles );//PF4
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_15, 3, ADC_SampleTime_480Cycles );//PF5
    ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 4, ADC_SampleTime_480Cycles );//PF6
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 5, ADC_SampleTime_480Cycles );//PF7
	ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 6, ADC_SampleTime_480Cycles );//PF8
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 7, ADC_SampleTime_480Cycles );//PF9
	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 8, ADC_SampleTime_480Cycles );//PF10	
	
    ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); //连续使能DMA
	ADC_DMACmd(ADC3, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
//	ADC_ClearFlag(ADC3, ADC_FLAG_EOC);
//	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);
//	ADC_SoftwareStartConv(ADC3);

  
}

/*************************************************

Function:	DMA_Configuration(void)

Description:  DMA配置初始化

*************************************************/
void DMA_Configuration(void) 
{
    /* ADC3  DMA2 Channel Config */  
    
    DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //使能DMA时钟
	
	DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
   
	DMA_InitStructure.DMA_Channel=DMA_Channel_2;//通道2为ADC3
    DMA_InitStructure.DMA_PeripheralBaseAddr =  ADC3_DR_Address;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Value;      //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = 8*N;  //此值为完整一轮DMA传输的次数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据位宽度16位，即DMA传输尺寸
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式，一轮结束后自动开始下轮传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//FIFO 阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
    DMA_InitStructure.DMA_PeripheralInc=0;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);  //根据DMA_InitStruct中参数DMA通道
	DMA_ClearFlag(DMA2_Stream0,DMA_IT_TC);
	DMA_ITConfig( DMA2_Stream0, DMA_IT_TC, ENABLE);
//	DMA_ITConfig( DMA2_Stream0, DMA_IT_HT, ENABLE); 
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

/*************************************************

Function:	DMA_IT_Ctrl(void)

Description:  DMA中断处理

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
		if(BiSemapDma!=NULL)//接收到数据，并且二值信号量有效
	{
//		Uart1_RecOK = 0;
		xSemaphoreGiveFromISR(BiSemapDma,&xHigherPriorityTaskWoken);	//释放二值信号量
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
	}

 
	
}

/*************************************************

Function:	 ADC_IT_Ctrl();

Description:  ADC中断处理

*************************************************/
 void ADC_IT_Ctrl(void)
 {

 }	 
 /*************************************************

Function:	Fallpre_Ctrl(void)

Description:  防跌距离采集

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
		printf("第%d路距离是:%d\n",j,(uint8_t)((Distance[j])&0x00FF));
		#endif
	}	
 }
 