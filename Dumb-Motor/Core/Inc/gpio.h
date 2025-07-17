#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define LEFT_DIR_Pin  GPIO_PIN_7
#define LEFT_DIR_GPIO_Port GPIOB
#define RIGHT_DIR_Pin GPIO_PIN_4
#define RIGHT_DIR_GPIO_Port GPIOA

#define BEEP_Pin       GPIO_PIN_3
#define BEEP_GPIO_Port GPIOC

#define KEY_Pin        GPIO_PIN_2
#define KEY_GPIO_Port  GPIOC
#define KEY_EXTI_IRQn  EXTI2_IRQn

#define IR_RIGHT_Pin   GPIO_PIN_7
#define IR_RIGHT_GPIO_Port GPIOA
#define IR_LEFT_Pin    GPIO_PIN_0
#define IR_LEFT_GPIO_Port GPIOB

#define TRIG_Pin  GPIO_PIN_0
#define TRIG_GPIO_Port GPIOC
#define ECHO_Pin  GPIO_PIN_1
#define ECHO_GPIO_Port GPIOC

//void MX_GPIO_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */
