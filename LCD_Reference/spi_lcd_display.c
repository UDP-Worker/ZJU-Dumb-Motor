#include <stdio.h>
#include "stm32f10x.h"
#include "spi_lcd_display.h"
#include "sys_cfg.h"
#include "delay.h"
#include "global_params.h"

void SendByteLCD(uint8_t WLCDData)
{
//		uint8_t i;
//		for(i=0;i<8;i++)
//		{
//				if((WLCDData<<i)&0x80)RW=1;
//				else RW=0;
//				E=0;
//				E=1 ;
//		}
#ifdef _SPI_MODE_		
		SPI1->DR = WLCDData;	//д���ַ���LCD12864  
		while(SPI1->SR&SPI_I2S_FLAG_BSY){};
		//systick_delay_us(2000);
#else
		uint8_t i;
		for(i=0;i<8;i++)
		{
				if((WLCDData<<i)&0x80)
					SPI_DAT_HIGH;
				else
					SPI_DAT_LOW;
				lcd_serial_delay(2);
				SPI_CLK_LOW;
				lcd_serial_delay(2);				
				SPI_CLK_HIGH ;
		}
#endif
		//systick_delay_us(72);		
}

void write_data(uint8_t Dispdata)
{
			SPI_CS_DISABLE;
			systick_delay_us(1);
			SPI_CS_ENABLE;
			SendByteLCD(LCD_WR_DAT);
			SendByteLCD(Dispdata & 0xF0);
			SendByteLCD((Dispdata << 4) & 0xF0);
			//systick_delay_us(72);
}

/********************************************************************
* ���� : write_com()
* ���� : д����ָ��
* ���� : cmdcode
* ��� : ��
***********************************************************************/
void write_com(uint8_t cmdcode)
{
			SPI_CS_DISABLE;
			systick_delay_us(2);
			SPI_CS_ENABLE;
			SendByteLCD(LCD_WR_COM);
			SendByteLCD(cmdcode & 0xF0);
			SendByteLCD((cmdcode << 4) & 0xF0);
			//systick_delay_us(72);
}

// ************Һ���˿ڳ�ʼ��**************
void lcd_port_init(void)
{ //����
			LCD_PSB_LOW;	//0�����ô��нӿڷ�ʽ����Һ����ʾ
			LCD_RST_LOW;
			systick_delay_us(90000);
			LCD_RST_HIGH;
			systick_delay_us(90000);
			SPI_CS_DISABLE;
			systick_delay_us(10);
			SPI_CS_ENABLE;
			write_com(0x30); //����ָ�
			systick_delay_us(72);//4.6mS
			write_com(0x06); //��ʼ���趨���������
//			write_com(0x06); //��ʼ���趨���������
			systick_delay_us(72);//72uS
			write_com(0x01); //�����ʾ
			systick_delay_us(4600);//4.6mS	
			write_com(0x0C); //��ʾ״̬���أ�������ʾ���������ʾ�أ������ʾ���׹�
			systick_delay_us(4600);//4.6mS
			write_com(0x02); //��ַ����
			systick_delay_us(4600);//4.6mS
}

/********************************************************************
* ���� : hzkdis()
* ���� : ��ʾ�ַ���
* ���� : *s
* ��� : ��
***********************************************************************/
//void hzkdis(uint8_t code *str)
//{  
//		while(*s > 0)
//    { 
//				write_data(*s);
//				s++;
//				systick_delay_us(50);
//    }
//}

/******************�ַ��ͺ�����ʾ****************/ 
void Disp(uint8_t y,uint8_t x,uint8_t i,uint8_t *z)   
{     
			uint8_t Address;    
			//Y�жϵڼ���,X�жϵڼ���,0x80ΪҺ���г�ʼ��ַ  
			write_com(0x30);//����ָ�����
			switch(y)	//GDRAMˮƽλַ:������ʾ����
			{
				case 0x01:	//0x80-0x87
					Address=0x80+x;
					break;
				case 0x02:	//0x90-0x97
					Address=0x90+x;
					break;
				case 0x03:	//0x88-8F
					Address=0x88+x;
					break;
				case 0x04:	//0x98-0x9F
					Address=0x98+x;
					break;
				default:
					Address=0x80+x;
					break;
			}
			write_com(Address);	//д���ַ���LCD12864  
			while(i)       			//д����ʾ���ݵĴ�С  
			{    
					write_data(*(z++));	//д����ʾ���ݵ�LCD12864    
					i--;   
			}   
} 

	
/*********�Զ����ַ�1��16X16��д��CGRAM**********/
void WRCGRAM1(uchar addr)//���»���
{
			uchar i;
			write_com(0x30);//����ָ�����
			for(i=0;i<2;)
			{
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0xff);
					write_data(0x00);
					i++;
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0x00);
					write_data(0x00);
					i++;
			}
			for(i=2;i<16;i++)
			{
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0x00);
					write_data(0x00);
			}
}

