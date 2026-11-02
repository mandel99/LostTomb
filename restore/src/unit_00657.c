#include "restore_types.h"
#define DAT_005d2c4c (*(undefined4 *)0x005d2c4cu)
#define DAT_00775810 (*(undefined4 *)0x00775810u)
#define DAT_0077592c (*(undefined4 *)0x0077592cu)
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcd667 (*(undefined4 *)0x00bcd667u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea40de (*(undefined4 *)0x00ea40deu)
#define DAT_00ea44d4 (*(undefined4 *)0x00ea44d4u)
#define DAT_00ea5e3c (*(undefined4 *)0x00ea5e3cu)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00444bd0();
extern int FUN_0045b570();
extern int FUN_0045d510();
extern int FUN_004cde40();

undefined4 FUN_00445e50(int param_1,int param_2,int param_3,int param_4)

{
  byte *pbVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_4;
  
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x48) {
    iVar3 = FUN_0045d510(param_1,param_2);
    pbVar1 = &DAT_00bcd667 + param_4 * 0x184;
    if (((byte)(&DAT_00bcd667)[param_4 * 0x184] < 0xc) && (99 < iVar3)) {
      if (iVar3 < 400) {
        iVar3 = iVar3 / 100;
      }
      else {
        iVar3 = 4;
      }
      iVar4 = (DAT_00eaac28 * 0x24ec + param_2) * 2;
      if (*(short *)(&DAT_00ea4096 + iVar4) == 2) {
        if ((&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + param_2] + iVar3 * -100 <
            *(short *)(&DAT_00ea40de + iVar4) * 100) {
          iVar3 = ((&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + param_2] +
                  *(short *)(&DAT_00ea40de + iVar4) * -100) / 100;
        }
      }
      else if ((&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + param_2] + iVar3 * -100 <
               *(int *)(&DAT_00775810 + param_2 * 4)) {
        iVar3 = ((&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + param_2] -
                *(int *)(&DAT_00775810 + param_2 * 4)) / 100;
      }
      if (iVar3 != 0) {
        local_4 = 0;
        if (0 < iVar3) {
          do {
            iVar4 = DAT_00eaac28;
            *pbVar1 = *pbVar1 + 1;
            FUN_0045b570(iVar4,param_1,param_2,100);
            iVar4 = DAT_00eaac28;
            if ((int)(&DAT_00ea5e3c)[DAT_00eaac28 * 0x1276] < 1) {
              iVar5 = *(int *)(&DAT_005d2c4c + param_2 * 8);
              *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) =
                   *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) + iVar5;
              (&DAT_00ea44d4)[iVar4 * 0x1276] = (&DAT_00ea44d4)[iVar4 * 0x1276] + iVar5;
            }
            else {
              iVar6 = DAT_00eaac28 * 0x49d8;
              iVar5 = FUN_004cde40(*(undefined4 *)(&DAT_005d2c4c + param_2 * 8),0x96);
              iVar4 = DAT_00eaac28;
              uVar2 = *(undefined4 *)(&DAT_005d2c4c + param_2 * 8);
              *(int *)(&DAT_00ea187c + iVar6) = *(int *)(&DAT_00ea187c + iVar6) + iVar5;
              iVar5 = FUN_004cde40(uVar2,0x96);
              (&DAT_00ea44d4)[iVar4 * 0x1276] = (&DAT_00ea44d4)[iVar4 * 0x1276] + iVar5;
            }
            if (0xb < *pbVar1) {
              return 1;
            }
            iVar4 = param_2 + (uint)(byte)(&DAT_0077de04)[param_3 * 0x6a] * 0x24;
            *(int *)(&DAT_0077592c + iVar4 * 4) = *(int *)(&DAT_0077592c + iVar4 * 4) + 1;
            FUN_00444bd0(param_4,param_2);
            local_4 = local_4 + 1;
          } while (local_4 < iVar3);
        }
        return 1;
      }
    }
  }
  return 0;
}
