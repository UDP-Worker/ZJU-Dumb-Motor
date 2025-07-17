#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        MOTOR_RIGHT = 0,
        MOTOR_LEFT
    } Motor_Channel;

    void Motor_Init(void);
    void motor_fwd(void);
    void motor_bwd(void);
    void motor_turn_left(void);
    void motor_turn_right(void);
    void motor_brake(void);

#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_H */
