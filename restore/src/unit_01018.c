#include "restore_types.h"
#define DAT_005d06dc (*(undefined4 *)0x005d06dcu)
#define DAT_00605994 (*(undefined *)0x00605994u)
#define DAT_006059b4 (*(undefined *)0x006059b4u)
#define DAT_006059b8 (*(undefined *)0x006059b8u)
#define DAT_006059bc (*(undefined *)0x006059bcu)
#define DAT_006059c0 (*(undefined *)0x006059c0u)
#define DAT_006059c4 (*(undefined *)0x006059c4u)
#define DAT_006059c8 (*(undefined *)0x006059c8u)
#define DAT_006059cc (*(undefined *)0x006059ccu)
#define DAT_00605a48 (*(undefined *)0x00605a48u)
#define DAT_00705c6c (*(undefined *)0x00705c6cu)
#define DAT_00705ea4 (*(undefined4 *)0x00705ea4u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00804000 (*(undefined4 *)0x00804000u)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc96d0 (*(undefined *)0x00bc96d0u)
#define DAT_00bc96f8 (*(undefined *)0x00bc96f8u)
#define DAT_00bc9714 (*(undefined *)0x00bc9714u)
#define DAT_00bc9718 (*(undefined *)0x00bc9718u)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bc9724 (*(undefined *)0x00bc9724u)
#define DAT_00bc9800 (*(undefined *)0x00bc9800u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca310 (*(undefined *)0x00bca310u)
#define DAT_00bca315 (*(char *)0x00bca315u)
#define DAT_00bcb9b0 (*(undefined *)0x00bcb9b0u)
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5cf6 (*(undefined4 *)0x00ea5cf6u)
#define DAT_00ea5e70 (*(undefined4 *)0x00ea5e70u)
#define DAT_00ea620c (*(undefined4 *)0x00ea620cu)
#define DAT_00ea6240 (*(undefined4 *)0x00ea6240u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00bca328 (*(undefined4 *)0x00bca328u)
extern void FUN_0040f920();
extern undefined4 FUN_00440700();
extern undefined4 FUN_00440850();
extern undefined4 FUN_00440b10();
extern undefined4 FUN_00440ed0();
extern int FUN_0046fde0();
extern void FUN_00474130();
extern void FUN_004743f0();
extern void FUN_00474f50();
extern void FUN_00475090();
extern void FUN_00475220();
extern void FUN_004754a0();
extern void FUN_004759e0();
extern void FUN_0047bae0();
extern int FUN_0047cc70();
extern int FUN_0047d060();
extern void FUN_0047d360();
extern undefined4 FUN_0047d530();
extern undefined4 FUN_0047eda0();
extern undefined4 FUN_00482650();
extern undefined4 FUN_00482720();
extern undefined4 FUN_004828e0();
extern bool FUN_004829b0();
extern void FUN_00486020();
extern undefined4 FUN_00486a30();
extern undefined4 FUN_00486cf0();
extern void FUN_004bfa50();

void FUN_0046fef0(undefined4 param_1,undefined4 param_2,int param_3,int param_4,int param_5)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  iVar4 = param_5;
  DAT_00bc9724 = 0;
  DAT_00bc9718 = 0;
  if (param_5 == 0) {
    DAT_00bc9718 = 0;
    DAT_00bc9724 = 0;
    return;
  }
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < -4999) {
    DAT_00bc9718 = 0;
    DAT_00bc9724 = 0;
    return;
  }
  FUN_0040f920(&DAT_00b285cc,0xcb10,0xaf);
  iVar2 = param_4;
  DAT_00bc9724 = (&DAT_00705ea4)[iVar4 * 10];
  DAT_00bc96d0 = *(undefined4 *)(&DAT_005d06dc + iVar4 * 0x14);
  uVar5 = DAT_00bc96d0;
  switch((int)(iVar4)) {
  case 5: ;
    FUN_00474f50(1,param_1,param_2,param_3,param_4);
    break;
  case 6: ;
    FUN_00475220(1,param_1,param_2,param_3,param_4,0x4000);
    break;
  case 7: ;
    FUN_0047eda0(1,param_1,param_2,param_3,param_4);
    _DAT_00bca328 = 1;
    FUN_0047bae0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0);
    DAT_00605994 = 0;
    goto switchD_0046ff7e_caseD_d5;
  case 8: ;
    FUN_00475090(1,param_1,param_2,param_3,param_4);
    break;
  case 9: ;
    FUN_004743f0(1,param_1,param_2,param_3,param_4);
    break;
  case 10: ;
    FUN_00474130(1,param_1,param_2,param_3,param_4);
    break;
