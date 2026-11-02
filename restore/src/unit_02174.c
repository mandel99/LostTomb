#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaae84 (*(char *)0x00eaae84u)
#define DAT_00eaae88 (*(undefined *)0x00eaae88u)
extern bool FUN_004ecea0();
extern void FUN_004ed360();

int FUN_004f5d90(int param_1)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  
  DAT_00eaae88 = param_1;
  iVar4 = 1;
  do {
    if (((&DAT_007999d8)[iVar4 * 0x108] != '\0') && ((&DAT_007999e8)[iVar4 * 0x84] == 0xb7)) {
      bVar1 = *(byte *)(&DAT_00799a2c + iVar4 * 0x84);
      cVar2 = FUN_004ecea0((uint)bVar1);
      if (cVar2 != '\0') {
        iVar3 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) + *(int *)(&DAT_007999e4 + iVar4 * 0x108)
                ) - DAT_00bca2d8;
        FUN_004ed360(iVar3 % 0xe4,iVar3 / 0xe4,6,0);
        if (DAT_00eaae84 != '\0') {
          return iVar4;
        }
      }
      if (DAT_005e4ef4 == 5) {
        iVar3 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar4 * 0x84) * 4);
      }
      else {
        iVar3 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar4 * 0x84) * 4);
      }
      iVar4 = iVar4 + -1 + iVar3;
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 4000);
  return param_1;
}
