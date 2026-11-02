#include "restore_types.h"
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_0077de05 (*(undefined4 *)0x0077de05u)
#define DAT_0077de06 (*(undefined4 *)0x0077de06u)
#define DAT_0077de2a (*(undefined4 *)0x0077de2au)
#define DAT_0077de54 (*(undefined4 *)0x0077de54u)
#define DAT_0077f754 (*(undefined4 *)0x0077f754u)
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern int FUN_004425c0();
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern int FUN_004449d0();
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();
extern uint FUN_005599c0();

void FUN_0044e2a0(void)

{
  bool bVar1;
  char cVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int in_ECX;
  undefined1 uVar7;
  int iVar8;
  int iVar9;
  undefined4 local_24;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  local_24 = 0x83;
  bVar1 = false;
  cVar2 = FUN_0044d680(0);
  if (cVar2 == '\0') {
    return;
  }
  *(uint *)(in_ECX + 0x28) = *(uint *)(in_ECX + 0x28) | 4;
  iVar4 = FUN_004449d0((int)*(short *)(in_ECX + 0x1c));
  if (iVar4 == 0) goto switchD_0044e308_default;
  iVar4 = iVar4 * 0x6a;
  iVar8 = (int)*(short *)((int)&DAT_0077de54 + iVar4 + 2);
  switch(*(undefined1 *)(in_ECX + 0x60)) {
  case 0: ;
    if ((&DAT_0077f760)[iVar8 * 0x62] == '\x03') {
      uVar7 = 5;
    }
    else {
      if ((&DAT_0077f760)[iVar8 * 0x62] != '\x04') goto switchD_0044e308_default;
      uVar7 = 6;
    }
    local_1e = 0x7e;
    local_20 = 0x7f;
    local_10 = FUN_0044f660();
    local_24 = 0x152;
    goto LAB_0044e4ba;
  case 1: ;
    if ((&DAT_0077f760)[iVar8 * 0x62] == '\x05') {
      uVar7 = 3;
    }
    else {
      if ((&DAT_0077f760)[iVar8 * 0x62] != '\x06') goto switchD_0044e308_default;
      uVar7 = 4;
    }
    local_1e = 0x82;
    local_20 = 0x83;
    local_10 = FUN_0044f660();
    local_24 = 0x153;
    goto LAB_0044e4ba;
  case 2: ;
    cVar2 = (&DAT_0077f760)[iVar8 * 0x62];
    if (cVar2 == '\x06') {
      uVar7 = 5;
      local_1e = 0x86;
      local_20 = 0x87;
    }
    else if (cVar2 == '\x04') {
      uVar7 = 3;
      local_1e = 0x86;
      local_20 = 0x87;
    }
    else {
      if (cVar2 != '\x02') goto switchD_0044e308_default;
      uVar7 = 1;
      local_1e = 0x86;
      local_20 = 0x87;
    }
    break;
  case 3: ;
    cVar2 = (&DAT_0077f760)[iVar8 * 0x62];
    if (cVar2 == '\x05') {
      uVar7 = 6;
    }
    else if (cVar2 == '\x03') {
      uVar7 = 4;
    }
    else {
      if (cVar2 != '\x01') goto switchD_0044e308_default;
      uVar7 = 2;
    }
    bVar1 = true;
    local_1e = 0x8a;
    local_20 = 0x8b;
    break;
  case 4: ;
    cVar2 = (&DAT_0077f760)[iVar8 * 0x62];
    if (cVar2 == '\x05') {
      uVar7 = 6;
    }
    else if (cVar2 == '\x03') {
      uVar7 = 4;
    }
    else {
      if (cVar2 != '\x01') goto switchD_0044e308_default;
      uVar7 = 2;
    }
    bVar1 = true;
    local_1e = 0x8e;
    local_20 = 0x8f;
    break;
default: ;
    goto switchD_0044e308_default;
  }
  local_10 = FUN_0044f660();
LAB_0044e4ba: ;
  FUN_0044e6f0(&local_20);
  (&DAT_0077f760)[iVar8 * 0x62] = uVar7;
  uVar3 = FUN_004425c0(iVar8);
  (&DAT_0077f754)[iVar8 * 0x31] = uVar3;
  (&DAT_0077de02)[iVar4] = (&DAT_0077f760)[iVar8 * 0x62];
  if (bVar1) {
    (&DAT_0077de05)[iVar4] = 0;
  }
  iVar9 = 1;
  do {
    (&DAT_0077de2a)[iVar9 + iVar4] = 0;
    (&DAT_0077de06)[iVar9 + iVar4] = 0;
    cVar2 = (&DAT_0077de02)[iVar4];
    if ((((cVar2 == '\x01') || (cVar2 == '\x03')) || (cVar2 == '\x05')) || (cVar2 == '\0')) {
      iVar5 = FUN_00443000(iVar8,iVar9);
      if (iVar5 != 0) {
        (&DAT_0077de2a)[iVar9 + iVar4] = 1;
      }
      iVar5 = FUN_00443040(iVar8,iVar9);
      if (iVar5 != 0) {
        (&DAT_0077de06)[iVar9 + iVar4] = 1;
      }
    }
    FUN_005599c0(iVar9);
    iVar9 = iVar9 + 1;
  } while (iVar9 < 0x24);
  DAT_00e950dc = 1;
  FUN_004e0cf0(local_24,0,0,&local_20,0);
switchD_0044e308_default: ;
  cVar2 = FUN_004496d0((int)*(short *)(in_ECX + 0x24));
  if (cVar2 != '\0') {
    uVar6 = FUN_0044fd30(0);
    FUN_0044c490((int)*(short *)(in_ECX + 0x24),uVar6);
  }
  return;
}
