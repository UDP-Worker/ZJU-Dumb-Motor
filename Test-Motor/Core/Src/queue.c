#include "queue.h"
#include "motor.h"
#include "config.h"

volatile Cmd q[QLEN];
volatile uint8_t qi = 0, qj = 0;
static uint16_t rest = 0;

void enqueue(Act a, uint16_t t)
{
    q[qj] = (Cmd){a,t};
    qj = (qj + 1) & (QLEN - 1);
}

void queue_tick(void)
{
    if(rest){
        rest--; return;
    }
    if(qi==qj){
        motor_fwd();
        rest = FWD_MS;
        return;
    }
    Cmd c = q[qi];
    qi = (qi + 1) & (QLEN - 1);
    switch(c.a){
        case FWD: motor_fwd(); break;
        case BWD: motor_bwd(); break;
        case BRK: motor_brake(); break;
        case L10: motor_turn_left(); break;
        case R10: motor_turn_right(); break;
    }
    rest = c.t;
}
