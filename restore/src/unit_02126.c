#include "monument_layout_queries.h"
int32_t restore_monument_find_layout_pair(int32_t building)
{
 uint32_t record=restore_monument_worker_building(building),kind,mode,pair,root,left;
 int32_t piece;
 kind=MW_B(record+84u);
 mode=MW_U(0x5e4ef4u);
 pair=mode==5u ? 0x5f5d58u+kind*160u : 0x5f5f38u+kind*624u;
 piece=MW_H(record+18u);
 root=(uint32_t)building-(uint32_t)piece;
 left=MW_U(pair);
 while ((int32_t)left>=0) {
  if ((int32_t)MW_U(pair+4u)==piece) return (int32_t)(MW_U(pair)+root);
  if ((int32_t)MW_U(pair)==piece) return (int32_t)(MW_U(pair+4u)+root);
  pair+=8u;
  left=MW_U(pair);
 }
 return -1;
}
int32_t FUN_004f2720(int32_t building)
{ return restore_monument_find_layout_pair(building); }
