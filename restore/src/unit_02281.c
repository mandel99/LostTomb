#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
extern void FUN_004264d0();
extern void FUN_004cb6a0();
extern void FUN_0051f780();

void FUN_00516990(void)

{
  if (DAT_00e38e74 == '\0') {
    FUN_004264d0(0x5e,0x0051f780u,1);
    return;
  }
  FUN_0051f780(1);
  FUN_004cb6a0();
  DAT_00e38dca = 1;
  return;
}
