#ifndef RESTORE_WIDGET_DRAW_H
#define RESTORE_WIDGET_DRAW_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_WIDGET_DRAW_FRAME_DEFINED
#define RESTORE_RESTORE_WIDGET_DRAW_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_widget_draw_frame;
typedef void (*restore_widget_draw_frame_call)(uint32_t,uint32_t,struct restore_widget_draw_frame *);
struct restore_widget_draw_frame { uint32_t r[8],flags; restore_widget_draw_frame_call boundary; };
#endif
void restore_widget_draw_C_0041fff0(struct restore_widget_draw_frame *f);
void restore_widget_draw_C_00420050(struct restore_widget_draw_frame *f);
void restore_widget_draw_C_004206f0(struct restore_widget_draw_frame *f);
void restore_widget_draw_C_00420a30(struct restore_widget_draw_frame *f);
void restore_widget_draw_C_00420d60(struct restore_widget_draw_frame *f);
void restore_widget_draw_C_00421060(struct restore_widget_draw_frame *f);
void restore_widget_draw_C_00421920(struct restore_widget_draw_frame *f);
void restore_widget_draw_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
void FUN_0041fff0(int32_t x,int32_t y);
void FUN_00420050(int32_t x,int32_t y);
void FUN_004206f0(int32_t x,int32_t y);
void FUN_00420a30(int32_t x,int32_t y);
void FUN_00420d60(int32_t x,int32_t y);
void FUN_00421060(int32_t x,int32_t y);
void FUN_00421920(int32_t x,int32_t y,int32_t building);
#endif
