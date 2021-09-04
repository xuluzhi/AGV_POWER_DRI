/************************************************************
Copyright (C), 2014-2999, Tech. Co., Ltd.
------------------------------------------------------------
FileName: User_Conf.c

Author: Yan Hong

Version: 1.0.0 

Date: 2021.02.03

Description: 

***********************************************************/
#include "User_Conf.h"
#include "stdio.h"
int fputc(int ch, FILE *f)
{
/* ����һ���ֽ����ݵ�USART1 */
USART_SendData(USART1,  (u8)ch);
/* �ȴ�������� */
while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
return (ch);

}