#ifndef RESTORE_MONUMENT_WORKER_MOTION_H
#define RESTORE_MONUMENT_WORKER_MOTION_H
#include <stdint.h>
#define MW_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MW_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MW_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MW_H(a) ((int32_t)(int16_t)MW_W(a))
/* Native actor records are 388 bytes. Building links and travel counters
   are signed WORDs; map indices and view settings are full DWORDs. */
enum RestoreMonumentWorkerField {
 MW_TYPE=0xa,MW_DIRECTION=0x11,MW_X=0x14,MW_Y=0x16,MW_CELL=0x20,
 MW_TARGET_X=0x24,MW_TARGET_Y=0x26,MW_TARGET_CELL=0x28,
 MW_ACTION_CLOCK=0x36,MW_STATE=0x38,MW_FROM_BUILDING=0x4a,MW_FROM_POSITION=0x4c,
 MW_TO_BUILDING=0x4e,MW_TO_POSITION=0x50,
 MW_TRAVEL_PROGRESS=0x52,MW_TRAVEL_LENGTH=0x54,MW_MONUMENT_ACTION=0x120
};
static __inline uint32_t restore_monument_worker_record(int32_t actor)
{return 0xbcd5f8u+(uint32_t)actor*388u;}
static __inline uint32_t restore_monument_worker_building(int32_t building)
{return 0x7999d8u+(uint32_t)building*264u;}
void restore_monument_worker_reset_travel(int32_t actor);
void restore_monument_worker_begin_travel(int32_t actor,int32_t building,uint16_t position,uint8_t select_action);
uint8_t restore_monument_worker_advance_travel(int32_t actor,int16_t amount);
void restore_monument_worker_snap_to_target(int32_t actor);
int32_t restore_pyramid_next_work_piece(int32_t building);
void restore_pyramid_collect_work_pieces(int32_t building);
void restore_monument_worker_find_work(int32_t actor);
void FUN_004f1cb0(int32_t actor);
void FUN_004f02f0(int32_t actor,int32_t building,uint16_t position,uint8_t select_action);
uint8_t FUN_004f1d10(int32_t actor,int16_t amount);
void FUN_004f2410(int32_t actor);
int32_t FUN_004f2190(int32_t building);
void FUN_004ef450(int32_t building);
void FUN_004f2230(int32_t actor);
#endif
