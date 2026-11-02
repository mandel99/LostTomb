#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ee (*(undefined4 *)0x007999eeu)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a4c (*(undefined4 *)0x00799a4cu)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00799a75 (*(undefined4 *)0x00799a75u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea41f8 (*(undefined4 *)0x00ea41f8u)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00401540();
extern int FUN_0040d940();
extern int FUN_00417050();
extern void FUN_004dea50();
extern int FUN_004df040();
extern void FUN_004e0cf0();
extern int FUN_004e6710();
extern bool FUN_004f6020();
extern int FUN_00542ad0();
extern undefined4 FUN_00545740();
extern void FUN_00547a60();
extern undefined4 FUN_0054f470();
extern undefined4 FUN_00555e80();
extern undefined4 FUN_00558be0();

void FUN_0046f200(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  
  local_1c = 0xffff;
  local_20 = 0xffff;
  local_1e = 0xffff;
  iVar3 = 1;
  do {
    iVar4 = iVar3 * 0x108;
    if ((&DAT_007999d8)[iVar4] == '\x01') {
      switch((&DAT_007999e8)[iVar3 * 0x84]) {
      case 0xb7: ;
        cVar1 = FUN_004f6020(iVar3);
        if ((cVar1 != '\0') && ((&DAT_00799a58)[iVar4] == '\0')) {
          switch((int)(DAT_005e4ef4)) {
          case 1: ;
            local_1e = 0x22a;
            local_20 = 0x22b;
            (&DAT_00799a58)[iVar3 * 0x108] = 1;
            break;
          case 2: ;
            local_1e = 0x22c;
            local_20 = 0x22d;
            (&DAT_00799a58)[iVar3 * 0x108] = 1;
            break;
          case 3: ;
            local_1e = 0x228;
            local_20 = 0x229;
            (&DAT_00799a58)[iVar3 * 0x108] = 1;
            break;
          case 4: ;
            local_1e = 0x226;
            local_20 = 0x227;
            (&DAT_00799a58)[iVar3 * 0x108] = 1;
            break;
          case 5: ;
            local_1e = 0x22e;
            local_20 = 0x22f;
default: ;
            (&DAT_00799a58)[iVar3 * 0x108] = 1;
          }
          goto LAB_0046f714;
        }
        if (DAT_005e4ef4 == 5) {
          iVar3 = iVar3 + -1 +
                  *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar3 * 0x84) * 4);
        }
        else {
          iVar3 = iVar3 + -1 +
                  *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar3 * 0x84) * 4);
        }
        break;
      case 0xcf: ;
      case 0xd0: ;
        if ((*(char *)((int)&DAT_00799a2c + iVar4 + 1) == 'd') && ((&DAT_00799a58)[iVar4] == '\0'))
        {
          local_1e = 0x232;
          local_20 = 0x233;
          (&DAT_00799a58)[iVar3 * 0x108] = 1;
          goto LAB_0046f714;
        }
        break;
      case 0xd2: ;
        if ((0xf < *(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2)) &&
           ((&DAT_00ea41f8)[DAT_00eaac28 * 0x1276] == 0)) {
          local_1e = 0x230;
          local_20 = 0x231;
          (&DAT_00ea41f8)[DAT_00eaac28 * 0x1276] = 1;
          goto LAB_0046f714;
        }
        break;
      case 0xd5: ;
        iVar4 = FUN_00542ad0(iVar3,0);
        if ((99 < iVar4) && (*(char *)(&DAT_00799a40 + iVar3 * 0x84) == '\0')) {
          local_1e = 0x234;
          local_20 = 0x235;
          *(undefined1 *)(&DAT_00799a40 + iVar3 * 0x84) = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 99;
        break;
      case 0xde: ;
        iVar2 = FUN_004df040(iVar3,0);
        if ((99 < iVar2) && ((&DAT_00799a4c)[iVar4] == '\0')) {
          local_1e = 0x23e;
          local_20 = 0x23f;
          (&DAT_00799a4c)[iVar3 * 0x108] = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 0x73;
        break;
      case 0xe1: ;
        iVar4 = FUN_0040d940(iVar3,0);
        if ((99 < iVar4) && (*(char *)(&DAT_00799a40 + iVar3 * 0x84) == '\0')) {
          local_1e = 0x236;
          local_20 = 0x237;
          *(undefined1 *)(&DAT_00799a40 + iVar3 * 0x84) = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 0x18;
        break;
      case 0xe3: ;
        iVar4 = FUN_00417050(iVar3,0);
        if ((99 < iVar4) && (*(char *)(&DAT_00799a40 + iVar3 * 0x84) == '\0')) {
          local_1e = 0x23a;
          local_20 = 0x23b;
          *(undefined1 *)(&DAT_00799a40 + iVar3 * 0x84) = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 8;
        break;
      case 0xe4: ;
        iVar4 = FUN_004e6710(iVar3,0);
        if ((99 < iVar4) && (*(char *)(&DAT_00799a40 + iVar3 * 0x84) == '\0')) {
          local_1e = 0x23c;
          local_20 = 0x23d;
          *(undefined1 *)(&DAT_00799a40 + iVar3 * 0x84) = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 1;
        break;
      case 0xe5: ;
        iVar2 = FUN_00545740(iVar3,0);
        if (((99 < iVar2) && (-2 < *(short *)(&DAT_007999ee + iVar4))) &&
           ((&DAT_00799a75)[iVar4] == '\0')) {
          FUN_00547a60();
          FUN_004dea50();
          local_1e = 0x240;
          local_20 = 0x241;
LAB_0046f6f9: ;
          (&DAT_00799a75)[iVar3 * 0x108] = 1;
LAB_0046f714: ;
          DAT_00e950dc = 1;
          FUN_004e0cf0(0x92,0xffffffff,0,&local_20,0);
          return;
        }
        iVar3 = iVar3 + 0xa6;
        break;
      case 0xe6: ;
        iVar2 = (FUN_00401540(iVar3,0), restore_unresolved_return(0x00401540u));
        if ((99 < iVar2) && ((&DAT_00799a38)[iVar4] == '\0')) {
          local_1e = 0x238;
          local_20 = 0x239;
          (&DAT_00799a38)[iVar3 * 0x108] = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 0xf;
        break;
      case 0xea: ;
        iVar2 = FUN_00558be0(iVar3,0);
        if ((99 < iVar2) && ((&DAT_00799a75)[iVar4] == '\0')) {
          FUN_00547a60();
          FUN_004dea50();
          local_1e = 0x242;
          local_20 = 0x243;
          (&DAT_00799a75)[iVar3 * 0x108] = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 0x9d;
        break;
      case 0xeb: ;
        iVar2 = FUN_00555e80(iVar3,0);
        if ((99 < iVar2) && ((&DAT_00799a75)[iVar4] == '\0')) {
          FUN_00547a60();
          FUN_004dea50();
          local_1e = 0x244;
          local_20 = 0x245;
          (&DAT_00799a75)[iVar3 * 0x108] = 1;
          goto LAB_0046f714;
        }
        iVar3 = iVar3 + 0x1b4;
        break;
      case 0xec: ;
        iVar2 = FUN_0054f470(iVar3,0);
        if ((99 < iVar2) && ((&DAT_00799a75)[iVar4] == '\0')) {
          FUN_00547a60();
          FUN_004dea50();
          local_1e = 0x246;
          local_20 = 0x247;
          goto LAB_0046f6f9;
        }
        iVar3 = iVar3 + 0x200;
      }
    }
    iVar3 = iVar3 + 1;
    if (3999 < iVar3) {
      return;
    }
  } while( true );
}
