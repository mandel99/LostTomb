#include "restore_types.h"
#define DAT_005cc888 (*(undefined4 *)0x005cc888u)
#define DAT_005cc890 (*(undefined4 *)0x005cc890u)
#define DAT_005cc8b8 (*(undefined4 *)0x005cc8b8u)
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_0078d79c (*(undefined *)0x0078d79cu)
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_0078d7cc (*(undefined *)0x0078d7ccu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca34c (*(undefined *)0x00bca34cu)
#define DAT_00d386d4 (*(undefined *)0x00d386d4u)
#define DAT_00f5609c (*(undefined *)0x00f5609cu)
#define DAT_00f560a4 (*(undefined *)0x00f560a4u)
#define DAT_00f560ac (*(undefined *)0x00f560acu)
#define DAT_00f56544 (*(undefined *)0x00f56544u)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern void FUN_0047bf50();
extern int FUN_00489050();
extern int FUN_0053e590();
extern longlong FUN_0056133c();
#define _param_6 restore_param_6

void FUN_00488840(int param_1,int param_2,int param_3,int param_4,int param_5,unsigned int restore_param_6)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int *piVar8;
  int local_58;
  uint local_54;
  uint local_4c;
  int local_48 [18];
  
  iVar3 = DAT_00f56544;
  if (RESTORE_SLICE1(restore_param_6,0) == '\0') {
    iVar2 = FUN_00489050(param_2,param_3,3,3);
    FUN_0047bf50(param_1,param_2,param_3,3,3,iVar2 / 0xe + iVar3,8);
    return;
  }
  _param_6 = 0;
  iVar2 = param_1 * 0x108;
  iVar3 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar2 + 2) +
          *(short *)(&DAT_007999e0 + iVar2) * 0xe4;
  switch((int)(DAT_00bca2e0)) {
  case 0: ;
    _param_6 = *(int *)(&DAT_005cc890 + (uint)(byte)(&DAT_007999db)[iVar2] * 0x30);
    break;
default: ;
    goto switchD_004888e3_caseD_1;
  case 2: ;
    _param_6 = DAT_005cc8c0 + iVar3;
    goto switchD_004888e3_caseD_1;
  case 4: ;
    _param_6 = *(int *)(&DAT_005cc8b8 + (uint)(byte)(&DAT_007999db)[iVar2] * 8);
    break;
  case 6: ;
    _param_6 = *(int *)(&DAT_005cc888 + (uint)(byte)(&DAT_007999db)[iVar2] * 0x38);
  }
  _param_6 = _param_6 + iVar3;
switchD_004888e3_caseD_1: ;
  bVar1 = (&DAT_00adc36c)[_param_6];
  iVar3 = FUN_0056133c();
  local_58 = 0;
  local_54 = 0xffffffff;
  if ((bVar1 != 0) && (0 < (short)(&DAT_00799a18)[param_1 * 0x84])) {
    if (iVar3 * param_5 < DAT_00bca34c / 5) {
      local_58 = 1;
      local_54 = 4;
    }
    else if (DAT_00d386d4 == 1) {
      local_58 = 3;
      local_54 = (uint)(byte)(&DAT_00acf85c)[_param_6 + DAT_0078d7c4] % 7;
    }
    else {
      local_58 = 2;
      local_54 = FUN_0056133c();
    }
  }
  param_5 = param_5 / 10;
  local_48[3] = 0xffffffff;
  local_48[6] = 0xffffffff;
  local_48[7] = 0xffffffff;
  local_48[0xe] = 0xffffffff;
  local_4c = 0;
  local_48[1] = 0xfffffffe;
  local_48[2] = 1;
  local_48[4] = 2;
  local_48[5] = 0;
  local_48[8] = 0;
  local_48[9] = 0;
  local_48[10] = 1;
  local_48[0xb] = 1;
  local_48[0xc] = 0xfffffffe;
  local_48[0xd] = 0;
  local_48[0xf] = 1;
  local_48[0x10] = 0;
  local_48[0x11] = 2;
  piVar8 = local_48 + 1;
  do {
    iVar3 = piVar8[-1] * DAT_0070e45c + param_2;
    if (param_5 < 5) {
      iVar2 = param_4 + param_5;
    }
    else {
      iVar2 = param_4 + 5;
    }
    iVar4 = iVar2;
    if (DAT_0126517c <= iVar2) {
      iVar4 = 0;
    }
    iVar4 = *piVar8 * DAT_0070e44c + param_3 +
            (DAT_0070e448 * 2 - (int)(short)(&DAT_010d6a0e)[iVar4 * 0x20]);
    FUN_0053e590(iVar2,iVar3,iVar4);
    param_5 = param_5 + -2;
    if (param_5 < 0) {
      param_5 = 0;
    }
    if (((local_58 != 0) && (local_54 == local_4c)) && (DAT_0078d79c < 1)) {
      iVar2 = 0;
      if (local_58 == 1) {
        uVar7 = (DAT_0078d7cc >> 3) + param_1 & 7;
        iVar2 = DAT_00f560a4;
LAB_00488b7c: ;
        iVar2 = iVar2 + uVar7 + -8 + (uint)bVar1 * 8;
      }
      else {
        if (local_58 == 2) {
          uVar7 = (int)((byte)(&DAT_00acf85c)[_param_6] + local_4c) % 7;
          iVar2 = DAT_00f5609c;
          goto LAB_00488b7c;
        }
        if (local_58 == 3) {
          uVar7 = (uint)(byte)(&DAT_00acf85c)[_param_6] % 7;
          iVar2 = DAT_00f560ac;
          goto LAB_00488b7c;
        }
      }
      iVar5 = iVar2;
      if (DAT_0126517c <= iVar2) {
        iVar5 = 0;
      }
      iVar6 = iVar2;
      if (DAT_0126517c <= iVar2) {
        iVar6 = 0;
      }
      FUN_0053e590(iVar2,(DAT_0070e45c - (short)(&DAT_010d6a1a)[iVar5 * 0x20]) + iVar3,
                   (DAT_0070e44c * 3 - (int)(short)(&DAT_010d6a1c)[iVar6 * 0x20]) + -5 + iVar4);
    }
    piVar8 = piVar8 + 2;
    local_4c = local_4c + 1;
    if (8 < (int)local_4c) {
      return;
    }
  } while( true );
}
