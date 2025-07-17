#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define LCD_CS_Pin       GPIO_PIN_5
#define LCD_CS_GPIO_Port GPIOA

#define LCD_CLK_Pin      GPIO_PIN_4
#define LCD_CLK_GPIO_Port GPIOA

#define LCD_DIO_Pin      GPIO_PIN_6
#define LCD_DIO_GPIO_Port GPIOA

#define LCD_RST_Pin      GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOA

#define LCD_PSB_Pin      GPIO_PIN_9
#define LCD_PSB_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */
