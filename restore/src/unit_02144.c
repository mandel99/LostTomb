#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define PTR_DAT_005f6cd0 (*(unsigned char * *)0x005f6cd0u)
#define PTR_DAT_005f6d58 (*(unsigned char * *)0x005f6d58u)

uint FUN_004f3d70(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  byte bVar6;
  undefined **ppuVar7;
  
  iVar1 = (int)*(short *)(&DAT_00b4e6fc + param_1 * 2);
  if (iVar1 == 0) {
    return 0xffffffff;
  }
  iVar4 = iVar1 * 0x108;
  if ((&DAT_007999e8)[iVar1 * 0x84] != 0xb7) {
    return 0xffffffff;
  }
  switch(param_1 - *(int *)(&DAT_007999e4 + iVar4)) {
  case 0: ;
    bVar6 = 1;
    break;
  case 1: ;
    bVar6 = 2;
    break;
default: ;
    goto LAB_004f3e66;
  case 0xe4: ;
    bVar6 = 4;
    break;
  case 0xe5: ;
    bVar6 = 8;
  }
  uVar2 = (uint)*(byte *)((int)&DAT_00799a2c + iVar4 + 1);
  if (uVar2 == 3) {
    ppuVar7 = &PTR_DAT_005f6d58;
    if (DAT_005e4ef4 != 5) {
      ppuVar7 = &PTR_DAT_005f6cd0;
    }
    iVar3 = 0;
    if (0 < (int)ppuVar7[(uint)*(byte *)(&DAT_00799a2c + iVar1 * 0x84) * 2 + 1]) {
      piVar5 = (int *)ppuVar7[(uint)*(byte *)(&DAT_00799a2c + iVar1 * 0x84) * 2];
      do {
        if ((int)(short)(&DAT_007999ea)[iVar1 * 0x84] == *piVar5) {
          return (param_1 ==
                 *(int *)((int)ppuVar7[(uint)*(byte *)(&DAT_00799a2c + iVar1 * 0x84) * 2] +
                         (iVar3 * 2 + 1) * 4) + *(int *)(&DAT_007999e4 + iVar4)) + 2;
        }
        iVar3 = iVar3 + 1;
        piVar5 = piVar5 + 2;
      } while (iVar3 < (int)ppuVar7[(uint)*(byte *)(&DAT_00799a2c + iVar1 * 0x84) * 2 + 1]);
      return 2;
    }
  }
  else {
    if ((*(byte *)((int)&DAT_00799a30 + iVar4 + 3) & bVar6) != 0) {
      return uVar2;
    }
    if (uVar2 - 1 != 3) {
      return uVar2 - 1;
    }
  }
LAB_004f3e66: ;
  return 2;
}
