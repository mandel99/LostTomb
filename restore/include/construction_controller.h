#ifndef RESTORE_RESTORE_CONSTRUCTION_C_00474700_H
#define RESTORE_RESTORE_CONSTRUCTION_C_00474700_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_CONSTRUCTION_FRAME_DEFINED
#define RESTORE_RESTORE_CONSTRUCTION_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_construction_frame;
typedef void (*restore_construction_frame_call)(uint32_t,uint32_t,struct restore_construction_frame *);
struct restore_construction_frame { uint32_t r[8],flags; restore_construction_frame_call boundary; };
#endif
void restore_construction_C_00474700(struct restore_construction_frame *frame);
void restore_construction_C_004743f0(struct restore_construction_frame *frame);
void restore_construction_dispatch(uint32_t site,uint32_t target,struct restore_construction_frame *frame);
void FUN_00474700(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2);
void FUN_004743f0(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2);
#endif
