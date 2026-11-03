#include "house_layout.h"
#include "building_controllers.h"
#include "clear_footprint.h"
#include "road_place.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
static int neighbour(uint32_t cell,uint32_t building,uint32_t own,uint32_t merging)
{
 uint32_t id=(uint32_t)H(0xb4e6fcu+cell*2u),r;
 if(id==building)return 1;
 r=id*264u;
 if(B(0x7999d8u+r)!=1u||!B(0x7999ddu+r))return 0;
 if(merging){if(W(0x7999eau+r)!=W(0x7999eau+own))return 0;return !B(0x7999dcu+r);}
 return H(0x7999eau+r)<=H(0x7999eau+own);
}
uint32_t restore_house_can_expand(uint32_t building,uint32_t area)
{
 uint32_t r=building*264u,origin=U(0x7999e4u+r),pass,candidate,i;
 for(pass=0;pass<3u;++pass)for(candidate=0;candidate<4u;++candidate){
  uint32_t base=origin+U(0x5da2e4u+candidate*4u),count=0;
  if((int32_t)area>0)for(i=0;i<area;++i){
   uint32_t cell=base+U(0x5da2f4u+i*4u),flags;
   if(!pass){if((B(0xaf598cu+cell*4u)&8u)&&neighbour(cell,building,r,0))++count;}
   else{
    flags=U(0xaf598cu+cell*4u)&0xeefed77fu;
    if(!flags)++count;
    if(flags&8u){if(neighbour(cell,building,r,0))++count;}
    else if(pass==2u&&(flags&0x20u))++count;
   }
  }
  if(count==area){
   uint32_t x=(uint32_t)H(0x7999deu+r)+U(0x5da2c4u+candidate*8u),y=(uint32_t)H(0x7999e0u+r);
   U(0x78d770u)=x;U(0x78d76cu)=y+U(0x5da2c8u+candidate*8u);return 1;
  }
 }
 B(0x799a56u+r)=1;return 0;
}
uint32_t restore_house_prepare_expansion(uint32_t building,uint32_t area)
{
 uint32_t x=U(0x78d770u),base,population=0,i,j;
 for(i=0;i<12u;++i)U(0x78d590u+i*4u)=0;
 base=x+U(0x78d76cu)*228u+U(0xbca2d8u);
 if((int32_t)area>0)for(i=0;i<area;++i){
  uint32_t cell=base+U(0x5da2f4u+i*4u),id,r;
  if(!(B(0xaf598cu+cell*4u)&8u))continue;
  id=(uint32_t)H(0xb4e6fcu+cell*2u);if(id==building)continue;
  r=id*264u;if(!B(0x7999ddu+r))continue;
  population+=(uint32_t)H(0x7999f4u+r);
  for(j=0;j<12u;++j)U(0x78d590u+j*4u)=U(0x78d590u+j*4u)+(uint32_t)H(0x799a2cu+r+j*2u);
  W(0x7999f4u+r)=0;B(0x7999d8u+r)=5;
 }
 return population;
}
void restore_house_clear_expansion(uint32_t building,uint32_t area)
{
 uint32_t base=U(0x78d76cu)*228u+U(0x78d770u)+U(0xbca2d8u),i;
 if((int32_t)area>0)for(i=0;i<area;++i){
  uint32_t cell=base+U(0x5da2f4u+i*4u),id,r,target=0;
  if(!(B(0xaf598cu+cell*4u)&8u))continue;
  id=(uint32_t)H(0xb4e6fcu+cell*2u);if(id==building)continue;
  r=id*264u;if(!B(0x7999ddu+r))continue;
  if(B(0x7999dcu+r)==1u)target=0x46be90u;
  else if(B(0x7999dbu+r)==2u)target=0x46c1c0u;
  else if(B(0x7999dbu+r)==3u)target=0x46c500u;
  if(target)restore_house_split_for_expansion(id,target);
 }
}
void restore_house_apply_layout(uint32_t native_target,uint32_t building,uint32_t population)
{
 uint32_t r=building*264u,size,level=0,merge=0,i,image,variation=0,x,y;
 switch(native_target){
 case 0x46bab0u:size=2;level=10;break;
 case 0x46bbc0u:size=3;level=14;break;
 case 0x46bcb0u:size=4;level=18;break;
 case 0x46bda0u:size=2;merge=1;break;
 default:abort();
 }
 if(!merge){W(0x7999e8u+r)=(uint16_t)(level+10u);W(0x7999eau+r)=(uint16_t)level;}
 B(0x7999dbu+r)=(uint8_t)size;B(0x7999ddu+r)=(uint8_t)size;
 W(0x7999f4u+r)=(uint16_t)(W(0x7999f4u+r)+(uint16_t)population);
 for(i=0;i<12u;++i)W(0x799a2cu+r+i*2u)=(uint16_t)(W(0x799a2cu+r+i*2u)+W(0x78d590u+i*4u));
 level=(uint32_t)H(0x7999eau+r)*4u;
 if(native_target==0x46bab0u)variation=B(0xacf85cu+U(0x7999e4u+r))&1u;
 image=U(0xf55bb4u+U(0x5d03ecu+level)*8u);
 if(merge)image+=4u+(U(0x5d043cu+level)!=0u);
 else image+=U(0x5d043cu+level)+variation;
 y=(uint32_t)H(0x7999e0u+r);x=(uint32_t)H(0x7999deu+r);
 FUN_0047c480((int32_t)building,(int32_t)x,(int32_t)y);
 /* The old-footprint clear may alter relocation globals and record size. */
 y=U(0x78d76cu);x=U(0x78d770u);
 W(0x7999deu+r)=(uint16_t)x;W(0x7999e0u+r)=(uint16_t)y;
 U(0x7999e4u+r)=U(0xbca2d8u)+y*228u+x;
 if(merge)B(0x7999dcu+r)=1;
 size=merge?2u:B(0x7999dbu+r);
 y=(uint32_t)H(0x7999e0u+r);x=(uint32_t)H(0x7999deu+r);
 FUN_0047bf50((uint16_t)building,(int32_t)x,(int32_t)y,(int32_t)size,(int32_t)size,image,8);
}
uint32_t restore_house_try_merge(uint32_t building)
{
 uint32_t r=building*264u,base,count=0,i,x,y,population;
 if(B(0x7999dcu+r))return 0;
 base=U(0x7999e4u+r);if((B(0xacf85cu+base)&7u)>=5u)return 0;
 for(i=0;i<4u;++i){uint32_t cell=base+U(0x5da2f4u+i*4u);if((B(0xaf598cu+cell*4u)&8u)&&neighbour(cell,building,r,1))++count;}
 if(count!=4u)return 0;
 x=(uint32_t)H(0x7999deu+r)+U(0x5da2c4u);y=(uint32_t)H(0x7999e0u+r)+U(0x5da2c8u);
 U(0x78d770u)=x;U(0x78d76cu)=y;
 population=restore_house_prepare_expansion(building,4);
 restore_house_apply_layout(0x46bda0u,building,population);return 1;
}
void restore_house_merge_neighbours(uint32_t building){(void)restore_house_try_merge(building);}
void restore_house_expand(uint32_t building,uint32_t layout,uint32_t population)
{restore_house_apply_layout(layout,building,population);}
void restore_building_control_C_0046b480(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=restore_house_can_expand(U(sp+4u),U(sp+8u));f->r[4]=sp+4u;}
void restore_building_control_C_0046b840(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=restore_house_try_merge(U(sp+4u));f->r[4]=sp+4u;}
void restore_building_control_C_0046b940(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_house_clear_expansion(U(sp+4u),U(sp+8u));f->r[4]=sp+4u;}
void restore_building_control_C_0046b9e0(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=restore_house_prepare_expansion(U(sp+4u),U(sp+8u));f->r[4]=sp+4u;}
#define APPLY(token) \
void restore_building_control_C_##token(struct restore_building_control_frame *f) \
{uint32_t sp=f->r[4];restore_house_apply_layout(0x##token##u,U(sp+4u),U(sp+8u));f->r[4]=sp+4u;}
APPLY(0046bab0)
APPLY(0046bbc0)
APPLY(0046bcb0)
APPLY(0046bda0)
