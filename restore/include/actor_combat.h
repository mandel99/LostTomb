#ifndef RESTORE_ACTOR_COMBAT_H
#define RESTORE_ACTOR_COMBAT_H
#include <stdint.h>
void FUN_004b17d0(int32_t actor),FUN_004b1da0(int32_t actor),FUN_004b3f00(int32_t actor);
uint32_t FUN_004b1d10(int32_t direction,int32_t other);
void FUN_004b3250(int32_t actor),FUN_004b5050(int32_t actor),FUN_004b5210(int32_t actor),FUN_004b7ab0(int32_t formation);
uint32_t FUN_004ea9a0(int32_t type);int32_t FUN_004ea9c0(int32_t type);
void restore_actor_combat_sound(int32_t event);
#define CB_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define CB_I(a) (*(volatile int32_t *)(uintptr_t)(uint32_t)(a))
#define CB_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define CB_S(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define CB_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static __inline uint32_t cb_actor(uint32_t id){return 0xbcd600u+id*388u;}
static __inline uint32_t cb_formation(int32_t id){return 0xbcb9c8u+(uint32_t)id*144u;}
#endif