default: ;
    cVar1 = FUN_0046fde0(iVar4);
    iVar2 = param_4;
    if ((cVar1 == '\0') &&
       (iVar3 = FUN_004828e0(param_3,param_4,DAT_00bc96d0,0,0x10000), iVar3 != 0)) {
      return;
    }
    if (DAT_006059cc != 0) {
      uVar7 = 0x10800;
      uVar6 = 1;
      uVar5 = 3;
      goto LAB_0047065d;
    }
    if (DAT_00605a48 != 0) {
      iVar4 = FUN_00482720(param_3,iVar2,2,1,DAT_00605a48);
      goto joined_r0x0047068e;
    }
    if (DAT_006059c8 == 0) {
      if (DAT_006059c4 != 0) {
        uVar7 = 4;
        uVar6 = 3;
        uVar5 = 2;
        goto LAB_0047065d;
      }
      if (DAT_006059bc == 0) {
        if (DAT_006059b8 == 0) {
          if (DAT_006059b4 != 0) {
            iVar4 = FUN_004829b0(param_3,iVar2,DAT_00bc96d0,0,0x80,DAT_006059b4);
            goto joined_r0x0047068e;
          }
          if (DAT_006059c0 != 0) {
            uVar7 = 0x40000;
            uVar6 = 1;
            uVar5 = 2;
            goto LAB_0047065d;
          }
          if (iVar4 == 0xb7) goto switchD_0046ff7e_caseD_d5;
          uVar6 = 0xffffffff;
          uVar5 = DAT_00bc96d0;
        }
        else {
          iVar4 = FUN_004828e0(param_3,iVar2,1,0,0x40);
          if (iVar4 == 0) {
            return;
          }
          cVar1 = '\0';
          uVar6 = 0xffffffbf;
          uVar5 = DAT_00bc96d0;
        }
        goto LAB_00470753;
      }
      iVar4 = FUN_004828e0(param_3,iVar2,2,0,0x4000);
    }
    else {
      uVar7 = 1;
      uVar6 = 1;
      uVar5 = 2;
LAB_0047065d: ;
      iVar4 = FUN_00482650(param_3,iVar2,uVar5,uVar6,uVar7);
    }
