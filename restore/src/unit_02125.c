#include "pyramid_worker_state.h"
/* Whole native4F26A0:119 bytes,41 instructions. Return AL only. */
uint8_t restore_pyramid_linked_worker_active(int32_t actor,int32_t owner)
{
 uint32_t record=restore_monument_worker_record(actor),state;
 if(!MW_B(record+PS_CLAIM_VALIDITY)||MW_B(record+MW_TYPE)!=85u)return 0;
 state=MW_B(record+MW_STATE);if(state==6u||state==7u)state=MW_B(record+PS_SAVED_STATE);
 switch(state){case 15:case 16:case 17:case 30:case 18:case 19:case 29:case 20:break;default:return 0;}
 return (uint8_t)(MW_H(record+PS_LINKED_ACTOR)==owner);
}
uint8_t FUN_004f26a0(int32_t actor,int32_t owner){return restore_pyramid_linked_worker_active(actor,owner);}
