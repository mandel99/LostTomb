#include "restore_types.h"
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f2 (*(undefined4 *)0x00bcb9f2u)
#define DAT_00bcba46 (*(undefined4 *)0x00bcba46u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();

void FUN_00510b00(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  
  param_1 = param_1 * 0x90;
  if ((&DAT_00bcb9f0)[param_1] == '\0') {
    return;
  }
  iVar3 = 0;
  do {
    uVar2 = 0;
    if (DAT_007056c8 == 0) {
      sVar1 = *(short *)(&DAT_00bcb9f2 + param_1);
      if ((((((sVar1 == 2) || (sVar1 == 1)) && (iVar3 == 0)) ||
           (((sVar1 == 3 || (sVar1 == 4)) && (iVar3 == 1)))) ||
          (((sVar1 == 0 && (iVar3 == 1)) && (*(short *)(&DAT_00bcba46 + param_1) == 0)))) ||
         (((sVar1 == 0xf && (iVar3 == 2)) || ((sVar1 == 6 && (iVar3 == 3)))))) goto LAB_00510b7f;
    }
    else if (iVar3 == DAT_007056c8 + -1) {
LAB_00510b7f: ;
      uVar2 = 1;
    }
    FUN_004d63d0(0,iVar3 * 0x55 + 0x13 + DAT_00eadd90,DAT_00eadd8c + 0x8b,0x54,0x54,uVar2,
                 DAT_00eadd90,DAT_00eadd8c);
    iVar3 = iVar3 + 1;
  } while (iVar3 < 5);
  FUN_004d65b0(DAT_00eadd90 + 0x10,DAT_00eadd8c + 0xe6,DAT_00eadd88 + -2,5,DAT_00eadd90,DAT_00eadd8c
              );
  if (DAT_007056c8 == 1) {
LAB_00510caf: ;
    uVar2 = 0xc;
    uVar4 = 0xd;
  }
  else {
    if (DAT_007056c8 == 2) {
      if (*(short *)(&DAT_00bcb9cc + param_1) == 0xd) {
        uVar2 = 0x14;
        uVar4 = 0x15;
        goto LAB_00510cb9;
      }
LAB_00510c16: ;
      uVar2 = 0xe;
      uVar4 = 0xf;
      goto LAB_00510cb9;
    }
    if (DAT_007056c8 == 3) {
      uVar2 = 0x10;
      uVar4 = 0x11;
      goto LAB_00510cb9;
    }
    if (DAT_007056c8 == 4) {
      uVar2 = 0x12;
      uVar4 = 0x13;
      goto LAB_00510cb9;
    }
    if (DAT_007056c8 != 5) {
      sVar1 = *(short *)(&DAT_00bcb9f2 + param_1);
      if ((sVar1 == 2) || (sVar1 == 1)) goto LAB_00510caf;
      if ((sVar1 == 3) || (sVar1 == 4)) goto LAB_00510c16;
      if (sVar1 == 0) {
        uVar2 = 0x14;
        uVar4 = 0x15;
        goto LAB_00510cb9;
      }
      if (sVar1 == 0xf) {
        uVar2 = 0x10;
        uVar4 = 0x11;
        goto LAB_00510cb9;
      }
      if (sVar1 == 6) {
        uVar2 = 0x12;
        uVar4 = 0x13;
        goto LAB_00510cb9;
      }
    }
    uVar2 = 0x3a;
    uVar4 = 0x16;
  }
LAB_00510cb9: ;
  FUN_004d62f0(0x8a,uVar2,DAT_00eadd90 + 0x18,DAT_00eadd8c + 0xec,DAT_00f56f9c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6160(0x8a,uVar4,DAT_00eadd90 + 0x18,DAT_00eadd8c + 0xfc,(DAT_00eadd88 + -4) * 0x10,
               DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004d63d0(0,((DAT_00eadd88 + -0x14) * 0x10) / 2 + DAT_00eadd90,
               DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,0x140,0x1e,DAT_007056c4 == 1,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6250(0x8a,0x4d,((DAT_00eadd88 + -0x14) * 0x10) / 2 + DAT_00eadd90,
               (DAT_00eadd84 + -2) * 0x10 + DAT_00eadd8c,0x140,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  return;
}
