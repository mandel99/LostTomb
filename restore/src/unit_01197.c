#include "restore_types.h"
#define DAT_005c9afe (*(undefined4 *)0x005c9afeu)
#define DAT_005c9b16 (*(undefined4 *)0x005c9b16u)
#define DAT_005c9b22 (*(undefined4 *)0x005c9b22u)
#define DAT_005c9b2e (*(undefined4 *)0x005c9b2eu)
#define DAT_005c9b3a (*(undefined4 *)0x005c9b3au)
#define DAT_005c9b46 (*(undefined4 *)0x005c9b46u)
#define DAT_005c9b6a (*(undefined4 *)0x005c9b6au)
#define DAT_005c9b76 (*(undefined4 *)0x005c9b76u)
#define DAT_005c9b82 (*(undefined4 *)0x005c9b82u)
#define DAT_005c9b8e (*(undefined4 *)0x005c9b8eu)
#define DAT_005c9b9a (*(undefined4 *)0x005c9b9au)
#define DAT_0070bf8c (*(undefined4 *)0x0070bf8cu)
#define DAT_0070c1cc (*(undefined4 *)0x0070c1ccu)
#define DAT_0070c3dc (*(undefined4 *)0x0070c3dcu)
#define DAT_0070c5bc (*(undefined4 *)0x0070c5bcu)
#define DAT_0070c76c (*(undefined4 *)0x0070c76cu)
#define DAT_0070c8ec (*(undefined4 *)0x0070c8ecu)
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define s_restore_005c9b4a (*(char (*)[2])0x005c9b4au)
extern bool FUN_00481aa0();

void FUN_004818a0(int param_1,int param_2,int param_3,short *param_4,char param_5)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar3 = (int)param_4;
  iVar6 = param_1 - (int)param_4;
  iVar5 = param_1 + param_3 + -1 + (int)param_4;
  iVar1 = param_2 + param_3 + -1 + (int)param_4;
  iVar4 = param_2 - (int)param_4;
  switch((int)(param_3)) {
  case 1: ;
    iVar8 = (int)*(short *)(&DAT_005c9afe + (int)param_4 * 2);
    param_4 = &DAT_0070c8ec + ((undefined4 *)&s_restore_005c9b4a)[(int)param_4 + 4];
    break;
  case 2: ;
    iVar8 = (int)*(short *)(&DAT_005c9b16 + (int)param_4 * 2);
    param_4 = &DAT_0070c76c + *(short *)(&DAT_005c9b6a + (int)param_4 * 2);
    break;
  case 3: ;
    iVar8 = (int)*(short *)(&DAT_005c9b22 + (int)param_4 * 2);
    param_4 = &DAT_0070c5bc + *(short *)(&DAT_005c9b76 + (int)param_4 * 2);
    break;
  case 4: ;
    iVar8 = (int)*(short *)(&DAT_005c9b2e + (int)param_4 * 2);
    param_4 = &DAT_0070c3dc + *(short *)(&DAT_005c9b82 + (int)param_4 * 2);
    break;
  case 5: ;
    iVar8 = (int)*(short *)(&DAT_005c9b3a + (int)param_4 * 2);
    param_4 = &DAT_0070c1cc + *(short *)(&DAT_005c9b8e + (int)param_4 * 2);
    break;
  case 6: ;
    iVar8 = (int)*(short *)(&DAT_005c9b46 + (int)param_4 * 2);
    param_4 = &DAT_0070bf8c + *(short *)(&DAT_005c9b9a + (int)param_4 * 2);
    break;
default: ;
    goto switchD_0048190d_default;
  }
  iVar7 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00bc9768 = iVar7;
  if (DAT_00bca2d4 < iVar1 || (DAT_00bca2d0 < iVar5 || (iVar4 < -1 || iVar6 < -1))) {
    iVar5 = 0;
    if (0 < iVar8) {
      do {
        iVar4 = FUN_00481aa0(param_1,param_2,param_3,iVar3,iVar5);
        iVar1 = DAT_00bc9768;
        if (iVar4 == 0) {
          (&DAT_00ac2d4c)[*param_4 + DAT_00bc9768] =
               (&DAT_00ac2d4c)[*param_4 + DAT_00bc9768] + param_5;
          if ((char)(&DAT_00ac2d4c)[iVar1] < -100) {
            (&DAT_00ac2d4c)[iVar1] = 0x9c;
          }
          if ('d' < (char)(&DAT_00ac2d4c)[iVar1]) {
            (&DAT_00ac2d4c)[iVar1] = 100;
          }
        }
        iVar5 = iVar5 + 1;
        param_4 = param_4 + 1;
      } while (iVar5 < iVar8);
    }
  }
  else if (0 < iVar8) {
    do {
      sVar2 = *param_4;
      (&DAT_00ac2d4c)[iVar7 + sVar2] = (&DAT_00ac2d4c)[iVar7 + sVar2] + param_5;
      iVar5 = sVar2 + iVar7;
      if ((char)(&DAT_00ac2d4c)[iVar5] < -100) {
        (&DAT_00ac2d4c)[iVar5] = 0x9c;
      }
      if ('d' < (char)(&DAT_00ac2d4c)[iVar5]) {
        (&DAT_00ac2d4c)[iVar5] = 100;
      }
      param_4 = param_4 + 1;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    return;
  }
switchD_0048190d_default: ;
  return;
}
