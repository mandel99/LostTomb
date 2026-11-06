#include "monument_layout_queries.h"
#include "pyramid_work_selection.h"
uint32_t restore_monument_worker_query_raw(int32_t building)
{
 uint32_t record,initial_kind,count,i=0,result;
 if (MW_U(0x5e4ef4u)==5u) return 0;
 record=restore_monument_worker_building(building);
 initial_kind=MW_B(record+84u);
 count=MW_U(0x570070u+initial_kind*4u);
 if ((int32_t)count<=0) return count&0xffffff00u;
 do {
  uint32_t kind=MW_B(record+84u);
  uint32_t part=MW_U(0x5f3878u+kind*960u+i*20u)+(uint32_t)building;
  result=(uint32_t)FUN_004ef100((int32_t)part);
  if (result!=0) return (result&0xffffff00u)|1u;
  count=MW_U(0x570070u+initial_kind*4u);
  ++i;
 } while ((int32_t)i<(int32_t)count);
 return count&0xffffff00u;
}
uint32_t FUN_004f5d10(int32_t building)
{ return restore_monument_worker_query_raw(building); }
