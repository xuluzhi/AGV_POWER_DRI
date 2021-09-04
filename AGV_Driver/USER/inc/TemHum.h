/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: Temhum.h

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#ifndef __TEMHUM_H
#define __TEMHUM_H

#include "stm32f4xx.h"


extern float temperature;
extern float humidity;
/* º¯Êý½Ó¿Ú */
void SHT3X_Init(u8 i2cAddress);
u8 SHT_I2c_WriteByte(u8 txByte);
u8 SHT3X_StartWriteAccess(void);
u8 SHT3X_StartReadAccess(void);
u8 SHT_I2c_Read2Byte(u16 *data,u8 ack,u8 timeout);
u8 SHT_I2c_ReadByte(u8 *rxByte,u8 ack,u8 timeout);
float SHT3X_CalcTemperature(u16 rawValue);
float SHT3X_CalcHumidity(u16 rawValue);
u8 SHT3X_GetTempAndHumi(float *temp,float *humidity,u8 timeout);
u8 SHT_I2c_WriteCMD(u16 command);














#endif
