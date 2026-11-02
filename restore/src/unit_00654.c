#include "restore_types.h"
#define DAT_005d2c48 (*(undefined4 *)0x005d2c48u)
#define DAT_0077592c (*(undefined4 *)0x0077592cu)
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea44dc (*(undefined4 *)0x00ea44dcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00444b80();
extern int FUN_004454d0();
extern int FUN_0045b2a0();
extern int FUN_004c5240();

undefined4 FUN_00445980(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (((&DAT_007999e8)[param_1 * 0x84] == 0x48) || ((&DAT_007999e8)[param_1 * 0x84] == 0xc1)) {
    iVar3 = FUN_004c5240(*(undefined4 *)
                          (&DAT_0077646c +
                          (param_2 + (uint)(byte)(&DAT_0077de04)[param_3 * 0x6a] * 0x24) * 4));
    iVar4 = (iVar3 - *(int *)(&DAT_0077592c +
                             (param_2 + (uint)(byte)(&DAT_0077de04)[param_3 * 0x6a] * 0x24) * 4)) *
            100;
    iVar3 = 400;
    if ((iVar4 < 400) && (iVar3 = iVar4, iVar4 < 0)) {
      iVar3 = 0;
    }
    iVar3 = FUN_004454d0(param_1,param_2,iVar3);
    sVar1 = *(short *)(&DAT_00bcd66c + param_4 * 0x184);
    if (sVar1 < 0) {
      iVar3 = sVar1 + 4;
    }
    if (iVar3 < 4) {
      *(short *)(&DAT_00bcd66c + param_4 * 0x184) = (sVar1 - (short)iVar3) + 4;
    }
    if (iVar3 != 0) {
      if (0 < iVar3) {
        do {
          FUN_0045b2a0(DAT_00eaac28,param_1,param_2,100);
          iVar2 = DAT_00eaac28;
          iVar4 = *(int *)(&DAT_005d2c48 + param_2 * 8);
          *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) =
               *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) - iVar4;
          (&DAT_00ea44dc)[iVar2 * 0x1276] = (&DAT_00ea44dc)[iVar2 * 0x1276] + iVar4;
          *(int *)(&DAT_0077592c +
                  (param_2 + (uint)(byte)(&DAT_0077de04)[param_3 * 0x6a] * 0x24) * 4) =
               *(int *)(&DAT_0077592c +
                       (param_2 + (uint)(byte)(&DAT_0077de04)[param_3 * 0x6a] * 0x24) * 4) + 1;
          FUN_00444b80(param_4,param_2);
          iVar3 = iVar3 + -1;
        } while (iVar3 != 0);
      }
      return 1;
    }
  }
  return 0;
}
