#ifndef RESTORE_ACTOR_WALKING_H
#define RESTORE_ACTOR_WALKING_H
#include <stdint.h>
#define AW_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define AW_I(a) ((int32_t)AW_U(a))
#define AW_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define AW_S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define AW_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define AW_C(a) ((int32_t)(int8_t)AW_B(a))
void FUN_004a5da0(int32_t actor,uint32_t speed);
void FUN_004b27e0(int32_t actor,int32_t substeps);
void FUN_004b33d0(int32_t actor),FUN_004b3530(int32_t actor),FUN_004b3c60(int32_t actor);
void FUN_004b52f0(int32_t actor),FUN_004b5420(int32_t actor);
uint32_t restore_actor_link_to_grid(int32_t actor);
uint32_t restore_actor_unlink_from_grid(int32_t actor);
void FUN_004b3f00(int32_t actor),FUN_004b3250(int32_t actor),FUN_004b2d20(int32_t actor);
void FUN_004b1210(int32_t actor,int32_t other);
uint32_t FUN_0051cba0(int32_t x,int32_t y,int32_t target_x,int32_t target_y);
uint32_t FUN_004baa10(int32_t actor);
void FUN_0051f780(void);
void restore_actor_walking_service(int32_t actor,int32_t unused);
void restore_actor_walking_empty(int32_t actor);
uint32_t restore_actor_walking_queue_count(const void *self);
uint32_t restore_actor_walking_queue_full(const void *self);
uint32_t restore_actor_walking_queue_push(void *self,const int32_t *value);
uint32_t FUN_004ad880(int32_t *x,int32_t *y);
uint32_t FUN_004b3470(int32_t actor,int32_t building);
void FUN_004690f0(int32_t tile,int32_t amount);
#endif
