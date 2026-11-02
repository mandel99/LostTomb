#include "restore_types.h"
#include "simulation_tick.h"
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern bool FUN_0044bd20();
extern void FUN_0044bd50();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern void FUN_004e0cf0();

undefined1 FUN_0044be30(uintptr_t in_ECX)

{
  short sVar1;
  uint uVar2;
  char cVar3;
  ushort uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  if (*(char *)(in_ECX + 2) != '\x03') {
    return 0;
  }
  if (*(short *)(in_ECX + 0x30) == 0) {
    cVar3 = FUN_0044d680(0);
    if (cVar3 != '\0') {
      *(undefined1 *)(in_ECX + 0x32) = 1;
      *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
      *(undefined2 *)(in_ECX + 0x30) = 1;
      *(undefined2 *)(in_ECX + 0x3a) = 0;
      *(undefined2 *)(in_ECX + 0x3e) = 0;
      FUN_0044bd50((int)*(short *)(in_ECX + 0x40),(int)*(short *)(in_ECX + 0x42));
      sVar1 = *(short *)(in_ECX + 0x42);
      DAT_00e950dc = 1;
      local_1e = 0xd7;
      iVar8 = DAT_00bca2d8 + *(short *)(in_ECX + 0x40);
      local_20 = 0xd8;
      local_10 = FUN_0044f660();
      FUN_0044e6f0(&local_20);
      FUN_004e0cf0(0x105,0,iVar8 + sVar1 * 0xe4,&local_20,0);
      return 1;
    }
    return 0;
  }
  iVar8 = 2;
  if (*(short *)(in_ECX + 0x30) == 2) {
    return 0;
  }
  *(short *)(in_ECX + 0x3e) = *(short *)(in_ECX + 0x3e) + 1;
  if (*(short *)(in_ECX + 0x3e) < *(short *)(in_ECX + 0x3c)) {
    return 1;
  }
  *(undefined2 *)(in_ECX + 0x3e) = 0;
  iVar5 = *(short *)(in_ECX + 0xc) * 0x19;
  if (iVar5 < 0x19) {
    iVar5 = 0x19;
  }
  *(short *)(in_ECX + 0x3a) = *(short *)(in_ECX + 0x3a) + 1;
  if (iVar5 <= *(short *)(in_ECX + 0x3a)) {
    *(undefined2 *)(in_ECX + 0x30) = 2;
    *(undefined1 *)(in_ECX + 0x32) = 0;
  }
  uVar2 = DAT_00d3b374 & 0xf;
  uVar4 = (ushort)DAT_00d3b374 & 0xf;
  *(ushort *)(in_ECX + 0x50) = uVar4;
  if (uVar2 == 0) {
    iVar8 = 0;
    iVar6 = 0;
    iVar5 = -1;
  }
  else if (uVar4 == 4) {
    iVar8 = 0;
    iVar6 = 0;
    iVar5 = -1;
  }
  else if (uVar4 == 5) {
    iVar8 = 0;
    iVar6 = -1;
    iVar5 = 0;
  }
  else if (uVar4 == 6) {
    iVar8 = 0;
    iVar6 = 1;
    iVar5 = 0;
  }
  else if (uVar4 == 1) {
    iVar8 = 1;
    iVar5 = 0;
    iVar6 = 1;
  }
  else if (uVar4 == 7) {
    iVar8 = 1;
    iVar5 = 0;
    iVar6 = 1;
  }
  else if (uVar4 == 8) {
    iVar6 = 0;
    iVar8 = 1;
    iVar5 = -1;
  }
  else {
    if (uVar4 == 9) {
      iVar8 = 1;
    }
    else if ((uVar4 != 2) && (uVar4 != 10)) {
      if (uVar4 == 0xb) {
        iVar6 = -1;
        iVar5 = 0;
        goto LAB_0044c060;
      }
      if (uVar4 == 0xc) {
        iVar6 = 1;
        iVar5 = 0;
        goto LAB_0044c060;
      }
      if (uVar4 == 3) {
        iVar6 = -1;
        iVar8 = 3;
        iVar5 = 0;
        goto LAB_0044c060;
      }
      if (uVar4 == 0xd) {
        iVar6 = -1;
        iVar8 = 3;
        iVar5 = 0;
        goto LAB_0044c060;
      }
      if (uVar4 == 0xe) {
        iVar6 = 0;
        iVar8 = 3;
        iVar5 = -1;
        goto LAB_0044c060;
      }
      if (uVar4 != 0xf) {
        return 1;
      }
      iVar8 = 3;
    }
    iVar6 = 0;
    iVar5 = 1;
  }
LAB_0044c060: ;
  iVar6 = *(short *)(in_ECX + 0x40 + iVar8 * 4) + iVar6;
  iVar5 = *(short *)(in_ECX + 0x42 + iVar8 * 4) + iVar5;
  if (iVar6 < 0) {
    iVar6 = 0;
  }
  if (DAT_00bca2d0 + -1 < iVar6) {
    iVar6 = DAT_00bca2d0 + -1;
  }
  if (iVar5 < 0) {
    iVar5 = 0;
  }
  if (DAT_00bca2d4 + -1 < iVar5) {
    iVar5 = DAT_00bca2d4 + -1;
  }
  iVar7 = FUN_0044bd20(iVar6,iVar5);
  if (iVar7 != 0) {
    *(short *)(in_ECX + 0x40 + iVar8 * 4) = (short)iVar6;
    *(short *)(in_ECX + 0x42 + iVar8 * 4) = (short)iVar5;
    FUN_0044bd50(iVar6,iVar5);
  }
  return 1;
}
