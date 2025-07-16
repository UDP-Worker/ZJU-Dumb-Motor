/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "motor.h"
#include "gpio.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    Motor_Init();

    while (1)
    {
        if (HAL_GPIO_ReadPin(AVOID_GPIO_Port, AVOID_Pin) == GPIO_PIN_SET)
        {
            GPIO_PinState left  = HAL_GPIO_ReadPin(SEARCH_L_GPIO_Port, SEARCH_L_Pin);
            GPIO_PinState right = HAL_GPIO_ReadPin(SEARCH_R_GPIO_Port, SEARCH_R_Pin);

            if (left == GPIO_PIN_RESET && right == GPIO_PIN_RESET)
            {
                Motor_Run(50, 10);
            }
            else if (left == GPIO_PIN_SET && right == GPIO_PIN_RESET)
            {
                Motor_Left(50, 10);
            }
            else if (right == GPIO_PIN_SET && left == GPIO_PIN_RESET)
            {
                Motor_Right(50, 10);
            }
            else
            {
                Motor_Brake(10);
            }
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
            Motor_Brake(300);
            Motor_Back(50, 400);
            Motor_Left(50, 500);
        }
    }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file;
  (void)line;
}
#endif /* USE_FULL_ASSERT */
