#ifndef RESTORE_PLACEMENT_PREVIEW_H
#define RESTORE_PLACEMENT_PREVIEW_H
#include "restore_types.h"
#include "sprite_render.h"
#include "graphics_53f250.h"
#define PV_I(a) (*(int32_t *)(uintptr_t)(a))
#define PV_U(a) (*(uint32_t *)(uintptr_t)(a))
#define PV_W(a) (*(uint16_t *)(uintptr_t)(a))
#define PV_S(a) (*(int16_t *)(uintptr_t)(a))
#define PV_B(a) (*(uint8_t *)(uintptr_t)(a))
static __inline int32_t pv_add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t pv_sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static __inline int32_t pv_mul(int32_t a,int32_t b){return (int32_t)((uint32_t)a*(uint32_t)b);}
void FUN_0043b630(void);
void restore_placement_preview_initialize_classes(void);
void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);
void FUN_004f0ab0(int32_t row,int32_t column,int32_t *map_y,int32_t *map_x);
int32_t restore_placement_mask_sprite(RestoreSpriteLibraries *self,int32_t image,int32_t x,int32_t y,uint32_t mask);
int32_t FUN_0053f4f0(int32_t image,int32_t x,int32_t y,uint32_t mask);
void restore_placement_member_004e5490(void *self,const void *pieces,int32_t count);
void restore_placement_member_00545fc0(void *self,const void *pieces,int32_t count);
code *restore_placement_rule(uint32_t token);
uint8_t restore_placement_predicate(void *self,uint32_t terrain,int32_t piece);
uint8_t FUN_0046f9a0(int32_t x,int32_t y,int32_t flags);
uint32_t FUN_004893c0(int32_t x,int32_t y,int32_t width,int32_t height,uint32_t bits);
void restore_placement_member_adapter(uint32_t address,void *self);
int16_t restore_placement_key_state(int32_t key);
void FUN_0043d720(void),FUN_00441490(void),FUN_00541c00(void),FUN_004debe0(void),FUN_004edbb0(void),FUN_0043e090(void),FUN_004bfcd0(void),FUN_00441f80(void),FUN_0043d300(void);
void FUN_0043df10(int32_t image),FUN_0043dd70(uint32_t first,uint32_t second),FUN_0043f5b0(int32_t mode),FUN_004c0f30(int32_t mode),FUN_004c2f50(int32_t mode);
uint32_t FUN_00482650(int32_t x,int32_t y,int32_t w,int32_t h,uint32_t bits),FUN_00482720(int32_t x,int32_t y,int32_t w,int32_t h,code *predicate),FUN_004828e0(int32_t x,int32_t y,int32_t w,int32_t h,uint32_t bits);
uint32_t FUN_004829b0(int32_t x,int32_t y,int32_t w,int32_t h,uint32_t bits,int32_t flag);
uint32_t FUN_004ec410(uint32_t terrain,char mode),FUN_00477d40(int32_t x,int32_t y);
uint32_t FUN_0046fde0(uint32_t mode);
uint32_t FUN_0047ed60(uint32_t tile),FUN_0047b920(void);
int32_t FUN_00487f80(int32_t mode),FUN_00489050(int32_t x,int32_t y,int32_t w,int32_t h);
uint32_t FUN_00489470(int32_t x,int32_t y,int32_t w,int32_t h),FUN_0051c510(int32_t tile),FUN_0051c390(int32_t tile),FUN_0051c630(int32_t tile);
int32_t FUN_0051c5f0(uint32_t tile,int32_t image);
/* The original51C460 produces fullEAX values1,15,16,36,42; its existingC
   symbol returns char. Promotion preserves every canonical native result. */
char FUN_0051c460(int32_t tile);
uint8_t FUN_0047ed30(int32_t tile,uint32_t bits),FUN_0047ed00(int32_t tile,uint32_t bits);
void FUN_0047e870(void),FUN_0047f200(int32_t x,int32_t y,int32_t bridge_kind);
int32_t FUN_00484be0(int32_t table,int32_t count,int32_t limit,char mode);
#endif
