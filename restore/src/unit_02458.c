#include "restore_types.h"
#include "monument_rating.h"
#define LAB_00537470 (*(undefined1 *)0x00537470u)
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005c6f08 (*(undefined4 *)0x005c6f08u)
#define DAT_005c6f5c (*(undefined4 *)0x005c6f5cu)
#define DAT_005c7100 (*(undefined4 *)0x005c7100u)
#define DAT_005d19c8 (*(undefined4 *)0x005d19c8u)
#define DAT_005d19ca (*(undefined4 *)0x005d19cau)
#define DAT_005d19cc (*(undefined4 *)0x005d19ccu)
#define DAT_005d19d0 (*(undefined4 *)0x005d19d0u)
#define DAT_005d19d4 (*(undefined4 *)0x005d19d4u)
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00784c2c (*(int32_t *)0x00784c2cu)
#define DAT_00784c3e (*(undefined1 *)0x00784c3eu)
#define DAT_00784cbc (*(byte *)0x00784cbcu)
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define DAT_00784de8 (*(undefined4 *)0x00784de8u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern undefined4 FUN_0040c8b0();
extern undefined2 FUN_0040cb40();
extern undefined4 FUN_00442870();
extern void FUN_004cce90();
extern void FUN_004cd1a0();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6380();
extern void FUN_004d63d0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();
extern void FUN_00562069();

void FUN_005374a0(void)

{
  short sVar1;
  bool bVar2;
  int iVar3;
  undefined4 uVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  short *psVar8;
  undefined *puVar9;
  undefined4 *puVar10;
  int local_8;
  undefined *local_4;
  
  iVar3 = DAT_00e38da8;
  iVar7 = DAT_00e38da4;
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1c);
  FUN_004d6250(0xd,3,iVar3,iVar7 + 0x1a6,0x260,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  psVar8 = &DAT_00784de0;
  do {
    cVar5 = FUN_0040c8b0((int)*psVar8);
    if (cVar5 == '\0') {
LAB_0053754a: ;
      *psVar8 = 0;
    }
    else {
      iVar6 = (int)*psVar8;
      cVar5 = FUN_0040cb40(DAT_00784cbc & 0xff,*(undefined4 *)(&DAT_005d19c8 + iVar6 * 8),
                           *(undefined4 *)(&DAT_005d19cc + iVar6 * 8),
                           *(undefined4 *)(&DAT_005d19d0 + iVar6 * 0x10),
                           *(undefined4 *)(&DAT_005d19d4 + iVar6 * 0x10));
      if (cVar5 == '\0') goto LAB_0053754a;
    }
    psVar8 = psVar8 + 1;
  } while ((int)psVar8 < 0x784de6);
  FUN_00562069(&DAT_00784de0,3,2,&LAB_00537470);
  iVar6 = 0;
  psVar8 = &DAT_00784de0;
  puVar9 = &DAT_005c6f08;
  do {
    uVar4 = DAT_00f56f8c;
    iVar6 = iVar6 + 1;
    FUN_004cd1a0(1,puVar9,DAT_007056c8 == iVar6,0,0);
    FUN_004d6380(0xc6,(int)*psVar8,puVar9,uVar4,0,4,4,DAT_00e38da8,DAT_00e38da4);
    puVar9 = puVar9 + 0x1c;
    psVar8 = psVar8 + 1;
  } while ((int)puVar9 < 0x5c6f5c);
  FUN_004d63d0(1,DAT_00e38da8 + 10,DAT_00e38da4 + 0xa6,0x10e,100,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(199,0xb,DAT_00e38da8 + 10,DAT_00e38da4 + 0xbe,0x10e,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  iVar6 = restore_monument_rating_int32(1);
  FUN_004cce90(iVar6,0x40,&DAT_007027d4,DAT_00e38da8 + 10,DAT_00e38da4 + 0xd6,0x10e,DAT_00f56fb4,0);
  DAT_00784c3e = iVar6 != 0;
  DAT_00784c2c = iVar6;
  FUN_004cd1a0(1,&DAT_005c7100,DAT_007056c8 == 0x13,0,0);
  FUN_004d6380(0x137,DAT_00784cbc & 0xff,&DAT_005c7100,DAT_00f56f8c,0,4,4,DAT_00e38da8,DAT_00e38da4)
  ;
  bVar2 = true;
  psVar8 = &DAT_00784de0;
  do {
    if (((&DAT_005d19c8)[*psVar8 * 8] == 1) ||
       (((&DAT_005d19c8)[*psVar8 * 8] == 2 &&
        (((((sVar1 = (&DAT_005d19ca)[*psVar8 * 8], sVar1 == 4 || (sVar1 == 8)) || (sVar1 == 10)) ||
          ((sVar1 == 0xb || (sVar1 == 0xc)))) || (sVar1 == 0xd)))))) {
      bVar2 = false;
    }
    psVar8 = psVar8 + 1;
  } while ((int)psVar8 < 0x784de6);
  if (bVar2) {
    puVar10 = &DAT_00784de8;
    for (iVar7 = 0x24; iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar10 = 0;
      puVar10 = puVar10 + 1;
    }
    return;
  }
  FUN_004d62f0(0x2c,0xd5,iVar3 + 0x122,iVar7 + 400,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar6 = 0;
  local_8 = 4;
  local_4 = &DAT_005c6f5c;
  iVar7 = iVar7 + 0x1d;
  do {
    switch((int)(iVar6)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      FUN_0053e590(DAT_00f571fc + iVar6,iVar3 + 0x15e,iVar7 + -4);
      FUN_0053e590(DAT_00f571fc + iVar6,iVar3 + 600,iVar7 + -4);
      FUN_004d62f0(0x17,iVar6,iVar3 + 0x1c2,iVar7,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004cd1a0(1,local_4,DAT_007056c8 == local_8,0,0);
      if ((iVar6 == 0x1c) || (DAT_00d3b198 == 10)) {
        cVar5 = FUN_00442870(iVar6);
        if (cVar5 == '\0') goto LAB_00537851;
LAB_005378bb: ;
        if (0xc80 < (int)(&DAT_00784de8)[iVar6]) {
          (&DAT_00784de8)[iVar6] = 0xc80;
        }
        FUN_004d6860((&DAT_00784de8)[iVar6],0x40,&DAT_0057c048,iVar3 + 0x181,iVar7,DAT_00f56f8c,0,
                     DAT_00e38da8,DAT_00e38da4);
      }
      else {
        cVar5 = FUN_00442870(iVar6);
        if (cVar5 != '\0') goto LAB_005378bb;
LAB_00537851: ;
        FUN_004d62f0(0x12,6,iVar3 + 0x181,iVar7,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
        (&DAT_00784de8)[iVar6] = 0;
      }
      local_8 = local_8 + 1;
      local_4 = local_4 + 0x1c;
      iVar7 = iVar7 + 0x19;
    }
    iVar6 = iVar6 + 1;
    if (0x23 < iVar6) {
      return;
    }
  } while( true );
}
