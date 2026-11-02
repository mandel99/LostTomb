#include "pyramid_work_selection.h"
/* Whole native4F27B0:250 bytes,99 instructions. The BYTE out parameter
   identifies the second side only after its paired piece is found. */
int32_t restore_pyramid_find_paired_work_piece(int32_t building,uint8_t *second)
{
 uint32_t record=restore_monument_worker_building(building),kind=MW_B(record+84u),table,i;
 int32_t piece,target;uint8_t side;
 *second=0;
 table=MW_U(0x5e4ef4u)==5u?0x5f5d58u+kind*160u:0x5f5f38u+kind*624u;
 piece=MW_H(record+18u);
 for(;;table+=8u){
  int32_t first=(int32_t)MW_U(table),last;
  if(first<0)return -1;
  last=(int32_t)MW_U(table+4u);
  if(piece==last){target=first;side=1;break;}
  if(piece==first){target=last;side=0;break;}
 }
 for(i=0;(int32_t)i<(int32_t)MW_U(0xeaaf58u);++i){
  int32_t candidate=(int32_t)MW_U(0xeaac44u+i*4u);
  if(MW_H(restore_monument_worker_building(candidate)+18u)==target){*second=side;return candidate;}
 }
 return -1;
}
int32_t FUN_004f27b0(int32_t building,uint8_t *second)
{return restore_pyramid_find_paired_work_piece(building,second);}
