#include "restore_types.h"
#define DAT_00579590 (*(undefined *)0x00579590u)
#define DAT_006057e4 (*(undefined *)0x006057e4u)
#define DAT_007027c0 (*(char *)0x007027c0u)
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5df8 (*(undefined4 *)0x00ea5df8u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00417ae0();
extern uint FUN_004496d0();
extern void FUN_0044c490();
extern void FUN_0044c9e0();
extern void FUN_0044e6f0();
extern int FUN_0044f660();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();
extern void FUN_004e96c0();

void FUN_0044cdc0(void)

{
  short sVar1;
  char cVar2;
  int iVar3;
  short *in_ECX;
  undefined2 extraout_var;
  undefined2 extraout_var_00;
  undefined2 extraout_var_01;
  undefined2 extraout_var_02;
  undefined2 extraout_var_03;
  undefined2 uVar4;
  undefined4 uVar5;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  short local_14;
  undefined2 local_10;
  
  if (in_ECX[2] == 0x25) {
    FUN_0044c9e0();
    return;
  }
  if (in_ECX[0x18] == 8) {
    return;
  }
  if (in_ECX[0x18] == 4) {
    return;
  }
  uVar5 = 0x82;
  if ((char)in_ECX[0x2b] == '\0') {
    switch((char)in_ECX[0x30]) {
    case '\0': ;
      local_1e = 0x25;
      break;
    case '\x03': ;
      local_1e = 0x37;
      uVar5 = 0x113;
      break;
    case '\x04': ;
      local_1e = 0x49;
      uVar5 = 0x114;
      break;
    case '\x05': ;
      local_1e = 0x5b;
      uVar5 = 0x14a;
      break;
    case '\x06': ;
      local_1e = 0x6d;
    }
  }
  else {
    switch((char)in_ECX[0x30]) {
    case '\0': ;
      local_1e = 0x24;
      break;
    case '\x03': ;
      local_1e = 0x36;
      uVar5 = 0x113;
      break;
    case '\x04': ;
      local_1e = 0x48;
      uVar5 = 0x114;
      break;
    case '\x05': ;
      local_1e = 0x5a;
      uVar5 = 0x14a;
      break;
    case '\x06': ;
      local_1e = 0x6c;
    }
  }
  local_10 = FUN_0044f660();
  FUN_0044e6f0(&local_20);
  sVar1 = in_ECX[0x18];
  if ((sVar1 != 2) && (sVar1 != 3)) {
    if ((char)in_ECX[0x19] != '\0') {
      in_ECX[0x17] = in_ECX[0x17] + -1;
      local_14 = in_ECX[0x17];
      iVar3 = DAT_00eaac28;
      uVar4 = extraout_var;
      if (sVar1 == 0) {
        if (local_14 == 6) {
          DAT_00e950dc = 1;
          if ((char)in_ECX[0x2b] == '\0') {
            switch((char)in_ECX[0x30]) {
            case '\0': ;
              local_20 = 0x29;
              break;
            case '\x03': ;
              local_20 = 0x3b;
              break;
            case '\x04': ;
              local_20 = 0x4d;
              break;
            case '\x05': ;
              local_20 = 0x5f;
              break;
            case '\x06': ;
              local_20 = 0x71;
            }
          }
          else {
            switch((char)in_ECX[0x30]) {
            case '\0': ;
              local_20 = 0x28;
              break;
            case '\x03': ;
              local_20 = 0x3a;
              break;
            case '\x04': ;
              local_20 = 0x4c;
              break;
            case '\x05': ;
              local_20 = 0x5e;
              break;
            case '\x06': ;
              local_20 = 0x70;
            }
          }
          FUN_004e0cf0(uVar5,(int)*in_ECX,0,&local_20,0);
          iVar3 = DAT_00eaac28;
          uVar4 = extraout_var_00;
        }
        else if (local_14 < 1) {
          DAT_00e950dc = 1;
          if ((char)in_ECX[0x2b] == '\0') {
            switch((char)in_ECX[0x30]) {
            case '\0': ;
              local_20 = 0x2b;
              break;
            case '\x03': ;
              local_20 = 0x3d;
              break;
            case '\x04': ;
              local_20 = 0x4f;
              break;
            case '\x05': ;
              local_20 = 0x61;
              break;
            case '\x06': ;
              local_20 = 0x73;
            }
          }
          else {
            switch((char)in_ECX[0x30]) {
            case '\0': ;
              local_20 = 0x2a;
              break;
            case '\x03': ;
              local_20 = 0x3c;
              break;
            case '\x04': ;
              local_20 = 0x4e;
              break;
            case '\x05': ;
              local_20 = 0x60;
              break;
            case '\x06': ;
              local_20 = 0x72;
            }
          }
          in_ECX[0x17] = 0x18;
          local_14 = 0x18;
          in_ECX[0x18] = 1;
          FUN_004e0cf0(0x82,(int)*in_ECX,0,&local_20,0);
          FUN_004e96c0(0xfffffffd);
          iVar3 = DAT_00eaac28;
          (&DAT_00ea5df8)[DAT_00eaac28 * 0x1276] = 3;
          uVar4 = extraout_var_01;
        }
      }
      else if ((sVar1 == 1) && (local_14 < 1)) {
        cVar2 = FUN_004496d0((int)in_ECX[0x13]);
        uVar4 = extraout_var_02;
        if (cVar2 != '\0') {
          uVar5 = FUN_0044fd30(1);
          FUN_0044c490((int)in_ECX[0x13],uVar5);
          uVar4 = extraout_var_03;
        }
        in_ECX[0x18] = 4;
        *(undefined1 *)(in_ECX + 0x19) = 0;
        iVar3 = DAT_00eaac28;
      }
      if (*(char *)((int)in_ECX + 0x33) != '\0') {
        return;
      }
      sVar1 = in_ECX[2];
      if (sVar1 != 0x24) {
        if ((int)(&DAT_00ea6160)[iVar3 * 0x1276 + (int)sVar1] < in_ECX[6] * 100) {
          return;
        }
        local_14 = in_ECX[0x17];
        *(undefined1 *)((int)in_ECX + 0x33) = 1;
        DAT_00e950dc = 1;
        DAT_006057e4 = (int)*in_ECX;
        local_1c = 0xffff;
        local_1e = 0xffff;
        local_20 = 0xffff;
        FUN_004e0cf0(0x13a,(int)*in_ECX,0,&local_20,
                     CONCAT31((int3)((uint)(in_ECX[6] * 100) >> 8),(char)in_ECX[0x30] == '\x06'));
        return;
      }
      if (*(int *)(&DAT_00ea187c + iVar3 * 0x49d8) < (int)in_ECX[6]) {
        return;
      }
      local_14 = in_ECX[0x17];
      *(undefined1 *)((int)in_ECX + 0x33) = 1;
      DAT_00e950dc = 1;
      local_1c = 0xffff;
      local_1e = 0xffff;
      local_20 = 0xffff;
      FUN_004e0cf0(0x135,(int)*in_ECX,0,&local_20,
                   CONCAT31((int3)(CONCAT22(uVar4,sVar1) >> 8),(char)in_ECX[0x30] == '\x06'));
      return;
    }
    if ((*(byte *)(in_ECX + 0x14) & 0x11) != 0) {
      return;
    }
    if (DAT_0078d7bc != (int)in_ECX[10] + (int)DAT_007848d8) {
      return;
    }
    if (DAT_0078d7c0 != *(byte *)((int)in_ECX + 3)) {
      return;
    }
    *(undefined1 *)(in_ECX + 0x19) = 1;
    if (in_ECX[2] == 0x24) {
      if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < (int)in_ECX[6]) goto LAB_0044d23e;
    }
    else if ((int)(&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + (int)in_ECX[2]] < in_ECX[6] * 100)
    goto LAB_0044d23e;
    *(undefined1 *)((int)in_ECX + 0x33) = 1;
LAB_0044d23e: ;
    in_ECX[0x17] = in_ECX[0x16];
    DAT_00e950dc = 1;
    if ((char)in_ECX[0x2b] == '\0') {
      switch((char)in_ECX[0x30]) {
      case '\0': ;
        local_20 = 0x27;
        break;
      case '\x03': ;
        local_20 = 0x39;
        uVar5 = 0x113;
        break;
      case '\x04': ;
        local_20 = 0x4b;
        uVar5 = 0x114;
        break;
      case '\x05': ;
        local_20 = 0x5d;
        uVar5 = 0x14a;
        break;
      case '\x06': ;
        local_20 = 0x6f;
      }
    }
    else {
      switch((char)in_ECX[0x30]) {
      case '\0': ;
        local_20 = 0x26;
        break;
      case '\x03': ;
        local_20 = 0x38;
        uVar5 = 0x113;
        break;
      case '\x04': ;
        local_20 = 0x4a;
        uVar5 = 0x114;
        break;
      case '\x05': ;
        local_20 = 0x5c;
        uVar5 = 0x14a;
        break;
      case '\x06': ;
        local_20 = 0x6e;
      }
    }
    FUN_004e0cf0(uVar5,(int)*in_ECX,0,&local_20,0);
    if (DAT_007027c0 != '\0') {
      return;
    }
    if (DAT_00e38ea8 != DAT_00579590) {
      return;
    }
    if ((char)in_ECX[1] != '\x01') {
      return;
    }
    if (in_ECX[2] != 0xd) {
      return;
    }
    if (DAT_00e38ebc != 0) {
      return;
    }
    FUN_00417ae0(8);
    return;
  }
  in_ECX[0x17] = in_ECX[0x17] + -1;
  if (0 < in_ECX[0x17]) {
    return;
  }
  DAT_00e950dc = 1;
  if (in_ECX[0x18] == 2) {
    cVar2 = FUN_004496d0((int)in_ECX[0x12]);
    if (cVar2 == '\0') goto LAB_0044d3ba;
    uVar5 = FUN_0044fd30(0);
    sVar1 = in_ECX[0x12];
  }
  else {
    cVar2 = FUN_004496d0((int)in_ECX[0x29]);
    if (cVar2 == '\0') goto LAB_0044d3ba;
    uVar5 = FUN_0044fd30(2);
    sVar1 = in_ECX[0x29];
  }
  FUN_0044c490((int)sVar1,uVar5);
LAB_0044d3ba: ;
  in_ECX[0x18] = 8;
  *(undefined1 *)(in_ECX + 0x19) = 0;
  *(uint *)(in_ECX + 0x14) = *(uint *)(in_ECX + 0x14) | 4;
  return;
}
