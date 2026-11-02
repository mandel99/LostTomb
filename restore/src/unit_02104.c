#include "monument_worker_motion.h"
/* Whole native 4EF450:256 bytes,86 instructions. Choose all tied work
   pieces at the preferred phase/row/column; phase0 sorts ascending and
   the other phases sort descending. Table counts are DWORD values. */
void restore_pyramid_collect_work_pieces(int32_t building)
{
 uint32_t record=restore_monument_worker_building(building),kind,best=0,current;
 uint32_t root,index,best_record,table,count,phase,column,row,old_phase,old_column,old_row;
 int32_t mode;
 MW_U(0xeaaf58u)=0;
 kind=MW_B(record+84u);root=(uint32_t)building-(uint32_t)MW_H(record+18u);
 index=root;
 for(;;++index){
  mode=(int32_t)MW_U(0x5e4ef4u);table=mode==5?0x56f7ecu:0x56f7d8u;
  if((int32_t)index>=(int32_t)(root+MW_U(table+kind*4u)))return;
  current=restore_monument_worker_building((int32_t)index);
  phase=MW_B(current+85u);column=MW_B(current+86u);row=MW_B(current+87u);
  if(phase&&(column==0||mode==3||mode==5))continue;
  if(best){
   best_record=restore_monument_worker_building((int32_t)best);
   old_phase=MW_B(best_record+85u);old_column=MW_B(best_record+86u);old_row=MW_B(best_record+87u);
   if(phase!=old_phase){if(phase)continue;}
   else if(phase){
    if(row<old_row||(row==old_row&&column<old_column))continue;
    if(row==old_row&&column==old_column)goto append;
   }else{
    if(row>old_row||(row==old_row&&column>old_column))continue;
    if(row==old_row&&column==old_column)goto append;
   }
  }
  MW_U(0xeaaf58u)=0;best=index;
append:
  count=MW_U(0xeaaf58u);MW_U(0xeaac44u+count*4u)=index;MW_U(0xeaaf58u)=count+1u;
 }
}
void FUN_004ef450(int32_t building){restore_pyramid_collect_work_pieces(building);}
