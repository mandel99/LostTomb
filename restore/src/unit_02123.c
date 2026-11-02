#include "monument_worker_motion.h"
extern void FUN_004b5420(int32_t),FUN_004b52f0(int32_t);
/* Whole native 4F2410:147 bytes,37 instructions. The source of the target
   coordinates is the current actor global, reloaded after grid unlink. */
void restore_monument_worker_snap_to_target(int32_t actor)
{
 uint32_t record=restore_monument_worker_record(actor),current,building;
 FUN_004b5420(actor);
 current=restore_monument_worker_record((int32_t)MW_U(0xd056f8u));
 MW_W(record+MW_X)=MW_W(current+MW_TARGET_X);
 MW_W(record+MW_Y)=MW_W(current+MW_TARGET_Y);
 MW_U(record+MW_CELL)=MW_U(current+MW_TARGET_CELL);
 FUN_004b52f0(actor);
 building=restore_monument_worker_building(MW_H(record+MW_TO_BUILDING));
 MW_W(record+MW_TO_POSITION)=(uint16_t)(MW_B(building+86u)+MW_B(building+87u)*6u);
 MW_W(record+MW_ACTION_CLOCK)=0;
}
void FUN_004f2410(int32_t actor){restore_monument_worker_snap_to_target(actor);}
