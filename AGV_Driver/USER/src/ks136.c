#include "ks136.h"
#include "Usart.h"
#include "Data.h"
#include "User_Conf.h"

//#define  Ks136Debug
#if 1
PROBE probe ;

/*************************************************

Function:	KS136_WriteOneByte()

Description:  KS136写一个字节

address 片选
register 寄存器
command 命令
*************************************************/

void KS136_WriteOneByte(u8 address,u8 reg,u8  command)
{
	IIC_Start();
	IIC_Send_Byte(address); //发送写命令  要写的设备地址
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);//发送高地址		设备上的哪一个地址
	IIC_Wait_Ack();
	IIC_Send_Byte(command); //发送低地址	命令
	IIC_Wait_Ack();
	IIC_Stop();//产生一个停止条件
}

/*************************************************

Function:	KS136_ReadOneByte()

Description:  KS136读一个字节，返回读到内容

address ：片选
register ：寄存器
*************************************************/

u8 KS136_ReadOneByte(u8 address,u8 reg)
{
	u8 temp=0;
	IIC_Start();
	IIC_Send_Byte(address); //发送低地址		要读的设备地址
	IIC_Wait_Ack();
	IIC_Send_Byte(reg); //发送低地址		读设备上的哪一个地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(address + 1); //进入接收模式
	IIC_Wait_Ack();
	delay_us(50); //增加此代码通信成功！！！
	temp=IIC_Read_Byte(0); //读寄存器 3  
	IIC_Stop();//产生一个停止条件
	return temp;
}

/*************************************************

Function:	change_i2c_address

Description:  修改片选

*************************************************/
void change_i2c_address(unsigned char addr_old, unsigned char addr_new)
// addr_old is the address now, addr_new will be the new address
{ //that you want change to
	delay_ms(2000); // Protect the eeprom ,you can delete this sentence
	KS136_WriteOneByte(addr_old,2,0x9a);
	delay_ms(1);
	KS136_WriteOneByte(addr_old,2,0x92);
	delay_ms(1);
	KS136_WriteOneByte(addr_old,2,0x9e);
	delay_ms(1);
	KS136_WriteOneByte(addr_old,2, addr_new);
	delay_ms(500); //Protect the eeprom, you can delete this sentence
}

/*************************************************

Function:	Ks136_Init()

Description:  Ks136初始化

*************************************************/
void Ks136_Init(void)
{
	u8 readyCount = 10;
			do
			{
				
				IIC_SCL = 0;
				IIC_SDA = 0;
				delay_us(10);
				IIC_SCL = 1;
				IIC_SDA = 1;	
				if(--readyCount==0)
				{
					
					break;	//跳出循环
				 }	
			}while(KS136_ReadOneByte(0xe8,8)!= 0xe0);//返回值为e8时，超声波才初始化成功，否则重新进去把时钟线拉低
		
	if(readyCount>0) //初始化成功
	{
	    delay_ms(2000);//给ks136  自检
		KS136_WriteOneByte(0XE8,0X02,0x72);	//三级降噪
		delay_ms(80);
	}
	else
	{
	  faultFlag|= BIT6;  //ks136故障
	}
}

/*************************************************

Function:	Single_SendRec(void)

Description:  单探头收发一体

*************************************************/
/*单探头收发一体  探测距离13cm~4.5m

	IIC写  iic地址+寄存器地址+命令
	
	*返回值 mm
*/
void Single_SendRec(void)
{

		u8 i;
		KS136_WriteOneByte(0XE8,0X02,0x10);		//探头1收发一体，13cm~4.5m  ，返回mm值
		delay_ms(50);	//发送完命令不能立即读取
		probe.single_rec [0] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [0] <<= 8;
		probe.single_rec [0] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x18);		//探头2收发一体，返回mm
		delay_ms(50);
		probe.single_rec [1] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [1] <<= 8;
		probe.single_rec [1] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		
		KS136_WriteOneByte(0XE8,0X02,0x20);		//探头3收发一体，返回mm
		delay_ms(50);
		probe.single_rec [2] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [2] <<= 8;
		probe.single_rec [2] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x28);		//探头4收发一体，返回mm
		delay_ms(50);
		probe.single_rec [3] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [3] <<= 8;
		probe.single_rec [3] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x30);		//探头5收发一体，返回mm
		delay_ms(50);
		probe.single_rec [4] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [4] <<= 8;
		probe.single_rec [4] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x38);		//探头6收发一体，返回mm
		delay_ms(50);
		probe.single_rec [5] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [5] <<= 8;
		probe.single_rec [5] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x40);		//探头7收发一体，返回mm
		delay_ms(50);
		probe.single_rec [6] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [6] <<= 8;
		probe.single_rec [6] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x48);		//探头8收发一体，返回mm
		delay_ms(50);
		probe.single_rec [7] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [7] <<= 8;
		probe.single_rec [7] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
