#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define DAT_00ec7630 (*(undefined4 *)0x00ec7630u)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
#define DAT_00f535bc (*(undefined *)0x00f535bcu)
#define DAT_00f56b8c (*(undefined *)0x00f56b8cu)
#define DAT_00f56c34 (*(undefined *)0x00f56c34u)
extern void FUN_0043f060();
extern void FUN_004a36b0();

void FUN_00516f90(int param_1,short *param_2,int param_3,int param_4,char param_5)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  int local_10;
  
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
  iVar5 = DAT_00bca2d8 + (int)param_2 * 0xe4 + param_1;
  if ((int)param_2 <= param_4) {
    local_10 = (param_4 - (int)param_2) + 1;
    do {
      if (param_1 <= param_3) {
        param_2 = (short *)(&DAT_00b4e6fc + iVar5 * 2);
        param_4 = (param_3 - param_1) + 1;
        do {
          if (((param_5 != '\0') && (((&DAT_00af598c)[iVar5] & 0x10044) == 0x44)) &&
             ((&DAT_00adc36c)[iVar5] == '\0')) {
            (&DAT_00af598c)[iVar5] = (&DAT_00af598c)[iVar5] & 0xffffffbf;
          }
          iVar3 = DAT_00f56b8c;
          uVar2 = (&DAT_00af598c)[iVar5];
          if (((uVar2 & 0x80000) != 0) || ((uVar2 & 0x10004) == 0x10004)) goto LAB_005172ad;
          if (((uVar2 & 0x40) == 0) && ((uVar2 & 0x40000000) == 0)) {
            if ((uVar2 & 0x400) == 0) {
              if ((uVar2 & 0x10000000) == 0) {
                if ((uVar2 & 0x8008) == 0) {
                  if ((uVar2 & 0x100) == 0) {
                    if ((*(int *)(&DAT_00d05720 + iVar5 * 4) == 0) ||
                       (*(char *)(*(int *)(&DAT_00d05720 + iVar5 * 4) + 0x18) != '\0')) {
                      if ((uVar2 & 0x10000) == 0) {
                        if ((uVar2 & 0x2040011) == 0) {
                          if ((uVar2 & 0x1000) == 0) {
                            if ((uVar2 & 0xeeffd77f) != 0) goto LAB_005172ad;
                            *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 4;
                          }
                          else {
                            *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 2;
                          }
                        }
                        else {
                          *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 4;
                        }
                      }
                      else {
                        *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 4;
                      }
                    }
                    else {
                      iVar3 = (&DAT_00b7482c)[iVar5] - DAT_00f56c34;
                      if (iVar3 < 0x30) {
                        *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 4;
                      }
                      else if ((((iVar3 == 0x34) || (iVar3 == 0x30)) || (iVar3 == 0x36)) ||
                              (iVar3 == 0x32)) {
                        *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 0xfb;
                      }
                      else {
LAB_005172ad: ;
                        *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 0xff;
                      }
                    }
                  }
                  else {
                    *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 4;
                  }
                }
                else {
                  iVar4 = (int)*param_2;
                  if (iVar4 == 0) {
                    (&DAT_00af598c)[iVar5] = uVar2 & 0xfffffff7;
                    (&DAT_00ec7630)[iVar5] = 4;
                    (&DAT_00b7482c)[iVar5] = ((byte)(&DAT_00acf85c)[iVar5] & 7) + iVar3;
                    (&DAT_00b67d1c)[iVar5] = 0x40;
                    (&DAT_00b285cc)[iVar5] = (&DAT_00b285cc)[iVar5] & 0xf0;
                    DAT_00f535bc = DAT_00f535bc + 1;
                  }
                  else {
                    sVar1 = (&DAT_007999e8)[iVar4 * 0x84];
                    if (sVar1 == 0x8a) {
                      *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 0;
                    }
                    else if (sVar1 == 0xd7) {
                      *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 0;
                    }
                    else {
                      if (sVar1 == 0x88) goto LAB_005171ab;
                      if (sVar1 == 0x36) {
                        *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 6;
                      }
                      else if (sVar1 == 0xd1) {
                        *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 4;
                      }
                      else {
                        if (sVar1 != 0x5a) goto LAB_005172ad;
                        if (((uint)*(byte *)(&DAT_00799a82 + iVar4 * 0x84) - DAT_00bca2e0 / 2 & 1)
                            == 0) {
                          *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 0xfa;
                        }
                        else {
                          *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 0xfc;
                        }
                      }
                    }
                  }
                }
              }
              else {
                *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 5;
              }
            }
            else {
              *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 2;
            }
          }
          else {
LAB_005171ab: ;
            *(undefined1 *)((int)&DAT_00ee0c50 + iVar5) = 0;
          }
          iVar5 = iVar5 + 1;
          param_2 = param_2 + 1;
          param_4 = param_4 + -1;
        } while (param_4 != 0);
      }
      iVar5 = iVar5 + (param_1 - param_3) + 0xe3;
      local_10 = local_10 + -1;
    } while (local_10 != 0);
  }
  if (param_5 != '\0') {
    iVar5 = 0;
    pcVar6 = &DAT_007999d8;
    do {
      if ((*(short *)(pcVar6 + 0x10) == 0x88) && ((*pcVar6 == '\x01' || (*pcVar6 == '\x03')))) {
        if ((*(short *)(pcVar6 + 0x40) == 0) ||
           ((short)(&DAT_00799a18)[*(int *)(pcVar6 + 0xc4) * 0x84] < 1)) {
          FUN_0043f060(iVar5,1,3);
          FUN_004a36b0(iVar5);
        }
        else {
          FUN_0043f060(iVar5,1,1);
        }
      }
      pcVar6 = pcVar6 + 0x108;
      iVar5 = iVar5 + 1;
    } while ((int)pcVar6 < 0x89b6d8);
  }
  return;
}
