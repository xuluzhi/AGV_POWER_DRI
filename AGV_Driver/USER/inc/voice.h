#ifndef __VOICE_H
#define __VOICE_H
#include "stm32f4xx.h"
#include "Data.h"

/* º¯ÊýÉùÃ÷ */
void SendCommand_SYN6658(u8 SendCmd,u16 num ,u8* str);
void SelectSYN(u16 num ,u8* str);
void SYN_ctrl(RobotDef *robot);
u8 SetVoi_SYN6658(u8 voi);
u8 SetVol_SYN6658(u8 vol);









#endif
