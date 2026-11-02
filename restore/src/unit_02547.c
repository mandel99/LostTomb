#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00601de0 (*(undefined *)0x00601de0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
extern void FUN_0047bf50();
extern uint FUN_005414b0();

void FUN_00541920(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  undefined4 uVar5;
  int local_10;
  int *local_c;
  uint local_8;
  int local_4;
  
  local_4 = FUN_005414b0(param_1,&local_8);
  iVar2 = param_1 * 0x108;
  iVar1 = *(int *)(&DAT_00601de0 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34);
  if ((local_8 & 1) == 0) {
    if ((&DAT_007999ea)[param_1 * 0x84] == 0) {
      uVar5 = 8;
    }
    else {
      uVar5 = 0x10000008;
    }
    FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar2 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar2),iVar1,iVar1,local_4,uVar5);
  }
  else {
    local_10 = 0;
    DAT_00bc9768 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar2 + 2) +
                   *(short *)(&DAT_007999e0 + iVar2) * 0xe4;
    if (0 < iVar1) {
      local_c = &DAT_005cc8c0;
      do {
        iVar2 = 0;
        piVar4 = local_c;
        do {
          iVar3 = *piVar4 + DAT_00bc9768;
          (&DAT_00af598c)[iVar3] = (&DAT_00af598c)[iVar3] | 0x10000000;
          *(undefined2 *)(&DAT_00b4e6fc + iVar3 * 2) = (undefined2)param_1;
          (&DAT_00b285cc)[iVar3] = (&DAT_00b285cc)[iVar3] & 0xe0;
          (&DAT_00b67d1c)[iVar3] = (&DAT_00b67d1c)[iVar3] | 0x40;
          if ((((local_8 & 2) == 0) ||
              (((iVar2 == 0 || (iVar2 == iVar1 + -1)) &&
               ((local_c == &DAT_005cc8c0 || (local_10 == iVar1 + -1)))))) && (local_4 != 0)) {
            if (local_4 == DAT_00f56b54) {
              (&DAT_00b7482c)[iVar3] = ((byte)(&DAT_00acf85c)[iVar3] & 7) + local_4;
            }
            else {
              (&DAT_00b7482c)[iVar3] = local_4;
            }
          }
          iVar2 = iVar2 + 1;
          piVar4 = piVar4 + 2;
        } while (iVar2 < iVar1);
        local_10 = local_10 + 1;
        local_c = local_c + 0xc;
      } while (local_10 < iVar1);
      return;
    }
  }
  return;
}
