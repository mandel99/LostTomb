#include "restore_types.h"
#define DAT_00775900 (*(undefined *)0x00775900u)
#define DAT_007848fc (*(undefined *)0x007848fcu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_00446a30();
extern void FUN_0044c490();
extern bool FUN_0044d680();
extern uint FUN_0044d700();
extern int FUN_0044d750();
extern void FUN_0044e6f0();
extern uint FUN_0044ef60();
extern void FUN_0044ef90();
extern undefined4 FUN_0044fd30();
extern void FUN_004e0cf0();

void FUN_00451540(void)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  byte bVar5;
  short *in_ECX;
  undefined2 local_20;
  undefined2 local_1e;
  short local_10;
  
  if ((*(uint *)(in_ECX + 0x14) >> 2 & 1) != 0) {
    return;
  }
  switch(in_ECX[2]) {
  case 1: ;
    local_1e = 0xb6;
    local_10 = 0xbe;
    break;
  case 2: ;
    local_1e = 0xac;
    cVar2 = FUN_0044ef60();
    local_10 = (cVar2 != '\0') + 0xb4;
    break;
  case 3: ;
    local_1e = 0xa2;
    cVar2 = FUN_0044ef60();
    local_10 = (cVar2 != '\0') + 0xaa;
    break;
  case 4: ;
    local_1e = 0xff;
    local_10 = 0x107;
  }
  FUN_0044e6f0(&local_20);
  cVar2 = FUN_0044d700();
  if (cVar2 == '\0') {
    return;
  }
  cVar2 = FUN_0044ef60();
  if ((cVar2 != '\0') && (0xe < (int)(&DAT_00ea5b48)[DAT_00eaac28 * 0x1276])) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x78,0,0,0,0);
    FUN_0044ef90();
    return;
  }
  iVar3 = FUN_0044d750();
  if (iVar3 == 0) {
    bVar1 = false;
    switch(in_ECX[2]) {
    case 1: ;
      iVar3 = FUN_00446a30((int)DAT_007848fc,(int)in_ECX[6],(int)in_ECX[0xe],(int)in_ECX[0x1c],
                           (int)*in_ECX);
      if (iVar3 != 0) {
        DAT_00e950dc = 1;
        bVar1 = true;
        FUN_0044fd30(0);
        local_20 = 0xbc;
      }
      break;
    case 2: ;
      iVar3 = FUN_00446a30(0xc,(int)in_ECX[6],(int)in_ECX[0xe],(int)in_ECX[0x1c],(int)*in_ECX);
      if (iVar3 != 0) {
        DAT_00e950dc = 1;
        bVar1 = true;
        local_20 = 0xb2;
      }
      break;
    case 3: ;
      iVar3 = FUN_00446a30(0xc,(int)in_ECX[6],(int)in_ECX[0xe],(int)in_ECX[0x1c],(int)*in_ECX);
      if (iVar3 != 0) {
        DAT_00e950dc = 1;
        bVar1 = true;
        local_20 = 0xa8;
      }
      break;
    case 4: ;
      iVar3 = FUN_00446a30(0xd,(int)in_ECX[6],(int)in_ECX[0xe],(int)in_ECX[0x1c],(int)*in_ECX);
      if (iVar3 != 0) {
        DAT_00e950dc = 1;
        bVar1 = true;
        local_20 = 0x105;
      }
      break;
default: ;
      iVar3 = 0;
    }
    *(uint *)(in_ECX + 0x14) = *(uint *)(in_ECX + 0x14) | 4;
    if (bVar1) {
      DAT_00e950dc = 1;
      FUN_004e0cf0(0x85,(int)DAT_00775900,iVar3,&local_20,0);
    }
    uVar4 = FUN_0044fd30(0);
    FUN_0044c490((int)in_ECX[0x12],uVar4);
    return;
  }
  uVar4 = 0;
  switch((int)(iVar3)) {
  case 1: ;
    bVar5 = 0x10;
    switch(in_ECX[2]) {
    case 1: ;
      local_20 = 0xbb;
      uVar4 = 0x71;
      break;
    case 2: ;
      local_20 = 0xb1;
      uVar4 = 0x7f;
      break;
    case 3: ;
      local_20 = 0xa7;
      uVar4 = 0x7c;
      break;
    case 4: ;
      local_20 = 0x104;
      uVar4 = 0x71;
    }
    break;
default: ;
    cVar2 = FUN_0044d680(1);
    if (cVar2 == '\0') {
      return;
    }
    if (iVar3 < 2) {
      return;
    }
    bVar5 = 1;
    switch(in_ECX[2]) {
    case 1: ;
      local_20 = 0xb7;
      uVar4 = 0x77;
      break;
    case 2: ;
      local_20 = 0xad;
      uVar4 = 0x81;
      break;
    case 3: ;
      local_20 = 0xa3;
      bVar5 = 1;
LAB_0045196f: ;
      uVar4 = 0x7e;
      break;
    case 4: ;
      local_20 = 0x100;
      uVar4 = 0x77;
    }
    break;
  case 6: ;
    bVar5 = 8;
    switch(in_ECX[2]) {
    case 1: ;
      local_20 = 0xba;
      uVar4 = 0x71;
      break;
    case 2: ;
      local_20 = 0xb0;
      uVar4 = 0x7f;
      break;
    case 3: ;
      local_20 = 0xa6;
      uVar4 = 0x7c;
      break;
    case 4: ;
      local_20 = 0x103;
      uVar4 = 0x71;
    }
    break;
  case 0xc: ;
    bVar5 = 4;
    switch(in_ECX[2]) {
    case 1: ;
      local_20 = 0xb9;
      uVar4 = 0x76;
      break;
    case 2: ;
      local_20 = 0xaf;
      uVar4 = 0x80;
      break;
    case 3: ;
      local_20 = 0xa5;
      uVar4 = 0x7d;
      break;
    case 4: ;
      local_20 = 0x102;
      uVar4 = 0x76;
    }
    break;
  case 0x18: ;
    bVar5 = 2;
    switch(in_ECX[2]) {
    case 1: ;
      local_20 = 0xb8;
      uVar4 = 0x77;
      break;
    case 2: ;
      local_20 = 0xae;
      uVar4 = 0x81;
      break;
    case 3: ;
      local_20 = 0xa4;
      bVar5 = 2;
      goto LAB_0045196f;
    case 4: ;
      local_20 = 0x101;
      uVar4 = 0x77;
    }
  }
  if ((bVar5 & *(byte *)((int)in_ECX + 0x57)) == 0) {
    *(byte *)((int)in_ECX + 0x57) = *(byte *)((int)in_ECX + 0x57) | bVar5;
    cVar2 = FUN_0044d680(1);
    DAT_00e950dc = (uint)(cVar2 != '\0');
    FUN_004e0cf0(uVar4,0,0,&local_20,0);
  }
  return;
}
