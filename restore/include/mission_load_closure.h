#ifndef RESTORE_MISSION_LOAD_CLOSURE_H
#define RESTORE_MISSION_LOAD_CLOSURE_H
#include <stdint.h>
void FUN_004860d0(void);
void FUN_00486e70(void);
void FUN_00442660(void);
void FUN_004de600(void);
/* Genuine four-stack-argument cdecl helper; no ECX receiver. */
uint32_t restore_mission_reach_005190a0(int32_t x,int32_t y,
                                     const uint32_t *targets,int32_t count);
#endif
