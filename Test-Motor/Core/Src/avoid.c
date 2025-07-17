#include "avoid.h"
#include "queue.h"

int front_blocked(void)
{
    return us_dist[10] < D_SAFE;
}

void avoid_plan(void)
{
    uint8_t free[21];
    for(int i=0;i<21;i++)
        free[i] = (us_dist[i] > D_SAFE) ? 1 : 0;

    if(g_L)
        for(int i=0;i<5;i++)
            free[i] = 0;
    if(g_R)
        for(int i=16;i<21;i++)
            free[i] = 0;

    int best_len = 0, best_start = 10;
    int len = 0, start = 0;
    for(int i=0;i<=21;i++)
    {
        if(i<21 && free[i])
        {
            if(len==0) start = i;
            len++;
        }
        else
        {
            if(len>best_len){ best_len = len; best_start = start; }
            len = 0;
        }
    }
    int center = best_start + best_len/2;
    int n = center - 10;

    queue_clear();
    if(n>0)
        for(int k=0;k<n;k++)
            enqueue(L10, TURN_MS);
    else if(n<0)
        for(int k=0;k<-n;k++)
            enqueue(R10, TURN_MS);
    enqueue(FWD, 3*FWD_MS);
}
