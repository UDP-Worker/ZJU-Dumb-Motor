#ifndef __AVOID_H
#define __AVOID_H

#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

void servo_sweep(void);
void avoid_plan(void);
bool front_blocked(void);

#ifdef __cplusplus
}
#endif

#endif /* __AVOID_H */
