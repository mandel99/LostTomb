#ifndef RESTORE_ACTOR_COMMAND_H
#define RESTORE_ACTOR_COMMAND_H
#include <stdint.h>
void restore_actor_command_spawn_hippos(void);
typedef uint32_t (*restore_actor_tile_callback)(uint32_t tile,uint32_t actor);
uint32_t restore_actor_find_other_kind_on_tile(uint32_t tile,uint32_t actor);
uint32_t restore_actor_spiral_search(int32_t x,int32_t y,volatile int32_t *out_x,
 volatile int32_t *out_y,int32_t rings,uint32_t actor,restore_actor_tile_callback callback);
uint32_t restore_actor_seek_other_kind(int32_t actor);
uint32_t restore_actor_choose_command_state(int32_t actor);
uint32_t restore_actor_apply_command_state(int32_t actor,uint32_t state);
uint32_t FUN_004ec0f0(int32_t x,int32_t y,volatile int32_t *out_x,
 volatile int32_t *out_y,int32_t rings,uint32_t actor,restore_actor_tile_callback callback);
/* Hippo Stomp: reviewed nullary command-table row3. */
void restore_actor_command_0041cd00(void);
int restore_actor_command_action(uint32_t token);
/* Prefix helper preserves the native one-DWORD/RET0 contract. */
uint32_t restore_actor_command_004ec990(int32_t actor);
uint32_t FUN_004ec4a0(int32_t actor,uint32_t state);
void restore_actor_command_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
#endif
