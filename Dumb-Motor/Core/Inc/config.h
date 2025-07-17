#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdint.h>

#define TURN_MS        52
#define STEP_LIMIT      3
#define FWD_MS         60
#define BWD_MS         60
#define BRAKE_MS       40
#define D_SAFE         40

#include <stdbool.h>

typedef enum {FOLLOW, AVOID, RECOVER} State;

extern volatile uint8_t g_L, g_R;
extern volatile uint16_t us_dist[21];
extern volatile uint8_t us_idx;
extern volatile State g_state;

#endif /* __CONFIG_H */
