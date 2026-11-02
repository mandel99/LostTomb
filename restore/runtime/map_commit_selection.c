#include "map_commit_selection.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_00426b30(void); /* Complete accepted native leaf returns 38. */
int32_t FUN_004f44e0(void)
{
 uint32_t slot,index;
 for(slot=0;slot<3;++slot){
  index=(uint32_t)(int32_t)H(0x784de0u+slot*2u);
  if(index&&H(0x5d19c8u+index*16u)==1)return H(0x5d19cau+index*16u);
 }
 return 4;
}
int32_t FUN_0053d930(void)
{
 uint32_t slot,index;
 for(slot=0;slot<2;++slot){
  index=(uint32_t)(int32_t)H(0x784de0u+slot*2u);
  if(index&&H(0x5d19c8u+index*16u)==2&&H(0x5d19cau+index*16u)==4)return H(0x5d19ccu+index*16u);
 }
 return 0;
}
/* mode0 is the pyramid family/variant search; mode1 compares a fixed third
 * WORD; mode2 obtains that WORD from the first-two-slot god lookup; mode3
 * compares only the first two WORDs. Every native count/family call is retained. */
static int32_t find_option(uint32_t record,int32_t kind,int32_t subtype,int32_t parameter,uint32_t mode)
{
 uint32_t row=0,index=0;int32_t count=(int32_t)FUN_00426b30(),family;
 if(count<=0)return -1;
 do{
  row=0x5d19c8u+index*16u;
  if(H(row)==kind){
   if(mode==0){
    family=FUN_004f44e0();
    if(H(row+2u)==family){parameter=B(record+84u);if((uint16_t)H(row+4u)==(uint16_t)parameter)return (int32_t)index;}
   }else if(H(row+2u)==subtype){
    if(mode==3)return (int32_t)index;
    if(mode==2)parameter=FUN_0053d930();
    if(H(row+4u)==parameter)return (int32_t)index;
   }
  }
  ++index;count=(int32_t)FUN_00426b30();
 }while((int32_t)index<count);
 return -1;
}
int32_t FUN_004f4b70(int32_t building)
{
 uint32_t record=0x7999d8u+(uint32_t)building*264u;int32_t result;
 switch(H(record+16u)){
 case 0xb7:return find_option(record,1,0,0,0);
 case 0xcf:return find_option(record,2,2,0,1);
 case 0xd0:return find_option(record,2,2,1,1);
 case 0xd2:return find_option(record,2,1,0,3);
 case 0xd5:return find_option(record,2,3,0,3);
 case 0xde:result=find_option(record,2,4,0,2);if(result!=-1)return result;return find_option(record,2,10,0,3);
 case 0xe1:return find_option(record,2,6,0,3);
 case 0xe3:return find_option(record,2,7,0,3);
 case 0xe4:return find_option(record,2,5,0,3);
 case 0xe5:return find_option(record,2,10,0,3);
 case 0xe6:return find_option(record,2,14,0,3);
 case 0xea:return find_option(record,2,11,0,3);
 case 0xeb:return find_option(record,2,12,0,3);
 case 0xec:return find_option(record,2,13,0,3);
 default:return -1;
 }
}
void FUN_004f61e0(void)
{
 uint32_t player=U(0xeaac28u),slot,index=1,original,record,state,variant,size,last;int32_t type,result;uint32_t match;
 for(slot=0;slot<3;++slot){U(0xea5fa0u+player*0x49d8u+slot*4u)=0;U(0xea5facu+player*0x49d8u+slot*4u)=0;}
 last=U(0x78d790u);if((int32_t)last<1)return;
 do{
  record=0x7999d8u+index*264u;state=B(record);
  if((state!=1&&state!=3)||H(record+22u)==-1)goto next_record;
  type=H(record+16u);original=index;
  switch(type){
  case 0xb7:
   result=FUN_004f4b70((int32_t)index);state=U(0x5e4ef4u);variant=B(record+84u);
   size=U((state==5?0x56f7ecu:0x56f7d8u)+variant*4u);index+=size-1u;break;
  case 0xcf:case 0xd0:case 0xe4:result=FUN_004f4b70((int32_t)index);break;
  case 0xd2:if(H(record+56u))goto next_record;result=FUN_004f4b70((int32_t)index);break;
  case 0xd5:result=FUN_004f4b70((int32_t)index);index+=99u;break;
  case 0xde:result=FUN_004f4b70((int32_t)index);index+=115u;break;
  case 0xe1:result=FUN_004f4b70((int32_t)index);index+=24u;break;
  case 0xe3:result=FUN_004f4b70((int32_t)index);index+=8u;break;
  case 0xe5:result=FUN_004f4b70((int32_t)index);index+=166u;break;
  case 0xe6:result=FUN_004f4b70((int32_t)index);index+=15u;break;
  case 0xea:result=FUN_004f4b70((int32_t)index);index+=157u;break;
  case 0xeb:result=FUN_004f4b70((int32_t)index);index+=436u;break;
  case 0xec:result=FUN_004f4b70((int32_t)index);index+=512u;break;
  default:goto next_record;
  }
  if(result>0){
   player=U(0xeaac28u);
   for(slot=0;slot<3;++slot){
    match=(uint32_t)(int32_t)H(0x784de0u+slot*2u);
    if(match==(uint32_t)result&&!U(0xea5facu+player*0x49d8u+slot*4u))break;
   }
   if(slot<3){U(0xea5fa0u+player*0x49d8u+slot*4u)=(uint32_t)result;U(0xea5facu+player*0x49d8u+slot*4u)=original;}
  }
next_record:
  last=U(0x78d790u);++index;
 }while((int32_t)index<=(int32_t)last);
}
