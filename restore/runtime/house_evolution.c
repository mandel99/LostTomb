#include "house_evolution.h"
#include "building_controllers.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define C(a) ((int32_t)(int8_t)B(a))
extern void FUN_0046cea0(int32_t building,int16_t kind);

uint32_t restore_house_check_global_mode(void)
{
 uint32_t building,r;
 /* The original mode field is unnamed; a nonzero value blocks evolution. */
 if(!U(0x978d70u))return 0;
 building=U(0x78d838u);r=building*264u;
 if(!B(0x7999dcu+r))FUN_0046cea0((int32_t)building,(int16_t)W(0x7999e8u+r));
 return 1;
}
int32_t restore_house_compare_desirability(uint32_t building)
{
 uint32_t r=building*264u;int32_t level=H(0x7999eau+r),low,high,value;
 uint32_t row=(uint32_t)level*88u;
 low=(int32_t)U(0x70cdccu+row);high=(int32_t)U(0x70cdd0u+row);
 if(level>=19)high=1000;
 value=C(0x799a96u+r);
 if(value<=low)return -1;
 return value>=high;
}
static void count(uint32_t address,uint32_t player)
{address+=player*0x49d8u;U(address)=U(address)+1u;}
static uint32_t unmet(uint32_t address,uint32_t player)
{count(address,player);return 0;}
static int32_t goods_required(int32_t quantity,uint8_t size)
{
 if(size==1u&&quantity>0){quantity=(int32_t)((uint32_t)quantity+2u)/4;if(quantity<1)quantity=1;}
 return quantity;
}
uint32_t restore_house_check_requirements(uint32_t building,uint32_t next_level)
{
 uint32_t r=building*264u,row,player,food_count=0,food_total,i;
 int32_t level=H(0x7999eau+r),water,entertainment,religion,education,dentist,physician,health,foods;
 int32_t pottery,linen,jewelry,beer,value;uint8_t size=B(0x7999dbu+r);
 if(next_level)level=(int32_t)((uint32_t)level+1u);
 row=(uint32_t)level*88u;
 /* Cache model requirements before any counter writes or record reloads.
    Column names are documented in the original Pharaoh_Model files. */
 religion=(int32_t)U(0x70cddcu+row);water=(int32_t)U(0x70cdd8u+row);
 dentist=(int32_t)U(0x70cde8u+row);physician=(int32_t)U(0x70cdecu+row);
 health=(int32_t)U(0x70cdf0u+row);entertainment=(int32_t)U(0x70cdd4u+row);
 education=(int32_t)U(0x70cde0u+row);foods=(int32_t)U(0x70cdf4u+row);
 pottery=goods_required((int32_t)U(0x70cdf8u+row),size);
 linen=goods_required((int32_t)U(0x70cdfcu+row),size);
 jewelry=goods_required((int32_t)U(0x70ce00u+row),size);
 beer=goods_required((int32_t)U(0x70ce04u+row),size);
 if(!B(0x799a0du+r)){
  if(water>=2)return unmet(0xea5b74u,U(0xeaac28u));
  if(water==1&&!B(0x799a17u+r))return unmet(0xea5b78u,U(0xeaac28u));
 }
 value=C(0x799a58u+r);
 if(value<entertainment)return unmet(value?0xea5b7cu:0xea5bf8u,U(0xeaac28u));
 value=C(0x799a59u+r);
 if(value<education)return unmet(value?0xea5b80u:0xea5b84u,U(0xeaac28u));
 /* Native latches the full DWORD player here, after the early failures. */
 player=U(0xeaac28u);
 if(education==2){count(0xea5b88u,player);count(0xea5b8cu,player);}
 else if(education==1)count(0xea5b88u,player);
 value=C(0x799a5bu+r);
 if(value<religion){
  if(religion==1)return unmet(0xea5bc0u,player);
  if(religion==2)return unmet(0xea5bc4u,player);
  if(religion==3)return unmet(0xea5bc8u,player);
 }else if(religion>0)count(0xea5bccu,player);
 if((int32_t)B(0x799a4du+r)<dentist)return unmet(0xea5b94u,player);
 if(dentist==1)count(0xea5bb4u,player);
 value=B(0x799a48u+r);
 if(value<physician)return unmet(value?0xea5b98u:0xea5fbcu,player);
 if(physician==2){count(0xea5bb8u,player);count(0xea5fb8u,player);}
 else if(physician==1)count(0xea5bb8u,player);
 value=C(0x799a5au+r);
 if(value<health)return unmet(health>=2?0xea5bb0u:0xea5bacu,player);
 if(health>0)count(0xea5bbcu,player);
 food_total=U(0xf53dd8u);
 if((int32_t)food_total>0)for(i=0;i<food_total;++i){
  uint32_t resource=U(0xf53ddcu+i*4u),slot;
  if(!resource)continue;
  switch(resource){
  case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:slot=resource-1u;break;
  case 15:slot=8;break;case 13:slot=9;break;case 17:slot=10;break;case 19:slot=11;break;
  default:slot=0xffffffffu;break;
  }
  /* An unsupported nonzero resource reads the preceding WORD, as native. */
  if(W(0x799a2cu+r+slot*2u))++food_count;
 }
 if((int32_t)food_count<foods)return unmet(0xea5b9cu,player);
 if(H(0x799a3eu+r)<pottery)return 0;
 if(beer&&H(0x799a3cu+r)<=0)return 0;
 if(H(0x799a40u+r)<linen)return 0;
 if(H(0x799a42u+r)<jewelry)return 0;
 /* Preserve the native jewelry-gated beer check; it does not test beer>=2. */
 if(jewelry>=2&&(int32_t)U(0xea5f14u+player*0x49d8u)<2)return unmet(0xea5f6cu,player);
 return 1;
}
void restore_building_control_C_004539a0(struct restore_building_control_frame *f)
{f->r[0]=restore_house_check_global_mode();f->r[4]+=4u;}
void restore_building_control_C_00453af0(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_house_compare_desirability(U(sp+4u));f->r[4]=sp+4u;}
void restore_building_control_C_00453b40(struct restore_building_control_frame *f)
{uint32_t sp=f->r[4];f->r[0]=restore_house_check_requirements(U(sp+4u),U(sp+8u));f->r[4]=sp+4u;}
