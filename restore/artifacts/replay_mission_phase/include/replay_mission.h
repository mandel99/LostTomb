#ifndef RESTORE_REPLAY_MISSION_H
#define RESTORE_REPLAY_MISSION_H
#include "restore_types.h"
#include <stdint.h>
#ifndef RM_PUBLIC
#define RM_PUBLIC(name) name
#endif
#define RM_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define RM_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#ifdef REPLAY_MISSION_TRACE
void restore_replay_mission_write(uint32_t address,uint32_t width,uint32_t value);
#define RM_PUT32(a,v) restore_replay_mission_write((uint32_t)(a),4,(uint32_t)(v))
#define RM_PUT8(a,v) restore_replay_mission_write((uint32_t)(a),1,(uint8_t)(v))
#else
#define RM_PUT32(a,v) (RM_U(a)=(uint32_t)(v))
#define RM_PUT8(a,v) (RM_B(a)=(uint8_t)(v))
#endif
void FUN_00418510(void);
void FUN_004181e0(void);
#endif
