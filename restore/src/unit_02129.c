#include "pyramid_work_selection.h"
/* Whole native4F28E0:120 bytes,49 instructions. A missing paired piece
   returns-1. A pair without an active worker returns0. */
int32_t restore_pyramid_paired_worker(int32_t building)
{
 uint8_t second;int32_t pair=restore_pyramid_find_paired_work_piece(building,&second),owner;
 uint32_t record,type,state;
 if(pair<=0)return -1;
 owner=MW_H(restore_monument_worker_building(pair)+100u);
 if(!owner)return 0;
 record=restore_monument_worker_record(owner);type=MW_B(record+MW_TYPE);state=MW_B(record+MW_STATE);
 if((type==80u&&state==11u)||(type==81u&&state==10u))return owner;
 return 0;
}
int32_t FUN_004f28e0(int32_t building){return restore_pyramid_paired_worker(building);}
