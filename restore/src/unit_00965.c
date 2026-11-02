#include "restore_types.h"
#define DAT_007900c0 (*(undefined4 *)0x007900c0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a28 (*(undefined4 *)0x00799a28u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00bcb9b0 (*(undefined *)0x00bcb9b0u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00ea3ff8 (*(undefined4 *)0x00ea3ff8u)
#define DAT_00ea3ffa (*(undefined4 *)0x00ea3ffau)
#define DAT_00ea3ffc (*(undefined4 *)0x00ea3ffcu)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5cf6 (*(undefined4 *)0x00ea5cf6u)
#define DAT_00ea5cfa (*(undefined4 *)0x00ea5cfau)
#define DAT_00ea5d14 (*(undefined4 *)0x00ea5d14u)
#define DAT_00ea5d18 (*(undefined4 *)0x00ea5d18u)
#define DAT_00ea5d20 (*(undefined4 *)0x00ea5d20u)
#define DAT_00ea5e64 (*(undefined4 *)0x00ea5e64u)
#define DAT_00ea5e68 (*(undefined4 *)0x00ea5e68u)
#define DAT_00ea5e70 (*(undefined4 *)0x00ea5e70u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea6010 (*(undefined4 *)0x00ea6010u)
#define DAT_00ea6014 (*(undefined4 *)0x00ea6014u)
#define DAT_00ea6082 (*(undefined4 *)0x00ea6082u)
#define DAT_00ea620c (*(undefined4 *)0x00ea620cu)
#define DAT_00ea6240 (*(undefined4 *)0x00ea6240u)
#define DAT_00ea6244 (*(undefined4 *)0x00ea6244u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eaaf68 (*(undefined4 *)0x00eaaf68u)
#define _DAT_00775678 (*(undefined4 *)0x00775678u)
extern void FUN_0046a2e0();
extern int FUN_00475b50();
extern void FUN_0047b590();
extern void FUN_0047bd50();
extern void FUN_004a36b0();
extern void FUN_004b82e0();
extern void FUN_004c0920();
extern void FUN_004dea50();
extern void FUN_004e5830();
extern void FUN_004f4ae0();

void FUN_0046ac50(int param_1)

