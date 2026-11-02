#ifndef RESTORE_BUILDING_CONTROLLERS_H
#define RESTORE_BUILDING_CONTROLLERS_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_BUILDING_CONTROL_FRAME_DEFINED
#define RESTORE_RESTORE_BUILDING_CONTROL_FRAME_DEFINED
/* Registers: EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI.
   boundary receives the native entry or IAT slot and an explicit call frame.
   Bind genuine callees; unsupported calls must be reported by the caller. */
struct restore_building_control_frame;
typedef void (*restore_building_control_frame_call)(uint32_t,uint32_t,struct restore_building_control_frame *);
struct restore_building_control_frame { uint32_t r[8],flags; restore_building_control_frame_call boundary; };
#endif
void restore_building_control_C_0046b100(struct restore_building_control_frame *frame);
void restore_building_control_C_00472790(struct restore_building_control_frame *frame);
void restore_building_control_C_004b6e10(struct restore_building_control_frame *frame);
void restore_building_control_C_004bfb30(struct restore_building_control_frame *frame);
void restore_building_control_C_004c0d50(struct restore_building_control_frame *frame);
void restore_building_control_C_004c3420(struct restore_building_control_frame *frame);
void restore_building_control_C_004c0be0(struct restore_building_control_frame *frame);
void restore_building_control_C_004c0860(struct restore_building_control_frame *frame);
void restore_building_control_C_004c53d0(struct restore_building_control_frame *frame);
void restore_building_control_C_004b7c80(struct restore_building_control_frame *frame);
void restore_building_control_C_004c52d0(struct restore_building_control_frame *frame);
int32_t FUN_0046b100(int32_t building);
void FUN_00472790(int32_t count,int32_t group,int32_t x,int32_t y);
int32_t FUN_004b6e10(int32_t building);
void FUN_004bfb30(int32_t building);
void FUN_004c0d50(int32_t building,int32_t image_offset);
void FUN_004c3420(int32_t building,int32_t x,int32_t y,uint32_t owner,int32_t kind,uint32_t initialize);
void FUN_004c0be0(int32_t building);
void FUN_004c0860(int32_t x,int32_t y,int32_t group,uint32_t image_offset);
int32_t FUN_004c53d0(int32_t category);
void FUN_004b7c80(void);
void restore_building_control_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
void FUN_00454b40(int32_t building),FUN_0046b740(int32_t building);
uint32_t FUN_004539a0(void),FUN_00453af0(int32_t building);
uint32_t FUN_00453b40(int32_t building,int32_t level);
void restore_building_control_C_004528b0(struct restore_building_control_frame *);
void restore_building_control_C_00454b40(struct restore_building_control_frame *);
void restore_building_control_C_0046b740(struct restore_building_control_frame *);
void restore_building_control_C_004539a0(struct restore_building_control_frame *);
void restore_building_control_C_00453af0(struct restore_building_control_frame *);
void restore_building_control_C_00453b40(struct restore_building_control_frame *);
void restore_building_control_C_0046b840(struct restore_building_control_frame *);
void restore_building_control_C_0046b480(struct restore_building_control_frame *);
void restore_building_control_C_0046b940(struct restore_building_control_frame *);
void restore_building_control_C_0046b9e0(struct restore_building_control_frame *);
void restore_building_control_C_0046bab0(struct restore_building_control_frame *);
void restore_building_control_C_0046bbc0(struct restore_building_control_frame *);
void restore_building_control_C_0046bcb0(struct restore_building_control_frame *);
void restore_building_control_C_0046c1c0(struct restore_building_control_frame *);
void restore_building_control_C_0046cb30(struct restore_building_control_frame *);
void restore_building_control_C_0046ccf0(struct restore_building_control_frame *);
void restore_building_control_C_0046bda0(struct restore_building_control_frame *);
void restore_building_control_C_0046be90(struct restore_building_control_frame *);
void restore_building_control_C_0046c500(struct restore_building_control_frame *);
void restore_building_control_C_00452980(struct restore_building_control_frame *);
void restore_building_control_C_00452a20(struct restore_building_control_frame *);
void restore_building_control_C_00452b00(struct restore_building_control_frame *);
void restore_building_control_C_00452bd0(struct restore_building_control_frame *);
void restore_building_control_C_00452ca0(struct restore_building_control_frame *);
void restore_building_control_C_00452d70(struct restore_building_control_frame *);
void restore_building_control_C_00452e40(struct restore_building_control_frame *);
void restore_building_control_C_00452f10(struct restore_building_control_frame *);
void restore_building_control_C_00452fe0(struct restore_building_control_frame *);
void restore_building_control_C_004530b0(struct restore_building_control_frame *);
void restore_building_control_C_004531c0(struct restore_building_control_frame *);
void restore_building_control_C_00453280(struct restore_building_control_frame *);
void restore_building_control_C_00453340(struct restore_building_control_frame *);
void restore_building_control_C_00453400(struct restore_building_control_frame *);
void restore_building_control_C_00453500(struct restore_building_control_frame *);
void restore_building_control_C_004535c0(struct restore_building_control_frame *);
void restore_building_control_C_00453680(struct restore_building_control_frame *);
void restore_building_control_C_00453740(struct restore_building_control_frame *);
void restore_building_control_C_00453840(struct restore_building_control_frame *);
void restore_building_control_C_00453900(struct restore_building_control_frame *);
#endif
