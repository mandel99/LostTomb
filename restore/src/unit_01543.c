#include "restore_types.h"
#include "mission_loaders.h"

void FUN_004bd360(RestoreLoadSettings *settings)

{
  int32_t *in_ECX=settings->values;
  
  in_ECX[3] = *in_ECX;
  in_ECX[4] = in_ECX[1];
  in_ECX[5] = in_ECX[2];
  in_ECX[7] = in_ECX[2];
  in_ECX[6] = 0;
  return;
}
