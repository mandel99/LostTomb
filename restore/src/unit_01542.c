#include "restore_types.h"
#include "mission_loaders.h"

void FUN_004bd330(RestoreLoadSettings *settings)

{
  int32_t *in_ECX=settings->values;
  
  in_ECX[3] = 0xb4;
  in_ECX[4] = 0x46;
  in_ECX[1] = 0x46;
  *in_ECX = 0xb4;
  in_ECX[7] = 100;
  in_ECX[5] = 100;
  in_ECX[2] = 100;
  in_ECX[6] = 0;
  return;
}
