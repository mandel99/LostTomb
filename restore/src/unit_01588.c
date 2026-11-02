#include "placement_preview.h"
#include "temple_upgrade.h"
#include "sprite_libraries.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))
/* Each native switch arm is an affine traversal of the same91 records.
   Descriptor: first record, rows, column/row record steps, x/y origins,
   column x/y steps, row x/y steps, and the record's variant DWORD index. */
static const int8_t tile_order[2][4][4][11]={
 {
  {
   {0,5,1,13,0,0,1,0,0,1,1},
   {77,2,-1,13,-5,-12,0,1,-1,0,4},
   {38,5,-1,13,-12,-2,1,0,0,-1,3},
   {13,2,1,-13,1,0,0,1,-1,0,2},
  },
  {
   {26,5,1,13,2,0,0,1,1,0,3},
   {12,5,-1,13,12,0,-1,0,0,1,2},
   {12,5,-1,13,0,-12,0,1,-1,0,1},
   {26,5,1,13,0,-2,-1,0,0,-1,4},
  },
  {
   {13,2,1,-13,0,-1,-1,0,0,1,1},
   {38,5,-1,13,2,12,0,-1,1,0,4},
   {77,2,-1,13,12,5,-1,0,0,1,3},
   {0,5,1,13,0,0,0,-1,-1,0,2},
  },
  {
   {65,2,1,13,-5,0,0,-1,-1,0,3},
   {25,2,-1,-13,-12,-1,1,0,0,1,2},
   {25,2,-1,-13,1,12,0,-1,-1,0,1},
   {65,2,1,13,0,5,1,0,0,1,4},
  },
 },
 {
  {
   {65,2,1,13,0,5,1,0,0,1,1},
   {12,5,-1,13,0,-12,0,1,-1,0,4},
   {25,2,-1,-13,-12,-1,1,0,0,1,3},
   {26,5,1,13,2,0,0,1,1,0,2},
  },
  {
   {13,2,1,-13,1,0,0,1,-1,0,3},
   {77,2,-1,13,12,5,-1,0,0,1,2},
   {77,2,-1,13,-5,-12,0,1,-1,0,1},
   {13,2,1,-13,0,-1,-1,0,0,1,4},
  },
  {
   {26,5,1,13,0,-2,-1,0,0,-1,1},
   {25,2,-1,-13,1,12,0,-1,-1,0,4},
   {64,5,-1,-13,12,4,-1,0,0,-1,3},
   {65,2,1,13,-5,0,0,-1,-1,0,2},
  },
  {
   {52,5,1,-13,-4,0,0,-1,1,0,3},
   {38,5,-1,13,-12,-2,1,0,0,-1,2},
   {38,5,-1,13,2,12,0,-1,1,0,1},
   {52,5,1,-13,0,4,1,0,0,-1,4},
  },
 },
};
static void draw_tiles(uint32_t phase,uint32_t base_x,uint32_t base_y)
{
 uint32_t camera=U(0xbca2e0u),angle,rot,row,col,record,kind,variant,x,y;
 int32_t sx,sy;const int8_t *d;
 if(camera>6u||(camera&1u))return;
 angle=U(0xbc9714u)*2u;if(angle>6u)return;rot=angle/2u;
 d=tile_order[phase][camera/2u][rot];
 for(row=0;row<(uint32_t)d[1];++row)for(col=0;col<13u;++col){
  record=0x5df9b0u+(uint32_t)((int32_t)d[0]+(int32_t)row*d[3]+(int32_t)col*d[2])*20u;
  kind=U(record);if(!kind)continue;variant=U(record+(uint32_t)d[10]*4u);
  x=base_x+(uint32_t)((int32_t)d[4]+(int32_t)row*d[8]+(int32_t)col*d[6]);
  y=base_y+(uint32_t)((int32_t)d[5]+(int32_t)row*d[9]+(int32_t)col*d[7]);
  FUN_004f0ba0(I(x),I(y),&sx,&sy);
  (void)FUN_0053e590(I(U(0xf55bb4u+kind*8u)+variant),sx,sy);
  (void)FUN_0053f250(I(U(0xf55bb4u+kind*8u)+variant),sx,sy);
 }
}
static void ground_position(uint32_t camera,uint32_t angle,uint32_t bx,uint32_t by,
                            uint32_t row,uint32_t col,uint32_t *x,uint32_t *y)
{
 uint32_t c=camera==0u?0u:camera==2u?1u:camera==4u?2u:3u;
 uint32_t r=angle==0u?0u:angle==2u?1u:angle==4u?2u:3u;
 switch((r+4u-c)&3u){
 case 0:*x=bx+col;*y=by+row;break;
 case 1:*x=bx-row;*y=by-col;break;
 case 2:*x=bx-col;*y=by-row;break;
 default:*x=bx+row;*y=by+col;break;
 }
}
static void draw_pair(uint32_t image,uint32_t x,uint32_t y)
{
 (void)FUN_0053e590(I(image),I(x),I(y));
 (void)FUN_0053f250(I(image),I(x),I(y));
}
static void draw_buildings(uint32_t rot,const int32_t *sx,const int32_t *sy)
{
 uint32_t x,y,image;
 if(rot==0u){
  draw_pair(U(0xf56d7cu),(uint32_t)*sx,(uint32_t)*sy-U(0x70e44cu));
  x=U(0x5df850u)*U(0x70e458u)+U(0x5df890u)+U(0x70e45cu)+(uint32_t)*sx;
  y=U(0x5df854u)*U(0x70e448u)+(uint32_t)*sy;image=U(0xf56d84u);draw_pair(image,x,y);
  x=U(0x5df870u)*U(0x70e458u)+U(0x5df8a0u)+(uint32_t)*sx;
  y=U(0x5df874u)*U(0x70e448u)-U(0x70e44cu)+(uint32_t)*sy;
  draw_pair(U(0xf56d8cu),x,y);
 }else if(rot==2u){
  x=U(0x5df880u)*U(0x70e458u)+U(0x5df8a8u)+(uint32_t)*sx;
  y=U(0x5df884u)*U(0x70e448u)-U(0x70e44cu)+(uint32_t)*sy;
  draw_pair(U(0xf56d7cu),x,y);
  x=U(0x5df860u)*U(0x70e458u)+U(0x5df898u)+U(0x70e45cu)+(uint32_t)*sx;
  y=U(0x5df864u)*U(0x70e448u)+(uint32_t)*sy;draw_pair(U(0xf56d84u),x,y);
  draw_pair(U(0xf56d8cu),(uint32_t)*sx,(uint32_t)*sy-U(0x70e44cu));
 }else if(rot==3u){
  draw_pair(U(0xf56d7cu)+3u,(uint32_t)*sx,(uint32_t)*sy-U(0x70e44cu));
  x=U(0x5df868u)*U(0x70e458u)+U(0x5df89cu)+U(0x70e45cu)+(uint32_t)*sx;
  y=U(0x5df86cu)*U(0x70e448u)+(uint32_t)*sy;draw_pair(U(0xf56d84u)+3u,x,y);
  x=U(0x5df888u)*U(0x70e458u)+U(0x5df8acu)+(uint32_t)*sx;
  y=U(0x5df88cu)*U(0x70e448u)-U(0x70e44cu)+(uint32_t)*sy;
  draw_pair(U(0xf56d8cu)+3u,x,y);
 }else{
  image=U(0xf56d7cu)+3u;
  x=U(0x5df870u+rot*8u)*U(0x70e458u)+U(0x5df8a0u+rot*4u)+(uint32_t)*sx;
  y=U(0x5df874u+rot*8u)*U(0x70e448u)-U(0x70e44cu)+(uint32_t)*sy;draw_pair(image,x,y);
  x=U(0x5df850u+rot*8u)*U(0x70e458u)+U(0x5df890u+rot*4u)+U(0x70e45cu)+(uint32_t)*sx;
  y=U(0x5df854u+rot*8u)*U(0x70e448u)+(uint32_t)*sy;draw_pair(U(0xf56d84u)+3u,x,y);
  draw_pair(U(0xf56d8cu)+3u,(uint32_t)*sx,(uint32_t)*sy-U(0x70e44cu));
 }
}
/* Complete7942-byte native4C0F30, including the32 tile-loop switch arms
   absent from Ghidra's partial function body. All original data widths and
   draw/cache boundaries are explicit; no original code pointer is called. */
