#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00ec7630 (*(undefined4 *)0x00ec7630u)

void FUN_005173a0(int param_1,short *param_2,int param_3,int param_4)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint *puVar5;
  int iVar6;
  
  if (param_1 < 0) {
    param_1 = 0;
  }
  if ((int)param_2 < 0) {
    param_2 = (short *)0x0;
  }
  if (DAT_00bca2d0 <= param_3) {
    param_3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  iVar3 = DAT_00bca2d0 - param_3;
  iVar4 = DAT_00bca2d8 + (int)param_2 * 0xe4 + param_1;
  if ((int)param_2 <= param_4) {
    param_4 = (param_4 - (int)param_2) + 1;
    do {
      if (param_1 <= param_3) {
        param_2 = (short *)(&DAT_00b4e6fc + iVar4 * 2);
        puVar5 = &DAT_00af598c + iVar4;
        iVar6 = (param_3 - param_1) + 1;
        do {
          uVar2 = *puVar5;
          if (((uVar2 & 0x80000) != 0) || ((uVar2 & 0x10004) == 0x10004)) goto LAB_00517610;
          if ((uVar2 & 0x8000) == 0) {
            if (((uVar2 & 0x40) == 0) && ((uVar2 & 0x40000000) == 0)) {
              if ((uVar2 & 0x20) == 0) {
                if ((uVar2 & 0x400) == 0) {
                  if ((uVar2 & 8) == 0) {
                    if ((uVar2 & 0x100) == 0) {
                      if ((uVar2 & 0x4000) == 0) {
                        if ((uVar2 & 0x2040011) == 0) {
                          if ((uVar2 & 0x1000) == 0) {
                            if ((uVar2 & 0xeefed77f) != 0) goto LAB_00517610;
                            (&DAT_00ec7630)[iVar4] = 0;
                          }
                          else {
                            (&DAT_00ec7630)[iVar4] = 0;
                          }
                        }
                        else {
                          (&DAT_00ec7630)[iVar4] = 0;
                        }
                      }
                      else {
                        (&DAT_00ec7630)[iVar4] = 3;
                      }
                    }
                    else {
                      (&DAT_00ec7630)[iVar4] = 0;
                    }
                  }
                  else {
                    sVar1 = (&DAT_007999e8)[*param_2 * 0x84];
                    if (sVar1 == 0x48) {
                      (&DAT_00ec7630)[iVar4] = 1;
                    }
                    else if (sVar1 == 0xca) {
                      (&DAT_00ec7630)[iVar4] = 4;
                    }
                    else if (((((((sVar1 == 99) || (sVar1 == 0x58)) || (sVar1 == 0x59)) ||
                               ((sVar1 == 0x5d || (sVar1 == 0xb7)))) ||
                              ((sVar1 == 0xd2 || ((sVar1 == 0xcf || (sVar1 == 0xd0)))))) ||
                             (sVar1 == 0xde)) ||
                            (((((sVar1 == 0xd5 || (sVar1 == 0xe1)) || (sVar1 == 0xe6)) ||
                              (((sVar1 == 0xe3 || (sVar1 == 0xe5)) ||
                               ((sVar1 == 0xea || ((sVar1 == 0xeb || (sVar1 == 0xec)))))))) ||
                             ((sVar1 == 0xe4 || (sVar1 == 0xd7)))))) {
LAB_00517610: ;
                      (&DAT_00ec7630)[iVar4] = 0xff;
                    }
                    else if (sVar1 == 0x39) {
                      (&DAT_00ec7630)[iVar4] = 5;
                    }
                    else {
                      if (sVar1 == 0x36) goto LAB_005175ae;
                      if (sVar1 == 0x88) goto LAB_00517610;
                      (&DAT_00ec7630)[iVar4] = 1;
                    }
                  }
                }
                else {
                  (&DAT_00ec7630)[iVar4] = 2;
                }
              }
              else {
                (&DAT_00ec7630)[iVar4] = 0;
              }
            }
            else {
LAB_005175ae: ;
              (&DAT_00ec7630)[iVar4] = 0;
            }
          }
          else {
            (&DAT_00ec7630)[iVar4] = 4;
          }
          iVar4 = iVar4 + 1;
          puVar5 = puVar5 + 1;
          param_2 = param_2 + 1;
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
      }
      iVar4 = iVar4 + DAT_00bca2dc + iVar3 + -1 + param_1;
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  return;
}
