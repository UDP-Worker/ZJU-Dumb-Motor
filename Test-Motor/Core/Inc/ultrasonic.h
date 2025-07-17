#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void Ultrasonic_Init(void);
void Ultrasonic_Trigger(void);
float Ultrasonic_GetDistance(void);
void Ultrasonic_StartMeasurement(void);

#ifdef __cplusplus
}
#endif

#endif /* __ULTRASONIC_H */
