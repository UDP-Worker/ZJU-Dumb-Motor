#include "avoid.h"
#include "servo.h"
#include "ultrasonic.h"
#include "queue.h"
#include "config.h"

int8_t sweep_idx = -10;

bool front_blocked(void)
{
    return us_dist[10] < D_SAFE;
}

void servo_sweep(void)
{
    sweep_idx++;
    if(sweep_idx>10) sweep_idx=-10;
    us_idx = sweep_idx + 10;
    /* Servo scans from 20° to 180° with 100° as the centre */
    float angle = 100.0f + sweep_idx*8.0f;
    Servo_SetAngle(angle);
    Ultrasonic_Trigger();
}

void avoid_plan(void)
{
    uint8_t free[21];
    for(int i=0;i<21;i++)
        free[i] = (us_dist[i] > D_SAFE) ? 1 : 0;

    if(g_L) for(int i=0;i<10;i++) free[i]=0;
    if(g_R) for(int i=11;i<21;i++) free[i]=0;

    int best_len=0,best_start=0,len=0,start=0;
    for(int i=0;i<=21;i++){
        if(i<21 && free[i]){
            if(len==0) start=i; len++; 
        }else{
            if(len>best_len){best_len=len;best_start=start;}
            len=0;
        }
    }
    int center = best_start + best_len/2;
    int n = center - 10;
    if(n>0) for(int k=0;k<n;k++) enqueue(L10, TURN_MS);
    if(n<0) for(int k=0;k<-n;k++) enqueue(R10, TURN_MS);
    enqueue(FWD, 120);
}
