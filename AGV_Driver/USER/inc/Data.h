/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Data.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __DATA_H
#define __DATA_H
#include "stm32f4xx.h"
#include "Rfid.h"
typedef enum  {
	INITS =1,
	FORWARD, //2
	BACK,    //3
	TOLEFT,  //4
	TORIGHT, //5
	PRERUN,  //6
	ESTOP,   //7
	FSTOP,   //8
	DIDLE	 //9
}runDirection;

typedef enum  {	
	INIT=1,
	RUN,
	STOP,
	RIDLE,
	FAULT
}runStatus;

typedef struct 
{
	
  runDirection dir;             //运动方向
  runStatus sta;                //工作状态
  runDirection (*p)(void);     
  runStatus (*t)(void);
} robotRunStatus;

typedef struct
{
  u8 vol;
  u8 chargeSta;
  u8 temp;
  u8 soc;
  void(*p)(void);
} bmsStatus;
typedef struct
{
 robotRunStatus RunSta;
 bmsStatus BmsSta;
} RobotDef;
extern volatile uint8_t faultFlag;

/* 变量 */
extern RobotDef Robot;
extern RfidDef Rfid;
/* 函数声明 */
void vUser_Init(void);
#endif 
