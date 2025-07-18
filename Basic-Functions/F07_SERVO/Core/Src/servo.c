#include "servo.h"
#include "main.h"

extern TIM_HandleTypeDef htim5;

static uint16_t pwm_period = 0;

void Servo_Init(void)
{
    pwm_period = htim5.Init.Period;
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
}

void Servo_SetAngle(float angle)
{
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
    uint32_t compare = (uint32_t)(50.0f * angle / 9.0f + 249.0f);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, compare);
}
