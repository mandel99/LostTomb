#ifndef RESTORE_RESTORE_C_0043F250_H
#define RESTORE_RESTORE_C_0043F250_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
#define RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_menu_input_frame;
typedef void (*restore_menu_input_frame_call)(uint32_t,uint32_t,struct restore_menu_input_frame *);
struct restore_menu_input_frame { uint32_t r[8],flags; restore_menu_input_frame_call boundary; };
#endif
/* These two predicates preserve full native EAX; the logical result is AL. */
uint32_t FUN_0043f250(uint32_t x,int32_t y,int32_t origin_x,int32_t origin_y,int32_t extent);
uint32_t FUN_0043f3a0(int32_t x,int32_t y,int32_t extent);
uint32_t FUN_00441040(int32_t extent,int32_t rows,int32_t columns,uint32_t *count,int32_t *locations);
void FUN_004413b0(int32_t x,int32_t y,int32_t kind);
uint32_t restore_construction_road_access(uint32_t x,int32_t y,int32_t origin_x,int32_t origin_y,int32_t extent);
uint32_t restore_construction_footprint_mask(int32_t x,int32_t y,int32_t extent);
uint32_t restore_construction_empty_slots(int32_t extent,int32_t rows,int32_t columns,uint32_t *count,int32_t *locations);
void restore_construction_occupancy(int32_t x,int32_t y,int32_t kind);
#endif
