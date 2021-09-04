/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: UserTask.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef _USER_TASK_H
#define _USER_TASK_H
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#define DEBUG_STATE  0//���Խ׶ο��� 
/* ȫ�ֱ������� */
extern SemaphoreHandle_t BiSemapUsart1;	//����1��ֵ�ź������
extern SemaphoreHandle_t BiSemapDma; //DMA��ֵ�ź���
extern SemaphoreHandle_t BiSemapBms; //BMS���ն�ֵ�ź�
extern EventGroupHandle_t  EventGroup_Handler;//�¼���־����;//�������¼�
//�������¼������
#define SENSOR1_JACK_BIT0	(1<<0)	//����������1
#define SENSOR2_JACK_BIT1	(1<<1)  //����������2
#define SENSOR_ROTATE_BIT2	(1<<2)  //��ת��㴫����
#define EVENTBIT_ALL	(SENSOR1_JACK_BIT0|SENSOR2_JACK_BIT1|SENSOR_ROTATE_BIT2)
/* �������� */

void vUserTaskSchedule(void);


/* ���������� */
void UserTask1(void);
void UserTask2(void);
void UserTask3(void);

/* �ⲿ���� */
extern void Uart1_DataRespond(void);
extern void RequestBmsData(void);
extern void Change_DetectMode (void);
extern void BmsDataUpdate(void);
#endif
