#include "restore_types.h"
extern int FUN_0055c650();
extern void FUN_0055c990();
extern void FUN_0055ca20();
extern void FUN_0055cb10();
extern void FUN_0055d250();

void FUN_0055c2d0(uint *param_1)

{
  byte bVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  byte *pbVar9;
  byte *pbStack_10;
  int local_8;
  int local_4;
  
  local_4 = 0;
  bVar2 = false;
  *(char *)((int)param_1 + 0x1fca) = (char)param_1[5];
  pbVar9 = (byte *)(param_1[6] + 0x29d0 + (int)param_1);
  *(char *)((int)param_1 + 0x1fcb) = (char)param_1[3];
  param_1[1] = 2;
  FUN_0055ca20(param_1 + 0x7f3,0,0x800);
  param_1[2] = 0;
  do {
    iVar8 = 0x1000;
    iVar7 = 0;
    do {
      local_8 = iVar8;
      iVar3 = (*(code *)param_1[0x26d])
                        ((int)param_1 + param_1[6] + iVar7 + 0x29d0,&local_8,param_1[0x26c]);
      if (iVar3 == 0) {
        if ((iVar7 == 0) && (local_4 == 0)) {
          FUN_0055c990(param_1,*(undefined1 *)((int)param_1 + 0x3a1),(uint16_t)param_1[0x26b]);
          if (param_1[2] != 0) {
            param_1[1] = param_1[1] + 1;
          }
          (*(code *)param_1[0x26e])((int)param_1 + 0x1fca,param_1 + 1,param_1[0x26c]);
          return;
        }
        bVar2 = true;
        break;
      }
      iVar7 = iVar7 + iVar3;
      iVar8 = iVar8 - iVar3;
    } while (iVar8 != 0);
    uVar4 = param_1[6];
    pbStack_10 = (byte *)((int)param_1 + uVar4 + iVar7 + 0x27cc);
    if (bVar2) {
      pbStack_10 = pbStack_10 + 0x204;
    }
    if (local_4 == 0) {
      FUN_0055cb10(param_1,pbVar9,pbStack_10 + 1);
      local_4 = local_4 + 1;
      if (param_1[6] != 0x1000) goto LAB_0055c3e6;
    }
    else if (local_4 == 1) {
      FUN_0055cb10(param_1,pbVar9 + (0x204 - uVar4),pbStack_10 + 1);
LAB_0055c3e6: ;
      local_4 = local_4 + 1;
    }
    else {
      FUN_0055cb10(param_1,(int)pbVar9 - uVar4,pbStack_10 + 1);
    }
joined_r0x0055c3ee: ;
    if (pbVar9 < pbStack_10) {
      uVar4 = FUN_0055c650(param_1,pbVar9);
      while( true ) {
        if ((uVar4 == 0) || ((uVar4 == 2 && (0xff < *param_1)))) goto LAB_0055c49f;
        if ((bVar2) && (pbStack_10 < pbVar9 + uVar4)) break;
        if ((7 < uVar4) || (pbStack_10 <= pbVar9 + 1)) goto LAB_0055c56f;
        uVar6 = *param_1;
        uVar5 = FUN_0055c650(param_1,pbVar9 + 1);
        if ((uVar5 <= uVar4) || ((uVar5 <= uVar4 + 1 && (uVar6 < 0x81)))) {
          *param_1 = uVar6;
          goto LAB_0055c56f;
        }
        bVar1 = *pbVar9;
        pbVar9 = pbVar9 + 1;
        FUN_0055c990(param_1,*(undefined1 *)(bVar1 + 0x9c + (int)param_1),
                     *(undefined2 *)((int)param_1 + (uint)bVar1 * 2 + 0x3a2));
        uVar4 = uVar5;
      }
      uVar4 = (int)pbStack_10 - (int)pbVar9;
      if ((1 < uVar4) && ((uVar4 != 2 || (*param_1 < 0x100)))) {
LAB_0055c56f: ;
        FUN_0055c990(param_1,*(undefined1 *)((int)param_1 + uVar4 + 0x19a),
                     *(undefined2 *)((int)param_1 + uVar4 * 2 + 0x59e));
        if (uVar4 == 2) {
          uVar4 = *param_1 >> 2;
          FUN_0055c990(param_1,*(undefined1 *)((int)param_1 + uVar4 + 0x1c),
                       *(undefined1 *)((int)param_1 + uVar4 + 0x5c));
          pbVar9 = pbVar9 + 2;
          FUN_0055c990(param_1,2,*param_1 & 3);
        }
        else {
          uVar6 = *param_1 >> ((byte)param_1[3] & 0x1f);
          FUN_0055c990(param_1,*(undefined1 *)((int)param_1 + uVar6 + 0x1c),
                       *(undefined1 *)((int)param_1 + uVar6 + 0x5c));
          pbVar9 = pbVar9 + uVar4;
          FUN_0055c990(param_1,param_1[3],param_1[4] & *param_1);
        }
        goto joined_r0x0055c3ee;
      }
LAB_0055c49f: ;
      bVar1 = *pbVar9;
      pbVar9 = pbVar9 + 1;
      FUN_0055c990(param_1,*(undefined1 *)(bVar1 + 0x9c + (int)param_1),
                   *(undefined2 *)((int)param_1 + (uint)bVar1 * 2 + 0x3a2));
      goto joined_r0x0055c3ee;
    }
    if (bVar2) {
      FUN_0055c990(param_1,*(undefined1 *)((int)param_1 + 0x3a1),(uint16_t)param_1[0x26b]);
      if (param_1[2] != 0) {
        param_1[1] = param_1[1] + 1;
      }
      (*(code *)param_1[0x26e])((int)param_1 + 0x1fca,param_1 + 1,param_1[0x26c]);
      return;
    }
    pbVar9 = pbVar9 + -0x1000;
    FUN_0055d250(param_1 + 0x9f3,param_1 + 0xdf3,param_1[6] + 0x204);
  } while( true );
}
