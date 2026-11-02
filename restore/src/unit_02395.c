#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_0070573c (*(undefined4 *)0x0070573cu)
#define DAT_0078d5f0 (*(undefined *)0x0078d5f0u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5b7c (*(undefined4 *)0x00ea5b7cu)
#define DAT_00ea5bd0 (*(undefined4 *)0x00ea5bd0u)
#define DAT_00ea5bd8 (*(undefined4 *)0x00ea5bd8u)
#define DAT_00ea5be0 (*(undefined4 *)0x00ea5be0u)
#define DAT_00ea5be8 (*(undefined4 *)0x00ea5be8u)
#define DAT_00ea5bf0 (*(undefined4 *)0x00ea5bf0u)
#define DAT_00ea5bf4 (*(undefined4 *)0x00ea5bf4u)
#define DAT_00ea5bf8 (*(undefined4 *)0x00ea5bf8u)
#define DAT_00ea5d38 (*(undefined4 *)0x00ea5d38u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern void FUN_004550e0();
extern void FUN_004cce90();
extern void FUN_004cd680();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_004e9890();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00528950(void)

{
  int iVar1;
  undefined4 uVar2;
  char cVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_4;
  
  FUN_004e9890(0);
  FUN_004550e0();
  iVar1 = 5 - DAT_00f53f48;
  DAT_00f53f0c = DAT_00e38da8;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x13;
  if (iVar1 < 5) {
    if (0 < iVar1) {
      FUN_004cd680(iVar1 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar1 * 8,(iVar1 * -2 + 0x28) * 0x10,
                   (iVar1 * -2 + 0x13) * 0x10,0);
      return;
    }
    FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x13);
    FUN_0053e590(DAT_00f5681c + 8,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    FUN_004d62f0(0x3a,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x3a,1,DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x2a,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x3a,0x37,DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x3e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x3a,2,DAT_00f53f0c + 0x104,DAT_00f53f08 + 0x3e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x3a,3,DAT_00f53f0c + 0x154,DAT_00f53f08 + 0x3e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x3a,4,DAT_00f53f0c + 0x1e0,DAT_00f53f08 + 0x3e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x4c,0x24,8,DAT_00e38da8,DAT_00e38da4);
    local_4 = 0;
    do {
      switch((int)(local_4)) {
      case 0: ;
        iVar1 = 0;
        iVar7 = 0x1f;
        uVar2 = 0x2f;
        uVar4 = (&DAT_00ea5bd0)[DAT_00eaac28 * 0x1276];
        iVar5 = 400;
        break;
      case 1: ;
        iVar7 = 0x1e;
        uVar2 = 0x30;
        iVar1 = 1;
        uVar4 = (&DAT_00ea5bd8)[DAT_00eaac28 * 0x1276];
        iVar5 = 700;
        break;
      case 2: ;
        iVar7 = 0x21;
        uVar2 = 0x31;
        iVar1 = 2;
        uVar4 = (&DAT_00ea5be0)[DAT_00eaac28 * 0x1276];
        iVar5 = 0x4b0;
        break;
      case 3: ;
        iVar7 = 0x20;
        uVar2 = 0x32;
        iVar1 = 3;
        uVar4 = (&DAT_00ea5be8)[DAT_00eaac28 * 0x1276];
        iVar5 = 5000;
        break;
default: ;
        iVar7 = 0xe2;
        uVar2 = 0x38;
        iVar1 = 4;
        uVar4 = (&DAT_00ea5d38)[DAT_00eaac28 * 0x1276];
        iVar5 = 0x1d4c;
      }
      iVar6 = local_4 * 0x18;
      FUN_004d62f0(0x3a,uVar2,DAT_00f53f0c + 0x28,iVar6 + 0x50 + DAT_00f53f08,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004cce90((&DAT_0070573c)[iVar7],0x40,&DAT_0057c048,DAT_00f53f0c + 0x96,
                   iVar6 + 0x50 + DAT_00f53f08,100,DAT_00f56f9c,0);
      FUN_004cce90(uVar4,0x40,&DAT_0057c048,DAT_00f53f0c + 0xe6,iVar6 + 0x50 + DAT_00f53f08,100,
                   DAT_00f56f9c,0);
      DAT_00e20778 = 0;
      FUN_004d6860((&DAT_0070573c)[iVar7] * iVar5,0x40,&DAT_0057c048,DAT_00f53f0c + 0x154,
                   iVar6 + 0x50 + DAT_00f53f08,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x3a,5,DAT_00e20778 + 0x154 + DAT_00f53f0c,iVar6 + 0x50 + DAT_00f53f08,
                   DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
      iVar1 = (&DAT_0078d5f0)[iVar1];
      if (iVar1 == 0) {
        iVar5 = iVar6 + 0x50 + DAT_00f53f08;
        iVar1 = 0x23;
      }
      else {
        iVar5 = iVar6 + 0x50 + DAT_00f53f08;
        if (iVar1 < 100) {
          iVar1 = iVar1 / 10 + 0x24;
        }
        else {
          iVar1 = 0x2e;
        }
      }
      FUN_004d6250(0x3a,iVar1,DAT_00f53f0c + 0x1d6,iVar5,100,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      local_4 = local_4 + 1;
    } while (local_4 < 5);
    if ((int)(&DAT_00ea5b7c)[DAT_00eaac28 * 0x1276] < (int)(&DAT_00ea5bf8)[DAT_00eaac28 * 0x1276]) {
      cVar3 = '\n';
    }
    else if ((&DAT_00ea5b7c)[DAT_00eaac28 * 0x1276] == 0) {
      cVar3 = ((&DAT_00ea5bf4)[DAT_00eaac28 * 0x1276] != 0) + '\a';
    }
    else {
      iVar1 = (&DAT_00ea5bf0)[DAT_00eaac28 * 0x1276];
      if (iVar1 == 0) {
        cVar3 = '\t';
      }
      else if (iVar1 == 1) {
        cVar3 = '\v';
      }
      else if (iVar1 == 2) {
        cVar3 = '\f';
      }
      else {
        cVar3 = (iVar1 != 3) + '\r';
      }
    }
    FUN_004d6160(0x3a,cVar3,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xdc,0x200,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