/***********�Զ����ַ�2��16X16��д��CGRAM**********/
void WRCGRAM2(uchar addr)//˫�»���
{
			uchar i;
			write_com(0x30);//����ָ�����
			for(i=0;i<4;)
			{
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0xff);
					write_data(0x00);
					i++;
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0x00);
					write_data(0x00);
					i++;
			}
			for(i=4;i<16;i++)
			{
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0x00);
					write_data(0x00);
			}
}

/**********�Զ����ַ�3��16X16��д��CGRAM************/
void WRCGRAM3(uchar addr)//���»��߼ӵ�
{
			uchar i;
			write_com(0x30);//����ָ�����
			for(i=0;i<2;)
			{
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0xff);
					write_data(0x00);
					i++;
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0x00);
					write_data(0x00);
					i++;
			}
			for(i=2;i<6;)
			{
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0x00);
					write_data(0x00);
					i++;
			}
			write_com(addr+6);write_data(0x18);write_data(0x00);
			write_com(addr+7);write_data(0x3c);write_data(0x00);
			write_com(addr+8);write_data(0x3c);write_data(0x00);
			write_com(addr+9);write_data(0x18);write_data(0x00);
			for(i=10;i<16;i++)
			{
					write_com(addr+i); //�趨CGRAM��ַ
					write_data(0x00);
					write_data(0x00);
			}
}

/*************�Զ����ַ�4��16X16��д��CGRAM**********/
void WRCGRAM4(uchar addr)//˫�»��߼ӵ�
{
			uchar i;
			write_com(0x34);//����ָ�����
			write_com(0x36);//����ָ��,8λ����,������ͼ����(ON)
			systick_delay_us(72000);
			for(i=0;i<4;)
			{
				write_com(addr+i); //�趨CGRAM��ַ
				write_data(0xff);
				write_data(0x00);
				i++;
				write_com(addr+i); //�趨CGRAM��ַ
				write_data(0x00);
				write_data(0x00);
				i++;
			}
			write_com(addr+4);write_data(0x00);write_data(0x00);
			write_com(addr+5);write_data(0x00);write_data(0x00);
			write_com(addr+6);write_data(0x18);write_data(0x00);
			write_com(addr+7);write_data(0x3c);write_data(0x00);
			write_com(addr+8);write_data(0x3c);write_data(0x00);
			write_com(addr+9);write_data(0x18);write_data(0x00);
			for(i=10;i<16;i++)
			{
				write_com(addr+i); //�趨CGRAM��ַ
				write_data(0x00);
				write_data(0x00);
			}
}



//��ʾ�����ַ���(��ʾ�������16*8 ����)
void ShowQQChar(uint8_t addr,uint8_t *english,uint8_t count)
{
			uint8_t i;
			write_com(0x30);//����ָ�����
			write_com(addr);//�趨DDRAM ��ַ
			for(i=0;i<count;)
			{
					write_data(english[i*2]);
					write_data(english[i*2+1]);
					i++;
			}
}	
	
//��ʾ�����ִ�(����ַ�)
void ShowNUMChar(uint8_t addr,uint8_t i,uint8_t count)
{
			uint8_t j;
			for(j=0;j<count;)
			{
					write_com(addr);//�趨DDRAM ��ַ
					write_data(i+j);
					j++;
					write_data(i+j);
					addr++;
					j++;
			}
}	
	
//�Զ����ַ�д��CGRAM
void WRCGRAM(uint8_t data1,uint8_t data2,uint8_t addr)
{
			uint8_t i;
			write_com(0x34);//����ָ�����
			write_com(0x36);//����ָ��,8λ����,������ͼ����(ON)
			for(i=0;i<16;)
			{
					write_com(addr+i); //�趨CGRAM ��ַ
					write_data(data1);
					write_data(data1);
					i++;
					write_com(addr+i); //�趨CGRAM ��ַ
					write_data(data2);
					write_data(data2);
					i++;
			}
}	
	
