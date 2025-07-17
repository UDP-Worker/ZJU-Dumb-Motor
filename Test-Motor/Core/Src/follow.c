#include "follow.h"
#include "gpio.h"
#include "queue.h"
#include "config.h"

volatile uint8_t g_L = 0, g_R = 0, g_avoid_ir = 0;

void ir_update(void)
{
    g_L = (HAL_GPIO_ReadPin(IR_LEFT_GPIO_Port, IR_LEFT_Pin) == GPIO_PIN_SET);
    g_R = (HAL_GPIO_ReadPin(IR_RIGHT_GPIO_Port, IR_RIGHT_Pin) == GPIO_PIN_SET);
    g_avoid_ir = (HAL_GPIO_ReadPin(AVOID_GPIO_Port, AVOID_Pin) == GPIO_PIN_RESET);
}

void follow_update(void)
{
    static int8_t err = 0;
    static uint8_t step_cnt = 0;

    if(!queue_is_empty())
        return;

    if(!g_L && !g_R)      err = 0;
    else if(g_L && !g_R)  err = +1;
    else if(!g_L && g_R)  err = -1;
    else                  err = (err>0)? +2 : -2;

    if(err==0) step_cnt = 0;
    else if(step_cnt < STEP_LIMIT) step_cnt++;

    if(err != 0)
        queue_clear();

    switch(err){
        case 0:
            enqueue(FWD, FWD_MS);
            break;
        case +1:
            enqueue(BWD, 10*BWD_MS);
            enqueue(R10, 10*TURN_MS);
            enqueue(FWD, FWD_MS/2);
            break;
        case -1:
            enqueue(BWD, 10*BWD_MS);
            enqueue(L10, 10*TURN_MS);
            enqueue(FWD, FWD_MS/2);
            break;
        default:
            enqueue(BRK, BRAKE_MS);
            enqueue(BWD, 2*BWD_MS);
            enqueue((err>0)? R10 : L10, TURN_MS*2);
            break;
    }
}
