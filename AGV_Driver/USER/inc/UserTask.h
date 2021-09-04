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
#define DEBUG_STATE  0//调试阶段开启 
/* 全局变量声明 */
extern SemaphoreHandle_t BiSemapUsart1;	//串口1二值信号量句柄
extern SemaphoreHandle_t BiSemapDma; //DMA二值信号量
extern SemaphoreHandle_t BiSemapBms; //BMS接收二值信号
extern EventGroupHandle_t  EventGroup_Handler;//事件标志组句柄;//传感器事件
//传感器事件标记组
#define SENSOR1_JACK_BIT0	(1<<0)	//顶升传感器1
#define SENSOR2_JACK_BIT1	(1<<1)  //顶升传感器2
#define SENSOR_ROTATE_BIT2	(1<<2)  //旋转零点传感器
#define EVENTBIT_ALL	(SENSOR1_JACK_BIT0|SENSOR2_JACK_BIT1|SENSOR_ROTATE_BIT2)
/* 函数声明 */

void vUserTaskSchedule(void);


/* 任务函数声明 */
void UserTask1(void);
void UserTask2(void);
void UserTask3(void);

/* 外部函数 */
extern void Uart1_DataRespond(void);
extern void RequestBmsData(void);
extern void Change_DetectMode (void);
extern void BmsDataUpdate(void);
#endif
