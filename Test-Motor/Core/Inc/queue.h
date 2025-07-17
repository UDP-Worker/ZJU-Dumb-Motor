#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {FWD, BRK, L10, R10, BWD} Act;

typedef struct {Act a; uint16_t t;} Cmd;

#define QLEN 32
extern volatile Cmd q[QLEN];
extern volatile uint8_t qi, qj;

void enqueue(Act a, uint16_t t);
void queue_tick(void);
int is_empty(void);
#ifdef __cplusplus
}
#endif

#endif /* __QUEUE_H */
