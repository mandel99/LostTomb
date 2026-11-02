#include "restore_types.h"
#define DAT_006fdce4 (*(undefined *)0x006fdce4u)
#define DAT_006fdcf0 (*(undefined *)0x006fdcf0u)
#define DAT_006fdcfc (*(undefined *)0x006fdcfcu)
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00bcb9bc (*(undefined *)0x00bcb9bcu)
#define DAT_00bcb9c8 (*(undefined4 *)0x00bcb9c8u)
#define DAT_00bcb9ca (*(undefined4 *)0x00bcb9cau)
#define DAT_00bcb9cb (*(undefined4 *)0x00bcb9cbu)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9ce (*(undefined4 *)0x00bcb9ceu)
#define DAT_00bcb9f2 (*(undefined4 *)0x00bcb9f2u)
#define DAT_00bcb9f4 (*(undefined4 *)0x00bcb9f4u)
#define DAT_00bcb9f6 (*(undefined4 *)0x00bcb9f6u)
#define DAT_00bcb9f8 (*(undefined4 *)0x00bcb9f8u)
#define DAT_00bcb9fa (*(undefined4 *)0x00bcb9fau)
#define DAT_00bcb9fc (*(undefined4 *)0x00bcb9fcu)
#define DAT_00bcb9fe (*(undefined4 *)0x00bcb9feu)
#define DAT_00bcba00 (*(undefined4 *)0x00bcba00u)
#define DAT_00bcba08 (*(undefined4 *)0x00bcba08u)
#define DAT_00bcba0a (*(undefined4 *)0x00bcba0au)
#define DAT_00bcba3f (*(undefined4 *)0x00bcba3fu)
#define DAT_00bcba46 (*(undefined4 *)0x00bcba46u)
#define DAT_00bcba48 (*(undefined4 *)0x00bcba48u)
#define DAT_00bcba4a (*(undefined4 *)0x00bcba4au)
#define DAT_00bcba58 (*(undefined4 *)0x00bcba58u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004b4800();
extern void FUN_004b77d0();

int FUN_004b6e10(int param_1)

{
  undefined1 uVar1;
  uint uVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  short sVar6;
  short sVar7;
  int iVar8;
  int iVar9;
  char local_8 [8];
  
  FUN_004b77d0();
  uVar1 = (undefined1)DAT_00eaac28;
  uVar2 = 0;
  local_8[0] = '\0';
  local_8[1] = '\0';
  local_8[2] = '\0';
  local_8[3] = '\0';
  local_8[4] = '\0';
  local_8[5] = '\0';
  local_8[6] = '\0';
  local_8[7] = '\0';
  do {
    if ((DAT_006fdce4 & 0xff) == uVar2) {
      local_8[uVar2] = '\x01';
    }
    if ((DAT_006fdcf0 & 0xff) == uVar2) {
      local_8[uVar2] = '\x01';
    }
    if ((DAT_006fdcfc & 0xff) == uVar2) {
      local_8[uVar2] = '\x01';
    }
    uVar2 = uVar2 + 1;
  } while ((int)uVar2 < 8);
  pcVar3 = &DAT_00bcba0a;
  do {
    if ((pcVar3[-0x42] != '\0') && (*pcVar3 == '\x01')) {
      local_8[(byte)pcVar3[-0x40]] = '\x01';
    }
    pcVar3 = pcVar3 + 0x90;
  } while ((int)pcVar3 < 0xbcd62a);
  iVar4 = 0;
  do {
    if (local_8[iVar4] == '\0') goto LAB_004b6eb1;
    iVar4 = iVar4 + 1;
  } while (iVar4 < 8);
  iVar4 = 0;
LAB_004b6eb1: ;
  iVar9 = 1;
  pcVar3 = &DAT_00bcba58;
  do {
    if (*pcVar3 == '\0') {
      iVar8 = iVar9 * 0x90;
      (&DAT_00bcb9c8)[iVar8] = 1;
      (&DAT_00bcba0a)[iVar8] = 1;
      iVar5 = param_1 * 0x108;
      *(undefined1 *)(iVar8 + 0xbcb9c9) = uVar1;
      *(undefined2 *)(&DAT_00bcb9cc + iVar8) = (&DAT_007999ea)[param_1 * 0x84];
      *(short *)(&DAT_00bcb9ce + iVar8) = (short)param_1;
      sVar7 = *(short *)(&DAT_007999e0 + iVar5);
      *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 1;
      *(undefined2 *)(&DAT_00bcb9f4 + iVar8) = 0x32;
      (&DAT_00bcb9cb)[iVar8] = 1;
      *(undefined2 *)(&DAT_00bcba4a + iVar8) = 0xffff;
      (&DAT_00bcba3f)[iVar8] = 0;
      (&DAT_00bcb9ca)[iVar8] = (char)iVar4;
      sVar6 = *(short *)((int)&DAT_007999dc + iVar5 + 2) + 3;
      sVar7 = sVar7 + -1;
      *(short *)(&DAT_00bcb9f6 + iVar8) = sVar6;
      *(short *)(&DAT_00bcb9f8 + iVar8) = sVar7;
      *(short *)(&DAT_00bcb9fa + iVar8) = sVar6;
      *(short *)(&DAT_00bcb9fc + iVar8) = sVar7;
      *(short *)(&DAT_00bcb9fe + iVar8) = sVar6;
      *(short *)(&DAT_00bcba00 + iVar8) = sVar7;
      *(undefined2 *)(&DAT_00bcba48 + iVar8) = 0;
      *(undefined2 *)(&DAT_00bcba46 + iVar8) = 0;
      iVar5 = FUN_004b4800((&DAT_007999d9)[iVar5],0xe,0,0,0);
      *(short *)(&DAT_00bcba08 + iVar8) = (short)iVar5;
      iVar4 = DAT_00bcb9bc;
      (&DAT_00bcd652)[iVar5 * 0xc2] = (short)param_1;
      (&DAT_00bcd658)[iVar5 * 0xc2] = (short)iVar9;
      DAT_00bcb9b4 = DAT_00bcb9b4 + 1;
      if (iVar4 < iVar9) {
        DAT_00bcb9bc = iVar9;
      }
      return iVar9;
    }
    pcVar3 = pcVar3 + 0x90;
    iVar9 = iVar9 + 1;
  } while ((int)pcVar3 < 0xbcd5e8);
  return 0;
}
