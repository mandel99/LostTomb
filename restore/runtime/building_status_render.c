/* Complete pinned435E70:171 instructions, no added orientation clamp. */
#include "building_status_render.h"
#include "menu_prepare_integer.h"
#include "menu_building_kind.h"
#include "graphics_53f250.h"
#include "sprite_render.h"
#include "world_building_draw.h"
/* 442280 is a real open dependency. Its legacy second C argument is an
 * index encoded in a pointer slot; preserve the bits until its typed repair.
 * No computed result or renderer is synthesized here. */
extern uint32_t FUN_00442280(int32_t building,int32_t *tile_slot,int32_t x,int32_t y);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(v) ((int32_t)(uint32_t)(v))
void FUN_00435e70(int32_t building,int32_t x,int32_t y)
{
 uint32_t root=FUN_00475b50((uint32_t)building),root_offset=root*264u,tile,image,footprint;
 uint32_t px=(uint32_t)x,py=(uint32_t)y,shift_x,shift_y,flags,selected=(uint32_t)building,bit;
 int32_t kind;
 if(!U(0x799ad8u+root_offset))return;
 tile=U(0x70e488u);flags=B(0xb285ccu+tile);image=U(0xb7482cu+tile*4u);
 footprint=flags&15u;U(0xd3b5c0u)=image;
 switch(footprint){
 case 0:break;
 case 1:py-=U(0x70e44cu);px+=U(0x70e45cu)+1u;break;
 case 2:py-=U(0x70e448u);px+=U(0x70e458u)+2u;break;
 case 3:py-=U(0x70e444u);px+=U(0x70e454u)+3u;break;
 case 4:py-=U(0x70e440u);px+=U(0x70e450u)+4u;break;
 case 5:py-=U(0x70e440u);py-=U(0x70e44cu);shift_x=U(0x70e45cu);px+=U(0x70e450u)+shift_x+5u;break;
 default:goto after_upper;
 }
 (void)restore_sprite_draw_upper(RESTORE_SPRITE_LIBRARIES,S(image),S(px),S(py));
after_upper:
 if(!B(0x7999ddu+selected*264u)){
  if(H(0x7999e8u+selected*264u)==71u){
   tile=U(0x70e488u);(void)FUN_00442280(building,(int32_t *)(uintptr_t)tile,x,y);
  }else if((uint8_t)FUN_00488030(selected)){
   tile=U(0x70e488u);
   if(B(0xb67d1cu+tile)&64u){
    py=(uint32_t)y-U(0x70e448u);px=U(0x70e458u)+(uint32_t)x+2u;
    restore_world_building_draw_00488c20(0xbca350u,building,S(px),S(py),1u);
   }
  }else{
   kind=(int16_t)H(0x7999e8u+root_offset);
   if(kind>=65&&kind<=69){
    if(!footprint)return;
    tile=U(0x70e488u);if(!(B(0xb67d1cu+tile)&64u))return;
    selected=root;
   }
  }
 }
 shift_x=U(0x70e45cu);shift_y=U(0x70e44cu);
 px=shift_x*footprint+(uint32_t)x+18u;py=shift_y*footprint+(uint32_t)y-2u;
 for(bit=0;bit<14;++bit){
  if(U(0x799ad8u+selected*264u)&(1u<<bit)){
   image=U(0xf5644cu)+(bit==12u?9u:bit);
   (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,S(image),S(px),S(py));py-=14u;
  }
 }
}
