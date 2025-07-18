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
int queue_is_empty(void);
int queue_is_full(void);
void queue_clear(void);
#define is_empty queue_is_empty
#ifdef __cplusplus
}
#endif

#endif /* __QUEUE_H */
