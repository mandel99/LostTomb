#include "building_support.h"
#include "building_allocator.h"
#include "clear_footprint.h"
#include "road_place.h"
#include "map_commit_selection.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define S(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
extern void FUN_00486060(int32_t);

uint32_t FUN_00405ba0(void)
{
 uint32_t changed=B(0xe38ddau)!=1u,state;
 if(U(0x6059e8u))changed=1;
 state=U(0xbca310u);B(0xe38ddau)=1;
 U(0x6059e8u)=0;U(0x6059e4u)=0;U(0x6059dcu)=0;
 U(0x6059e0u)=0;U(0x6059d4u)=0;U(0x6059d8u)=0;
 if(state==1u){uint32_t building=U(0xbc9800u);FUN_00486060((int32_t)building);}
 if(U(0xbca310u)==2u)U(0xbca310u)=0;
 B(0xe38dcau)=1;return changed;
}

int32_t FUN_0046efd0(uint32_t owner)
{
 uint32_t id=1,p=0xeab048u,r;
 do{
  if(!U(p)){
   r=id*224u;FUN_0040f8f0((uint32_t *)(uintptr_t)(0xeaaf68u+r),224,0);
   U(0xeaaf68u+r)=1;U(0xeab044u+r)=owner;return (int32_t)id;
  }
  p+=224u;++id;
 }while(p<0xeadb28u);
 return 0;
}

void FUN_00482580(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask)
{
 uint32_t left=(uint32_t)x-(uint32_t)radius,top=(uint32_t)y-(uint32_t)radius;
 uint32_t right=(uint32_t)x+(uint32_t)size+(uint32_t)radius-1u;
 uint32_t bottom=(uint32_t)y+(uint32_t)size+(uint32_t)radius-1u,width,height,cell,stride,rows,columns,p;
 if((int32_t)left<0)left=0;if((int32_t)top<0)top=0;
 width=U(0xbca2d0u);if((int32_t)right>=(int32_t)width)right=width-1u;
 height=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
 cell=U(0xbca2d8u);U(0xd3b4e0u)=top;stride=left-right+227u;
 cell+=top*228u+left;U(0xbc9768u)=cell;
 if((int32_t)top>(int32_t)bottom)return;
 rows=bottom-top+1u;U(0xd3b4e0u)=rows+top;
 do{
  U(0xd3b4e4u)=left;
  if((int32_t)left<=(int32_t)right){
   columns=right-left+1u;p=0xaf598cu+cell*4u;cell+=columns;U(0xd3b4e4u)=columns+left;
   do{U(p)&=mask;p+=4u;--columns;}while(columns);
  }
  cell+=stride;--rows;
 }while(rows);
 U(0xbc9768u)=cell;
}

void FUN_004f4a60(uint32_t building,int32_t slot)
{
 uint32_t player=U(0xeaac28u);U(0xea5facu+player*0x49d8u+(uint32_t)slot*4u)=building;
}

int32_t FUN_004f4f80(int32_t building)
{
 uint32_t r=(uint32_t)building*264u,index=0,p=0xeaa978u,q=0x784de0u;int32_t kind;
 if(W(0x799a10u+r))return -1;
 kind=FUN_004f4b70(building);if(kind<0)return -1;
 do{
  if((int32_t)S(q)==kind&&!U(p)){
   U(0xeaa978u+index*4u)=(uint32_t)kind;
   if((int32_t)index>=3)return -1;
   FUN_004f4a60((uint32_t)building,(int32_t)index);return (int32_t)index;
  }
  q+=2u;++index;p+=4u;
 }while(q<0x784de6u);
 return -1;
}

uint32_t restore_ring_004f6520(void *self)
{
 uint32_t object=(uint32_t)(uintptr_t)self,i=0,p;
 U(object+4u)=0;U(object+8u)=0;U(object+12u)=50;p=object+20u;
 do{U(p)=0;++i;p+=4u;}while((int32_t)i<(int32_t)U(object+12u));
 return 1;
}

void FUN_0046cea0(int32_t building,int16_t kind)
{
 uint32_t r=(uint32_t)building*264u,index,group,variants,image,special,width;int32_t x,y;
 W(0x7999e8u+r)=(uint16_t)kind;W(0x7999eau+r)=(uint16_t)((uint16_t)kind-10u);
 special=B(0x7999dcu+r);index=(uint32_t)(int32_t)(int16_t)((uint16_t)kind-10u)*4u;
 group=U(0x5d03ecu+index);variants=U(0x5d034cu+index);image=U(0xf55bb4u+group*8u);
 if(special){variants=U(0x5d043cu+index);image+=4u;if(variants)++image;}
 else{
  image+=U(0x5d043cu+index);
  if(variants){uint32_t cell=U(0x7999e4u+r),variation=B(0xacf85cu+cell);image+=variation&(variants-1u);}
 }
 width=B(0x7999dbu+r);y=S(0x7999e0u+r);x=S(0x7999deu+r);
 FUN_0047bf50((uint16_t)building,x,y,(int32_t)width,(int32_t)width,image,8);
}

static void draw_new(uint32_t id,uint32_t image)
{
 uint32_t r=id*264u;int32_t y,x;
 W(0x7999f4u+r)=0;W(0x7999f8u+r)=0;y=S(0x7999e0u+r);x=S(0x7999deu+r);
 FUN_0047bf50((uint16_t)id,x,y,1,1,image,8);
}
void FUN_0046cf40(int32_t building)
{
 uint32_t r=(uint32_t)building*264u,image=U(0xf5653cu),id,owner;int32_t x,y,kind,x2,y2;
 W(0x7999e8u+r)=10;W(0x7999eau+r)=0;
 if(!B(0x7999dcu+r)){uint32_t cell=U(0x7999e4u+r);U(0xb7482cu+cell*4u)=image;return;}
 y=S(0x7999e0u+r);x=S(0x7999deu+r);FUN_0047c480(building,x,y);
 B(0x7999dcu+r)=0;B(0x7999dbu+r)=1;B(0x7999ddu+r)=1;
 y=S(0x7999e0u+r);x=S(0x7999deu+r);FUN_0047bf50((uint16_t)building,x,y,1,1,image,8);
 x=S(0x7999deu+r);y=S(0x7999e0u+r);kind=S(0x7999e8u+r);x2=(int32_t)((uint32_t)x+1u);owner=U(0xeaac28u);
 id=(uint32_t)restore_building_allocate_0046a400(owner,(uint32_t)kind,x2,y,0,0);draw_new(id,image);
 owner=U(0xeaac28u);y2=(int32_t)((uint32_t)y+1u);kind=S(0x7999e8u+r);
 id=(uint32_t)restore_building_allocate_0046a400(owner,(uint32_t)kind,x,y2,0,0);draw_new(id,image);
 kind=S(0x7999e8u+r);owner=U(0xeaac28u);
 id=(uint32_t)restore_building_allocate_0046a400(owner,(uint32_t)kind,x2,y2,0,0);draw_new(id,image);
}
