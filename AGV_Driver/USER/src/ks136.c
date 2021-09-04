#include "ks136.h"
#include "Usart.h"
#include "Data.h"
#include "User_Conf.h"

//#define  Ks136Debug
#if 1
PROBE probe ;

/*************************************************

Function:	KS136_WriteOneByte()

Description:  KS136дһ���ֽ�

address Ƭѡ
register �Ĵ���
command ����
*************************************************/

void KS136_WriteOneByte(u8 address,u8 reg,u8  command)
{
	IIC_Start();
	IIC_Send_Byte(address); //����д����  Ҫд���豸��ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);//���͸ߵ�ַ		�豸�ϵ���һ����ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(command); //���͵͵�ַ	����
	IIC_Wait_Ack();
	IIC_Stop();//����һ��ֹͣ����
}

/*************************************************

Function:	KS136_ReadOneByte()

Description:  KS136��һ���ֽڣ����ض�������

address ��Ƭѡ
register ���Ĵ���
*************************************************/

u8 KS136_ReadOneByte(u8 address,u8 reg)
{
	u8 temp=0;
	IIC_Start();
	IIC_Send_Byte(address); //���͵͵�ַ		Ҫ�����豸��ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(reg); //���͵͵�ַ		���豸�ϵ���һ����ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(address + 1); //�������ģʽ
	IIC_Wait_Ack();
	delay_us(50); //���Ӵ˴���ͨ�ųɹ�������
	temp=IIC_Read_Byte(0); //���Ĵ��� 3  
	IIC_Stop();//����һ��ֹͣ����
	return temp;
}

/*************************************************

Function:	change_i2c_address

Description:  �޸�Ƭѡ

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

Description:  Ks136��ʼ��

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
					
					break;	//����ѭ��
				 }	
			}while(KS136_ReadOneByte(0xe8,8)!= 0xe0);//����ֵΪe8ʱ���������ų�ʼ���ɹ����������½�ȥ��ʱ��������
		
	if(readyCount>0) //��ʼ���ɹ�
	{
	    delay_ms(2000);//��ks136  �Լ�
		KS136_WriteOneByte(0XE8,0X02,0x72);	//��������
		delay_ms(80);
	}
	else
	{
	  faultFlag|= BIT6;  //ks136����
	}
}

/*************************************************

Function:	Single_SendRec(void)

Description:  ��̽ͷ�շ�һ��

*************************************************/
/*��̽ͷ�շ�һ��  ̽�����13cm~4.5m

	IICд  iic��ַ+�Ĵ�����ַ+����
	
	*����ֵ mm
*/
void Single_SendRec(void)
{

		u8 i;
		KS136_WriteOneByte(0XE8,0X02,0x10);		//̽ͷ1�շ�һ�壬13cm~4.5m  ������mmֵ
		delay_ms(50);	//�����������������ȡ
		probe.single_rec [0] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [0] <<= 8;
		probe.single_rec [0] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x18);		//̽ͷ2�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [1] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [1] <<= 8;
		probe.single_rec [1] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		
		KS136_WriteOneByte(0XE8,0X02,0x20);		//̽ͷ3�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [2] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [2] <<= 8;
		probe.single_rec [2] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x28);		//̽ͷ4�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [3] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [3] <<= 8;
		probe.single_rec [3] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x30);		//̽ͷ5�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [4] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [4] <<= 8;
		probe.single_rec [4] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x38);		//̽ͷ6�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [5] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [5] <<= 8;
		probe.single_rec [5] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x40);		//̽ͷ7�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [6] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [6] <<= 8;
		probe.single_rec [6] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x48);		//̽ͷ8�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [7] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [7] <<= 8;
		probe.single_rec [7] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
#if 0
		KS136_WriteOneByte(0XE8,0X02,0x50);		//̽ͷ9�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [8] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [8] <<= 8;
		probe.single_rec [8] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x58);		//̽ͷ10�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [9] = 0x00ff&KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [9] <<= 8;
		probe.single_rec [9] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x60);		//̽ͷ11�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [10] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [10] <<= 8;
		probe.single_rec [10] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
		
		KS136_WriteOneByte(0XE8,0X02,0x68);		//̽ͷ12�շ�һ�壬����mm
		delay_ms(50);
		probe.single_rec [11] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
		probe.single_rec [11] <<= 8;
		probe.single_rec [11] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
#endif
		
	#ifdef  Ks136Debug
	   for(i=0;i<8;i++)
	   {
		   printf("K136��%d·:%dmm\n",i+1,probe.single_rec[i]);
	   }
	#endif
