#include "follow.h"
#include "queue.h"
#include "config.h"
#include "gpio.h"
#include "main.h"

static int8_t err = 0;
static uint8_t step_cnt = 0;

void follow_update(void)
{
    g_L = (HAL_GPIO_ReadPin(IR_LEFT_GPIO_Port, IR_LEFT_Pin) == GPIO_PIN_RESET);
    g_R = (HAL_GPIO_ReadPin(IR_RIGHT_GPIO_Port, IR_RIGHT_Pin) == GPIO_PIN_RESET);

    if(qi!=qj) return;

    if(!g_L && !g_R) err = 0;
    else if(g_L && !g_R) err = +1;
    else if(!g_L && g_R) err = -1;
    else err = (err>0)?+2:-2;

    if(err==0) step_cnt=0;
    else if(step_cnt<STEP_LIMIT) step_cnt++;

    switch(err){
        case 0: enqueue(FWD, FWD_MS); break;
        case +1: enqueue(R10, TURN_MS); enqueue(FWD, FWD_MS/2); break;
        case -1: enqueue(L10, TURN_MS); enqueue(FWD, FWD_MS/2); break;
        default:
            enqueue(BRK, BRAKE_MS);
            enqueue(BWD, BWD_MS);
            enqueue((err>0)?R10:L10, TURN_MS*2);
            break;
    }
}
