#ifndef RESTORE_ACTOR_COMMAND_H
#define RESTORE_ACTOR_COMMAND_H
#include <stdint.h>
void restore_actor_command_spawn_hippos(void);
uint32_t restore_actor_route_line_mode(uint32_t mode);
int32_t restore_actor_find_clear_vector(int32_t x,int32_t y,int32_t first_vector,
 int32_t target_x,int32_t target_y,uint32_t mode);
uint32_t restore_actor_update_hippo(void);
uint32_t restore_actor_update_crocodile(void);
uint32_t restore_actor_update_hyena(void);
uint32_t restore_actor_update_enemy_kind57(void);
void FUN_00499010(void);
uint32_t restore_actor_update_egyptian_archers(void);
void FUN_00498c60(void);
void restore_actor_update_enemy_group(void);
void FUN_004991a0(void);
uint32_t restore_actor_update_enemy_kind53(void);
void FUN_00498a10(void);
uint32_t restore_actor_update_barbarian_kind52(void);
void FUN_004988b0(void);
uint32_t restore_actor_update_egyptian_infantry(void);
void FUN_00498ad0(void);
uint32_t restore_actor_update_barbarian_kind51(void);
void FUN_004987f0(void);
uint32_t restore_actor_update_barbarian_kind50(void);
void FUN_00498750(void);
uint32_t restore_actor_update_barbarian_kind49(void);
void FUN_004986b0(void);
uint32_t restore_actor_update_enemy_kind48(void);
void FUN_00498590(void);
uint32_t restore_actor_update_enemy_kind47(void);
void FUN_004984a0(void);
uint32_t restore_actor_update_enemy_kind46(void);
void FUN_00498370(void);
uint32_t restore_actor_update_enemy_kind44(void);
void FUN_00497fc0(void);
uint32_t restore_actor_update_enemy_kind43(void);
void FUN_00497dc0(void);
void restore_actor_update_service_state(void);
void FUN_00493610(void);
uint32_t restore_actor_update_sentry(void);
void FUN_00497450(void);
uint32_t restore_actor_update_barbarian(void);
void FUN_00497010(void);
uint32_t restore_actor_update_docker(void);
void FUN_00494f70(void);
uint32_t restore_actor_update_missionary(void);
void FUN_0049b2a0(void);
uint32_t restore_actor_update_creature(void);
void FUN_0049b280(void);
uint32_t restore_actor_update_shipwreck(void);
void FUN_0049b610(void);
uint32_t restore_actor_update_ballista(void);
void FUN_0049aeb0(void);
uint32_t restore_actor_update_bolt(void);
void FUN_0049ac00(void);
uint32_t restore_actor_update_javelin(void);
void FUN_0049a880(void);
uint32_t restore_actor_update_arrow(void);
void FUN_0049a5a0(void);
uint32_t restore_actor_update_native_trader(void);
void FUN_0049a150(void);
void restore_actor_update_ostrich(void);
void FUN_0049bd40(void);
void restore_actor_update_antelope(void);
void FUN_0049c750(void);
uint32_t restore_actor_update_birds(void);
void FUN_0049b750(void);
uint32_t restore_actor_update_scorpion(void);
uint32_t restore_actor_update_lion(void);
uint32_t restore_actor_update_asp(void);
void FUN_0049f710(void),FUN_004a04d0(void),FUN_004a1130(void);
void restore_actor_play_kind_cue(int32_t actor);
uint32_t restore_actor_choose_crocodile_state(int32_t actor);
/* Preserve the initializer's native full EAX through named/raw entries. */
uint32_t restore_actor_enter_animal_state(int32_t actor,uint32_t state);
void FUN_0049d0f0(void),FUN_0049dc50(void);
void FUN_004b5050(int32_t actor),FUN_004ecd40(int32_t actor);
uint8_t FUN_004eaac0(int32_t mode);
int32_t FUN_004eab10(int32_t x,int32_t y,int32_t first_vector,
 int32_t target_x,int32_t target_y,int32_t mode);
void FUN_0049e8b0(void);
uint32_t restore_actor_random_seed(uint32_t seed);
uint32_t restore_actor_try_periodic_target(int32_t actor);
uint8_t restore_actor_update_group_target(int32_t actor);
int32_t restore_actor_find_state_nearby(int32_t x,int32_t y,int32_t type_count,
 const uint32_t *types,uint32_t terrain_mask,volatile int32_t *out_x,
 volatile int32_t *out_y,int32_t rings,int32_t excluded,uint32_t state);
uint8_t restore_actor_choose_clear_step(int32_t x,int32_t y,int32_t from_x,
 int32_t from_y,volatile int32_t *out_x,volatile int32_t *out_y);
/* Existing byte/int/void source interfaces are retained in actor_civic.c. */
uint8_t FUN_004eaec0(int32_t actor),FUN_004eb000(int32_t actor);
int32_t FUN_004ebc90(int32_t x,int32_t y,int32_t type_count,const uint32_t *types,
 uint32_t mask,int32_t *out_x,int32_t *out_y,int32_t rings,int32_t excluded,int32_t state);
uint8_t FUN_004eabf0(int32_t x,int32_t y,int32_t from_x,int32_t from_y,int32_t *out_x,int32_t *out_y);
void FUN_0056233e(uint32_t seed);
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
