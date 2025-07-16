#ifndef UART_IO_H
#define UART_IO_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void UART_IO_Init(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif // UART_IO_H
