#include "avoid.h"
#include "queue.h"
#include "ultrasonic.h"

int front_blocked(void)
{
    return dist_center < D_SAFE;
}

void avoid_plan(void)
{
    queue_clear();
    if(dist_left > dist_right)
        enqueue(L10, 5 * TURN_MS);
    else
        enqueue(R10, 5 * TURN_MS);
    enqueue(FWD, 3 * FWD_MS);
}
