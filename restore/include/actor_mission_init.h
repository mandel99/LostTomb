#ifndef RESTORE_ACTOR_MISSION_INIT_H
#define RESTORE_ACTOR_MISSION_INIT_H
#include <stdint.h>
void restore_actor_initialize_reserved(void);
void restore_actor_reset_pool(void);
void restore_actor_reset_town_links(void);
void restore_actor_initialize_scenario_groups(void);
void restore_actor_initialize_scenario_points(void);
int32_t restore_actor_group_allocate(uint16_t kind,int32_t mode,int32_t orientation,uint16_t x,uint16_t y);
int32_t FUN_004b6ff0(uint16_t kind,int32_t mode,int32_t orientation,uint16_t x,uint16_t y);
void FUN_004bb090(void);
void FUN_004bb3f0(void);
void FUN_004bbc10(void);
void FUN_004b4760(void);
void FUN_004b8c90(void);
/* Genuine native ECX-only ten-instruction method, explicit C receiver. */
void restore_town_actor_links_00448a60(void *self);
#endif
