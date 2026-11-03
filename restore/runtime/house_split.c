#include "house_split.h"
#include "house_layout.h"
#include "building_controllers.h"
#include "building_allocator.h"
#include "clear_footprint.h"
#include "road_place.h"
#include "surface_passes.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
static void small_house(uint32_t building,uint32_t population,const int32_t stock[12],uint32_t image)
{
 uint32_t r=building*264u,i;
 W(0x7999f4u+r)=(uint16_t)population;
 for(i=0;i<12u;++i)W(0x799a2cu+r+i*2u)=(uint16_t)stock[i];
 W(0x7999f8u+r)=0;
 image+=B(0xacf85cu+U(0x7999e4u+r))&1u;
 FUN_0047bf50((uint16_t)building,H(0x7999deu+r),H(0x7999e0u+r),1,1,image,8);
}
void restore_house_subdivide(uint32_t native_target,uint32_t building)
{
 uint32_t r=building*264u,divisor,width,grid=0,level=0,preserve=0,i,row,column,image,base,cell;
 int32_t quotient[12],remainder[12],population=H(0x7999f4u+r),part,x,y;
 switch(native_target){
 case 0x46be90u:divisor=4;width=1;grid=2;preserve=1;break;
 case 0x46c1c0u:divisor=4;width=1;grid=2;level=9;break;
 case 0x46c500u:divisor=9;width=1;grid=3;level=9;break;
 case 0x46cb30u:divisor=6;width=2;level=13;break;
 case 0x46ccf0u:divisor=8;width=3;level=17;break;
 default:abort();
 }
 for(i=0;i<12u;++i){int32_t value=H(0x799a2cu+r+i*2u);quotient[i]=value/(int32_t)divisor;remainder[i]=value%(int32_t)divisor;}
 part=population/(int32_t)divisor;
 FUN_0047c480((int32_t)building,H(0x7999deu+r),H(0x7999e0u+r));
 if(!preserve){W(0x7999e8u+r)=(uint16_t)(level+10u);W(0x7999eau+r)=(uint16_t)level;}
 B(0x7999dbu+r)=(uint8_t)width;B(0x7999ddu+r)=(uint8_t)width;B(0x7999dcu+r)=0;
 W(0x7999f4u+r)=(uint16_t)(part+population%(int32_t)divisor);
 for(i=0;i<12u;++i)W(0x799a2cu+r+i*2u)=(uint16_t)(quotient[i]+remainder[i]);
 W(0x7999f8u+r)=0;
 /* Image selection follows the clear and record changes, then stays cached
    for every child. Each one-cell stamp reads its own terrain variation. */
 level=(uint32_t)H(0x7999eau+r)*4u;
 image=U(0xf55bb4u+U(0x5d03ecu+level)*8u)+U(0x5d043cu+level);
 base=image;if(width<3u)base+=B(0xacf85cu+U(0x7999e4u+r))&1u;
 FUN_0047bf50((uint16_t)building,H(0x7999deu+r),H(0x7999e0u+r),(int32_t)width,(int32_t)width,base,8);
 x=H(0x7999deu+r);y=H(0x7999e0u+r);
 if(grid){
  for(row=0;row<grid;++row)for(column=0;column<grid;++column){
   uint32_t owner,kind,id;
   if(!row&&!column)continue;
   /* Reload these between allocations; keep the earlier root coordinates. */
   owner=U(0xeaac28u);kind=grid==3u?19u:(uint32_t)H(0x7999e8u+r);
   id=(uint32_t)restore_building_allocate_0046a400(owner,kind,x+(int32_t)column,y+(int32_t)row,0,0);
   small_house(id,(uint32_t)part,quotient,image);
  }
  return;
 }
 base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 for(i=0;i<=width;++i){
  cell=base+width+i*228u;U(0xaf598cu+cell*4u)|=0x20u;
  cell=base+width*228u+i;U(0xaf598cu+cell*4u)|=0x20u;
 }
 FUN_00476530();
 FUN_00516f90(x,y,x+(int32_t)width,y+(int32_t)width,1);
 FUN_005173a0(x,y,x+(int32_t)width,y+(int32_t)width);
}
void restore_house_reduce(uint32_t building,uint32_t target)
{
 if(target!=0x46c1c0u&&target!=0x46cb30u&&target!=0x46ccf0u)abort();
 restore_house_subdivide(target,building);
}
void restore_house_split_for_expansion(uint32_t building,uint32_t target)
{
 if(target!=0x46be90u&&target!=0x46c1c0u&&target!=0x46c500u)abort();
 restore_house_subdivide(target,building);
}
#define SPLIT(token) \
void restore_building_control_C_##token(struct restore_building_control_frame *f) \
{uint32_t sp=f->r[4];restore_house_subdivide(0x##token##u,U(sp+4u));f->r[4]=sp+4u;}
SPLIT(0046be90)
SPLIT(0046c1c0)
SPLIT(0046c500)
SPLIT(0046cb30)
SPLIT(0046ccf0)
