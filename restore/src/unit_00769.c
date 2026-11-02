#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5cc8 (*(undefined4 *)0x00ea5cc8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern int FUN_0045aac0();
extern int FUN_0045afe0();
extern void FUN_004e0cf0();

void FUN_0044e890(void)

{
  bool bVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  undefined4 uVar5;
  short *in_ECX;
  int iVar6;
  short local_20 [8];
  undefined2 local_10;
  short local_c;
  short local_a;
  
  if (in_ECX[0x18] == 9) {
    return;
  }
  local_20[1] = 0x108;
  if ((char)in_ECX[0x2b] == '\0') {
    local_20[1] = 0x109;
  }
  local_10 = FUN_0044f660();
  FUN_0044e6f0(local_20);
  iVar4 = DAT_00eaac28;
  sVar3 = in_ECX[0x18];
  if (((sVar3 != 6) && (sVar3 != 7)) && (sVar3 != 8)) {
    if ((char)in_ECX[0x19] == '\0') {
      if ((*(byte *)(in_ECX + 0x14) & 0x11) != 0) {
        return;
      }
      if (DAT_0078d7bc != (int)in_ECX[10] + (int)DAT_007848d8) {
        return;
      }
      if (DAT_0078d7c0 != *(byte *)((int)in_ECX + 3)) {
        return;
      }
      iVar6 = (int)in_ECX[6];
      *(undefined1 *)(in_ECX + 0x19) = 1;
      iVar4 = iVar6;
      if (in_ECX[2] != 0x24) {
        iVar4 = FUN_0045aac0(DAT_00eaac28,(int)in_ECX[2]);
        iVar4 = iVar4 / 100;
      }
      if (iVar4 < iVar6) {
        if (iVar4 < iVar6 / 2) {
          local_a = 0;
          in_ECX[0x17] = 1;
          local_20[0] = 0x111 - (ushort)((char)in_ECX[0x2b] != '\0');
        }
        else {
          local_a = 7;
          local_20[0] = 0x10f - (ushort)((char)in_ECX[0x2b] != '\0');
        }
      }
      else if (in_ECX[2] == 0x24) {
        local_a = 5;
        local_20[0] = 0x10d - (ushort)((char)in_ECX[0x2b] != '\0');
      }
      else {
        local_a = 5;
        local_20[0] = 0x10b - (ushort)((char)in_ECX[0x2b] != '\0');
      }
      DAT_00e950dc = 1;
      FUN_004e0cf0(0x84,(int)*in_ECX,0,local_20,0);
      in_ECX[0x18] = 2 - (ushort)(local_a != 0);
      return;
    }
    in_ECX[0x17] = in_ECX[0x17] + -1;
    if (0 < in_ECX[0x17]) {
      return;
    }
    iVar6 = (int)in_ECX[6];
    iVar4 = iVar6;
    if (in_ECX[2] != 0x24) {
      iVar4 = FUN_0045aac0(DAT_00eaac28,(int)in_ECX[2]);
      iVar4 = iVar4 / 100;
    }
    bVar1 = iVar4 < iVar6 / 2;
    if (in_ECX[0x18] != 2) {
      if (in_ECX[0x18] != 4) {
        return;
      }
      if (iVar4 < iVar6) {
        if (bVar1) {
          local_a = 0;
          local_20[0] = 0x115 - (ushort)((char)in_ECX[0x2b] != '\0');
        }
        else {
          local_a = 7;
          local_20[0] = 0x113 - (ushort)((char)in_ECX[0x2b] != '\0');
        }
      }
      else if (in_ECX[2] == 0x24) {
        local_a = 5;
        local_20[0] = 0x10d - (ushort)((char)in_ECX[0x2b] != '\0');
      }
      else {
        local_a = 5;
        local_20[0] = 0x10b - (ushort)((char)in_ECX[0x2b] != '\0');
      }
      DAT_00e950dc = 1;
      FUN_004e0cf0(0x84,(int)*in_ECX,0,local_20,0);
      in_ECX[0x18] = (-(ushort)(local_a != 0) & 0xfffd) + 8;
      return;
    }
    if (iVar4 < iVar6) {
      if (bVar1) {
        local_a = 0;
        local_20[0] = 0x111 - (ushort)((char)in_ECX[0x2b] != '\0');
      }
      else {
        local_a = 7;
        local_20[0] = 0x10f - (ushort)((char)in_ECX[0x2b] != '\0');
      }
    }
    else if (in_ECX[2] == 0x24) {
      local_a = 5;
      local_20[0] = 0x10d - (ushort)((char)in_ECX[0x2b] != '\0');
    }
    else {
      local_a = 5;
      local_20[0] = 0x10b - (ushort)((char)in_ECX[0x2b] != '\0');
    }
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x84,(int)*in_ECX,0,local_20,0);
    in_ECX[0x18] = 4 - (ushort)(local_a != 0);
    return;
  }
  DAT_00e950dc = 1;
  sVar3 = in_ECX[0x18];
  if (sVar3 == 6) {
    local_c = in_ECX[6];
    iVar6 = (int)local_c;
    if (in_ECX[2] == 0x24) {
      *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) =
           *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) + iVar6;
      (&DAT_00ea5cc8)[iVar4 * 0x1276] = (&DAT_00ea5cc8)[iVar4 * 0x1276] + iVar6;
      local_20[0] = 0x119 - (ushort)((char)in_ECX[0x2b] != '\0');
    }
    else {
      sVar3 = FUN_0045afe0((int)in_ECX[2],iVar6 * 100,0);
      local_20[0] = 0x117 - (ushort)((char)in_ECX[0x2b] != '\0');
      local_c = (sVar3 / 100 + (sVar3 >> 0xf)) - (short)((longlong)(int)sVar3 * 0x51eb851f >> 0x3f);
    }
    FUN_004e0cf0(0x84,(int)*in_ECX,0,local_20,0);
    cVar2 = FUN_004496d0((int)in_ECX[0x12]);
    if (cVar2 == '\0') goto LAB_0044edb1;
    uVar5 = FUN_0044fd30(0);
    iVar4 = (int)in_ECX[0x12];
  }
  else if (sVar3 == 7) {
    local_20[0] = 0x11d - (ushort)((char)in_ECX[0x2b] != '\0');
    FUN_004e0cf0(0x84,(int)*in_ECX,0,local_20,0);
    cVar2 = FUN_004496d0((int)in_ECX[0x13]);
    if (cVar2 == '\0') goto LAB_0044edb1;
    uVar5 = FUN_0044fd30(1);
    iVar4 = (int)in_ECX[0x13];
  }
  else {
    if (sVar3 != 8) goto LAB_0044edb1;
    local_20[0] = 0x115 - (ushort)((char)in_ECX[0x2b] != '\0');
    FUN_004e0cf0(0x84,(int)*in_ECX,0,local_20,0);
    cVar2 = FUN_004496d0((int)in_ECX[0x29]);
    if (cVar2 == '\0') goto LAB_0044edb1;
    uVar5 = FUN_0044fd30(2);
    iVar4 = (int)in_ECX[0x29];
  }
  FUN_0044c490(iVar4,uVar5);
LAB_0044edb1: ;
  in_ECX[0x18] = 9;
  *(undefined1 *)(in_ECX + 0x19) = 0;
  *(uint *)(in_ECX + 0x14) = *(uint *)(in_ECX + 0x14) | 4;
  return;
}