//       delay_ms(10);
}
 /*
		̽ͷ 1 ����̽ͷ 12 ���գ����� us ֵ���ǳ�������̽ͷ 1 ������
		̽ͷ 12 ���յ��ķ���ʱ��  ����Ҫת��mm ����� 5.8

*/
void Double_SendRec(void)
{
	KS136_WriteOneByte(0XE8,0X02,0x01);		//̽ͷ1���ͣ�̽ͷ12���գ�����usֵ
	delay_ms(80);	//�����������������ȡ
	probe.double_rec [0] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
	probe.double_rec [0] <<= 8;
	probe.double_rec [0] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
	
	KS136_WriteOneByte(0XE8,0X02,0x02);		//̽ͷ2���ͣ�̽ͷ11���գ�����usֵ
	delay_ms(80);	//�����������������ȡ
	probe.double_rec [1] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
	probe.double_rec [1] <<= 8;
	probe.double_rec [1] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
	
	KS136_WriteOneByte(0XE8,0X02,0x03);		//̽ͷ3���ͣ�̽ͷ10���գ�����usֵ
	delay_ms(80);	//�����������������ȡ
	probe.double_rec [2] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
	probe.double_rec [2] <<= 8;
	probe.double_rec [2] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
	
	KS136_WriteOneByte(0XE8,0X02,0x04);		//̽ͷ4���ͣ�̽ͷ9���գ�����usֵ
	delay_ms(80);	//�����������������ȡ
	probe.double_rec [3] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
	probe.double_rec [3] <<= 8;
	probe.double_rec [3] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
	
	KS136_WriteOneByte(0XE8,0X02,0x05);		//̽ͷ5���ͣ�̽ͷ8���գ�����usֵ
	delay_ms(80);	//�����������������ȡ
	probe.double_rec [4] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
	probe.double_rec [4] <<= 8;
	probe.double_rec [4] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
	                  
	KS136_WriteOneByte(0XE8,0X02,0x06);		//̽ͷ6���ͣ�̽ͷ7���գ�����usֵ
	delay_ms(80);	//�����������������ȡ
	probe.double_rec [5] = KS136_ReadOneByte(0xe8, 0x02);	//��8λ
	probe.double_rec [5] <<= 8;
	probe.double_rec [5] += KS136_ReadOneByte(0xe8, 0x03);	//��8λ
	
}

u8 mode_flag = 1;	//1 ��̽ͷģʽ    2 ˫̽ͷģʽ
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
	u8 chksum = 0;	//У���
	u8 dataSum = 0;	//����̽ͷ���ݺ�
	switch (mode_flag)
	{
		case 1:		//�ϴ���̽ͷ���յ�������
		{
			for(i = 0;i<8;i++)
			{
				dataSum += probe.single_rec[i]>>8;
				dataSum += probe.single_rec[i];
			}
			
			USARTSendByte(0x55,USART1);		//��ʼ��
			USARTSendByte(0xAA,USART1);
			USARTSendByte(0x11,USART1);		//���ݳ���	16 +1 
			USARTSendByte(0x03,USART1);		//��������
			
			USARTSendByte(probe.single_rec[0],USART1);	//�ȷ�1̽ͷ��λ
			USARTSendByte(probe.single_rec[0]>>8,USART1);	//1̽ͷ��λ
			
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
			
			USARTSendByte(0x08,USART1);		//����������12��   �����ڽ��շ�ֻ��8������  ���Է�8������   
			
			chksum = 0X55 + 0XAA + 0X11 + 0X03 + dataSum + 0X08;
			USARTSendByte(chksum,USART1);		//����У���
			
//				send_flag = 0;	//������ɱ�־λ��0
//				memset(probe.single_rec,0,12);	//������0
			
		}
		break;
		case 2:		//�ϴ�˫̽ͷ���յ�������
		{
			
			for(i = 0;i<6;i++)
			{
				dataSum += probe.double_rec[i];
			}
			USARTSendByte(0x55,USART1);		//��ʼ��
			USARTSendByte(0xAA,USART1);
			USARTSendByte(0x09,USART1);		//���ݳ���	6��u16
			USARTSendByte(0x03,USART1);		//��������
			
			
			temp  =(double)probe.double_rec[0]/5.8;	//��usת��mm ��Ҫ����5.8
			USARTSendByte(temp>>8,USART1);	//�ȷ�1 12̽ͷ��λ
			USARTSendByte(temp,USART1);	//̽ͷ��λ
			                    
			temp  =(double )probe.double_rec[1]/5.8;	//��usת��mm ��Ҫ����5.8
			USARTSendByte(temp>>8,USART1);	//�ȷ�2 11̽ͷ��λ
			USARTSendByte(temp,USART1);	//̽ͷ��λ
			                     
			temp  =(double  )probe.double_rec[2]/5.8;	//��usת��mm ��Ҫ����5.8
			USARTSendByte(temp>>8,USART1);	//�ȷ�3 10̽ͷ��λ
			USARTSendByte(temp,USART1);	//̽ͷ��λ
			                     
			temp  =(double  )probe.double_rec[3]/5.8;	//��usת��mm ��Ҫ����5.8
			USARTSendByte(temp>>8,USART1);	//�ȷ�4 9̽ͷ��λ
			USARTSendByte(temp,USART1);	//̽ͷ��λ
			                    
			temp  =(double )probe.double_rec[4]/5.8;	//��usת��mm ��Ҫ����5.8
			USARTSendByte(temp>>8,USART1);	//�ȷ�5 8̽ͷ��λ
			USARTSendByte(temp,USART1);	//̽ͷ��λ
			                   
			temp  =(double  )probe.double_rec[5]/5.8;	//��usת��mm ��Ҫ����5.8
			USARTSendByte(temp>>8,USART1);	//�ȷ�6 7̽ͷ��λ
			USARTSendByte(temp,USART1);	//̽ͷ��λ
			
			chksum = 0X55 + 0XAA + 0x09 + 0X03 + dataSum + 0X0C;
			USARTSendByte(chksum,USART1);
			
			
//			send_flag = 0;	//������ɱ�־λ��0
//			memset(probe.double_rec,0,12);	//������0

		}
		break;
		default :
			break;
		
	}
}
#endif


