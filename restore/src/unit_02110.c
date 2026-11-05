#include "actor_value_contracts.h"
#include "pyramid_work_selection.h"

/* 4F01E0: a nonzero availability query ends the descriptor search even
   when its claim is busy. The descriptor category is live, while the
   count-table category is the BYTE cached before entering that search. */
uint32_t restore_pyramid_assign_available_worker(int32_t actor)
{
 uint32_t root=1,record,kind,index,piece,family,step;
 if(MW_U(0x5e4ef4u)==5u)return 0;
 do{
  record=restore_monument_worker_building((int32_t)root);
  if(MW_B(record)==1u&&MW_W(record+16u)==183u){
   kind=MW_B(record+84u);index=0;
   if((int32_t)MW_U(0x570070u+kind*4u)>0)do{
    piece=root+MW_U(0x5f3878u+(MW_B(record+84u)*48u+index)*20u);
    if(restore_pyramid_available_worker_slot((int32_t)piece)){
     if(MW_W(restore_monument_worker_building((int32_t)piece)+100u)==0||
        !restore_pyramid_worker_claim_active((int32_t)piece)){
      MW_W(restore_monument_worker_building((int32_t)piece)+100u)=(uint16_t)actor;
      return restore_monument_worker_publish_target_eax(actor,(int32_t)piece);
     }
     break;
    }
    ++index;
   }while((int32_t)index<(int32_t)MW_U(0x570070u+kind*4u));
   family=MW_U(0x5e4ef4u);
   step=MW_U((family==5u?0x56f7ecu:0x56f7d8u)+MW_B(record+84u)*4u);
   root=root+step-1u;
  }
  ++root;
 }while((int32_t)root<4000);
 return 0;
}
