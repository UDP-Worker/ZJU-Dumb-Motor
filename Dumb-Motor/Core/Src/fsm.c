#include "fsm.h"
#include "follow.h"
#include "avoid.h"
#include "queue.h"
#include "config.h"
#include "main.h"
#include "beep.h"

volatile State g_state = FOLLOW;
static uint16_t recover_ms = 0;

void fsm_tick(void)
{
    switch(g_state){
        case FOLLOW:
            Beep_Off();
            if(front_blocked() && qi==qj){
                avoid_plan();
                g_state = AVOID;
            }else if(!(HAL_GetTick() % 10)){
                follow_update();
            }
            break;
        case AVOID:
            Beep_On();
            if(qi==qj){
                g_state = RECOVER;
                recover_ms = 0;
            }
            break;
        case RECOVER:
            if(g_L || g_R) recover_ms = 0; else recover_ms++;
            if(recover_ms > 200) g_state = FOLLOW;
            break;
    }
}
