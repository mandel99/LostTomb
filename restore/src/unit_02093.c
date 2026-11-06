#include "monument_layout_queries.h"
extern void FUN_004ee4a0(int32_t building);
void restore_monument_refresh_all(void)
{
 uint32_t building=1;
 do {
  uint32_t record=restore_monument_worker_building((int32_t)building);
  if (MW_B(record)!=0 && MW_W(record+16u)==0xb7u) {
   uint32_t mode,kind,count;
   FUN_004ee4a0((int32_t)building);
   mode=MW_U(0x5e4ef4u);
   kind=MW_B(record+84u);
   count=MW_U((mode==5u ? 0x56f7ecu : 0x56f7d8u)+kind*4u);
   building+=count-1u;
  }
  ++building;
 } while ((int32_t)building<4000);
}
void FUN_004ee560(void)
{ restore_monument_refresh_all(); }
