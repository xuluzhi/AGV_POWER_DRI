#ifndef _KS136_H_
#define _KS136_H_

#include "sysz.h"
#include "iic.h"
#include "delay.h"



typedef struct 
{
	u16 double_rec[6] ;		//双探头返回值是us  若需要转换mm 需除以 5.8
	u16 single_rec[12];		//接收单探头数据
	
}PROBE;

extern u8 mode_flag; //1 单探头模式    2 双探头模式

extern PROBE probe ;

void Ks136_Init(void);
void Single_SendRec(void);
void Double_SendRec(void);
void Change_DetectMode (void);
void Usrat1_SendKs136Data(void);
void KS136_WriteOneByte(u8 daddr,u8 addr,u8 data);
u8 KS136_ReadOneByte(u8 daddr,u8 addr);	 
void change_i2c_address(unsigned char addr_old, unsigned char addr_new);

#endif
