#ifndef RESTORE_MENU_RESOURCE_RENDER_H
#define RESTORE_MENU_RESOURCE_RENDER_H
#include <stdint.h>
struct restore_resource_render_frame;
typedef void (*restore_resource_render_call)(uint32_t,uint32_t,struct restore_resource_render_frame *);
struct restore_resource_render_frame {uint32_t r[8],flags;restore_resource_render_call boundary;};
/* Native member receivers remain explicit in the raw entry. 449750 RET16;
 * all other newly owned bodies RET0. Town iterators use their true native
 * RET4/RET8 inside dispatch. No physical ECX or native callback casts. */
void restore_resource_render_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
void restore_resource_render_dispatch(uint32_t site,uint32_t target,struct restore_resource_render_frame *f);
void restore_resource_render_C_00520000(struct restore_resource_render_frame *f);
void FUN_00520000(void);
void restore_resource_render_C_004e1870(struct restore_resource_render_frame *f);
void restore_resource_render_C_004e1a50(struct restore_resource_render_frame *f);
void FUN_004e1870(void);
void FUN_004e1a50(void);
void FUN_0052ec50(void);
void FUN_0052f8b0(void);
void FUN_0052f850(void);
uint32_t FUN_00444980(uint32_t value);
void FUN_0040fb10(int32_t p0,int32_t p1);
void FUN_004cd3e0(int32_t p0,int32_t p1,int32_t p2,int32_t p3);
void FUN_004cd490(int32_t p0);
void FUN_004cd4e0(int32_t p0,int32_t p1,int32_t p2);
void FUN_004cd510(int32_t p0,int32_t p1,int32_t p2,int32_t p3,int32_t p4);
void FUN_004cd710(int32_t p0,int32_t p1,int32_t p2,int32_t p3);
void FUN_004cd810(void);
void FUN_004cd850(void);
void FUN_004cd890(int32_t p0,int32_t p1,int32_t p2,int32_t p3);
void FUN_004cd930(int32_t p0,int32_t p1,int32_t p2,int32_t p3);
void FUN_004cd9d0(int32_t p0,int32_t p1,int32_t p2,int32_t p3,int32_t p4);
uint8_t FUN_004cdc30(int32_t p0,int32_t p1);
uint32_t FUN_0052f790(int32_t image,uint32_t state);
void restore_resource_render_C_0040fb10(struct restore_resource_render_frame *f);
void restore_resource_render_C_00444980(struct restore_resource_render_frame *f);
void restore_resource_render_C_004449d0(struct restore_resource_render_frame *f);
void restore_resource_render_C_00447430(struct restore_resource_render_frame *f);
void restore_resource_render_C_004477d0(struct restore_resource_render_frame *f);
void restore_resource_render_C_00449370(struct restore_resource_render_frame *f);
void restore_resource_render_C_00449380(struct restore_resource_render_frame *f);
void restore_resource_render_C_00449390(struct restore_resource_render_frame *f);
void restore_resource_render_C_00449750(struct restore_resource_render_frame *f);
void restore_resource_render_C_0044c110(struct restore_resource_render_frame *f);
void restore_resource_render_C_0044d750(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd3e0(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd490(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd4e0(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd510(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd710(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd810(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd850(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd890(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd930(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cd9d0(struct restore_resource_render_frame *f);
void restore_resource_render_C_004cdc30(struct restore_resource_render_frame *f);
void restore_resource_render_C_0052ec50(struct restore_resource_render_frame *f);
void restore_resource_render_C_0052f790(struct restore_resource_render_frame *f);
void restore_resource_render_C_0052f850(struct restore_resource_render_frame *f);
void restore_resource_render_C_0052f8b0(struct restore_resource_render_frame *f);
#endif
