#include "restore_types.h"
#define DAT_005cbfb8 (*(undefined4 *)0x005cbfb8u)
#define DAT_00784cb0 (*(undefined1 *)0x00784cb0u)
#define DAT_00a509bc (*(undefined4 *)0x00a509bcu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9748 (*(undefined *)0x00bc9748u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern int FUN_00484be0();
extern int FUN_00486d30();
extern int FUN_00486ec0();
extern undefined4 FUN_00486f20();

void FUN_00487450(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint *puVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int local_1c;
  int local_14;
  
  uVar3 = DAT_00bc9768;
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (param_2 < 0) {
    param_2 = 0;
  }
  if (DAT_00bca2d0 <= param_3) {
    param_3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  local_14 = 0x10;
  iVar1 = (DAT_00bca2dc - param_3) + DAT_00bca2d0 + -1 + param_1;
  if (DAT_00784cb0 == '\0') {
    local_14 = 0x2e;
  }
  else if (DAT_00784cb0 == '\x02') {
    local_14 = 8;
  }
  local_1c = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  iVar11 = param_2;
  do {
    if (param_4 < iVar11) {
      iVar11 = param_2 * 0xe4 + DAT_00bca2d8 + param_1;
      if (param_2 <= param_4) {
        param_4 = (param_4 - param_2) + 1;
        do {
          if (param_1 <= param_3) {
            puVar8 = &DAT_00af598c + iVar11;
            iVar9 = (param_3 - param_1) + 1;
            do {
              if (((*puVar8 & 0x80) != 0) &&
                 (iVar7 = FUN_00486f20(iVar11), uVar4 = DAT_00bca2e0, DAT_00bca2e0 = uVar4,
                 iVar7 != 0)) {
                *puVar8 = *puVar8 & 0xfffff7ff;
                DAT_00bca2e0 = 0;
                FUN_00484be0(&DAT_005cbfb8,0x1a,0xffffffff,0);
                if (DAT_00bc9748 == 0xe) {
                  (&DAT_00a509bc)[iVar11] = 100;
                  DAT_00bca2e0 = uVar4;
                }
                else {
                  (&DAT_00a509bc)[iVar11] = (byte)DAT_00bc9748 | 0x80;
                  DAT_00bca2e0 = uVar4;
                }
              }
              iVar11 = iVar11 + 1;
              puVar8 = puVar8 + 1;
              iVar9 = iVar9 + -1;
            } while (iVar9 != 0);
          }
          iVar11 = iVar11 + iVar1;
          param_4 = param_4 + -1;
        } while (param_4 != 0);
        DAT_00bc9768 = uVar3;
        return;
      }
      DAT_00bc9768 = uVar3;
      return;
    }
    if (param_1 <= param_3) {
      puVar8 = &DAT_00af598c + local_1c;
      iVar9 = param_1;
      do {
        uVar2 = *puVar8;
        if ((uVar2 & 0x50004) == 0) {
          iVar7 = FUN_00486d30(iVar9,iVar11,4,local_14);
          iVar5 = FUN_00486d30(iVar9,iVar11,0x50000,8);
          if (iVar7 < 0) {
            if (-1 < iVar5) {
              iVar7 = 0;
              goto LAB_00487578;
            }
LAB_004875fb: ;
            *puVar8 = *puVar8 & 0xffffff7f;
            (&DAT_00a509bc)[local_1c] = 0;
          }
          else {
            iVar6 = (iVar7 - (local_14 + -0xc)) + 1;
            iVar10 = local_14;
            if (iVar7 < local_14 + -0xc) {
              iVar7 = 0;
            }
            else if (-1 < iVar6) {
              iVar10 = 0xc;
              iVar7 = iVar6;
            }
            iVar7 = FUN_00486ec0(iVar7 - 1U & ((int)(iVar7 - 1U) < 1) - 1,iVar10);
LAB_00487578: ;
            if (iVar5 < 0) {
              iVar5 = 0;
            }
            else {
              iVar5 = FUN_00486ec0(iVar5 - 1U & ((int)(iVar5 - 1U) < 1) - 1,8);
            }
            if (iVar5 < iVar7) {
              iVar5 = iVar7;
            }
            if (iVar5 < 1) {
              iVar5 = 1;
            }
            else if (iVar5 < 1) goto LAB_004875fb;
            *puVar8 = *puVar8 | 0x80;
            (&DAT_00a509bc)[local_1c] = (char)iVar5;
          }
        }
        else {
          (&DAT_00a509bc)[local_1c] = 0;
          *puVar8 = uVar2 & 0xffffff7f;
        }
        iVar9 = iVar9 + 1;
        local_1c = local_1c + 1;
        puVar8 = puVar8 + 1;
      } while (iVar9 <= param_3);
    }
    iVar11 = iVar11 + 1;
    local_1c = local_1c + iVar1;
  } while( true );
}
