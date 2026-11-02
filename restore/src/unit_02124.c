#include "pyramid_worker_state.h"
/* Whole native4F25C0:218 bytes,69 instructions. The genuine C port of
   native qsort/comparator preserves tie ordering; do not substitute CRT qsort. */
void restore_pyramid_sort_work_list(void)
{
 uint32_t count=MW_U(0xeaaf58u),first,record,root,table,limit,i;
 int32_t position,anchor;
 if((int32_t)count<2)return;
 first=MW_U(0xeaac44u);record=restore_monument_worker_building((int32_t)first);
 root=first-(uint32_t)MW_H(record+18u);position=(int32_t)(MW_B(record+86u)+MW_B(record+87u)*6u);
 if(MW_U(0x5e4ef4u)==5u){limit=16;table=0x5f5498u+MW_B(record+84u)*128u;}
 else{limit=60;table=0x5f4b38u+MW_B(record+84u)*480u;}
 anchor=(int32_t)first;
 for(i=0;i<limit;++i){anchor=(int32_t)(root+MW_U(table+i*8u));if((int32_t)MW_U(table+i*8u+4u)>=position)break;}
 record=restore_monument_worker_building(anchor);
 MW_U(0xeaac40u)=(uint32_t)MW_H(record+6u);MW_U(0xeaac3cu)=(uint32_t)MW_H(record+8u);
 restore_pyramid_sort_entries(count);
}
void FUN_004f25c0(void){restore_pyramid_sort_work_list();}
