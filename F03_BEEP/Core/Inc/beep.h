#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f1xx_hal.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

void Beep_Init(void);
void Beep_On(void);
void Beep_Off(void);
void Beep_Toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* __BEEP_H */