joined_r0x0047068e: ;
    if (iVar4 == 0) {
      return;
    }
    goto switchD_0046ff7e_caseD_d5;
  case 0x1e: ;
    FUN_00440850(1,param_1,param_2,param_3,param_4);
    break;
  case 0x1f: ;
    FUN_00440700(1,param_1,param_2,param_3,param_4);
    break;
  case 0x21: ;
    FUN_00440b10(1,param_1,param_2,param_3,param_4);
    break;
  case 0x26: ;
    FUN_004754a0(1,param_1,param_2,param_3,param_4);
    break;
  case 0x27: ;
    FUN_004759e0(1,param_1,param_2,param_3,param_4);
    break;
  case 0x28: ;
  case 0x2c: ;
  case 0x2d: ;
    if (5 < DAT_00bcb9b4) {
      return;
    }
    FUN_0047d530(3,param_3,param_4,0xffffffff,1);
    goto switchD_0046ff7e_caseD_d5;
  case 0x32: ;
    if ((&DAT_00ea5e70)[DAT_00eaac28 * 0x1276] != 0) goto switchD_0046ff7e_caseD_d5;
    goto LAB_004705fd;
  case 0x38: ;
    cVar1 = '\0';
    uVar6 = 0xffffffbf;
    uVar5 = 3;
    goto LAB_00470753;
  case 0x3a: ;
  case 0xc9: ;
    cVar1 = '\0';
    uVar6 = 0xffffffbf;
    uVar5 = 2;
    goto LAB_00470753;
  case 0x41: ;
  case 0x42: ;
  case 0x43: ;
  case 0x44: ;
  case 0x45: ;
  case 0x9c: ;
  case 0x9d: ;
  case 0x9e: ;
  case 0x9f: ;
  case 0xa0: ;
    FUN_0047d530(3,param_3,param_4,0xffffffff,1);
    goto switchD_0046ff7e_caseD_d5;
  case 0x48: ;
  case 0xc1: ;
    cVar1 = '\0';
    uVar6 = 0xffffffff;
    uVar5 = 3;
    goto LAB_00470753;
  case 0x4a: ;
    goto switchD_0046ff7e_caseD_4a;
  case 0x4b: ;
    iVar4 = FUN_0047d060(param_3,param_4,1);
    goto LAB_00470502;
  case 0x4c: ;
  case 0xb5: ;
    goto LAB_004704fa;
  case 0x4d: ;
  case 0x4e: ;
  case 0x4f: ;
    if (*(int *)(&DAT_00ea620c + DAT_00eaac28 * 0x49d8) != 0) goto switchD_0046ff7e_caseD_d5;
    if ((DAT_006059b4 != 0) &&
       (iVar4 = FUN_004829b0(param_3,param_4,DAT_00bc96d0,0,0x80,DAT_006059b4), iVar4 == 0)) {
      return;
    }
    cVar1 = '\0';
    uVar6 = 0xffffffff;
    uVar5 = DAT_00bc96d0;
    goto LAB_00470753;
  case 0x52: ;
  case 0x53: ;
    if (1 < DAT_00bc96f8) {
      DAT_00bc9724 = DAT_00bc96f8 * DAT_00bc9724;
    }
    goto switchD_0046ff7e_caseD_d5;
  case 0x54: ;
  case 0x55: ;
  case 0x56: ;
  case 0x57: ;
  case 0xbb: ;
  case 0xbc: ;
  case 0xbd: ;
    if ((&DAT_00ea5cf6)[DAT_00eaac28 * 0x24ec] != 0) goto switchD_0046ff7e_caseD_d5;
    cVar1 = '\0';
    uVar6 = 0xffffffff;
    goto LAB_00470753;
  case 0x5a: ;
LAB_004704fa: ;
    iVar4 = FUN_0047cc70(param_3,param_4,1,0);
    goto LAB_00470502;
  case 0x5e: ;
    if (*(int *)(&DAT_00ea6240 + DAT_00eaac28 * 0x49d8) != 0) goto switchD_0046ff7e_caseD_d5;
    cVar1 = '\0';
    uVar6 = 0xffffffff;
    goto LAB_00470753;
  case 0x5f: ;
    if (0 < DAT_00705c6c) goto switchD_0046ff7e_caseD_d5;
