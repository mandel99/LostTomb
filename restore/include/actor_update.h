#ifndef RESTORE_ACTOR_UPDATE_H
#define RESTORE_ACTOR_UPDATE_H
#include <stdint.h>
typedef void (*RestoreActorCallback)(void);
void FUN_00489b70(void);
void FUN_0049bd40(void);
void restore_actor_callback(uint32_t);
RestoreActorCallback restore_actor_callback_resolve(uint32_t);
#endif
