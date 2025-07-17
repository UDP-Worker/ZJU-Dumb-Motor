#include "fsm.h"
#include "follow.h"
#include "queue.h"
#include "avoid.h"

volatile State g_state = FOLLOW;

void fsm_tick(void)
{
    static uint8_t cnt = 0;

    ir_update();

    switch(g_state)
    {
        case FOLLOW:
            if(front_blocked() && queue_is_empty())
            {
                avoid_plan();
                g_state = AVOID;
            }
            else if(++cnt >= 10)
            {
                cnt = 0;
                follow_update();
            }
            break;
        case AVOID:
            if(queue_is_empty())
                g_state = FOLLOW;
            break;
        case RECOVER:
            break;
    }
}
