#ifndef RESTORE_RESTORE_WARNING_C_004E47D0_H
#define RESTORE_RESTORE_WARNING_C_004E47D0_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_WARNING_FRAME_DEFINED
#define RESTORE_RESTORE_WARNING_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_warning_frame;
typedef void (*restore_warning_frame_call)(uint32_t,uint32_t,struct restore_warning_frame *);
struct restore_warning_frame { uint32_t r[8],flags; restore_warning_frame_call boundary; };
#endif
void restore_warning_C_004e47d0(struct restore_warning_frame *frame);
void restore_warning_C_004e3760(struct restore_warning_frame *frame);
void restore_warning_C_004e3aa0(struct restore_warning_frame *frame);
void restore_warning_C_004e3da0(struct restore_warning_frame *frame);
void restore_warning_C_004e3f60(struct restore_warning_frame *frame);
void restore_warning_C_004e4120(struct restore_warning_frame *frame);
void restore_warning_C_004e42d0(struct restore_warning_frame *frame);
void restore_warning_C_004e4550(struct restore_warning_frame *frame);
void restore_warning_C_004e4ed0(struct restore_warning_frame *frame);
void restore_warning_C_004e5070(struct restore_warning_frame *frame);
void FUN_004e3760(int32_t tool);
void FUN_004e3aa0(int32_t tool);
void FUN_004e3da0(int32_t tool);
void FUN_004e3f60(int32_t tool);
void FUN_004e4120(int32_t tool);
void FUN_004e42d0(int32_t tool);
void FUN_004e4550(int32_t tool);
void FUN_004e47d0(int32_t tool);
void FUN_004e4ed0(int32_t tool);
void FUN_004e5070(int32_t tool);
#endif
