#include "widget_group_draw.h"
#include "widget_position.h"
#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

#include "pyramid_widget_draw.h"

/* 4206F0 reads the whole signed DWORD, divides toward zero, then masks bit0.
   The original src DAT byte declaration cannot express the native load. */
void restore_draw_widget_group_004206f0(int32_t x,int32_t y)
{
 uint32_t orientation=(uint32_t)((int32_t)U(0x00bca2e0u)/2)&1u;
 uint32_t base_x,base_y,table,image;
 int32_t out_x,out_y;
 if(orientation!=FUN_0041f8b0())FUN_0041f8c0((int32_t)orientation);
 base_y=(uint32_t)y;base_x=(uint32_t)x;table=0x0059edf0u;
 do{
  uint32_t self=U(table);
  restore_widget_position_0041f910(self,&out_x,&out_y,&image);
  if(image){
   (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,(int32_t)image,
     (int32_t)(base_x+(uint32_t)out_x),(int32_t)(base_y+(uint32_t)out_y));
  }
  table+=16u;
 }while(table<0x0059efa0u);
}