void restore_temple_complex_preview(int32_t tool)
{
 int32_t sx,sy,px,py,kind;uint32_t camera,rot,draw_rot,angle,table,bx,by,dx=0,dy=0;
 uint32_t row,col,x,y,cell,blocked=0,forced,offset;
 FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&sx,&sy);
 sy=I((uint32_t)sy+U(0x70e44cu));
 offset=U(0xeaac28u)*0x49d8u;
 if(!W(0xea5ce8u+offset)){
  uint16_t key=(uint16_t)restore_placement_key_state(0x52);
  camera=U(0xbca2e0u);rot=U(0xbc9714u);
  if(key==0x8001u){
   uint32_t half=(uint32_t)(I(camera)/2);
   rot=rot==half?half+3u:half;if(I(rot)>=4)rot-=4u;U(0xbc9714u)=rot;
  }
  if(camera==0u){if(rot!=0u&&rot!=3u){rot=0;U(0xbc9714u)=rot;}}
  else if(camera==2u){if(rot!=0u&&rot!=1u){rot=1;U(0xbc9714u)=rot;}}
  else if(camera==4u){if(rot!=2u&&rot!=1u){rot=2;U(0xbc9714u)=rot;}}
  else if(rot!=2u&&rot!=3u){rot=3;U(0xbc9714u)=rot;}
  draw_rot=rot;
 }else{camera=U(0xbca2e0u);rot=U(0xbc9714u);draw_rot=(uint32_t)(I(camera)/2);}
 if(camera==2u)dx=0xfffffffeu;
 else if(camera==4u){dx=0xfffffffeu;dy=0xfffffffeu;}
 else if(camera==6u)dy=0xfffffffeu;
 angle=rot*2u;if(I(angle)>=8)angle-=8u;
 table=0x5e00ccu+(rot+(uint32_t)(I(camera)/2)*4u)*8u;
 bx=U(table)+U(0xbca304u);by=U(table+4u)+U(0xbca308u);
 for(row=0;row<7u;++row)for(col=0;col<13u;++col){
  ground_position(U(0xbca2e0u),angle,bx,by,row,col,&x,&y);
  cell=U(0xbca2d8u)+y*228u+x;
  if(U(0xaf598cu+cell*4u)&0xeeffd77fu)blocked=1;
 }
 forced=W(0xea5ce8u+U(0xeaac28u)*0x49d8u)!=0u;
 if(forced||blocked){
  for(row=0;row<7u;++row)for(col=0;col<13u;++col){
   ground_position(U(0xbca2e0u),U(0xbc9714u)*2u,bx,by,row,col,&x,&y);
   cell=U(0xbca2d8u)+y*228u+x;
   FUN_004f0ba0(I(x),I(y),&px,&py);
   (void)FUN_0053f4f0(I(U(0xf56be4u)),px,py,
                     forced||(U(0xaf598cu+cell*4u)&0xeeffd77fu)?0xfffff863u:0x1fe3u);
  }
  return;
 }
 kind=restore_temple_complex_library_kind(tool);
 (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,*(const char **)(uintptr_t)(0x5e018cu+(uint32_t)kind*4u),10);
 bx+=dx;by+=dy;W(0x70e120u)=0x1fe3;
 draw_tiles(0,bx,by);draw_buildings(draw_rot,&sx,&sy);draw_tiles(1,bx,by);
 W(0x70e120u)=0;
}
void FUN_004c0f30(int32_t tool){restore_temple_complex_preview(tool);}
