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

#define MotCmd_net  0x0024  //�������ģʽ
#define MotCmd_pos  0x0025  //λ��ģʽ
#define MotCmd_run  0x002D  //�����˶�ģʽ 00����˶�ģʽ 01�����˶�
#define MotCmd_ctr  0x0028  //������� 01ʹ�ܣ�00ʧ��
#define MotCmd_dis  0x0026  //�����˶�����
#define MotCmd_str  0x0029  //��ʼ�˶�
#define MotCmd_setpos 0x00AA //��ȡ���λ���趨ֵ
#define MotCmd_locpos 0x00AC //��ȡ���λ��ʵ��ֵ

#define MotCmdRead  0x03
#define MotCmdWrite 0x10
#define MotCmdCommand 0x06

//����  1097647  --->65mm
//����  20002499 --->0 mm
#define MAX_POS 0x10BFAF
#define MIN_POS 0x13136C3

typedef struct 
{
	int32_t pos_data;
}motorData_t;

/* �������� */
void InitMotor(void);
extern void Uart4_ReceiveIQR(uint8_t Buffer);
void dataCtrl_MOTOR(motorData_t* MotorData); 
void SendCmdToMotor(u16 Cmd,u16 cmD,int32_t disSet);
void SendToMotor(u8 *str ,u8 len);























#endif
