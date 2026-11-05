#include "widget_group_draw.h"
#include "widget_position.h"
#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* 420A30: original table walk, with the omitted thiscall receiver explicit.
   x and y are deliberately reused as the child's out_y and out_image slots. */
void restore_draw_widget_group_00420a30(int32_t x,int32_t y)
{
 uint32_t base_x=(uint32_t)x,base_y=(uint32_t)y,table=0x005a8148u;
 int32_t out_x;
 do{
  uint32_t self=U(table),image;
  restore_widget_position_0041f910(self,&out_x,&x,(uint32_t *)&y);
  image=(uint32_t)y;
  if(image){
   (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,(int32_t)image,
     (int32_t)(base_x+(uint32_t)out_x),(int32_t)(base_y+(uint32_t)x));
  }
  table+=16u;
 }while(table<0x005a84c8u);
}