//��ʾ�Զ�����ַ�,��������ַ�����ȫ��16*16
void ShowCGChar(uint8_t addr,uint8_t i)
{
			uint8_t j;
			for(j=0;j<0x20;)
			{
					write_com(addr+j); //�趨DDRAM ��ַ
					write_data(0x00);
					write_data(i);
					j++;
			}
}	

void Lcd_Clear(uint16_t addr,uint8_t dat)
{
			uint8_t x_addr,y_addr,i;
//			x_addr = addr>>4;//����16
//			y_addr = addr%16;//����16������	
	
			//write_com(0x80+y_addr);//�趨��ֱ(��)��ַAC6~AC0
			write_com(0x80+addr);//X ����
			//write_data(dat>>8);
			for(x_addr=0;x_addr<16;x_addr++)
			write_data(dat);
			systick_delay_us(20);
}	


//void Lcd_Clear(uint16_t wdat)
//{
//			uint8_t j,i;
//			write_com(0x01);//�α��λ		
//			systick_delay_us(4600);//4.6mS
//			write_com(0x36);//����ָ�����
//			systick_delay_us(4600);//4.6mS
//			write_com(0x03);//����ָ�-����ֱ����ַ
//			systick_delay_us(4600);//4.6mS
//			for(j=0;j<0x20;j++)
//			{
//					write_com(0x80+j);//�趨��ֱ(��)��ַAC6~AC0
//					write_com(0x80);//X ����
//					for(i=0;i<0x10;i++)//	
//					{
//						write_data(wdat>>8);
//						write_data(wdat);
//					}
//			}
//			systick_delay_ms(20);
//}	

//void CLEARGDRAM(uint16_t wdat)
//{
//		uint8_t j;
//		uint8_t i;
//		write_com(0x34);
//		write_com(0x36);
//		for(j=0;j<32;j++)
//		{
//				write_com(0x80+j);
//				write_com(0x80);//X ����
//				for(i=0;i<32;i++)//
//				{
//					write_data(wdat);
//				}
//		}
//}

void CLEARGDRAM(uint16_t wdat)
{
			uint8_t j;
			uint16_t i;
			write_com(0x01);//�α��λ		
			systick_delay_us(4600);//4.6mS
			write_com(0x36);//����ָ��,8λ����,������ͼ����(ON)
//			write_com(0x03);//����ָ�-����ֱ����ַ
			systick_delay_us(4600);
			for(j=0;j<0x20;j++)
			{
					write_com(0x80+j);//�趨��ֱ(��)��ַAC6~AC0
					write_com(0x80);//X ����
					for(i=0;i<0x10;i++)//	
					{
						write_data(wdat>>8);
						write_data(wdat);
						systick_delay_us(20);
						//systick_delay_ms(20);
					}
			}
}	





//void CLEARGDRAM(uint16_t wdat)
//{
//			uint8_t j,k;
//			uint16_t i;
//			write_com(0x01);//�α��λ		
//			systick_delay_us(4600);//4.6mS
//			write_com(0x34);//����ָ�����
//			write_com(0x36);//����ָ��,8λ����,������ͼ����(ON)
//			systick_delay_us(4600);
//	
//			for(j=0;j<0x10;j++)
//			{
//					for(i=0;i<0x10;i++)//	
//					{
//						write_com(0x80+i);//��ֱ���꣨�е�ַ��ֱ��ַ Y����ַAC6~AC0
//						write_com(0x80+j);//Ȼ��дˮƽ���꣨�е�ַ��ˮƽ��ַ X��
//						write_data(wdat>>8);
//						write_data(wdat);
//					}
//			}
//			for(j=0;j<0x10;j++)
//			{
//					for(i=10;i<0x20;i++)//	
//					{
//						write_com(0x80+i);//��ֱ���꣨�е�ַ��ֱ��ַ Y����ַAC6~AC0
//						write_com(0x80+j);//Ȼ��дˮƽ���꣨�е�ַ��ˮƽ��ַ X��
//						write_data(wdat>>8);
//						write_data(wdat);
//					}
//			}
////	
////			for(j=0;j<0x20;j++)
////			{
////					write_com(0x80+j);//�趨��ֱ(��)��ַAC6~AC0
////					write_com(0x80);//X ����
////					for(i=0;i<0x10;i++)//	
////					{
////						write_data(wdat>>8);
////						write_data(wdat);
////					}
////			}
////			k += 4;
//			systick_delay_ms(20);
//}	





void lcd_serial_delay(u32 nus)
{		
			while (nus--){}
}