{
  byte bVar1;
  short sVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  
  iVar6 = param_1 * 0x108;
  bVar1 = *(byte *)((int)&DAT_00799a98 + iVar6 + 1);
  if (bVar1 != 0) {
    *(undefined1 *)((int)&DAT_00799a98 + iVar6 + 1) = 0;
    (&DAT_007900c0)[(uint)bVar1 * 0xc4] = 0;
  }
  bVar1 = *(byte *)((int)&DAT_00799aa0 + iVar6 + 2);
  if (bVar1 != 0) {
    *(undefined1 *)((int)&DAT_00799aa0 + iVar6 + 2) = 0;
    (&DAT_00eaaf68)[(uint)bVar1 * 0x38] = 0;
  }
  if (*(char *)((int)&DAT_00799aa0 + iVar6 + 3) != '\0') {
    FUN_0046a2e0(param_1);
    *(undefined1 *)((int)&DAT_00799aa0 + iVar6 + 3) = 0;
  }
  iVar5 = DAT_00eaac28;
  sVar2 = (&DAT_007999e8)[param_1 * 0x84];
  iVar4 = (int)sVar2;
  switch((int)(iVar4)) {
  case 0x32: ;
    iVar4 = DAT_00eaac28 * 0x49d8;
    if (*(int *)(&DAT_007999e4 + iVar6) == (&DAT_00ea5e68)[DAT_00eaac28 * 0x1276]) {
      (&DAT_00ea5e68)[DAT_00eaac28 * 0x1276] = 0;
      *(undefined2 *)(&DAT_00ea5e64 + iVar5 * 0x1276) = 0;
      *(undefined2 *)((int)&DAT_00ea5e64 + iVar4 + 2) = 0;
      (&DAT_00ea5e70)[iVar5 * 0x1276] = 0;
      return;
    }
    break;
default: ;
    if ((sVar2 == 0xcf) || (sVar2 == 0xd0)) {
      FUN_004e5830(param_1);
      return;
    }
    if ((0x40 < iVar4) && (iVar4 < 0x46)) {
      (&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] = 0;
      (&DAT_00ea5cfa)[iVar5 * 0x24ec] = 0;
      uVar3 = FUN_00475b50(param_1);
      FUN_004c0920(uVar3);
      (&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276] = 0;
    }
    break;
  case 0x39: ;
    FUN_004b82e0((int)*(short *)(&DAT_00799a28 + iVar6));
    return;
  case 0x4a: ;
    FUN_0046a2e0(param_1);
    return;
  case 0x4d: ;
  case 0x4e: ;
  case 0x4f: ;
    *(undefined4 *)(&DAT_00ea620c + DAT_00eaac28 * 0x49d8) = 0;
    return;
  case 0x5e: ;
    *(undefined4 *)(&DAT_00ea6240 + DAT_00eaac28 * 0x49d8) = 0;
    return;
  case 0x5f: ;
    iVar4 = DAT_00eaac28 * 0x49d8;
    if (*(int *)(&DAT_007999e4 + iVar6) == (&DAT_00ea5d18)[DAT_00eaac28 * 0x1276]) {
      (&DAT_00ea5d18)[DAT_00eaac28 * 0x1276] = 0;
      *(undefined2 *)(&DAT_00ea5d14 + iVar5 * 0x1276) = 0;
      *(undefined2 *)((int)&DAT_00ea5d14 + iVar4 + 2) = 0;
      (&DAT_00ea5d20)[iVar5 * 0x1276] = 0;
      return;
    }
    break;
  case 0x88: ;
    if (0 < (int)(&DAT_00799a9c)[param_1 * 0x42]) {
      (&DAT_00799a9c)[(&DAT_00799a9c)[param_1 * 0x42] * 0x42] = 0xffffffff;
      FUN_004a36b0(param_1);
      return;
    }
    break;
  case 0x8a: ;
  case 0xd7: ;
    iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2);
    FUN_0047b590(iVar5,(int)*(short *)(&DAT_007999e0 + iVar6),iVar5,
                 (int)*(short *)(&DAT_007999e0 + iVar6));
    iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2);
    FUN_0047bd50(iVar5,(int)*(short *)(&DAT_007999e0 + iVar6),iVar5,
                 (int)*(short *)(&DAT_007999e0 + iVar6));
    return;
  case 0xb6: ;
    DAT_00bcb9b0 = DAT_00bcb9b0 + -1;
    return;
  case 0xbb: ;
  case 0xbc: ;
  case 0xbd: ;
    iVar4 = DAT_00eaac28 * 0x49d8;
    if (*(int *)(&DAT_007999e4 + iVar6) == *(int *)(&DAT_00ea3ffc + iVar4)) {
      *(undefined4 *)(&DAT_00ea3ffc + iVar4) = 0;
      *(undefined2 *)(&DAT_00ea3ff8 + iVar4) = 0;
      *(undefined2 *)(&DAT_00ea3ffa + iVar4) = 0;
      (&DAT_00ea5cf6)[iVar5 * 0x24ec] = 0;
      return;
    }
    break;
  case 0xd1: ;
    iVar6 = DAT_00eaac28 * 0x49d8;
    *(undefined4 *)(&DAT_00ea6010 + iVar6) = 0xffffffff;
    *(undefined4 *)(&DAT_00ea6014 + iVar6) = 0xffffffff;
    *(undefined2 *)((int)&DAT_00ea6082 + iVar6 + 2) = 0;
    _DAT_00775678 = 0;
    return;
  case 0xd2: ;
    iVar6 = DAT_00eaac28 * 0x49d8;
    *(int *)(&DAT_00ea6244 + iVar6) = (int)*(short *)((int)&DAT_00ea5ba8 + iVar6 + 2);
    *(undefined2 *)((int)&DAT_00ea5ba8 + iVar6 + 2) = 0;
  case 0xb7: ;
  case 0xd5: ;
  case 0xde: ;
  case 0xe1: ;
  case 0xe3: ;
    FUN_004f4ae0(param_1);
    return;
  case 0xe4: ;
    FUN_004f4ae0(param_1);
    if ((&DAT_007999ea)[param_1 * 0x84] == 1) {
      FUN_004dea50();
      DAT_00e38dca = 1;
      return;
    }
    break;
  case 0xe5: ;
    FUN_004f4ae0(param_1);
    bVar7 = (&DAT_007999ea)[param_1 * 0x84] == 0xa6;
    goto LAB_0046aef9;
  case 0xe6: ;
    FUN_004f4ae0(param_1);
    bVar7 = (&DAT_007999ea)[param_1 * 0x84] == 0xf;
    goto LAB_0046aef9;
  case 0xea: ;
    FUN_004f4ae0(param_1);
    bVar7 = (&DAT_007999ea)[param_1 * 0x84] == 0x9d;
    goto LAB_0046aef9;
  case 0xeb: ;
    FUN_004f4ae0(param_1);
    bVar7 = (&DAT_007999ea)[param_1 * 0x84] == 0x1b4;
    goto LAB_0046aef9;
  case 0xec: ;
    FUN_004f4ae0(param_1);
    bVar7 = (&DAT_007999ea)[param_1 * 0x84] == 0x200;
LAB_0046aef9: ;
    if (bVar7) {
      FUN_004dea50();
      DAT_00e38dca = 1;
      return;
    }
  }
  return;
}
