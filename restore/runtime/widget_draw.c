#include "widget_group_draw.h"
#include "widget_draw.h"
#include "widget_position.h"
#include "pyramid_widget_draw.h"
#include "sprite_render.h"
#include <stdlib.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define A(n) U(sp+4u*(n))

static void draw_dispatch(uint32_t site,uint32_t target,struct restore_widget_draw_frame *f)
{
 uint32_t sp=f->r[4];(void)site;
 switch(target){
 case 0x41fff0u:restore_widget_draw_C_0041fff0(f);return;
 case 0x420050u:restore_widget_draw_C_00420050(f);return;
 case 0x4206f0u:restore_widget_draw_C_004206f0(f);return;
 case 0x420a30u:restore_widget_draw_C_00420a30(f);return;
 case 0x420d60u:restore_widget_draw_C_00420d60(f);return;
 case 0x421060u:restore_widget_draw_C_00421060(f);return;
 case 0x421920u:restore_widget_draw_C_00421920(f);return;
 case 0x41f910u:case 0x41fad0u:
  restore_widget_position_raw(target,f->r,&f->flags);return;
 case 0x41f8b0u:f->r[0]=FUN_0041f8b0();break;
 case 0x41f8c0u:FUN_0041f8c0((int32_t)A(1));break;
 case 0x53e590u:
  f->r[0]=(uint32_t)restore_sprite_draw((RestoreSpriteLibraries *)(uintptr_t)f->r[1],
    (int32_t)A(1),(int32_t)A(2),(int32_t)A(3));
  f->r[4]=sp+16u;return;
 default:abort();
 }
 f->r[4]=sp+4u;
}
#undef A

void restore_widget_draw_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_widget_draw_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=draw_dispatch;
 draw_dispatch(0,target,&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}

void FUN_0041fff0(int32_t x,int32_t y){restore_draw_widget_group_0041fff0(x,y);}
void FUN_00420050(int32_t x,int32_t y){restore_draw_widget_group_00420050(x,y);}
void FUN_004206f0(int32_t x,int32_t y){restore_draw_widget_group_004206f0(x,y);}
void FUN_00420a30(int32_t x,int32_t y){restore_draw_widget_group_00420a30(x,y);}
void FUN_00420d60(int32_t x,int32_t y){restore_draw_widget_group_00420d60(x,y);}
void FUN_00421060(int32_t x,int32_t y){restore_draw_widget_group_00421060(x,y);}
void FUN_00421920(int32_t x,int32_t y,int32_t building){restore_draw_building_widget_group(x,y,building);}
