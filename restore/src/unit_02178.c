#include "restore_types.h"
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaae90 (*(undefined *)0x00eaae90u)
extern bool FUN_004ecea0();
extern void FUN_004ed360();

bool FUN_004f6020(int param_1)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xb7) {
    param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
    bVar1 = *(byte *)(&DAT_00799a2c + param_1 * 0x84);
    if (*(short *)(&DAT_00799aa0 + param_1 * 0x42) < 1) {
      cVar2 = FUN_004ecea0((uint)bVar1);
      if (cVar2 != '\0') {
        iVar3 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) +
                *(int *)(&DAT_007999e4 + param_1 * 0x108)) - DAT_00bca2d8;
        FUN_004ed360(iVar3 % 0xe4,iVar3 / 0xe4,3,0);
        return DAT_00eaae90 == 0;
      }
      return true;
    }
  }
  return false;
}
