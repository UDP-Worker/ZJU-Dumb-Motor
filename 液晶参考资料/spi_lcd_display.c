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
		SPI1->DR = WLCDData;	//写入地址命令到LCD12864  
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
* 名称 : write_com()
* 功能 : 写串口指令
* 输入 : cmdcode
* 输出 : 无
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

// ************液晶端口初始化**************
void lcd_port_init(void)
{ //串口
			LCD_PSB_LOW;	//0：采用串行接口方式进行液晶显示
			LCD_RST_LOW;
			systick_delay_us(90000);
			LCD_RST_HIGH;
			systick_delay_us(90000);
			SPI_CS_DISABLE;
			systick_delay_us(10);
			SPI_CS_ENABLE;
			write_com(0x30); //基本指令集
			systick_delay_us(72);//4.6mS
			write_com(0x06); //启始点设定：光标右移
//			write_com(0x06); //启始点设定：光标右移
			systick_delay_us(72);//72uS
			write_com(0x01); //清除显示
			systick_delay_us(4600);//4.6mS	
			write_com(0x0C); //显示状态开关：整体显示开，光标显示关，光标显示反白关
			systick_delay_us(4600);//4.6mS
			write_com(0x02); //地址归零
			systick_delay_us(4600);//4.6mS
}

/********************************************************************
* 名称 : hzkdis()
* 功能 : 显示字符串
* 输入 : *s
* 输出 : 无
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

/******************字符和汉字显示****************/ 
void Disp(uint8_t y,uint8_t x,uint8_t i,uint8_t *z)   
{     
			uint8_t Address;    
			//Y判断第几行,X判断第几列,0x80为液晶行初始地址  
			write_com(0x30);//基本指令集动作
			switch(y)	//GDRAM水平位址:汉字显示坐标
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
			write_com(Address);	//写入地址命令到LCD12864  
			while(i)       			//写入显示数据的大小  
			{    
					write_data(*(z++));	//写入显示数据到LCD12864    
					i--;   
			}   
} 

	
/*********自定义字符1（16X16）写入CGRAM**********/
void WRCGRAM1(uchar addr)//单下划线
{
			uchar i;
			write_com(0x30);//基本指令集动作
			for(i=0;i<2;)
			{
					write_com(addr+i); //设定CGRAM地址
					write_data(0xff);
					write_data(0x00);
					i++;
					write_com(addr+i); //设定CGRAM地址
					write_data(0x00);
					write_data(0x00);
					i++;
			}
			for(i=2;i<16;i++)
			{
					write_com(addr+i); //设定CGRAM地址
					write_data(0x00);
					write_data(0x00);
			}
}

/***********自定义字符2（16X16）写入CGRAM**********/
void WRCGRAM2(uchar addr)//双下划线
{
			uchar i;
			write_com(0x30);//基本指令集动作
			for(i=0;i<4;)
			{
					write_com(addr+i); //设定CGRAM地址
					write_data(0xff);
					write_data(0x00);
					i++;
					write_com(addr+i); //设定CGRAM地址
					write_data(0x00);
					write_data(0x00);
					i++;
			}
			for(i=4;i<16;i++)
			{
					write_com(addr+i); //设定CGRAM地址
					write_data(0x00);
					write_data(0x00);
			}
}

/**********自定义字符3（16X16）写入CGRAM************/
void WRCGRAM3(uchar addr)//单下划线加点
{
			uchar i;
			write_com(0x30);//基本指令集动作
			for(i=0;i<2;)
			{
					write_com(addr+i); //设定CGRAM地址
					write_data(0xff);
					write_data(0x00);
					i++;
					write_com(addr+i); //设定CGRAM地址
					write_data(0x00);
					write_data(0x00);
					i++;
			}
			for(i=2;i<6;)
			{
					write_com(addr+i); //设定CGRAM地址
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
					write_com(addr+i); //设定CGRAM地址
					write_data(0x00);
					write_data(0x00);
			}
}

/*************自定义字符4（16X16）写入CGRAM**********/
void WRCGRAM4(uchar addr)//双下划线加点
{
			uchar i;
			write_com(0x34);//扩充指令集动作
			write_com(0x36);//扩充指令,8位数据,开启绘图开关(ON)
			systick_delay_us(72000);
			for(i=0;i<4;)
			{
				write_com(addr+i); //设定CGRAM地址
				write_data(0xff);
				write_data(0x00);
				i++;
				write_com(addr+i); //设定CGRAM地址
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
				write_com(addr+i); //设定CGRAM地址
				write_data(0x00);
				write_data(0x00);
			}
}



