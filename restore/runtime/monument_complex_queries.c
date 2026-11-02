#include "monument_complex_queries.h"
#define MC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MC_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MC_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static uint8_t complex_worker(uint32_t building,uint8_t pyramid)
{
 uint32_t offset=building*264u,slot=0x799a34u+offset,worker,actor,state;uint8_t type;
 if(pyramid&&MC_W(0x7999e8u+offset)!=0xde)return 0;
 worker=MC_U(slot);if(!worker)return 0;actor=worker*388u;
 if(!MC_B(0xbcd606u+actor)){MC_U(slot)=0;return 0;}
 state=MC_B(0xbcd630u+actor);if(state==6||state==7)state=MC_B(0xbcd608u+actor);
 type=MC_B(0xbcd602u+actor);
 if(type==0x4f&&(state==9||state==10))return 1;
 if(type==0x51&&(state==9||state==10||state==11))return 1;
 if(type==0x55&&(state==15||state==16||(state>=(pyramid?26u:23u)&&state<=(pyramid?28u:25u))))return 1;
 MC_U(slot)=0;return 0;
}
uint8_t FUN_004e0530(uint32_t building){return complex_worker(building,1);}
uint8_t FUN_00542110(uint32_t building){return complex_worker(building,0);}
void FUN_004204a0(uint32_t type,int32_t *output)
{
 uint32_t row;volatile uint32_t *out=(volatile uint32_t *)output;*out=0;
 for(row=0x596084u;row<0x596154u;row+=16u){
  uint32_t object=MC_U(row+4u);
  if(MC_W(object+0x12u)&&MC_U(row)==type)*out+=1u;
 }
}
static uint32_t complex_root(uint32_t building)
{return building-(uint32_t)(int32_t)(int16_t)MC_W(0x7999eau+building*264u);}
static int count_worker(uint32_t building,uint32_t *q,uint32_t *q10,uint32_t *u,uint32_t *o)
{
 uint32_t worker=MC_U(0x799a34u+building*264u),actor;uint8_t type;
 if(!worker)return 0;actor=worker*388u;type=MC_B(0xbcd602u+actor);
 if(type==0x4f)++*o;else if(type==0x51){if(MC_B(0xbcd630u+actor)==10)++*q10;++*q;}else if(type==0x55)++*u;
 return 1;
}
int32_t FUN_004df040(uint32_t building,int32_t *output)
{
 uint32_t root=complex_root(building),flags=MC_U(0x799a3cu+root*264u),remaining=0,phase=1;
 uint32_t missing=0,empty=0,tiles=0,cost=0,q=0,q10=0,u=0,o=0,i;int32_t percent;
 if(flags&1){remaining=(flags&0x10)?3u:4u;if(flags&0x20)--remaining;if(flags&0x40)--remaining;if(flags&0x80)--remaining;}
 for(i=0;i<116;++i){
  uint32_t id=root+i,slot=0x799a30u+id*264u,kind,stage;
  /* Native invariant check retained: actual helper AL1 leaves this worker nonzero. */
  if(FUN_004e0530(id)&&!count_worker(id,&q,&q10,&u,&o))return 0;
  kind=MC_U(0x5e2ae4u+(uint32_t)(int32_t)(int16_t)MC_W(slot-0x46u)*52u)&255u;
  if(kind<=3){
   uint32_t bits=MC_U(slot),set=0,j;
   for(j=0;j<16;++j)if(bits&(1u<<j))++set;
   tiles+=16u-set;cost+=(16u-set)*400u;if(set)phase=2;
  }else if((kind>=4&&kind<=10)||kind==12){stage=MC_U(slot);if(stage==0){++missing;++empty;}else if(stage==1)++empty;}
 }
 if(!missing&&phase==1)phase=2;if(flags&3)phase=3;
 percent=(int32_t)((288u-empty-tiles-missing)*100u)/288;
 if(output){volatile uint32_t *out=(volatile uint32_t *)output;out[1]=missing;out[0]=phase;out[4]=q;out[5]=q10;out[6]=u;out[7]=cost;out[10]=o;out[2]=tiles;out[3]=empty;out[8]=(uint32_t)percent;out[9]=0x6400;out[11]=remaining;}
 return percent;
}
int32_t FUN_00542ad0(uint32_t building,int32_t *output)
{
 uint32_t root=complex_root(building),phase=1,flag=(MC_U(0x799a3cu+root*264u)>>1)&1u;
 uint32_t missing=0,stages=0,kind1=0,kind47=0,kind8=0,special=0,cost=0,q=0,q10=0,u=0,o=0,i;int32_t global_o=0,global_q=0,percent;
 for(i=0;i<100;++i){
  uint32_t id=root+i,slot=0x799a30u+id*264u,kind,stage;
  if(FUN_00542110(id)&&!count_worker(id,&q,&q10,&u,&o))return 0;
  kind=MC_U(0x601de4u+(uint32_t)(int32_t)(int16_t)MC_W(slot-0x46u)*52u);
  switch(kind){
  case 0:
   stage=MC_U(slot);stages+=13u-stage;
   if(stage==0)phase=flag+1u;else if(stage<=3)phase=2;else if(stage<=12)phase=3;else if(stage==13)phase=4;
   break;
  case 1:
   stage=MC_U(slot);if(stage==0){++missing;++kind1;cost+=400u;}else if(stage==1){++kind1;cost+=400u;}break;
  case 2:case 3:case 4:case 5:case 6:case 7:
   stage=MC_U(slot);if(stage==0){++missing;++kind47;cost+=400u;}else if(stage==1){++kind47;cost+=400u;}break;
  case 8:case 9:case 10:case 11:
   stage=MC_U(slot);if(stage==0){++missing;++kind8;}else if(stage==1)++kind8;break;
  case 12:
   stage=MC_U(slot);if(stage<=3){if(!stage)++missing;special+=stage<=1?3u:4u-stage;cost+=(stage<=1?3u:4u-stage)*400u;}break;
  }
 }
 FUN_004204a0(0x4f,&global_o);FUN_004204a0(0x51,&global_q);
 percent=(int32_t)((213u-special-kind8-kind47-stages-missing)*100u)/213;
 if(output){volatile uint32_t *out=(volatile uint32_t *)output;out[1]=missing;out[0]=phase;out[2]=stages;out[5]=kind8;out[3]=kind1;out[4]=kind47;out[7]=(uint32_t)global_q+q;out[8]=q10;out[9]=u;out[6]=special;out[10]=(uint32_t)global_o+o;out[11]=cost;out[12]=(uint32_t)percent;out[13]=38000;}
 return percent;
}
