#include "city_supply.h"
#include "industry_logistics.h"
#include "storage_delivery.h"
#include "menu_prepare_integer.h"
#include "menu_resource_neighbors.h"
#include "menu_list_scroll.h"
#include "actor_transport.h"
#include "city_updates.h"
#include "map_commit_population.h"
#include "event_publisher.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

uint32_t restore_storage_delivery_destination(int32_t source,uint32_t actor,int32_t resource,uint32_t network)
{
 uint32_t id,record,root,slot,count=0,origin,x,y,tile,selected;int32_t amount,index,kind;uint8_t policy;
 U(0x96e6b0u)=0;if(resource==21||I(U(0x78d790u))<1)return 0;
 for(id=1;;++id){
  record=id*264u;
  if(B(0x7999d8u+record)==1u&&W(0x7999e8u+record)==73u&&B(0x799a1cu+record)&&H(0x7999f8u+record)>0&&B(0x7999ecu+record)==network){
   root=FUN_00475b50(id);slot=B(0x799a99u+root*264u)*196u;
   if((uint32_t)source==root&&B(0x7900c2u+slot+(uint32_t)resource)!=2u)goto rejected;
   policy=B(0x7900c2u+slot+(uint32_t)resource);if(policy!=0u&&policy!=2u)goto rejected;
   amount=FUN_0045d510(I(root),resource);
   if(amount>=H((policy==0u?0x7900ecu:0x790134u)+slot+(uint32_t)resource*2u))goto rejected;
   kind=H(0x7999e8u+root*264u);
   amount=FUN_004cde60(H(0x799a18u+root*264u),I(U(0x705eb8u+(uint32_t)kind*40u)));
   if(amount<100){U(0xd05628u)=U(0xd05628u)+1u;goto rejected;}
   if(W(0x7999eau+record)&&(H(0x7999eau+record)!=resource||H(0x799a14u+record)>=400))goto rejected;
   root=FUN_00475b50(id);
   if(B(0x799a1cu+root*264u)==1u){
    y=(uint32_t)H(0x799a00u+root*264u);x=(uint32_t)H(0x7999feu+root*264u);count=U(0x96e6b0u);origin=U(0xbca2d8u);
    U(0x972534u+count*4u)=id;++count;U(0x96e6b0u)=count;U(0x96e6b0u+count*4u)=origin+x+y*228u;
   }else{
    if(!FUN_00480180(H(0x7999deu+root*264u),H(0x7999e0u+root*264u),3))goto rejected;
    y=U(0xd3b4e0u);count=U(0x96e6b0u);U(0x972534u+count*4u)=id;
    tile=U(0xd3b4e4u)+y*228u+U(0xbca2d8u);U(0x96e6b4u+count*4u)=tile;++count;U(0x96e6b0u)=count;
   }
   goto next;
  }
 rejected:count=U(0x96e6b0u);
 next:if(I(id+1u)>I(U(0x78d790u)))break;
 }
 if(!count)return 0;
 index=FUN_004b28f0(I(actor),0x96e6b4,count);if(!index)return 0;
 selected=U(0x972530u+(uint32_t)index*4u);root=FUN_00475b50(selected);record=root*264u;
 x=(uint32_t)H(0x7999feu+record);y=(uint32_t)H(0x799a00u+record);U(0xd3b4e4u)=x;policy=B(0x799a1cu+record);U(0xd3b4e0u)=y;
 if(policy==1u||FUN_00480180(H(0x7999deu+record),H(0x7999e0u+record),3))return selected;
 return 0;
}
void restore_city_update_food(void)
{
 uint32_t count,id,record,editor;
 if(B(0xea624au))FUN_0045d630(1);
 count=U(0x78d790u);if(I(count)<1)return;
 editor=U(0x784bbcu);
 for(id=1;id<=count;++id){
  record=id*264u;
  if(B(0x7999d8u+record)==1u&&(W(0x7999e8u+record)==70u||W(0x7999e8u+record)==190u)&&editor)W(0x799a2eu+record)=200;
 }
}
void restore_city_rebuild_food(int32_t player)
{
 uint32_t off=(uint32_t)player*0x49d8u,local[9]={0},index,id,record,total30=0,total34=0,total,rate,need,bound;
 int32_t kind,percentage,quantity;
 for(index=0;index<9u;++index)U(0xea60acu+off+index*4u)=0;
 bound=U(0x78d790u);U(0xea41dcu+off)=0;U(0xea41e0u+off)=0;U(0xea4388u+off)=0;U(0xea438cu+off)=0;
 U(0xea5e20u+off)=0;U(0xea5e24u+off)=0;U(0xea5e28u+off)=0;
 if(I(bound)>=1)for(id=1;;++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u)goto next_food;
  kind=H(0x7999e8u+record);
  if(kind==71||kind==191){
   B(0x799a1cu+record)=0;
   if(!FUN_004806a0(H(0x7999deu+record),H(0x7999e0u+record)))goto next_food;
   B(0x799a1cu+record)=1;kind=H(0x7999e8u+record);
   percentage=FUN_004cde60(H(0x799a18u+record),I(U(0x705eb8u+(uint32_t)kind*40u)));
   if(percentage<100)U(0xea5e20u+off)=U(0xea5e20u+off)+1u;
   if(percentage<50){
    total=0;for(index=1;index<=8u;++index)total+=(uint32_t)H(0x799a2eu+record+index*2u);
    U(0xea5e24u+off)=U(0xea5e24u+off)+1u;if(I(total)>0)U(0xea5e28u+off)=U(0xea5e28u+off)+1u;
   }else{
    total30+=(uint32_t)H(0x799a3au+record);total34+=(uint32_t)H(0x799a3eu+record);
    for(index=0;index<9u;++index)U(0xea60acu+off+index*4u)=U(0xea60acu+off+index*4u)+(uint32_t)H(0x799a2eu+record+index*2u);
    total=0;for(index=1;index<=8u;++index)total+=(uint32_t)H(0x799a2eu+record+index*2u);(void)total;
    U(0xea438cu+off)=U(0xea438cu+off)+1u;
    if(!U(0xe38ebcu)){
     if((int32_t)(int8_t)B(0xe38ea8u)==I(U(0x5794e8u))&&!B(0x7027bau)&&B(0x7027b9u)&&I(total34)>=400){
      FUN_00417ae0(2);if(U(0xe38ebcu))goto next_food;
     }
     if((int32_t)(int8_t)B(0xe38ea8u)==I(U(0x579574u))&&!B(0x7027bfu)&&I(total30)>=800)FUN_00417ae0(7);
    }
   }
  }else if(B(0x7999ddu+record)&&W(0x7999f4u+record)){
   for(index=0;index<8u;++index)local[index+1u]+=(uint32_t)H(0x799a2cu+record+index*2u);
  }
 next_food:if(I(id+1u)>I(U(0x78d790u)))break;
 }
 for(index=1;index<=8u;++index){
  total=U(0xea60acu+off+index*4u);
  if(total||local[index]){U(0xea41dcu+off)=U(0xea41dcu+off)+total;U(0xea41e0u+off)=U(0xea41e0u+off)+1u;}
 }
 rate=FUN_004c5340(player);need=(uint32_t)FUN_004cde40(I(U(0xea1894u+off)),I(rate));U(0xea5e1cu+off)=need;
 if(I(need)<=0)U(0xea4388u+off)=I(U(0xea41dcu+off))>0;
 else{quantity=I(U(0xea41dcu+off))/I(need);U(0xea4388u+off)=(uint32_t)quantity;}
 if(U(0x784bbcu)){U(0xea41e0u+off)=1;U(0xea4388u+off)=12;}
}
void restore_city_update_industry(void)
{if(B(0xea624au))FUN_0045e6e0(1);}
void restore_city_rebuild_industry(int32_t player)
{
 uint32_t off=(uint32_t)player*0x49d8u,index,id,record,at,value;int32_t kind,type,stock,space,secondary;
 for(index=0;index<27u;++index){U(0xea54d4u+off+index*4u)=0;U(0xea5574u+off+index*4u)=0;}
 for(id=1;I(id)<=I(U(0x78d790u));++id){
  record=id*264u;if(B(0x7999d8u+record)!=1u||!restore_industry_accepts_goods(H(0x7999e8u+record)))continue;
  B(0x799a1cu+record)=0;
  if(!FUN_00480180(H(0x7999deu+record),H(0x7999e0u+record),B(0x7999dbu+record)))continue;
  B(0x799a1cu+record)=1;kind=H(0x7999e8u+record);stock=H(0x799a14u+record);space=restore_industry_capacity(kind)-stock;if(space<0)space=0;
  type=H(0x7999eau+record);at=off+(uint32_t)type*4u;U(0xea5574u+at)=U(0xea5574u+at)+(uint32_t)space;
  value=U(0xea54d4u+at)+(uint32_t)stock;secondary=H(0x799aa6u+record);U(0xea54d4u+at)=value;
  if(secondary){
   stock=H(0x799aa4u+record);space=restore_industry_capacity(kind)-stock;if(space<0)space=0;at=off+(uint32_t)secondary*4u;
   U(0xea5574u+at)=U(0xea5574u+at)+(uint32_t)space;U(0xea54d4u+at)=U(0xea54d4u+at)+(uint32_t)stock;
  }
 }
}
