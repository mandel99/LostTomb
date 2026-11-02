#ifndef RESTORE_ACTOR_COMMAND_H
#define RESTORE_ACTOR_COMMAND_H
#include <stdint.h>
struct restore_actor_command_frame;
typedef void (*restore_actor_command_call)(uint32_t,uint32_t,struct restore_actor_command_frame *);
struct restore_actor_command_frame {uint32_t r[8],flags;restore_actor_command_call boundary;};
void restore_actor_command_C_0041cd00(struct restore_actor_command_frame *);
void restore_actor_command_C_004ec990(struct restore_actor_command_frame *);
void restore_actor_command_C_004ec0f0(struct restore_actor_command_frame *);
void restore_actor_command_C_004ec7c0(struct restore_actor_command_frame *);
void restore_actor_command_C_004ec4a0(struct restore_actor_command_frame *);
void restore_actor_command_C_004eae20(struct restore_actor_command_frame *);
/* Reviewed nullary command-table row3, no incoming ECX or scratch payload. */
void restore_actor_command_0041cd00(void);
int restore_actor_command_action(uint32_t token);
/* Prefix helper preserves the native one-DWORD/RET0 contract. */
void restore_actor_command_004ec990(int32_t actor);
uint32_t FUN_004ec4a0(int32_t actor,uint32_t state);
void restore_actor_command_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
#endif
