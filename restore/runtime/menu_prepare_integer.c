#include "menu_prepare_integer.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define C(a) ((int32_t)(int8_t)B(a))
#define I(a) ((int32_t)(uint32_t)(a))

uint32_t restore_building_root(uint32_t building)
{
 uint32_t steps;int32_t previous;
 for(steps=0;steps<525u;++steps){
  previous=H(0x799a10u+building*264u);if(previous<=0)return building;
  building=(uint32_t)previous;
 }
 return 0;
}
uint32_t restore_eight_link_status(uint32_t building)
{
 uint32_t step,unfinished=0,total=0;int32_t next;
 for(step=0;step<8u;++step){
  next=H(0x799a12u+building*264u);if(next<=0)return 0;building=(uint32_t)next;
  if(!W(0x7999eau+building*264u))++unfinished;else total+=(uint32_t)H(0x799a14u+building*264u);
 }
 return unfinished?0u:I(total)<3200?2u:1u;
}
uint32_t restore_has_active_formation(void)
{
 uint32_t at;
 for(at=0xbcba9au;at<0xbcd62au;at+=144u)
  if(B(at-66u)==1u&&B(at)&&H(at+4u)>0)return 1;
 return 0;
}
void restore_house_find_negative_influence(void)
{
 uint32_t selected=U(0xeadd6cu),record=selected*264u,cell,after,step,at,id,kind,dx,dy,distance;
 int32_t sx=H(0x7999deu+record),sy=H(0x7999e0u+record),left=sx-6,right=sx+6,top=sy-6,bottom=sy+6,x,y,value,change,range;
 U(0xeadcd8u)=0;U(0xeadcd4u)=0;
 if(left<0)left=0;if(top<0)top=0;
 if(right>=I(U(0xbca2d0u)))right=I(U(0xbca2d0u)-1u);
 if(bottom>=I(U(0xbca2d4u)))bottom=I(U(0xbca2d4u)-1u);
 cell=U(0xbca2d8u)+(uint32_t)top*228u+(uint32_t)left;step=(uint32_t)left-(uint32_t)right+227u;
 for(y=top;y<=bottom;++y){
  if(left<=right){
   after=cell+(uint32_t)right-(uint32_t)left+1u;
   for(x=left;x<=right;++x,++cell){
    id=(uint32_t)H(0xb4e6fcu+cell*2u);
    if(I(id)<=0||B(0x7999d8u+id*264u)!=1u||id==selected)continue;
    if(B(0x7999ddu+id*264u)&&H(0x7999e8u+id*264u)>=H(0x7999e8u+selected*264u))continue;
    kind=(uint32_t)H(0x7999e8u+id*264u);at=kind*40u;value=I(U(0x705ea8u+at));if(value>=0)continue;
    change=I(U(0x705eb0u+at));range=I(U(0x705eb4u+at));record=selected*264u;
    sy=H(0x7999e0u+record);sx=H(0x7999deu+record);
    dx=x>sx?(uint32_t)x-(uint32_t)sx:x<sx?(uint32_t)sx-(uint32_t)x:0;
    U(0xd3b4fcu)=dx;
    dy=y>sy?(uint32_t)y-(uint32_t)sy:y<sy?(uint32_t)sy-(uint32_t)y:0;
    U(0xd3b4f8u)=dy;distance=I(dx)<I(dy)?dy:dx;
    if(I(distance)<=range){
     if(I(distance)>1)value=I((uint32_t)value+(distance-1u)*(uint32_t)change);
     if(value<I(U(0xeadcd8u))){U(0xeadcd8u)=(uint32_t)value;U(0xeadcd4u)=kind==54u?FUN_00475b50(id):id;}
    }
    selected=U(0xeadd6cu);
   }
   cell=after;
  }
  cell+=step;
 }
}
struct house_requirements {int32_t entertainment,water,school,religion,a4d,a48,a5a,foods,goods[4];};
static int32_t goods_requirement(int32_t value,uint8_t merged)
{
 if(merged==1u&&value>0){value=I((uint32_t)value+2u)/4;if(value<1)value=1;}
 return value;
}
static struct house_requirements requirements(uint32_t model,uint32_t record)
{
 struct house_requirements q;uint32_t j;uint8_t merged=B(0x7999dbu+record);
 q.entertainment=I(U(model+8u));q.water=I(U(model+12u));q.school=I(U(model+16u));q.religion=I(U(model+20u));
 q.a4d=I(U(model+28u));q.a48=I(U(model+32u));q.a5a=I(U(model+36u));q.foods=I(U(model+40u));
 for(j=0;j<4u;++j)q.goods[j]=goods_requirement(I(U(model+44u+j*4u)),merged);
 return q;
}
static uint32_t unmet_reason(uint32_t r,const struct house_requirements *q,uint32_t foods,int next)
{
 uint32_t offset=next?30u:0u;int32_t current=C(0x799a58u+r);
 if(q->water==1&&!B(0x799a0du+r)&&!B(0x799a17u+r))return 1u+offset;
 if(q->water==2&&!B(0x799a0du+r))return 2u+offset;
 if(current<q->entertainment){
  if(!B(0x799a58u+r))return 3u+offset;
  return (q->entertainment<10?4u:q->entertainment<25?5u:q->entertainment<50?6u:q->entertainment<80?7u:8u)+offset;
 }
 if(I(foods)<q->foods&&q->foods>=1&&q->foods<=3)return (uint32_t)q->foods+8u+offset;
 if(C(0x799a59u+r)<q->religion){
  if(q->religion==1)return 14u+offset;
  if(q->religion==2){if(B(0x799a4au+r))return 15u+offset;if(B(0x799a4bu+r))return 16u+offset;}
  else if(q->religion==3&&!next)return 17;
 }
 if((int32_t)B(0x799a48u+r)<q->a48)return 18u+offset;
 if(H(0x799a3eu+r)<q->goods[0])return 19u+offset;
 if(C(0x799a5bu+r)<q->school&&q->school>=1&&q->school<=3)return (uint32_t)q->school+19u+offset;
 if((int32_t)B(0x799a4du+r)<q->a4d)return 23u+offset;
 if(C(0x799a5au+r)<q->a5a){
  if(q->a5a==1)return 24u+offset;
  return (B(0x799a46u+r)?25u:26u)+offset;
 }
 if(H(0x799a3cu+r)<q->goods[3])return 29u+offset;
 if(H(0x799a40u+r)<q->goods[1])return 27u+offset;
 if(H(0x799a42u+r)<q->goods[2])return next?68u:72u;
 if(q->goods[2]>1&&I(U(0xea5f14u+U(0xeaac28u)*0x49d8u))<2)return next?74u:70u;
 return 0;
}
void restore_house_explain_evolution(uint32_t building)
{
 uint32_t r=building*264u,model=(uint32_t)H(0x7999eau+r)*88u+0x70cdccu,mode,count,at,foods=0,food,index,reason;
 int32_t level=H(0x7999eau+r),desire=C(0x799a96u+r);struct house_requirements q;
 mode=desire<=I(U(model))?UINT32_MAX:(uint32_t)(desire>=I(U(model+4u)));
 q=requirements(model,r);count=U(0xf53dd8u);
 if(I(count)>0)for(at=0xf53ddcu;count;--count,at+=4u){
  food=U(at);if(!food)continue;
  index=food>=1u&&food<=8u?food-1u:food==13u?9u:food==15u?8u:food==17u?10u:food==19u?11u:UINT32_MAX;
  if(W(0x799a2cu+r+index*2u))++foods;
 }
 if(mode==UINT32_MAX){B(0x799a5du+r)=0;return;}
 reason=unmet_reason(r,&q,foods,0);
 if(reason){B(0x799a5du+r)=(uint8_t)reason;return;}
 if(level>=19){B(0x799a5du+r)=60;return;}
 q=requirements(model+88u,r);
 if(!mode){B(0x799a5du+r)=U(0xeadcd4u)?62:30;return;}
 reason=unmet_reason(r,&q,foods,1);
 if(reason){B(0x799a5du+r)=(uint8_t)reason;return;}
 B(0x799a5du+r)=61;if(B(0x799a56u+r)==1u)B(0x799a5du+r)=64;
}
void FUN_00454190(uint32_t building){restore_house_explain_evolution(building);}
uint32_t FUN_0045d570(uint32_t building){return restore_eight_link_status(building);}
uint32_t FUN_00475b50(uint32_t building){return restore_building_root(building);}
uint32_t FUN_004b7590(void){return restore_has_active_formation();}
void FUN_0050f0f0(void){restore_house_find_negative_influence();}
