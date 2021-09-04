/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: SystemCtrl.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "SystemCtrl.h"
#include "Data.h"

#define vStatus_IDLE      1
#define vStatus_FRONT     2
#define vStatus_BACK      3
#define vStatus_LEFT      4
#define vStatus_RIGHT     5

u8 InitRunFlag = 0;
u8 Old_vStatus = 0;
u8 StatusMes = 0 ;
/*************************************************

Function:	×´Ì¬»ú¿ØÖÆ

Description: Status_vUpdate()

*************************************************/
void Status_vUpdate(u8 newStatus)
{
	
	switch(Old_vStatus)
	{
	
		case vStatus_IDLE:
		{
			if(Old_vStatus!= newStatus)
			{
				Old_vStatus = newStatus;
				InitRunFlag = 1;
			}
	
		}break;
		
		case vStatus_FRONT:
	    case vStatus_BACK:
		case vStatus_LEFT:
		case vStatus_RIGHT:
		{
			 if(Old_vStatus!=newStatus)
			{
				Old_vStatus = newStatus ;
			}
		}break;
	}
}






