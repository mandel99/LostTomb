#include "monument_worker_motion.h"
/* Whole native 4F1CB0:87 bytes,16 instructions. */
void restore_monument_worker_reset_travel(int32_t actor)
{
 uint32_t record=restore_monument_worker_record(actor);
 uint16_t building;
 MW_W(record+MW_MONUMENT_ACTION)=1;
 building=MW_W(0xb4e6fcu+MW_U(record+MW_TARGET_CELL)*2u);
 MW_W(record+MW_FROM_BUILDING)=building;
 MW_W(record+MW_FROM_POSITION)=0;
 MW_W(record+MW_TO_BUILDING)=building;
 MW_W(record+MW_TO_POSITION)=0;
 MW_W(record+MW_TRAVEL_PROGRESS)=1;
 MW_W(record+MW_TRAVEL_LENGTH)=15;
}
void FUN_004f1cb0(int32_t actor){restore_monument_worker_reset_travel(actor);}
