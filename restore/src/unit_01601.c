#include "restore_types.h"
#define DAT_00705c10 (*(undefined *)0x00705c10u)
#define DAT_00705df4 (*(undefined *)0x00705df4u)
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea5c0a (*(undefined4 *)0x00ea5c0au)
#define DAT_00ea5c14 (*(undefined4 *)0x00ea5c14u)
#define DAT_00ea5c50 (*(undefined4 *)0x00ea5c50u)
#define DAT_00ea5c54 (*(undefined4 *)0x00ea5c54u)
#define DAT_00ea5c58 (*(undefined4 *)0x00ea5c58u)
#define DAT_00ea5c5c (*(undefined4 *)0x00ea5c5cu)
#define DAT_00ea5d68 (*(undefined4 *)0x00ea5d68u)
#define DAT_00ea5d6c (*(undefined4 *)0x00ea5d6cu)
#define DAT_00ea5d70 (*(undefined4 *)0x00ea5d70u)
#define DAT_00ea5d98 (*(undefined4 *)0x00ea5d98u)
#define DAT_00ea5e3c (*(undefined4 *)0x00ea5e3cu)
#define DAT_00ea5e40 (*(undefined4 *)0x00ea5e40u)
#define DAT_00ea6024 (*(undefined4 *)0x00ea6024u)
#define DAT_00ea6086 (*(undefined4 *)0x00ea6086u)
#define DAT_00ea61f4 (*(undefined4 *)0x00ea61f4u)
#define DAT_00ea61f6 (*(undefined4 *)0x00ea61f6u)
#define DAT_00ea61f8 (*(undefined4 *)0x00ea61f8u)
#define DAT_00ea61fc (*(undefined4 *)0x00ea61fcu)
#define DAT_00ea6204 (*(undefined4 *)0x00ea6204u)
#define DAT_00ea6208 (*(undefined4 *)0x00ea6208u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_0042fcf0();
extern void FUN_004b4710();
extern void FUN_004bd5e0();
extern int FUN_004c3950();
extern undefined4 FUN_004c3ac0();
extern undefined4 FUN_004c3ed0();
extern void FUN_004c4020();
extern undefined4 FUN_004c41b0();
extern void FUN_004c42b0();
extern void FUN_004c45e0();
extern void FUN_004e0cf0();
extern void FUN_004e6eb0();
extern void FUN_004e6ed0();
extern void FUN_004e70b0();
extern void FUN_004e96c0();
extern void FUN_004ea000();
extern void FUN_004f3aa0();
extern undefined4 FUN_004f5340();
extern uint FUN_00562348();

