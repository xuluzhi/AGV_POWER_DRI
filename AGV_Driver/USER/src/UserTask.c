/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Task.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "UserTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "Adc.h"
#include "Usart1.h"
#include "voice.h"
#include "Rfid.h"
#include "Data.h"
#include "Power.h"
#include "motor.h"
#include "event_groups.h"
#include "usart.h"
#include "ws2812b.h"
#define Queue_usart1_Len   1

#define vTask1_Priority    6        /* 机器人数据更新任务 */
#define vTask2_Priority    7        /* 电源任务*/
#define vTask3_Priority    4        /* 语音处理 */
#define vTask4_Priority    5        /* RFID */
#define vTask5_Priority    4        /* RGB逻辑 */
#if DEBUG_STATE //调试阶段开启 
#define vTask6_Priority    4        /* 验证事件标记 */
#endif
/* 任务句柄 */
TaskHandle_t AppTaskCreate_Handle1 = NULL;
TaskHandle_t AppTaskCreate_Handle2 = NULL;
TaskHandle_t AppTaskCreate_Handle3 = NULL;
TaskHandle_t AppTaskCreate_Handle4 = NULL;
TaskHandle_t AppTaskCreate_Handle5 = NULL;
TaskHandle_t AppTaskCreate_Handle6 = NULL;
//二值信号量句柄
SemaphoreHandle_t BiSemapUsart1;	//串口1二值信号量句柄
SemaphoreHandle_t BiSemapDma; //DMA二值信号量
SemaphoreHandle_t BiSemapBms; //BMS接收二值信号

EventGroupHandle_t  EventGroup_Handler;
/* 队列句柄 */
QueueHandle_t Queue_usart1_Rec;
/* 用户任务函数 */
extern motorData_t motorData;
/*************************************************

Function:	

Description:  用户任务

*************************************************/
void UserTask1(void)//串口1任务
{
	BaseType_t err=pdFALSE;
      while(1)
	{
/* 添加任务接口函数 */	
	
		if(BiSemapUsart1!=NULL)
		{	
			err=xSemaphoreTake(BiSemapUsart1,portMAX_DELAY);	//获取信号量
			if(err==pdTRUE)
			{
//			   Uart1_DataRespond();
				robotvStatus(&Robot);
			}
			else
			{
			  vTaskDelay(10);
			}
		}

		
/* END */ 
    }

}

void UserTask2(void)
{
   BaseType_t err=pdFALSE;
	while(1)
{
/* 添加任务接口函数 */

//  
//			if(BiSemapBms!=NULL)
//		{	
//			err=xSemaphoreTake(BiSemapBms,portMAX_DELAY);	//获取信号量
//			if(err==pdTRUE)
//			{
////			   	BmsDataUpdate();
////				vLedCtrl(&Robot);
//			}
//			else
//			{
//			  vTaskDelay(10);
//			}
//		}
	 PowerKey_ctrl();
	 Scram_Key_ctrl();
     taskYIELD();
/* END */ 
}

}

void UserTask3(void)
{
	while(1)
	{
	/* 添加任务接口函数 */	
	//	dataCtrl_RFID(&Rfid);

	SYN_ctrl(&Robot);
	 taskYIELD();
	/* END */ 
	}
}

void UserTask4(void) 
{	
    while(1)
   { 
     dataCtrl_RFID(&Rfid);
	 dataCtrl_MOTOR(&motorData);
     taskYIELD();
   }
  
}
 
void UserTask5(void)
{
  while(1)
 {
/* 添加任务接口函数 */
    //vLedCtrl(&Robot);
	 #if 0//DEBUG_STATE  
	 Robot.RunSta.dir=BACK;
	 #endif
	RGB_Ctrl(&Robot);
	taskYIELD();
/* END */ 
  }
}
#if DEBUG_STATE //调试阶段开启 
void UserTask6(void)
{
	EventBits_t group;
	EventBits_t EventBits;
	while(1)
	{
	/* 添加任务接口函数 */
	EventBits=xEventGroupWaitBits((EventGroupHandle_t) EventGroup_Handler,
								 (EventBits_t) SENSOR1_JACK_BIT0|SENSOR2_JACK_BIT1,//要等待的标志位
								 (BaseType_t) pdTRUE, //是否清零标志位
								 (BaseType_t) pdTRUE, //任意或者全部置位
								 (TickType_t) portMAX_DELAY);//最大等待时间
	group=xEventGroupGetBits(EventGroup_Handler);
	USARTSendByte((u8)group,USART1);
	taskYIELD();
//	vTaskDelay(100);
	/* END */ 
	}
}
#endif
/*************************************************

Function:	vUserTaskCreate()

Description:  用户任务创建

*************************************************/
void vUserTaskCreate(void)
{
	//动态创建任务
	
   taskENTER_CRITICAL(); //进入临界区
	
	/* 队列创建 */
	
	
	/* 信号量创建 */
	BiSemapUsart1=xSemaphoreCreateBinary();
	BiSemapDma=xSemaphoreCreateBinary();
	BiSemapBms=xSemaphoreCreateBinary();
	/* 事件标记组创建*/
	EventGroup_Handler=xEventGroupCreate();//创建事件标记组
	/* 添加需要的任务 */
	
  xTaskCreate((TaskFunction_t) UserTask1,
							(const char *) "UserTask1",
							(const uint16_t) 512,
							(void *) NULL,
							(UBaseType_t) vTask1_Priority,
							(TaskHandle_t *) AppTaskCreate_Handle1);
							
  xTaskCreate((TaskFunction_t) UserTask2,
							(const char *) "UserTask2",
							(const uint16_t) 512,
							(void *) NULL,
							(UBaseType_t) vTask2_Priority,
							(TaskHandle_t *) AppTaskCreate_Handle2);
							
  xTaskCreate((TaskFunction_t) UserTask3,
							(const char *) "UserTask3",
							(const uint16_t) 512,
							(void *) NULL,
							(UBaseType_t) vTask3_Priority,
							(TaskHandle_t *) AppTaskCreate_Handle3);
							
	xTaskCreate((TaskFunction_t) UserTask4,
							(const char *) "UserTask4",
							(const uint16_t) 512,
							(void *) NULL,
							(UBaseType_t) vTask4_Priority,
							(TaskHandle_t *) AppTaskCreate_Handle4);
							
    xTaskCreate((TaskFunction_t) UserTask5,
							(const char *) "UserTask5",
							(const uint16_t) 512,
							(void *) NULL,
							(UBaseType_t) vTask5_Priority,
							(TaskHandle_t *) AppTaskCreate_Handle5);	
#if DEBUG_STATE //调试阶段开启 							
    xTaskCreate((TaskFunction_t) UserTask6,
							(const char *) "UserTask6",
							(const uint16_t) 512,
							(void *) NULL,
							(UBaseType_t) vTask6_Priority,
							(TaskHandle_t *) AppTaskCreate_Handle6);	
#endif							
							
   /* END */
  taskEXIT_CRITICAL(); //退出临界区

}

/*************************************************

Function:	vUserQueueCreate()

Description:  用户队列创建

*************************************************/
void vUserQueueCreate(void)
{
taskENTER_CRITICAL(); //进入临界区
	
Queue_usart1_Rec=xQueueCreate(Queue_usart1_Len,sizeof(uint8_t));
	
	
taskEXIT_CRITICAL(); //退出临界区
}
/*************************************************

Function:	vUserTaskSchedule()

Description:  任务处理

*************************************************/
void vUserTaskSchedule(void)
{

	vUserTaskCreate();
	vTaskStartScheduler();
    while(1);

}
