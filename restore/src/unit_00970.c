#include "restore_types.h"
#define DAT_0078d5c0 (*(undefined *)0x0078d5c0u)
#define DAT_0078d5c4 (*(undefined *)0x0078d5c4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a56 (*(undefined4 *)0x00799a56u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)

void FUN_0046b740(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  
  iVar3 = param_1 * 0x108;
  sVar1 = *(short *)((int)&DAT_007999dc + iVar3 + 2);
  (&DAT_00799a56)[iVar3] = 0;
  if ((*(int *)(&DAT_007999e4 + iVar3) !=
       DAT_00bca2d8 + sVar1 + *(short *)(&DAT_007999e0 + iVar3) * 0xe4) ||
     (*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_007999e4 + iVar3) * 2) != param_1)) {
    DAT_0078d5c4 = DAT_0078d5c4 + 1;
    iVar5 = 0;
    iVar4 = DAT_00bca2d8;
    if (0 < DAT_00bca2d4) {
      do {
        iVar2 = 0;
        if (0 < DAT_00bca2d0) {
          psVar6 = (short *)(&DAT_00b4e6fc + iVar4 * 2);
          do {
            if (*psVar6 == param_1) {
              iVar5 = iVar4 - DAT_00bca2d8;
              *(int *)(&DAT_007999e4 + iVar3) = iVar4;
              *(short *)((int)&DAT_007999dc + iVar3 + 2) = (short)(iVar5 % 0xe4);
              *(short *)(&DAT_007999e0 + iVar3) =
                   ((short)(iVar5 / 0xe4) + (short)(iVar5 >> 0x1f)) -
                   (short)((longlong)iVar5 * 0x8fb823ef >> 0x3f);
              return;
            }
            iVar2 = iVar2 + 1;
            iVar4 = iVar4 + 1;
            psVar6 = psVar6 + 1;
          } while (iVar2 < DAT_00bca2d0);
        }
        iVar5 = iVar5 + 1;
        iVar4 = iVar4 + DAT_00bca2dc;
      } while (iVar5 < DAT_00bca2d4);
    }
    (&DAT_007999d8)[iVar3] = 4;
    DAT_0078d5c0 = DAT_0078d5c0 + 1;
  }
  return;
}
