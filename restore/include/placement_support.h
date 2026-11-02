#ifndef RESTORE_PLACEMENT_SUPPORT_H
#define RESTORE_PLACEMENT_SUPPORT_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_PLACEMENT_SUPPORT_FRAME_DEFINED
#define RESTORE_RESTORE_PLACEMENT_SUPPORT_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_placement_support_frame;
typedef void (*restore_placement_support_frame_call)(uint32_t,uint32_t,struct restore_placement_support_frame *);
struct restore_placement_support_frame { uint32_t r[8],flags; restore_placement_support_frame_call boundary; };
#endif
void restore_placement_support_C_00457ac0(struct restore_placement_support_frame *frame);
void restore_placement_support_C_00459fa0(struct restore_placement_support_frame *frame);
void restore_placement_support_C_0045b4d0(struct restore_placement_support_frame *frame);
void restore_placement_support_C_0047f530(struct restore_placement_support_frame *frame);
void restore_placement_support_C_004bff60(struct restore_placement_support_frame *frame);
void restore_placement_support_C_004c3730(struct restore_placement_support_frame *frame);
void restore_placement_support_C_00481420(struct restore_placement_support_frame *frame);
void restore_placement_support_C_00459ef0(struct restore_placement_support_frame *frame);
void restore_placement_support_C_0045b570(struct restore_placement_support_frame *frame);
void restore_placement_support_C_0045b490(struct restore_placement_support_frame *frame);
uint32_t FUN_00457ac0(int32_t x,int32_t y);
uint32_t FUN_00459fa0(void);
int32_t FUN_0045b4d0(int32_t player,int32_t resource,int32_t amount);
void FUN_0047f530(int32_t x,int32_t y,int32_t mode);
uint32_t FUN_004bff60(int32_t x,int32_t y,int32_t mode);
int32_t FUN_004c3730(int32_t x,int32_t y);
uint32_t FUN_00481420(int32_t x,int32_t y,int32_t mode);
uint32_t FUN_00459ef0(int32_t building);
int32_t FUN_0045b570(int32_t player,int32_t building,int32_t resource,int32_t amount);
int32_t FUN_0045b490(int32_t amount);
#endif
