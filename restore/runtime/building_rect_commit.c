#include "building_rect_commit.h"

void FUN_00474130(uint32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 restore_building_rect_00474130(preview,x1,y1,x2,y2);
}
#include "building_allocator.h"
#include "building_reservation.h"
#include "editor_brush_validation.h"
#include "road_place.h"
#include "region_validation.h"
#include "map_commit_population.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "map_commit_proximity.h"
#include "pending_message.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* A dragged housing rectangle is a set of independent plots. Protect both
 * entry-point tables per cell, including entry markers without a road flag. */
static int housing_cell_allowed(uint32_t cell,int32_t x,int32_t y)
{
 uint32_t flags=U(0xaf598cu+cell*4u),object;
 if(flags&0xeeffd77fu)return 0;
 object=U(0xd05720u+cell*4u);
 if(object&&!B(object+24u))return 0;
 if(H(0xb350dcu+cell*2u))return 0;
 return (uint8_t)FUN_00470900(x,y,x,y)==0;
}
void restore_building_warning_004e2d80(void)
{
 uint32_t p;if(U(0x605a3cu)==1)return;
 if(!U(0xe38ebcu)&&!B(0xe38ea8u))return;
 p=U(0xeaac28u)*18904u;if((int32_t)U(0xea1894u+p)<300)return;
 if((int32_t)U(0xea446cu+p)>10)FUN_004e2970(92);
}
void restore_building_warning_004e2dd0(void)
{
 uint32_t p;if(U(0x605a3cu)==1)return;p=U(0xeaac28u)*18904u;
 if((int32_t)U(0xea1894u+p)<300)return;
 if((int32_t)U(0xea41dcu+p)<=(int32_t)U(0xea5e1cu+p)/2)FUN_004e2970(93);
}
void restore_building_warning_004e2fb0(int32_t tool)
{
 uint32_t p,value,message;
 if(U(0x605a3cu)==1)return;p=U(0xeaac28u)*18904u;if((int32_t)U(0xea1894u+p)<20)return;
 if(!U(0xe38ebcu)&&(int8_t)B(0xe38ea8u)<2){value=U(0xea5c80u+p);if(!value||value==1||value==3||value==5)return;}
 value=U(0xea3fb0u+p);
 if(!value){
  if((int32_t)(U(0xea5eb4u+p)+U(0xea5ea4u+p))>3){FUN_004e2970(216);return;}
  if((int32_t)U(0xea3facu+p)>=5){if(!tool)FUN_004e2970(122);return;}
  if(U(0xea3f98u+p)||(int32_t)U(0xea18a8u+p)<=0){if(!tool)FUN_004e2970(123);return;}
  if((int32_t)U(0xea3f9cu+p)>=80){if(!tool)FUN_004e2970(122);return;}
  message=U(0xea5c80u+p)+124u;
 }else{
  if(value!=1)return;
  if(U(0xea3f98u+p)||(int32_t)U(0xea18a8u+p)<=0){if(!tool)FUN_004e2970(130);return;}
  message=U(0xea5c80u+p)+131u;
 }
 if(message)FUN_004e2970((int32_t)message);
}
void restore_building_rect_00474130(uint32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 int32_t left=x1,right=x2,top=y1,bottom=y2,temp,building;
 uint32_t span,cell,flags,failed=0,value;
 if(left>right){temp=left;left=right;right=temp;}if(top>bottom){temp=top;top=bottom;bottom=temp;}
 if(left<0)left=0;if(right>=(int32_t)U(0xbca2d0u))right=(int32_t)(U(0xbca2d0u)-1u);
 if(top<0)top=0;if(bottom>=(int32_t)U(0xbca2d4u))bottom=(int32_t)(U(0xbca2d4u)-1u);
 U(0xbc971cu)=0;span=(uint32_t)right-(uint32_t)left+1u;FUN_00485080();
 cell=U(0xbca2d8u)+(uint32_t)top*228u+(uint32_t)left;U(0xbc9760u)=(uint32_t)top;U(0xbc9768u)=cell;
 if(top<=bottom)do{
  U(0xbc9764u)=(uint32_t)left;
  if(left<=right)do{
   if(housing_cell_allowed(cell,(int32_t)U(0xbc9764u),(int32_t)U(0xbc9760u))){
     if(preview){
      value=U(0xbc971cu)+1u;B(0xb285ccu+cell)=(uint8_t)(B(0xb285ccu+cell)|16u);U(0xbc971cu)=value;
     }else{
      building=restore_building_allocate_0046a400(U(0xeaac28u),10,(int32_t)U(0xbc9764u),(int32_t)U(0xbc9760u),0,0);
      (void)FUN_00485100(building);
      if(building>0){
       value=U(0xbc971cu)+1u;flags=U(0xf5653cu);temp=(int32_t)U(0xbc9764u);U(0xbc971cu)=value;
       FUN_0047bf50((uint16_t)building,temp,(int32_t)U(0xbc9760u),1,1,flags,8);
       if(!FUN_00482650((int32_t)U(0xbc9764u),(int32_t)U(0xbc9760u),1,2,64))failed=1;
      }
      cell=U(0xbc9768u);
     }
   }
   value=U(0xbc9764u)+1u;++cell;U(0xbc9764u)=value;U(0xbc9768u)=cell;
  }while((int32_t)value<=right);
  value=U(0xbc9760u)+1u;cell+=228u-span;U(0xbc9760u)=value;U(0xbc9768u)=cell;
 }while((int32_t)value<=bottom);
 if(!preview){
  FUN_004e3330(10);restore_building_warning_004e2d80();restore_building_warning_004e2dd0();restore_building_warning_004e2fb0(10);
  if(failed==1)FUN_004e2970(62);
  FUN_00516f90(x1,y1,x2,y2,1);FUN_005173a0(x1,y1,x2,y2);B(0xe38dcau)=1;
 }
}
