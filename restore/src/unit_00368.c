#include "widget_group_draw.h"
#include "widget_position.h"
#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* 421920: ordinary version of the original switch/table walks. All memory
   fields below are DWORD native loads; record arithmetic wraps at 32 bits.
   building/x/y remain the three aliased output slots, while base_x/base_y
   and the original record offset survive the child calls. */
void restore_draw_building_widget_group(int32_t x,int32_t y,int32_t building)
{
 uint32_t record=(uint32_t)building*0x108u,table,end,base_x,base_y;
 switch(U(0x00799a40u+record)){
 case 0:table=0x005be600u;break;
 case 1:case 29:table=0x005be510u;break;
 case 2:table=0x005be540u;break;
 case 3:table=0x005be570u;break;
 case 4:table=0x005be750u;break;
 case 5:table=0x005be780u;break;
 case 6:table=0x005be630u;break;
 case 7:table=0x005be7b0u;break;
 case 8:table=0x005be7e0u;break;
 case 9:table=0x005be660u;break;
 case 10:table=0x005be690u;break;
 case 11:table=0x005be8a0u;break;
 case 12:table=0x005be810u;break;
 case 13:table=0x005be8d0u;break;
 case 14:table=0x005be900u;break;
 case 15:table=0x005be5a0u;break;
 case 16:table=0x005be5d0u;break;
 case 19:table=0x005be6c0u;break;
 case 20:table=0x005be6f0u;break;
 case 21:table=0x005be720u;break;
 case 22:table=0x005be840u;break;
 case 23:table=0x005be870u;break;
 case 24:table=0x005be930u;break;
 case 25:table=0x005be960u;break;
 case 26:table=0x005be990u;break;
 default:return;
 }
 base_y=(uint32_t)y;base_x=(uint32_t)x;end=table+0x30u;
 do{
  uint32_t selected=U(0x00799a38u+record);
  uint32_t table_selected=U(table-8u);
  if(selected==table_selected){
   uint32_t direction=U(0x00799a68u+record);
   uint32_t self=U(table),image;
   restore_widget_position_0041fad0(self,&building,&x,(uint32_t *)&y,(int32_t)direction);
   /* Every native continuation overwrites EAX from this output slot before
      testing it. The old unresolved-return expressions discarded their value. */
   image=(uint32_t)y;
   if(image){
    (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,(int32_t)image,
      (int32_t)(base_x+(uint32_t)building),(int32_t)(base_y+(uint32_t)x));
   }
  }
  table+=16u;
 }while(table<end);
}
