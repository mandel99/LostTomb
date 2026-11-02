#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea3ff4 (*(undefined4 *)0x00ea3ff4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00446260();
extern uint FUN_004496d0();
extern undefined4 FUN_0044c110();
extern void FUN_0044c490();
extern int FUN_0044c9a0();
extern int FUN_0044d750();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004b8690();
extern void FUN_004e0cf0();

void FUN_0044c9e0(void)

{
  char cVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  short *in_ECX;
  undefined4 uVar5;
  undefined4 uVar6;
  short local_20 [6];
  undefined2 local_14;
  undefined2 local_10;
  
  if (in_ECX[0x18] == 8) {
    return;
  }
  if ((char)in_ECX[0x2b] == '\0') {
    if ((char)in_ECX[0x30] != '\x01') {
      local_20[1] = 0x13;
      goto LAB_0044ca3c;
    }
    local_20[1] = 1;
LAB_0044ca0e: ;
    local_10 = FUN_0044f660();
    uVar5 = 0x10a;
  }
  else {
    if ((char)in_ECX[0x30] == '\x01') {
      local_20[1] = 0;
      goto LAB_0044ca0e;
    }
    local_20[1] = 0x12;
LAB_0044ca3c: ;
    local_10 = FUN_0044f660();
    uVar5 = 0x110;
  }
  FUN_0044e6f0(local_20);
  iVar3 = FUN_0044d750();
  if (iVar3 == 0) {
    DAT_00e950dc = 1;
    if (in_ECX[0x18] == 0) {
      cVar1 = FUN_004496d0((int)in_ECX[0x13]);
      if (cVar1 == '\0') goto LAB_0044cb7a;
      uVar5 = FUN_0044fd30(1);
      sVar2 = in_ECX[0x13];
LAB_0044cb73: ;
      FUN_0044c490((int)sVar2,uVar5);
LAB_0044cb7a: ;
      in_ECX[0x18] = 8;
      *(undefined1 *)(in_ECX + 0x19) = 0;
      return;
    }
    if (in_ECX[0x18] != 5) {
      in_ECX[0x18] = 8;
      *(undefined1 *)(in_ECX + 0x19) = 0;
      return;
    }
    if (in_ECX[0x17] < 3) {
      iVar3 = FUN_00446260((int)*in_ECX);
      if (iVar3 == 0) {
        cVar1 = FUN_004496d0((int)in_ECX[0x2a]);
        if (cVar1 == '\0') goto LAB_0044cb7a;
        uVar5 = FUN_0044fd30(3);
        sVar2 = in_ECX[0x2a];
        goto LAB_0044cb73;
      }
      cVar1 = FUN_004496d0((int)in_ECX[0x12]);
      if (cVar1 != '\0') {
        uVar5 = FUN_0044fd30(0);
        FUN_0044c490((int)in_ECX[0x12],uVar5);
      }
      in_ECX[0x18] = 6;
      sVar2 = FUN_0044c110();
    }
    else {
      cVar1 = FUN_004496d0((int)in_ECX[0x29]);
      if (cVar1 != '\0') {
        uVar5 = FUN_0044fd30(2);
        FUN_0044c490((int)in_ECX[0x29],uVar5);
      }
      in_ECX[0x18] = 7;
      sVar2 = FUN_0044c110();
      sVar2 = sVar2 - in_ECX[0x17];
    }
    in_ECX[0x17] = sVar2;
LAB_0044cb42: ;
    if (sVar2 < 1) {
      in_ECX[0x17] = 1;
      return;
    }
  }
  else {
    switch(in_ECX[0x18]) {
    case 0: ;
      local_14 = FUN_0044c9a0();
      if ((char)in_ECX[0x19] == '\0') {
        if ((((*(byte *)(in_ECX + 0x14) & 0x11) == 0) &&
            (DAT_0078d7bc == (int)in_ECX[10] + (int)DAT_007848d8)) &&
           (DAT_0078d7c0 == *(byte *)((int)in_ECX + 3))) {
          *(undefined1 *)(in_ECX + 0x19) = 1;
          in_ECX[0x17] = in_ECX[0x16];
          DAT_00e950dc = 1;
          if ((char)in_ECX[0x2b] == '\0') {
            local_20[0] = (-(ushort)((char)in_ECX[0x30] != '\x01') & 0x12) + 3;
          }
          else {
            local_20[0] = (-(ushort)((char)in_ECX[0x30] != '\x01') & 0x12) + 2;
          }
          FUN_004e0cf0(uVar5,(int)*in_ECX,0,local_20,0);
          return;
        }
      }
      else {
        iVar4 = FUN_0044c110();
        if (iVar4 < 1) {
          iVar4 = 0xc;
        }
        if (iVar3 - iVar4 == (uint)*(byte *)((int)in_ECX + 0x61)) {
          if ((char)in_ECX[0x2b] == '\0') {
            local_20[0] = (-(ushort)((char)in_ECX[0x30] != '\x01') & 0x12) + 5;
          }
          else {
            local_20[0] = (-(ushort)((char)in_ECX[0x30] != '\x01') & 0x12) + 4;
          }
          sVar2 = *in_ECX;
        }
        else {
          if (iVar3 - iVar4 != (uint)*(byte *)(in_ECX + 0x31)) {
            return;
          }
          if ((char)in_ECX[0x2b] == '\0') {
            local_20[0] = (-(ushort)((char)in_ECX[0x30] != '\x01') & 0x12) + 7;
          }
          else {
            local_20[0] = (-(ushort)((char)in_ECX[0x30] != '\x01') & 0x12) + 6;
          }
          sVar2 = *in_ECX;
        }
        FUN_004e0cf0(0x82,(int)sVar2,0,local_20,0);
      }
      break;
    case 5: ;
      in_ECX[0x17] = in_ECX[0x17] + -1;
      sVar2 = in_ECX[0x17];
      goto LAB_0044cb42;
    case 6: ;
      in_ECX[0x17] = in_ECX[0x17] + -1;
      if (in_ECX[0x17] < 1) {
        uVar5 = *(undefined4 *)(&DAT_00ea3ff4 + DAT_00eaac28 * 0x49d8);
        uVar6 = 0x120;
LAB_0044cbdc: ;
        DAT_00e950dc = 1;
        FUN_004e0cf0(uVar6,0,uVar5,0,0);
        FUN_004b8690((int)*in_ECX);
        in_ECX[0x18] = 8;
        *(undefined1 *)(in_ECX + 0x19) = 0;
        return;
      }
      break;
    case 7: ;
      in_ECX[0x17] = in_ECX[0x17] + -1;
      if (in_ECX[0x17] < 1) {
        uVar5 = *(undefined4 *)(&DAT_00ea3ff4 + DAT_00eaac28 * 0x49d8);
        uVar6 = 0x11f;
        goto LAB_0044cbdc;
      }
    }
  }
  return;
}
