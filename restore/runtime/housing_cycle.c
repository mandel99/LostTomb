#include "housing_cycle.h"
#include "house_levels.h"
#include "building_controllers.h"
#include "simulation_tick.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
extern void FUN_00517370(void);
extern void FUN_00517660(void);

void restore_housing_update_all(void)
{
 uint32_t cursor=1,next,phase,kind;
 FUN_00454d70();U(0x78d588u)=0;U(0x78d838u)=cursor;
 do {
  next=cursor;
  if(B(0x7999d8u+cursor*264u)==1u&&
     (int32_t)(kind=(uint32_t)H(0x7999e8u+cursor*264u))>=10&&(int32_t)kind<=29){
   restore_house_repair_position(cursor);
   next=U(0x78d838u);U(0x78d7e4u)=0;
   kind=(uint32_t)H(0x7999e8u+next*264u);
   restore_house_update_by_entry(U(0x5da3d4u+kind*4u));
   if(!U(0x78d7c4u))FUN_00454b40((int32_t)U(0x78d838u));
   phase=U(0x78d7c4u);next=U(0x78d838u);
   if(phase==7u){FUN_00454b40((int32_t)next);next=U(0x78d838u);}
  }
  cursor=next+1u;U(0x78d838u)=cursor;
 }while((int32_t)cursor<4000);
 if(U(0x78d588u)){FUN_00517370();FUN_00517660();}
}

void restore_house_repair_position(uint32_t building)
{
 uint32_t r=building*264u,tile=U(0x7999e4u+r),origin,row=0,column,width;
 uint32_t expected=(uint32_t)H(0x7999e0u+r)*228u+(uint32_t)H(0x7999deu+r);
 B(0x799a56u+r)=0;origin=U(0xbca2d8u);
 if(tile==origin+expected&&(uint32_t)H(0xb4e6fcu+tile*2u)==building)return;
 U(0x78d5c4u)=U(0x78d5c4u)+1u;tile=origin;
 if((int32_t)U(0xbca2d4u)>0){
  width=U(0xbca2d0u);
  do {
   if((int32_t)width>0)for(column=0;(int32_t)column<(int32_t)width;++column,++tile){
    if((uint32_t)H(0xb4e6fcu+tile*2u)==building){
     int32_t delta=(int32_t)(tile-U(0xbca2d8u));
     U(0x7999e4u+r)=tile;W(0x7999deu+r)=(uint16_t)(delta%228);
     W(0x7999e0u+r)=(uint16_t)(delta/228);return;
    }
   }
   tile+=U(0xbca2dcu);++row;
  }while((int32_t)row<(int32_t)U(0xbca2d4u));
 }
 B(0x7999d8u+r)=4;U(0x78d5c0u)=U(0x78d5c0u)+1u;
}

static int32_t demand(uint32_t value,const uint32_t slots[3],uint32_t beer,uint32_t jewelry,uint32_t ebx)
{
 int32_t signed_value=(int32_t)value,remainder=signed_value%4;
 if(!remainder)return signed_value/4;
 if(remainder>0)return (int32_t)slots[remainder-1];
 if(remainder==-1)return (int32_t)beer;
 if(remainder==-2)return (int32_t)jewelry;
 return (int32_t)ebx;
}
static void consume(uint32_t address,int32_t amount)
{
 if(amount>0){int32_t stock=H(address);W(address)=amount>stock?0:(uint16_t)(stock-amount);}
}
void restore_house_consume_goods(uint32_t building,const restore_house_goods_context *context)
{
 uint32_t r=building*264u,model=(uint32_t)H(0x7999eau+r)*88u;
 uint32_t large=B(0x7999dbu+r)>1u,pottery,linen,jewelry,beer,slots[3];
 int32_t phase,amounts[4];
 pottery=U(0x70cdf8u+model);if(large)pottery<<=2;
 linen=U(0x70cdfcu+model);U(context->argument_word)=linen;
 if(large){linen<<=2;U(context->argument_word)=linen;}
 jewelry=U(0x70ce00u+model);if(large)jewelry<<=2;
 beer=U(0x70ce04u+model);if(large)beer<<=2;
 phase=(int32_t)U(0x78d7c4u)%4;
 slots[0]=context->slots[0];slots[1]=context->slots[1];slots[2]=context->slots[2];
 if(!phase){slots[0]=1;slots[1]=1;slots[2]=0;}
 else if(phase==2){slots[0]=0;slots[1]=1;slots[2]=1;}
 else if(phase>0){slots[0]=0;slots[1]=0;slots[2]=1;}
 amounts[0]=demand(beer,slots,beer,jewelry,context->ebx);
 amounts[1]=demand(pottery,slots,beer,jewelry,context->ebx);
 amounts[2]=demand(linen,slots,beer,jewelry,context->ebx);
 amounts[3]=demand(jewelry,slots,beer,jewelry,context->ebx);
 consume(0x799a3cu+r,amounts[0]);consume(0x799a3eu+r,amounts[1]);
 consume(0x799a40u+r,amounts[2]);consume(0x799a42u+r,amounts[3]);
}
void restore_building_control_C_004528b0(struct restore_building_control_frame *f)
{restore_housing_update_all();f->r[4]+=4u;}
void restore_building_control_C_0046b740(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];restore_house_repair_position(U(sp+4u));f->r[4]=sp+4u;}
void restore_building_control_C_00454b40(struct restore_building_control_frame *f)
{
 uint32_t sp=f->r[4],building=U(sp+4u);
 restore_house_goods_context context={f->r[3],{U(sp-12u),U(sp-8u),U(sp-4u)},sp+4u};
 restore_house_consume_goods(building,&context);f->r[4]=sp+4u;
}
