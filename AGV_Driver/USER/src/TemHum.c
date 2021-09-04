/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: TemHum.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: SHT3X模块

***********************************************************/
#include "TemHum.h"
#include "iic.h"
#include "delay.h"

#include "stdio.h"

#define POLYNOMIAL 0x131
#define I2CADDR    0x45

u8 i2c_Addr;

float temperature;
float humidity;
/*************************************************

Function:	SHT3X CRC计算

Description: SHT_CalcCrc()

*************************************************/
static u8 SHT3X_CalcCrc(u8 data[],u8 nbrOfBytes)
{
	u8 bit;
	u8 crc = 0XFF;
	u8 byteCtr;
	
	for(byteCtr = 0; byteCtr<nbrOfBytes;byteCtr++)
	{
		crc^=(data[byteCtr]);
		for(bit = 8;bit>0;--bit)
		{
			if(crc&0x80)crc=(crc<<1)^POLYNOMIAL;
			else     crc=(crc<<1);
		}
		
		
	}	
	return crc;
}

/*************************************************

Function:	SHT3X 初始化配置

Description: SHT3X_Init()

*************************************************/

void SHT3X_Init(u8 i2c_addr)
{
    	  	  
	IIC_SCL=0;
	i2c_Addr=i2c_addr;
	IIC_SCL=1;
    delay_ms(50);
}

/*************************************************

Function:	SHT3X 得到温度和湿度

Description: SHT3X_GetTempAndHumi()

*************************************************/
u8 SHT3X_GetTempAndHumi(float *temp,float *humidity,u8 timeout)
{
	u16 rawValueTemp;
	u16 rawValueHumi;
	u8 res=1;
	if(SHT3X_StartWriteAccess()==1)
	{

		IIC_Send_Byte(0x21);
		IIC_Wait_Ack();
		IIC_Send_Byte(0x26);
		IIC_Wait_Ack();
//		IIC_Stop();

		IIC_Send_Byte(0xE0);
		IIC_Wait_Ack();
		IIC_Send_Byte(0x00);
		IIC_Wait_Ack();
		delay_ms(5);
			IIC_Start();
		
		IIC_Send_Byte(0x89);
		IIC_Wait_Ack();
        IIC_Read_Byte(1);
        IIC_Read_Byte(1);
		IIC_Read_Byte(1);
		IIC_Read_Byte(1);
		IIC_Read_Byte(1);
		IIC_Read_Byte(0);
//		if(res==1)res=SHT_I2c_Read2Byte(&rawValueTemp,1,timeout);
//        if(res==1)res=SHT_I2c_Read2Byte(&rawValueTemp,0,0);
		IIC_Stop();
//		if(res==1)
//		{
//			*temp = SHT3X_CalcTemperature(rawValueTemp);
//			*humidity = SHT3X_CalcHumidity(rawValueHumi);
//		}
		
	}

	return res;
	
}

/*************************************************

Function:	SHT3X 开始读过程

Description: SHT3X_StartReadAccess()

*************************************************/
u8 SHT3X_StartReadAccess(void)
{
	u8 res;
	IIC_Start();
//	res=SHT_I2c_WriteByte(i2c_Addr<<1|0x01);
	IIC_Send_Byte(0x89);
	res=IIC_Wait_Ack();
	return res;
}

/*************************************************

Function:	SHT3X 开始写过程

Description: SHT3X_StartWriteAccess()

*************************************************/
u8 SHT3X_StartWriteAccess(void)
{
	u8 res;
	IIC_Start();
	
//	res=SHT_I2c_WriteByte(i2c_Addr<<1);
	IIC_Send_Byte(0x88);
	res=IIC_Wait_Ack();
//	res=SHT_I2c_WriteByte(0xe8);
	
	return res;
}

/*************************************************

Function:	SHT3X 写一个字节

Description: SHT_I2c_WriteByte()

*************************************************/
u8 SHT_I2c_WriteByte(u8 txByte)
{
	u8 res=1;
	u8 mask;
	for(mask = 0x80; mask>0;mask>>=1)
	{
		if((mask & txByte)==0)
		{
			IIC_SDA=0;
		}
		else
		{
			IIC_SDA=1;
		}
		delay_us(1);
		IIC_SCL=1;
		delay_us(5);
		IIC_SCL=0;
		delay_us(1);
	}
	SDA_IN();
	IIC_SDA=1;
	delay_us(6);
	IIC_SCL=1;
	delay_us(1);

	if(READ_SDA) res= 0;

	IIC_SCL=0;
	delay_us(20);
	return res;
}

/*************************************************

Function:	SHT3X 写一个命令

Description: SHT_I2c_WriteCMD()

*************************************************/
u8 SHT_I2c_WriteCMD(u16 command)
{
	u8 res=1;
	res=SHT_I2c_WriteByte(command>>8);
//	delay_us(20);
	if(res==1)
	{
		printf("123");
		res=SHT_I2c_WriteByte(command&0xff);
	}
	return res;	
}

/*************************************************

Function:	SHT3X 读一个字节

Description: SHT_I2c_ReadByte()

*************************************************/
u8 SHT_I2c_ReadByte(u8 *rxByte,u8 ack,u8 timeout)
{
	u8 res=1;
	u8 mask;
	*rxByte=0x00;
	IIC_SDA=1;
	for(mask = 0x80; mask>0;mask>>=1)
	{
         IIC_SCL=1;
		 delay_us(1);
		while(READ_SCL==0)
		{
			if(timeout--==0) return 0;
			delay_us(1000);
		}
		delay_us(3);
		if(READ_SDA) *rxByte |= mask;
		IIC_SCL=0;
		delay_us(1);
	}
	if(ack == 1) IIC_SDA=0;
	else         IIC_SDA=1;
	delay_us(1);
    IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
	IIC_SDA=1;
	delay_us(20);
	return res;
}

/*************************************************

Function:	SHT3X 读2个字节和CRC

Description: SHT_I2c_Read2Byte()

*************************************************/
u8 SHT_I2c_Read2Byte(u16 *data,u8 ack,u8 timeout)
{
	u8 bytes[2];
	u8 checksum;
	u8 res=1;
	res=SHT_I2c_ReadByte(&bytes[0],1,timeout);
	if(res==1)res=SHT_I2c_ReadByte(&bytes[1],1,0);
	if(res==1)res=SHT_I2c_ReadByte(&checksum,ack,0);
	if(res==1)
	{
		if(checksum==SHT3X_CalcCrc(bytes,2))
		{
			*data=(bytes[0]<<8)|bytes[1];
		}
		else  
		{   res=0;	}
	}
	return res;	
}

/*************************************************

Function:	SHT3X 计算温度

Description: SHT3X_CalcTemperature()

*************************************************/
float SHT3X_CalcTemperature(u16 rawValue)
{
	//T=-45+175*rawValue/(2^16-1)
	return 175.0f*(float)rawValue/65535.0f-45.0f;
}

/*************************************************

Function:	SHT3X 计算湿度

Description: SHT3X_CalcHumidity()

*************************************************/
float SHT3X_CalcHumidity(u16 rawValue)
{
	//T=100*rawValue/(2^16-1）
	return 100.0f*(float)rawValue/65535.0f;
}
