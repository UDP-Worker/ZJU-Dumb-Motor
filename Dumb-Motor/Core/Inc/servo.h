#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void Servo_Init(void);
void Servo_SetAngle(float angle);

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H */
