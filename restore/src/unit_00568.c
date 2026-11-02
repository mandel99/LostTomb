#include "ferry_preview.h"
#include "actor_coordinates.h"
#include "map_commit_links.h"
#include "footprint.h"
#include "preview_geometry.h"
#include "route_query_model.h"
#include "sprite_render.h"
#include "graphics_53f250.h"
#include "placement_preview.h"
static void invalid_outline(volatile int32_t *sx,volatile int32_t *sy)
{
 uint32_t i;
 for(i=0;i<4u;++i)FUN_0053f4f0(FRY_I(FRY_U(0xf56be4u)),FRY_I(FRY_U(0x5cfcf0u+i*8u)+(uint32_t)*sx),
  FRY_I(FRY_U(0x5cfcf4u+i*8u)+(uint32_t)*sy-FRY_U(0x70e44cu)),0xfffff863u);
}
static uint32_t landing_image(int32_t orientation)
{
 uint32_t image=FRY_U(0xf55bb4u+FRY_U(0x5d1184u)*8u)+FRY_U(0x5d1188u);
 return image+(uint32_t)restore_ferry_image_orientation(orientation);
}
/* Whole native43D300:1041 bytes,312 instructions. The route array owns
   1000 DWORDs; exactly its first250 are initialized by the original. */
void restore_ferry_preview(void)
{
 volatile int32_t sx,sy,ex,ey;int32_t orientation,fit,start_x,start_y,cached_x,cached_y,path_count;
 uint32_t image,building,record,layout,other_orientation,i,result,path[1000];uint8_t existing=0;
 FRY_B(0xbca314u)=0;
 FUN_004f0ba0(FRY_I(FRY_U(0xbca304u)),FRY_I(FRY_U(0xbca308u)),(int32_t *)&sx,(int32_t *)&sy);
 sy=FRY_I((uint32_t)sy+FRY_U(0x70e44cu));
 if(!FRY_U(0xbca310u)){
  restore_ferry_scan_crossing(FRY_I(FRY_U(0xbca304u)),FRY_I(FRY_U(0xbca308u)));
  orientation=FRY_I(FRY_U(0xbc96e0u));if(orientation<0)orientation=FRY_I((uint32_t)orientation+8u);
  cached_x=sx;cached_y=FRY_I((uint32_t)sy-FRY_U(0x70e44cu));FRY_U(0x70e13cu)=0;
  fit=FUN_0047cc70(FRY_I(FRY_U(0xbca304u)),FRY_I(FRY_U(0xbca308u)),1,0);
  building=(uint32_t)FRY_H(0xb4e6fcu+FRY_U(0xbca300u)*2u);
  if(FUN_00486cf0(FRY_U(0xbca300u))&&FRY_U(0x799a9cu+building*264u)==UINT32_MAX){
   existing=1;
   restore_ferry_highlight_approaches(orientation,FRY_I(FRY_U(0xbca304u)),FRY_I(FRY_U(0xbca308u)),cached_x,cached_y,FRY_I(FRY_U(0xbc96e4u)));
   FRY_W(0x70e120u)=0x1fe3;image=landing_image(orientation);
   FUN_0053e590(FRY_I(image),sx,FRY_I((uint32_t)sy-FRY_U(0x70e44cu)));
   FUN_0053f250(FRY_I(image),sx,FRY_I((uint32_t)sy-FRY_U(0x70e44cu)));FRY_W(0x70e120u)=0;
  }
  if(fit==0&&restore_ferry_highlight_approaches(orientation,FRY_I(FRY_U(0xbca304u)),FRY_I(FRY_U(0xbca308u)),cached_x,cached_y,FRY_I(FRY_U(0xbc96e4u)))){
   FRY_B(0xbca314u)=1;FUN_0043dd70(0,0);FRY_W(0x70e120u)=0;return;
  }
  if(!existing){FRY_B(0xbca314u)=0;invalid_outline(&sx,&sy);}return;
 }
 for(i=0;i<250u;++i)path[i]=UINT32_MAX;
 orientation=FRY_I(FUN_0047cbd0(FRY_I(FRY_U(0xbca304u)),FRY_I(FRY_U(0xbca308u))));
 record=0x7999d8u+FRY_U(0xbc9800u)*264u;path_count=0;
 layout=FRY_B(record+3u);other_orientation=(uint32_t)FRY_B(record+170u)*2u;
 start_x=FRY_H(record+6u);start_y=FRY_H(record+8u);
 if(FUN_00486cf0(FRY_U(0xbca300u))){
  building=(uint32_t)FRY_H(0xb4e6fcu+FRY_U(0xbca300u)*2u);
  if(building!=FRY_U(0xbc9800u)&&FRY_U(0x799a9cu+building*264u)==UINT32_MAX){
   FRY_W(0x70e120u)=0x1fe3;image=landing_image(FRY_I((uint32_t)orientation*2u));
   FUN_0053e590(FRY_I(image),sx,FRY_I((uint32_t)sy-FRY_U(0x70e44cu)));
   FUN_0053f250(FRY_I(image),sx,FRY_I((uint32_t)sy-FRY_U(0x70e44cu)));FRY_W(0x70e120u)=0;return;
  }
 }
 FUN_004f0ba0(start_x,start_y,(int32_t *)&ex,(int32_t *)&ey);
 restore_ferry_highlight_approaches(FRY_I(other_orientation),start_x,start_y,ex,ey,FRY_I(FRY_U(0xbc96e4u)));
 result=FUN_00486a30(FRY_I(FRY_U(0xbca304u)),FRY_I(FRY_U(0xbca308u)),start_x,start_y,layout,
   FRY_I((uint32_t)orientation*2u),FRY_I((other_orientation+4u)&7u),path,&path_count);
 if(result==1u){FRY_B(0xbca314u)=1;FUN_0043dd70(0,0);return;}
 FRY_B(0xbca314u)=0;invalid_outline(&sx,&sy);
}
void FUN_0043d300(void){restore_ferry_preview();}
