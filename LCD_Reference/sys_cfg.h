#ifndef _SYS_CFG_H_
#define _SYS_CFG_H_

#include "stm32f10x.h"


#define uchar unsigned char 
#define uint  unsigned int 	  

//#define DATA  P2 //数据输出端0~7 
#define BF 0x80 //用于检测LCD忙状态的标志
//const uchar delay=250; //延时时间常数  

//#define KeyIO 		P1//按键阵列串口为P1
//#define SW_CTRL 	P3//切换程序控制
#define OPT_CHECK GPIOB->IDR	//光开关检测

/*
sbit RS=P4^0; //LCD12864 RS端 
sbit RW=P4^1; //LCD12864 RW端 
sbit E =P4^2; //LCD12864 E 端 
sbit PSB =P4^3;  
*/ 

//Hard Config

/*
#define SPI_CS GPIO_Pin_4	//LCD_RS:DI
#define SPI_CS_GPIO_Port GPIOA

#define SPI_SCLK GPIO_Pin_5//LCD_E
#define SPI_SCLK_GPIO_Port GPIOA

//#define SPI_MISO GPIO_Pin_6
//#define SPI_MISO_GPIO_Port GPIOA

#define SPI_MOSI GPIO_Pin_7//LCD_RW
#define SPI_MOSI_GPIO_Port GPIOA
*/

#define SPI_CS GPIO_Pin_5	//LCD_RS:CS
#define SPI_CS_GPIO_Port GPIOA

#define SPI_SCLK GPIO_Pin_4//LCD_E-CLK
#define SPI_SCLK_GPIO_Port GPIOA

//#define SPI_MISO GPIO_Pin_6
//#define SPI_MISO_GPIO_Port GPIOA

#define SPI_MOSI GPIO_Pin_6//LCD_RW-SID
#define SPI_MOSI_GPIO_Port GPIOA


//#define LCD_PSB GPIO_Pin_13
//#define LCD_PSB_GPIO_Port GPIOC

#define LCD_PSB GPIO_Pin_9
#define LCD_PSB_GPIO_Port GPIOB




#define KEYINT GPIO_Pin_0
#define KEYINT_GPIO_Port GPIOA

#define SW_S1 GPIO_Pin_1
#define SW_S1_GPIO_Port GPIOA

#define SW_S2 GPIO_Pin_2
#define SW_S2_GPIO_Port GPIOA

#define SYS_BEEP GPIO_Pin_3
#define SYS_BEEP_GPIO_Port GPIOA

#define SYS_TX GPIO_Pin_9
#define SYS_TX_GPIO_Port GPIOA

#define SYS_RX GPIO_Pin_10
#define SYS_RX_GPIO_Port GPIOA

#define LCD_RST GPIO_Pin_6
#define LCD_RST_GPIO_Port GPIOA

#define DIS_LED1 GPIO_Pin_8
#define DIS_LED1_GPIO_Port GPIOA

#define DIS_LED2 GPIO_Pin_15
#define DIS_LED2_GPIO_Port GPIOA


#define KEYS_GPIO_Port GPIOB

#define KEY0_PIN GPIO_Pin_7
#define KEY1_PIN GPIO_Pin_6
#define KEY2_PIN GPIO_Pin_5
#define KEY3_PIN GPIO_Pin_4

#define KEY4_PIN GPIO_Pin_3
#define KEY5_PIN GPIO_Pin_2
#define KEY6_PIN GPIO_Pin_1
#define KEY7_PIN GPIO_Pin_0




#define LCD_PSB_LOW    GPIO_ResetBits(LCD_PSB_GPIO_Port,LCD_PSB)
#define LCD_PSB_HIGH   GPIO_SetBits(LCD_PSB_GPIO_Port,LCD_PSB)

#define SPI_CS_DISABLE GPIO_ResetBits(SPI_CS_GPIO_Port,SPI_CS)
#define SPI_CS_ENABLE  GPIO_SetBits(SPI_CS_GPIO_Port,SPI_CS)

#define SPI_CLK_LOW   GPIO_ResetBits(SPI_SCLK_GPIO_Port,SPI_SCLK)
#define SPI_CLK_HIGH  GPIO_SetBits(SPI_SCLK_GPIO_Port,SPI_SCLK)
#define SPI_DAT_LOW  	GPIO_ResetBits(SPI_MOSI_GPIO_Port,SPI_MOSI)
#define SPI_DAT_HIGH 	GPIO_SetBits(SPI_MOSI_GPIO_Port,SPI_MOSI) 
#define SPI_DAT_IN    PAin(6)

// ****************************************************
#define LCD_DI_LOW   	GPIO_ResetBits(GPIOA,GPIO_Pin_4)//LCD_RS
#define LCD_DI_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define LCD_CS_LOW   	GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define LCD_CS_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LCD_RW_LOW   	GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LCD_RW_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define LCD_SID_LOW   	GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LCD_SID_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define LCD_E_LOW   	GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LCD_E_HIGH   	GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define LCD_SCLK_LOW   	GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LCD_SCLK_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LCD_RST_LOW   	GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LCD_RST_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_6)


#endif

