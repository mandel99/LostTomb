#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00ea60d0 (*(undefined4 *)0x00ea60d0u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00f566b4 (*(undefined *)0x00f566b4u)
#define DAT_00f566bc (*(undefined *)0x00f566bcu)
#define DAT_00f57204 (*(undefined *)0x00f57204u)
extern undefined4 FUN_0045b490();

int FUN_0045b570(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short sVar6;
  
  if (param_4 < 1) {
    return 0;
  }
  if (((&DAT_007999e8)[param_2 * 0x84] == 0x48) || ((&DAT_007999e8)[param_2 * 0x84] == 0xc1)) {
    do {
      sVar6 = 0x191;
      iVar4 = 0;
      iVar5 = 8;
      iVar3 = param_2;
      do {
        iVar3 = (int)*(short *)(&DAT_00799a12 + iVar3 * 0x108);
        if ((((0 < iVar3) && ((short)(&DAT_007999ea)[iVar3 * 0x84] == param_3)) &&
            (sVar1 = (&DAT_00799a14)[iVar3 * 0x84], 0 < sVar1)) && (sVar1 < sVar6)) {
          iVar4 = iVar3;
          sVar6 = sVar1;
        }
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      if (iVar4 == 0) {
        return param_4;
      }
      bVar2 = false;
      iVar3 = (int)(short)(&DAT_00799a14)[iVar4 * 0x84];
      if (param_4 < iVar3) {
        iVar3 = param_1 * 0x1276 + param_3;
        (&DAT_00799a14)[iVar4 * 0x84] = (&DAT_00799a14)[iVar4 * 0x84] - (short)param_4;
        (&DAT_00ea60d0)[iVar3] = (&DAT_00ea60d0)[iVar3] + param_4;
        iVar5 = (&DAT_00ea6160)[iVar3] - param_4;
        param_4 = 0;
        (&DAT_00ea6160)[iVar3] = iVar5;
      }
      else {
        param_4 = param_4 - iVar3;
        iVar5 = param_1 * 0x1276 + param_3;
        (&DAT_00799a14)[iVar4 * 0x84] = 0;
        (&DAT_007999ea)[iVar4 * 0x84] = 0;
        (&DAT_00ea60d0)[iVar5] = (&DAT_00ea60d0)[iVar5] + iVar3;
        (&DAT_00ea6160)[iVar5] = (&DAT_00ea6160)[iVar5] - iVar3;
        if (0 < param_4) {
          bVar2 = true;
        }
      }
      iVar3 = *(int *)(&DAT_007999e4 + iVar4 * 0x108);
      sVar6 = (&DAT_00799a14)[iVar4 * 0x84];
      iVar4 = DAT_00f566b4;
      if (0 < sVar6) {
        if (param_3 < 0x1f) {
          iVar4 = DAT_00f566bc + -4;
          iVar5 = FUN_0045b490((int)sVar6);
          iVar4 = iVar4 + param_3 * 4 + iVar5;
        }
        else {
          iVar4 = DAT_00f57204 + -0x7c;
          iVar5 = FUN_0045b490((int)sVar6);
          iVar4 = iVar4 + param_3 * 4 + iVar5;
        }
      }
      (&DAT_00b7482c)[iVar3] = iVar4;
    } while (bVar2);
  }
  return param_4;
}
