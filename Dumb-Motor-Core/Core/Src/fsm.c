#include "fsm.h"
#include "follow.h"
#include "queue.h"
#include "avoid.h"
#include "beep.h"

volatile State g_state = FOLLOW;
static uint32_t avoid_ticks = 0;

void fsm_tick(void)
{
    static uint8_t cnt = 0;

    ir_update();

    if(g_avoid_ir && queue_is_empty())
    {
        queue_clear();
        enqueue(BWD, 12*BWD_MS);
        enqueue(L10, 20*TURN_MS);
        return;
    }

    switch(g_state)
    {
        case FOLLOW:
            if(front_blocked() && queue_is_empty())
            {
                avoid_plan();
                g_state = AVOID;
                avoid_ticks = 0;
            }
            else if(++cnt >= 10)
            {
                cnt = 0;
                follow_update();
            }
            break;
        case AVOID:
            avoid_ticks++;
            if(avoid_ticks > AVOID_TIMEOUT)
            {
                queue_clear();
                enqueue(BWD, 4*BWD_MS);
                enqueue(L10, 3*TURN_MS);
                avoid_ticks = 0;
            }
            if(queue_is_empty())
            {
                g_state = FOLLOW;
                avoid_ticks = 0;
            }
            break;
        case RECOVER:
            break;
    }
}
