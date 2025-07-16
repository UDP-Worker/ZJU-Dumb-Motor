#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define LED0_Pin GPIO_PIN_5
#define LED0_GPIO_Port GPIOA

#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOD

//void MX_GPIO_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */
