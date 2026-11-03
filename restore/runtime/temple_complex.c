#include "temple_complex.h"
#include "temple_upgrade.h"
#include "building_controllers.h"
#include "building_allocator.h"
#include "city_prepare.h"
#include "road_place.h"
#include "editor_brush_validation.h"
#include "menu_prepare_integer.h"
#include "actor_predicate.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))
static uint32_t view_half(void){return (uint32_t)(I(U(0xbca2e0u))/2);}
static uint32_t sideways(uint32_t direction)
{
 uint32_t view=U(0xbca2e0u),axis=direction==2u||direction==6u;
 return ((view==0u||view==4u)?axis:!axis)?3u:0u;
}
static uint32_t group_for(uint32_t direction,uint32_t part)
{return U(0x5df8b0u+((uint32_t)(I(direction)/2)*4u+view_half())*12u+part*4u);}

void restore_temple_complex_tile(int32_t x,int32_t y,uint32_t group,uint32_t image_offset)
{
 uint32_t cell,image,player;uint8_t flags,corner;
 if(!(uint8_t)FUN_0046f930(I(U(0xbca2e8u)+(uint32_t)x),I(U(0xbca2ecu)+(uint32_t)y)))return;
 cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 image=U(0xf55bb4u+group*8u)+image_offset;U(0xbc9768u)=cell;
 flags=B(0xb67d1cu+cell);U(0xb7482cu+cell*4u)=image;
 corner=B(0xb285ccu+cell);B(0xb67d1cu+cell)=(uint8_t)(flags|0x40u);
 player=U(0xeaac28u)*0x49d8u;
 B(0xae8e7cu+cell)=0;B(0xb285ccu+cell)=(uint8_t)(corner&0xf0u);
 W(0xb4e6fcu+cell*2u)=W(0xea5f9cu+player);U(0xaf598cu+cell*4u)|=8u;
}
void restore_temple_complex_decorate(uint32_t building)
{
 uint32_t r=building*264u,direction=B(0x799a82u+r),half=direction/2u;
 uint32_t x=(uint32_t)H(0x7999deu+r)+U(0x5e00ccu+half*8u);
 uint32_t y=(uint32_t)H(0x7999e0u+r)+U(0x5e00d0u+half*8u);
 uint32_t view=U(0x5e014cu+(half+view_half()*4u)*4u),row,column,index=0;
 for(row=0;row<7u;++row)for(column=0;column<13u;++column,++index){
  uint32_t px,py,entry=0x5df9b0u+index*20u,group;
  if(direction==0u){px=x+column;py=y+row;}
  else if(direction==2u){px=x-row;py=y-column;}
  else if(direction==4u){px=x-column;py=y-row;}
  else{px=x+row;py=y+column;}
  group=U(entry);
  if(group){
   uint32_t offset=U(entry+(view==0u?4u:view==2u?8u:view==4u?12u:16u));
   restore_temple_complex_tile(I(px),I(py),group,offset);
   FUN_004746d0(I(px),I(py),1);
  }
 }
}
void restore_temple_complex_redraw(uint32_t building,uint32_t image_offset)
{
 uint32_t current=building,direction,index=0;
 if(W(0x799a10u+building*264u))return;
 direction=B(0x799a82u+building*264u);
 while(current){
  uint32_t bias=sideways(direction),group=group_for(direction,index),image,r;
  if(group==0x23bu){
   if(FUN_004c52d0(I(U(0xea5f9cu+U(0xeaac28u)*0x49d8u)),1))image=U(0xf56dacu)+(bias?3u:2u);
   else image=U(0xf56d8cu)+bias+image_offset;
  }else if(group==0x23au){
   if(FUN_004c52d0(I(U(0xea5f9cu+U(0xeaac28u)*0x49d8u)),2))image=U(0xf56dacu)+(bias?1u:0u);
   else image=U(0xf56d84u)+bias+image_offset;
  }else image=U(0xf55bb4u+group*8u)+bias+image_offset;
  r=current*264u;
  FUN_0047bf50((uint16_t)current,H(0x7999deu+r),H(0x7999e0u+r),3,3,image,8);
  current=(uint32_t)H(0x799a12u+r);++index;
 }
 restore_temple_complex_decorate(building);
}
void restore_temple_complex_construct(uint32_t building,int32_t x,int32_t y,uint32_t owner,uint32_t kind,uint32_t initialize)
{
 uint32_t current=building,r=building*264u,direction,part,group,player,previous;
 if(W(0x799a10u+r))return;
 if(initialize){
  player=U(0xeaac28u)*0x49d8u;B(0x799a81u+r)=0;
  W(0xea5ce8u+player)=1;W(0xea5cfau+player)=2;U(0xea5f9cu+player)=building;
  direction=U(0xbc9714u)*2u-U(0xbca2e0u);if(I(direction)<0)direction+=8u;
  B(0x799a82u+r)=(uint8_t)direction;
 }else direction=B(0x799a82u+r);
 for(part=0;part<3u;++part){
  if(!part){group=group_for(direction,0);if(initialize)W(0x799a10u+r)=0;}
  else{
   previous=part==1u?building:(uint32_t)H(0x799a12u+building*264u);
   if(initialize){
    uint32_t px=U(0x5df970u+direction*8u)*part+(uint32_t)x;
    uint32_t py=U(0x5df974u+direction*8u)*part+(uint32_t)y;
    current=(uint32_t)restore_building_allocate_0046a400(owner,kind,I(px),I(py),0,0);r=current*264u;
    B(0x799a82u+r)=(uint8_t)direction;(void)FUN_00485100(I(current));
    W(0x799a10u+r)=(uint16_t)previous;W(0x799a12u+previous*264u)=(uint16_t)current;W(0x799a12u+r)=0;
   }
   group=part==1u?0x23au:group_for(direction,part);
  }
  if(initialize){
   if(direction==0u||direction==4u)W(0x7999eau+r)=(uint16_t)part;
   else if(direction==2u||direction==6u)W(0x7999eau+r)=(uint16_t)(part+3u);
  }
  {
   uint32_t bias=sideways(direction),image=U(0xf55bb4u+group*8u)+bias;
   uint32_t px=U(0x5df970u+direction*8u)*part+(uint32_t)x;
   uint32_t py=U(0x5df974u+direction*8u)*part+(uint32_t)y;
   FUN_0047bf50((uint16_t)current,I(px),I(py),3,3,image,8);
  }
 }
 restore_temple_complex_decorate(FUN_00475b50(current));
 kind=FUN_004c5120(I(current));player=U(0xeaac28u)*0x49d8u;
 U(0xea6200u+player)=kind==4u?10u:0u;
}
void restore_building_control_C_004c0860(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_temple_complex_tile(I(U(sp+4u)),I(U(sp+8u)),U(sp+12u),U(sp+16u));f->r[4]=sp+4u;}
void restore_building_control_C_004c0be0(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_temple_complex_decorate(U(sp+4u));f->r[4]=sp+4u;}
void restore_building_control_C_004c0d50(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_temple_complex_redraw(U(sp+4u),U(sp+8u));f->r[4]=sp+4u;}
void restore_building_control_C_004c3420(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_temple_complex_construct(U(sp+4u),I(U(sp+8u)),I(U(sp+12u)),U(sp+16u),U(sp+20u),U(sp+24u));f->r[4]=sp+4u;}
void restore_building_control_C_004c52d0(struct restore_building_control_frame *f)
{
 uint32_t sp=f->r[4],building=U(sp+4u),both=B(0x799a81u+building*264u)==3u;
 uint32_t value=restore_temple_upgrade_present(I(building),U(sp+8u));
 /* Existing frame callers can retain the native incidental high EAX bytes;
    the public C predicate continues to promise only AL. */
 f->r[0]=(both?(building&0xffffff00u):0u)|value;f->r[4]=sp+4u;
}