#if 0
		KS136_WriteOneByte(0XE8,0X02,0x50);		//探头9收发一体，返回mm
		delay_ms(50);
		probe.single_rec [8] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [8] <<= 8;
		probe.single_rec [8] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x58);		//探头10收发一体，返回mm
		delay_ms(50);
		probe.single_rec [9] = 0x00ff&KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [9] <<= 8;
		probe.single_rec [9] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x60);		//探头11收发一体，返回mm
		delay_ms(50);
		probe.single_rec [10] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [10] <<= 8;
		probe.single_rec [10] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
		
		KS136_WriteOneByte(0XE8,0X02,0x68);		//探头12收发一体，返回mm
		delay_ms(50);
		probe.single_rec [11] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
		probe.single_rec [11] <<= 8;
		probe.single_rec [11] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
#endif
		
	#ifdef  Ks136Debug
	   for(i=0;i<8;i++)
	   {
		   printf("K136第%d路:%dmm\n",i+1,probe.single_rec[i]);
	   }
	#endif
//       delay_ms(10);
}
 /*
		探头 1 发射探头 12 接收，返回 us 值，是超声波从探头 1 发出到
		探头 12 接收到的飞行时间  若需要转换mm 需除以 5.8

*/
void Double_SendRec(void)
{
	KS136_WriteOneByte(0XE8,0X02,0x01);		//探头1发送，探头12接收，返回us值
	delay_ms(80);	//发送完命令不能立即读取
	probe.double_rec [0] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
	probe.double_rec [0] <<= 8;
	probe.double_rec [0] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
	
	KS136_WriteOneByte(0XE8,0X02,0x02);		//探头2发送，探头11接收，返回us值
	delay_ms(80);	//发送完命令不能立即读取
	probe.double_rec [1] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
	probe.double_rec [1] <<= 8;
	probe.double_rec [1] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
	
	KS136_WriteOneByte(0XE8,0X02,0x03);		//探头3发送，探头10接收，返回us值
	delay_ms(80);	//发送完命令不能立即读取
	probe.double_rec [2] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
	probe.double_rec [2] <<= 8;
	probe.double_rec [2] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
	
	KS136_WriteOneByte(0XE8,0X02,0x04);		//探头4发送，探头9接收，返回us值
	delay_ms(80);	//发送完命令不能立即读取
	probe.double_rec [3] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
	probe.double_rec [3] <<= 8;
	probe.double_rec [3] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
	
	KS136_WriteOneByte(0XE8,0X02,0x05);		//探头5发送，探头8接收，返回us值
	delay_ms(80);	//发送完命令不能立即读取
	probe.double_rec [4] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
	probe.double_rec [4] <<= 8;
	probe.double_rec [4] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
	                  
	KS136_WriteOneByte(0XE8,0X02,0x06);		//探头6发送，探头7接收，返回us值
	delay_ms(80);	//发送完命令不能立即读取
	probe.double_rec [5] = KS136_ReadOneByte(0xe8, 0x02);	//高8位
	probe.double_rec [5] <<= 8;
	probe.double_rec [5] += KS136_ReadOneByte(0xe8, 0x03);	//低8位
	
}

u8 mode_flag = 1;	//1 单探头模式    2 双探头模式
void Change_DetectMode (void)
{
	switch (mode_flag)
	{
		case 1:
		{
			Single_SendRec();
		}
		break;
		case 2:
		{
			Double_SendRec();
		}
		break;
		default :
			break;
	}
	
}

