#ifndef LCD_H
#define LCD_H

#include "stm32f1xx_hal.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Print(const char *str);

#ifdef __cplusplus
}
#endif

#endif // LCD_H
