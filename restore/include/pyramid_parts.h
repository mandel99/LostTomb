#ifndef RESTORE_PYRAMID_PARTS_H
#define RESTORE_PYRAMID_PARTS_H
#include <stdint.h>
#define PY_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define PY_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define PY_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define PY_I(v) ((int32_t)(uint32_t)(v))
/* Offsets inside the 264-byte building record, proved by the native loads.
   State word64 is reset here; its wider gameplay interpretation is pending. */
enum RestorePyramidField {
 PY_SIZE=3,PY_X=6,PY_Y=8,PY_CELL=12,PY_BUILDING_KIND=16,PY_PIECE_INDEX=18,
 PY_PREVIOUS=56,PY_NEXT=58,PY_LAYOUT_KIND=84,PY_RENDER_PHASE=85,
 PY_SPRITE_STEP=86,PY_LAYER_INDEX=87,PY_TEMPLATE_HEIGHT=88,
 PY_WORKER_POSITION_A=89,PY_WORKER_POSITION_B=90,PY_CELL_PHASE_MASK=91,
 PY_WORKER_X_A=92,PY_WORKER_Y_A=94,PY_WORKER_X_B=96,PY_WORKER_Y_B=98,
 PY_STATE_WORD_64=100,PY_WORKER_IMAGE=104,PY_TRANSIENT_IMAGE=108,PY_CONSTRUCTION_AMOUNT=200
};
static __inline uint32_t restore_pyramid_record(int32_t building)
{return 0x7999d8u+(uint32_t)building*264u;}
void restore_pyramid_stamp_footprint(int32_t building);
void restore_pyramid_create_parts(uint32_t owner,int32_t x,int32_t y,uint32_t unused,int32_t building);
void restore_pyramid_update_piece_image(int32_t building,int32_t rotation,int32_t kind);
void restore_pyramid_refresh_all_parts(int32_t root);
void restore_pyramid_mark_special_parts(int32_t root);
void restore_pyramid_update_finished_piece(int32_t building,int32_t rotation,int32_t kind);
void restore_pyramid_create_entrance(int32_t building);
void FUN_004ee6c0(int32_t building);
void FUN_004ee780(uint32_t owner,int32_t x,int32_t y,uint32_t unused,int32_t building);
void FUN_004edf50(int32_t building,int32_t rotation,int32_t kind);
void FUN_004ee4a0(int32_t root);
void FUN_004f3c90(int32_t root);
void FUN_004f4160(int32_t building,int32_t rotation,int32_t kind);
void FUN_004f5f10(int32_t building);
#endif
