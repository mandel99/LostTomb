#include "restore_types.h"
#define DAT_00e38d94 (*(undefined *)0x00e38d94u)
#define DAT_00e92e18 (*(undefined *)0x00e92e18u)

void FUN_0040fa70(int param_1,int param_2,ushort param_3)

{
  ushort *puVar1;
  
  puVar1 = (ushort *)(DAT_00e92e18 + param_1 * 2 + param_2 * DAT_00e38d94);
  *puVar1 = *puVar1 & param_3;
  return;
}
