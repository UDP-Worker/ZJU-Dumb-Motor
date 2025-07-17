#ifndef _SPI_DISPLAY_H_
#define _SPI_DISPLAY_H_

//#include "spi_lcd_display.h"
#include <stdio.h>
#include "stm32f10x.h"

#define	LCD_WR_COM	0xF8	//Ð´ÃüÁîMCU->LCD
#define	LCD_WR_DAT	0xFA	//Ð´Êý¾ÝMCU->LCD





void SendByteLCD(uint8_t WLCDData);
void SPIWR(uint8_t Wdata,uint8_t WRS);
void WRData(uint8_t Data);
void WRCommand(uint8_t CMD);
void lcd_port_init(void);
//void hzkdis(uint8_t code *str);
void Disp(uint8_t y,uint8_t x,uint8_t i,unsigned char *z);
void ShowQQChar(uint8_t addr,uint8_t *english,uint8_t count);
void ShowNUMChar(uint8_t addr,uint8_t i,uint8_t count);
void WRCGRAM(uint8_t data1,uint8_t data2,uint8_t addr);
void ShowCGChar(uint8_t addr,uint8_t i);
void Lcd_Clear(uint16_t addr,uint8_t dat);
void CLEARGDRAM(uint16_t wdat);
void WRGDRAM(uint8_t Y1,uint8_t clong,uint8_t hight,uint8_t *TAB1);
void menu(void);
void menu2(void);
void lcd_serial_delay(u32 nus);

#endif
