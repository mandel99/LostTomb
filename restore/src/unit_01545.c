#include "restore_types.h"
#include "flood_simulation.h"

void FUN_004bd550(uintptr_t receiver)

{
  undefined4 uVar1;
  undefined4 *in_ECX=(undefined4 *)receiver;
  
  in_ECX[4] = in_ECX[1];
  in_ECX[3] = *in_ECX;
  in_ECX[7] = in_ECX[2];
  uVar1 = restore_flood_quality(receiver);
  in_ECX[8] = uVar1;
  FUN_004bd380(receiver);
  return;
}
