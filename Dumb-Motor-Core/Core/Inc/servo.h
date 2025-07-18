#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SERVO_LEFT_BOUNDARY  180.0f
#define SERVO_RIGHT_BOUNDARY 20.0f
#define SERVO_CENTER_ANGLE   100.0f

void Servo_Init(void);
void Servo_SetAngle(float angle);
void Servo_SweepStep(void);

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H */
