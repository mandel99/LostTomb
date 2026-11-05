#include "crt_format.h"

void restore_crt_scale_raw96(RestoreCrt96 *value,int32_t power,int32_t preserve_tail)
{
 uintptr_t table=0x605438u;
 uint32_t magnitude=(uint32_t)power,index;
 const RestoreCrt96 *factor;
 RestoreCrt96 adjusted;
 if(power==0)return;
 if(power<0){magnitude=0u-magnitude;table=0x605598u;}
 if(preserve_tail==0)value->half[0]=0;
 do{
  table+=0x54u;
  index=magnitude&7u;
  /* Native SAR, retained even for the INT_MIN magnitude case. */
  magnitude=(uint32_t)((int32_t)magnitude>>3);
  if(index!=0){
   factor=(const RestoreCrt96 *)(table+index*12u);
   if(factor->half[0]>=0x8000u){
    adjusted=*factor;
    *(uint32_t *)(void *)(adjusted.byte+2)-=1u;
    factor=&adjusted;
   }
   FUN_0056a904(value,factor);
  }
 }while(magnitude!=0);
}
