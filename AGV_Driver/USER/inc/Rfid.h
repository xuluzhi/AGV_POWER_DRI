#ifndef __RFID_H
#define __RFID_H
#include "stm32f4xx.h"

typedef struct
{
  u16 shelf_ID;
  u16 factor_ID;
  u16 other;//Ô¤Áô
}Rfid_Epc;
typedef struct 
{
	u16 pc;
	Rfid_Epc epc;
	u8 (*getChecksum)(void);
}RfidDef;

extern void Uart3_ReceiveIQR(uint8_t Buffer);
extern void dataCtrl_RFID(RfidDef* RfidData);
u8 RFID_init(void);
void RFID_readEPC(void);
void RFID_writeEPC(RfidDef* RfidData);













#endif