//显示数组字符串(显示半宽字型16*8 点阵)
void ShowQQChar(uint8_t addr,uint8_t *english,uint8_t count)
{
			uint8_t i;
			write_com(0x30);//基本指令集动作
			write_com(addr);//设定DDRAM 地址
			for(i=0;i<count;)
			{
					write_data(english[i*2]);
					write_data(english[i*2+1]);
					i++;
			}
}	
	
//显示连续字串(半宽字符)
void ShowNUMChar(uint8_t addr,uint8_t i,uint8_t count)
{
			uint8_t j;
			for(j=0;j<count;)
			{
					write_com(addr);//设定DDRAM 地址
					write_data(i+j);
					j++;
					write_data(i+j);
					addr++;
					j++;
			}
}	
	
//自定义字符写入CGRAM
void WRCGRAM(uint8_t data1,uint8_t data2,uint8_t addr)
{
			uint8_t i;
			write_com(0x34);//扩充指令集动作
			write_com(0x36);//扩充指令,8位数据,开启绘图开关(ON)
			for(i=0;i<16;)
			{
					write_com(addr+i); //设定CGRAM 地址
					write_data(data1);
					write_data(data1);
					i++;
					write_com(addr+i); //设定CGRAM 地址
					write_data(data2);
					write_data(data2);
					i++;
			}
}	
	
//显示自定义的字符,并把这个字符填满全屏16*16
void ShowCGChar(uint8_t addr,uint8_t i)
{
			uint8_t j;
			for(j=0;j<0x20;)
			{
					write_com(addr+j); //设定DDRAM 地址
					write_data(0x00);
					write_data(i);
					j++;
			}
}	

void Lcd_Clear(uint16_t addr,uint8_t dat)
{
			uint8_t x_addr,y_addr,i;
//			x_addr = addr>>4;//除以16
//			y_addr = addr%16;//除以16的余数	
	
			//write_com(0x80+y_addr);//设定垂直(列)地址AC6~AC0
			write_com(0x80+addr);//X 坐标
			//write_data(dat>>8);
			for(x_addr=0;x_addr<16;x_addr++)
			write_data(dat);
			systick_delay_us(20);
}	


//void Lcd_Clear(uint16_t wdat)
//{
//			uint8_t j,i;
//			write_com(0x01);//游标归位		
//			systick_delay_us(4600);//4.6mS
//			write_com(0x36);//扩充指令集动作
//			systick_delay_us(4600);//4.6mS
//			write_com(0x03);//扩充指令集-允许垂直卷动地址
//			systick_delay_us(4600);//4.6mS
//			for(j=0;j<0x20;j++)
//			{
//					write_com(0x80+j);//设定垂直(列)地址AC6~AC0
//					write_com(0x80);//X 坐标
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
//				write_com(0x80);//X 坐标
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
			write_com(0x01);//游标归位		
			systick_delay_us(4600);//4.6mS
			write_com(0x36);//扩充指令,8位数据,开启绘图开关(ON)
//			write_com(0x03);//扩充指令集-允许垂直卷动地址
			systick_delay_us(4600);
			for(j=0;j<0x20;j++)
			{
					write_com(0x80+j);//设定垂直(列)地址AC6~AC0
					write_com(0x80);//X 坐标
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
//			write_com(0x01);//游标归位		
//			systick_delay_us(4600);//4.6mS
//			write_com(0x34);//扩充指令集动作
//			write_com(0x36);//扩充指令,8位数据,开启绘图开关(ON)
//			systick_delay_us(4600);
//	
//			for(j=0;j<0x10;j++)
//			{
//					for(i=0;i<0x10;i++)//	
//					{
//						write_com(0x80+i);//垂直坐标（行地址或垂直地址 Y）地址AC6~AC0
//						write_com(0x80+j);//然后写水平坐标（列地址或水平地址 X）
//						write_data(wdat>>8);
//						write_data(wdat);
//					}
//			}
//			for(j=0;j<0x10;j++)
//			{
//					for(i=10;i<0x20;i++)//	
//					{
//						write_com(0x80+i);//垂直坐标（行地址或垂直地址 Y）地址AC6~AC0
//						write_com(0x80+j);//然后写水平坐标（列地址或水平地址 X）
//						write_data(wdat>>8);
//						write_data(wdat);
//					}
//			}
////	
////			for(j=0;j<0x20;j++)
////			{
////					write_com(0x80+j);//设定垂直(列)地址AC6~AC0
////					write_com(0x80);//X 坐标
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