void Usrat1_SendKs136Data(void)
{
	u8 i =0;
	u16 temp = 0;
	u8 chksum = 0;	//校验和
	u8 dataSum = 0;	//接收探头数据和
	switch (mode_flag)
	{
		case 1:		//上传单探头接收到的数据
		{
			for(i = 0;i<8;i++)
			{
				dataSum += probe.single_rec[i]>>8;
				dataSum += probe.single_rec[i];
			}
			
			USARTSendByte(0x55,USART1);		//起始码
			USARTSendByte(0xAA,USART1);
			USARTSendByte(0x11,USART1);		//数据长度	16 +1 
			USARTSendByte(0x03,USART1);		//发送命令
			
			USARTSendByte(probe.single_rec[0],USART1);	//先发1探头低位
			USARTSendByte(probe.single_rec[0]>>8,USART1);	//1探头高位
			
			USARTSendByte(probe.single_rec[1],USART1);	
			USARTSendByte(probe.single_rec[1]>>8,USART1);	
			
			USARTSendByte(probe.single_rec[2],USART1);	
			USARTSendByte(probe.single_rec[2]>>8,USART1);	
			
			USARTSendByte(probe.single_rec[3],USART1);	
			USARTSendByte(probe.single_rec[3]>>8,USART1);	
			
			USARTSendByte(probe.single_rec[4],USART1);	
			USARTSendByte(probe.single_rec[4]>>8,USART1);	
			
			USARTSendByte(probe.single_rec[5],USART1);	
			USARTSendByte(probe.single_rec[5]>>8,USART1);	
			
			USARTSendByte(probe.single_rec[6],USART1);	
			USARTSendByte(probe.single_rec[6]>>8,USART1);	
			
			USARTSendByte(probe.single_rec[7],USART1);	
			USARTSendByte(probe.single_rec[7]>>8,USART1);	
			
			USARTSendByte(0x08,USART1);		//超声波个数12个   但由于接收方只接8个数据  所以发8个数据   
			
			chksum = 0X55 + 0XAA + 0X11 + 0X03 + dataSum + 0X08;
			USARTSendByte(chksum,USART1);		//发送校验和
			
//				send_flag = 0;	//接收完成标志位清0
//				memset(probe.single_rec,0,12);	//数据清0
			
		}
		break;
		case 2:		//上传双探头接收到的数据
		{
			
			for(i = 0;i<6;i++)
			{
				dataSum += probe.double_rec[i];
			}
			USARTSendByte(0x55,USART1);		//起始码
			USARTSendByte(0xAA,USART1);
			USARTSendByte(0x09,USART1);		//数据长度	6个u16
			USARTSendByte(0x03,USART1);		//发送命令
			
			
			temp  =(double)probe.double_rec[0]/5.8;	//将us转换mm 需要除以5.8
			USARTSendByte(temp>>8,USART1);	//先发1 12探头高位
			USARTSendByte(temp,USART1);	//探头低位
			                    
			temp  =(double )probe.double_rec[1]/5.8;	//将us转换mm 需要除以5.8
			USARTSendByte(temp>>8,USART1);	//先发2 11探头高位
			USARTSendByte(temp,USART1);	//探头低位
			                     
			temp  =(double  )probe.double_rec[2]/5.8;	//将us转换mm 需要除以5.8
			USARTSendByte(temp>>8,USART1);	//先发3 10探头高位
			USARTSendByte(temp,USART1);	//探头低位
			                     
			temp  =(double  )probe.double_rec[3]/5.8;	//将us转换mm 需要除以5.8
			USARTSendByte(temp>>8,USART1);	//先发4 9探头高位
			USARTSendByte(temp,USART1);	//探头低位
			                    
			temp  =(double )probe.double_rec[4]/5.8;	//将us转换mm 需要除以5.8
			USARTSendByte(temp>>8,USART1);	//先发5 8探头高位
			USARTSendByte(temp,USART1);	//探头低位
			                   
			temp  =(double  )probe.double_rec[5]/5.8;	//将us转换mm 需要除以5.8
			USARTSendByte(temp>>8,USART1);	//先发6 7探头高位
			USARTSendByte(temp,USART1);	//探头低位
			
			chksum = 0X55 + 0XAA + 0x09 + 0X03 + dataSum + 0X0C;
			USARTSendByte(chksum,USART1);
			
			
//			send_flag = 0;	//接收完成标志位清0
//			memset(probe.double_rec,0,12);	//数据清0

		}
		break;
		default :
			break;
		
	}
}
#endif


