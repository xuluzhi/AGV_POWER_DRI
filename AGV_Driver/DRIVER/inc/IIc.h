/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: IIc.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __IIC_H
#define __IIC_H
#include "sysz.h" 
#if 1
//IO��������
#define SDA_IN()  {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=0<<9*2;}	//PC9����ģʽ
#define SDA_OUT() {GPIOC->MODER&=~(3<<(9*2));GPIOC->MODER|=1<<9*2;} //PC9���ģʽ
//IO��������

#define IIC_SCL    PAout(8) //SCL
#define IIC_SDA    PCout(9) //SDA	 
#define READ_SDA   PCin(9)  //����SDA 
#define READ_SCL   PAin(8) //����SCL
#endif

#if 0
#define SDA_IN()  {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=0<<0*2;}	//PC9����ģʽ
#define SDA_OUT() {GPIOF->MODER&=~(3<<(0*2));GPIOF->MODER|=1<<0*2;} //PC9���ģʽ
#define IIC_SCL    PFout(1) //SCL
#define IIC_SDA    PFout(0) //SDA	 
#define READ_SDA   PFin(0)  //����SDA 
#define READ_SCL   PFin(1) //����SCL
#endif

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
