#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"

typedef enum {
    MOTOR_RIGHT = 0,
    MOTOR_LEFT
} Motor_Channel;

void Motor_Init(void);
void Motor_SetSpeed(Motor_Channel ch, int8_t speed);

void Motor_Run(int8_t speed, uint32_t time_ms);
void Motor_Brake(uint32_t time_ms);
void Motor_Left(int8_t speed, uint32_t time_ms);
void Motor_SpinLeft(int8_t speed, uint32_t time_ms);
void Motor_Right(int8_t speed, uint32_t time_ms);
void Motor_SpinRight(int8_t speed, uint32_t time_ms);
void Motor_Back(int8_t speed, uint32_t time_ms);

#endif
