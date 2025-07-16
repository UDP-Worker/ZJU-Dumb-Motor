#include "ultrasonic.h"

extern TIM_HandleTypeDef htim2;

static volatile float last_distance = 0.0f;

static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = (HAL_RCC_GetHCLKFreq() / 1000000) * us;
    while ((DWT->CYCCNT - start) < cycles) {
    }
}

void Ultrasonic_Init(void)
{
    /* enable DWT cycle counter for microsecond delay */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void Ultrasonic_Trigger(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

float Ultrasonic_GetDistance(void)
{
    return last_distance;
}

float Ultrasonic_Measure(void)
{
    Ultrasonic_Trigger();
    HAL_Delay(60);
    return last_distance;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_1)
    {
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        HAL_TIM_Base_Start(&htim2);
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_SET) {
        }
        HAL_TIM_Base_Stop(&htim2);
        uint32_t ticks = __HAL_TIM_GET_COUNTER(&htim2);
        last_distance = ticks * 100.0f / 58.0f; /* timer tick = 100us */
    }
}