LAB_004705fd: ;
    cVar1 = '\0';
    uVar6 = 0xffffffff;
    goto LAB_00470753;
  case 0x88: ;
    iVar4 = FUN_0047cc70(param_3,param_4,1,0);
    if (iVar4 == 0) {
      DAT_00605994 = 1;
    }
    iVar3 = iVar2 * 0xe4 + DAT_00bca2d8 + param_3;
    iVar4 = FUN_00486cf0(iVar3);
    if (iVar4 != 0) {
      if (DAT_00bca310 == 0) {
        DAT_00bca315 = 0;
        if ((int)(&DAT_00799a9c)[*(short *)(&DAT_00b4e6fc + iVar3 * 2) * 0x42] < 0) {
          DAT_00bca310 = 2;
          DAT_00bca315 = 1;
          DAT_00bc9800 = (int)*(short *)(&DAT_00b4e6fc + (iVar2 * 0xe4 + DAT_00bca2d8 + param_3) * 2
                                        );
        }
      }
      else if (DAT_00bca310 == 1) {
        DAT_00bca315 = 0;
        if ((int)(&DAT_00799a9c)[*(short *)(&DAT_00b4e6fc + iVar3 * 2) * 0x42] < 0) {
          iVar4 = DAT_00bc9800 * 0x108;
          param_4 = 0;
          iVar2 = FUN_00486a30(param_3,iVar2,(int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                               (int)*(short *)(&DAT_007999e0 + iVar4),(&DAT_007999db)[iVar4],
                               (uint)*(byte *)(&DAT_00799a82 +
                                              *(short *)(&DAT_00b4e6fc + iVar3 * 2) * 0x84) << 1,
                               *(char *)(&DAT_00799a82 + DAT_00bc9800 * 0x84) * '\x02' + 4U & 7,0,
                               &param_4);
          iVar4 = DAT_00bc9800;
          if (iVar2 != 0) {
            iVar2 = (int)*(short *)(&DAT_00b4e6fc + iVar3 * 2);
            DAT_00bca310 = 3;
            DAT_00bca315 = 1;
            (&DAT_00799a9c)[iVar2 * 0x42] = DAT_00bc9800;
            (&DAT_00799a9c)[iVar4 * 0x42] = iVar2;
            FUN_00486020(iVar2);
          }
        }
      }
    }
    goto switchD_0046ff7e_caseD_d5;
  case 0xa8: ;
    FUN_00475220(1,param_1,param_2,param_3,param_4,0x404000);
    break;
  case 0xa9: ;
    FUN_00475220(1,param_1,param_2,param_3,param_4,&DAT_00804000);
    break;
  case 0xaa: ;
    cVar1 = '\0';
    uVar6 = 0xffffffbf;
    uVar5 = 2;
    goto LAB_00470753;
  case 0xab: ;
    FUN_004bfa50(param_3,param_4,DAT_00bc9714);
    goto switchD_0046ff7e_caseD_d5;
  case 0xb6: ;
    iVar4 = FUN_0047d060(param_3,param_4,1);
    if (iVar4 == 0) {
      DAT_00605994 = 1;
    }
    if (5 < DAT_00bcb9b0) {
      return;
    }
    goto switchD_0046ff7e_caseD_4a;
  case 200: ;
    cVar1 = '\0';
    uVar6 = 0xffffffbf;
    uVar5 = 2;
    goto LAB_00470753;
  case 0xca: ;
    FUN_004bfa50(param_3,param_4,DAT_00bc9714);
    goto switchD_0046ff7e_caseD_d5;
  case 0xcf: ;
    cVar1 = '\0';
    uVar6 = 0xffffffbf;
    uVar5 = 3;
    goto LAB_00470753;
  case 0xd0: ;
    cVar1 = '\0';
    uVar6 = 0xffffffbf;
    uVar5 = 5;
LAB_00470753: ;
    FUN_0047d360(uVar5,param_3,iVar2,uVar6,0,cVar1);
    goto switchD_0046ff7e_caseD_d5;
  case 0xd1: ;
    FUN_00440ed0(1,param_1,param_2,param_3,param_4);
    break;
  case 0xd2: ;
    FUN_0047d530(6,param_3,param_4,0xffffffff,1);
  case 0xd5: ;
  case 0xde: ;
  case 0xe1: ;
  case 0xe3: ;
  case 0xe4: ;
  case 0xe5: ;
  case 0xe6: ;
  case 0xea: ;
  case 0xeb: ;
  case 0xec: ;
    goto switchD_0046ff7e_caseD_d5;
  }
  if (-1 < DAT_00bc971c) {
    DAT_00bc9724 = DAT_00bc971c * DAT_00bc9724;
  }
switchD_0046ff7e_caseD_d5: ;
  DAT_00bc9724 = DAT_00bc9724 - DAT_00bc9718;
  return;
switchD_0046ff7e_caseD_4a: ;
  iVar4 = FUN_0047d060(param_3,iVar2,1);
LAB_00470502: ;
  if (iVar4 == 0) {
    DAT_00605994 = 1;
  }
  goto switchD_0046ff7e_caseD_d5;
}
