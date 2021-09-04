/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: motor.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"

#define MotCmd_net  0x0024  //网络操作模式
#define MotCmd_pos  0x0025  //位置模式
#define MotCmd_run  0x002D  //设置运动模式 00相对运动模式 01绝对运动
#define MotCmd_ctr  0x0028  //电机开关 01使能，00失能
#define MotCmd_dis  0x0026  //设置运动距离
#define MotCmd_str  0x0029  //开始运动
#define MotCmd_setpos 0x00AA //读取电机位置设定值
#define MotCmd_locpos 0x00AC //读取电机位置实际值

#define MotCmdRead  0x03
#define MotCmdWrite 0x10
#define MotCmdCommand 0x06

//上限  1097647  --->65mm
//下限  20002499 --->0 mm
#define MAX_POS 0x10BFAF
#define MIN_POS 0x13136C3

typedef struct 
{
	int32_t pos_data;
}motorData_t;

/* 函数定义 */
void InitMotor(void);
extern void Uart4_ReceiveIQR(uint8_t Buffer);
void dataCtrl_MOTOR(motorData_t* MotorData); 
void SendCmdToMotor(u16 Cmd,u16 cmD,int32_t disSet);
void SendToMotor(u8 *str ,u8 len);























#endif
