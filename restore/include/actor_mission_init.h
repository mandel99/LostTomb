#ifndef RESTORE_ACTOR_MISSION_INIT_H
#define RESTORE_ACTOR_MISSION_INIT_H
#include <stdint.h>
struct restore_actor_init_frame;
typedef void (*restore_actor_init_frame_call)(uint32_t,uint32_t,struct restore_actor_init_frame *);
struct restore_actor_init_frame {uint32_t r[8],flags;restore_actor_init_frame_call boundary;};
void restore_actor_initialize_reserved(void);
void restore_actor_reset_pool(void);
void restore_actor_reset_town_links(void);
void restore_actor_init_C_004bb3f0(struct restore_actor_init_frame *);
void restore_actor_init_C_004bbc10(struct restore_actor_init_frame *);
void FUN_004bb090(void);
void FUN_004bb3f0(void);
void FUN_004bbc10(void);
void FUN_004b4760(void);
void FUN_004b8c90(void);
/* Genuine native ECX-only ten-instruction method, explicit C receiver. */
void restore_town_actor_links_00448a60(void *self);
#endif
