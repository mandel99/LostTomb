#include "road_check_model.h"
#include "road_scan_model.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))
uint32_t restore_construction_site_valid(int32_t kind)
{
 uint32_t oy=U(0xbca308u),ox=U(0xbca304u),row,col,maprow,size;
 int terrain_clear=1,unoccupied=1,access=0;
 size=kind==31?2u:kind==30?3u:kind==33?4u:5u;
 if(I(oy)>=I(oy+size))return 0;
 maprow=ox+oy*228u;
 /* Cache selection coordinates, reload the map origin after both helpers on
    every cell. Neither a found road nor a blocker terminates this scan. */
 for(row=0;row<size;++row){
  for(col=0;I(ox)<I(ox+size)&&col<size;++col){
   uint32_t tile,object,terrain;
   if((uint8_t)FUN_0043f3a0(I(col),I(row),I(size))&&
      (uint8_t)FUN_0043f250(ox+col,I(oy+row),I(ox),I(oy),I(size)))access=1;
   tile=U(0xbca2d8u)+maprow+col;
   object=U(0xd05720u+tile*4u);terrain=U(0xaf598cu+tile*4u);
   if(object&&!B(object+24u))terrain_clear=0;
   if((terrain&0xeeffd77fu)&&!(terrain&0x40u))terrain_clear=0;
   if(H(0xb4e6fcu+tile*2u)>0)unoccupied=0;
  }
  maprow+=228u;
 }
 return access&&terrain_clear&&unoccupied?1u:0u;
}

uint32_t restore_construction_select_slot(int32_t x,int32_t y,int32_t *out_x,int32_t *out_y,
                                        int32_t primary,int32_t secondary,uint32_t inherited_x,uint32_t inherited_y)
{
 uint32_t extent=primary==31?2u:primary==30?3u:primary==33?4u:primary==209?5u:0u;
 uint32_t rows=1,columns=1,rotated=0,count,index,chosen=0,cell,dx,dy;
 int32_t score=5,locations[50];
 /* A5x5 grid can yield25 coordinate pairs. Native space holds only16;
    reserve the full output capacity rather than overwrite the return address
    when a caller supplies an unusually empty5x5 grid. */
 locations[0]=I(inherited_x);locations[1]=I(inherited_y);
 if(secondary==30){columns=2;locations[0]=1;locations[1]=1;}
 else if(secondary==31){locations[0]=0;locations[1]=0;}
 else if(secondary==33){rows=2;columns=2;locations[0]=2;locations[1]=2;}
 (void)FUN_00441040(I(extent),I(rows),I(columns),&count,locations);
 if(I(count)<=0){
  if(secondary==30){
   rotated=1;
   (void)FUN_00441040(I(extent),I(columns),I(rows),&count,locations);
  }
  if(I(count)<=0){
   U((uint32_t)(uintptr_t)out_x)=(uint32_t)x+(uint32_t)locations[0];
   U((uint32_t)(uintptr_t)out_y)=(uint32_t)y+(uint32_t)locations[1];
   return UINT32_MAX;
  }
 }
 for(index=0;index<count;++index){
  int32_t rank;
  if(extent==2u)break;
  dx=(uint32_t)locations[index*2u];dy=(uint32_t)locations[index*2u+1u];
  rank=extent==3u?I(U(0x5d2960u+(dx*3u+dy)*4u)):
       extent==4u?I(U(0x5d2984u+(dx*4u+dy)*4u)):0;
  if(rank<score){score=rank;chosen=index;}
 }
 dx=(uint32_t)locations[chosen*2u];dy=(uint32_t)locations[chosen*2u+1u];
 U((uint32_t)(uintptr_t)out_x)=(uint32_t)x+dx;
 U((uint32_t)(uintptr_t)out_y)=(uint32_t)y+dy;
 cell=0x775614u+(dx*5u+dy)*4u;
 U(cell)=1;
 if(secondary==30){U(cell+(rotated?20u:4u))=1;}
 else if(secondary==33){U(cell+20u)=1;U(cell+4u)=1;U(cell+24u)=1;}
 return rotated;
}
uint32_t FUN_0043f410(int32_t kind)
{return restore_construction_site_valid(kind);}

/* Unknown secondary kinds leave the two failure coordinates inherited from
   native entrySP-128/-124. Capture those inputs before a C prologue; all
   placement, ranking and reservation logic above is ordinary C. */
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) uint32_t FUN_00441130(int32_t x,int32_t y,int32_t *out_x,int32_t *out_y,int32_t primary_kind,int32_t secondary_kind)
{
 __asm {
  mov ecx,esp
  push dword ptr[ecx-124]
  push dword ptr[ecx-128]
  push dword ptr[ecx+24]
  push dword ptr[ecx+20]
  push dword ptr[ecx+16]
  push dword ptr[ecx+12]
  push dword ptr[ecx+8]
  push dword ptr[ecx+4]
  call restore_construction_select_slot
  add esp,32
  ret
 }
}
#pragma warning(pop)
