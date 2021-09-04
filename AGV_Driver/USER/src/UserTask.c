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

#define vTask1_Priority    6        /* ���������ݸ������� */
#define vTask2_Priority    7        /* ��Դ����*/
#define vTask3_Priority    4        /* �������� */
#define vTask4_Priority    5        /* RFID */
#define vTask5_Priority    4        /* RGB�߼� */
#if DEBUG_STATE //���Խ׶ο��� 
#define vTask6_Priority    4        /* ��֤�¼���� */
#endif
/* ������ */
TaskHandle_t AppTaskCreate_Handle1 = NULL;
TaskHandle_t AppTaskCreate_Handle2 = NULL;
TaskHandle_t AppTaskCreate_Handle3 = NULL;
TaskHandle_t AppTaskCreate_Handle4 = NULL;
TaskHandle_t AppTaskCreate_Handle5 = NULL;
TaskHandle_t AppTaskCreate_Handle6 = NULL;
//��ֵ�ź������
SemaphoreHandle_t BiSemapUsart1;	//����1��ֵ�ź������
SemaphoreHandle_t BiSemapDma; //DMA��ֵ�ź���
SemaphoreHandle_t BiSemapBms; //BMS���ն�ֵ�ź�

EventGroupHandle_t  EventGroup_Handler;
/* ���о�� */
QueueHandle_t Queue_usart1_Rec;
/* �û������� */
extern motorData_t motorData;
/*************************************************

Function:	

Description:  �û�����

*************************************************/
void UserTask1(void)//����1����
{
	BaseType_t err=pdFALSE;
      while(1)
	{
/* �������ӿں��� */	
	
		if(BiSemapUsart1!=NULL)
		{	
			err=xSemaphoreTake(BiSemapUsart1,portMAX_DELAY);	//��ȡ�ź���
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
/* �������ӿں��� */

//  
//			if(BiSemapBms!=NULL)
//		{	
//			err=xSemaphoreTake(BiSemapBms,portMAX_DELAY);	//��ȡ�ź���
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
	/* �������ӿں��� */	
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
/* �������ӿں��� */
    //vLedCtrl(&Robot);
	 #if 0//DEBUG_STATE  
	 Robot.RunSta.dir=BACK;
	 #endif
	RGB_Ctrl(&Robot);
	taskYIELD();
/* END */ 
  }
}
#if DEBUG_STATE //���Խ׶ο��� 
void UserTask6(void)
{
	EventBits_t group;
	EventBits_t EventBits;
	while(1)
	{
	/* �������ӿں��� */
	EventBits=xEventGroupWaitBits((EventGroupHandle_t) EventGroup_Handler,
								 (EventBits_t) SENSOR1_JACK_BIT0|SENSOR2_JACK_BIT1,//Ҫ�ȴ��ı�־λ
								 (BaseType_t) pdTRUE, //�Ƿ������־λ
								 (BaseType_t) pdTRUE, //�������ȫ����λ
								 (TickType_t) portMAX_DELAY);//���ȴ�ʱ��
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

Description:  �û����񴴽�

*************************************************/
void vUserTaskCreate(void)
{
	//��̬��������
	
   taskENTER_CRITICAL(); //�����ٽ���
	
	/* ���д��� */
	
	
	/* �ź������� */
	BiSemapUsart1=xSemaphoreCreateBinary();
	BiSemapDma=xSemaphoreCreateBinary();
	BiSemapBms=xSemaphoreCreateBinary();
	/* �¼�����鴴��*/
	EventGroup_Handler=xEventGroupCreate();//�����¼������
	/* �����Ҫ������ */
	
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
#if DEBUG_STATE //���Խ׶ο��� 							
    xTaskCreate((TaskFunction_t) UserTask6,
							(const char *) "UserTask6",
							(const uint16_t) 512,
							(void *) NULL,
							(UBaseType_t) vTask6_Priority,
							(TaskHandle_t *) AppTaskCreate_Handle6);	
#endif							
							
   /* END */
  taskEXIT_CRITICAL(); //�˳��ٽ���

}

/*************************************************

Function:	vUserQueueCreate()

Description:  �û����д���

*************************************************/
void vUserQueueCreate(void)
{
taskENTER_CRITICAL(); //�����ٽ���
	
Queue_usart1_Rec=xQueueCreate(Queue_usart1_Len,sizeof(uint8_t));
	
	
taskEXIT_CRITICAL(); //�˳��ٽ���
}
/*************************************************

Function:	vUserTaskSchedule()

Description:  ������

*************************************************/
void vUserTaskSchedule(void)
{

	vUserTaskCreate();
	vTaskStartScheduler();
    while(1);

}
