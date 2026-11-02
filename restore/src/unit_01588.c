#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_005df850 (*(undefined *)0x005df850u)
#define DAT_005df854 (*(undefined *)0x005df854u)
#define DAT_005df860 (*(undefined *)0x005df860u)
#define DAT_005df864 (*(undefined *)0x005df864u)
#define DAT_005df868 (*(undefined *)0x005df868u)
#define DAT_005df86c (*(undefined *)0x005df86cu)
#define DAT_005df870 (*(undefined *)0x005df870u)
#define DAT_005df874 (*(undefined *)0x005df874u)
#define DAT_005df880 (*(undefined *)0x005df880u)
#define DAT_005df884 (*(undefined *)0x005df884u)
#define DAT_005df888 (*(undefined *)0x005df888u)
#define DAT_005df88c (*(undefined *)0x005df88cu)
#define DAT_005df890 (*(undefined *)0x005df890u)
#define DAT_005df898 (*(undefined *)0x005df898u)
#define DAT_005df89c (*(undefined *)0x005df89cu)
#define DAT_005df8a0 (*(undefined *)0x005df8a0u)
#define DAT_005df8a8 (*(undefined *)0x005df8a8u)
#define DAT_005df8ac (*(undefined *)0x005df8acu)
#define DAT_005e00cc (*(undefined4 *)0x005e00ccu)
#define DAT_005e00d0 (*(undefined4 *)0x005e00d0u)
#define DAT_0070e120 (*(undefined *)0x0070e120u)
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e458 (*(undefined *)0x0070e458u)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9714 (*(undefined *)0x00bc9714u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
#define DAT_00f56d7c (*(undefined *)0x00f56d7cu)
#define DAT_00f56d84 (*(undefined *)0x00f56d84u)
#define DAT_00f56d8c (*(undefined *)0x00f56d8cu)
#define PTR_LAB_004c2e54 (*(unsigned char * *)0x004c2e54u)
#define PTR_LAB_004c2e70 (*(unsigned char * *)0x004c2e70u)
#define PTR_LAB_004c2e8c (*(unsigned char * *)0x004c2e8cu)
#define PTR_LAB_004c2ea8 (*(unsigned char * *)0x004c2ea8u)
#define PTR_LAB_004c2ee0 (*(unsigned char * *)0x004c2ee0u)
#define PTR_LAB_004c2efc (*(unsigned char * *)0x004c2efcu)
#define PTR_LAB_004c2f18 (*(unsigned char * *)0x004c2f18u)
#define PTR_LAB_004c2f34 (*(unsigned char * *)0x004c2f34u)
#define PTR_s_TEMPLE_NILE_005e018c (*(unsigned char * *)0x005e018cu)
extern undefined4 FUN_004c0ed0();
extern void FUN_004f0ba0();
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();
extern int FUN_0053f4f0();

void FUN_004c0f30(int param_1)

{
  SHORT SVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  undefined4 uVar9;
  undefined4 local_40;
  undefined4 local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  local_4 = 0;
  local_8 = 0;
  FUN_004f0ba0(DAT_00bca304,DAT_00bca308,&local_2c,&local_30);
  local_30 = local_30 + DAT_0070e44c;
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] == 0) {
    SVar1 = GetAsyncKeyState(0x52);
    if (SVar1 == -0x7fff) {
      iVar2 = DAT_00bca2e0 / 2;
      bVar8 = DAT_00bc9714 != iVar2;
      DAT_00bc9714 = iVar2 + 3;
      if (bVar8) {
        DAT_00bc9714 = iVar2;
      }
      if (3 < DAT_00bc9714) {
        DAT_00bc9714 = DAT_00bc9714 + -4;
      }
    }
    local_14 = DAT_00bc9714;
    if (DAT_00bca2e0 == 0) {
      if ((DAT_00bc9714 != 0) && (DAT_00bc9714 != 3)) {
        DAT_00bc9714 = 0;
        local_14 = DAT_00bc9714;
      }
    }
    else if (DAT_00bca2e0 == 2) {
      if ((DAT_00bc9714 != 0) && (DAT_00bc9714 != 1)) {
        DAT_00bc9714 = 1;
        local_14 = 1;
      }
    }
    else if (DAT_00bca2e0 == 4) {
      if ((DAT_00bc9714 != 2) && (DAT_00bc9714 != 1)) {
        DAT_00bc9714 = 2;
        local_14 = 2;
      }
    }
    else if ((DAT_00bc9714 != 2) && (DAT_00bc9714 != 3)) {
      DAT_00bc9714 = 3;
      local_14 = 3;
    }
  }
  else {
    local_14 = DAT_00bca2e0 / 2;
  }
  local_1c = 0;
  local_18 = 0;
  if (DAT_00bca2e0 == 2) {
    local_1c = -2;
  }
  else if (DAT_00bca2e0 == 4) {
    local_1c = -2;
    local_18 = -2;
  }
  else if (DAT_00bca2e0 == 6) {
    local_18 = -2;
  }
  local_24 = DAT_00bc9714 * 2;
  if (7 < local_24) {
    local_24 = local_24 + -8;
  }
  local_c = 7;
  iVar2 = (DAT_00bc9714 + (DAT_00bca2e0 / 2) * 4) * 8;
  local_38 = *(int *)(&DAT_005e00cc + iVar2) + DAT_00bca304;
  local_34 = *(int *)(&DAT_005e00d0 + iVar2) + DAT_00bca308;
  local_20 = local_38;
  local_28 = local_34;
  do {
    iVar5 = local_38 - local_34;
    iVar6 = local_34 - local_38;
    local_10 = 0xd;
    iVar2 = local_38;
    iVar7 = local_34;
    do {
      iVar3 = iVar7;
      iVar4 = local_20;
      if (DAT_00bca2e0 == 0) {
        if (local_24 == 0) {
LAB_004c1191: ;
          iVar3 = local_28;
          iVar4 = iVar2;
        }
        else if (local_24 != 2) {
          if (local_24 == 4) goto LAB_004c116d;
LAB_004c1186: ;
          iVar3 = iVar6 + iVar2;
          iVar4 = local_28 + iVar5;
        }
      }
      else if (DAT_00bca2e0 == 2) {
        if (local_24 == 0) {
LAB_004c113b: ;
          iVar3 = iVar6 + iVar2;
          iVar4 = local_28 + iVar5;
        }
        else {
          if (local_24 == 2) goto LAB_004c1191;
          if (local_24 != 4) goto LAB_004c116d;
        }
      }
      else if (DAT_00bca2e0 == 4) {
        if (local_24 == 0) goto LAB_004c116d;
        if (local_24 == 2) goto LAB_004c1186;
        if (local_24 == 4) goto LAB_004c1191;
      }
      else if (local_24 != 0) {
        if (local_24 != 2) {
          if (local_24 != 4) goto LAB_004c1191;
          goto LAB_004c113b;
        }
LAB_004c116d: ;
        iVar3 = iVar6 + local_20;
        iVar4 = iVar5 + iVar7;
      }
      if (((&DAT_00af598c)[DAT_00bca2d8 + iVar3 * 0xe4 + iVar4] & 0xeeffd77f) != 0) {
        local_8 = 1;
      }
      iVar7 = iVar7 + -1;
      iVar2 = iVar2 + 1;
      local_10 = local_10 + -1;
    } while (local_10 != 0);
    local_20 = local_20 + -1;
    local_28 = local_28 + 1;
    local_c = local_c + -1;
  } while (local_c != 0);
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) {
    local_4 = 1;
LAB_004c121e: ;
    local_c = 7;
    local_10 = 0;
    local_28 = local_38;
    local_24 = local_34;
    do {
      iVar5 = local_38 - local_34;
      param_1 = local_38;
      iVar7 = local_34 - local_38;
      local_8 = 0xd;
      iVar2 = local_34;
      do {
        iVar3 = iVar2;
        iVar6 = local_28;
        if (DAT_00bca2e0 == 0) {
          iVar4 = DAT_00bc9714 * 2;
          if (iVar4 == 0) {
LAB_004c130b: ;
            iVar3 = local_24;
            iVar6 = param_1;
          }
          else if (iVar4 != 2) {
            if (iVar4 == 4) goto LAB_004c129a;
            iVar6 = iVar5 + local_24;
            iVar3 = param_1 + iVar7;
          }
        }
        else if (DAT_00bca2e0 == 2) {
          iVar4 = DAT_00bc9714 * 2;
          if (iVar4 == 0) {
LAB_004c128a: ;
            iVar3 = param_1 + iVar7;
            iVar6 = iVar5 + local_24;
          }
          else {
            if (iVar4 == 2) goto LAB_004c130b;
            if (iVar4 != 4) {
              iVar6 = iVar5 + iVar2;
              iVar3 = iVar7 + local_28;
            }
          }
        }
        else {
          iVar4 = DAT_00bc9714 * 2;
          if (DAT_00bca2e0 == 4) {
            if (iVar4 == 0) goto LAB_004c129a;
            if (iVar4 == 2) {
              iVar6 = local_24 + iVar5;
              iVar3 = param_1 + iVar7;
            }
            else if (iVar4 == 4) goto LAB_004c130b;
          }
          else if (iVar4 != 0) {
            if (iVar4 != 2) {
              if (iVar4 != 4) goto LAB_004c130b;
              goto LAB_004c128a;
            }
LAB_004c129a: ;
            iVar6 = iVar5 + iVar2;
            iVar3 = local_28 + iVar7;
          }
        }
        iVar4 = DAT_00bca2d8 + iVar3 * 0xe4;
        FUN_004f0ba0(iVar6,iVar3,&local_3c,&local_40);
        if (((&DAT_00af598c)[iVar4 + iVar6] & 0xeeffd77f) == 0) {
          if (local_4 == 0) {
            uVar9 = 0x1fe3;
          }
          else {
            uVar9 = 0xfffff863;
          }
        }
        else {
          uVar9 = 0xfffff863;
        }
        FUN_0053f4f0(DAT_00f56be4,local_3c,local_40,uVar9);
        iVar2 = iVar2 + -1;
        param_1 = param_1 + 1;
        local_8 = local_8 + -1;
      } while (local_8 != 0);
      local_28 = local_28 + -1;
      local_24 = local_24 + 1;
      local_c = local_c + -1;
      if (local_c == 0) {
        return;
      }
    } while( true );
  }
  if (local_8 != 0) goto LAB_004c121e;
  uVar9 = 10;
  iVar2 = FUN_004c0ed0(param_1,10);
  FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_TEMPLE_NILE_005e018c)[iVar2],uVar9);
  iVar5 = local_14;
  iVar7 = local_2c;
  iVar2 = DAT_00f56d7c;
  local_38 = local_38 + local_1c;
  local_34 = local_34 + local_18;
  DAT_0070e120 = 0x1fe3;
  switch((int)(DAT_00bca2e0)) {
  case 0: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c142e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2e54)[DAT_00bc9714 * 2])();
      return;
    }
    break;
  case 2: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c16ff. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2e70)[DAT_00bc9714 * 2])();
      return;
    }
    break;
  case 4: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c19bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2e8c)[DAT_00bc9714 * 2])();
      return;
    }
    break;
  case 6: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c1c89. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2ea8)[DAT_00bc9714 * 2])();
      return;
    }
  }
  if (local_14 == 0) {
    iVar5 = local_30 - DAT_0070e44c;
    FUN_0053e590(DAT_00f56d7c,local_2c,iVar5);
    FUN_0053f250(iVar2,iVar7,iVar5);
    iVar2 = DAT_00f56d84;
    iVar5 = DAT_005df854 * DAT_0070e448 + local_30;
    iVar7 = DAT_005df850 * DAT_0070e458 + DAT_005df890 + DAT_0070e45c + local_2c;
    FUN_0053e590(DAT_00f56d84,iVar7,iVar5);
    FUN_0053f250(iVar2,iVar7,iVar5);
    local_2c = DAT_005df870 * DAT_0070e458 + DAT_005df8a0 + local_2c;
    iVar2 = DAT_00f56d8c;
    iVar7 = DAT_005df874;
LAB_004c2148: ;
    iVar7 = (iVar7 * DAT_0070e448 - DAT_0070e44c) + local_30;
    FUN_0053e590(iVar2,local_2c,iVar7);
  }
  else {
    if (local_14 == 2) {
      iVar5 = (DAT_005df884 * DAT_0070e448 - DAT_0070e44c) + local_30;
      iVar7 = DAT_005df880 * DAT_0070e458 + DAT_005df8a8 + local_2c;
      FUN_0053e590(DAT_00f56d7c,iVar7,iVar5);
      FUN_0053f250(iVar2,iVar7,iVar5);
      iVar2 = DAT_00f56d84;
      iVar7 = DAT_005df860 * DAT_0070e458 + DAT_005df898 + DAT_0070e45c + local_2c;
      iVar5 = DAT_005df864 * DAT_0070e448 + local_30;
      FUN_0053e590(DAT_00f56d84,iVar7,iVar5);
      FUN_0053f250(iVar2,iVar7,iVar5);
      iVar2 = DAT_00f56d8c;
    }
    else {
      if (local_14 == 3) {
        iVar5 = local_30 - DAT_0070e44c;
        iVar2 = DAT_00f56d7c + 3;
        FUN_0053e590(iVar2,local_2c,iVar5);
        FUN_0053f250(iVar2,iVar7,iVar5);
        iVar2 = DAT_00f56d84 + 3;
        iVar5 = DAT_005df86c * DAT_0070e448 + local_30;
        iVar7 = DAT_005df868 * DAT_0070e458 + DAT_005df89c + DAT_0070e45c + local_2c;
        FUN_0053e590(iVar2,iVar7,iVar5);
        FUN_0053f250(iVar2,iVar7,iVar5);
        iVar2 = DAT_00f56d8c + 3;
        local_2c = DAT_005df888 * DAT_0070e458 + DAT_005df8ac + local_2c;
        iVar7 = DAT_005df88c;
        goto LAB_004c2148;
      }
      iVar2 = DAT_00f56d7c + 3;
      iVar7 = (&DAT_005df870)[local_14 * 2] * DAT_0070e458 + (&DAT_005df8a0)[local_14] + local_2c;
      iVar6 = ((&DAT_005df874)[local_14 * 2] * DAT_0070e448 - DAT_0070e44c) + local_30;
      FUN_0053e590(iVar2,iVar7,iVar6);
      FUN_0053f250(iVar2,iVar7,iVar6);
      iVar2 = DAT_00f56d84 + 3;
      iVar7 = (&DAT_005df850)[iVar5 * 2] * DAT_0070e458 + (&DAT_005df890)[iVar5] + DAT_0070e45c +
              local_2c;
      iVar5 = (&DAT_005df854)[iVar5 * 2] * DAT_0070e448 + local_30;
      FUN_0053e590(iVar2,iVar7,iVar5);
      FUN_0053f250(iVar2,iVar7,iVar5);
      iVar2 = DAT_00f56d8c + 3;
    }
    iVar7 = local_30 - DAT_0070e44c;
    FUN_0053e590(iVar2,local_2c,iVar7);
  }
  FUN_0053f250(iVar2,local_2c,iVar7);
  switch((int)(DAT_00bca2e0)) {
  case 0: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c2282. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2ee0)[DAT_00bc9714 * 2])();
      return;
    }
    break;
  case 2: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c255a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2efc)[DAT_00bc9714 * 2])();
      return;
    }
    break;
  case 4: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c286b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2f18)[DAT_00bc9714 * 2])();
      return;
    }
    break;
  case 6: ;
    if ((uint)(DAT_00bc9714 * 2) < 7) {
                    /* WARNING: Could not recover jumptable at 0x004c2b34. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(&PTR_LAB_004c2f34)[DAT_00bc9714 * 2])();
      return;
    }
  }
  DAT_0070e120 = 0;
  return;
}
