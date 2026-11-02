#ifndef RESTORE_ACTOR_CALLBACKS_CHAIN_H
#define RESTORE_ACTOR_CALLBACKS_CHAIN_H
#include <stdint.h>
void FUN_004a2e90(void),FUN_0049b400(void);
int32_t FUN_0049b3c0(int32_t actor),FUN_004b57e0(int32_t actor,uint32_t type);
#define AC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define AC_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define AC_S(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define AC_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static __inline uint32_t ac_actor(uint32_t id){return 0x00bcd600u+id*388u;}
static __inline uint32_t ac_id(void){return AC_U(0x00d056f8u);}
static __inline uint32_t ac_current(void){return ac_actor(ac_id());}
#endif