void FUN_004c47a0(int param_1,undefined4 param_2,int param_3)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_10;
  
  if ((param_1 < 5) && ((&DAT_007848e2)[param_1] != 0)) {
    cVar1 = *(char *)((int)&DAT_00ea6086 + param_1 + DAT_00eaac28 * 0x49d8);
    iVar2 = DAT_00eaac28 * 0x49d8 + param_1;
    if (((cVar1 == '2') &&
        (((int)(&DAT_00ea5c50)[DAT_00eaac28 * 0x1276 + param_1] < 0xf && (param_3 == -1)))) ||
       (param_3 == 1)) {
      *(undefined1 *)((int)&DAT_00ea6086 + iVar2) = 0;
      DAT_00e950dc = 1;
      *(char *)((int)&DAT_00ea5c0a + iVar2) = *(char *)((int)&DAT_00ea5c0a + iVar2) + -0x1e;
      iVar2 = FUN_004f5340();
      if (iVar2 != 0) {
        uVar3 = FUN_00562348();
        uVar3 = uVar3 & 0x80000001;
        bVar4 = uVar3 == 0;
        if ((int)uVar3 < 0) {
          bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
        }
        if (bVar4) {
          local_1e = 0x251;
          local_20 = 0x252;
          switch((int)(param_1)) {
          case 0: ;
            local_10 = 0x254;
            break;
          case 1: ;
            local_10 = 0x255;
            break;
          case 2: ;
            local_10 = 0x256;
            break;
          case 3: ;
            local_10 = 599;
            break;
          case 4: ;
            local_10 = 600;
          }
          DAT_00e950dc = 1;
          FUN_004e0cf0(0x83,0,0,&local_20,0);
          iVar2 = FUN_004f5340();
          if (iVar2 != 2) {
            FUN_004f3aa0(1);
            return;
          }
          FUN_004f3aa0(2);
          return;
        }
      }
      switch((int)(param_1)) {
      case 0: ;
        uVar3 = FUN_00562348();
        uVar3 = uVar3 & 0x80000001;
        bVar4 = uVar3 == 0;
        if ((int)uVar3 < 0) {
          bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
        }
        if (!bVar4) {
          FUN_004e0cf0(0x154,0,0,0,0);
          iVar2 = FUN_00562348();
          FUN_004bd5e0(((iVar2 % 3) * 5 + 10) * 2);
          return;
        }
        FUN_004e0cf0(0x127,0,0,0,0);
        *(undefined4 *)(&DAT_00ea6204 + DAT_00eaac28 * 0x49d8) = 1;
        return;
      case 1: ;
        uVar3 = FUN_00562348();
        uVar3 = uVar3 & 0x80000001;
        bVar4 = uVar3 == 0;
        if ((int)uVar3 < 0) {
          bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
        }
        if (!bVar4) {
          FUN_004e0cf0(0x149,0,0,0,0);
          FUN_004e96c0(0xf);
          return;
        }
        FUN_004e0cf0(0x128,0,0,0,0);
        (&DAT_00ea5e3c)[DAT_00eaac28 * 0x1276] = 0xc;
        return;
      case 2: ;
        iVar2 = FUN_004c3950();
        if (iVar2 == 0) {
          FUN_004e0cf0(0x91,0,0,0,0);
          return;
        }
        FUN_004e0cf0(0x129,0,0,0,0);
        return;
      case 3: ;
        FUN_004e0cf0(0x12a,0,0,0,0);
        (&DAT_00ea5e40)[DAT_00eaac28 * 0x1276] = 10;
        return;
      case 4: ;
        FUN_004e0cf0(299,0,0,0,0);
        FUN_004c42b0();
        return;
      }
    }
    else if ((((cVar1 < '\x14') || (0xe < (int)(&DAT_00ea5c50)[DAT_00eaac28 * 0x1276 + param_1])) ||
             (param_3 != -1)) && (param_3 != 2)) {
      if ((((*(char *)((int)&DAT_00ea5c14 + iVar2) == '2') &&
           (3 < (int)(&DAT_00ea5c50)[DAT_00eaac28 * 0x1276 + param_1])) && (param_3 == -1)) ||
         (param_3 == 3)) {
        *(undefined1 *)((int)&DAT_00ea5c14 + iVar2) = 0;
        *(char *)((int)&DAT_00ea5c0a + iVar2) = *(char *)((int)&DAT_00ea5c0a + iVar2) + '\x1e';
        DAT_00e950dc = 1;
        switch((int)(param_1)) {
        case 0: ;
          uVar3 = FUN_00562348();
          uVar3 = uVar3 & 0x80000001;
          bVar4 = uVar3 == 0;
          if ((int)uVar3 < 0) {
            bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (!bVar4) {
            FUN_004e0cf0(0x93,0,0,0,0);
            FUN_004e6eb0();
            return;
          }
          FUN_004e0cf0(0x8c,0,0,0,0);
          iVar2 = FUN_00562348();
          FUN_004bd5e0((-2 - iVar2 % 3) * 10);
          return;
        case 1: ;
          iVar2 = FUN_00562348();
          if (iVar2 % 3 == 0) {
            FUN_004e0cf0(0x14c,0,0,0,0);
            *(undefined2 *)(&DAT_00ea61f6 + DAT_00eaac28 * 0x49d8) = 0xc;
            return;
          }
          iVar2 = FUN_00562348();
          if (iVar2 % 3 != 1) {
            FUN_004e0cf0(0x14d,0,0,0,0);
            *(undefined4 *)(&DAT_00ea6024 + DAT_00eaac28 * 0x49d8) = 0xc;
            return;
          }
          FUN_004e0cf0(0x118,0,0,0,0);
          FUN_004e96c0(0xfffffff1);
          return;
        case 2: ;
          uVar3 = FUN_00562348();
          uVar3 = uVar3 & 0x80000001;
          bVar4 = uVar3 == 0;
          if ((int)uVar3 < 0) {
            bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (!bVar4) {
            FUN_004e0cf0(0x94,0,0,0,0);
            FUN_004e70b0(6);
            return;
          }
          iVar2 = FUN_004c3ed0();
          if (iVar2 != 0) {
            FUN_004e0cf0(0x8e,0,0,0,0);
            return;
          }
LAB_004c4e37: ;
          FUN_004e0cf0(0x8d,0,0,0,0);
          return;
        case 3: ;
          uVar3 = FUN_00562348();
          uVar3 = uVar3 & 0x80000001;
          bVar4 = uVar3 == 0;
          if ((int)uVar3 < 0) {
            bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (!bVar4) {
            FUN_004e0cf0(0x95,0,0,0,0);
            FUN_004e6ed0();
            return;
          }
          FUN_004e0cf0(0x119,0,0,0,0);
          FUN_004b4710();
          return;
        case 4: ;
          FUN_004e0cf0(0x90,0,0,0,0);
          FUN_004c45e0();
          return;
        }
      }
      else if (((('\x13' < *(char *)((int)&DAT_00ea5c14 + iVar2)) &&
                (3 < (int)(&DAT_00ea5c50)[DAT_00eaac28 * 0x1276 + param_1])) && (param_3 == -1)) ||
              (param_3 == 4)) {
        *(undefined1 *)((int)&DAT_00ea5c14 + iVar2) = 0;
        DAT_00e950dc = 1;
        *(char *)((int)&DAT_00ea5c0a + iVar2) = *(char *)((int)&DAT_00ea5c0a + iVar2) + '\f';
        switch((int)(param_1)) {
        case 0: ;
          uVar3 = FUN_00562348();
          uVar3 = uVar3 & 0x80000001;
          bVar4 = uVar3 == 0;
          if ((int)uVar3 < 0) {
            bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (!bVar4) {
            FUN_004e0cf0(0x155,0,0,0,0);
            uVar3 = FUN_00562348();
            uVar3 = uVar3 & 0x80000003;
            if ((int)uVar3 < 0) {
              uVar3 = (uVar3 - 1 | 0xfffffffc) + 1;
            }
            FUN_004bd5e0((-1 - uVar3) * 5);
            return;
          }
          FUN_004e0cf0(0x122,0,0,0,0);
          *(undefined4 *)(&DAT_00ea6208 + DAT_00eaac28 * 0x49d8) = 1;
          return;
        case 1: ;
          uVar3 = FUN_00562348();
          uVar3 = uVar3 & 0x80000001;
          bVar4 = uVar3 == 0;
          if ((int)uVar3 < 0) {
            bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (!bVar4) {
            FUN_004e0cf0(0x123,0,0,0,0);
            FUN_004e96c0(0xfffffffb);
            return;
          }
          FUN_004e0cf0(0x14e,0,0,0,0);
          *(undefined2 *)(&DAT_00ea61f8 + DAT_00eaac28 * 0x49d8) = 0xc;
          return;
        case 2: ;
          if (DAT_00705df4 + DAT_00705c10 != 0) {
            FUN_004e0cf0(0x124,0,0,0,0);
            FUN_004c4020(1);
            return;
          }
          goto LAB_004c4e37;
        case 3: ;
          iVar2 = FUN_004c41b0();
          if (iVar2 == 0) {
            FUN_004e0cf0(0x8f,0,0,0,0);
            return;
          }
          FUN_004e0cf0(0x125,0,0,0,0);
          return;
        case 4: ;
          FUN_004e0cf0(0x126,0,0,0,0);
          FUN_0042fcf0(1);
        }
      }
    }
    else {
      *(undefined1 *)((int)&DAT_00ea6086 + iVar2) = 0;
      DAT_00e950dc = 1;
      *(char *)((int)&DAT_00ea5c0a + iVar2) = *(char *)((int)&DAT_00ea5c0a + iVar2) + -0xc;
      iVar2 = FUN_004f5340();
      if (iVar2 == 2) {
        uVar3 = FUN_00562348();
        uVar3 = uVar3 & 0x80000001;
        bVar4 = uVar3 == 0;
        if ((int)uVar3 < 0) {
          bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
        }
        if (bVar4) {
          local_1e = 0x251;
          local_20 = 0x253;
          switch((int)(param_1)) {
          case 0: ;
            local_10 = 0x254;
            break;
          case 1: ;
            local_10 = 0x255;
            break;
          case 2: ;
            local_10 = 0x256;
            break;
          case 3: ;
            local_10 = 599;
            break;
          case 4: ;
            local_10 = 600;
          }
          DAT_00e950dc = 1;
          FUN_004e0cf0(0x83,0,0,&local_20,0);
          FUN_004f3aa0(1);
          return;
        }
      }
      switch((int)(param_1)) {
      case 0: ;
        FUN_004e0cf0(0x141,0,0,0,0);
        uVar3 = FUN_00562348();
        uVar3 = uVar3 & 0x80000003;
        if ((int)uVar3 < 0) {
          uVar3 = (uVar3 - 1 | 0xfffffffc) + 1;
        }
        FUN_004bd5e0(uVar3 * 5 + 5);
        return;
      case 1: ;
        uVar3 = FUN_00562348();
        uVar3 = uVar3 & 0x80000001;
        bVar4 = uVar3 == 0;
        if ((int)uVar3 < 0) {
          bVar4 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
        }
        if (!bVar4) {
          FUN_004e0cf0(0x142,0,0,0,0);
          FUN_004e96c0(5);
          return;
        }
        FUN_004e0cf0(0x14b,0,0,0,0);
        *(undefined2 *)(&DAT_00ea61f4 + DAT_00eaac28 * 0x49d8) = 0xc;
        return;
      case 2: ;
        FUN_004e0cf0(0x143,0,0,0,0);
        FUN_004c3ac0();
        return;
      case 3: ;
        FUN_004e0cf0(0x144,0,0,0,0);
        *(undefined4 *)(&DAT_00ea61fc + DAT_00eaac28 * 0x49d8) = 10;
        return;
      case 4: ;
        FUN_004e0cf0(0x145,0,0,0,0);
        iVar2 = DAT_00eaac28;
        (&DAT_00ea5d68)[DAT_00eaac28 * 0x1276] = 4;
        (&DAT_00ea5d6c)[iVar2 * 0x1276] = 1;
        (&DAT_00ea5d98)[iVar2 * 0x1276] = 1;
        (&DAT_00ea5d70)[iVar2 * 0x1276] = 0;
        FUN_004ea000();
        iVar2 = DAT_00eaac28 * 0x49d8;
        *(undefined4 *)(&DAT_00ea5c54 + iVar2) = 0;
        *(undefined4 *)(&DAT_00ea5c58 + iVar2) = 0;
        *(undefined4 *)(&DAT_00ea5c5c + iVar2) = 0;
        return;
      }
    }
  }
  return;
}
