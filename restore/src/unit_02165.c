#include "restore_types.h"
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaae90 (*(undefined *)0x00eaae90u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern bool FUN_004ecea0();
extern void FUN_004ed360();
extern void FUN_004ef450();

undefined4 FUN_004f53d0(int param_1)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  
  if ((1 < *(byte *)((int)&DAT_00799a2c + param_1 * 0x108 + 1)) ||
     ((FUN_004ef450(param_1), restore_unresolved_return(0x004ef450u)), DAT_00eaaf58 != 0)) {
    return 0;
  }
  bVar1 = *(byte *)(&DAT_00799a2c + param_1 * 0x84);
  cVar2 = FUN_004ecea0((uint)bVar1);
  if ((cVar2 != '\0') &&
     (iVar3 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) + *(int *)(&DAT_007999e4 + param_1 * 0x108)
              ) - DAT_00bca2d8, (FUN_004ed360(iVar3 % 0xe4,iVar3 / 0xe4,3,0), restore_unresolved_return(0x004ed360u)), DAT_00eaae90 != 0)) {
    return 0;
  }
  return 1;
}
