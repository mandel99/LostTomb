#ifndef RESTORE_ACTOR_CONSTABLE_H
#define RESTORE_ACTOR_CONSTABLE_H
#include <stdint.h>
struct restore_constable_frame;
typedef void (*restore_constable_call)(uint32_t,uint32_t,struct restore_constable_frame *);
struct restore_constable_frame {uint32_t r[8],flags;restore_constable_call boundary;};
void restore_actor_constable_dispatch(uint32_t,uint32_t,struct restore_constable_frame *);
void restore_actor_constable_raw(uint32_t,uint32_t registers[8],uint32_t *flags);
void restore_constable_C_004abe40(struct restore_constable_frame *);
void restore_constable_C_0048e6f0(struct restore_constable_frame *);
void restore_constable_C_0048e820(struct restore_constable_frame *);
void restore_constable_C_00518b30(struct restore_constable_frame *);
void FUN_004abe40(void);
uint32_t FUN_0048e6f0(int32_t actor);
int32_t FUN_0048e820(int32_t x,int32_t y,int32_t radius);
int32_t FUN_00518b30(int32_t x,int32_t y,const int32_t *tiles,int32_t count);
void FUN_0053b4a0(int32_t actor,int32_t sound,int32_t variant);
#endif
