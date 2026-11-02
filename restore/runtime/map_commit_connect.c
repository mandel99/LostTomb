#include "map_commit_connect.h"
#include "map_commit_links.h"
#include "map_commit_paths.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
void FUN_0043f060(int32_t building,uint8_t enabled,int32_t marker)
{
 uint32_t record=(uint32_t)building*264u,linked=U(0x799a9cu+record),layout,old_x,old_y,start_x,start_y,end_x,end_y,row,cell,flags,route[1000],index;
 int32_t x,y,link_x,link_y,count;
 if((int32_t)linked<=0)return;
 y=H(0x7999e0u+record);x=H(0x7999deu+record);(void)FUN_0047cbd0(x,y);
 for(index=0;index<250;++index)route[index]=0xffffffffu;
 link_x=H(0x7999deu+linked*264u);link_y=H(0x7999e0u+linked*264u);layout=B(0x7999dbu+record);
 old_x=U(0xd3b4e4u);old_y=U(0xd3b4e0u);
 (void)FUN_004806c0(x,y,layout);start_x=U(0xd3b4e4u);start_y=U(0xd3b4e0u);
 (void)FUN_004806c0(link_x,link_y,layout);end_y=U(0xd3b4e0u);end_x=U(0xd3b4e4u);
 (void)FUN_0051de70((int32_t)start_x,(int32_t)start_y,(int32_t)end_x,(int32_t)end_y);
 count=FUN_0051e720(1001,(int32_t)start_x,(int32_t)start_y,(int32_t)end_x,(int32_t)end_y,0,route);
 U(0xd3b4e4u)=old_x;U(0xd3b4e0u)=old_y;if(!count)return;
 row=U(0xbca2d8u)+start_y*228u;cell=row+start_x;
 flags=U(0xaf598cu+cell*4u);
 if(enabled){U(0xaf598cu+cell*4u)=flags|0x40u;B(0xee0c50u+cell)=marker==1?1:3;}
 else{B(0xee0c50u+cell)=0xff;U(0xaf598cu+cell*4u)=flags&~0x40u;}
 for(index=0;(int32_t)index<count;++index){
  if(route[index]==0)row-=228u;else if(route[index]==2)++start_x;else if(route[index]==4)row+=228u;else --start_x;
  cell=row+start_x;
  flags=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=enabled?flags|0x40u:flags&~0x40u;
  B(0xee0c50u+cell)=enabled?(marker==1?1:3):0xff;
 }
}
void FUN_00516f90(int32_t x1,int32_t y1,int32_t x2,int32_t y2,uint8_t update_links)
{
 uint32_t cell,tail,raw,flags,record,object,image,base,access,value,id;uint16_t type;uint8_t cost;int32_t x,y,width,height,orientation;
 if(x1<0)x1=0;if(y1<0)y1=0;
 width=(int32_t)U(0xbca2d0u);if(x2>=width)x2=(int32_t)((uint32_t)width-1u);
 height=(int32_t)U(0xbca2d4u);if(y2>=height)y2=(int32_t)((uint32_t)height-1u);
 tail=(uint32_t)x1-(uint32_t)x2+227u;cell=U(0xbca2d8u)+(uint32_t)y1*228u+(uint32_t)x1;
 for(y=y1;y<=y2;++y){
  for(x=x1;x<=x2;++x,++cell){
   if(update_links){
    raw=U(0xaf598cu+cell*4u);
    if((raw&0x10044u)==0x44u&&!B(0xadc36cu+cell))U(0xaf598cu+cell*4u)=raw&~0x40u;
   }
   raw=U(0xaf598cu+cell*4u);
   if((raw&0x80000u)||(raw&0x10004u)==0x10004u)cost=0xff;
   else{
    flags=raw&0xeeffd77fu;
    if((flags&0x40u)||(flags&0x40000000u))cost=0;
    else if(raw&0x400u)cost=2;
    else if(raw&0x10000000u)cost=5;
    else if(flags&0x8008u){
     record=(uint32_t)(int32_t)H(0xb4e6fcu+cell*2u)*264u;
     if(!record){
      base=U(0xf56b8cu);U(0xaf598cu+cell*4u)=raw&~8u;
      value=B(0xacf85cu+cell)&7u;B(0xec7630u+cell)=4;U(0xb7482cu+cell*4u)=value+base;
      access=B(0xb285ccu+cell)&0xf0u;B(0xb67d1cu+cell)=0x40;B(0xb285ccu+cell)=(uint8_t)access;
      value=U(0xf535bcu);U(0xf535bcu)=value+1u;continue;
     }
     type=(uint16_t)H(0x7999e8u+record);
     if(type==0x8a||type==0xd7||type==0x88)cost=0;
     else if(type==0x36)cost=6;
     else if(type==0xd1)cost=4;
     else if(type==0x5a){
      value=B(0x799a82u+record);orientation=(int32_t)U(0xbca2e0u);
      cost=((value-(uint32_t)(orientation/2))&1u)?0xfc:0xfa;
     }else cost=0xff;
    }
    else if(flags&0x100u)cost=4;
    else{
     object=U(0xd05720u+cell*4u);
     if(object&&!B(object+0x18u)){
      image=U(0xb7482cu+cell*4u);base=U(0xf56c34u);image-=base;
      if((int32_t)image<48)cost=4;
      else if(image==52||image==48||image==54||image==50)cost=0xfb;
      else cost=0xff;
     }else if(flags&0x10000u)cost=4;
     else if(flags&0x12040011u)cost=4;
     else if(flags&0x1000u)cost=2;
     else if(!flags)cost=4;
     else cost=0xff;
    }
   }
   B(0xee0c50u+cell)=cost;
  }
  cell+=tail;
 }
 if(update_links){
  for(id=0;id<4000;++id){
   record=0x7999d8u+id*264u;
   if(H(record+16u)==0x88){
    value=B(record);if(value==1||value==3){
     if(H(record+64u)&&H(0x799a18u+U(record+196u)*264u)>0)FUN_0043f060((int32_t)id,1,1);
     else{FUN_0043f060((int32_t)id,1,3);FUN_004a36b0((int32_t)id);}
    }
   }
  }
 }
}
void FUN_00517370(void)
{
 int32_t last_x,last_y;FUN_0040f8f0((uint32_t *)0xee0c50u,0xcb10u,0xff);
 last_y=(int32_t)(U(0xbca2d4u)-1u);last_x=(int32_t)(U(0xbca2d0u)-1u);
 FUN_00516f90(0,0,last_x,last_y,1);
}
