#ifndef RESTORE_ACTOR_CALLBACKS_SMALL_H
#define RESTORE_ACTOR_CALLBACKS_SMALL_H
#include <stdint.h>
/* Original-address aliases for the no-argument current-actor dispatch table. */
void FUN_0049b390(void),FUN_00496f20(void),FUN_00493ec0(void),FUN_00494010(void);
/* Native493610 does not inspect the stack word pushed by its callers. */
void FUN_00493610(void);
void restore_actor_small_state(int32_t unused);
int32_t FUN_004578b0(int32_t building),FUN_004579d0(int32_t building);
uint32_t FUN_004c5120(int32_t building);
#define AS_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define AS_I(a) (*(volatile int32_t *)(uintptr_t)(uint32_t)(a))
#define AS_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define AS_S(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define AS_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static __inline uint32_t as_actor(uint32_t id){return 0x00bcd600u+id*388u;}
static __inline uint32_t as_current(void){return as_actor(AS_U(0x00d056f8u));}
static __inline uint32_t as_building(int32_t id){return 0x007999d8u+(uint32_t)id*264u;}
#endif
