#include "restore_types.h"
#define DAT_00e95232 (*(undefined4 *)0x00e95232u)
extern int FUN_004e1600();

void FUN_004d32a0(void)

{
  short sVar1;
  short *psVar2;
  
  psVar2 = &DAT_00e95232;
  do {
    if (*psVar2 == 0) {
      return;
    }
    sVar1 = FUN_004e1600((int)*psVar2);
    *psVar2 = sVar1;
    psVar2 = psVar2 + 0x18;
  } while ((int)psVar2 < 0xea0db2);
  return;
}
