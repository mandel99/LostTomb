#include "restore_types.h"
#define DAT_00799a06 (*(undefined4 *)0x00799a06u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd654 (*(undefined4 *)0x00bcd654u)
#define DAT_00bcd65c (*(undefined4 *)0x00bcd65cu)
#define DAT_00bcd66a (*(undefined4 *)0x00bcd66au)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
#define s_Created_frog__d_at___d___d__head_005e4958 (*(char (*)[53])0x005e4958u)
extern int FUN_004b4800();
extern undefined4 FUN_00561a36();

void FUN_004e7410(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int local_1f8;
  CHAR local_1f4 [500];
  
  iVar1 = FUN_004b4800(DAT_00eaac28,0x6a,param_2,param_3,0);
  iVar4 = iVar1 * 0x184;
  (&DAT_00bcd630)[iVar4] = 8;
  (&DAT_00bcd654)[iVar1 * 0xc2] = (short)param_1;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  (&DAT_00799a06)[param_1 * 0x84] = (short)iVar1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  local_1f8 = 0x1f;
  do {
    uVar2 = DAT_00d3b384 >> 4;
    uVar5 = DAT_00d3b384 & 1;
    uVar6 = DAT_00d3b380 >> 4;
    uVar3 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar2 & 1) != uVar5) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar6 & 1) != uVar3) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    local_1f8 = local_1f8 + -1;
  } while (local_1f8 != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  *(short *)(&DAT_00bcd62e + iVar4) = (short)((ulonglong)_DAT_00d3b370 % 0x96);
  (&DAT_00bcd65c)[iVar4] = 0;
  (&DAT_00bcd66a)[iVar4] = 6;
  FUN_00561a36(local_1f4,s_Created_frog__d_at___d___d__head_005e4958,iVar1,param_2,param_3,param_1);
  OutputDebugStringA(local_1f4);
  return;
}
