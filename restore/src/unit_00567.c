#include "ferry_preview.h"
#include "actor_coordinates.h"
#include "map_commit_links.h"
#include "footprint.h"
#include "preview_geometry.h"
#include "route_query_model.h"
#include "sprite_render.h"
#include "graphics_53f250.h"
#include "editor_brush_validation.h"
/* Whole native43D030:709 bytes,228 instructions. Expand laterally to five
   cells while scanning twice the supplied length. Preserve cached fit and
   ownership values across provider calls and full WORD tint writes. */
uint8_t restore_ferry_highlight_approaches(int32_t orientation,int32_t x,int32_t y,int32_t screen_x,int32_t screen_y,int32_t length)
{
 int32_t depth,radius=0,lateral,dx,dy,near_x,near_y,fit,wanted,path_count=0,maximum=FRY_I((uint32_t)length*2u);
 volatile int32_t px,py,hx,hy;uint32_t base,cell,building,image,i;uint8_t found=0;
 if(maximum<0)return 0;
 for(depth=0;depth<=maximum;++depth){
  base=(uint32_t)x+(uint32_t)y*228u;wanted=FRY_I((uint32_t)orientation+4u)%8;
  for(lateral=-radius;lateral<=radius;++lateral){
   switch(orientation){case 0:dx=lateral;dy=-depth;break;case 2:dx=depth;dy=-lateral;break;
   case 4:dx=-lateral;dy=depth;break;default:dx=-depth;dy=lateral;break;}
   near_x=FRY_I((uint32_t)x+(uint32_t)dx);near_y=FRY_I((uint32_t)y+(uint32_t)dy);
   px=screen_x;py=screen_y;FUN_004f0ba0(near_x,near_y,(int32_t *)&px,(int32_t *)&py);
   base=FRY_U(0xbca2d8u)+(uint32_t)x+(uint32_t)y*228u;
   cell=FRY_U(0xbca2d8u)+(uint32_t)near_y*228u+(uint32_t)near_x;
   fit=FUN_0047cc70(near_x,near_y,1,0);building=(uint32_t)FRY_H(0xb4e6fcu+cell*2u);
   if(FUN_00486cf0(cell)&&FRY_U(0x799a9cu+building*264u)==UINT32_MAX&&cell!=base&&
      FRY_W(0xb4e6fcu+cell*2u)!=FRY_W(0xb4e6fcu+base*2u)){
    FUN_004f0ba0(near_x,near_y,(int32_t *)&hx,(int32_t *)&hy);
    FRY_W(0x70e120u)=0x1fe3;
    image=FRY_U(0xf55bb4u+FRY_U(0x5d1184u)*8u)+FRY_U(0x5d1188u);
    image+=(uint32_t)restore_ferry_image_orientation(wanted);
    FUN_0053e590(FRY_I(image),hx,hy);FUN_0053f250(FRY_I(image),hx,hy);
    FRY_W(0x70e120u)=0;found=1;
   }
   image=FUN_0047cbd0(near_x,near_y)*2u;
   if(image==(uint32_t)wanted&&fit==0&&FUN_00486a30(near_x,near_y,x,y,2,FRY_I(image),wanted,0,&path_count)){
    for(i=0;i<4u;++i)FUN_0053f4f0(FRY_I(FRY_U(0xf56be4u)),FRY_I(FRY_U(0x5cfcf0u+i*8u)+(uint32_t)px),FRY_I(FRY_U(0x5cfcf4u+i*8u)+(uint32_t)py),0x1fe3u);
    found=1;
   }
  }
  if(radius<5)++radius;
 }
 return found;
}
uint8_t FUN_0043d030(int32_t orientation,int32_t x,int32_t y,int32_t screen_x,int32_t screen_y,int32_t length)
{return restore_ferry_highlight_approaches(orientation,x,y,screen_x,screen_y,length);}
