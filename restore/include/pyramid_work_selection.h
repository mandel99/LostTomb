#ifndef RESTORE_PYRAMID_WORK_SELECTION_H
#define RESTORE_PYRAMID_WORK_SELECTION_H
#include "monument_worker_motion.h"
enum RestorePyramidWorkActorField {
 PS_CLAIM_VALIDITY=14,PS_SAVED_STATE=16,
 PS_ROUTE_SLOT=0x3a,PS_ROUTE_PROGRESS=0x3c,PS_ROUTE_LENGTH=0x3e,PS_LINKED_ACTOR=0x68
};
uint8_t restore_pyramid_worker_claim_active(int32_t building);
int32_t restore_pyramid_available_worker_slot(int32_t building);
uint8_t restore_pyramid_piece_has_pending_work(int32_t building);
int32_t restore_pyramid_find_paired_work_piece(int32_t building,uint8_t *second);
int32_t restore_pyramid_paired_worker(int32_t building);
uint8_t restore_monument_worker_route_has_gap(int32_t actor);
void restore_pyramid_worker_select_piece(int32_t actor,uint8_t refresh_position,
 uint8_t failed_state,uint8_t ready_state,uint8_t travelling_state);
uint8_t FUN_004eea10(int32_t building);
int32_t FUN_004ef100(int32_t building);
uint8_t FUN_004efd90(int32_t building);
int32_t FUN_004f27b0(int32_t building,uint8_t *second);
int32_t FUN_004f28e0(int32_t building);
uint8_t FUN_004f2ba0(int32_t actor);
void FUN_004f0ef0(int32_t actor,uint8_t refresh_position,uint8_t failed_state,uint8_t ready_state,uint8_t travelling_state);
#endif
