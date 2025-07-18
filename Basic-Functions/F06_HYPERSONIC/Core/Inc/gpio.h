#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define TRIG_Pin  GPIO_PIN_0
#define TRIG_GPIO_Port GPIOC

#define ECHO_Pin  GPIO_PIN_1
#define ECHO_GPIO_Port GPIOC

#define BEEP_Pin  GPIO_PIN_3
#define BEEP_GPIO_Port GPIOC

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */
