#include "restore_types.h"
#include "flood_simulation.h"

void FUN_004bd580(uintptr_t receiver)

{
  undefined4 *in_ECX=(undefined4 *)receiver;
  
  in_ECX[3] = *in_ECX;
  in_ECX[4] = in_ECX[1];
  return;
}
