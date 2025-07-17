#include "ultrasonic.h"
#include "gpio.h"
#include "tim.h"
#include "config.h"
#include "beep.h"
#include "queue.h"

extern TIM_HandleTypeDef htim2;

static volatile float last_distance = 0.0f;
static volatile UltrasonicPos active_pos = US_POS_CENTER;

static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = (HAL_RCC_GetHCLKFreq() / 1000000U) * us;
    while ((DWT->CYCCNT - start) < cycles) {}
}

void Ultrasonic_Init(void)
{
    /* enable DWT cycle counter for microsecond delay */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void Ultrasonic_Trigger(void)
{
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
}

float Ultrasonic_GetDistance(void)
{
    return last_distance;
}

void Ultrasonic_StartMeasurement(UltrasonicPos pos)
{
    active_pos = pos;
    Ultrasonic_Trigger();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ECHO_Pin)
    {
        static uint16_t start = 0;
        if (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET)
        {
            __HAL_TIM_SET_COUNTER(&htim2, 0);
            HAL_TIM_Base_Start(&htim2);
            start = 1;
        }
        else if (start)
        {
            HAL_TIM_Base_Stop(&htim2);
            uint32_t ticks = __HAL_TIM_GET_COUNTER(&htim2);
            last_distance = ticks * 100.0f / 58.0f; /* 100us tick */


            switch(active_pos)
            {
                case US_POS_CENTER:
                    dist_center = (uint16_t)last_distance;
                    if(dist_center < D_SAFE)
                        Beep_On();
                    else
                        Beep_Off();
                    break;
                case US_POS_LEFT:
                    dist_left = (uint16_t)last_distance;
                    if(dist_left < D_SAFE)
                    {
                        queue_clear();
                        enqueue(R10, 5 * TURN_MS);
                    }
                    break;
                case US_POS_RIGHT:
                    dist_right = (uint16_t)last_distance;
                    if(dist_right < D_SAFE)
                    {
                        queue_clear();
                        enqueue(L10, 5 * TURN_MS);
                    }
                    break;

            }

            start = 0;
        }
    }
}
